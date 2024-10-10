/*
 *  Copyright (C) SplinterGU (Fenix/BennuGD) (Since 2006)
 *
 *  This file is part of Bennu Game Development
 *
 *  This software is provided 'as-is', without any express or implied
 *  warranty. In no event will the authors be held liable for any damages
 *  arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *     1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software
 *     in a product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 *
 *     2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 *
 *     3. This notice may not be removed or altered from any source
 *     distribution.
 *
 */

/* --------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#ifdef _WIN32
#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0600  // Define Windows Vista or higher
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#elif __SWITCH__
#include <switch.h>
#endif

#include "net.h"

#ifndef _WIN32
    #define closesocket(x)    close(x)
#endif

/* --------------------------------------------------------------------------- */

/** Initialize networking library.
 * This function initializes the networking library. On Windows, it initializes Winsock,
 * and on Switch, it initializes sockets.
 * \return 0 on success, -1 on failure.
 */
int net_init() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("WSAStartup failed");
        return -1;
    }
#elif __SWITCH__
    socketInitializeDefault();
#endif
    return 0;
}

/* --------------------------------------------------------------------------- */

/** Cleanup networking library.
 * This function cleans up the networking library. On Windows, it calls WSACleanup,
 * and on Switch, it exits the socket service.
 */
void net_exit() {
#ifdef _WIN32
    WSACleanup();
#elif __SWITCH__
    socketExit();
#endif
}

/* --------------------------------------------------------------------------- */

/** Open a network connection.
 * This function opens a network connection based on the specified mode, protocol, address, and port.
 * \param mode Mode of operation (server or client).
 * \param proto Protocol to use (TCP or UDP).
 * \param addr Address to bind/connect.
 * \param port Port to bind/connect.
 * \return Pointer to the newly opened network connection (_net structure), or NULL on failure.
 */
_net *net_open(enum net_mode mode, enum net_proto proto, char *addr, int port) {
#ifndef PS3_PPU
    _net *neth = malloc(sizeof(_net));
    if (!neth) {
        perror("Error allocating memory for _net");
        return NULL;
    }

    memset(neth, 0, sizeof(_net));
    neth->mode = mode;
    neth->proto = proto;

    struct addrinfo hints, *res;
    char port_str[6]; // Maximum number of characters for a port (including null terminator)

    snprintf(port_str, sizeof(port_str), "%d", port);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // IPv4 or IPv6
    hints.ai_socktype = (proto == NET_PROTO_UDP) ? SOCK_DGRAM : SOCK_STREAM;
    hints.ai_flags = (mode == NET_MODE_SERVER) ? AI_PASSIVE : 0; // For binding in server mode

    int status = getaddrinfo(addr, port_str, &hints, &res);
    if (status != 0) {
        fprintf(stderr, "Error obtaining address information: %s\n", gai_strerror(status));
        free(neth);
        return NULL;
    }

    // Iterate over all addresses and connect/bind to the first available
    struct addrinfo *p;
    for (p = res; p != NULL; p = p->ai_next) {
        neth->sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (neth->sock == -1) {
            perror("Error creating socket");
            continue;
        }

        setsockopt(neth->sock, SOL_SOCKET, SO_REUSEADDR, (const char *)&(int){1}, sizeof(int));

        if (proto == NET_PROTO_TCP) {
            // Configure other socket options (KEEPALIVE, NODELAY)
            setsockopt(neth->sock, SOL_SOCKET, SO_KEEPALIVE, (const char *)&(int){1}, sizeof(int));
            setsockopt(neth->sock, IPPROTO_TCP, TCP_NODELAY, (const char *)&(int){1}, sizeof(int));
//        setsockopt(neth->sock, SOL_SOCKET, SO_DONTROUTE, &(int){1}, sizeof(int));
        }

        if (mode == NET_MODE_SERVER) {
            if (bind(neth->sock, p->ai_addr, p->ai_addrlen) != 0) {
                closesocket(neth->sock);
                perror("Error in socket bind");
                continue;
            }

            if (proto == NET_PROTO_TCP && listen(neth->sock, SOMAXCONN) == -1) {
                closesocket(neth->sock);
                perror("Error in socket listen");
                continue;
            }
        } else if (proto == NET_PROTO_TCP) {
            if (connect(neth->sock, p->ai_addr, p->ai_addrlen) != 0) {
                closesocket(neth->sock);
                perror("Error connecting socket");
                continue;
            }
        }

#if _WIN32
        ioctlsocket(neth->sock, FIONBIO, &(unsigned long){1});
#else
        ioctl(neth->sock, FIONBIO, &(unsigned long){1});
#endif

        memcpy(&neth->srcaddr, p->ai_addr, p->ai_addrlen);
        neth->srcaddrlen = p->ai_addrlen;

        break; // Successful connection/binding
    }

    freeaddrinfo(res);

    if (p == NULL) {
        fprintf(stderr, "Error: could not connect/bind\n");
        free(neth);
        return NULL;
    }

    return neth;
#else
    return NULL;
#endif
}

