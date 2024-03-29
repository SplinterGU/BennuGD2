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

#include <stdio.h>
#include <stdlib.h>

#include "bgdc.h"

#include "varspace.h"

/* ---------------------------------------------------------------------- */
/* Constants manager                                                      */
/* ---------------------------------------------------------------------- */

static CONSTANT * constants ;
static int constants_used ;
static int constants_reserved ;

void constants_init() {
    constants = ( CONSTANT * ) calloc( 16, sizeof( CONSTANT ) );
    constants_reserved = 16 ;
    constants_used = 0 ;
}

void constants_alloc( int count ) {
    constants = ( CONSTANT * ) realloc( constants, ( constants_reserved += count ) * sizeof( CONSTANT ) ) ;
    if ( !constants ) {
        fprintf( stdout, "constants_alloc: out of memory\n" ) ;
        exit( 1 ) ;
    }
}

CONSTANT * constants_search( int64_t code ) {
    int i ;
    for ( i = 0 ; i < constants_used ; i++ ) if ( constants[i].code == code ) return &constants[i] ;
    return 0 ;
}

void constants_add( int64_t code, TYPEDEF type, int64_t value ) {
    CONSTANT * c;

    if ( constants_used == constants_reserved ) constants_alloc( 16 ) ;

    if ( varspace_search( &global, code ) ) {
        token.code = code;
        token.type = IDENTIFIER;
        compile_error( MSG_VARIABLE_REDECLARED_AS_CONSTANT ) ;
    }

    if ( varspace_search( &local, code ) ) {
        token.code = code;
        token.type = IDENTIFIER;
        compile_error( MSG_VARIABLE_REDECLARED_AS_CONSTANT ) ;
    }

    if (( c = constants_search( code ) ) && ( !typedef_is_equal( c->type, type ) || c->value != value ) ) {
        token.code = string_new( identifier_name( code ) );
        token.type = STRING;
        compile_error( "Constant redefined" );
    }

    constants[constants_used].code = code ;
    constants[constants_used].type = type ;
    constants[constants_used].value = value ;
    constants_used++ ;
}

void constants_dump() {
    int i ;
    printf( "\n---- %d constants ----\n\n", constants_used ) ;
    for ( i = 0 ; i < constants_used ; i++ ) {
        printf( "%4d: %-32s= ", i, identifier_name( constants[i].code ) ) ;
    
        if ( typedef_is_integer( constants[i].type ) ) {
            if ( typedef_is_unsigned( constants[i].type ) ) {
                printf( "%" PRIu64 "\n", constants[i].value ) ;
            } else {
                printf( "%" PRId64 "\n", constants[i].value ) ;
            }
        }
        else
        if ( typedef_is_float( constants[i].type ) || typedef_is_double( constants[i].type ) ) {
            printf("%f\n", *(double *)(&constants[i].value));
        }
        else
        if ( typedef_is_string( constants[i].type ) ) {
            printf("%s\n", string_get(constants[i].value));
        }
    }
}
