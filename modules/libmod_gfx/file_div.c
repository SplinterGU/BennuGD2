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

/* --------------------------------------------------------------------------- */

#include "file_div.h"

/* --------------------------------------------------------------------------- */

struct {
    int     code;
    int     regsize;
    char    name[32];
    char    fpname[12];
    int     width;
    int     height;
    int     flags;
} chunk;

/* --------------------------------------------------------------------------- */

typedef struct _chardata {
    int width;
    int height;
    int xadvance;
    int yadvance;
    int xoffset;
    int yoffset;
    int fileoffset;
} _chardata;

/* --------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------- */

static char * gr_read_pal( file * fp ) {
    uint8_t * colors = malloc( 768 );
    int i;

    if ( !file_read( fp, colors, 768 ) ) return NULL;

    for ( i = 0; i < 768; i++ ) colors[i] <<= 2;

    return ( char * ) colors;
}

/* --------------------------------------------------------------------------- */

static char * gr_read_pal_with_gamma( file * fp ) {
    char * colors = gr_read_pal( fp );

    /* Skip gamma definition */
    if ( colors ) file_seek( fp, 576, SEEK_CUR );

    return colors;
}

/* --------------------------------------------------------------------------- */

/* Static convenience function */
static int64_t gr_read_lib( file * fp ) {
    char header[8];
    short int px, py;
    int bpp, ii;
    int64_t libid;
    uint32_t y;
    unsigned c;
    GRLIB * lib;
    char * colors = NULL;
    int st = 0;

    libid = grlib_new();
    if ( libid < 0 ) return -1;

    lib = grlib_get( libid );
    if ( !lib ) {
        grlib_destroy( libid );
        return -1;
    }

    if ( file_read( fp, header, sizeof( header ) ) != sizeof( header ) ) {
        grlib_destroy( libid );
        return -1;
    }

    if ( strcmp( header, F32_MAGIC ) == 0 ) bpp = 32;
    else if ( strcmp( header, F16_MAGIC ) == 0 ) bpp = 16;
    else if ( strcmp( header, FPG_MAGIC ) == 0 ) bpp = 8;
    else if ( strcmp( header, F01_MAGIC ) == 0 ) bpp = 1;
    else {
        grlib_destroy( libid );
        return -1;
    }

    if ( bpp == 8 && !( colors = gr_read_pal_with_gamma( fp ) ) ) {
        grlib_destroy( libid );
        return -1;
    }

    SDL_Palette * pal = NULL;
    if ( colors ) {
        pal = SDL_AllocPalette( 256 );
        if ( !pal ) {
            grlib_destroy( libid );
            free( colors );
            return -1;
        }

        SDL_Color * cl = malloc( sizeof( SDL_Color ) * 256 );
        if ( !cl ) {
            SDL_FreePalette( pal );
            grlib_destroy( libid );
            free( colors );
            return -1;
        }

        int i;

        for ( i = 0; i < 256; i++ ) {
            cl[i].r = colors[i*3];
            cl[i].g = colors[i*3+1];
            cl[i].b = colors[i*3+2];
            cl[i].a = 255;
        }
        free( colors );
        SDL_SetPaletteColors( pal, (const SDL_Color *)cl, 0, 256 );
        free( cl );
    }

    uint32_t rmask, gmask, bmask, amask;
    getRGBA_mask( bpp, &rmask, &gmask, &bmask, &amask );

    while ( !file_eof( fp ) ) {
        if ( file_read( fp, &chunk, sizeof( chunk ) ) != sizeof( chunk ) ) break;

        ARRANGE_DWORD( &chunk.code );
        if ( chunk.code < 0 || chunk.code > 999 ) break;
        ARRANGE_DWORD( &chunk.regsize );
        ARRANGE_DWORD( &chunk.width );
        ARRANGE_DWORD( &chunk.height );
        ARRANGE_DWORD( &chunk.flags );

        /* Graph header */

        SDL_Surface* surface = SDL_CreateRGBSurface(0, chunk.width, chunk.height, bpp, rmask, gmask, bmask, amask );
        if ( !surface ) {
            if ( pal ) SDL_FreePalette( pal );
            grlib_destroy( libid );
            return -1;
        }

        if ( pal ) SDL_SetSurfacePalette( surface, pal );

        // Set transparent color
        if ( bpp != 32 ) {
            if ( bpp == 1 ) SDL_SetColorKey( surface, SDL_TRUE, 1 );
            else            SDL_SetColorKey( surface, SDL_TRUE, 0 );
        }

        int ncpoints = chunk.flags;
        CPOINT * cpoints = NULL;

        if ( ncpoints ) {
            cpoints = ( CPOINT * ) malloc( ncpoints * sizeof( CPOINT ) );
            if ( !cpoints ) {
                SDL_FreeSurface( surface );
                if ( pal ) SDL_FreePalette( pal );
                grlib_destroy( libid );
                return -1;
            }

            for ( c = 0; c < ncpoints; c++ ) {
                file_readSint16( fp, &px );
                file_readSint16( fp, &py );
                if ( px == -1 && py == -1 ) {
                    cpoints[c].x = CPOINT_UNDEFINED;
                    cpoints[c].y = CPOINT_UNDEFINED;
                } else {
                    cpoints[c].x = px;
                    cpoints[c].y = py;
                }
            }
        }

        /* Graphic data */

        int widthb = chunk.width * bpp / 8;
        if (( widthb * 8 / bpp ) < chunk.width ) widthb++;

        for ( y = 0; y < chunk.height; y++ ) {
            uint8_t * line = ( uint8_t * ) surface->pixels + surface->pitch * y;

            switch ( bpp ) {
                case    32:
                    st = file_readUint32A( fp, ( uint32_t * ) line, chunk.width );
                    break;

                case    16:
                    st = file_readUint16A( fp, ( uint16_t * ) line, chunk.width );
                    break;

                case    8:
                    st = file_read( fp, line, widthb );
                    break;

                case    1:
                    st = file_read( fp, line, widthb );
                    for ( ii = 0; ii < widthb; ii++ ) line[ii] = ~line[ii];
                    break;

            }

            if ( !st ) {
                free( cpoints );
                SDL_FreeSurface( surface );
                if ( pal ) SDL_FreePalette( pal );
                grlib_destroy( libid );
                return -1;
            }
        }

        GRAPH *gr = bitmap_new( chunk.code, 0, 0, surface );
        if ( !gr ) {
            free( cpoints );
            SDL_FreeSurface( surface );
            if ( pal ) SDL_FreePalette( pal );
            grlib_destroy( libid );
            return -1;
        }
        SDL_FreeSurface( surface );

        memcpy( gr->name, chunk.name, sizeof( chunk.name ) );
        gr->name[31] = 0;
        gr->ncpoints = ncpoints;
        gr->cpoints = cpoints;

        grlib_add_map( libid, gr );
    }

    if ( pal ) SDL_FreePalette( pal );

    return libid;
}

