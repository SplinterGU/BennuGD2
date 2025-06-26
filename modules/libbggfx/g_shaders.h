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

#ifndef __G_SHADERS_H
#define __G_SHADERS_H

#if USE_SDL2
#ifndef __ANDROID__
#include <GL/glew.h>
#endif
#include <SDL_opengl.h>
#endif

#ifdef USE_SDL2_GPU
#define SHADER_LANGUAGE_NONE            GPU_LANGUAGE_NONE
#define SHADER_LANGUAGE_ARB_ASSEMBLY    GPU_LANGUAGE_ARB_ASSEMBLY
#define SHADER_LANGUAGE_GLSL            GPU_LANGUAGE_GLSL
#define SHADER_LANGUAGE_GLSLES          GPU_LANGUAGE_GLSLES
#define SHADER_LANGUAGE_HLSL            GPU_LANGUAGE_HLSL
#define SHADER_LANGUAGE_CG              GPU_LANGUAGE_CG
#else
#define SHADER_LANGUAGE_NONE            -1
#define SHADER_LANGUAGE_ARB_ASSEMBLY    -1
#define SHADER_LANGUAGE_GLSL            -1
#define SHADER_LANGUAGE_GLSLES          -1
#define SHADER_LANGUAGE_HLSL            -1
#define SHADER_LANGUAGE_CG              -1
#endif


// Define los tipos de atributo y uniforme
enum {
    SHADER_IMAGE,
#if 0
    ATTRIBUTE_INT,
    ATTRIBUTE_INT_ARRAY,
    ATTRIBUTE_UINT,
    ATTRIBUTE_UINT_ARRAY,
    ATTRIBUTE_FLOAT,
    ATTRIBUTE_FLOAT_ARRAY,
#endif
    UNIFORM_INT,
    UNIFORM_INT_ARRAY,
    UNIFORM_INT2_ARRAY,
    UNIFORM_INT3_ARRAY,
    UNIFORM_INT4_ARRAY,
    UNIFORM_UINT,
    UNIFORM_UINT_ARRAY,
    UNIFORM_UINT2_ARRAY,
    UNIFORM_UINT3_ARRAY,
    UNIFORM_UINT4_ARRAY,
    UNIFORM_FLOAT,
    UNIFORM_FLOAT_ARRAY,
    UNIFORM_FLOAT2_ARRAY,
    UNIFORM_FLOAT3_ARRAY,
    UNIFORM_FLOAT4_ARRAY,
    UNIFORM_MATRIX
};

/* --------------------------------------------------------------------------- */

typedef struct {
#ifdef USE_SDL2_GPU
    uint32_t shader;
    GPU_ShaderBlock block;
#elif 0 // USE_SDL2
    GLuint shader;
#endif
} BGD_SHADER;

typedef struct {
    int type;
    int location;

    /* for shader_image begin */
    int image_unit;
    /* for shader_image end */

    /* for matrix begin */
    int num_matrices;
    int num_rows;
    int num_columns;
    int transpose;
    /* for matrix end */

    int n_values;
    union {
        int32_t value;
        uint32_t uvalue;
        float fvalue;
        void * values;
    };
} BGD_SHADER_PARAM;

typedef struct {
    uint32_t nparams;
    BGD_SHADER_PARAM *params;
} BGD_SHADER_PARAMETERS;

/* --------------------------------------------------------------------------- */

extern BGD_SHADER * g_current_shader;

/* --------------------------------------------------------------------------- */

extern int shader_get_language();
extern int shader_get_min_version();
extern int shader_get_max_version();

extern BGD_SHADER * shader_create( char * vertex, char * fragment );
extern void shader_free( BGD_SHADER * shader );

extern void shader_activate( BGD_SHADER * shader );
extern void shader_deactivate( void );

#if 0
extern int shader_getattributelocation( BGD_SHADER * shader, const char * name );
#endif

extern int shader_getuniformlocation( BGD_SHADER * shader, const char * name );

extern int shader_set_param( BGD_SHADER_PARAMETERS * params, int type, int location, int n_values, void *values, int image_unit, int num_matrices, int num_rows, int num_columns, int transpose );
extern int shader_get_param( BGD_SHADER_PARAMETERS * params, int location, int *type, int *n_values, void **values, int *image_unit, int *num_matrices, int *num_rows, int *num_columns, int *transpose);
extern BGD_SHADER_PARAMETERS * shader_create_parameters( int num_params );
extern void shader_free_parameters( BGD_SHADER_PARAMETERS* params );
extern void shader_apply_parameters( BGD_SHADER_PARAMETERS* params );

/* --------------------------------------------------------------------------- */

#endif

/* --------------------------------------------------------------------------- */
