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

#include <stdint.h>
#include <stddef.h>

#include "bgddl.h"
#include "libmod_misc.h"

int64_t libmod_misc_list_create( INSTANCE * my, int64_t * params ) {
    return ( int64_t )( intptr_t ) list_create( -1 );
}

int64_t libmod_misc_list_create2( INSTANCE * my, int64_t * params ) {
    return ( int64_t )( intptr_t ) list_create( params[ 0 ]);
}

int64_t libmod_misc_list_free( INSTANCE * my, int64_t * params ) {
    list_free( ( List * )( intptr_t ) params[0], NULL );
    return 1;
}

int64_t libmod_misc_list_empty( INSTANCE * my, int64_t * params ) {
    list_empty( ( List * )( intptr_t ) params[0], NULL );
    return 1;
}

int64_t libmod_misc_list_insertItem( INSTANCE * my, int64_t * params ) {
    list_insertItem( ( List * )( intptr_t ) params[0], ( void * )( intptr_t ) params[ 1 ] );
    return 1;
}

int64_t libmod_misc_list_removeItem( INSTANCE * my, int64_t * params ) {
    list_removeItem( ( List * )( intptr_t ) params[0], ( void * )( intptr_t ) params[ 1 ], NULL );
    return 1;
}

int64_t libmod_misc_list_walk( INSTANCE * my, int64_t * params ) {
    return ( int64_t )( intptr_t ) list_walk( ( List * )( intptr_t ) params[0], ( void ** )( intptr_t ) params[1] );
}

int64_t libmod_misc_list_size( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) list_size( ( List * )( intptr_t ) params[0] );
}

/* ----------------------------------------------------------------- */