/* --------------------------------------------------------------------------- */

static int64_t gr_font_loadfrom( file * fp ) {
    char header[8];
    int bpp;
    int types, i, ii;
    int64_t id;
    uint32_t y;
    FONT * f;
    char * colors = NULL;

    struct {
        int width;
        int height;
        int yoffset;
        int fileoffset;
    } oldchardata[256];

    _chardata chardata[256];

    /* Read the file header */

    if ( file_read( fp, header, sizeof( header ) ) != sizeof( header ) ) return -1;

    if ( memcmp( header, FNT_MAGIC, 7 ) != 0 &&
         memcmp( header, FNX_MAGIC, 7 ) != 0 ) {
        return -1;
    }

    bpp = header[7];
    if ( bpp == 0 ) bpp = 8;

    /* Read or ignore the palette */

    if ( bpp == 8 && !( colors = gr_read_pal_with_gamma( fp ) ) ) return -1;

    SDL_Palette * pal = NULL;
    if ( colors ) {
        pal = SDL_AllocPalette( 256 );
        if ( !pal ) {
            free( colors );
            return -1;
        }

        SDL_Color * cl = malloc( sizeof( SDL_Color ) * 256 );
        if ( !cl ) {
            SDL_FreePalette( pal );
            free( colors );
            return -1;
        }

        for ( i = 0; i < 256; i++ ) {
            cl[i].r = colors[i*3];
            cl[i].g = colors[i*3+1];
            cl[i].b = colors[i*3+2];
            cl[i].a = 255;
        }
        free( colors );
        SDL_SetPaletteColors( pal, (const SDL_Color *)cl, 0, 256 );
        free( cl );
    }

    /* Read the character data (detect old format) */

    if ( header[2] == 'x' ) {
        if ( !file_readSint32( fp, &types ) ) {
            if ( pal ) SDL_FreePalette( pal );
            return -1;
        }
        if ( file_read( fp, chardata, sizeof( chardata ) ) != sizeof( chardata ) ) {
            if ( pal ) SDL_FreePalette( pal );
            return -1;
        }
        for ( i = 0; i < 256; i++ ) {
            ARRANGE_DWORD( &chardata[i].width );
            ARRANGE_DWORD( &chardata[i].height );
            ARRANGE_DWORD( &chardata[i].xadvance );
            ARRANGE_DWORD( &chardata[i].yadvance );
            ARRANGE_DWORD( &chardata[i].xoffset );
            ARRANGE_DWORD( &chardata[i].yoffset );
            ARRANGE_DWORD( &chardata[i].fileoffset );
        }
    } else {
        if ( !file_readSint32( fp, &types ) ) {
            if ( pal ) SDL_FreePalette( pal );
            return -1;
        }
        if ( file_read( fp, oldchardata, sizeof( oldchardata ) ) != sizeof( oldchardata ) ) {
            if ( pal ) SDL_FreePalette( pal );
            return -1;
        }
        for ( i = 0; i < 256; i++ ) {
            ARRANGE_DWORD( &oldchardata[i].width );
            ARRANGE_DWORD( &oldchardata[i].height );
            ARRANGE_DWORD( &oldchardata[i].yoffset );
            ARRANGE_DWORD( &oldchardata[i].fileoffset );

            chardata[i].width      = oldchardata[i].width;
            chardata[i].height     = oldchardata[i].height;
            chardata[i].xoffset    = 0;
            chardata[i].yoffset    = oldchardata[i].yoffset;
            chardata[i].xadvance   = oldchardata[i].width;
            chardata[i].yadvance   = oldchardata[i].height + oldchardata[i].yoffset;
            chardata[i].fileoffset = oldchardata[i].fileoffset;
        }
    }

    /* Create the font */

    if ( header[2] == 'x' )
        id = gr_font_new( types );
    else
        id = gr_font_new( CHARSET_CP850 );

    if ( id == -1 ) {
        if ( pal ) SDL_FreePalette( pal );
        return -1;
    }

    f = fonts[id];
    if ( !f ) {
        gr_font_destroy( id );
        if ( pal ) SDL_FreePalette( pal );
        return -1;
    }

    uint32_t rmask, gmask, bmask, amask;
    getRGBA_mask( bpp, &rmask, &gmask, &bmask, &amask );

    int min_xadvance = 0;

    /* Load the character bitmaps */

    for ( i = 0; i < 256; i++ ) {
        f->glyph[i].xadvance = chardata[i].xadvance;
        f->glyph[i].yadvance = chardata[i].yadvance;

        if ( !min_xadvance ) min_xadvance = chardata[i].xadvance;
        else if ( chardata[i].xadvance > 0 && chardata[i].xadvance < min_xadvance ) min_xadvance = chardata[i].xadvance;

        if ( chardata[i].fileoffset == 0 || chardata[i].width == 0 || chardata[i].height == 0 ) continue;

        f->glyph[i].xoffset = chardata[i].xoffset;
        f->glyph[i].yoffset = chardata[i].yoffset;

        file_seek( fp, chardata[i].fileoffset, SEEK_SET );

        SDL_Surface* surface = SDL_CreateRGBSurface(0, chardata[i].width, chardata[i].height, bpp, rmask, gmask, bmask, amask );
        if ( !surface ) {
            gr_font_destroy( id );
            if ( pal ) SDL_FreePalette( pal );
            return -1;
        }

        if ( pal ) SDL_SetSurfacePalette( surface, pal );

        // Set transparent color
        if ( bpp != 32 ) {
            if ( bpp == 1 ) SDL_SetColorKey( surface, SDL_TRUE, 1 );
            else            SDL_SetColorKey( surface, SDL_TRUE, 0 );
        }

        /* Graphic data */

        int widthb = chardata[i].width * bpp / 8;
        if (( widthb * 8 / bpp ) < chardata[i].width ) widthb++;

        for ( y = 0; y < chardata[i].height; y++ ) {
            uint8_t * line = ( uint8_t * ) surface->pixels + surface->pitch * y;
            int st;

            switch ( bpp ) {
                case    32:
                    st = file_readUint32A( fp, ( uint32_t * ) line, chardata[i].width );
                    break;

                case    16:
                    st = file_readUint16A( fp, ( uint16_t * ) line, chardata[i].width );
                    break;

                case    8:
                    if ( ( st = file_read( fp, line, widthb ) ) != widthb ) st = 0;
                    break;

                case    1:
                    if ( ( st = file_read( fp, line, widthb ) ) != widthb ) st = 0;
                    else
                        for ( ii = 0; ii < widthb; ii++ ) line[ii] = ~line[ii];
                    break;
            }

            if ( !st ) {
                SDL_FreeSurface( surface );
                gr_font_destroy( id );
                if ( pal ) SDL_FreePalette( pal );
                return -1;
            }
        }

        f->glyph[i].glymap = bitmap_new( i, 0, 0, surface );
        SDL_FreeSurface( surface );
        if ( !f->glyph[i].glymap ) {
            if ( pal ) SDL_FreePalette( pal );
            gr_font_destroy( id );
            return -1;
        }

        bitmap_add_cpoint( f->glyph[i].glymap, 0, 0 );

        f->glyph[i].yoffset = chardata[i].yoffset;
    }

    if ( f->glyph[32].xadvance == 0 ) f->glyph[32].xadvance = min_xadvance; // f->glyph['j'].xadvance;

    if ( pal ) SDL_FreePalette( pal );

    return id;
}

