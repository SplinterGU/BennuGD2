/*
 * FreeType UTF-8 font support for BennuGD2.
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
#define TTF_CACHE_BUCKETS 257
#define TTF_REPLACEMENT_CHARACTER 0xFFFDU

typedef struct _ttf_cached_glyph {
    uint32_t codepoint;
    TTF_GLYPH_INFO glyph;
    struct _ttf_cached_glyph * next;
} TTF_CACHED_GLYPH;

typedef struct {
    FT_Face face;
    FT_Byte * data;
    size_t data_size;
    int64_t pixel_size;
    TTF_CACHED_GLYPH * cache[TTF_CACHE_BUCKETS];
} TTF_FONT_SLOT;

static FT_Library ttf_library;
static int ttf_library_initialized = 0;
static TTF_FONT_SLOT ttf_fonts[MAX_FONTS];

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

static void destroy_glyph_graphs( TTF_GLYPH_INFO * glyphs, size_t count ) {
    size_t n;
    for ( n = 0; n < count; ++n ) {
        if ( glyphs[n].glymap ) bitmap_destroy( glyphs[n].glymap );
    }
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

static void clear_dynamic_cache( TTF_FONT_SLOT * ttf ) {
    size_t bucket;

    if ( !ttf ) return;

    for ( bucket = 0; bucket < TTF_CACHE_BUCKETS; ++bucket ) {
        TTF_CACHED_GLYPH * entry = ttf->cache[bucket];
        while ( entry ) {
            TTF_CACHED_GLYPH * next = entry->next;
            if ( entry->glyph.glymap ) bitmap_destroy( entry->glyph.glymap );
            free( entry );
            entry = next;
        }
        ttf->cache[bucket] = NULL;
    }
}

static int64_t face_ascender( FT_Face face ) {
    return ft_26_6_round( face->size->metrics.ascender );
}

static int64_t face_line_height( FT_Face face, int64_t fallback ) {
    int64_t height = ft_26_6_round( face->size->metrics.height );

    if ( height < 1 ) {
        int64_t ascender = face_ascender( face );
        int64_t descender = -ft_26_6_round( face->size->metrics.descender );
        height = ascender + descender;
    }

    if ( height < 1 ) height = fallback;
    return height;
}

static int load_unicode_glyph(
    TTF_FONT_SLOT * ttf,
    uint32_t codepoint,
    int64_t graph_code,
    TTF_GLYPH_INFO * glyph
) {
    FT_UInt glyph_index;
    FT_GlyphSlot slot;
    int64_t ascender;

    if ( !ttf || !ttf->face || !glyph ) return -1;
    memset( glyph, 0, sizeof( *glyph ) );

    glyph_index = FT_Get_Char_Index( ttf->face, ( FT_ULong )codepoint );
    if ( !glyph_index && codepoint != 0 ) {
        glyph_index = FT_Get_Char_Index( ttf->face, TTF_REPLACEMENT_CHARACTER );
    }

    if ( FT_Load_Glyph( ttf->face, glyph_index, FT_LOAD_DEFAULT ) != 0 ) return -1;
    if ( FT_Render_Glyph( ttf->face->glyph, FT_RENDER_MODE_LIGHT ) != 0 ) return -1;

    slot = ttf->face->glyph;
    ascender = face_ascender( ttf->face );

    glyph->xoffset = slot->bitmap_left;
    glyph->yoffset = ascender - slot->bitmap_top;
    glyph->xadvance = ft_26_6_round( slot->advance.x );
    glyph->yadvance = ft_26_6_round( slot->advance.y );

    if ( glyph->xadvance < 0 ) glyph->xadvance = 0;

    if ( slot->bitmap.width && slot->bitmap.rows ) {
        glyph->glymap = graph_from_freetype_bitmap( graph_code, &slot->bitmap );
        if ( !glyph->glymap ) return -1;
    }

    return 0;
}

static int render_base_glyphs( int64_t fontid ) {
    FONT * font;
    TTF_FONT_SLOT * ttf;
    TTF_GLYPH_INFO glyphs[MAX_GLYPH];
    int64_t max_width = 0;
    int64_t line_height;
    int n;

    font = gr_font_get( fontid );
    if ( !font || fontid < 0 || fontid >= MAX_FONTS ) return -1;

    ttf = &ttf_fonts[fontid];
    if ( !ttf->face ) return -1;

    memset( glyphs, 0, sizeof( glyphs ) );

    if ( FT_Set_Pixel_Sizes( ttf->face, 0, ( FT_UInt )ttf->pixel_size ) != 0 ) return -1;
    line_height = face_line_height( ttf->face, ttf->pixel_size );

    /* Keep U+0000..U+00FF in FONT.glyph for compatibility with GLYPH_GET and
     * existing engine code. All larger Unicode characters are cached lazily. */
    for ( n = 0; n < MAX_GLYPH; ++n ) {
        if ( load_unicode_glyph( ttf, ( uint32_t )n, n, &glyphs[n] ) != 0 ) {
            destroy_glyph_graphs( glyphs, MAX_GLYPH );
            return -1;
        }

        if ( max_width < glyphs[n].xadvance ) max_width = glyphs[n].xadvance;
        if ( glyphs[n].glymap && max_width < glyphs[n].xoffset + ( int64_t )glyphs[n].glymap->width ) {
            max_width = glyphs[n].xoffset + ( int64_t )glyphs[n].glymap->width;
        }
    }

    clear_dynamic_cache( ttf );
    clear_font_glyphs( font );

    font->charset = CHARSET_UTF8;
    font->maxheight = line_height;
    font->maxwidth = max_width;

    for ( n = 0; n < MAX_GLYPH; ++n ) {
        font->glyph[n].glymap = glyphs[n].glymap;
        font->glyph[n].xoffset = glyphs[n].xoffset;
        font->glyph[n].yoffset = glyphs[n].yoffset;
        font->glyph[n].xadvance = glyphs[n].xadvance;
        font->glyph[n].yadvance = glyphs[n].yadvance;
        glyphs[n].glymap = NULL;
    }

    return 0;
}

