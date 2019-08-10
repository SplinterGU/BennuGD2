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

#ifndef __G_SHADERS_H
#define __G_SHADERS_H

/* --------------------------------------------------------------------------- */

typedef struct {
    uint32_t shader;
    GPU_ShaderBlock block;
} BGD_SHADER;

/* --------------------------------------------------------------------------- */

extern BGD_SHADER * shader_create( char * vertex, char * fragment, const char * position_name, const char * texcoord_name, const char * color_name, const char * modelViewMatrix_name );
extern void shader_free( BGD_SHADER * shader );

extern void shader_activate( BGD_SHADER * shader );
extern void shader_deactivate( void );

extern int shader_getattributelocation( BGD_SHADER * shader, const char * name );
extern int shader_getuniformlocation( BGD_SHADER * shader, const char * name );
extern void shader_setshaderimage( GRAPH * image, int location, int image_unit );

extern void shader_setattributei( int location, int value );
extern void shader_setattributeiv( int location, int nvalues, int * values );
extern void shader_setattributeui( int location, unsigned int value );
extern void shader_setattributeuiv( int location, int nvalues, unsigned int * values );
extern void shader_setattributef( int location, float value );
extern void shader_setattributefv( int location, int nvalues, float * values );

extern void shader_setuniformi( int location, int value );
extern void shader_setuniformiv( int location, int nvalues, int * values );
extern void shader_setuniform2iv( int location, int nvalues, int * values );
extern void shader_setuniform3iv( int location, int nvalues, int * values );
extern void shader_setuniform4iv( int location, int nvalues, int * values );
extern void shader_setuniformui( int location, unsigned int value );
extern void shader_setuniformuiv( int location, int nvalues, unsigned int * values );
extern void shader_setuniform2uiv( int location, int nvalues, unsigned int * values );
extern void shader_setuniform3uiv( int location, int nvalues, unsigned int * values );
extern void shader_setuniform4uiv( int location, int nvalues, unsigned int * values );
extern void shader_setuniformf( int location, float value );
extern void shader_setuniformfv( int location, int nvalues, float * values );
extern void shader_setuniform2fv( int location, int nvalues, float * values );
extern void shader_setuniform3fv( int location, int nvalues, float * values );
extern void shader_setuniform4fv( int location, int nvalues, float * values );
extern void shader_setuniformmatrix( int location, int num_matrices, int num_rows, int num_columns, GPU_bool transpose, float * values );

/* --------------------------------------------------------------------------- */

#endif

/* --------------------------------------------------------------------------- */
