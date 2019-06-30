/*
 *  Copyright (C) 2006-2019 SplinterGU (Fenix/BennuGD)
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
#ifdef TARGET_BEOS
#include <posix/assert.h>
#else
#include <assert.h>
#endif

#include "bgdc.h"

/* ---------------------------------------------------------------------- */
/* Gestor de cadenas                                                      */
/* ---------------------------------------------------------------------- */

char * string_mem = NULL;
int64_t string_allocated = 0;
int64_t string_used = 0;

int64_t * string_offset = NULL;
int64_t string_max = 0;
int64_t string_count = 0;

int64_t autoinclude = 0;

void string_init() {
    string_mem = ( char * ) calloc( 4096, sizeof( char ) );
    string_allocated = 4096;
    string_used = 1;
    string_count = 1;
    string_offset = ( int64_t * ) calloc( 1024, sizeof( int64_t ) );
    string_max = 1024;
    string_mem[ 0 ] = 0;
    string_offset[ 0 ] = 0;
}

void string_alloc( int64_t bytes ) {
    string_mem = ( char * ) realloc( string_mem, string_allocated += bytes );
    if ( !string_mem ) {
        fprintf( stdout, "string_alloc: out of memory\n" );
        exit( 1 );
    }
}

void string_dump( void ( *wlog )( const char *fmt, ... ) ) {
    int64_t i;
    printf( "\n---- %" PRId64 " strings ----\n\n", string_count );
    for ( i = 0 ; i < string_count ; i++ )
        printf( "%4" PRId64 ": %s\n", i, string_mem + string_offset[ i ] );
}

int64_t string_new( const char * text ) {
    int64_t len = strlen( text ) + 1, i;

    /* Reuse strings */
    for ( i = 0; i < string_count; i++ ) if ( !strcmp( text, string_mem + string_offset[ i ] ) ) return i;

    if ( string_count == string_max ) {
        string_max += 1024;
        string_offset = ( int64_t * ) realloc( string_offset, string_max * sizeof( int64_t ) );
        if ( string_offset == NULL ) {
            fprintf( stdout, "Too many strings\n" );
            exit( 1 );
        }
    }

    while ( string_used + len >= string_allocated ) string_alloc( 1024 );

    string_offset[ string_count ] = string_used;
    strcpy( string_mem + string_used, text );
    string_used += len;
    return string_count++;
}


char * validchars = "\\/.";
char * invalidchars = \
        "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F" \
        "\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F" \
        "*?\"<>|";

int64_t check_for_valid_pathname( char * pathname ) {
    int n, l;

    if ( !pathname || ( l = strlen( pathname ) ) > __MAX_PATH ) return 0;

#if WIN32
    /* Only ':' with this sintax: "L:..." */
    if ( pathname[ 0 ] == ':' || ( l > 2 && strchr( pathname + 2, ':' ) ) ) return 0;
#endif

    /* Some invalid character? */
    for ( n = 0; n < strlen( invalidchars ); n++ )
        if ( strchr( pathname, invalidchars[ n ] ) ) return 0;

    return 1;
}

int64_t no_include_this_file = 0;

