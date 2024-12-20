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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xctype.h"
#include "fmath.h"

/* Character conversion table from cp850 (MS-DOS) to iso8859-1 (isolatin-1/Windows) */

unsigned char cp850_to_iso8859_1[256] = {
      0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
     16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
     32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
     48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
     64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
     80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,
     96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
    112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
    199, 252, 233, 226, 228, 224, 229, 231, 234, 235, 232, 239, 238, 236, 196, 197,
    201, 230, 198, 244, 246, 242, 251, 249, 255, 214, 220, 248, 163, 216, 215, 131,
    225, 237, 243, 250, 241, 209, 170, 186, 191, 174, 172, 189, 188, 161, 171, 187,
    166, 166, 166, 166, 166, 193, 194, 192, 169, 166, 166,  43,  43, 162, 165,  43,
     43,  45,  45,  43,  45,  43, 227, 195,  43,  43,  45,  45, 166,  45,  43, 164,
    240, 208, 202, 203, 200, 105, 205, 206, 207,  43,  43, 166,  95, 166, 204, 175,
    211, 223, 212, 210, 245, 213, 181, 254, 222, 218, 219, 217, 253, 221, 175, 180,
    173, 177,  61, 190, 182, 167, 247, 184, 176, 168, 183, 185, 179, 178, 166, 160
};

/* Character conversion table from iso8859-1 (isolatin-1/Windows) to cp850 (MS-DOS) */

unsigned char iso8859_1_to_cp850[256] =
{
      0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
     16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
     32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
     48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
     64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
     80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,
     96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
    112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
    128, 129, 130, 159, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
    144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
    255, 173, 189, 156, 207, 190, 254, 245, 249, 184, 166, 174, 170, 240, 169, 238,
    248, 241, 253, 252, 239, 230, 244, 250, 247, 251, 167, 175, 172, 171, 243, 168,
    183, 181, 182, 199, 142, 143, 146, 128, 212, 144, 210, 211, 222, 214, 215, 216,
    209, 165, 227, 224, 226, 229, 153, 158, 157, 235, 233, 234, 154, 237, 232, 225,
    133, 160, 131, 198, 132, 134, 145, 135, 138, 130, 136, 137, 141, 161, 140, 139,
    208, 164, 149, 162, 147, 228, 148, 246, 155, 151, 163, 150, 129, 236, 231, 152
};

/* Character types */

char c_type[256];
unsigned char c_upper[256];
unsigned char c_lower[256];

int  c_type_initialized = 0;

unsigned char convert( unsigned char c ) {
    return cp850_to_iso8859_1[c];
}

int utf8_to_iso8859_1( unsigned char * in, int inlen, unsigned char * out, int outlen ) {
    unsigned char c;
    int ix, ox, is_binary = 0;

    ox = ix = 0;

    while ( ix < inlen && ox < outlen ) {
        int trailing;
        unsigned char d = in[ix++];
        
        trailing = 0;

             if ( d < 0x80 ) { c = d; }
        else if ( d < 0xC0 ) { is_binary = 1; break; }
        else if ( d < 0xE0 ) { c = d & 0x1F; trailing = 1; }
        else if ( d < 0xF0 ) { c = d & 0x0F; trailing = 2; }
        else if ( d < 0xF8 ) { c = d & 0x07; trailing = 3; }
        else                 { is_binary = 1; break; }

        for (; trailing; trailing-- ) {
            if ( ( ( d = in[ix++] ) & 0xC0 ) != 0x80 ) { is_binary = 1; break; }
            c <<= 6;
            c |= d & 0x3F;
        }

        out[ox++] = c;
    }

    if ( is_binary ) {
        memmove( out, in, MIN( outlen, inlen ) );
        out[outlen] = '\0';
        return outlen;
    }
    
    out[ox] = '\0';
    return ox;
}

static void set_c_range( int first, int last, int type ) {
    while ( first <= last ) c_type[first++] |= type;
}

static void set_c_from( const unsigned char * chars, int type ) {
    unsigned char * cc = malloc( strlen( chars ) ),
                  * ccp = cc;
    utf8_to_iso8859_1( ( unsigned char * ) chars, strlen(chars), cc, strlen(chars) );
    while ( *cc ) c_type[*cc++] |= type;
    free(ccp);
}

static void set_c_as( int prev_type, int type ) {
    int n;
    for ( n = 0; n < 256; n++ ) if ( c_type[n] & prev_type ) c_type[n] |= type;
}

