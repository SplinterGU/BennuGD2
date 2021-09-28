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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bgddl.h"

#include "arrange.h"
#include "files.h"
#include "xstrings.h"
#include "dcb.h"
#include "varspace_file.h"

#include "libmod_misc.h"

/* ----------------------------------------------------------------- */

int64_t libmod_misc_file_save( INSTANCE * my, int64_t * params ) {
    file * fp;
    const char * filename;
    int64_t result = 0;

    filename = string_get( params[0] );
    if ( !filename ) return 0;

    fp = file_open( filename, "wb0" );
    if ( fp ) {
        result = savetypes( fp, ( void * )( intptr_t )params[1], ( void * )( intptr_t )params[2], params[3], 0 );
        file_close( fp );
    }
    string_discard( params[0] );
    return result;
}

int64_t libmod_misc_file_load( INSTANCE * my, int64_t * params ) {
    file * fp;
    const char * filename;
    int64_t result = 0;

    filename = string_get( params[0] );
    if ( !filename ) return 0;

    fp = file_open( filename, "rb0" );
    if ( fp ) {
        result = loadtypes( fp, ( void * )( intptr_t )params[1], ( void * )( intptr_t )params[2], params[3], 0 );
        file_close( fp );
    }
    string_discard( params[0] );
    return result;
}

int64_t libmod_misc_file_fopen( INSTANCE * my, int64_t * params ) {
    static char * ops[] = { "rb0", "r+b0", "wb0", "rb", "wb6" };
    int64_t r;

    if ( params[1] < 0 || params[1] > 4 )
        params[0] = 0;

    r = ( int64_t ) ( intptr_t ) file_open( string_get( params[0] ), ops[params[1]] );
    string_discard( params[0] );
    return r;
}

int64_t libmod_misc_file_fclose( INSTANCE * my, int64_t * params ) {
    file_close(( file * )( intptr_t )params[0] );
    return 1;
}

int64_t libmod_misc_file_fread( INSTANCE * my, int64_t * params ) {
    return loadtypes(( file * )( intptr_t )params[0], ( void * )( intptr_t )params[1], ( void * )( intptr_t )params[2], params[3], 0 );
}

int64_t libmod_misc_file_fwrite( INSTANCE * my, int64_t * params ) {
    return savetypes(( file * )( intptr_t )params[0], ( void * )( intptr_t )params[1], ( void * )( intptr_t )params[2], params[3], 0 );
}

int64_t libmod_misc_file_freadC( INSTANCE * my, int64_t * params ) {
    return file_read(( file * )( intptr_t )params[2], ( void * )( intptr_t )params[0], params[1] );
}

int64_t libmod_misc_file_fwriteC( INSTANCE * my, int64_t * params ) {
    return file_write(( file * )( intptr_t )params[2], ( void * )( intptr_t )params[0], params[1] );
}

int64_t libmod_misc_file_fseek( INSTANCE * my, int64_t * params ) {
    return file_seek(( file * )( intptr_t )params[0], params[1], params[2] );
}

int64_t libmod_misc_file_frewind( INSTANCE * my, int64_t * params ) {
    file_rewind(( file * )( intptr_t )params[0] );
    return 1;
}

int64_t libmod_misc_file_ftell( INSTANCE * my, int64_t * params ) {
    return file_pos(( file * )( intptr_t )params[0] );
}

int64_t libmod_misc_file_fflush( INSTANCE * my, int64_t * params ) {
    return file_flush(( file * )( intptr_t )params[0] );
}

int64_t libmod_misc_file_filelength( INSTANCE * my, int64_t * params ) {
    return file_size(( file * )( intptr_t )params[0] );
}

int64_t libmod_misc_file_fputs( INSTANCE * my, int64_t * params ) {
    char * str = ( char * ) string_get( params[1] );
    int64_t r = file_puts(( file * )( intptr_t )params[0], str );
    if ( str[strlen( str )-1] != '\n' ) file_puts(( file * )( intptr_t )params[0], "\r\n" );
    /*    int64_t r = file_puts ((file *)params[0], string_get(params[1])); */
    string_discard( params[1] );
    return r;
}

int64_t libmod_misc_file_fgets( INSTANCE * my, int64_t * params ) {
    char buffer[1025];
    int64_t str = string_new( "" ), done = 0;

    while ( !done ) {
        int64_t len = file_gets(( file * )( intptr_t )params[0], buffer, sizeof( buffer ) - 1);
        if ( len < 1 ) break;

        if ( buffer[len-1] == '\r' || buffer[len-1] == '\n' ) {
            len--;
            if ( len && ( buffer[len-1] == '\r' || buffer[len-1] == '\n' )) len--;
            buffer[len] = '\0';
            done = 1;
        }
        string_concat( str, buffer );
    }
    string_use( str );
    return str;
}

int64_t libmod_misc_file_file( INSTANCE * my, int64_t * params ) {
    int64_t str = string_new( "" );
    file * f;

    f = file_open( string_get( params[0] ), "rb" );
    string_discard( params[0] );

    if ( f ) {
        char buffer[1025];
        while ( !file_eof( f ) ) {
            int64_t l;
            l = file_read( f, buffer, sizeof( buffer ) - 1 );
            buffer[l] = '\0';
            if ( l ) {
                string_concat( str, buffer );
                buffer[0] = '\0';
            } else {
                break;
            }
        }
        file_close( f );
    }

    string_use( str );

    return str;
}

int64_t libmod_misc_file_feof( INSTANCE * my, int64_t * params ) {
    return file_eof(( file * )( intptr_t )params[0] );
}

int64_t libmod_misc_file_exists( INSTANCE * my, int64_t * params ) {
    int64_t r = file_exists( string_get( params[0] ) );
    string_discard( params[0] );
    return r;
}

int64_t libmod_misc_file_remove( INSTANCE * my, int64_t * params ) {
    int64_t r = file_remove( string_get( params[0] ) );
    string_discard( params[0] );
    return r;
}

int64_t libmod_misc_file_move( INSTANCE * my, int64_t * params ) {
    int64_t r = file_move( string_get( params[0] ), string_get( params[1] ) );
    string_discard( params[1] );
    string_discard( params[0] );
    return r;
}

/* ----------------------------------------------------------------- */
