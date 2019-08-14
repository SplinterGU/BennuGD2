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
#include <string.h>

#include "libbggfx.h"

/* --------------------------------------------------------------------------- */

BGD_SHADER * shader_create( char * vertex, char * fragment ) {
#ifdef USE_SDL2_GPU
    uint32_t vertex_shader = 0, frags_shader = 0;
    char * source = NULL, * source2 = NULL;
    int header_size;
    const char * header = "";
    BGD_SHADER * shader = malloc( sizeof( BGD_SHADER ) );

    if ( !shader ) return NULL;

    GPU_Renderer * renderer = GPU_GetCurrentRenderer();

    // Get size from header
    if ( renderer->shader_language == GPU_LANGUAGE_GLSL ) {
        if ( renderer->max_shader_version >= 130 ) header = "#version 130\n";
        else if ( renderer->max_shader_version >= 120 ) header = "#version 120\n";
        else header = "#version 110\n";  // Maybe this is good enough?
    } else if( renderer->shader_language == GPU_LANGUAGE_GLSLES ) header = "#version 100\nprecision mediump int;\nprecision mediump float;\n";

    header_size = strlen( header );

    // Allocate source buffer
    source = ( char * ) malloc( header_size + strlen( vertex ) + 1 );
    if ( !source ) {
        free( shader );
        return NULL;
    }

    // Prepend header
    strcpy( source, header );
    strcat( source, vertex );

    // Compile the shader
    if ( !( vertex_shader = GPU_CompileShader( GPU_VERTEX_SHADER, source ) ) ) {
        printf("ERROR compiling vertex shader: %s\n", GPU_GetShaderMessage());
        free( source );
        free( shader );
        return NULL;
    }

    // Allocate source buffer
    source2 = ( char * ) realloc( source, header_size + strlen( fragment ) + 1 );
    if ( !source2 ) {
        GPU_FreeShader( vertex_shader );
        free( source );
        free( shader );
        return NULL;
    }

    // Prepend header
    strcpy( source2, header );
    strcat( source2, fragment );

    // Compile the shader
    if ( !( frags_shader = GPU_CompileShader( GPU_FRAGMENT_SHADER, source2 ) ) ) {
        printf("ERROR compiling fragment shader: %s\n", GPU_GetShaderMessage());
        GPU_FreeShader( vertex_shader );
        free( source2 );
        free( shader );
        return NULL;
    }

    // Clean up
    free( source2 );

    if ( !( shader->shader = GPU_LinkShaders( vertex_shader, frags_shader ) ) ) {
        printf("ERROR linking shaders: %s\n", GPU_GetShaderMessage());
        GPU_FreeShader( vertex_shader );
        GPU_FreeShader( frags_shader );
        free( shader );
        return NULL;
    }

    shader->block = GPU_LoadShaderBlock( shader->shader, "bgd_Vertex", "bgd_TexCoord", "bgd_Color", "bgd_ModelViewProjectionMatrix" );
    GPU_ActivateShaderProgram( shader->shader, &shader->block );

    return shader;
#else
    return NULL;
#endif
}

/* --------------------------------------------------------------------------- */

void shader_activate( BGD_SHADER * shader ) {
#ifdef USE_SDL2_GPU
    GPU_ActivateShaderProgram( shader ? shader->shader : 0, shader ? &shader->block : NULL );
#endif
}

/* --------------------------------------------------------------------------- */

void shader_deactivate( void ) {
#ifdef USE_SDL2_GPU
    GPU_DeactivateShaderProgram();
#endif
}

/* --------------------------------------------------------------------------- */

int shader_getattributelocation( BGD_SHADER * shader, const char * name ) {
#ifdef USE_SDL2_GPU
    if ( !shader ) return -1;
    return ( int ) GPU_GetAttributeLocation( shader->shader, name );
#else
    return -1;
#endif
}

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

void shader_setshaderimage( GRAPH * image, int location, int image_unit ) {
#ifdef USE_SDL2_GPU
    GPU_SetShaderImage( image->tex, location, image_unit );
#endif
}

/* --------------------------------------------------------------------------- */

void shader_setattributei( int location, int value ) {
#ifdef USE_SDL2_GPU
    GPU_SetAttributei( location, value );
#endif
}

/* --------------------------------------------------------------------------- */

void shader_setattributeiv( int location, int nvalues, int * values ) {
#ifdef USE_SDL2_GPU
    GPU_SetAttributeiv( location, nvalues, values );
#endif
}

/* --------------------------------------------------------------------------- */

void shader_setattributeui( int location, unsigned int value ) {
#ifdef USE_SDL2_GPU
    GPU_SetAttributeui( location, value );
#endif
}

