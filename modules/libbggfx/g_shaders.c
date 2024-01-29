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
#include <string.h>

#include "libbggfx.h"
#include "files.h"

/* --------------------------------------------------------------------------- */

BGD_SHADER * g_current_shader = NULL;

/*
    sample headers:

    GPU_LANGUAGE_GLSL

        #version 130
        #version 120
        #version 110

    GPU_LANGUAGE_GLSLES

        #version 300 es
        precision mediump int
        precision mediump float;

*/

int shader_get_language() {
#ifdef USE_SDL2_GPU
    GPU_Renderer * renderer = GPU_GetCurrentRenderer();
    if ( !renderer ) return -1;
    return renderer->shader_language;
#endif
}

int shader_get_min_version() {
#ifdef USE_SDL2_GPU
    GPU_Renderer * renderer = GPU_GetCurrentRenderer();
    if ( !renderer ) return -1;
    return renderer->min_shader_version;
#endif
}

int shader_get_max_version() {
#ifdef USE_SDL2_GPU
    GPU_Renderer * renderer = GPU_GetCurrentRenderer();
    if ( !renderer ) return -1;
    return renderer->max_shader_version;
#endif
}

/* --------------------------------------------------------------------------- */

BGD_SHADER * shader_create( char * vertex, char * fragment ) {
#ifdef USE_SDL2_GPU
    uint32_t vertex_shader = 0, frags_shader = 0;

    GPU_Renderer * renderer = GPU_GetCurrentRenderer();
    if ( !renderer ) {
        return NULL;
    }

    BGD_SHADER * shader = malloc( sizeof( BGD_SHADER ) );
    if ( !shader ) return NULL;

    // Compile the shader
    if ( !( vertex_shader = GPU_CompileShader( GPU_VERTEX_SHADER, vertex ) ) ) {
        printf("ERROR compiling vertex shader: %s\n", GPU_GetShaderMessage());
        free( shader );
        return NULL;
    }

    // Compile the shader
    if ( !( frags_shader = GPU_CompileShader( GPU_FRAGMENT_SHADER, fragment ) ) ) {
        printf("ERROR compiling fragment shader: %s\n", GPU_GetShaderMessage());
        GPU_FreeShader( vertex_shader );
        free( shader );
        return NULL;
    }

    if ( !( shader->shader = GPU_LinkShaders( vertex_shader, frags_shader ) ) ) {
        printf("ERROR linking shaders: %s\n", GPU_GetShaderMessage());
        GPU_FreeShader( vertex_shader );
        GPU_FreeShader( frags_shader );
        free( shader );
        return NULL;
    }

    shader->block = GPU_LoadShaderBlock( shader->shader, "bgd_Vertex", "bgd_TexCoord", "bgd_Color", "bgd_ModelViewProjectionMatrix" );

//    GPU_ActivateShaderProgram( shader->shader, &shader->block );

    return shader;
#else
    return NULL;
#endif
}

/* --------------------------------------------------------------------------- */

void shader_activate( BGD_SHADER * shader ) {
#ifdef USE_SDL2_GPU
    if ( !shader && g_current_shader ) {
        shader_deactivate();
    } else if ( g_current_shader != shader ) {
        GPU_ActivateShaderProgram( shader ? shader->shader : 0, shader ? &shader->block : NULL );
        g_current_shader = shader;
    }
#endif
}

/* --------------------------------------------------------------------------- */

void shader_deactivate( void ) {
#ifdef USE_SDL2_GPU
    GPU_DeactivateShaderProgram();
    g_current_shader = NULL;
#endif
}

/* --------------------------------------------------------------------------- */

int shader_set_param(   BGD_SHADER_PARAMETERS * params,
                        int type,
                        int location,
                        int n_values,
                        void *values,
                        int image_unit,
                        int num_matrices,
                        int num_rows,
                        int num_columns,
                        int transpose )
{
    BGD_SHADER_PARAM *param = NULL;

    for ( int i = 0; i < params->nparams; ++i ) {
        if ( !param && params->params[ i ].location == -1 ) param = &params->params[ i ];
        if ( params->params[ i ].location == location ) {
            param = &params->params[ i ];
            break;
        }
    }

    if ( !param ) return 1;

    param->type = type;
    param->location = location;
    param->n_values = n_values;
    param->values = values;

    // shader_image
    param->image_unit = image_unit;

    // matrix
    param->num_matrices = num_matrices;
    param->num_rows = num_rows;
    param->num_columns = num_columns;
    param->transpose = transpose;

    return 0;
}

/* --------------------------------------------------------------------------- */

// Create and fill a BGD_SHADER_PARAMETERS structure
BGD_SHADER_PARAMETERS * shader_create_parameters( int num_params ) {
    BGD_SHADER_PARAMETERS* params = (BGD_SHADER_PARAMETERS*)malloc(sizeof(BGD_SHADER_PARAMETERS));
    if (!params) {
        // Handle memory allocation error
        return NULL;
    }

    params->nparams = num_params;
    params->params = (BGD_SHADER_PARAM*)calloc(num_params, sizeof(BGD_SHADER_PARAM));
    if (!params->params) {
        // Handle memory allocation error
        free(params);
        return NULL;
    }

    for ( int i = 0; i < num_params; ++i ) params->params[ i ].type = params->params[ i ].location = -1;

    return params;
}

