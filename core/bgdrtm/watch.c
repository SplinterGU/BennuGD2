/*
 *  Copyright (C) 2019 SplinterGU (Fenix/BennuGD)
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

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "watch.h"

/* --------------------------------------------------------------------------- */
/* Create watcheable element                                                   */
/* --------------------------------------------------------------------------- */

watch * watch_create( void * element, int size ) {
    watch * t = malloc( sizeof( watch ) );
    if ( !t ) return NULL;
    t->size = size;
    if ( !( t->original = ( void * ) malloc( size ) ) ) {
        free( t );
        return NULL;
    }
    t->original = malloc( size );
    if ( !( t->verify = ( void * ) malloc( size ) ) ) {
        free( t->original );
        free( t );
        return NULL;
    }

    memmove( t->original, element, t->size );
    memmove( t->verify, element, t->size );
    return t;
}

/* --------------------------------------------------------------------------- */
/* Destroy watcheable element                                                  */
/* --------------------------------------------------------------------------- */

void watch_destroy( watch * t ) {
    if ( t ) {
        free( t->verify );
        free( t->original );
        free( t );
    }
}

/* --------------------------------------------------------------------------- */
/* Reset element to original value                                             */
/* --------------------------------------------------------------------------- */

int watch_reset( watch * t, void * element ) {
    if ( !t ) return -1;
    memmove( element, t->original, t->size );
    memmove( t->verify, t->original, t->size );
    return 0;
}

/* --------------------------------------------------------------------------- */
/* Set element as default/original value                                       */
/* --------------------------------------------------------------------------- */

int watch_set_default( watch * t, void * element ) {
    if ( !t ) return -1;
    memmove( t->original, element, t->size );
    memmove( t->verify, element, t->size );
    return 0;
}

/* --------------------------------------------------------------------------- */
/* Set transitory value to element                                             */
/* --------------------------------------------------------------------------- */

int watch_set( watch * t, void * element, void * value ) {
    if ( !t ) return -1;
    memmove( element, value, t->size );
    memmove( t->verify, value, t->size );
    return 0;
}

/* --------------------------------------------------------------------------- */
/* Test element.                                                               */
/* if it's changed then changed value is the original/default value            */
/* --------------------------------------------------------------------------- */

int watch_test( watch * t, void * element ) {
    if ( !t ) return 0;
    int sz = t->size;
    uint8_t *a = element, *b = t->verify;
    while( sz-- ) if ( *a++ != *b++ ) return 0;
    watch_set_default( t, element );
    return 1;
}

/* --------------------------------------------------------------------------- */
