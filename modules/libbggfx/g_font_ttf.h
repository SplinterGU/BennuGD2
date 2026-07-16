/*
 * FreeType font support for BennuGD2.
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

int64_t gr_font_ttf_load( const char * filename );
int64_t gr_font_ttf_load_from_memory( const void * data, size_t data_size );
int64_t gr_font_ttf_set_size( int64_t fontid, int64_t pixels );
int64_t gr_font_ttf_is_font( int64_t fontid );
int64_t gr_font_ttf_get_size( int64_t fontid );
int64_t gr_font_ttf_get_kerning( int64_t fontid, uint8_t left, uint8_t right );

const char * gr_font_ttf_get_family( int64_t fontid );
const char * gr_font_ttf_get_style( int64_t fontid );

void gr_font_ttf_forget( int64_t fontid );
void gr_font_ttf_shutdown( void );

#endif
