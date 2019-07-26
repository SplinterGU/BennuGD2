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

/* --------------------------------------------------------------------------- */

#include <stdlib.h>

#include "bgdrtm.h"
#include "bgddl.h"

#include "xstrings.h"

#include "libbggfx.h"
#include "libmod_gfx.h"

#include "file_div.h"

/* --------------------------------------------------------------------------- */
/** LOAD_FNT (STRING FILENAME)
 *  Load a .FNT font from disk (returns the font ID)
 */

int64_t libmod_gfx_load_fnt( INSTANCE * my, int64_t * params ) {
    int64_t r = gr_font_load( ( char * )string_get( params[0] ) );
    string_discard( params[0] ) ;
    return r ;
}

// /* --------------------------------------------------------------------------- */
// /* --------------------------------------------------------------------------- */
// /* --------------------------------------------------------------------------- */
//
// /* --------------------------------------------------------------------------- */
// /** LOAD_BDF (STRING FILENAME)
//  *  Load a .BDF font from disk (returns the font ID)
//  */
//
// int64_t libmod_gfx_load_bdf( INSTANCE * my, int64_t * params )
// {
//     int r = gr_load_bdf( ( char * )string_get( params[0] ) ) ;
//     string_discard( params[0] ) ;
//     return r ;
// }
//
/* --------------------------------------------------------------------------- */
/** UNLOAD_FNT (FONT)
 *  Destroys a font in memory
 */

int64_t libmod_gfx_unload_fnt( INSTANCE * my, int64_t * params ) {
    if ( params[0] > 0 ) gr_font_destroy( params[0] );
    return 0;
}

/* --------------------------------------------------------------------------- */
/** FNT_NEW ()
 *  Create a new font in memory (returns the font ID)
 */

int64_t libmod_gfx_fnt_new( INSTANCE * my, int64_t * params ) {
    return gr_font_new( CHARSET_CP850 );
}

/* --------------------------------------------------------------------------- */
/** FNT_NEW (CHARSET)
 *  Create a new font in memory (returns the font ID)
 */

int64_t libmod_gfx_fnt_new_charset( INSTANCE * my, int64_t * params ) {
    return gr_font_new( params[0] );
}

/* --------------------------------------------------------------------------- */
/** FNT_NEW (FILE, GRAPH, CHARSET, WIDTH, HEIGHT, FIRST, LAST, FLAGS)
 *  Create a new font in memory (returns the font ID)
 */

int64_t libmod_gfx_fnt_new_from_bitmap( INSTANCE * my, int64_t * params ) {
    GRAPH * bmp = bitmap_get( params[0], params[1] ) ;
    if ( !bmp ) return -1;
#ifdef USE_SDL2_GPU
    return gr_font_new_from_bitmap( bmp, NULL, params[2], params[3], params[4], params[5], params[6], params[7], NULL );
#endif
#ifdef USE_SDL2
    return gr_font_new_from_bitmap( bmp, params[2], params[3], params[4], params[5], params[6], params[7], NULL );
#endif
}

/* --------------------------------------------------------------------------- */
/** FNT_NEW (FILE, GRAPH, CHARSET, WIDTH, HEIGHT, FIRST, LAST, FLAGS, CHARMAP)
 *  Create a new font in memory (returns the font ID)
 */

int64_t libmod_gfx_fnt_new_from_bitmap2( INSTANCE * my, int64_t * params ) {
    GRAPH * bmp = bitmap_get( params[0], params[1] ) ;
    if ( !bmp ) return -1;
#ifdef USE_SDL2_GPU
    int64_t ret = gr_font_new_from_bitmap( bmp, NULL, params[2], params[3], params[4], params[5], params[6], params[7], string_get( params[8] ) );
#endif
#ifdef USE_SDL2
    int64_t ret = gr_font_new_from_bitmap( bmp, params[2], params[3], params[4], params[5], params[6], params[7], string_get( params[8] ) );
#endif
    string_discard( params[8] );
    return ret;
}

/* --------------------------------------------------------------------------- */
/** GET_GLYPH (FONT, GLYPH)
 *  Create a system map as a copy of one of the font glyphs (returns the map ID)
 */

int64_t libmod_gfx_get_glyph( INSTANCE * my, int64_t * params ) {
    FONT  * font = gr_font_get( params[0] );
    GRAPH * map ;
    unsigned char c = params[1];

    if ( font->charset == /*CHARSET_CP850*/ CHARSET_ISO8859 ) c = win_to_dos[c];
    if ( !font ) return 0;
    if ( !font->glyph[c].glymap ) return 0;

    map = bitmap_clone( font->glyph[c].glymap );
    if ( !map ) return 0;

    map->code = bitmap_next_code();

    if ( !map->ncpoints ) bitmap_add_cpoint( map, map->width / 2, map->height / 2 );
    bitmap_add_cpoint( map, font->glyph[c].xoffset, font->glyph[c].yoffset );
    bitmap_add_cpoint( map, font->glyph[c].xadvance, font->glyph[c].yadvance );

    grlib_add_map( 0, map );

    return map->code;
}

/* --------------------------------------------------------------------------- */
/** SET_GLYPH (FONT, GLYPH, LIBRARY, GRAPHIC)
 *  Change one of the font's glyphs
 */

int64_t libmod_gfx_set_glyph( INSTANCE * my, int64_t * params ) {
    FONT  * font = gr_font_get( params[0] );
    GRAPH * map  = bitmap_get( params[2], params[3] );
    unsigned char c = params[1];

    if ( font->charset == /*CHARSET_CP850*/ CHARSET_ISO8859 ) c = win_to_dos[c];

    if ( font && map ) {
        if ( font->glyph[c].glymap ) grlib_unload_map( 0, font->glyph[c].glymap->code );
        font->glyph[c].glymap = bitmap_clone( map );
        if ( font->glyph[c].glymap ) {
            font->glyph[c].glymap->code = bitmap_next_code();

            if ( map->ncpoints >= 3 && map->cpoints ) {
                font->glyph[c].xoffset = map->cpoints[1].x;
                font->glyph[c].yoffset = map->cpoints[1].y;
                font->glyph[c].xadvance = map->cpoints[2].x;
                font->glyph[c].yadvance = map->cpoints[2].y;
            } else {
                font->glyph[c].xoffset = 0;
                font->glyph[c].yoffset = 0;
                font->glyph[c].xadvance = map->width + map->width / 5 ;
                font->glyph[c].yadvance = map->height + map->height / 5 ;
                bitmap_add_cpoint( font->glyph[c].glymap, 0, 0 ) ;
            }
            grlib_add_map( 0, font->glyph[c].glymap );
        }
    }
    return 0;
}

// /* --------------------------------------------------------------------------- */
// /** SAVE_FNT (FONT, STRING FILENAME)
//  *  Saves a font to disk
//  */
//
// int64_t libmod_gfx_save_fnt( INSTANCE * my, int64_t * params )
// {
//     int r = gr_font_save( params[0], ( char * )string_get( params[1] ) ) ;
//     string_discard( params[1] ) ;
//     return r ;
// }
//