/* --------------------------------------------------------------------------- */

static GRAPH * gr_read_map( file * fp ) {
    char header[8], name[32];
    unsigned short int w, h, c;
    int height, width, ii;
    uint32_t y;
    int bpp, code;
    int st = 0;
    char * colors = NULL;

    /* Carga los datos de cabecera */
    if ( file_read( fp, header, sizeof( header ) ) != sizeof( header ) ) return NULL;

    if ( strcmp( header, M32_MAGIC ) == 0 ) bpp = 32;
    else if ( strcmp( header, M16_MAGIC ) == 0 ) bpp = 16;
    else if ( strcmp( header, MAP_MAGIC ) == 0 ) bpp = 8;
    else if ( strcmp( header, M01_MAGIC ) == 0 ) bpp = 1;
    else return NULL;

    file_readUint16( fp, &w );
    file_readUint16( fp, &h );
    file_readSint32( fp, &code );

    height = h;
    width = w;

    if ( file_read( fp, name, sizeof( name ) ) != sizeof( name ) ) return NULL;
    name[31] = 0;

    if ( bpp == 8 && !( colors = gr_read_pal_with_gamma( fp ) ) ) return NULL;

    SDL_Palette * pal = NULL;
    if ( colors ) {
        pal = SDL_AllocPalette( 256 );
        if ( !pal ) {
            free( colors );
            return NULL;
        }

        SDL_Color * cl = malloc( sizeof( SDL_Color ) * 256 );
        if ( !cl ) {
            SDL_FreePalette( pal );
            free( colors );
            return NULL;
        }

        int i;

        for ( i = 0; i < 256; i++ ) {
            cl[i].r = colors[i*3];
            cl[i].g = colors[i*3+1];
            cl[i].b = colors[i*3+2];
            cl[i].a = 255;
        }
        free( colors );
        SDL_SetPaletteColors( pal, (const SDL_Color *)cl, 0, 256 );
        free( cl );
    }

    /* Control points */

    file_readUint16( fp, &c );
    int ncpoints = c;
    CPOINT * cpoints = NULL;

    if ( ncpoints ) {
        cpoints = ( CPOINT * ) malloc( ncpoints * sizeof( CPOINT ) );
        if ( !cpoints ) {
            if ( pal ) SDL_FreePalette( pal );
            return NULL;
        }

        for ( c = 0; c < ncpoints; c++ ) {
            file_readUint16( fp, &w );
            file_readUint16( fp, &h );
            if (( short int ) w == -1 && ( short int ) h == -1 ) {
                w = CPOINT_UNDEFINED;
                h = CPOINT_UNDEFINED;
            }
            cpoints[c].x = w;
            cpoints[c].y = h;
        }
    }

    /* Graph header */

    uint32_t rmask, gmask, bmask, amask;
    getRGBA_mask( bpp, &rmask, &gmask, &bmask, &amask );

    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, bpp, rmask, gmask, bmask, amask );
    if ( !surface ) {
        if ( pal ) SDL_FreePalette( pal );
        free( cpoints );
        return NULL;
    }

    if ( pal ) SDL_SetSurfacePalette( surface, pal );

    // Set transparent color
    if ( bpp != 32 ) {
        if ( bpp == 1 ) SDL_SetColorKey( surface, SDL_TRUE, 1 );
        else            SDL_SetColorKey( surface, SDL_TRUE, 0 );
    }

    /* Graphic data */

    int widthb = width * bpp / 8;
    if (( widthb * 8 / bpp ) < width ) widthb++;

    for ( y = 0; y < height; y++ ) {
        uint8_t * line = ( uint8_t * ) surface->pixels + surface->pitch * y;

        switch ( bpp ) {
            case    32:
                st = file_readUint32A( fp, ( uint32_t * ) line, width );
                break;

            case    16:
                st = file_readUint16A( fp, ( uint16_t * ) line, width );
                break;

            case    8:
                if ( ( st = file_read( fp, line, widthb ) ) != widthb ) st = 0;
                break;

            case    1:
                if ( ( st = file_read( fp, line, widthb ) ) != widthb ) st = 0;
                else
                    for ( ii = 0; ii < widthb; ii++ ) line[ii] = ~line[ii];
                break;

        }

        if ( !st ) {
            SDL_FreeSurface( surface );
            if ( pal ) SDL_FreePalette( pal );
            free( cpoints );
            return 0;
        }
    }

    GRAPH *gr = bitmap_new( code, 0, 0, surface );
    SDL_FreeSurface( surface );
    if ( !gr ) {
        if ( pal ) SDL_FreePalette( pal );
        free( cpoints );
        return 0;
    }

    strcpy( gr->name, name );
    gr->ncpoints = ncpoints;
    gr->cpoints = cpoints;

    return gr;
}