static void set_c_upper( const unsigned char * from, const unsigned char * to ) {
    unsigned char * f = malloc( strlen( from ) + 1 ),
                  * t = malloc( strlen( to ) + 1 ),
                  * fp = f, *tp = t;
    f[ strlen( from ) ] = '\0';
    t[ strlen( to ) ] = '\0';
    utf8_to_iso8859_1( ( unsigned char * ) from, strlen(from), f, strlen(from) );
    utf8_to_iso8859_1( ( unsigned char * ) to, strlen(to), t, strlen(to) );
    while ( *f ) c_upper[*f++] = *t++;
    free(fp);
    free(tp);
}

static void set_c_lower( const unsigned char * from, const unsigned char * to ) {
    unsigned char * f = malloc( strlen( from ) + 1 ),
                  * t = malloc( strlen( to ) + 1 ),
                  * fp = f, *tp = t;
    f[ strlen( from ) ] = '\0';
    t[ strlen( to ) ] = '\0';
    utf8_to_iso8859_1( ( unsigned char * ) from, strlen(from), f, strlen(from) );
    utf8_to_iso8859_1( ( unsigned char * ) to, strlen(to), t, strlen(to) );
    while ( *f ) c_lower[*f++] = *t++;
    free(fp);
    free(tp);
}

void init_c_type()
{
    int c;

    memset ( c_type, 0, sizeof( c_type ) );

    for ( c = 0; c < 256; c++ ) c_lower[c] = c_upper[c] = c;

    set_c_upper  ( ( const unsigned char * )"abcdefghijklmnopqrstuvwxyz",
                   ( const unsigned char * )"ABCDEFGHIJKLMNOPQRSTUVWXYZ" );
    set_c_lower  ( ( const unsigned char * )"ABCDEFGHIJKLMNOPQRSTUVWXYZ",
                   ( const unsigned char * )"abcdefghijklmnopqrstuvwxyz" );
#ifdef IGNORE_ACCENT_MARK
    set_c_upper  ( ( const unsigned char * )"áéíóúñÑçÇàèìòùäëïöüýâêîôûæãåõÁÉÍÓÚ",
                   ( const unsigned char * )"AEIOUNNCCAEIOUAEIOUYAEIOUÆAAOAEIOU" );
    set_c_lower  ( ( const unsigned char * )"áéíóúñÑçÇàèìòùäëïöüýâêîôûæãåõÁÉÍÓÚ",
                   ( const unsigned char * )"aeiounnccaeiouaeiouyaeiouÆaaoaeiou" );
#else
    set_c_upper  ( ( const unsigned char * )"áéíóúñçàèìòùäëïöüýâêîôûæãåõ",
                   ( const unsigned char * )"ÁÉÍÓÚÑÇÀÈÌÒÙÄËÏÖÜÝÂÊÎÔÛÆÃÅÕ" );
    set_c_lower  ( ( const unsigned char * )"ÁÉÍÓÚÑÇÀÈÌÒÙÄËÏÖÜÝÂÊÎÔÛÆÃÅÕ",
                   ( const unsigned char * )"áéíóúñçàèìòùäëïöüýâêîôûæãåõ" );
#endif

    set_c_from  ( ( const unsigned char * )" \t\n\r", CTYPE_SPACE );

    set_c_range ( '0', '9', CTYPE_NUM );

    set_c_range ( '0', '9', CTYPE_XNUM );
    set_c_range ( 'A', 'F', CTYPE_XNUM );
    set_c_range ( 'a', 'f', CTYPE_XNUM );

    set_c_range ( 'A', 'Z', CTYPE_ALPHA );
    set_c_range ( 'a', 'z', CTYPE_ALPHA );
    set_c_from  ( ( const unsigned char * )"áéíóúÁÉÍÓÚñÑçÇàèìòùÀÈÌÒÙäëïöüÄËÏÖÜÿýâêîôûÂÊÎÔÛæÆãåðõªºÃÅÐÕØ", CTYPE_ALPHA );

    set_c_as    ( CTYPE_ALPHA, CTYPE_ALNUM );
    set_c_as    ( CTYPE_NUM, CTYPE_ALNUM );

    set_c_as    ( CTYPE_ALPHA, CTYPE_WORDCHAR );
    set_c_from  ( ( const unsigned char * )"_", CTYPE_WORDCHAR );
    set_c_as    ( CTYPE_WORDCHAR, CTYPE_WORDFIRST );

    set_c_as    ( CTYPE_NUM, CTYPE_WORDCHAR );
//    set_c_range ( '0', '9', CTYPE_WORDCHAR );

    c_type_initialized = 1;
}
