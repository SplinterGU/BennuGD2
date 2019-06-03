/*
 *  Copyright (C) 2006-2019 SplinterGU (Fenix/BennuGD)
 *  Copyright (C) 2002-2006 Fenix Team (Fenix)
 *  Copyright (C) 1999-2002 José Luis Cebrián Pagüe (Fenix)
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

#if USE_CRYPT

#include "b_crypt.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bgddl.h"

#include "libmod_misc.h"

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_crypt_new( INSTANCE * my, int64_t * params ) {
    return ( ( int64_t ) crypt_create( params[0], ( char * ) params[1] ) );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_crypt_del( INSTANCE * my, int64_t * params ) {
    crypt_destroy( ( crypt_handle * ) params[0] );
    return 1;
}

/* --------------------------------------------------------------------------- */

static int __crypt( crypt_handle * ch, char * in, char * out, int blocks, int enc ) {
    if ( !ch || !in || !out || blocks <= 0 ) return -1;

    while ( blocks-- ) {
        if ( crypt_data( ch, in, out, 8, enc ) <= 0 ) return -1;
        in += 8;
        out += 8;
    }

    return 0;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_crypt_encrypt( INSTANCE * my, int64_t * params ) {
    return ( __crypt( ( crypt_handle * ) params[0], ( char * ) params[1], ( char * ) params[2], params[3], 1 ) );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_crypt_decrypt( INSTANCE * my, int64_t * params ) {
    return ( __crypt( ( crypt_handle * ) params[0], ( char * ) params[1], ( char * ) params[2], params[3], 0 ) );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_crypt_encrypt2( INSTANCE * my, int64_t * params ) {
    int64_t r;
    crypt_handle * ch = crypt_create( params[0], ( char * ) params[1] );
    r = __crypt( ch, ( char * ) params[2], ( char * ) params[3], params[4], 1 );
    crypt_destroy( ch );
    return r;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_crypt_decrypt2( INSTANCE * my, int64_t * params ) {
    int64_t r;
    crypt_handle * ch = crypt_create( params[0], ( char * ) params[1] );
    r = __crypt( ch, ( char * ) params[2], ( char * ) params[3], params[4], 0 );
    crypt_destroy( ch );
    return r;
}

/* --------------------------------------------------------------------------- */

#endif
