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

#include "files.h"
#include "xstrings.h"
#include "xctype.h"

#include "libmod_misc.h"

/* STRINGS */

/** LEN (STRING SOURCE)
 *  Returns the size of a string
 */

int64_t libmod_misc_string_strlen( INSTANCE * my, int64_t * params ) {
    const char * str = string_get( params[0] ) ;
    int64_t r = str ? strlen( str ) : 0 ;
    string_discard( params[0] ) ;
    return r ;
}

/** STRING UCASE (STRING SOURCE)
 *  Converts a string to upper-case
 */

int64_t libmod_misc_string_strupper( INSTANCE * my, int64_t * params ) {
    int64_t r = string_ucase( params[0] ) ;
    string_discard( params[0] ) ;
    string_use( r ) ;
    return r ;
}

/** STRING LCASE (STRING SOURCE)
 *  Converts a string to lower-case
 */

int64_t libmod_misc_string_strlower( INSTANCE * my, int64_t * params ) {
    int64_t r = string_lcase( params[0] ) ;
    string_discard( params[0] ) ;
    string_use( r ) ;
    return r ;
}

/** STRCASECMP (STRING S1, STRING S2)
 *  Compares two strings, case-insensitive
 */

int64_t libmod_misc_string_strcasecmp( INSTANCE * my, int64_t * params ) {
    int64_t r = string_casecmp( params[0], params[1] ) ;
    string_discard( params[0] ) ;
    string_discard( params[1] ) ;
    return r ;
}

/** SUBSTR (STRING SOURCE, INT FIRST_CHARACTER, INT COUNT)
 *  Returns part of a given string, starting at the given character position
 *  and returning a string limited to COUNT characters
 */

int64_t libmod_misc_string_substr( INSTANCE * my, int64_t * params ) {
    int64_t r = string_substr( params[0], params[1], params[2] < 0 ? params[2] - 1 : params[2] ) ;
    string_discard( params[0] ) ;
    string_use( r ) ;
    return r ;
}

/** SUBSTR (STRING SOURCE, INT FIRST_CHARACTER)
 *  Returns a substring, from the character given to the end of the source string
 */

int64_t libmod_misc_string_substr2( INSTANCE * my, int64_t * params ) {
    int64_t r = string_substr( params[0], params[1], -1 ) ;
    string_discard( params[0] ) ;
    string_use( r ) ;
    return r ;
}

/** FIND (STRING SOURCE, STRING SUBSTRING)
 *  Searchs a substring in a string, and returns its position
 */

int64_t libmod_misc_string_strfind( INSTANCE * my, int64_t * params ) {
    int64_t r = string_find( params[0], params[1], 0 ) ;
    string_discard( params[0] ) ;
    string_discard( params[1] ) ;
    return r ;
}

/** FIND (STRING SOURCE, STRING SUBSTRING, INT FIRST)
 *  Searchs a substring in a string, starting from the given position, and returns its position
 */

int64_t libmod_misc_string_strfindSSI( INSTANCE * my, int64_t * params ) {
    int64_t r = string_find( params[0], params[1], params[2] ) ;
    string_discard( params[0] ) ;
    string_discard( params[1] ) ;
    return r ;
}

/** RFIND (STRING SOURCE, STRING SUBSTRING)
 *  Searchs a substring in a string, and returns its position
 */

int64_t libmod_misc_string_strrfind( INSTANCE * my, int64_t * params ) {
    int64_t r = string_rfind( params[0], params[1], 0 ) ;
    string_discard( params[0] ) ;
    string_discard( params[1] ) ;
    return r ;
}

/** RFIND (STRING SOURCE, STRING SUBSTRING, INT FIRST)
 *  Searchs a substring in a string, starting from the given position, and returns its position
 */

int64_t libmod_misc_string_strrfindSSI( INSTANCE * my, int64_t * params ) {
    int64_t r = string_rfind( params[0], params[1], params[2] ) ;
    string_discard( params[0] ) ;
    string_discard( params[1] ) ;
    return r ;
}

/** STRTOK (STRING SOURCE, STRING DELIM)
 *  Extract tokens from strings
 */

int64_t libmod_misc_string_tok( INSTANCE * my, int64_t * params ) {
    int64_t r = string_tok( params[0], params[1] ) ;
    string_discard( params[0] ) ;
    string_discard( params[1] ) ;
    string_use( r ) ;
    return r ;
}

/** STRING LPAD (STRING SOURCE, LENGTH)
 *  Expands the string up to the given length, adding spaces at the left
 */

