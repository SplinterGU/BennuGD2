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

#ifndef __FONT_H
#define __FONT_H

#include "g_bitmap.h"

/* -------------------------------------------------------------------------- */

#define MAX_FONTS       256

#define NFB_FIXEDWIDTH  1

/* -------------------------------------------------------------------------- */

enum {
    CHARSET_ISO8859 = 0,
    CHARSET_CP850   = 1
};

/* -------------------------------------------------------------------------- */

#ifndef __BGDC__

/* -------------------------------------------------------------------------- */

typedef struct _font {
    int64_t charset;
    GRAPH * fontmap;

    struct _glyph {
        union {
            GRAPH * glymap;
            SDL_Rect fontsource; // Used if fontmap
        };
        int64_t xoffset;
        int64_t yoffset;
        int64_t xadvance;
        int64_t yadvance;
    } glyph[256];

    int64_t maxheight;
    int64_t maxwidth;
} FONT;

/* -------------------------------------------------------------------------- */

extern FONT * fonts[MAX_FONTS];
extern int64_t font_count;

/* -------------------------------------------------------------------------- */

extern void gr_font_destroy( int64_t fontid );
extern FONT * gr_font_get( int64_t id );
extern int64_t gr_font_new( int64_t charset );
extern int64_t gr_font_new_from_bitmap( GRAPH * map, int64_t charset, int64_t width, int64_t height, int64_t first, int64_t last, int64_t options, const unsigned char * charmap );
extern int gr_font_systemfont();
void gr_font_init();

/* -------------------------------------------------------------------------- */

#endif

#endif
