
/*
 * PS3 networking backend for BennuGD.
 * Uses the IPv4 socket API provided by PSL1GHT.
 *
 * Keep net.h declarations unchanged.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <net/net.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

/* ------------------------------------------------------------------------- */

static int ps3_resolve_ipv4(const char *host, struct in_addr *address)
{
    struct hostent *entry;

    if (!address)
        return -1;

    if (!host || !host[0]) {
        address->s_addr = INADDR_ANY;
        return 0;
    }

    if (inet_pton(AF_INET, host, address) == 1)
        return 0;

    entry = gethostbyname(host);

    if (!entry ||
        entry->h_addrtype != AF_INET ||
        entry->h_length < (int)sizeof(address->s_addr) ||
        !entry->h_addr_list ||
        !entry->h_addr_list[0]) {
        fprintf(stderr, "PS3: cannot resolve IPv4 host: %s\n", host);
        return -1;
    }

    memcpy(&address->s_addr,
           entry->h_addr_list[0],
           sizeof(address->s_addr));

    return 0;
}

/* ------------------------------------------------------------------------- */

static void ps3_init_sockaddr(struct sockaddr_in *address,
                              const struct in_addr *ip,
                              int port)
{
    memset(address, 0, sizeof(*address));

    address->sin_len = sizeof(*address);
    address->sin_family = AF_INET;
    address->sin_port = htons((u16)port);
    address->sin_addr.s_addr = ip ? ip->s_addr : INADDR_ANY;
}

/* ------------------------------------------------------------------------- */

static int ps3_set_nonblocking(SOCKET sock)
{
    int enabled = 1;

    return setsockopt(sock, SOL_SOCKET, SO_NBIO,
                      &enabled, sizeof(enabled));
}

/* ------------------------------------------------------------------------- */

int net_init(void)
{
    int result = netInitialize();

    if (result != 0) {
        fprintf(stderr, "PS3: netInitialize failed: %d\n", result);
        return -1;
    }

    return 0;
}

/* ------------------------------------------------------------------------- */

void net_exit(void)
{
    netDeinitialize();
}

/* ------------------------------------------------------------------------- */

