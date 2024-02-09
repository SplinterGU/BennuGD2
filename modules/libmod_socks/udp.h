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

#ifndef _UDP_H
    #define _UDP_H

    #ifdef _WIN32
        #include <winsock2.h>
        #include <ws2tcpip.h>
        #pragma comment(lib, "ws2_32.lib")
    #else
#ifdef __SWITCH__
        #include <sys/socket.h>
#endif
        #include <arpa/inet.h>
        #include <netdb.h>
        #include <unistd.h>
        #include <sys/ioctl.h>

        #define SOCKET_ERROR    -1

        typedef int SOCKET;
    #endif

    #ifndef MSG_NOSIGNAL
        #define MSG_NOSIGNAL    0
    #endif

    /* --------------------------------------------------------------------------- */

    int udp_init();
    void udp_exit();
    SOCKET udp_bind(char *sAddr, int nPort);
    int udp_sendto(SOCKET socket, char *sBuf, int nLen, char *sDestAddr, int nDestPort);
    int udp_recvfrom(SOCKET socket, char *sBuf, int nLen, char *sSrcAddr, int *nSrcPort);
    int udp_wait(SOCKET *sockets, int nCount, int nTimeout, SOCKET *events);
    void udp_close(SOCKET socket);
    int udp_getavailablebytes(SOCKET socket);
    int udp_getReceiveBufferSize(SOCKET socket);

    /* --------------------------------------------------------------------------- */

#endif
