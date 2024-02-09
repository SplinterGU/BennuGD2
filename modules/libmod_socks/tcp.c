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

#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#endif

#include "tcp.h"

#ifndef _WIN32
#define closesocket(s)  close(s)
#endif

/* --------------------------------------------------------------------------- */

int tcp_init() {
#ifdef _WIN32
    WORD wVersionRequested = MAKEWORD( 2, 2 );
    WSADATA wsaData;
    /* Initialize WinSock and check the version */
    WSAStartup( wVersionRequested, &wsaData );
    if ( wsaData.wVersion != wVersionRequested ) return -1;
#endif
    return 0;
}

/* --------------------------------------------------------------------------- */

void tcp_exit() {
#ifdef _WIN32
    WSACleanup();
#endif
}

/* --------------------------------------------------------------------------- */

SOCKET tcp_listen(char *sAddr, int nPort) {
    SOCKET listenSocket;
    struct addrinfo hints, *result, *rp;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;     // Allow IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // Socket type
    hints.ai_flags = AI_PASSIVE;     // For obtaining the local machine's address

    char strPort[6];  // Assuming port is a 16-bit number (0-65535)
    snprintf(strPort, sizeof(strPort), "%d", nPort);

    if (getaddrinfo(sAddr, strPort, &hints, &result) != 0) {
        printf("Error obtaining information about IP address or hostname\n");
        return -1;
    }

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        listenSocket = socket( rp->ai_family, rp->ai_socktype, rp->ai_protocol );
        if ( listenSocket == -1 ) {
            continue;
        }

        setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));

        // Configure other sockopt (KEEPALIVE, NODELAY, DONTROUTE)
        setsockopt(listenSocket, SOL_SOCKET, SO_KEEPALIVE, &(int){1}, sizeof(int));
        setsockopt(listenSocket, IPPROTO_TCP, TCP_NODELAY, &(int){1}, sizeof(int));
//        setsockopt(listenSocket, SOL_SOCKET, SO_DONTROUTE, &(int){1}, sizeof(int));

#if _WIN32
        ioctlsocket(listenSocket, FIONBIO, &(unsigned long){1});
#else
        ioctl(listenSocket, FIONBIO, &(unsigned long){1});
#endif

        if (!bind(listenSocket, rp->ai_addr, rp->ai_addrlen)) {
            break;  // Successful bind, exit the loop
        }
        closesocket(listenSocket);
    }

    freeaddrinfo(result);

    if (rp == NULL) return -1;

    if (listen(listenSocket, SOMAXCONN) == -1) {
        closesocket(listenSocket);
        return -1;
    }

    return listenSocket;
}

/* --------------------------------------------------------------------------- */

SOCKET tcp_accept( SOCKET socket )
{
    SOCKET remotesocket;
    struct sockaddr addr;
    socklen_t addrLen = sizeof( addr );

    remotesocket = accept( socket, ( struct sockaddr * ) &addr, &addrLen );
    if ( remotesocket == -1 ) return -1;

/*
    uint32_t LongIPAddr = addr.sin_addr.s_addr ;

    if (addr->sa_family == AF_INET) {
        // IPv4
        struct sockaddr_in* ipv4 = (struct sockaddr_in*)addr;
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(ipv4->sin_addr), ip, INET_ADDRSTRLEN);
        printf("IPv4 Address: %s\n", ip);
    } else if (addr->sa_family == AF_INET6) {
        // IPv6
        struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)addr;
        char ip[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &(ipv6->sin6_addr), ip, INET6_ADDRSTRLEN);
        printf("IPv6 Address: %s\n", ip);
    } else {
        // Otro tipo de dirección (no se espera en un contexto de socket TCP/IP estándar)
        printf("Dirección de tipo desconocido\n");
    }
            
*/
    return remotesocket;
}

/* --------------------------------------------------------------------------- */

SOCKET tcp_connect( char *sHost, int nPort, int nTimeout )
{
    SOCKET clientSocket;
    struct addrinfo hints, *result, *rp;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;     // Allow IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // Socket type
    hints.ai_flags = AI_PASSIVE;     // For obtaining the local machine's address

    char strPort[6];  // Assuming port is a 16-bit number (0-65535)
    snprintf(strPort, sizeof(strPort), "%d", nPort);

    if (getaddrinfo(sHost, strPort, &hints, &result) != 0) {
        printf("Error obtaining information about IP address or hostname\n");
        return -1;
    }

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        clientSocket = socket( rp->ai_family, rp->ai_socktype, rp->ai_protocol );
        if ( clientSocket == -1 ) {
            continue;
        }

        setsockopt(clientSocket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));

        // Configure other sockopt (KEEPALIVE, NODELAY, DONTROUTE)
        setsockopt(clientSocket, SOL_SOCKET, SO_KEEPALIVE, &(int){1}, sizeof(int));
        setsockopt(clientSocket, IPPROTO_TCP, TCP_NODELAY, &(int){1}, sizeof(int));