_net *net_open(enum net_mode mode,
               enum net_proto proto,
               char *addr,
               int port)
{
    _net *neth;
    struct in_addr ip;
    struct sockaddr_in local;
    int type;
    int protocol;

    if (port < 0 || port > 65535)
        return NULL;

    if (mode != NET_MODE_SERVER && mode != NET_MODE_CLIENT)
        return NULL;

    if (proto != NET_PROTO_TCP && proto != NET_PROTO_UDP)
        return NULL;

    if (mode == NET_MODE_CLIENT) {
        if (!addr || !addr[0] ||
            ps3_resolve_ipv4(addr, &ip) < 0) {
            return NULL;
        }
    } else {
        if (addr && addr[0]) {
            if (ps3_resolve_ipv4(addr, &ip) < 0)
                return NULL;
        } else {
            ip.s_addr = INADDR_ANY;
        }
    }

    neth = (_net *)calloc(1, sizeof(*neth));

    if (!neth) {
        perror("PS3: allocating network handle");
        return NULL;
    }

    neth->mode = mode;
    neth->proto = proto;
    neth->is_new_connection = 0;

    type = (proto == NET_PROTO_UDP) ? SOCK_DGRAM : SOCK_STREAM;
    protocol = (proto == NET_PROTO_UDP) ? IPPROTO_UDP : IPPROTO_TCP;

    neth->sock = socket(AF_INET, type, protocol);

    if (neth->sock < 0) {
        fprintf(stderr, "PS3: socket creation failed\n");
        free(neth);
        return NULL;
    }

    {
        int enabled = 1;

        /* Best effort: not every socket option is required. */
        setsockopt(neth->sock, SOL_SOCKET, SO_REUSEADDR,
                   &enabled, sizeof(enabled));

        if (proto == NET_PROTO_TCP) {
            setsockopt(neth->sock, SOL_SOCKET, SO_KEEPALIVE,
                       &enabled, sizeof(enabled));
        }
    }

    ps3_init_sockaddr(&local,
                      (mode == NET_MODE_SERVER) ? &ip : NULL,
                      port);

    if (mode == NET_MODE_SERVER) {
        if (bind(neth->sock,
                 (struct sockaddr *)&local,
                 sizeof(local)) < 0) {
            fprintf(stderr, "PS3: bind failed\n");
            netClose(neth->sock);
            free(neth);
            return NULL;
        }

        if (proto == NET_PROTO_TCP) {
            if (listen(neth->sock, 16) < 0) {
                fprintf(stderr, "PS3: listen failed\n");
                netClose(neth->sock);
                free(neth);
                return NULL;
            }
        }
    } else if (proto == NET_PROTO_TCP) {
        struct sockaddr_in remote;

        ps3_init_sockaddr(&remote, &ip, port);

        if (connect(neth->sock,
                    (struct sockaddr *)&remote,
                    sizeof(remote)) < 0) {
            fprintf(stderr, "PS3: connect failed\n");
            netClose(neth->sock);
            free(neth);
            return NULL;
        }
    }

    /*
     * UDP clients retain their destination address.
     * UDP servers receive their destination address from recvfrom().
     */
    if (proto == NET_PROTO_UDP && mode == NET_MODE_CLIENT) {
        ps3_init_sockaddr(&neth->srcaddr, &ip, port);
    } else {
        memset(&neth->srcaddr, 0, sizeof(neth->srcaddr));
        neth->srcaddr.sin_len = sizeof(neth->srcaddr);
        neth->srcaddr.sin_family = AF_INET;
    }

    neth->srcaddrlen = sizeof(neth->srcaddr);

    if (ps3_set_nonblocking(neth->sock) < 0) {
        fprintf(stderr, "PS3: cannot set nonblocking socket\n");
        netClose(neth->sock);
        free(neth);
        return NULL;
    }

    return neth;
}

/* ------------------------------------------------------------------------- */

int net_wait(List *socks, int timeout, List *events)
{
    fd_set read_set;
    struct timeval tv;
    struct timeval *tvp = NULL;
    int max_sock = -1;
    int result;
    void *ctx = NULL;
    _net *s;

    if (!socks)
        return -1;

    if (events)
        list_empty(events, NULL);

    FD_ZERO(&read_set);

    while ((s = (_net *)list_walk(socks, &ctx)) != NULL) {
        s->is_new_connection = 0;

        if (s->sock < 0 || s->sock >= FD_SETSIZE) {
            fprintf(stderr, "PS3: socket exceeds FD_SETSIZE\n");
            return -1;
        }

        FD_SET(s->sock, &read_set);

        if (s->sock > max_sock)
            max_sock = s->sock;
    }

    if (max_sock < 0)
        return 0;

    if (timeout >= 0) {
        tv.tv_sec = timeout / 1000;
        tv.tv_usec = (timeout % 1000) * 1000;
        tvp = &tv;
    }

    result = select(max_sock + 1, &read_set, NULL, NULL, tvp);

    if (result <= 0)
        return result;

    if (!events)
        return result;

    ctx = NULL;

    while ((s = (_net *)list_walk(socks, &ctx)) != NULL) {
        if (!FD_ISSET(s->sock, &read_set))
            continue;

        if (s->mode == NET_MODE_SERVER &&
            s->proto == NET_PROTO_TCP) {
            struct sockaddr_in remote;
            socklen_t remote_len = sizeof(remote);
            SOCKET client_sock;

            memset(&remote, 0, sizeof(remote));

            client_sock = accept(
                s->sock,
                (struct sockaddr *)&remote,
                &remote_len
            );

            if (client_sock < 0)
                continue;

            if (ps3_set_nonblocking(client_sock) < 0) {
                netClose(client_sock);
                continue;
            }

            {
                _net *client = (_net *)calloc(1, sizeof(*client));

                if (!client) {
                    netClose(client_sock);
                    continue;
                }

                client->mode = NET_MODE_CLIENT;
                client->proto = NET_PROTO_TCP;
                client->sock = client_sock;
                client->is_new_connection = 1;
                client->srcaddr = remote;
                client->srcaddrlen = remote_len;

                list_insertItem(events, client);
            }
        } else {
            list_insertItem(events, s);
        }
    }

    return result;
}