/* --------------------------------------------------------------------------- */

/** Wait for events on multiple network connections.
 * This function waits for events on multiple network connections with optional timeout.
 * \param socks network connections List (_net structures).
 * \param timeout Timeout in milliseconds (-1 for infinite timeout).
 * \param events events List to store network connections with events.
 * \return Number of network connections with events, or -1 on error.
 */
int net_wait(List *socks, int timeout, List *events) {
#ifndef PS3_PPU
    fd_set set;
    struct timeval tv;
    int result, max_sock = 0;

    list_empty(events, NULL);

    FD_ZERO(&set);

    _net *s;
    void * ctx = NULL;
    while( ( s = list_walk( socks, &ctx ) ) ) {
        FD_SET(s->sock, &set);
        if (s->sock > max_sock) max_sock = s->sock;
        s->is_new_connection = 0;
    }

    if (max_sock >= FD_SETSIZE) max_sock = FD_SETSIZE - 2;

    if (timeout != -1) {
        tv.tv_sec = timeout / 1000;
        tv.tv_usec = (timeout % 1000) * 1000;
    }

    result = select(max_sock + 1, &set, NULL, NULL, (timeout != -1) ? &tv : NULL);
    if (result < 0) {
        perror("Error in select");
        return -1;
    } else if (result == 0) {
        return 0; // Timeout
    } else {
        if (events) {
            ctx = NULL;
            while( ( s = list_walk( socks, &ctx ) ) ) {
                if (FD_ISSET(s->sock, &set)) {
                    if (s->mode == NET_MODE_SERVER && s->proto == NET_PROTO_TCP) {
                        SOCKET client_sockfd = accept(s->sock, (struct sockaddr *)NULL, NULL);
                        if (client_sockfd != -1) {
                            _net *client = malloc(sizeof(_net));
                            if (!client) {
                                perror("Error allocating memory for client _net");
                                closesocket(client_sockfd);
                                continue;
                            }
                            client->mode = NET_MODE_CLIENT;
                            client->proto = NET_PROTO_TCP;
                            client->sock = client_sockfd;
                            client->is_new_connection = 1;
                            list_insertItem( events, client );
                        }
                    } else {
                        list_insertItem( events, s );
                    }
                    FD_CLR(s->sock, &set);
                }
            }
        }
        return result; // Number of _net with ready events
    }

    return result;
#else
    return -1;
#endif
}

/* --------------------------------------------------------------------------- */

/** Send data over a network connection.
 * This function sends data over the specified network connection.
 * \param neth Pointer to the network connection (_net structure).
 * \param buf Pointer to the data buffer.
 * \param len Length of the data to send.
 * \return Number of bytes sent on success, or -1 on error.
 */
int net_send(_net *neth, const void *buf, size_t len) {
#ifndef PS3_PPU
    int sent_bytes;
    if (neth->proto == NET_PROTO_TCP) {
        sent_bytes = send(neth->sock, buf, len, 0);
    } else {
        sent_bytes = sendto(neth->sock, buf, len, 0, (struct sockaddr *)&(neth->srcaddr), neth->srcaddrlen);
    }

    if (sent_bytes == -1) {
        perror("Error sending data");
        return -1;
    }
    return sent_bytes;
#else
    return -1;
#endif
}

/* --------------------------------------------------------------------------- */

/** Receive data from a network connection.
 * This function receives data from the specified network connection.
 * \param neth Pointer to the network connection (_net structure).
 * \param buf Pointer to the buffer to store received data.
 * \param len Maximum length of the buffer.
 * \return Number of bytes received on success, or -1 on error.
 */
int net_recv(_net *neth, void *buf, size_t len) {
#ifndef PS3_PPU
    int recv_bytes;
    if (neth->proto == NET_PROTO_TCP) {
        recv_bytes = recv(neth->sock, buf, len, 0);
    } else {
        recv_bytes = recvfrom(neth->sock, buf, len, 0, (struct sockaddr *)&(neth->srcaddr), (socklen_t *)&(neth->srcaddrlen));
    }

    if (recv_bytes == -1) {
        perror("Error receiving data");
        return -1;
    }
    return recv_bytes;
#else
    return -1;
#endif
}

