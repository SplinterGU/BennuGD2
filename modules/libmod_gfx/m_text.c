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
    GRAPH * gr = text ? gr_text_bitmap( params[0], text, params[2] ) : NULL;
    string_discard( params[1] );
    if ( !gr ) return 0;
    return gr->code;
}

/* --------------------------------------------------------------------------- */
/*
    int file_dest
    int graph_dest
    int font
    int x
    int y
    string text
    int alignment
*/

int64_t libmod_gfx_text_write_in_map2( INSTANCE * my, int64_t * params ) {
    const char * text = string_get( params[5] );
    GRAPH * dest = bitmap_get( params[0], params[1] );
    if ( !text || !dest ) {
        string_discard( params[5] );
        return 0; 
    }
    int64_t result = gr_text_in_bitmap( dest, params[2], params[3], params[4], text, params[6] );
    string_discard( params[5] );
    return result;
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

    if ( withz ) var = ( DCB_TYPEDEF * ) ( intptr_t ) params[6];
    else var = ( DCB_TYPEDEF * ) ( intptr_t ) params[5];

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

    if ( withz ) return gr_text_new_var2( params[0], params[1], params[2], params[3], params[4], ( void * )( intptr_t ) params[5], t );
    return gr_text_new_var( params[0], params[1], params[2], params[3], ( void * )( intptr_t ) params[4], t );
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
 *  content of data pointed by pointer param[4]
 */

int64_t libmod_gfx_text_write_value( INSTANCE * my, int64_t * params ) {
    return gr_text_new_var( params[0], params[1], params[2], params[3], ( void * )( intptr_t ) params[5], params[4] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_text_write_value2( INSTANCE * my, int64_t * params ) {
    return gr_text_new_var2( params[0], params[1], params[2], params[3], params[4], ( void * )( intptr_t ) params[6], params[5] );
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

int64_t libmod_gfx_text_set_rgba( INSTANCE * my, int64_t * params ) {
    gr_text_setrgba( params[0], params[1], params[2], params[3], params[4] );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_text_get_rgba( INSTANCE * my, int64_t * params ) {
    uint8_t r, g, b, a;
    int64_t ret = gr_text_getrgba( params[0], &r, &g, &b, &a );

    if ( params[1] ) *( uint8_t * )( intptr_t ) params[1] = r;
    if ( params[2] ) *( uint8_t * )( intptr_t ) params[2] = g;
    if ( params[3] ) *( uint8_t * )( intptr_t ) params[3] = b;
    if ( params[4] ) *( uint8_t * )( intptr_t ) params[4] = a;

    return( ret );
}

/* --------------------------------------------------------------------------- */
