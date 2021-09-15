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

#ifndef __TEXT_H
#define __TEXT_H

/* --------------------------------------------------------------------------- */

#ifndef __BGDC__
#include "g_bitmap.h"
#include "g_regions.h"
#include "g_font.h"
#endif

/* --------------------------------------------------------------------------- */

#define MAX_TEXTS           1024

// Tipos para gr_text_new_var

#define TEXT_TEXT           1
#define TEXT_STRING         2
#define TEXT_INT            3
#define TEXT_QWORD          4
#define TEXT_INT32          5
#define TEXT_DWORD          6
#define TEXT_SHORT          7
#define TEXT_WORD           8
#define TEXT_SBYTE          9
#define TEXT_BYTE           10
#define TEXT_CHAR           11
#define TEXT_DOUBLE         12
#define TEXT_FLOAT          13
#define TEXT_POINTER        14
#define TEXT_CHARARRAY      15

/* Alineacion para textos */
#define ALIGN_TOP_LEFT      0
#define ALIGN_TOP           1
#define ALIGN_TOP_RIGHT     2
#define ALIGN_CENTER_LEFT   3
#define ALIGN_CENTER        4
#define ALIGN_CENTER_RIGHT  5
#define ALIGN_BOTTOM_LEFT   6
#define ALIGN_BOTTOM        7
#define ALIGN_BOTTOM_RIGHT  8

#define COLOR_BLACK     "\e[30m"
#define COLOR_MAROON    "\e[31m"
#define COLOR_GREEN     "\e[32m"
#define COLOR_OLIVE     "\e[33m"
#define COLOR_NAVY      "\e[34m"
#define COLOR_PURPLE    "\e[35m"
#define COLOR_TEAL      "\e[36m"
#define COLOR_SILVER    "\e[37m"
#define COLOR_GRAY      "\e[90m"
#define COLOR_RED       "\e[91m"
#define COLOR_LIME      "\e[92m"
#define COLOR_YELLOW    "\e[93m"
#define COLOR_BLUE      "\e[94m"
#define COLOR_MAGENTA   "\e[95m"
#define COLOR_AQUA      "\e[96m"
#define COLOR_WHITE     "\e[97m"

#ifndef __BGDC__

/* --------------------------------------------------------------------------- */

extern int64_t gr_text_new( int64_t fontid, int64_t x, int64_t y, int64_t centered, const char * text ) ;
extern int64_t gr_text_new2( int64_t fontid, int64_t x, int64_t y, int64_t z, int64_t centered, const char * text ) ;
extern int64_t gr_text_new_var( int64_t fontid, int64_t x, int64_t y, int64_t centered, const void * var, int64_t type ) ;
extern int64_t gr_text_new_var2( int64_t fontid, int64_t x, int64_t y, int64_t z, int64_t centered, const void * var, int64_t type ) ;
extern void gr_text_move( int64_t textid, int64_t x, int64_t y ) ;
extern void gr_text_move2( int64_t textid, int64_t x, int64_t y, int64_t z ) ;
extern void gr_text_destroy( int64_t textid ) ;
extern int64_t gr_text_margintop( int64_t fontid, const unsigned char * text ) ;
extern int64_t gr_text_width( int64_t fontid, const unsigned char * text ) ;
extern int64_t gr_text_height( int64_t fontid, const unsigned char * text ) ;
extern int64_t gr_text_put( GRAPH * dest, void * ptext, REGION * region, int64_t fontid, int64_t x, int64_t y, const unsigned char * text ) ;
extern GRAPH * gr_text_bitmap( int64_t fontid, const char * text, int64_t centered ) ;
extern int64_t gr_text_in_bitmap( GRAPH * gr, int64_t fontid, int64_t x, int64_t y, const char * text, int64_t alignment );
extern void gr_text_setrgba( int64_t textid, uint8_t r, uint8_t g, uint8_t b, uint8_t a );
extern int64_t gr_text_getrgba( int64_t textid, uint8_t * r, uint8_t * g, uint8_t * b, uint8_t * a );
extern void * gr_text_alloc();
extern void gr_text_setrgba_ptext( void * t, uint8_t r, uint8_t g, uint8_t b, uint8_t a );
extern int64_t gr_text_getrgba_ptext( void * t, uint8_t * r, uint8_t * g, uint8_t * b, uint8_t * a );

/* --------------------------------------------------------------------------- */

#endif

#endif
