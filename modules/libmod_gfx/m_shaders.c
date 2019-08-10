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

int64_t libmod_gfx_shader_free(  INSTANCE * my, int64_t * params ) {
    shader_free( ( BGD_SHADER * ) params[ 0 ] );
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

int64_t libmod_gfx_shader_getattributelocation( INSTANCE * my, int64_t * params ) {
    int loc = shader_getattributelocation( ( BGD_SHADER * ) params[ 0 ], ( char * ) string_get( params[ 1 ] ) );
    string_discard( params[ 1 ] );
    return ( int64_t ) loc;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_getuniformlocation( INSTANCE * my, int64_t * params ) {
    int loc = shader_getuniformlocation( ( BGD_SHADER * ) params[ 0 ], ( char * ) string_get( params[ 1 ] ) );
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

int64_t libmod_gfx_shader_setattributei( INSTANCE * my, int64_t * params ) {
    shader_setattributei( ( int ) params[ 0 ], ( int ) params[ 1 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setattributeiv( INSTANCE * my, int64_t * params ) {
    shader_setattributeiv( ( int ) params[ 0 ], ( int ) params[ 1 ], ( int * ) params[ 2 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setattributeui( INSTANCE * my, int64_t * params ) {
    shader_setattributeui( ( int ) params[ 0 ], ( unsigned int ) params[ 1 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setattributeuiv( INSTANCE * my, int64_t * params ) {
    shader_setattributeuiv( ( int ) params[ 0 ], ( int ) params[ 1 ], ( unsigned int * ) params[ 2 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setattributef( INSTANCE * my, int64_t * params ) {
    shader_setattributef( ( int ) params[ 0 ], ( float ) params[ 1 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setattributefv( INSTANCE * my, int64_t * params ) {
    shader_setattributefv( ( int ) params[ 0 ], ( int ) params[ 1 ], ( float * ) params[ 2 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setuniformi( INSTANCE * my, int64_t * params ) {
    shader_setuniformi( ( int ) params[ 0 ], ( int ) params[ 1 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setuniformiv( INSTANCE * my, int64_t * params ) {
    shader_setuniformiv( ( int ) params[ 0 ], ( int ) params[ 1 ], ( int * ) params[ 2 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setuniform2iv( INSTANCE * my, int64_t * params ) {
    shader_setuniform2iv( ( int ) params[ 0 ], ( int ) params[ 1 ], ( int * ) params[ 2 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setuniform3iv( INSTANCE * my, int64_t * params ) {
    shader_setuniform3iv( ( int ) params[ 0 ], ( int ) params[ 1 ], ( int * ) params[ 2 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setuniform4iv( INSTANCE * my, int64_t * params ) {
    shader_setuniform4iv( ( int ) params[ 0 ], ( int ) params[ 1 ], ( int * ) params[ 2 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setuniformui( INSTANCE * my, int64_t * params ) {
    shader_setuniformui( ( int ) params[ 0 ], ( unsigned int ) params[ 1 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setuniformuiv( INSTANCE * my, int64_t * params ) {
    shader_setuniformuiv( ( int ) params[ 0 ], ( int ) params[ 1 ], ( unsigned int * ) params[ 2 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setuniform2uiv( INSTANCE * my, int64_t * params ) {
    shader_setuniform2uiv( ( int ) params[ 0 ], ( int ) params[ 1 ], ( unsigned int * ) params[ 2 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setuniform3uiv( INSTANCE * my, int64_t * params ) {
    shader_setuniform3uiv( ( int ) params[ 0 ], ( int ) params[ 1 ], ( unsigned int * ) params[ 2 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setuniform4uiv( INSTANCE * my, int64_t * params ) {
    shader_setuniform4uiv( ( int ) params[ 0 ], ( int ) params[ 1 ], ( unsigned int * ) params[ 2 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setuniformf( INSTANCE * my, int64_t * params ) {
    shader_setuniformf( ( int ) params[ 0 ], ( float ) params[ 1 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setuniformfv( INSTANCE * my, int64_t * params ) {
    shader_setuniformfv( ( int ) params[ 0 ], ( int ) params[ 1 ], ( float * ) params[ 2 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setuniform2fv( INSTANCE * my, int64_t * params ) {
    shader_setuniform2fv( ( int ) params[ 0 ], ( int ) params[ 1 ], ( float * ) params[ 2 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setuniform3fv( INSTANCE * my, int64_t * params ) {
    shader_setuniform3fv( ( int ) params[ 0 ], ( int ) params[ 1 ], ( float * ) params[ 2 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setuniform4fv( INSTANCE * my, int64_t * params ) {
    shader_setuniform4fv( ( int ) params[ 0 ], ( int ) params[ 1 ], ( float * ) params[ 2 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setuniformmatrix( INSTANCE * my, int64_t * params ) {
    shader_setuniformmatrix( ( int ) params[ 0 ], ( int ) params[ 1 ], ( int ) params[ 2 ], ( int ) params[ 3 ], ( GPU_bool ) params[ 4 ], ( float * ) params[ 5 ] );
}

/* --------------------------------------------------------------------------- */
