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

int64_t libmod_gfx_shader_get_language( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) shader_get_language();
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_get_min_version( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) shader_get_min_version();
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_get_max_version( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) shader_get_max_version();
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_create( INSTANCE * my, int64_t * params ) {
    BGD_SHADER * shader = shader_create(( char * ) string_get( params[ 0 ] ),
                                        ( char * ) string_get( params[ 1 ] )
                                       );
    string_discard( params[ 0 ] );
    string_discard( params[ 1 ] );
    return ( int64_t ) ( intptr_t ) shader;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_free(  INSTANCE * my, int64_t * params ) {
    shader_free( ( BGD_SHADER * ) ( intptr_t ) params[ 0 ] );
    return 1;
}

#if 0
/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_getattributelocation( INSTANCE * my, int64_t * params ) {
    int loc = shader_getattributelocation( ( BGD_SHADER * ) ( intptr_t ) params[ 0 ], ( char * ) string_get( params[ 1 ] ) );
    string_discard( params[ 1 ] );
    return ( int64_t ) loc;
}
#endif

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_getuniformlocation( INSTANCE * my, int64_t * params ) {
    int loc = shader_getuniformlocation( ( BGD_SHADER * ) ( intptr_t ) params[ 0 ], ( char * ) string_get( params[ 1 ] ) );
    string_discard( params[ 1 ] );
    return ( int64_t ) loc;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_create_parameters( INSTANCE * my, int64_t * params ) {
    return ( int64_t )( intptr_t ) shader_create_parameters( params[ 0 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_free_parameters( INSTANCE * my, int64_t * params ) {
    shader_free_parameters( ( BGD_SHADER_PARAMETERS * )( intptr_t ) params[ 0 ] );
    return 1;
}

/* --------------------------------------------------------------------------- */
/* Set functions                                                               */

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setparam( INSTANCE * my, int64_t * params ) {
    void * param = ( void * )( intptr_t ) params[ 0 ];
    if ( !param ) return -1;
    int type = params[ 1 ];
    if ( type != UNIFORM_INT && type != UNIFORM_UINT ) return -1;
    int location = params[ 2 ];
    if ( location == -1 ) return -1;
    return shader_set_param( ( BGD_SHADER_PARAMETERS * ) param, type, location, 0, ( void * )( intptr_t ) params[ 3 ], 0, 0, 0, 0, 0 );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setparam_float( INSTANCE * my, int64_t * params ) {
    void * param = ( void * )( intptr_t ) params[ 0 ];
    if ( !param ) return -1;
    int type = params[ 1 ];
    if ( type != UNIFORM_FLOAT ) return -1;
    int location = params[ 2 ];
    if ( location == -1 ) return -1;
    float value = *( float * ) &params[ 3 ];
    return shader_set_param( ( BGD_SHADER_PARAMETERS * ) param, type, location, 0, ( void * )( intptr_t ) *(( int32_t * )&value ), 0, 0, 0, 0, 0 );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setparam_image( INSTANCE * my, int64_t * params ) {
    void * param = ( void * )( intptr_t ) params[ 0 ];
    if ( !param ) return -1;
    int type = params[ 1 ];
    if ( type != SHADER_IMAGE ) return -1;
    int location = params[ 2 ];
    if ( location == -1 ) return -1;
    GRAPH * map = bitmap_get( params[ 3 ], params[ 4 ] );
    if ( !map ) return -1;
    return shader_set_param( ( BGD_SHADER_PARAMETERS * ) param, type, location, 0, ( void * ) map, params[ 5 ], 0, 0, 0, 0 );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setparam_vector( INSTANCE * my, int64_t * params ) {
    void * param = ( void * )( intptr_t ) params[ 0 ];
    if ( !param ) return -1;
    int type = params[ 1 ];
    if ( type != UNIFORM_INT_ARRAY  && type != UNIFORM_UINT_ARRAY  && type != UNIFORM_FLOAT_ARRAY &&
         type != UNIFORM_INT2_ARRAY && type != UNIFORM_UINT2_ARRAY && type != UNIFORM_FLOAT2_ARRAY &&
         type != UNIFORM_INT3_ARRAY && type != UNIFORM_UINT3_ARRAY && type != UNIFORM_FLOAT3_ARRAY &&
         type != UNIFORM_INT4_ARRAY && type != UNIFORM_UINT4_ARRAY && type != UNIFORM_FLOAT4_ARRAY
        ) return -1;
    int location = params[ 2 ];
    if ( location == -1 ) return -1;
    return shader_set_param( ( BGD_SHADER_PARAMETERS * ) param, type, location, params[ 3 ], ( void * )( intptr_t ) params[ 4 ], 0, 0, 0, 0, 0 );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_setparam_matrix( INSTANCE * my, int64_t * params ) {
    void * param = ( void * )( intptr_t ) params[ 0 ];
    if ( !param ) return -1;
    int type = params[ 1 ];
    if ( type != UNIFORM_MATRIX ) return -1;
    int location = params[ 2 ];
    if ( location == -1 ) return -1;
    return shader_set_param( ( BGD_SHADER_PARAMETERS * ) param, type, location, 0, ( void * )( intptr_t ) params[ 3 ], 0, params[ 4 ], params[ 5 ], params[ 6 ], params[ 7 ] );
}

/* --------------------------------------------------------------------------- */
/* Get functions                                                               */

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_getparam( INSTANCE *my, int64_t *params ) {
    void *param = (void *)(intptr_t)params[0];
    if (!param) return -1;
    int location = params[1];
    if (location == -1) return -1;

    void *values;
    int type;

    if (shader_get_param((BGD_SHADER_PARAMETERS *)param, location, &type, NULL, &values, NULL, NULL, NULL, NULL, NULL) == 0) {
        if ( params[2] ) *(int64_t *)(intptr_t)params[2] = type;
        if ( type != UNIFORM_INT && type != UNIFORM_UINT && type != UNIFORM_FLOAT ) return -1;
        *(void **)(intptr_t)params[3] = values;
        return 0;
    } else {
        return -1;
    }
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_getparam_vector( INSTANCE *my, int64_t *params ) {
    void *param = (void *)(intptr_t)params[0];
    if (!param) return -1;
    int location = params[1];
    if (location == -1) return -1;

    int type, n_values;
    void *values;

    if (shader_get_param((BGD_SHADER_PARAMETERS *)param, location, &type, &n_values, &values, NULL, NULL, NULL, NULL, NULL ) == 0) {
        if ( params[2] ) *(int64_t *)(intptr_t)params[2] = type;
        if ( type != UNIFORM_INT_ARRAY  && type != UNIFORM_UINT_ARRAY  && type != UNIFORM_FLOAT_ARRAY &&
             type != UNIFORM_INT2_ARRAY && type != UNIFORM_UINT2_ARRAY && type != UNIFORM_FLOAT2_ARRAY &&
             type != UNIFORM_INT3_ARRAY && type != UNIFORM_UINT3_ARRAY && type != UNIFORM_FLOAT3_ARRAY &&
             type != UNIFORM_INT4_ARRAY && type != UNIFORM_UINT4_ARRAY && type != UNIFORM_FLOAT4_ARRAY
            ) return -1;
        *(int64_t *)(intptr_t)params[3] = n_values;
        *(void **)(intptr_t)params[4] = values;
        return 0;
    } else {
        return -1;
    }
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_shader_getparam_matrix( INSTANCE *my, int64_t *params ) {
    void *param = (void *)(intptr_t)params[0];
    if (!param) return -1;
    int location = params[1];
    if (location == -1) return -1;

    int type, num_matrices, num_rows, num_columns, transpose;
    void *values;

    if (shader_get_param((BGD_SHADER_PARAMETERS *)param, location, &type, NULL, &values, NULL, &num_matrices, &num_rows, &num_columns, &transpose) == 0) {
        if ( params[2] ) *(int64_t *)(intptr_t)params[2] = type;
        if ( type != UNIFORM_MATRIX ) return -1;
        *(void **)(intptr_t)params[3] = values;
        *(int64_t *)(intptr_t)params[4] = num_matrices;
        *(int64_t *)(intptr_t)params[5] = num_rows;
        *(int64_t *)(intptr_t)params[6] = num_columns;
        *(int64_t *)(intptr_t)params[7] = transpose;
        return 0;
    } else {
        return -1;
    }
}

/* --------------------------------------------------------------------------- */
