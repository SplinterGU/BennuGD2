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

/****************************************************************************/
/* FILE        : xstrings.h                                                 */
/* DESCRIPTION : Strings management. Includes any function related to       */
/*               variable-length strings.                                   */
/****************************************************************************/
/* HISTORY     : 29/01/2001 (jlceb) string_internal no longer used          */
/****************************************************************************/

#ifndef __XSTRINGS_H
#define __XSTRINGS_H

extern void _string_ptoa( char *t, void * p );
extern void _string_ntoa( char *p, uint64_t n );
extern void _string_utoa( char *p, uint64_t n );

extern void         string_init() ;
extern const char * string_get( int64_t code ) ;
extern void         string_dump( void ( *wlog )( const char *fmt, ... ) );
extern void         string_load( void *, int64_t, int64_t, int64_t, int64_t ) ;
extern int64_t      string_new( const char * ptr ) ;
extern int64_t      string_newa( const char * ptr, unsigned count ) ;
extern void         string_use( int64_t code ) ;
extern void         string_discard( int64_t code ) ;
extern int64_t      string_add( int64_t code1, int64_t code2 ) ;
extern int64_t      string_compile( const char ** source ) ;
extern int64_t      string_itoa( int64_t n ) ;
extern int64_t      string_uitoa( int64_t n ) ;
extern int64_t      string_ftoa( double n ) ;
extern int64_t      string_ptoa( void * n ) ;
extern int64_t      string_comp( int64_t code1, int64_t code2 ) ;
extern int64_t      string_casecmp( int64_t code1, int64_t code2 ) ;
extern int64_t      string_char( int64_t n, int nchar ) ;
extern int64_t      string_substr( int64_t code, int first, int len ) ;
extern int64_t      string_find( int64_t code1, int64_t code2, int first ) ;
extern int64_t      string_ucase( int64_t code1 ) ;
extern int64_t      string_lcase( int64_t code1 ) ;
extern int64_t      string_strip( int64_t code ) ;
extern int64_t      string_pad( int64_t code, int length, int align ) ;
extern int64_t      string_format( double number, int dec, char point, char thousands ) ;
extern int64_t      string_concat( int64_t code1, char * str2 ) ;

#endif
