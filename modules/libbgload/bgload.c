/*
 *  Copyright (C) SplinterGU (Fenix/BennuGD) (Since 2006)
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

#include <stdint.h>
#include <string.h>
#include <SDL.h>

#include "xstrings.h"
#include "bgload.h"

/* --------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------- */
/* - bgload functions -------------------------------------------------------- */
/* --------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/**
 * PREP
 * Helper function preparing params
 **/

static bgdata *prep( int64_t *params ) {
    bgdata *t = ( bgdata* )malloc( sizeof( bgdata ) );
    if ( !t ) {
        string_discard( params[0] );
        return NULL;
    }
    t->file = strdup(( char * )string_get( params[0] ));
    string_discard( params[0] );
    if ( !t->file ) {
        free( t );
        return NULL;
    }
    t->id = ( int64_t * )( intptr_t )params[1];
    *( t->id ) = -2 ; // WAIT STATUS
    return t;
}

/* --------------------------------------------------------------------------- */
/**
 * bgDoLoad
 * Helper function executed in the new thread
 **/

static int bgDoLoad( void *d ) {
    bgdata *t = ( bgdata* )d;
    *( t->id ) = ( *t->fn )( t->file );
    free( t->file );
    free( t );
    return 0;
}

/* --------------------------------------------------------------------------- */

int bgload( int64_t ( *fn )( void * ), int64_t *params ) {
    bgdata *t = prep( params );
    if ( !t ) return -1;
    t->fn = fn;
    SDL_CreateThread( bgDoLoad, NULL, ( void * )t );
    return 0 ;
}

/* --------------------------------------------------------------------------- */
