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

    if ( w < 1 || h < 1 ) return NULL;

    /* Create and fill the struct */

    if ( !( gr = ( GRAPH * ) malloc( sizeof( GRAPH ) ) ) ) return NULL; // no memory

    /* Access:
        SDL_TEXTUREACCESS_STATIC
        SDL_TEXTUREACCESS_STREAMING
        SDL_TEXTUREACCESS_TARGET
    */
    if ( surface ) {
#ifdef USE_SDL2
 #ifdef __DISABLE_PALETTES__
        if ( surface->format->format == gPixelFormat->format /*|| surface->format->format == SDL_PIXELFORMAT_RGB565*/ ) {
            gr->surface = SDL_ConvertSurface(surface, surface->format, 0);
        } else {
            gr->surface = SDL_ConvertSurfaceFormat(surface, gPixelFormat->format, 0);
        }
 #else
        gr->surface = SDL_ConvertSurface(surface, surface->format, 0);
        if ( !gr->surface ) {
            free( gr );
            return NULL;
        }
 #endif
#endif
#ifdef USE_SDL2_GPU
        // If bitmap is largest that max texture size then copy texture for slice it in the blitter
        if ( gMaxTextureSize && ( w > gMaxTextureSize || h > gMaxTextureSize ) ) {
            gr->surface = SDL_ConvertSurface(surface, surface->format, 0);
            if ( !gr->surface ) {
                free( gr );
                return NULL;
            }
            gr->tex = NULL;
        } else {
            gr->surface = NULL;
            gr->tex = GPU_CopyImageFromSurface( surface );
            if ( !gr->tex ) {
//                fprintf(stderr, "NEW_BITMAP error creando image from surface %s\n", GPU_GetErrorString(e.error));
                free( gr );
                return NULL;
            }
            GPU_SetSnapMode( gr->tex, GPU_SNAP_NONE );
        }
#endif
    } else {
        gr->surface = NULL;
#ifdef USE_SDL2_GPU
        gr->tex = NULL;
        // If bitmap is largest that max texture size then return with error
        if ( gMaxTextureSize && ( width > gMaxTextureSize || height > gMaxTextureSize ) ) {
            free( gr );
            return NULL;
        }
#endif
    }

    gr->type = 0;

#ifdef USE_SDL2
    gr->tex = NULL;
    gr->texture_must_update = 0;
#endif

    gr->width = w;
    gr->height = h;

    gr->code = code;
    gr->name[ 0 ] = '\0';

    gr->ncpoints = 0;
    gr->cpoints = NULL;

    gr->ncboxes = 0;
    gr->cboxes = NULL;

    gr->nsegments = 0;
    gr->segments = NULL;

    gr->dirty = 1;

    return gr;
}

/* --------------------------------------------------------------------------- */

