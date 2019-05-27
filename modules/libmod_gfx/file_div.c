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
    int bpp, i, ii;
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

        for ( i = 0; i < 256; i++ ) {
            cl[i].r = colors[i*3];
            cl[i].g = colors[i*3+1];
            cl[i].b = colors[i*3+2];
            cl[i].a = 255;
        }
        free( colors );
        SDL_SetPaletteColors( pal, (const SDL_Color *)cl, 0, 256 );
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
                SDL_FreeSurface( surface );
                if ( pal ) SDL_FreePalette( pal );
                grlib_destroy( libid );
                return -1;
            }
        }

        GRAPH *gr = bitmap_new( chunk.code, 0, 0, surface );
        SDL_FreeSurface( surface );
        if ( !gr ) {
            if ( pal ) SDL_FreePalette( pal );
            grlib_destroy( libid );
            return -1;
        }

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

    if ( font_count == MAX_FONTS ) return -1;

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

    /* Load the character bitmaps */

    for ( i = 0; i < 256; i++ ) {
        f->glyph[i].xadvance = chardata[i].xadvance;
        f->glyph[i].yadvance = chardata[i].yadvance;

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

    if ( f->glyph[32].xadvance == 0 ) f->glyph[32].xadvance = f->glyph['j'].xadvance;

    if ( pal ) SDL_FreePalette( pal );

    return id;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : gr_load_bdf
 *
 *  Load a BDF font from disk. This is a very simple loader that ignores
 *  anything that is not relevant to screen display or non-horizontal
 *  writing fonts.
 *
 *  PARAMS :
 *  filename  Name of the BDF file
 *
 *  RETURN VALUE :
 *      ID of the font if succeded or -1 otherwise
 *
 */
#if 0
int gr_load_bdf( const char * filename )
{
    if ( !filename ) return -1;

    file * fp;
    char line[2048];
    uint8_t * ptr, * optr;
    FONT * font;
    int id, x, y, i;
    int error = 0;

    uint8_t nibbleh[256];
    uint8_t nibblel[256];

    int default_xadvance = 0;
    int default_yadvance = 0;
    int in_char = 0;
    int encoding = -1;
    int width = 0;
    int height = 0;
    int xoffset = 0;
    int yoffset = 0;
    int xadvance = 0;
    int yadvance = 0;
    int minyoffset = 0;
    int len;

    /* Arrays used to convert hex ASCII to binary */

    memset( nibbleh, 0, 256 );
    memset( nibblel, 0, 256 );

    for ( i = '0'; i <= '9'; i++ )
    {
        nibbleh[i] = (( i - '0' ) << 4 );
        nibblel[i] = i - '0';
    }
    for ( i = 10; i <= 15; i++ )
    {
        nibbleh['A' + i - 10] = ( i << 4 );
        nibbleh['a' + i - 10] = ( i << 4 );
        nibblel['A' + i - 10] = i;
        nibblel['a' + i - 10] = i;
    }

    /* Open the file and create the font */

    fp = file_open( filename, "r" );
    if ( !fp ) return -1;

    id = gr_font_new( CHARSET_ISO8859, 1 );
    if ( id < 0 ) return -1;
    font = fonts[id];
    font->maxwidth = 0;
    font->maxheight = 0;

    /* Process the file, a line each time */

    for ( line[2047] = 0;; )
    {
        if ( !( len = file_gets( fp, line, 2047 ) ) ) break;
        if ( line[len-1] == '\n' ) line[len-1] = '\0';

        /* Handle global-level commands */

        if ( strncmp( line, "DWIDTH ", 7 ) == 0 && !in_char )
        {
            default_xadvance = atoi( line + 7 );
            ptr = ( uint8_t * ) strchr( line + 7, ' ' );
            if ( ptr ) default_yadvance = atoi( ( char * ) ptr + 1 );
        }
        else if ( strncmp( line, "STARTCHAR", 9 ) == 0 )
        {
            in_char = 1;
            encoding = -1;
            height = 0;
            xadvance = default_xadvance;
            yadvance = default_yadvance;
        }
        else if ( strncmp( line, "ENDCHAR", 7 ) == 0 )
        {
            in_char = 0;
        }

        /* Handle character-level commands */

        else if ( strncmp( line, "DWIDTH ", 7 ) == 0 && in_char )
        {
            xadvance = atoi( line + 7 );
            ptr = ( uint8_t * ) strchr( line + 7, ' ' );
            if ( ptr ) yadvance = atoi( ( char * ) ptr + 1 );
        }
        else if ( strncmp( line, "ENCODING ", 9 ) == 0 && in_char )
        {
            encoding = atoi( line + 9 );
            if ( encoding == -1 )
            {
                ptr = ( uint8_t * ) strchr( line + 7, ' ' );
                if ( ptr ) encoding = atoi( ( char * ) ptr + 1 );
            }
        }
        else if ( strncmp( line, "BBX ", 4 ) == 0 && in_char )
        {
            width = atoi( ( char * ) line + 4 );
            if ( width & 7 ) width = ( width & ~7 ) + 8;
            if (( ptr = ( uint8_t * ) strchr( ( char * ) line + 4, ' ' ) ) == NULL ) continue;
            height = atoi( ( char * ) ptr + 1 );
            if (( ptr = ( uint8_t * ) strchr( ( char * ) ptr + 1, ' ' ) ) == NULL ) continue;
            xoffset = atoi( ( char * ) ptr + 1 );
            if (( ptr = ( uint8_t * ) strchr( ( char * ) ptr + 1, ' ' ) ) == NULL ) continue;
            yoffset = atoi( ( char * ) ptr + 1 );
        }
        else if ( strncmp( line, "BITMAP", 6 ) == 0 )
        {
            /* Read bitmap data */
            if ( encoding >= 0 && encoding < 256 && height > 0 )
            {
                font->glyph[encoding].xadvance = xadvance;
                font->glyph[encoding].yadvance = yadvance;
                font->glyph[encoding].xoffset  = xoffset;
                font->glyph[encoding].yoffset  = -yoffset - height;

                if ( minyoffset > -yoffset - height ) minyoffset = -yoffset - height;

                error = 1;
                font->glyph[encoding].bitmap = bitmap_new( encoding, width, height, 1 );
                if ( font->glyph[encoding].bitmap == 0 ) break;
                bitmap_add_cpoint( font->glyph[encoding].bitmap, 0, 0 );

                if ( font->maxwidth < width ) font->maxwidth = width;
                if ( font->maxheight < height ) font->maxheight = height;

                for ( y = 0; y < height; y++ )
                {
                    if ( !( len = file_gets( fp, line, 2047 ) ) ) break;
                    if ( line[len-1] == '\n' ) line[len-1] = '\0';
                    ptr  = ( uint8_t * ) line;
                    optr = ( uint8_t * ) font->glyph[encoding].bitmap->data + font->glyph[encoding].bitmap->pitch * y;

                    for ( x = 0; x < width; x += 8 )
                    {
                        if ( !ptr[0] || !ptr[1] ) break;
                        *optr++ = nibbleh[ptr[0]] | nibblel[ptr[1]];
                        ptr += 2;
                    }
                }
                if ( y != height ) break;
                error = 0;
            }
        }
    }

    file_close( fp );

    if ( error )
    {
        gr_font_destroy( id );
        return -1;
    }

    /* Adjust yoffsets to positive */

    for ( i = 0; i < 256; i++ ) font->glyph[i].yoffset -= minyoffset;

    if ( font->glyph[32].xadvance == 0 ) font->glyph[32].xadvance = font->glyph['j'].xadvance;

//    fonts[font_count] = font;
    return id /* font_count++ */;
}
#endif

/* --------------------------------------------------------------------------- */

static GRAPH * gr_read_map( file * fp ) {
    char header[8], name[32];
    unsigned short int w, h, c;
    int height, width, i, ii;
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

        for ( i = 0; i < 256; i++ ) {
            cl[i].r = colors[i*3];
            cl[i].g = colors[i*3+1];
            cl[i].b = colors[i*3+2];
            cl[i].a = 255;
        }
        free( colors );
        SDL_SetPaletteColors( pal, (const SDL_Color *)cl, 0, 256 );
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
