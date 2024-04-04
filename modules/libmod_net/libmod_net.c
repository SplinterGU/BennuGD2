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

#include "libmod_net.h"

#include "xstrings.h"

/* --------------------------------------------------------------------------- */

static int net_initialized = 0;

/* --------------------------------------------------------------------------- */

int64_t libmod_net_open( INSTANCE *my, int64_t *params ) {
    char * addr = ( char * ) string_get( params[ 2 ] );
    void * r = ( void * ) net_open( params[ 0 ], params[ 1 ], *addr ? addr : NULL, params[ 3 ] );
    string_discard( params[ 2 ] );
    return ( int64_t ) r;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_net_wait( INSTANCE *my, int64_t *params ) {
    return ( int64_t ) net_wait( ( List * ) params[ 0 ], params[ 1 ], ( List * ) params[ 2 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_net_accept( INSTANCE *my, int64_t *params ) {
    return ( int64_t ) net_accept( ( _net * ) params[0] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_net_send( INSTANCE *my, int64_t *params ) {
    return ( int64_t ) net_send( ( _net * ) params[ 0 ], ( const void * ) params[ 1 ], ( size_t ) params[ 2 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_net_recv( INSTANCE *my, int64_t *params ) {
    return ( int64_t ) net_recv( ( _net * ) params[ 0 ], ( void * ) params[ 1 ], ( size_t ) params[ 2 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_net_close( INSTANCE *my, int64_t *params ) {
    net_close( ( _net * ) params[ 0 ] );
    return 0;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_net_getavailablebytes( INSTANCE *my, int64_t *params ) {
    return ( int64_t ) net_getavailablebytes( ( _net * ) params[ 0 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_net_getReceiveBufferSize( INSTANCE *my, int64_t *params ) {
    return ( int64_t ) net_getReceiveBufferSize( ( _net * ) params[ 0 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_net_getremoteaddr( INSTANCE *my, int64_t *params ) {
    int64_t r = 0;
    char * ip = net_getremoteaddr( ( _net * ) params[ 0 ] );
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

int64_t libmod_net_is_new_connection( INSTANCE *my, int64_t *params ) {
    return ( int64_t ) net_is_new_connection( ( _net * ) params[ 0 ] );
}

/* --------------------------------------------------------------------------- */
/* Module/Plugin Initialization Functions                                      */

void  __bgdexport( libmod_net, module_initialize )() {
    if ( !net_initialized ) {
        net_init();
        net_initialized = 1;
    }
}

/* --------------------------------------------------------------------------- */

void __bgdexport( libmod_net, module_finalize )() {
    if ( net_initialized ) {
        net_exit();
        net_initialized = 0;
    }
}

/* --------------------------------------------------------------------------- */
/* exports                                                                     */
/* --------------------------------------------------------------------------- */

#include "libmod_net_exports.h"

/* --------------------------------------------------------------------------- */