static size_t glyph_hash( uint32_t codepoint ) {
    return ( size_t )( ( codepoint * 2654435761U ) % TTF_CACHE_BUCKETS );
}

int gr_font_ttf_get_glyph( int64_t fontid, uint32_t codepoint, TTF_GLYPH_INFO * glyph ) {
    FONT * font;
    TTF_FONT_SLOT * ttf;
    size_t bucket;
    TTF_CACHED_GLYPH * entry;

    if ( !glyph || !gr_font_ttf_is_font( fontid ) ) return -1;
    memset( glyph, 0, sizeof( *glyph ) );

    font = gr_font_get( fontid );
    ttf = &ttf_fonts[fontid];

    if ( codepoint < MAX_GLYPH ) {
        glyph->glymap = font->glyph[codepoint].glymap;
        glyph->xoffset = font->glyph[codepoint].xoffset;
        glyph->yoffset = font->glyph[codepoint].yoffset;
        glyph->xadvance = font->glyph[codepoint].xadvance;
        glyph->yadvance = font->glyph[codepoint].yadvance;
        return 0;
    }

    if ( codepoint > 0x10FFFFU || ( codepoint >= 0xD800U && codepoint <= 0xDFFFU ) ) {
        codepoint = TTF_REPLACEMENT_CHARACTER;
    }

    bucket = glyph_hash( codepoint );
    for ( entry = ttf->cache[bucket]; entry; entry = entry->next ) {
        if ( entry->codepoint == codepoint ) {
            *glyph = entry->glyph;
            return 0;
        }
    }

    entry = ( TTF_CACHED_GLYPH * )calloc( 1, sizeof( *entry ) );
    if ( !entry ) return -1;

    entry->codepoint = codepoint;
    if ( load_unicode_glyph( ttf, codepoint, ( int64_t )codepoint, &entry->glyph ) != 0 ) {
        free( entry );
        return -1;
    }

    entry->next = ttf->cache[bucket];
    ttf->cache[bucket] = entry;
    *glyph = entry->glyph;

    if ( font->maxwidth < glyph->xadvance ) font->maxwidth = glyph->xadvance;
    if ( glyph->glymap && font->maxwidth < glyph->xoffset + ( int64_t )glyph->glymap->width ) {
        font->maxwidth = glyph->xoffset + ( int64_t )glyph->glymap->width;
    }

    return 0;
}