/* --------------------------------------------------------------------------- */

void shader_setattributeuiv( int location, int nvalues, unsigned int * values ) {
#ifdef USE_SDL2_GPU
    GPU_SetAttributeuiv( location, nvalues, values );
#endif
}

/* --------------------------------------------------------------------------- */

void shader_setattributef( int location, float value ) {
#ifdef USE_SDL2_GPU
    GPU_SetAttributef( location, value );
#endif
}

/* --------------------------------------------------------------------------- */

void shader_setattributefv( int location, int nvalues, float * values ) {
#ifdef USE_SDL2_GPU
    GPU_SetAttributefv( location, nvalues, values );
#endif
}

/* --------------------------------------------------------------------------- */

void shader_setuniformi( int location, int value ) {
#ifdef USE_SDL2_GPU
    GPU_SetUniformi( location, value );
#endif
}

/* --------------------------------------------------------------------------- */

void shader_setuniformiv( int location, int nvalues, int * values ) {
#ifdef USE_SDL2_GPU
    GPU_SetUniformiv( location, 1, nvalues, values );
#endif
}

/* --------------------------------------------------------------------------- */

void shader_setuniform2iv( int location, int nvalues, int * values ) {
#ifdef USE_SDL2_GPU
    GPU_SetUniformiv( location, 2, nvalues, values );
#endif
}

/* --------------------------------------------------------------------------- */

void shader_setuniform3iv( int location, int nvalues, int * values ) {
#ifdef USE_SDL2_GPU
    GPU_SetUniformiv( location, 3, nvalues, values );
#endif
}

/* --------------------------------------------------------------------------- */

void shader_setuniform4iv( int location, int nvalues, int * values ) {
#ifdef USE_SDL2_GPU
    GPU_SetUniformiv( location, 4, nvalues, values );
#endif
}

/* --------------------------------------------------------------------------- */

void shader_setuniformui( int location, unsigned int value ) {
#ifdef USE_SDL2_GPU
    GPU_SetUniformui( location, value );
#endif
}

/* --------------------------------------------------------------------------- */

void shader_setuniformuiv( int location, int nvalues, unsigned int * values ) {
#ifdef USE_SDL2_GPU
    GPU_SetUniformuiv( location, 1, nvalues, values );
#endif
}

/* --------------------------------------------------------------------------- */

void shader_setuniform2uiv( int location, int nvalues, unsigned int * values ) {
#ifdef USE_SDL2_GPU
    GPU_SetUniformuiv( location, 2, nvalues, values );
#endif
}

/* --------------------------------------------------------------------------- */

void shader_setuniform3uiv( int location, int nvalues, unsigned int * values ) {
#ifdef USE_SDL2_GPU
    GPU_SetUniformuiv( location, 3, nvalues, values );
#endif
}

/* --------------------------------------------------------------------------- */

void shader_setuniform4uiv( int location, int nvalues, unsigned int * values ) {
#ifdef USE_SDL2_GPU
    GPU_SetUniformuiv( location, 4, nvalues, values );
#endif
}

/* --------------------------------------------------------------------------- */

void shader_setuniformf( int location, float value ) {
#ifdef USE_SDL2_GPU
    GPU_SetUniformf( location, value );
#endif
}

/* --------------------------------------------------------------------------- */

void shader_setuniformfv( int location, int nvalues, float * values ) {
#ifdef USE_SDL2_GPU
    GPU_SetUniformfv( location, 1, nvalues, values );
#endif
}

/* --------------------------------------------------------------------------- */

void shader_setuniform2fv( int location, int nvalues, float * values ) {
#ifdef USE_SDL2_GPU
    GPU_SetUniformfv( location, 2, nvalues, values );
#endif
}

/* --------------------------------------------------------------------------- */

void shader_setuniform3fv( int location, int nvalues, float * values ) {
#ifdef USE_SDL2_GPU
    GPU_SetUniformfv( location, 3, nvalues, values );
#endif
}

/* --------------------------------------------------------------------------- */

void shader_setuniform4fv( int location, int nvalues, float * values ) {
#ifdef USE_SDL2_GPU
    GPU_SetUniformfv( location, 4, nvalues, values );
#endif
}

/* --------------------------------------------------------------------------- */

void shader_setuniformmatrix( int location, int num_matrices, int num_rows, int num_columns, int transpose, float * values ) {
#ifdef USE_SDL2_GPU
    GPU_SetUniformMatrixfv( location, num_matrices, num_rows, num_columns, ( GPU_bool ) transpose, values );
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