int64_t string_compile( const char ** source ) {
    unsigned char c = *( *source )++, conv, cc;
    const char * ptr;
    int64_t string_used_back = string_used;

    if ( string_count == string_max ) {
        string_max += 1024;
        string_offset = ( int64_t * ) realloc( string_offset, string_max * sizeof( int64_t ) );
        if ( string_offset == NULL ) {
            fprintf( stdout, "Too many strings\n" );
            exit( 1 );
        }
    }

    string_offset[ string_count ] = string_used;

/*
\a  07  Alert (Beep, Bell) (added in C89)[1]
\b  08  Backspace
\e  1B  escape character
\f  0C  Formfeed Page Break
\n  0A  Newline (Line Feed); see notes below
\r  0D  Carriage Return
\t  09  Horizontal Tab
\v  0B  Vertical Tab
\\  5C  Backslash
\'  27  Apostrophe or single quotation mark
\"  22  Double quotation mark
\?  3F  Question mark (used to avoid trigraphs)
\nnn  any The byte whose numerical value is given by nnn interpreted as an octal number
\xhh…   any The byte whose numerical value is given by hh… interpreted as a hexadecimal number
*/

    while ( ( cc = *( *source ) ) ) {
        if ( cc == '\\' ) { // ESCAPE
            cc = *( *source )++;
            switch ( cc = *( *source ) ) {
                case 'a':   cc = '\a'; ( *source )++; break; /* Alert (Beep, Bell) (added in C89)[1] */
                case 'b':   cc = '\b'; ( *source )++; break; /* Backspace */
                case 'e':   cc = '\e'; ( *source )++; break; /* escape character */
                case 'f':   cc = '\f'; ( *source )++; break; /* Formfeed Page Break */
                case 'n':   cc = '\n'; ( *source )++; break; /* Newline (Line Feed); see notes below */
                case 'r':   cc = '\r'; ( *source )++; break; /* Carriage Return */
                case 't':   cc = '\t'; ( *source )++; break; /* Horizontal Tab */
                case 'v':   cc = '\v'; ( *source )++; break; /* Vertical Tab */
                case '\\':  cc = '\\'; ( *source )++; break; /* Backslash */
                case '\'':  cc = '\''; ( *source )++; break; /* Apostrophe or single quotation mark */
                case '"':   cc = '\"'; ( *source )++; break; /* Double quotation mark */
                case '?':   cc = '\?'; ( *source )++; break; /* Question mark (used to avoid trigraphs) */
                case 'x':   {          /*   hexadecimal number */
                    unsigned char cx, n;
                    ( *source )++;
                    for ( cc = 0, n = 2; n && ( cx = *( *source ) ) && ( ( cx >= '0' && cx <= '9' ) || ( cx >= 'a' && cx <= 'f' ) || ( cx >= 'A' && cx <= 'F' ) ); n-- ) {
                        if ( cx >= '0' && cx <= '9' ) cc = cc * 16 + cx - '0';
                        else if ( cx >= 'a' && cx <= 'f' ) cc = cc * 16 + 10 + cx - 'a';
                        else cc = cc * 16 + 10 + cx - 'A';
                        ( *source )++;
                    }
                    break;
                }

                default: { /* Octal or ignore */
                    unsigned char cx, n;
                    for ( cc = 0, n = 3; n && ( cx = *( *source ) ) && cx >= '0' && cx <= '7' ; n-- ) cc = cc * 8 + cx - '0', ( *source ) ++;
                    break;
                }
            }
            if ( cc ) {
#if 1
                conv = convert( cc );
                string_mem[ string_used++ ] = conv;
#else
                string_mem[ string_used++ ] = cc;
#endif
            }
        } else if ( cc == c ) {  /* Termina la string? */
            ( *source )++;
            /* Elimino todos los espacios para buscar si hay otra string, esto es para strings divididas */
            while ( ISSPACE( * ( * source ) ) ) if ( *( * source )++ == '\n' ) line_count++;
            /* Si despues de saltar todos los espacios, no tengo un delimitador de string, salgo */
            if ( *( * source ) != c ) break;
            ( *source )++;
            continue;
        } else if ( cc == '\n' ) {
            line_count++;
            string_mem[ string_used++ ] = '\n';
            ( *source )++;
        } else {
#if 1
                conv = convert( cc );
                string_mem[ string_used++ ] = conv;
#else
                string_mem[ string_used++ ] = cc;
#endif
            ( *source )++;
        }

        if ( string_used >= string_allocated )
            string_alloc( 1024 );
    }

    string_mem[ string_used++ ] = 0;

    int i;

    /* Reuse strings */

    for ( i = 0; i < string_count; i++ ) {
        if ( !strcmp( string_mem + string_used_back, string_mem + string_offset[ i ] ) ) {
            string_used = string_used_back;
            return i;
        }
    }

    if ( string_used >= string_allocated ) string_alloc( 1024 );

    /* Hack: a�ade el posible fichero al DCB */

    if ( !no_include_this_file && autoinclude && check_for_valid_pathname( string_mem + string_offset[ string_count ] ) )
        dcb_add_file( string_mem + string_offset[ string_count ] );

    no_include_this_file = 0;

    return string_count++;
}

const char * string_get( int64_t code ) {
    assert( code < string_count && code >= 0 );
    return string_mem + string_offset[ code ];
}