GRAPH * bitmap_clone( GRAPH * map ) {
    GRAPH * gr;

    if ( !( gr = bitmap_new( 0, map->width, map->height, map->surface ) ) ) return NULL;
#ifdef USE_SDL2_GPU
    if ( map->tex ) gr->tex = GPU_CopyImage( map->tex );
#endif

    if ( map->cpoints ) {
        gr->cpoints = malloc( sizeof( CPOINT ) * map->ncpoints );
        if ( !gr->cpoints ) {
            bitmap_destroy( gr );
            return NULL;
        }
        memcpy( gr->cpoints, map->cpoints, sizeof( CPOINT ) * map->ncpoints );
        gr->ncpoints = map->ncpoints;
    }

    if ( map->cboxes ) {
        gr->cboxes = malloc( sizeof( CBOX ) * map->ncboxes );
        if ( !gr->cboxes ) {
            bitmap_destroy( gr );
            return NULL;
        }
        memcpy( gr->cboxes, map->cboxes, sizeof( CBOX ) * map->ncboxes );
        gr->ncboxes = map->ncboxes;
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
    if ( map->ncpoints <= point ) {
        map->cpoints = ( CPOINT * ) realloc( map->cpoints, ( point + 1 ) * sizeof( CPOINT ) );
        uint64_t n;
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
    if ( map->cboxes ) free( map->cboxes );
    if ( map->code > 999 ) bit_clr( map_code_bmp, map->code - 1000 );

    if ( map->surface ) SDL_FreeSurface( map->surface );
#ifdef USE_SDL2
    if ( map->tex ) SDL_DestroyTexture( map->tex );
#endif
#ifdef USE_SDL2_GPU
    if ( map->tex && map->tex->target ) GPU_FreeTarget( map->tex->target );
    GPU_FreeImage( map->tex );
#endif
    if ( map->nsegments ) {
        int i;
        for ( i = 0; i < map->nsegments; i++ ) {
#ifdef USE_SDL2
            SDL_DestroyTexture( map->segments[i].tex );
#endif
#ifdef USE_SDL2_GPU
            if ( map->segments[i].tex->target ) GPU_FreeTarget( map->segments[i].tex->target );
            GPU_FreeImage( map->segments[i].tex );
#endif
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

    uint64_t * mcb = ( uint64_t * ) realloc( map_code_bmp, sizeof( uint64_t ) * ( ( map_code_allocated + 256 ) >> 6 ) );

    if ( !mcb ) return -1;

    map_code_allocated += 256;
    map_code_bmp = mcb;

    memset( &map_code_bmp[( map_code_last >> 6 )], 0, sizeof( uint64_t ) * ( ( map_code_allocated - map_code_last ) >> 6 ) );

    // Devuelvo map_code_last e incremento en 1, ya que ahora tengo BLOCK_INCR mas que antes
    bit_set( map_code_bmp, map_code_last );
    return 1000 + map_code_last++;

}

/* --------------------------------------------------------------------------- */

static int compare_cbox( const void * a, const void * b ) {
    const CBOX * A = ( const CBOX * ) a;
    const CBOX * B = ( const CBOX * ) b;

    return A->code - B->code;
}

/* --------------------------------------------------------------------------- */

static int search_cbox( const void * k, const void * b ) {
    int64_t key = *( int64_t * ) k;
    CBOX * B = ( CBOX * ) b;

    return key - B->code;
}

/* --------------------------------------------------------------------------- */

static inline CBOX * __bitmap_search_cbox( GRAPH * map, int64_t code ) {
    return bsearch( &code, map->cboxes, map->ncboxes, sizeof( CBOX ), search_cbox );
}

/* --------------------------------------------------------------------------- */

void bitmap_set_cbox( GRAPH * map, int64_t code, int64_t shape, int64_t x, int64_t y, int64_t width, int64_t height ) {
    CBOX * p = __bitmap_search_cbox( map, code );
    if ( !p ) {
        map->cboxes = ( CBOX * ) realloc( map->cboxes, ( map->ncboxes + 1 ) * sizeof( CBOX ) );
        p = &map->cboxes[ map->ncboxes ];
        map->ncboxes++;
    }

    p->code     = code;
    p->shape    = shape;
    p->x        = x;
    p->y        = y;
    p->width    = width;
    p->height   = height;

    qsort( map->cboxes, map->ncboxes, sizeof( CBOX ), compare_cbox );
}

/* --------------------------------------------------------------------------- */

CBOX * bitmap_get_cbox( GRAPH * map, int64_t code ) {
    return __bitmap_search_cbox( map, code );
}

/* --------------------------------------------------------------------------- */

void bitmap_remove_cbox( GRAPH * map, int64_t code ) {
    CBOX * p = __bitmap_search_cbox( map, code );
    if ( p ) {
        int count = ( map->ncboxes - ( p - map->cboxes + 1 ) );
        if ( count ) memmove( p, p + 1, sizeof( CBOX ) * count );
        map->ncboxes--;
    }
}

/* --------------------------------------------------------------------------- */

CBOX * bitmap_get_cbox_by_pos( GRAPH * map, int64_t pos ) {
    if ( pos < 0 || pos >= map->ncboxes ) return NULL;
    return &map->cboxes[ pos ];
}

/* --------------------------------------------------------------------------- */