//        setsockopt(clientSocket, SOL_SOCKET, SO_DONTROUTE, &(int){1}, sizeof(int));

        if (connect(clientSocket, rp->ai_addr, rp->ai_addrlen) != -1) {
#if _WIN32
            ioctlsocket(clientSocket, FIONBIO, &(unsigned long){1});
#else
            ioctl(clientSocket, FIONBIO, &(unsigned long){1});
#endif
            break;  // Successful bind, exit the loop
        }
        closesocket(clientSocket);
    }

    freeaddrinfo(result);

    if (rp == NULL) return -1;

    return clientSocket;

}

/* --------------------------------------------------------------------------- */

int tcp_send( SOCKET socket, char * sBuf, int nLen ) {
    return send( socket, sBuf, nLen, 0 );
}

/* --------------------------------------------------------------------------- */

#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL    0
#endif

int tcp_recv( SOCKET socket, char * sBuf, int nLen ) {
    int nRecv;
    if ( ( nRecv = recv( socket, sBuf, nLen, MSG_NOSIGNAL ) ) == -1 ) {
#ifdef _WIN32
        if ( WSAGetLastError() == WSAEWOULDBLOCK ) return -1;

        /*
                lastErr = WSAGetLastError();
                printf ("recv=%d WSAGetLastError()=%d\n", nRecv, lastErr);
                if (lastErr >= WSABASEERR)
                {
                    switch (lastErr)
                    {
                        case    WSAECONNRESET   :
                        case    WSAECONNABORTED :
                        case    WSAESHUTDOWN    :
                                return 0;

                    }
                    return -1;
                }
        */
#endif
    }
    return nRecv;
}

/* --------------------------------------------------------------------------- */

int tcp_wait( SOCKET *sockets, int nCount, int nTimeout, SOCKET *events ) {
    fd_set set;
    struct timeval timeout;
    int nSel;
    int i, n;

    if ( nCount > FD_SETSIZE ) nCount = FD_SETSIZE;

    /* Initialize the file descriptor set. */
    FD_ZERO( &set );

    for ( i = 0; i < nCount; i++ ) if ( sockets[i] ) FD_SET( sockets[i], &set );

    if ( nTimeout != -1 ) {
        /* Initialize the timeout data structure. */
        timeout.tv_sec = nTimeout / 1000;
        timeout.tv_usec = ( nTimeout % 1000 ) * 1000;
    }

    nSel = select( FD_SETSIZE, &set, NULL, NULL, ( nTimeout != -1 ) ? &timeout : NULL );
    if ( nSel > 0 ) {
        n = 0;
        for ( i = 0; i < nCount; i++ ) {
            if ( FD_ISSET( sockets[i], &set ) ) {
                events[n++] = sockets[i];
                FD_CLR( i, &set );
                if ( n == nSel ) break;
            }
        }
    }
    return nSel;
}

/* --------------------------------------------------------------------------- */

void tcp_close( SOCKET socket ) {
    closesocket( socket );
}

/* --------------------------------------------------------------------------- */

int tcp_getavailablebytes( SOCKET socket ) {
#ifdef _WIN32
    long unsigned int availableBytes;
    ioctlsocket(socket, FIONREAD, &availableBytes);
#else
    int availableBytes;
    ioctl(socket, FIONREAD, &availableBytes);
#endif
    return availableBytes;
}

/* --------------------------------------------------------------------------- */

int tcp_getReceiveBufferSize( SOCKET socket ) {
    int bufferSize;
#ifdef _WIN32
    int size = sizeof(bufferSize);
    if (getsockopt(socket, SOL_SOCKET, SO_RCVBUF, (char*)&bufferSize, &size) == SOCKET_ERROR)
#else
    socklen_t size = sizeof(bufferSize);
    if (getsockopt(socket, SOL_SOCKET, SO_RCVBUF, &bufferSize, &size) < 0)
#endif
    {
        return -1;
    }
    return bufferSize;
}

/* --------------------------------------------------------------------------- */

#if 0
int tcp_getremoteaddr( SOCKET socket, int type ) {
    // Obtén la dirección remota
    struct sockaddr_storage remoteAddr;
    socklen_t remoteAddrLen = sizeof(remoteAddr);
    getpeername(socket, (struct sockaddr*)&remoteAddr, &remoteAddrLen);

    if (remoteAddr.ss_family == AF_INET) {
        // IPv4
        struct sockaddr_in* ipv4 = (struct sockaddr_in*)&remoteAddr;
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(ipv4->sin_addr), ip, INET_ADDRSTRLEN);
        printf("IPv4 Address: %s\n", ip);
    } else if (remoteAddr.ss_family == AF_INET6) {
        // IPv6
        struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)&remoteAddr;
        char ip[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &(ipv6->sin6_addr), ip, INET6_ADDRSTRLEN);
        printf("IPv6 Address: %s\n", ip);
    } else {
        // Otro tipo de dirección (no se espera en un contexto de socket TCP/IP estándar)
        printf("Dirección de tipo desconocido\n");
    }

    return 0;
}
#endif
