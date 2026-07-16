/*
 * FreeType font support for BennuGD2.
 *
 * This is an altered/additional source file for Bennu Game Development.
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * Permission is granted to use, alter, and redistribute it for any purpose,
 * subject to the BennuGD2 source distribution's license terms.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "libbggfx.h"

#define TTF_DEFAULT_PIXEL_SIZE 20
#define TTF_MAX_PIXEL_SIZE 4096

typedef struct {
    FT_Face face;
    FT_Byte * data;
    size_t data_size;
    int64_t pixel_size;
} TTF_FONT_SLOT;

static FT_Library ttf_library;
static int ttf_library_initialized = 0;
static TTF_FONT_SLOT ttf_fonts[MAX_FONTS];

static const FT_ULong cp850_to_unicode[MAX_GLYPH] = {
    0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007,
    0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, 0x000F,
    0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017,
    0x0018, 0x0019, 0x001A, 0x001B, 0x001C, 0x001D, 0x001E, 0x001F,
    0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027,
    0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F,
    0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037,
    0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F,
    0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047,
    0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F,
    0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057,
    0x0058, 0x0059, 0x005A, 0x005B, 0x005C, 0x005D, 0x005E, 0x005F,
    0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067,
    0x0068, 0x0069, 0x006A, 0x006B, 0x006C, 0x006D, 0x006E, 0x006F,
    0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077,
    0x0078, 0x0079, 0x007A, 0x007B, 0x007C, 0x007D, 0x007E, 0x007F,
    0x00C7, 0x00FC, 0x00E9, 0x00E2, 0x00E4, 0x00E0, 0x00E5, 0x00E7,
    0x00EA, 0x00EB, 0x00E8, 0x00EF, 0x00EE, 0x00EC, 0x00C4, 0x00C5,
    0x00C9, 0x00E6, 0x00C6, 0x00F4, 0x00F6, 0x00F2, 0x00FB, 0x00F9,
    0x00FF, 0x00D6, 0x00DC, 0x00F8, 0x00A3, 0x00D8, 0x00D7, 0x0192,
    0x00E1, 0x00ED, 0x00F3, 0x00FA, 0x00F1, 0x00D1, 0x00AA, 0x00BA,
    0x00BF, 0x00AE, 0x00AC, 0x00BD, 0x00BC, 0x00A1, 0x00AB, 0x00BB,
    0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x00C1, 0x00C2, 0x00C0,
    0x00A9, 0x2563, 0x2551, 0x2557, 0x255D, 0x00A2, 0x00A5, 0x2510,
    0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x00E3, 0x00C3,
    0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x00A4,
    0x00F0, 0x00D0, 0x00CA, 0x00CB, 0x00C8, 0x0131, 0x00CD, 0x00CE,
    0x00CF, 0x2518, 0x250C, 0x2588, 0x2584, 0x00A6, 0x00CC, 0x2580,
    0x00D3, 0x00DF, 0x00D4, 0x00D2, 0x00F5, 0x00D5, 0x00B5, 0x00FE,
    0x00DE, 0x00DA, 0x00DB, 0x00D9, 0x00FD, 0x00DD, 0x00AF, 0x00B4,
    0x00AD, 0x00B1, 0x2017, 0x00BE, 0x00B6, 0x00A7, 0x00F7, 0x00B8,
    0x00B0, 0x00A8, 0x00B7, 0x00B9, 0x00B3, 0x00B2, 0x25A0, 0x00A0,
};

static int ttf_ensure_initialized( void ) {
    if ( ttf_library_initialized ) return 0;
    if ( FT_Init_FreeType( &ttf_library ) != 0 ) return -1;
    ttf_library_initialized = 1;
    return 0;
}

static int64_t ft_26_6_round( FT_Pos value ) {
    if ( value >= 0 ) return ( int64_t )( ( value + 32 ) >> 6 );
    return -( int64_t )( ( ( -value ) + 32 ) >> 6 );
}

static void destroy_graph_array( GRAPH ** maps ) {
    int n;
    for ( n = 0; n < MAX_GLYPH; ++n ) {
        if ( maps[n] ) bitmap_destroy( maps[n] );
    }
}

static uint8_t bitmap_coverage( const FT_Bitmap * bitmap, unsigned int x, unsigned int y ) {
    const unsigned char * row;
    int pitch;

    if ( !bitmap || !bitmap->buffer || x >= bitmap->width || y >= bitmap->rows ) return 0;

    pitch = bitmap->pitch;
    if ( pitch >= 0 ) {
        row = bitmap->buffer + ( size_t )y * ( size_t )pitch;
    } else {
        row = bitmap->buffer + ( size_t )( bitmap->rows - 1 - y ) * ( size_t )( -pitch );
    }

    switch ( bitmap->pixel_mode ) {
        case FT_PIXEL_MODE_GRAY: {
            unsigned int value = row[x];
            if ( bitmap->num_grays > 1 && bitmap->num_grays != 256 ) {
                value = ( value * 255U ) / ( bitmap->num_grays - 1U );
            }
            return ( uint8_t )value;
        }

        case FT_PIXEL_MODE_MONO:
            return ( row[x >> 3] & ( 0x80U >> ( x & 7U ) ) ) ? 255 : 0;

        case FT_PIXEL_MODE_BGRA:
            return row[x * 4U + 3U];

        default:
            return 0;
    }
}

static GRAPH * graph_from_freetype_bitmap( int64_t code, const FT_Bitmap * bitmap ) {
    SDL_Surface * surface;
    GRAPH * graph;
    uint32_t rmask, gmask, bmask, amask;
    unsigned int x, y;
    int locked = 0;

    if ( !bitmap || bitmap->width < 1 || bitmap->rows < 1 ) return NULL;

    getRGBA_mask( 32, &rmask, &gmask, &bmask, &amask );
    surface = SDL_CreateRGBSurface(
        0,
        ( int )bitmap->width,
        ( int )bitmap->rows,
        32,
        rmask,
        gmask,
        bmask,
        amask
    );
    if ( !surface ) return NULL;

    if ( SDL_MUSTLOCK( surface ) ) {
        if ( SDL_LockSurface( surface ) != 0 ) {
            SDL_FreeSurface( surface );
            return NULL;
        }
        locked = 1;
    }

    for ( y = 0; y < bitmap->rows; ++y ) {
        uint32_t * dst = ( uint32_t * )( ( uint8_t * )surface->pixels + ( size_t )y * ( size_t )surface->pitch );
        for ( x = 0; x < bitmap->width; ++x ) {
            uint8_t alpha = bitmap_coverage( bitmap, x, y );
            dst[x] = SDL_MapRGBA( surface->format, 255, 255, 255, alpha );
        }
    }

    if ( locked ) SDL_UnlockSurface( surface );
    SDL_SetColorKey( surface, SDL_FALSE, 0 );
    SDL_SetSurfaceBlendMode( surface, SDL_BLENDMODE_BLEND );

    graph = bitmap_new( code, 0, 0, surface );
    SDL_FreeSurface( surface );

    if ( graph ) bitmap_add_cpoint( graph, 0, 0 );
    return graph;
}

static void clear_font_glyphs( FONT * font ) {
    int n;

    if ( !font ) return;

    if ( font->fontmap ) {
        bitmap_destroy( font->fontmap );
        font->fontmap = NULL;
    }

    for ( n = 0; n < MAX_GLYPH; ++n ) {
        if ( font->glyph[n].glymap ) bitmap_destroy( font->glyph[n].glymap );
        memset( &font->glyph[n], 0, sizeof( font->glyph[n] ) );
    }

    font->maxwidth = 0;
    font->maxheight = 0;
}

static int render_font_glyphs( int64_t fontid ) {
    FONT * font;
    TTF_FONT_SLOT * ttf;
    GRAPH * maps[MAX_GLYPH] = { 0 };
    int64_t xoffset[MAX_GLYPH] = { 0 };
    int64_t yoffset[MAX_GLYPH] = { 0 };
    int64_t xadvance[MAX_GLYPH] = { 0 };
    int64_t yadvance[MAX_GLYPH] = { 0 };
    int64_t ascender;
    int64_t line_height;
    int64_t max_width = 0;
    int n;

    font = gr_font_get( fontid );
    if ( !font || fontid < 0 || fontid >= MAX_FONTS ) return -1;

    ttf = &ttf_fonts[fontid];
    if ( !ttf->face ) return -1;

    if ( FT_Set_Pixel_Sizes( ttf->face, 0, ( FT_UInt )ttf->pixel_size ) != 0 ) return -1;

    ascender = ft_26_6_round( ttf->face->size->metrics.ascender );
    line_height = ft_26_6_round( ttf->face->size->metrics.height );
    if ( line_height < 1 ) {
        int64_t descender = -ft_26_6_round( ttf->face->size->metrics.descender );
        line_height = ascender + descender;
    }
    if ( line_height < 1 ) line_height = ttf->pixel_size;

    for ( n = 0; n < MAX_GLYPH; ++n ) {
        FT_UInt glyph_index = FT_Get_Char_Index( ttf->face, cp850_to_unicode[n] );
        FT_GlyphSlot slot;

        if ( FT_Load_Glyph( ttf->face, glyph_index, FT_LOAD_DEFAULT ) != 0 ) continue;
        if ( FT_Render_Glyph( ttf->face->glyph, FT_RENDER_MODE_LIGHT ) != 0 ) continue;

        slot = ttf->face->glyph;

        xoffset[n] = slot->bitmap_left;
        yoffset[n] = ascender - slot->bitmap_top;
        xadvance[n] = ft_26_6_round( slot->advance.x );
        yadvance[n] = ft_26_6_round( slot->advance.y );

        if ( xadvance[n] < 0 ) xadvance[n] = 0;

        if ( slot->bitmap.width && slot->bitmap.rows ) {
            maps[n] = graph_from_freetype_bitmap( n, &slot->bitmap );
            if ( !maps[n] ) {
                destroy_graph_array( maps );
                return -1;
            }
        }

        if ( max_width < xadvance[n] ) max_width = xadvance[n];
        if ( max_width < xoffset[n] + ( int64_t )slot->bitmap.width ) {
            max_width = xoffset[n] + ( int64_t )slot->bitmap.width;
        }
    }

    clear_font_glyphs( font );

    font->charset = CHARSET_CP850;
    font->maxheight = line_height;
    font->maxwidth = max_width;

    for ( n = 0; n < MAX_GLYPH; ++n ) {
        font->glyph[n].glymap = maps[n];
        font->glyph[n].xoffset = xoffset[n];
        font->glyph[n].yoffset = yoffset[n];
        font->glyph[n].xadvance = xadvance[n];
        font->glyph[n].yadvance = yadvance[n];
    }

    return 0;
}

int64_t gr_font_ttf_load_from_memory( const void * data, size_t data_size ) {
    FT_Byte * owned_data;
    FT_Face face = NULL;
    int64_t fontid;
    TTF_FONT_SLOT * ttf;

    if ( !data || data_size < 1 ) return -1;
    if ( ttf_ensure_initialized() != 0 ) return -1;

    owned_data = ( FT_Byte * )malloc( data_size );
    if ( !owned_data ) return -1;
    memcpy( owned_data, data, data_size );

    if ( FT_New_Memory_Face(
            ttf_library,
            owned_data,
            ( FT_Long )data_size,
            0,
            &face
        ) != 0 ) {
        free( owned_data );
        return -1;
    }

    if ( !FT_IS_SCALABLE( face ) ) {
        FT_Done_Face( face );
        free( owned_data );
        return -1;
    }

    fontid = gr_font_new( CHARSET_CP850 );
    if ( fontid < 0 ) {
        FT_Done_Face( face );
        free( owned_data );
        return -1;
    }

    ttf = &ttf_fonts[fontid];
    memset( ttf, 0, sizeof( *ttf ) );
    ttf->face = face;
    ttf->data = owned_data;
    ttf->data_size = data_size;
    ttf->pixel_size = TTF_DEFAULT_PIXEL_SIZE;

    if ( render_font_glyphs( fontid ) != 0 ) {
        gr_font_destroy( fontid );
        return -1;
    }

    return fontid;
}

int64_t gr_font_ttf_load( const char * filename ) {
    FILE * fp;
    long length;
    unsigned char * data;
    size_t read_count;
    int64_t result;

    if ( !filename || !*filename ) return -1;

    fp = fopen( filename, "rb" );
    if ( !fp ) return -1;

    if ( fseek( fp, 0, SEEK_END ) != 0 ) {
        fclose( fp );
        return -1;
    }

    length = ftell( fp );
    if ( length <= 0 || fseek( fp, 0, SEEK_SET ) != 0 ) {
        fclose( fp );
        return -1;
    }

    data = ( unsigned char * )malloc( ( size_t )length );
    if ( !data ) {
        fclose( fp );
        return -1;
    }

    read_count = fread( data, 1, ( size_t )length, fp );
    fclose( fp );

    if ( read_count != ( size_t )length ) {
        free( data );
        return -1;
    }

    result = gr_font_ttf_load_from_memory( data, read_count );
    free( data );
    return result;
}

int64_t gr_font_ttf_set_size( int64_t fontid, int64_t pixels ) {
    TTF_FONT_SLOT * ttf;
    int64_t old_size;

    if ( fontid < 0 || fontid >= MAX_FONTS ) return -1;
    if ( pixels < 1 || pixels > TTF_MAX_PIXEL_SIZE ) return -1;

    ttf = &ttf_fonts[fontid];
    if ( !ttf->face || !gr_font_get( fontid ) ) return -1;
    if ( ttf->pixel_size == pixels ) return 0;

    old_size = ttf->pixel_size;
    ttf->pixel_size = pixels;

    if ( render_font_glyphs( fontid ) != 0 ) {
        ttf->pixel_size = old_size;
        FT_Set_Pixel_Sizes( ttf->face, 0, ( FT_UInt )old_size );
        return -1;
    }

    return 0;
}

int64_t gr_font_ttf_is_font( int64_t fontid ) {
    if ( fontid < 0 || fontid >= MAX_FONTS ) return 0;
    return ttf_fonts[fontid].face != NULL && gr_font_get( fontid ) != NULL;
}

int64_t gr_font_ttf_get_size( int64_t fontid ) {
    if ( !gr_font_ttf_is_font( fontid ) ) return -1;
    return ttf_fonts[fontid].pixel_size;
}

const char * gr_font_ttf_get_family( int64_t fontid ) {
    if ( !gr_font_ttf_is_font( fontid ) ) return "";
    return ttf_fonts[fontid].face->family_name ? ttf_fonts[fontid].face->family_name : "";
}

const char * gr_font_ttf_get_style( int64_t fontid ) {
    if ( !gr_font_ttf_is_font( fontid ) ) return "";
    return ttf_fonts[fontid].face->style_name ? ttf_fonts[fontid].face->style_name : "";
}

int64_t gr_font_ttf_get_kerning( int64_t fontid, uint8_t left, uint8_t right ) {
    TTF_FONT_SLOT * ttf;
    FT_UInt left_index;
    FT_UInt right_index;
    FT_Vector delta;

    if ( !left || !right || !gr_font_ttf_is_font( fontid ) ) return 0;

    ttf = &ttf_fonts[fontid];
    if ( !FT_HAS_KERNING( ttf->face ) ) return 0;

    left_index = FT_Get_Char_Index( ttf->face, cp850_to_unicode[left] );
    right_index = FT_Get_Char_Index( ttf->face, cp850_to_unicode[right] );
    if ( !left_index || !right_index ) return 0;

    if ( FT_Get_Kerning( ttf->face, left_index, right_index, FT_KERNING_DEFAULT, &delta ) != 0 ) return 0;
    return ft_26_6_round( delta.x );
}

void gr_font_ttf_forget( int64_t fontid ) {
    TTF_FONT_SLOT * ttf;

    if ( fontid < 0 || fontid >= MAX_FONTS ) return;

    ttf = &ttf_fonts[fontid];
    if ( ttf->face ) FT_Done_Face( ttf->face );
    free( ttf->data );
    memset( ttf, 0, sizeof( *ttf ) );
}

void gr_font_ttf_shutdown( void ) {
    int n;

    for ( n = 0; n < MAX_FONTS; ++n ) gr_font_ttf_forget( n );

    if ( ttf_library_initialized ) {
        FT_Done_FreeType( ttf_library );
        ttf_library_initialized = 0;
    }
}
