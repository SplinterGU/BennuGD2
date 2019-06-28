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

#include <SDL.h>
#include <stdlib.h>
#include <string.h>

#include "libbggfx.h"
#include "bitwise_map.h"

// #define __DISABLE_PALETTES__

/* --------------------------------------------------------------------------- */

uint64_t * map_code_bmp = NULL;
int64_t map_code_allocated = 0;
int64_t map_code_last = 0;

/* --------------------------------------------------------------------------- */

void getRGBA_mask( int bpp, uint32_t * rmask, uint32_t * gmask, uint32_t * bmask, uint32_t * amask ) {
    if ( bpp == 32 ) {
        * rmask = 0x00ff0000;
        * gmask = 0x0000ff00;
        * bmask = 0x000000ff;
        * amask = 0xff000000;
    } else
    if ( bpp == 24 ) {
        * rmask = 0x00ff0000;
        * gmask = 0x0000ff00;
        * bmask = 0x000000ff;
        * amask = 0;
    } else
    if ( bpp == 16 ) {
        * rmask = 0xf800;
        * gmask = 0x07e0;
        * bmask = 0x001f;
        * amask = 0;
    } else {
        * rmask = 0;
        * gmask = 0;
        * bmask = 0;
        * amask = 0;
    }
}

/* --------------------------------------------------------------------------- */

GRAPH * bitmap_new( int64_t code, int64_t width, int64_t height, SDL_Surface * surface ) {
    GRAPH * gr;
    int64_t w = width, h = height;

    if ( surface ) {
        w = surface->w;
        h = surface->h;
    }

    if ( w < 1 || h < 1 ) {
        return NULL;
    }

    /* Create and fill the struct */

    if ( !( gr = ( GRAPH * ) malloc( sizeof( GRAPH ) ) ) ) {
        return NULL; // no memory
    }

    /* Access:
        SDL_TEXTUREACCESS_STATIC
        SDL_TEXTUREACCESS_STREAMING
        SDL_TEXTUREACCESS_TARGET
    */
    if ( surface ) {
#ifdef __DISABLE_PALETTES__
        if ( surface->format->format == SDL_PIXELFORMAT_ARGB8888 /*|| surface->format->format == SDL_PIXELFORMAT_RGB565*/ ) {
            gr->surface = SDL_ConvertSurface(surface, surface->format, 0);
        } else {
            // Set transparent color
//                 if ( surface->format->BitsPerPixel == 1 )   SDL_SetColorKey( surface, SDL_TRUE, 1 );
//            else                                             SDL_SetColorKey( surface, SDL_TRUE, 0 );

            gr->surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_ARGB8888, 0);
        }
#else
//        if ( surface->format->format != SDL_PIXELFORMAT_ARGB8888 ) {
            // Set transparent color
//                 if ( surface->format->BitsPerPixel == 1 )   SDL_SetColorKey( surface, SDL_TRUE, 1 );
//            else                                             SDL_SetColorKey( surface, SDL_TRUE, 0 );
//        }
        // SDL_SetColorKey( surface, SDL_FALSE, 0 ); // Disable Color Key
        gr->surface = SDL_ConvertSurface(surface, surface->format, 0);
#endif
        if ( !gr->surface ) {
            free( gr );
            return NULL;
        }
    } else {
        gr->surface = NULL;
    }

    gr->type = 0;
    gr->texture = NULL;

    gr->texture_must_update = 0;

    gr->width = w;
    gr->height = h;

    gr->code = code;
    gr->name[ 0 ] = '\0';

    gr->ncpoints = 0;
    gr->cpoints = NULL;

    gr->nsegments = 0;
    gr->segments = NULL;

    return gr;
}

/* --------------------------------------------------------------------------- */

GRAPH * bitmap_clone( GRAPH * map ) {
    GRAPH * gr;

    if ( !( gr = bitmap_new( 0, map->width, map->height, map->surface ) ) ) return NULL;

    if ( map->cpoints ) {
        gr->cpoints = malloc( sizeof( CPOINT ) * map->ncpoints );
        memcpy( gr->cpoints, map->cpoints, sizeof( CPOINT ) * map->ncpoints );
        gr->ncpoints = map->ncpoints;
    }

    memcpy( gr->name, map->name, sizeof( map->name ) );

    return gr;
}

