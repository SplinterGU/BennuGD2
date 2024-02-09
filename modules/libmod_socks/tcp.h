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

#ifndef _TCP_H
    #define _TCP_H

    #ifdef _WIN32
        #include <winsock2.h>
    #else
        #include <netdb.h>
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

    /* --------------------------------------------------------------------------- */

    int tcp_init();
    void tcp_exit();
    SOCKET tcp_listen( char * saddr, int nport );
    SOCKET tcp_accept( SOCKET socket );
    SOCKET tcp_connect( char *shost, int nport, int ntimeout );
    int tcp_send( SOCKET socket, char * sbuf, int nlen );
    int tcp_recv( SOCKET socket, char * sbuf, int nlen );
    int tcp_wait( SOCKET *sockets, int ncount, int ntimeout, SOCKET *events );
    void tcp_close( SOCKET socket );
    int tcp_getavailablebytes( SOCKET socket );
    int tcp_getReceiveBufferSize( SOCKET socket );
    char * tcp_getremoteaddr( SOCKET socket );

    /* --------------------------------------------------------------------------- */

#endif