/* --------------------------------------------------------------------------- */
/* Publics                                                                     */
/* --------------------------------------------------------------------------- */

int64_t gr_load_map( const char * mapname ) {
    GRAPH * gr;
    file * fp = file_open( mapname, "rb" );
    if ( !fp ) return 0;

    gr = gr_read_map( fp );
    file_close( fp );

    if ( !gr ) return 0;

    // Don't matter the file code, we must force a new code...
    gr->code = bitmap_next_code();

    grlib_add_map( 0, gr );
    return gr->code;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : gr_font_load
 *
 *  Load a font from a given file, in FNT (DIV) format
 *
 *  PARAMS :
 *  filename  Name of the file
 *
 *  RETURN VALUE :
 *      ID of the new font, or -1 if error
 *
 */

int64_t gr_font_load( char * filename ) {
    file * fp;
    int64_t result;
    if ( !filename ) return -1;
    fp = file_open( filename, "rb" );
    if ( !fp ) return -1;
    result = gr_font_loadfrom( fp );
    file_close( fp );
    return result;
}

/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : gr_load_fpg
 *
 *  Load a FPG file
 *
 *  PARAMS :
 *  libname   Name of the file
 *
 *  RETURN VALUE :
 *      Internal ID of the library or -1 if invalid file
 *
 */

int64_t gr_load_fpg( const char * libname ) {
    int libid;
    file * fp = file_open( libname, "rb" );
    if ( !fp ) return -1;
    libid = gr_read_lib( fp );
    file_close( fp );
    return libid;
}

/* --------------------------------------------------------------------------- */
#if 0

/* --------------------------------------------------------------------------- */

int gr_save_map( GRAPH * gr, char * filename )
{
    int c, st = 0;
    MAP_HEADER mh ;
    uint8_t gamma[576];
    uint16_t cpoints;

    if ( !gr ) return 0;

    file * fp = file_open( filename, "wb" );
    if ( !fp ) return 0;

    switch ( gr->format->depth )
    {
        case    8:
            strcpy( ( char * ) mh.magic, MAP_MAGIC );
            break;

        case    32:
            strcpy( ( char * ) mh.magic, M32_MAGIC );
            break;

        case    16:
            strcpy( ( char * ) mh.magic, M16_MAGIC );
            break;

        case    1:
            strcpy( ( char * ) mh.magic, M01_MAGIC );
            break;
    }

    mh.version = 0x00;
    mh.width = gr->width;
    mh.height = gr->height;
    mh.code = gr->code ;
    strncpy( ( char * ) mh.name, gr->name, 32 );

    file_write( fp, &mh, sizeof( MAP_HEADER ) ) ;

    if ( gr->format->depth == 8 )
    {
        if ( gr->format->palette )
        {
            file_write( fp, gr->format->palette->rgb, 768 ) ;
        }
        else if ( sys_pixel_format->palette )
        {
            file_write( fp, sys_pixel_format->palette->rgb, 768 ) ;
        }
        else
        {
            file_write( fp, default_palette, 768 ) ;
        }
        memset( gamma, '\0', sizeof( gamma ) );
        file_write( fp, gamma, sizeof( gamma ) ) ;
    }

    cpoints = gr->ncpoints;
    file_writeUint16( fp, &cpoints );

    for ( c = 0 ; c < gr->ncpoints ; c++ )
    {
        file_writeUint16( fp, ( uint16_t * ) &gr->cpoints[c].x ) ;
        file_writeUint16( fp, ( uint16_t * ) &gr->cpoints[c].y ) ;
    }

    for ( c = 0 ; c < gr->height ; c++ )
    {
        uint8_t * line = ( uint8_t * )gr->data + gr->pitch * c;

        switch ( gr->format->depth )
        {
            case    32:
                st = file_writeUint32A( fp, ( uint32_t * )line, gr->width );
                break;

            case    16:
                st = file_writeUint16A( fp, ( uint16_t * )line, gr->width );
                break;

            case    8:
            case    1:
                st = file_write( fp, line, gr->widthb );
                break;
        }

        if ( !st ) break;
    }

    file_close( fp ) ;

    return 1;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : gr_font_save
 *
 *  Write a font to disk, in FNT/FNX format
 *
 *  PARAMS :
 *  fontid   ID of the font to save
 *  filename  Name of the file to create
 *
 *  RETURN VALUE :
 *      1 if succeded or 0 otherwise
 *
 */

int gr_font_save( int fontid, const char * filename )
{
    if ( !filename ) return 0;

    file * file;
    int n;
    uint32_t y;
    long offset;
    uint8_t * block = NULL ;
    uint8_t * lineptr;

    FONT * font;
    uint8_t header[8];
    _chardata chardata[256] ;
    int palette_saved = 0;

    if ( fontid < 0 || fontid > MAX_FONTS || !fonts[fontid] ) return 0;

    font = fonts[fontid];

    /* Open the file */

    file = file_open( filename, "wb0" );
    if ( !file ) return 0;

    /* Write the header */

    strcpy( ( char * ) header, FNX_MAGIC );
    header[7] = font->bpp;
    file_write( file, &header, 8 );

    /* Write the character information */

    memset( chardata, 0, sizeof( chardata ) );
    offset = 8 + 4 + (( font->bpp == 8 ) ? 576 + 768 : 0 ) + sizeof( chardata );

    for ( n = 0 ; n < 256 ; n++ )
    {
        chardata[n].xadvance   = font->glyph[n].xadvance;
        chardata[n].yadvance   = font->glyph[n].yadvance;

        if ( font->glyph[n].bitmap )
        {
            /* Write the palette */
            if ( !palette_saved && font->bpp == 8 )
            {
                uint8_t   colors[256][3];
                uint8_t * block = calloc( 576, 1 ) ;
                rgb_component * gpal = NULL;
                int k;

                if ( font->glyph[n].bitmap->format->palette )
                    gpal = font->glyph[n].bitmap->format->palette->rgb;
                else if ( sys_pixel_format->palette )
                    gpal = sys_pixel_format->palette->rgb;
                else
                    gpal = ( rgb_component * ) default_palette;

                /* Generate palette info */
                for ( k = 0 ; k < 256 ; k++ )
                {
                    colors[k][0] = gpal[k].r >> 2 ;
                    colors[k][1] = gpal[k].g >> 2 ;
                    colors[k][2] = gpal[k].b >> 2 ;
                }

                file_write( file, &colors, sizeof(colors) ) ;
                file_write( file, block, 576 ) ;
                free( block ) ;
                palette_saved = 1;
            }

            chardata[n].width      = font->glyph[n].bitmap->width;
            chardata[n].height     = font->glyph[n].bitmap->height;
            chardata[n].xadvance   = font->glyph[n].xadvance;
            chardata[n].yadvance   = font->glyph[n].yadvance;
            chardata[n].xoffset    = font->glyph[n].xoffset;
            chardata[n].yoffset    = font->glyph[n].yoffset;
            chardata[n].fileoffset = offset;

            offset += font->glyph[n].bitmap->widthb * chardata[n].height;
        }

        ARRANGE_DWORD( &chardata[n].xadvance );
        ARRANGE_DWORD( &chardata[n].yadvance );
        ARRANGE_DWORD( &chardata[n].width );
        ARRANGE_DWORD( &chardata[n].width );
        ARRANGE_DWORD( &chardata[n].xoffset );
        ARRANGE_DWORD( &chardata[n].yoffset );
        ARRANGE_DWORD( &chardata[n].fileoffset );
    }

    file_writeSint32( file, &font->charset );

    file_write( file, &chardata, sizeof( chardata ) );

    /* Write the character bitmaps */

    for ( n = 0 ; n < 256 ; n++ )
    {
        if ( font->glyph[n].bitmap )
        {
            GRAPH * gr = font->glyph[n].bitmap;

            if ( gr->format->depth != font->bpp )
            {
                file_close( file );
                return 0;
            }

            if ( gr->format->depth > 8 )
            {
                if (( block = malloc( gr->widthb ) ) == NULL )
                {
                    file_close( file );
                    return 0;
                }
            }

            lineptr = gr->data;

            for ( y = 0 ; y < gr->height ; y++, lineptr += gr->pitch )
            {
                if ( gr->format->depth > 8 )
                {
                    memcpy( block, lineptr, gr->widthb );
                    if ( gr->format->depth == 16 )
                    {
                        ARRANGE_WORDS( block, ( int )gr->width );
/*                        gr_convert16_ScreenTo565(( uint16_t * )block, gr->width ); */
                        file_write( file, block, gr->widthb );
                    }
                    else if ( gr->format->depth == 32 )
                    {
                        file_writeUint32A( file, ( uint32_t * ) block, gr->width );
                    }
                }
                else
                {
                    file_write( file, lineptr, gr->widthb );
                }
            }

            if ( gr->format->depth > 8 ) free( block );
        }
    }

    file_close( file );

    return 1;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : gr_save_fpg
 *
 *  Save a FPG file
 *
 *  PARAMS :
 *  filename  Name of the file
 *
 *  RETURN VALUE :
 *      1 if succeded
 *      >= 0 error
 *
 */

int gr_save_fpg( int libid, const char * filename )
{
    file  * fp;
    GRLIB * lib;
    int     n, y, c;
    int     bpp;
    int     nmaps;
    uint8_t header[8];
    rgb_component * palette = NULL;

    /* Get the library and open the file */
/*
    if ( !libid )
        lib = syslib ;
    else
*/
        lib = grlib_get( libid );

    if ( !lib ) return 0;

    /* Guess the FPG depth */

    nmaps = lib->map_reserved < 1000 ? lib->map_reserved : 1000;

    for ( bpp = n = 0 ; n < nmaps ; n++ ) {
        if ( lib->maps[n] ) {
            if ( bpp && lib->maps[n]->format->depth != bpp ) return 0; /* save_fpg: don't allow save maps with differents bpp */
            bpp = lib->maps[n]->format->depth;
            if ( !palette && bpp == 8 && lib->maps[n]->format->palette ) palette = lib->maps[n]->format->palette->rgb;
        }
    }

    if ( bpp == 32 ) strcpy( ( char * ) header, F32_MAGIC );
    else if ( bpp == 16 ) strcpy( ( char * ) header, F16_MAGIC );
    else if ( bpp == 8 ) strcpy( ( char * ) header, FPG_MAGIC );
    else if ( bpp == 1 ) strcpy( ( char * ) header, F01_MAGIC );
    else return 0; /* No maps for save */

    /* Create fpg */

    fp = file_open( filename, "wb" );
    if ( !fp ) return 0;

    /* Write the header */

    header[7] = 0x00; /* Version */
    file_write( fp, header, sizeof( header ) );

    /* Write the color palette */

    if ( bpp == 8 ) {
        uint8_t colors[256][3] ;
        uint8_t gamma[576];

        if ( !palette ) {
            if ( sys_pixel_format->palette )
                palette = sys_pixel_format->palette->rgb;
            else
                palette = ( rgb_component * ) default_palette;
        }

        for ( n = 0 ; n < 256 ; n++ ) {
            colors[n][0] = palette[n].r >> 2 ;
            colors[n][1] = palette[n].g >> 2 ;
            colors[n][2] = palette[n].b >> 2 ;
        }

        file_write( fp, &colors, 768 ) ;
        memset( gamma, '\0', sizeof( gamma ) );
        file_write( fp, gamma, sizeof( gamma ) ) ;
    }

    /* Write each map */

    for ( n = 0 ; n < nmaps ; n++ ) {
        GRAPH * gr = lib->maps[n];

        if ( gr ) {
            /* Write the bitmap header */

            chunk.code = n;
            chunk.regsize = 0;
            chunk.width = gr->width;
            chunk.height = gr->height;
            chunk.flags = gr->ncpoints;
            chunk.fpname[0] = 0;
            strncpy( chunk.name,  gr->name,  sizeof( chunk.name ) );

            ARRANGE_DWORD( &chunk.code );
            ARRANGE_DWORD( &chunk.regsize );
            ARRANGE_DWORD( &chunk.width );
            ARRANGE_DWORD( &chunk.height );
            ARRANGE_DWORD( &chunk.flags );

            file_write( fp, &chunk, sizeof( chunk ) );

            /* Write the control points */

            for ( c = 0 ; c < gr->ncpoints ; c++ ) {
                file_writeSint16( fp, &gr->cpoints[c].x );
                file_writeSint16( fp, &gr->cpoints[c].y );
            }

            /* Write the bitmap pixel data */

            for ( y = 0 ; y < gr->height ; y++ ) {
                switch( bpp ) {
                    case    32:
                            file_writeUint32A( fp, ( uint32_t * ) ( ( uint8_t * )gr->data + gr->pitch*y), gr->width );
                            break;

                    case    16:
                            file_writeUint16A( fp, ( uint16_t * ) ( ( uint8_t * )gr->data + gr->pitch*y), gr->width );
                            break;

                    case    8:
                    case    1:
                            file_write( fp, ( uint8_t * )gr->data + gr->pitch*y, gr->widthb );
                            break;
                }
            }
        }
    }

    file_close( fp );
    return 1;
}

/* --------------------------------------------------------------------------- */
#endif
