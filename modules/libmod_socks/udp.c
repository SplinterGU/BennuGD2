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

#include "udp.h"

#ifndef _WIN32
#define closesocket(s)  close(s)
#endif

int udp_init() {
#ifdef _WIN32
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;
    WSAStartup(wVersionRequested, &wsaData);
    if (wsaData.wVersion != wVersionRequested) return -1;
#endif
    return 0;
}

void udp_exit() {
#ifdef _WIN32
    WSACleanup();
#endif
}

SOCKET udp_bind(char *sAddr, int nPort) {
    SOCKET udpSocket;
    struct addrinfo hints, *result, *rp;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;     // Allow IPv4 or IPv6
    hints.ai_socktype = SOCK_DGRAM;  // Datagram socket type
    hints.ai_flags = AI_PASSIVE;     // For obtaining the local machine's address

    char strPort[6];  // Assuming port is a 16-bit number (0-65535)
    snprintf(strPort, sizeof(strPort), "%d", nPort);

    if (getaddrinfo(sAddr, strPort, &hints, &result) != 0) {
        printf("Error obtaining information about IP address or hostname\n");
        return -1;
    }

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        udpSocket = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (udpSocket == -1) {
            continue;
        }

        setsockopt(udpSocket, SOL_SOCKET, SO_REUSEADDR, ( const char * ) &(int){1}, sizeof(int));

        if (!bind(udpSocket, rp->ai_addr, rp->ai_addrlen)) {
            break;  // Successful bind, exit the loop
        }
        closesocket(udpSocket);
    }

    freeaddrinfo(result);

    if (rp == NULL) return -1;

    return udpSocket;
}

int udp_sendto(SOCKET socket, char *sBuf, int nLen, char *sDestAddr, int nDestPort) {
    struct sockaddr_storage destAddr;
    struct addrinfo *result;

    socklen_t addrLen = sizeof(destAddr);

    memset(&destAddr, 0, sizeof(destAddr));

    if (getaddrinfo(sDestAddr, NULL, NULL, &result) != 0) {
        printf("Error obtaining information about destination address\n");
        return -1;
    }

    destAddr.ss_family = result->ai_family;

    if (result->ai_family == AF_INET) {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)&destAddr;
        ipv4->sin_port = htons(nDestPort);
    } else if (result->ai_family == AF_INET6) {
        struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)&destAddr;
        ipv6->sin6_port = htons(nDestPort);
    } else {
        printf("Unsupported address family\n");
        return -1;
    }

    freeaddrinfo(result);

    return sendto(socket, sBuf, nLen, 0, (struct sockaddr *)&destAddr, addrLen);
}

int udp_recvfrom(SOCKET socket, char *sBuf, int nLen, char *sSrcAddr, int *nSrcPort) {
    struct sockaddr_storage srcAddr;
    socklen_t addrLen = sizeof(srcAddr);

    int nRecv = recvfrom(socket, sBuf, nLen, 0, (struct sockaddr *)&srcAddr, &addrLen);

    if (nRecv == -1) {
#ifdef _WIN32sendto
        if (WSAGetLastError() == WSAEWOULDBLOCK) return -1;
#endif
    } else {
        if (srcAddr.ss_family == AF_INET) {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)&srcAddr;
            inet_ntop(AF_INET, &(ipv4->sin_addr), sSrcAddr, INET_ADDRSTRLEN);
            *nSrcPort = ntohs(ipv4->sin_port);
        } else if (srcAddr.ss_family == AF_INET6) {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)&srcAddr;
            inet_ntop(AF_INET6, &(ipv6->sin6_addr), sSrcAddr, INET6_ADDRSTRLEN);
            *nSrcPort = ntohs(ipv6->sin6_port);
        } else {
            printf("Unsupported address family\n");
            return -1;
        }
    }

    return nRecv;
}

int udp_wait(SOCKET *sockets, int nCount, int nTimeout, SOCKET *events) {
    fd_set set;
    struct timeval timeout;
    int nSel;
    int i, n;

    if (nCount > FD_SETSIZE) nCount = FD_SETSIZE;

    FD_ZERO(&set);

    for (i = 0; i < nCount; i++) if (sockets[i]) FD_SET(sockets[i], &set);

    if (nTimeout != -1) {
        timeout.tv_sec = nTimeout / 1000;
        timeout.tv_usec = (nTimeout % 1000) * 1000;
    }

    nSel = select(FD_SETSIZE, &set, NULL, NULL, (nTimeout != -1) ? &timeout : NULL);
    if (nSel > 0) {
        n = 0;
        for (i = 0; i < nCount; i++) {
            if (FD_ISSET(sockets[i], &set)) {
                events[n++] = sockets[i];
                FD_CLR(i, &set);
                if (n == nSel) break;
            }
        }
    }
    return nSel;
}

void udp_close(SOCKET socket) {
    closesocket(socket);
}

int udp_getavailablebytes(SOCKET socket) {
#ifdef _WIN32
    long unsigned int availableBytes;
    ioctlsocket(socket, FIONREAD, &availableBytes);
#else
    int availableBytes;
    ioctl(socket, FIONREAD, &availableBytes);
#endif
    return availableBytes;
}

int udp_getReceiveBufferSize(SOCKET socket) {
    int bufferSize;
#ifdef _WIN32
    int size = sizeof(bufferSize);
    if (getsockopt(socket, SOL_SOCKET, SO_RCVBUF, (char *)&bufferSize, &size) == SOCKET_ERROR)
#else
    socklen_t size = sizeof(bufferSize);
    if (getsockopt(socket, SOL_SOCKET, SO_RCVBUF, &bufferSize, &size) < 0)
#endif
    {
        return -1;
    }
    return bufferSize;
}