/* --------------------------------------------------------------------------- */

// Free the memory allocated for BGD_SHADER_PARAMETERS
void shader_free_parameters( BGD_SHADER_PARAMETERS* params ) {
    if (params) {
        free(params->params);
        free(params);
    }
}

/* --------------------------------------------------------------------------- */

#if 0
int shader_getattributelocation( BGD_SHADER * shader, const char * name ) {
#ifdef USE_SDL2_GPU
    if ( !shader ) return -1;
    return ( int ) GPU_GetAttributeLocation( shader->shader, name );
#else
    return -1;
#endif
}
#endif

/* --------------------------------------------------------------------------- */

int shader_getuniformlocation( BGD_SHADER * shader, const char * name ) {
#ifdef USE_SDL2_GPU
    if ( !shader ) return -1;
    return ( int ) GPU_GetUniformLocation( shader->shader, name );
#else
    return -1;
#endif
}

/* --------------------------------------------------------------------------- */

void shader_free( BGD_SHADER * shader ) {
#ifdef USE_SDL2_GPU
    if ( !shader ) return;
    GPU_FreeShaderProgram( shader->shader );
    free( shader );
#endif
}

/* --------------------------------------------------------------------------- */

// Apply settings using BGD_SHADER_PARAMETERS
void shader_apply_parameters( BGD_SHADER_PARAMETERS* params ) {
#ifdef USE_SDL2_GPU
    // Handle case of null parameters
    if (!params) return;

    for (uint32_t i = 0; i < params->nparams; ++i) {
        BGD_SHADER_PARAM *param = &params->params[i];

        switch (param->type) {
            case SHADER_IMAGE:
                GPU_SetShaderImage( ( ( GRAPH * ) ( param->values ))->tex, param->location, param->image_unit );
                break;

#if 0
            case ATTRIBUTE_INT:
                GPU_SetAttributei( param->location, ( int32_t ) param->value );
                break;

            case ATTRIBUTE_INT_ARRAY:
                GPU_SetAttributeiv( param->location, param->nvalues, ( int32_t * ) param->values );
                break;

            case ATTRIBUTE_UINT:
                GPU_SetAttributeui( param->location, ( uint32_t ) param->value );
                break;

            case ATTRIBUTE_UINT_ARRAY:
                GPU_SetAttributeuiv( param->location, param->n_values, ( uint32_t * ) param->values );
                break;

            case ATTRIBUTE_FLOAT:
                GPU_SetAttributef( location, *( float * ) &param->values );
                break;

            case ATTRIBUTE_FLOAT_ARRAY:
                GPU_SetAttributefv( param->location, param->n_values, ( float * )param->values );
                break;
#endif
            case UNIFORM_INT:
                GPU_SetUniformi( param->location, param->value );
                break;

            case UNIFORM_INT_ARRAY:
                GPU_SetUniformiv( param->location, 1, param->n_values, ( int32_t * ) param->values );
                break;

            case UNIFORM_INT2_ARRAY:
                GPU_SetUniformiv( param->location, 2, param->n_values, ( int32_t * ) param->values );
                break;

            case UNIFORM_INT3_ARRAY:
                GPU_SetUniformiv( param->location, 3, param->n_values, ( int32_t * ) param->values );
                break;

            case UNIFORM_INT4_ARRAY:
                GPU_SetUniformiv( param->location, 4, param->n_values, ( int32_t * ) param->values );
                break;

            case UNIFORM_UINT:
                GPU_SetUniformui( param->location, param->uvalue );
                break;

            case UNIFORM_UINT_ARRAY:
                GPU_SetUniformuiv( param->location, 1, param->n_values, ( uint32_t * ) param->values );
                break;

            case UNIFORM_UINT2_ARRAY:
                GPU_SetUniformuiv( param->location, 2, param->n_values, ( uint32_t * ) param->values );
                break;

            case UNIFORM_UINT3_ARRAY:
                GPU_SetUniformuiv( param->location, 3, param->n_values, ( uint32_t * ) param->values );
                break;

            case UNIFORM_UINT4_ARRAY:
                GPU_SetUniformuiv( param->location, 4, param->n_values, ( uint32_t * ) param->values );
                break;

            case UNIFORM_FLOAT:
                GPU_SetUniformf( param->location, param->fvalue );
                break;

            case UNIFORM_FLOAT_ARRAY:
                GPU_SetUniformfv( param->location, 1, param->n_values, ( float * ) param->values );
                break;

            case UNIFORM_FLOAT2_ARRAY:
                GPU_SetUniformfv( param->location, 2, param->n_values, ( float * ) param->values );
                break;

            case UNIFORM_FLOAT3_ARRAY:
                GPU_SetUniformfv( param->location, 3, param->n_values, ( float * ) param->values );
                break;

            case UNIFORM_FLOAT4_ARRAY:
                GPU_SetUniformfv( param->location, 4, param->n_values, ( float * ) param->values );
                break;

            case UNIFORM_MATRIX:
                GPU_SetUniformMatrixfv( param->location, param->num_matrices, param->num_rows, param->num_columns, ( GPU_bool ) param->transpose, param->values );
                break;

            default:
                // Handle invalid parameter type
                break;
        }
    }
#endif
}

/* --------------------------------------------------------------------------- */