/* ------------------------------------------------------------------------- */

int net_send(_net *neth, const void *buf, size_t len)
{
    ssize_t sent;

    if (!neth || !buf)
        return -1;

    if (neth->proto == NET_PROTO_TCP) {
        sent = send(neth->sock, buf, len, 0);
    } else {
        if (neth->srcaddr.sin_addr.s_addr == INADDR_ANY)
            return -1;

        sent = sendto(
            neth->sock,
            buf,
            len,
            0,
            (struct sockaddr *)&neth->srcaddr,
            sizeof(neth->srcaddr)
        );
    }

    if (sent < 0)
        return -1;

    return (int)sent;
}

/* ------------------------------------------------------------------------- */

int net_recv(_net *neth, void *buf, size_t len)
{
    ssize_t received;

    if (!neth || !buf)
        return -1;

    if (neth->proto == NET_PROTO_TCP) {
        received = recv(neth->sock, buf, len, 0);
    } else {
        socklen_t from_len = sizeof(neth->srcaddr);

        received = recvfrom(
            neth->sock,
            buf,
            len,
            0,
            (struct sockaddr *)&neth->srcaddr,
            &from_len
        );

        if (received >= 0)
            neth->srcaddrlen = from_len;
    }

    if (received < 0)
        return -1;

    return (int)received;
}

/* ------------------------------------------------------------------------- */

void net_close(_net *neth)
{
    if (!neth)
        return;

    if (neth->sock >= 0)
        netClose(neth->sock);

    free(neth);
}

/* ------------------------------------------------------------------------- */

/*
 * PSL1GHT's public socket interface does not expose the Linux FIONREAD
 * ioctl contract here. Report unsupported instead of inventing a value.
 */
int net_getavailablebytes(_net *neth)
{
    (void)neth;
    return -1;
}

/* ------------------------------------------------------------------------- */

int net_getReceiveBufferSize(_net *neth)
{
    int buffer_size = 0;
    socklen_t size = sizeof(buffer_size);

    if (!neth)
        return -1;

    if (getsockopt(neth->sock, SOL_SOCKET, SO_RCVBUF,
                   &buffer_size, &size) < 0) {
        return -1;
    }

    return buffer_size;
}

/* ------------------------------------------------------------------------- */

char *net_getremoteaddr(_net *neth)
{
    struct sockaddr_in remote;
    socklen_t remote_len = sizeof(remote);
    struct in_addr ip;
    char *text;

    if (!neth)
        return NULL;

    if (neth->proto == NET_PROTO_TCP) {
        memset(&remote, 0, sizeof(remote));

        if (getpeername(neth->sock,
                        (struct sockaddr *)&remote,
                        &remote_len) < 0) {
            return NULL;
        }

        ip = remote.sin_addr;
    } else {
        /*
         * For UDP this is the configured destination for a client,
         * or the most recent sender recorded by recvfrom() for a server.
         */
        ip = neth->srcaddr.sin_addr;

        if (ip.s_addr == INADDR_ANY)
            return NULL;
    }

    text = (char *)malloc(INET_ADDRSTRLEN);

    if (!text)
        return NULL;

    if (!inet_ntop(AF_INET, &ip, text, INET_ADDRSTRLEN)) {
        free(text);
        return NULL;
    }

    return text;
}

/* ------------------------------------------------------------------------- */

int net_is_new_connection(_net *neth)
{
    if (!neth)
        return 0;

    return neth->is_new_connection;
}

/* ------------------------------------------------------------------------- */

int net_is_message_incoming(_net *neth)
{
    if (!neth)
        return 0;

    return !neth->is_new_connection;
}

/* ------------------------------------------------------------------------- */
