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

#ifndef __LIBMOD_SOCKS_EXPORTS
#define __LIBMOD_SOCKS_EXPORTS

/* --------------------------------------------------------------------------- */

#include "bgddl.h"
#include "libmod_socks.h"

/* --------------------------------------------------------------------------- */

#if defined(__BGDC__) || !defined(__STATIC__)

/* ----------------------------------------------------------------- */
/* Constants definition (used at compile time)                                 */

DLCONSTANT  __bgdexport( libmod_socks, constants_def )[] = {
    { "SOCKET_ERROR"                , TYPE_INT, SOCKET_ERROR                },

    { NULL                          , 0       , 0                           }
} ;

#endif

/* --------------------------------------------------------------------------- */

DLSYSFUNCS  __bgdexport( libmod_socks, functions_exports )[] = {
    FUNC( "TCP_LISTEN"                  , "SI"          , TYPE_INT      , libmod_socks_tcp_listen                           ),
    FUNC( "TCP_ACCEPT"                  , "I"           , TYPE_INT      , libmod_socks_tcp_accept                           ),
    FUNC( "TCP_CONNECT"                 , "SII"         , TYPE_INT      , libmod_socks_tcp_connect                          ),
    FUNC( "TCP_SEND"                    , "IPI"         , TYPE_INT      , libmod_socks_tcp_send                             ),
    FUNC( "TCP_RECV"                    , "IPI"         , TYPE_INT      , libmod_socks_tcp_recv                             ),
    FUNC( "TCP_WAIT"                    , "PIIP"        , TYPE_INT      , libmod_socks_tcp_wait                             ),
    FUNC( "TCP_CLOSE"                   , "I"           , TYPE_INT      , libmod_socks_tcp_close                            ),
    FUNC( "TCP_GETAVAILABLEBYTES"       , "I"           , TYPE_INT      , libmod_socks_tcp_getavailablebytes                ),
    FUNC( "TCP_GETREMOTEADDR"           , "I"           , TYPE_STRING   , libmod_socks_tcp_getremoteaddr                    ),

    FUNC( 0                             , 0             , 0             , 0                                                 )
};

/* --------------------------------------------------------------------------- */

#endif
