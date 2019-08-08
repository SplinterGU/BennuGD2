/*
 *  Copyright (C) 2006-2019 SplinterGU (Fenix/BennuGD)
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

#include <SDL.h>

#include <stdlib.h>

#include "bgdrtm.h"
#include "bgddl.h"

#include "xstrings.h"

#include "libbggfx.h"
#include "libmod_gfx.h"

#include "dlvaracc.h"

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_create( INSTANCE * my, int64_t * params ) {
    BGD_SHADER * shader = shader_create(( char * ) string_get( params[ 0 ] ),
                                        ( char * ) string_get( params[ 1 ] ),
                                        ( const char * ) string_get( params[ 2 ] ),
                                        ( const char * ) string_get( params[ 3 ] ),
                                        ( const char * ) string_get( params[ 4 ] ),
                                        ( const char * ) string_get( params[ 5 ] )
                                       );
    string_discard( params[ 0 ] );
    string_discard( params[ 1 ] );
    string_discard( params[ 2 ] );
    string_discard( params[ 3 ] );
    string_discard( params[ 4 ] );
    string_discard( params[ 5 ] );

    return ( int64_t ) ( intptr_t ) shader;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_activate( INSTANCE * my, int64_t * params ) {
    shader_activate( ( BGD_SHADER * ) params[ 0 ] );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_deactivate( INSTANCE * my, int64_t * params ) {
    shader_deactivate();
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_getlocation( INSTANCE * my, int64_t * params ) {
    int loc = shader_getlocation( ( BGD_SHADER * ) params[ 0 ], ( char * ) string_get( params[ 1 ] ) );
    string_discard( params[ 1 ] );
    return ( int64_t ) loc;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setshaderimage( INSTANCE * my, int64_t * params ) {
    GRAPH * map = bitmap_get( params[ 0 ], params[ 1 ] );
    if ( !map ) return -1;
    shader_setshaderimage( map, params[ 2 ], params[ 3 ] );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setvaluei( INSTANCE * my, int64_t * params ) {
    shader_setvaluei( ( int ) params[ 0 ], ( int ) params[ 1 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setvalueiv( INSTANCE * my, int64_t * params ) {
    shader_setvalueiv( ( int ) params[ 0 ], ( int ) params[ 1 ], ( int * ) params[ 2 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setvalue2iv( INSTANCE * my, int64_t * params ) {
    shader_setvalue2iv( ( int ) params[ 0 ], ( int ) params[ 1 ], ( int * ) params[ 2 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setvalue3iv( INSTANCE * my, int64_t * params ) {
    shader_setvalue3iv( ( int ) params[ 0 ], ( int ) params[ 1 ], ( int * ) params[ 2 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setvalue4iv( INSTANCE * my, int64_t * params ) {
    shader_setvalue4iv( ( int ) params[ 0 ], ( int ) params[ 1 ], ( int * ) params[ 2 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setvalueui( INSTANCE * my, int64_t * params ) {
    shader_setvalueui( ( int ) params[ 0 ], ( unsigned int ) params[ 1 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setvalueuiv( INSTANCE * my, int64_t * params ) {
    shader_setvalueuiv( ( int ) params[ 0 ], ( int ) params[ 1 ], ( unsigned int * ) params[ 2 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setvalue2uiv( INSTANCE * my, int64_t * params ) {
    shader_setvalue2uiv( ( int ) params[ 0 ], ( int ) params[ 1 ], ( unsigned int * ) params[ 2 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setvalue3uiv( INSTANCE * my, int64_t * params ) {
    shader_setvalue3uiv( ( int ) params[ 0 ], ( int ) params[ 1 ], ( unsigned int * ) params[ 2 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setvalue4uiv( INSTANCE * my, int64_t * params ) {
    shader_setvalue4uiv( ( int ) params[ 0 ], ( int ) params[ 1 ], ( unsigned int * ) params[ 2 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setvaluef( INSTANCE * my, int64_t * params ) {
    shader_setvaluef( ( int ) params[ 0 ], ( float ) params[ 1 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setvaluefv( INSTANCE * my, int64_t * params ) {
    shader_setvaluefv( ( int ) params[ 0 ], ( int ) params[ 1 ], ( float * ) params[ 2 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setvalue2fv( INSTANCE * my, int64_t * params ) {
    shader_setvalue2fv( ( int ) params[ 0 ], ( int ) params[ 1 ], ( float * ) params[ 2 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setvalue3fv( INSTANCE * my, int64_t * params ) {
    shader_setvalue3fv( ( int ) params[ 0 ], ( int ) params[ 1 ], ( float * ) params[ 2 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setvalue4fv( INSTANCE * my, int64_t * params ) {
    shader_setvalue4fv( ( int ) params[ 0 ], ( int ) params[ 1 ], ( float * ) params[ 2 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setvaluematrix( INSTANCE * my, int64_t * params ) {
    shader_setvaluematrix( ( int ) params[ 0 ], ( int ) params[ 1 ], ( int ) params[ 2 ], ( int ) params[ 3 ], ( GPU_bool ) params[ 4 ], ( float * ) params[ 5 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_free(  INSTANCE * my, int64_t * params ) {
    shader_free( ( BGD_SHADER * ) params[ 0 ] );
}

/* --------------------------------------------------------------------------- */
