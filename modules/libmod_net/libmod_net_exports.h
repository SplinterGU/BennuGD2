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

#ifndef __LIBMOD_NET_EXPORTS
#define __LIBMOD_NET_EXPORTS

/* --------------------------------------------------------------------------- */

#include "bgddl.h"
#include "libmod_net.h"

/* --------------------------------------------------------------------------- */

#if defined(__BGDC__) || !defined(__STATIC__)

/* ----------------------------------------------------------------- */
/* Constants definition (used at compile time)                                 */

DLCONSTANT  __bgdexport( libmod_net, constants_def )[] = {
    { "NET_MODE_SERVER"             , TYPE_INT  , NET_MODE_SERVER               },
    { "NET_MODE_CLIENT"             , TYPE_INT  , NET_MODE_CLIENT               },

    { "NET_PROTO_UDP"               , TYPE_INT  , NET_PROTO_UDP                 },
    { "NET_PROTO_TCP"               , TYPE_INT  , NET_PROTO_TCP                 },

    { "NET_ERROR"                   , TYPE_INT  , NET_ERROR                     },
    { "NET_TIMEOUT"                 , TYPE_INT  , NET_TIMEOUT                   },
    { "NET_DISCONNECTED"            , TYPE_INT  , NET_DISCONNECTED              },

    { NULL                          , 0         , 0                             }
} ;

#endif

/* --------------------------------------------------------------------------- */

DLSYSFUNCS  __bgdexport( libmod_net, functions_exports )[] = {
    FUNC( "NET_OPEN"                    , "IISI"        , TYPE_POINTER  , libmod_net_open                       ),
    FUNC( "NET_WAIT"                    , "PIP"         , TYPE_INT      , libmod_net_wait                       ),
    FUNC( "NET_ACCEPT"                  , "P"           , TYPE_POINTER  , libmod_net_accept                     ),
    FUNC( "NET_SEND"                    , "PPI"         , TYPE_INT      , libmod_net_send                       ),
    FUNC( "NET_RECV"                    , "PPI"         , TYPE_INT      , libmod_net_recv                       ),
    FUNC( "NET_CLOSE"                   , "P"           , TYPE_INT      , libmod_net_close                      ),
    FUNC( "NET_GETAVAILABLEBYTES"       , "P"           , TYPE_INT      , libmod_net_getavailablebytes          ),
    FUNC( "NET_GETRECEIVEBUFFERSIZE"    , "P"           , TYPE_INT      , libmod_net_getReceiveBufferSize       ),
    FUNC( "NET_GETREMOTEADDR"           , "P"           , TYPE_STRING   , libmod_net_getremoteaddr              ),
    FUNC( "NET_IS_NEW_CONNECTION"       , "P"           , TYPE_INT      , libmod_net_is_new_connection          ),

    FUNC( 0                             , 0             , 0             , 0                                     )
};

/* --------------------------------------------------------------------------- */

#endif
