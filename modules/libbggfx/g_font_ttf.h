/*
 * FreeType UTF-8 font support for BennuGD2.
 *
 * This is an altered/additional source file for Bennu Game Development.
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * Permission is granted to use, alter, and redistribute it for any purpose,
 * subject to the BennuGD2 source distribution's license terms.
 */

#ifndef __G_FONT_TTF_H
#define __G_FONT_TTF_H

#include <stddef.h>
#include <stdint.h>

#include "g_bitmap.h"

typedef struct {
    GRAPH * glymap;
    int64_t xoffset;
    int64_t yoffset;
    int64_t xadvance;
    int64_t yadvance;
} TTF_GLYPH_INFO;

int64_t gr_font_ttf_load( const char * filename );
int64_t gr_font_ttf_load_from_memory( const void * data, size_t data_size );
int64_t gr_font_ttf_set_size( int64_t fontid, int64_t pixels );
int64_t gr_font_ttf_is_font( int64_t fontid );
int64_t gr_font_ttf_get_size( int64_t fontid );
int64_t gr_font_ttf_get_line_height( int64_t fontid );
int64_t gr_font_ttf_get_kerning( int64_t fontid, uint32_t left, uint32_t right );
int gr_font_ttf_get_glyph( int64_t fontid, uint32_t codepoint, TTF_GLYPH_INFO * glyph );

/* Decode one UTF-8 scalar value and advance *text. Invalid input consumes one
 * byte and returns U+FFFD. A NUL terminator returns zero without advancing. */
uint32_t gr_font_ttf_utf8_next( const unsigned char ** text );

const char * gr_font_ttf_get_family( int64_t fontid );
const char * gr_font_ttf_get_style( int64_t fontid );

void gr_font_ttf_forget( int64_t fontid );
void gr_font_ttf_shutdown( void );

#endif
