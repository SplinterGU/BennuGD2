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

/* --------------------------------------------------------------------------- */

#include <stdlib.h>

#include "bgdrtm.h"
#include "bgddl.h"

#include "xstrings.h"

#include "libbggfx.h"
#include "libmod_gfx.h"

#include "dlvaracc.h"

/* --------------------------------------------------------------------------- */
/* Text Functions                                                              */

int64_t libmod_gfx_text_write( INSTANCE * my, int64_t * params ) {
    const char * text = string_get( params[4] );
    int64_t r = text ? gr_text_new( params[0], params[1], params[2], params[3], text ) : 0;
    string_discard( params[4] );
    return r;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_text_write2( INSTANCE * my, int64_t * params ) {
    const char * text = string_get( params[5] );
    int64_t r = text ? gr_text_new2( params[0], params[1], params[2], params[3], params[4], text ) : 0;
    string_discard( params[5] );
    return r;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_text_write_in_map( INSTANCE * my, int64_t * params ) {
    const char * text = string_get( params[1] );
    GRAPH * gr;
    if ( !text ) return 0;
    gr = gr_text_bitmap( params[0], text, params[2] );
    string_discard( params[1] );
    if ( !gr ) return 0;
    return gr->code;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : __libmod_gfx_text_write_var
 *
 *  Creates a new text associated with a variable
 *
 *  FENIX LANG PARAMS:
 *      params[0] : fnt to use
 *      params[1] : X
 *      params[2] : Y
 *      params[3] : align
 *      params[4] : pointer
 *      params[5] : DCB_TYPE
 *
 *  FENIX RETURN VALUE:
 *     Text ID for the newly created text
 *
 */

static int __libmod_gfx_text_write_var( int withz, INSTANCE * my, int64_t * params ) {
    DCB_TYPEDEF * var;
    int64_t t = 0;

    if ( withz ) var =( DCB_TYPEDEF * )params[6];
    else var =( DCB_TYPEDEF * )params[5];

    switch( var->BaseType[0] ) {
        case TYPE_DOUBLE:
            t = TEXT_DOUBLE;
            break;

        case TYPE_FLOAT:
            t = TEXT_FLOAT;
            break;

        case TYPE_QWORD:
            t = TEXT_QWORD;
            break;

        case TYPE_DWORD:
            t = TEXT_DWORD;
            break;

        case TYPE_WORD:
            t = TEXT_WORD;
            break;

        case TYPE_BYTE:
            t = TEXT_BYTE;
            break;

        case TYPE_STRING:
            t = TEXT_STRING;
            break;

        case TYPE_ARRAY:
            if ( var->BaseType[1] == TYPE_CHAR ) {
                t = TEXT_CHARARRAY;
                break;
            }
            break;

        case TYPE_SBYTE:
            t = TEXT_SBYTE;
            break;

        case TYPE_CHAR:
            t = TEXT_CHAR;
            break;

        case TYPE_SHORT:
            t = TEXT_SHORT;
            break;

        case TYPE_INT32:
            t = TEXT_INT32;
            break;

        case TYPE_INT:
            t = TEXT_INT;
            break;

        case TYPE_POINTER:
            t = TEXT_POINTER;
            break;

        default:
            return -1;
            break;
    }

    if ( withz ) return gr_text_new_var2( params[0], params[1], params[2], params[3], params[4], ( void * )params[5], t );
    return gr_text_new_var( params[0], params[1], params[2], params[3], ( void * )params[4], t );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_text_write_var( INSTANCE * my, int64_t * params ) {
    return __libmod_gfx_text_write_var( 0, my, params );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_text_write_var2( INSTANCE * my, int64_t * params ) {
    return __libmod_gfx_text_write_var( 1, my, params );
}

/* --------------------------------------------------------------------------- */
/*
 *  Same as libmod_gfx_text_write_var, but param[5] not given and always set to VAR_STRING
 */

int64_t libmod_gfx_text_write_string( INSTANCE * my, int64_t * params ) {
    return gr_text_new_var( params[0], params[1], params[2], params[3], ( void * )params[4], TEXT_STRING );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_text_write_string2( INSTANCE * my, int64_t * params ) {
    return gr_text_new_var2( params[0], params[1], params[2], params[3], params[4], ( void * )params[5], TEXT_STRING );
}

/* --------------------------------------------------------------------------- */
/*
 *  Same as libmod_gfx_text_write_var, but param[5] not given and always set to VAR_INT64
 */

int64_t libmod_gfx_text_write_int( INSTANCE * my, int64_t * params ) {
    return gr_text_new_var( params[0], params[1], params[2], params[3], ( void * )params[4], TEXT_INT );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_text_write_int2( INSTANCE * my, int64_t * params ) {
    return gr_text_new_var2( params[0], params[1], params[2], params[3], params[4], ( void * )params[5], TEXT_INT );
}

/* --------------------------------------------------------------------------- */
/*
 *  Same as libmod_gfx_text_write_var, but param[5] not given and always set to VAR_INT
 */

int64_t libmod_gfx_text_write_int32( INSTANCE * my, int64_t * params ) {
    return gr_text_new_var( params[0], params[1], params[2], params[3], ( void * )params[4], TEXT_INT32 );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_text_write_int322( INSTANCE * my, int64_t * params ) {
    return gr_text_new_var2( params[0], params[1], params[2], params[3], params[4], ( void * )params[5], TEXT_INT32 );
}

/* --------------------------------------------------------------------------- */
/*
 *  Same as libmod_gfx_text_write_var, but param[5] not given and always set to VAR_DOUBLE
 */

int64_t libmod_gfx_text_write_double( INSTANCE * my, int64_t * params ) {
    return gr_text_new_var( params[0], params[1], params[2], params[3], ( void * )params[4], TEXT_DOUBLE );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_text_write_double2( INSTANCE * my, int64_t * params ) {
    return gr_text_new_var2( params[0], params[1], params[2], params[3], params[4], ( void * )params[5], TEXT_DOUBLE );
}

/* --------------------------------------------------------------------------- */
/*
 *  Same as libmod_gfx_text_write_var, but param[5] not given and always set to VAR_FLOAT
 */

int64_t libmod_gfx_text_write_float( INSTANCE * my, int64_t * params ) {
    return gr_text_new_var( params[0], params[1], params[2], params[3], ( void * )params[4], TEXT_FLOAT );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_text_write_float2( INSTANCE * my, int64_t * params ) {
    return gr_text_new_var2( params[0], params[1], params[2], params[3], params[4], ( void * )params[5], TEXT_FLOAT );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_text_move( INSTANCE * my, int64_t * params ) {
    gr_text_move( params[0], params[1], params[2] );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_text_move2( INSTANCE * my, int64_t * params ) {
    gr_text_move2( params[0], params[1], params[2], params[3] );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_text_delete( INSTANCE * my, int64_t * params ) {
    gr_text_destroy( params[0] );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_text_height( INSTANCE * my, int64_t * params ) {
    const char * str = string_get( params[1] );
    int64_t result = gr_text_height( params[0], ( const unsigned char * ) str );
    string_discard( params[1] );
    return result;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_text_width( INSTANCE * my, int64_t * params ) {
    const char * str = string_get( params[1] );
    int64_t result = gr_text_width( params[0], ( const unsigned char * ) str );
    string_discard( params[1] );
    return result;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_text_set_color( INSTANCE * my, int64_t * params ) {
    gr_text_setcolor( params[0], params[1], params[2], params[3] );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_text_get_color( INSTANCE * my, int64_t * params ) {
    uint8_t r, g, b;
    int64_t ret = gr_text_getcolor( params[0], &r, &g, &b );

    if ( params[1] ) *( uint8_t * )params[1] = r;
    if ( params[2] ) *( uint8_t * )params[2] = g;
    if ( params[3] ) *( uint8_t * )params[3] = b;

    return( ret );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_text_set_alpha( INSTANCE * my, int64_t * params ) {
    gr_text_setalpha( params[0], params[1] );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_text_get_alpha( INSTANCE * my, int64_t * params ) {
    uint8_t alpha;
    int64_t ret = gr_text_getalpha( params[0], &alpha );

    if ( params[1] ) *( uint8_t * )params[1] = alpha;

    return( ret );
}
