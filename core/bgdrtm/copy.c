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

/*
 * FILE        : i_copy.c
 * DESCRIPTION : Struct copy functions based on varspace type info
 *
 * HISTORY:      0.85 - First version
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>
#include <time.h>

#include "xstrings.h"
#include "bgdrtm.h"

static int64_t copytype( void * dst, void * src, DCB_TYPEDEF * var );

/*
 *  FUNCTION : copyvars
 *
 *  Copy data using a varspace's type information. It updates the
 *  destination pointer while walking the variables.
 *
 *  PARAMS :
 *  dst    Pointer to the destination memory
 *  src    Pointer to the data
 *  var    Pointer to the type array
 *  nvars   Number of variables (length of var array)
 *
 *  RETURN VALUE :
 *      Number of bytes actually copied
 *
 */

int64_t copyvars( void * dst, void * src, DCB_VAR * var, int nvars ) {
    int64_t result = 0;

    for ( ; nvars > 0; nvars--, var++ ) {
        int64_t partial = copytype( dst, src, &var->Type );
        src = (( uint8_t* )src ) + partial;
        dst = (( uint8_t* )dst ) + partial;
        result += partial;
    }
    return result;
}


/*
 *  FUNCTION : copytypes
 *
 *  Copy data from memory using type information stored in memory
 *
 *  PARAMS :
 *  dst    Pointer to the destination memory
 *  src    Pointer to the source
 *  var    Pointer to the type array
 *  nvars   Number of variables (length of var array)
 *
 *  RETURN VALUE :
 *      Number of bytes actually coppied
 *
 */

int64_t copytypes( void * dst, void * src, DCB_TYPEDEF * var, int64_t nvars, int64_t reps ) {
    int64_t result = 0, partial, _nvars = nvars;
    DCB_TYPEDEF * _var = var;

    for ( ; reps > 0; reps-- ) {
        var = _var;
        nvars = _nvars;
        for ( ; nvars > 0; nvars--, var++ ) {
            partial = copytype( dst, src, var );
            result += partial;
            src = (( uint8_t* )src ) + partial;
            dst = (( uint8_t* )dst ) + partial;
        }
    }
    return result;
}

/*
 *  FUNCTION : copytype
 *
 *  Copy one variable using the given type information.
 *
 *  PARAMS :
 *  dst    Pointer to the file object
 *  src    Pointer to the data
 *  var    Pointer to the variable type
 *
 *  RETURN VALUE :
 *      Number of bytes actually written
 *
 */

static int64_t copytype( void * dst, void * src, DCB_TYPEDEF * var ) {
    int64_t count = 1, result = 0, n = 0;

    for ( ;; ) {
        switch ( var->BaseType[n] ) {
            case TYPE_DOUBLE:
            case TYPE_INT:
            case TYPE_QWORD:
            case TYPE_POINTER:
                memcpy( dst, src, sizeof( int64_t ) * count );
                return sizeof( int64_t ) * count;

            case TYPE_FLOAT:
            case TYPE_INT32:
            case TYPE_DWORD:
                memcpy( dst, src, sizeof( int32_t ) * count );
                return sizeof( int32_t ) * count;

            case TYPE_WORD:
            case TYPE_SHORT:
                memcpy( dst, src, sizeof( int16_t ) * count );
                return sizeof( int16_t ) * count;

            case TYPE_BYTE:
            case TYPE_SBYTE:
            case TYPE_CHAR:
                memcpy( dst, src, count );
                return count;

            case TYPE_STRING:
                while ( count-- ) {
                    string_discard( *( int64_t * )dst );
                    string_use( *( int64_t * )src );
                    *(( int64_t * )dst ) = *(( int64_t * )src );
                    dst = (( int64_t * )dst ) + 1;
                    src = (( int64_t * )src ) + 1;
                    result += sizeof( int64_t );
                }
                return result;

            case TYPE_ARRAY:
                count *= var->Count[n];
                n++;
                continue;

            case TYPE_STRUCT:
                for ( ; count; count-- ) {
                    int partial = copyvars( dst, src, dcb.varspace_vars[var->Members], dcb.varspace[var->Members].NVars );
                    src = (( uint8_t* )src ) + partial;
                    dst = (( uint8_t* )dst ) + partial;
                    result += partial;
                }
                break;

            default:
                fprintf( stderr, "ERROR: Runtime error - Could not copy datatype\n" ) ;
                exit( 1 );
                break;
        }
        break;
    }
    return result;
}

/**
 *  POINTER #COPY# (POINTER DEST, POINTER SRC, POINTER VARSPACE, INT VARS)
 *
 *  Copy struct data from src to dst, using the information varspace given
 **/

int64_t bgd_copy_struct( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) copytypes(( void * )( intptr_t )params[0], ( void * )( intptr_t )params[1], ( DCB_TYPEDEF * )( intptr_t )params[2], params[3], params[4] );
}

int64_t bgd_internal_memcopy( INSTANCE * my, int64_t * params ) {
    memmove(( void * )( intptr_t )( params[0] ), ( void * )( intptr_t )( params[1] ), params[2]);
    return 1 ;
}

int64_t bgd_internal_copy_string_array( INSTANCE * my, int64_t * params ) {
    int64_t n = params[ 2 ];
    int64_t * dst = (int64_t *)( intptr_t )( params[ 0 ] );
    int64_t * src = (int64_t *)( intptr_t )( params[ 1 ] );
    while( n-- ) {
        string_discard( *dst );
        *dst = *src;
        string_use( *dst );
        dst++; src++;
    }
    return 1 ;
}