int64_t libmod_misc_string_lpad( INSTANCE * my, int64_t * params ) {
    int64_t r = string_pad( params[0], params[1], 0 );
    string_discard( params[0] );
    string_use( r );
    return r;
}

/** STRING RPAD (STRING SOURCE, LENGTH)
 *  Expands the string up to the given length, adding spaces at the right
 */

int64_t libmod_misc_string_rpad( INSTANCE * my, int64_t * params ) {
    int64_t r = string_pad( params[0], params[1], 1 );
    string_discard( params[0] );
    string_use( r );
    return r;
}

/** ITOA (INT VALUE)
 *  Converts an integer to string
 */

int64_t libmod_misc_string_itos( INSTANCE * my, int64_t * params ) {
    int64_t r = string_itoa( params[0] ) ;
    string_use( r ) ;
    return r ;
}

/** FTOA (FLOAT VALUE)
 *  Converts a floating-point number to string
 */

int64_t libmod_misc_string_ftos( INSTANCE * my, int64_t * params ) {
    int64_t r = string_ftoa( *( double * ) & params[0] ) ;
    string_use( r ) ;
    return r ;
}

/** ATOI (STRING VALUE)
 *  Converts a string to integer
 */

int64_t libmod_misc_string_stoi( INSTANCE * my, int64_t * params ) {
    const char * str = string_get( params[0] ) ;
    int64_t r = str ? atoll( str ) : 0 ;
    string_discard( params[0] ) ;
    return r ;
}

/** ATOF (STRING VALUE)
 *  Converts a string to floating-point number
 */

int64_t libmod_misc_string_stof( INSTANCE * my, int64_t * params ) {
    const char * str = string_get( params[0] ) ;
    double res = ( double )( str ? atof( str ) : 0 );
    string_discard( params[0] ) ;
    return *(( int64_t * )&res ) ;
}

/** ASC(STRING C)
 *  Return the ASCII code of the first character at the string
 */

int64_t libmod_misc_string_asc( INSTANCE * my, int64_t * params ) {
    const unsigned char * str = ( unsigned char * ) string_get( params[0] ) ;
    int64_t r = str ? *str : '\0' ;
    string_discard( params[0] ) ;
    return r ;
}

/** CHR(ASCII)
 *  Returns a string of length 1, with the character of the given ASCII code
 */

int64_t libmod_misc_string_chr( INSTANCE * my, int64_t * params ) {
    unsigned char buffer[2] = " " ;
    int64_t r ;
    buffer[0] = ( unsigned char ) params[0] ;
    r = string_new( ( char * ) buffer ) ;
    string_use( r ) ;
    return r ;
}

/** STRING TRIM(STRING SOURCE)
 *  Returns the given string, stripping any space characters at the beginning or the end
 */

int64_t libmod_misc_string_trim( INSTANCE * my, int64_t * params ) {
    int64_t r = string_strip( params[0] ) ;
    string_discard( params[0] ) ;
    string_use( r ) ;
    return r;
}

#ifndef WIN32
static char * strrev( char * str ) {
    char *start = str;
    char *left = str;

    while ( *str++ )
        ;
    str -= 2;

    while ( left < str ) {
        char ch = *left;
        *left++ = *str;
        *str-- = ch;
    }

    return( start );
}
#endif

/** STRING STRREV (STRING SOURCE)
 *  Returns the reverse of the source string
 */

int64_t libmod_misc_string_strrev( INSTANCE * my, int64_t * params ) {
    int64_t r = string_new( string_get( params[0] ) );
    string_discard( params[0] ) ;
    string_use( r ) ;
    strrev(( char * ) string_get( r ) );
    return r;
}

/** FORMAT (INT VALUE)
 *  Converts a given integer value to string form
 */

int64_t libmod_misc_string_formatI( INSTANCE * my, int64_t * params ) {
    int64_t r = string_format( params[0], 0, '.', ',' );
    string_use( r ) ;
    return r;
}

/** FORMAT (FLOAT VALUE)
 *  Converts a given value to string form
 */

int64_t libmod_misc_string_formatF( INSTANCE * my, int64_t * params ) {
    int64_t r = string_format( *( double * ) &params[0], -1, '.', ',' );
    string_use( r ) ;
    return r;
}

/** FORMAT (INT VALUE, INT DECIMALS)
 *  Converts a given integer value to string form. Uses a fixed number
 *  of decimals, as given with the second parameter.
 */

int64_t libmod_misc_string_formatFI( INSTANCE * my, int64_t * params ) {
    int64_t r = string_format( *( double * ) &params[0], params[1], '.', ',' );
    string_use( r ) ;
    return r;
}

/* ----------------------------------------------------------------- */