/* --------------------------------------------------------------------------- */

/** Close a network connection.
 * This function closes the specified network connection and frees memory.
 * \param neth Pointer to the network connection (_net structure).
 */
void net_close(_net *neth) {
#ifndef PS3_PPU
    closesocket(neth->sock);
    free(neth);
#endif
}

/* --------------------------------------------------------------------------- */

/** Get the number of available bytes in the receive buffer of the network socket.
 *  This function retrieves the number of bytes available to read from the receive buffer of the network socket.
 *  \param neth Pointer to the network connection (_net structure).
 *  \return Number of available bytes in the receive buffer.
 */
int net_getavailablebytes(_net *neth) {
#ifndef PS3_PPU
#ifdef _WIN32
    long unsigned int availableBytes;
    ioctlsocket(neth->sock, FIONREAD, &availableBytes);
#else
    int availableBytes;
    ioctl(neth->sock, FIONREAD, &availableBytes);
#endif
    return availableBytes;
#else
    return 0;
#endif
}

/* --------------------------------------------------------------------------- */

/** Get the size of the receive buffer of the network socket.
 *  This function retrieves the size of the receive buffer of the network socket.
 *  \param neth Pointer to the network connection (_net structure).
 *  \return Size of the receive buffer.
 */
int net_getReceiveBufferSize(_net *neth) {
#ifndef PS3_PPU
    int bufferSize;
#ifdef _WIN32
    int size = sizeof(bufferSize);
    if (getsockopt(neth->sock, SOL_SOCKET, SO_RCVBUF, (char*)&bufferSize, &size) == SOCKET_ERROR)
#else
    socklen_t size = sizeof(bufferSize);
    if (getsockopt(neth->sock, SOL_SOCKET, SO_RCVBUF, &bufferSize, &size) < 0)
#endif
    {
        return -1;
    }
    return bufferSize;
#else
    return 0;
#endif
}

/* --------------------------------------------------------------------------- */

/** Get the remote IP address of the client connected to the network socket.
 *  This function retrieves the remote IP address of the client connected to the network socket.
 *  \param neth Pointer to the network connection (_net structure).
 *  \return Pointer to a string containing the remote IP address.
 */
char * net_getremoteaddr(_net *neth) {
#ifndef PS3_PPU
    struct sockaddr_storage remoteAddr;
    socklen_t remoteAddrLen = sizeof(remoteAddr);

    if (getpeername(neth->sock, (struct sockaddr*)&remoteAddr, &remoteAddrLen) == -1) return NULL;

    char *ip = NULL, *res = NULL;

    if (remoteAddr.ss_family == AF_INET) {
        // IPv4
        struct sockaddr_in *ipv4 = (struct sockaddr_in*)&remoteAddr;
        ip = malloc(INET_ADDRSTRLEN);
        if (!ip) return NULL;
        res = (char *)inet_ntop(AF_INET, &(ipv4->sin_addr), ip, INET_ADDRSTRLEN);

    } else if (remoteAddr.ss_family == AF_INET6) {
        // IPv6
        struct sockaddr_in6 *ipv6 = (struct sockaddr_in6*)&remoteAddr;
        ip = malloc(INET6_ADDRSTRLEN);
        if (!ip) return NULL;
        res = (char *)inet_ntop(AF_INET6, &(ipv6->sin6_addr), ip, INET6_ADDRSTRLEN);

    }

    if (!res) free(ip);

    return res;
#else
    return NULL;
#endif
}

/* --------------------------------------------------------------------------- */

/** Check if the event corresponds to a new client connection.
 *  This function checks if the given event represents a new client connection.
 *  \param event Pointer to the network event (_net structure).
 *  \return 1 if the event is a new client connection, 0 otherwise.
 */
int net_is_new_connection(_net *neth) {
#ifndef PS3_PPU
    if ( !neth ) return 0;
    return neth->is_new_connection;
#else
    return 0;
#endif
}

/* --------------------------------------------------------------------------- */

/** Check if the event corresponds to an incoming message.
 *  This function checks if the given event represents an incoming message.
 *  \param event Pointer to the network event (_net structure).
 *  \return 1 if the event is an incoming message, 0 otherwise.
 */
int net_is_message_incoming(_net *neth) {
#ifndef PS3_PPU
    if (!neth) return 0;
    return !neth->is_new_connection;
#else
    return 0;
#endif
}

/* --------------------------------------------------------------------------- */
