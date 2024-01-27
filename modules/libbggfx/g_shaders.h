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

#define SHADER_LANGUAGE_NONE            GPU_LANGUAGE_NONE
#define SHADER_LANGUAGE_ARB_ASSEMBLY    GPU_LANGUAGE_ARB_ASSEMBLY
#define SHADER_LANGUAGE_GLSL            GPU_LANGUAGE_GLSL
#define SHADER_LANGUAGE_GLSLES          GPU_LANGUAGE_GLSLES
#define SHADER_LANGUAGE_HLSL            GPU_LANGUAGE_HLSL
#define SHADER_LANGUAGE_CG              GPU_LANGUAGE_CG

/* --------------------------------------------------------------------------- */

#ifdef USE_SDL2_GPU
typedef struct {
    uint32_t shader;
    GPU_ShaderBlock block;
} BGD_SHADER;
#else
typedef void BGD_SHADER;
#endif

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

extern int shader_getattributelocation( BGD_SHADER * shader, const char * name );
extern int shader_getuniformlocation( BGD_SHADER * shader, const char * name );
extern void shader_setshaderimage( GRAPH * image, int location, int image_unit );

extern void shader_setattributei( int location, int32_t value );
extern void shader_setattributeiv( int location, int nvalues, int32_t * values );
extern void shader_setattributeui( int location, uint32_t value );
extern void shader_setattributeuiv( int location, int nvalues, uint32_t * values );
extern void shader_setattributef( int location, float value );
extern void shader_setattributefv( int location, int nvalues, float * values );

extern void shader_setuniformi( int location, int32_t value );
extern void shader_setuniformiv( int location, int nvalues, int32_t * values );
extern void shader_setuniform2iv( int location, int nvalues, int32_t * values );
extern void shader_setuniform3iv( int location, int nvalues, int32_t * values );
extern void shader_setuniform4iv( int location, int nvalues, int32_t * values );
extern void shader_setuniformui( int location, uint32_t value );
extern void shader_setuniformuiv( int location, int nvalues, uint32_t * values );
extern void shader_setuniform2uiv( int location, int nvalues, uint32_t * values );
extern void shader_setuniform3uiv( int location, int nvalues, uint32_t * values );
extern void shader_setuniform4uiv( int location, int nvalues, uint32_t * values );
extern void shader_setuniformf( int location, float value );
extern void shader_setuniformfv( int location, int nvalues, float * values );
extern void shader_setuniform2fv( int location, int nvalues, float * values );
extern void shader_setuniform3fv( int location, int nvalues, float * values );
extern void shader_setuniform4fv( int location, int nvalues, float * values );
extern void shader_setuniformmatrix( int location, int num_matrices, int num_rows, int num_columns, int transpose, float * values );

/* --------------------------------------------------------------------------- */

#endif

/* --------------------------------------------------------------------------- */