uint32_t gr_font_ttf_utf8_next( const unsigned char ** text ) {
    const unsigned char * s;
    uint32_t codepoint;

    if ( !text || !( s = *text ) || !*s ) return 0;

    if ( s[0] < 0x80U ) {
        *text = s + 1;
        return s[0];
    }

    if ( s[0] >= 0xC2U && s[0] <= 0xDFU && s[1] >= 0x80U && s[1] <= 0xBFU ) {
        codepoint = ( ( uint32_t )( s[0] & 0x1FU ) << 6 ) |
                    ( uint32_t )( s[1] & 0x3FU );
        *text = s + 2;
        return codepoint;
    }

    if ( s[0] >= 0xE0U && s[0] <= 0xEFU &&
         s[1] >= 0x80U && s[1] <= 0xBFU &&
         s[2] >= 0x80U && s[2] <= 0xBFU &&
         !( s[0] == 0xE0U && s[1] < 0xA0U ) &&
         !( s[0] == 0xEDU && s[1] >= 0xA0U ) ) {
        codepoint = ( ( uint32_t )( s[0] & 0x0FU ) << 12 ) |
                    ( ( uint32_t )( s[1] & 0x3FU ) << 6 ) |
                    ( uint32_t )( s[2] & 0x3FU );
        *text = s + 3;
        return codepoint;
    }

    if ( s[0] >= 0xF0U && s[0] <= 0xF4U &&
         s[1] >= 0x80U && s[1] <= 0xBFU &&
         s[2] >= 0x80U && s[2] <= 0xBFU &&
         s[3] >= 0x80U && s[3] <= 0xBFU &&
         !( s[0] == 0xF0U && s[1] < 0x90U ) &&
         !( s[0] == 0xF4U && s[1] > 0x8FU ) ) {
        codepoint = ( ( uint32_t )( s[0] & 0x07U ) << 18 ) |
                    ( ( uint32_t )( s[1] & 0x3FU ) << 12 ) |
                    ( ( uint32_t )( s[2] & 0x3FU ) << 6 ) |
                    ( uint32_t )( s[3] & 0x3FU );
        *text = s + 4;
        return codepoint;
    }

    *text = s + 1;
    return TTF_REPLACEMENT_CHARACTER;
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

    if ( !FT_IS_SCALABLE( face ) || FT_Select_Charmap( face, FT_ENCODING_UNICODE ) != 0 ) {
        FT_Done_Face( face );
        free( owned_data );
        return -1;
    }

    fontid = gr_font_new( CHARSET_UTF8 );
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

    if ( render_base_glyphs( fontid ) != 0 ) {
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

    if ( render_base_glyphs( fontid ) != 0 ) {
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

int64_t gr_font_ttf_get_line_height( int64_t fontid ) {
    FONT * font;
    if ( !gr_font_ttf_is_font( fontid ) || !( font = gr_font_get( fontid ) ) ) return 0;
    return font->maxheight;
}

const char * gr_font_ttf_get_family( int64_t fontid ) {
    if ( !gr_font_ttf_is_font( fontid ) ) return "";
    return ttf_fonts[fontid].face->family_name ? ttf_fonts[fontid].face->family_name : "";
}

const char * gr_font_ttf_get_style( int64_t fontid ) {
    if ( !gr_font_ttf_is_font( fontid ) ) return "";
    return ttf_fonts[fontid].face->style_name ? ttf_fonts[fontid].face->style_name : "";
}

int64_t gr_font_ttf_get_kerning( int64_t fontid, uint32_t left, uint32_t right ) {
    TTF_FONT_SLOT * ttf;
    FT_UInt left_index;
    FT_UInt right_index;
    FT_Vector delta;

    if ( !left || !right || !gr_font_ttf_is_font( fontid ) ) return 0;

    ttf = &ttf_fonts[fontid];
    if ( !FT_HAS_KERNING( ttf->face ) ) return 0;

    left_index = FT_Get_Char_Index( ttf->face, ( FT_ULong )left );
    right_index = FT_Get_Char_Index( ttf->face, ( FT_ULong )right );
    if ( !left_index || !right_index ) return 0;

    if ( FT_Get_Kerning( ttf->face, left_index, right_index, FT_KERNING_DEFAULT, &delta ) != 0 ) return 0;
    return ft_26_6_round( delta.x );
}

void gr_font_ttf_forget( int64_t fontid ) {
    TTF_FONT_SLOT * ttf;

    if ( fontid < 0 || fontid >= MAX_FONTS ) return;

    ttf = &ttf_fonts[fontid];
    clear_dynamic_cache( ttf );
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
