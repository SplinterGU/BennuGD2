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

#ifndef __NET_H
    #define __NET_H

    #include "list.h"

    #include <errno.h>

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    #ifdef __SWITCH__
        #include <switch.h>
    #endif

    #ifdef _WIN32
        #include <winsock2.h>
        #include <ws2tcpip.h>
        #pragma comment(lib, "ws2_32.lib")
    #else
        #include <arpa/inet.h>
        #include <netdb.h>
        #include <unistd.h>

        #include <netinet/tcp.h>

        #ifdef __SWITCH__
            #include <sys/socket.h>
        #else
            #include <sys/ipc.h>
            #include <sys/msg.h>
        #endif

        #include <sys/ioctl.h>  

        #define SOCKET_ERROR    -1

        typedef int SOCKET;
    #endif

    #ifndef MSG_NOSIGNAL
        #define MSG_NOSIGNAL    0
    #endif


    #define MAX_BUFFER_SIZE 1024

    enum net_mode {
        NET_MODE_SERVER,
        NET_MODE_CLIENT
    };

    enum net_proto {
        NET_PROTO_UDP,
        NET_PROTO_TCP
    };

    typedef struct {
        enum net_mode mode;
        enum net_proto proto;
        SOCKET sock;
        struct sockaddr_storage srcaddr;
        socklen_t srcaddrlen;
        int is_new_connection;
    } _net;

    #define NET_ERROR           -1
    #define NET_TIMEOUT         0
    #define NET_DISCONNECTED    0

    extern int net_init();
    extern void net_exit();
    extern _net *net_open(enum net_mode mode, enum net_proto proto, char *addr, int port);
    extern int net_wait(List *socks, int timeout, List *events);
    extern int net_send(_net *neth, const void *buf, size_t len);
    extern int net_recv(_net *neth, void *buf, size_t len);
    extern void net_close(_net *neth);
    extern int net_getavailablebytes(_net *neth);
    extern int net_getReceiveBufferSize(_net *neth);
    extern char * net_getremoteaddr(_net *neth);
    extern int net_is_new_connection(_net *neth);
    extern int net_is_message_incoming(_net *event);

#endif /* NET_H */