/* --------------------------------------------------------------------------- */

void bitmap_add_cpoint( GRAPH * map, int64_t x, int64_t y ) {
    map->cpoints = ( CPOINT * ) realloc( map->cpoints, ( map->ncpoints + 1 ) * sizeof( CPOINT ) );
    map->cpoints[ map->ncpoints ].x = x;
    map->cpoints[ map->ncpoints ].y = y;
    map->ncpoints++;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : bitmap_set_cpoint
 *
 *  Set a control point in a graphic
 *
 *  PARAMS :
 *      map             Pointer to the bitmap
 *      point           Control point index
 *      x               New X coordinate or CPOINT_UNDEFINED to unset
 *      y               New Y coordinate or CPOINT_UNDEFINED to unset
 *
 *  RETURN VALUE :
 *      None
 *
 */

void bitmap_set_cpoint( GRAPH * map, uint64_t point, int64_t x, int64_t y ) {
    uint64_t n;

    if ( point < 0 ) return;

    if ( map->ncpoints <= point ) {
        map->cpoints = ( CPOINT * ) realloc( map->cpoints, ( point + 1 ) * sizeof( CPOINT ) );
        for ( n = map->ncpoints; n < point; n++ ) {
            map->cpoints[ n ].x = CPOINT_UNDEFINED;
            map->cpoints[ n ].y = CPOINT_UNDEFINED;
        }
        map->ncpoints = point + 1;
    }
    map->cpoints[ point ].x = x;
    map->cpoints[ point ].y = y;
}

/* --------------------------------------------------------------------------- */

void bitmap_destroy( GRAPH * map ) {
    if ( !map ) return;
    if ( map->cpoints ) free( map->cpoints );
    if ( map->code > 999 ) bit_clr( map_code_bmp, map->code - 1000 );
    if ( map->surface ) SDL_FreeSurface( map->surface );
    if ( map->texture ) SDL_DestroyTexture( map->texture );
    if ( map->nsegments ) {
        int i;
        for ( i = 0; i < map->nsegments; i++ ) {
            SDL_DestroyTexture( map->segments[i].texture );
        }
    }
    free( map );
}

/* --------------------------------------------------------------------------- */
/* Returns the code of a new system library graph (1000+). Searchs
   for free slots if the program creates too many system maps */

int64_t bitmap_next_code() {
    int n, nb, lim, ini;

    // Si tengo suficientes alocados, retorno el siguiente segun map_code_last
    if ( map_code_last < map_code_allocated ) {
        if ( !bit_tst( map_code_bmp, map_code_last ) ) {
            bit_set( map_code_bmp, map_code_last );
            return 1000 + map_code_last++;
        }
    }

    // Ya no tengo mas espacio, entonces busco alguno libre entre ~+64 desde el ultimo fijo y ~-64 del ultimo asignado

    ini = ( map_code_last < map_code_allocated ) ? ( map_code_last >> 6 ) : 0;
    lim = ( map_code_allocated >> 6 );

    while ( 1 ) {
        for ( n = ini; n < lim; n++ ) {
            if ( map_code_bmp[n] != ( uint64_t ) 0xFFFFFFFFFFFFFFFF ) { // Aca hay 1 libre, busco cual es
                for ( nb = 0; nb < 64; nb++ ) {
                    if ( !bit_tst( map_code_bmp + n, nb ) ) {
                        map_code_last = ( n << 6 ) + nb;
                        bit_set( map_code_bmp, map_code_last );
                        return 1000 + map_code_last++;
                    }
                }
            }
        }
        if ( !ini ) break;
        lim = ini;
        ini = 0;
    }

    map_code_last = map_code_allocated;

    // Increment space, no more slots availables
    // 256 new maps available for alloc

    map_code_allocated += 256;
    map_code_bmp = ( uint64_t * ) realloc( map_code_bmp, sizeof( uint64_t ) * ( map_code_allocated >> 6 ) );

    memset( &map_code_bmp[( map_code_last >> 6 )], 0, sizeof( uint64_t ) * ( ( map_code_allocated - map_code_last ) >> 6 ) );

    // Devuelvo map_code_last e incremento en 1, ya que ahora tengo BLOCK_INCR mas que antes
    bit_set( map_code_bmp, map_code_last );
    return 1000 + map_code_last++;

}

/* --------------------------------------------------------------------------- */