/** STRING_NEWA( size )
 *  Create a new string array ("STRING *")
 */

int64_t modstring_string_new_array( INSTANCE * my, int64_t * params )
{
    int64_t * array = calloc( params[0] + 1, sizeof( int64_t ) ) ;

    array[0] = params[0];

    return ( int64_t ) ( array + 1 );
}

/* ----------------------------------------------------------------- */

/** STRING_NEWA()
 *  Create a new empty string array ("STRING *")
 */

int64_t modstring_string_new_array_empty( INSTANCE * my, int64_t * params )
{
    int64_t * array = calloc( 1, sizeof( int64_t ) ) ;
    array[0] = 0;
    return ( int64_t ) ( array + 1 );
}

/* ----------------------------------------------------------------- */

/** STRING_RESIZEA( STRING **, new_size )
 */

int64_t modstring_string_resize_array( INSTANCE * my, int64_t * params )
{
    if ( !params[0] ) return ( int64_t ) NULL;

    int64_t ** presult = ( int64_t ** ) ( params[0] );
    int64_t * current_array = ( *presult ) - 1;
    int64_t current_size = current_array[0];
    int64_t new_size = params[1];

    if ( current_size == new_size ) return new_size;

    if ( current_size > new_size ) {
        for ( int i = new_size + 1; i <= current_size; i++ ) {
            string_discard( current_array[ i ] );
        }
    }

    int64_t * new_array = realloc( current_array, ( new_size + 1 ) * sizeof( int64_t ) ) ;
    if ( !new_array ) return ( int64_t ) current_size ;
    
    if ( current_size < new_size ) memset( new_array + new_size, 0, ( new_size - current_size ) * sizeof( int64_t ) );

    *new_array = new_size;
    *presult = new_array + 1;

    return ( int64_t ) new_size ;
}

/* ----------------------------------------------------------------- */

/** STRING_DELA( STRING ** )
 * Release an allocated string
 */
int64_t modstring_string_delete_array( INSTANCE * my, int64_t * params )
{
    if ( !params[0] ) return ( int64_t ) 0;

    int64_t ** presult = ( int64_t ** ) ( params[0] );
    int64_t * current_array = ( *presult ) - 1;
    int64_t current_size = current_array[0];

    int i;
    for ( i = 1; i <= current_size; i++ ) {
        string_discard( current_array[ i ] );
    }

    free( current_array );

    * presult = NULL;

    return ( int64_t ) 1 ;
}

/* ----------------------------------------------------------------- */

/** STRING_SIZEA( STRING * )
 * Return string array items
 */
int64_t modstring_string_size_array( INSTANCE * my, int64_t * params )
{
    if ( !params[0] ) return ( int64_t ) 0;
    return ( ( int64_t * ) ( params[0] ) )[ -1 ];
}

/* ----------------------------------------------------------------- */

int64_t modstring_string_dump( INSTANCE * my, int64_t * params )
{
    string_dump( printf ) ;
    return 1 ;
}

/* ----------------------------------------------------------------- */

int64_t modstring_string_isalpha( INSTANCE * my, int64_t * params )
{
    return ISALPHA(params[0]) != 0;
}

/* ----------------------------------------------------------------- */

int64_t modstring_string_isspace( INSTANCE * my, int64_t * params )
{
    return ISSPACE(params[0]) != 0;
}

/* ----------------------------------------------------------------- */

int64_t modstring_string_isnum( INSTANCE * my, int64_t * params )
{
    return ISNUM(params[0]) != 0;
}

/* ----------------------------------------------------------------- */

int64_t modstring_string_isxnum( INSTANCE * my, int64_t * params )
{
    return ISXNUM(params[0]) != 0;
}

/* ----------------------------------------------------------------- */

int64_t modstring_string_isalnum( INSTANCE * my, int64_t * params )
{
    return ISALNUM(params[0]) != 0;
}

/* ----------------------------------------------------------------- */

int64_t modstring_string_iswordchar( INSTANCE * my, int64_t * params )
{
    return ISWORDCHAR(params[0]) != 0;
}

/* ----------------------------------------------------------------- */

int64_t modstring_string_iswordfirst( INSTANCE * my, int64_t * params )
{
    return ISWORDFIRST(params[0]) != 0;
}

/* ----------------------------------------------------------------- */

int64_t modstring_string_toupper( INSTANCE * my, int64_t * params )
{
    return TOUPPER(params[0]);
}

/* ----------------------------------------------------------------- */

int64_t modstring_string_tolower( INSTANCE * my, int64_t * params )
{
    return TOLOWER(params[0]);
}

/* ----------------------------------------------------------------- */
