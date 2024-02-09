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
#include <stdlib.h>
#include <string.h>

#include "bgddl.h"

#include "libmod_socks.h"

#include "xstrings.h"

/* --------------------------------------------------------------------------- */

static int socks_initialized = 0;

/* --------------------------------------------------------------------------- */

int64_t libmod_socks_tcp_listen( INSTANCE *my, int64_t *params ) {
    int64_t r = tcp_listen( ( char * ) string_get( params[0] ), params[ 1 ] );
    string_discard( params[0] );
    return r;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_socks_tcp_accept( INSTANCE *my, int64_t *params ) {
    return tcp_accept( params[ 0 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_socks_tcp_connect( INSTANCE *my, int64_t *params ) {
    int64_t r = tcp_connect( ( char * ) string_get( params[0] ), params[ 1 ], params[ 2 ] );
    string_discard( params[0] );
    return r;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_socks_tcp_send( INSTANCE *my, int64_t *params ) {
    return tcp_send( params[ 0 ], ( char * ) params[ 1 ], params[ 2 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_socks_tcp_recv( INSTANCE *my, int64_t *params ) {
    return tcp_recv( params[ 0 ], ( char * ) params[ 1 ], params[ 2 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_socks_tcp_wait( INSTANCE *my, int64_t *params ) {
    return tcp_wait( ( SOCKET * ) params[ 0 ], params[ 1 ], params[ 2 ], ( SOCKET * ) params[ 3 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_socks_tcp_close( INSTANCE *my, int64_t *params ) {
    tcp_close( params[ 0 ] );
    return 0;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_socks_tcp_getavailablebytes( INSTANCE *my, int64_t *params ) {
    return tcp_getavailablebytes( params[ 0 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_socks_tcp_getremoteaddr( INSTANCE *my, int64_t *params ) {
    int64_t r = 0;
    char * ip = tcp_getremoteaddr( params[ 0 ] );
    if ( ip ) {
        r = string_new( ip ) ;
        free( ip );
    } else {
        r = string_new( "" ) ;
    }
    string_use( r ) ;
    return r ;
}

/* --------------------------------------------------------------------------- */
/* Module/Plugin Initialization Functions                                      */

void  __bgdexport( libmod_socks, module_initialize )() {
    if ( !socks_initialized ) {
        tcp_init();
        socks_initialized = 1;
    }
}

/* --------------------------------------------------------------------------- */

void __bgdexport( libmod_socks, module_finalize )() {
    if ( socks_initialized ) {
        tcp_exit();
        socks_initialized = 0;
    }
}

/* --------------------------------------------------------------------------- */
/* exports                                                                     */
/* --------------------------------------------------------------------------- */

#include "libmod_socks_exports.h"

/* --------------------------------------------------------------------------- */
