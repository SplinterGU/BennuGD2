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
#include <stdio.h>
#include <string.h>

#include "g_bitmap.h"
#include "g_grlib.h"
#include "bitwise_map.h"

#include "libbggfx.h"

/* --------------------------------------------------------------------------- */

GRLIB * syslib = 0;

static uint64_t * libs_bmp = NULL;
static int64_t libs_allocated = 0;
static int64_t libs_last = 0;
static GRLIB ** libs = NULL;

/* --------------------------------------------------------------------------- */

static GRLIB * grlib_create();

static GRAPH * scrbitmap = NULL;

/* --------------------------------------------------------------------------- */

int64_t grlib_newid() {
    int64_t n, nb, lim, ini;

    // Si tengo suficientes alocados, retorno el siguiente segun libs_last
    if ( libs_last < libs_allocated ) {
        if ( !bit_tst( libs_bmp, libs_last ) ) {
            bit_set( libs_bmp, libs_last );
            return libs_last++;
        }
    }

    // Ya no tengo mas espacio, entonces busco alguno libre entre ~+32 desde el ultimo fijo y ~-32 del ultimo asignado

    ini = ( libs_last < libs_allocated ) ? ( libs_last >> 6 ) : 0;
    lim = ( libs_allocated >> 6 );

    while ( 1 ) {
        for ( n = ini; n < lim ; n++ ) {
            if ( libs_bmp[n] != ( uint64_t ) 0xFFFFFFFFFFFFFFFF ) { // Aca hay 1 libre, busco cual es
                for ( nb = 0; nb < 64; nb++ ) {
                    if ( !bit_tst( libs_bmp + n, nb ) ) {
                        libs_last = ( n << 6 ) + nb;
                        bit_set( libs_bmp, libs_last );
                        return libs_last++;
                    }
                }
            }
        }
        if ( !ini ) break;
        lim = ini;
        ini = 0;
    }

    libs_last = libs_allocated;

    /* Increment space, no more slots availables
       256 new maps available for alloc */

    libs_allocated += 256;
    libs_bmp = ( uint64_t * ) realloc( libs_bmp, sizeof( uint64_t ) * ( libs_allocated >> 6 ) );
    memset( &libs_bmp[( libs_last >> 6 )], 0, sizeof( uint64_t ) * ( libs_allocated >> 6 ) );

    libs = ( GRLIB ** ) realloc( libs, sizeof( GRLIB * ) * libs_allocated );
    memset( &libs[ libs_last ], 0, sizeof( GRLIB * ) * 256 );

    /* Devuelvo libs_last e incremento en 1, ya que ahora tengo BLOCK_INCR mas que antes */
    bit_set( libs_bmp, libs_last );

    return libs_last++;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : grlib_new
 *
 *  Create a new library
 *
 *  PARAMS :
 *  None
 *
 *  RETURN VALUE :
 *      ID of the new library or -1 if error
 */

int64_t grlib_new() {
    GRLIB * lib = grlib_create();
    int64_t i;

    if ( !lib ) return -1;

    i = grlib_newid();
    libs[ i ] = lib;

    return ( i );
}

/* --------------------------------------------------------------------------- */
/* Static convenience function */

static GRLIB * grlib_create() {
    GRLIB * lib;

    lib = ( GRLIB * ) malloc( sizeof( GRLIB ) );
    if ( !lib ) return 0;

    lib->maps = ( GRAPH ** ) calloc( 32, sizeof( GRAPH * ) );
    if ( !lib->maps ) {
        free( lib );
        return 0;
    }

    lib->name[ 0 ] = 0;
    lib->map_reserved = 32;

    return lib;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : grlib_get
 *
 *  Get a pointer to an library in memory given the internal ID
 *
 *  PARAMS :
 *  libid   Library code
 *
 *  RETURN VALUE :
 *      Pointer to the object required or NULL if it does not exist
 */

GRLIB * grlib_get( int64_t libid ) {
    if ( libid < 0 || libid >= libs_allocated ) return 0;
    return libs[ libid ];
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : grlib_destroy
 *
 *  Remove a library and all its maps from memory
 *
 *  PARAMS :
 *  libid   ID of the library
 *
 *  RETURN VALUE :
 *      0 if there wasn't a map with this code, 1 otherwise
 */

void grlib_destroy( int64_t libid ) {
    int i;
    GRLIB * lib = grlib_get( libid );
    if ( !lib ) return;

    libs[ libid ] = 0;

    for ( i = 0; i < lib->map_reserved; i++ ) bitmap_destroy( lib->maps[ i ] );

    free( lib->maps );
    free( lib );

    bit_clr( libs_bmp, libid );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : grlib_unload_map
 *
 *  Remove a map from a library, if present
 *
 *  PARAMS :
 *  libid   ID of the library
 *  mapcode   ID of the map
 *
 *  RETURN VALUE :
 *      0 if there wasn't a map with this code, 1 otherwise
 */

int64_t grlib_unload_map( int64_t libid, int64_t mapcode ) {
    GRLIB * lib;

    if ( mapcode < 1 || mapcode > 999 ) lib = syslib;
    else                                lib = grlib_get( libid );

    if ( lib == NULL ) return 0;

    if ( lib->map_reserved <= mapcode ) return 0;
    if ( !lib->maps[ mapcode ] ) return 0;

    bitmap_destroy( lib->maps[ mapcode ] );
    lib->maps[ mapcode ] = 0;
    return 1;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : grlib_add_map
 *
 *  Add a map to a graphic library. If any map with the same code
 *  is in the library, it will be unload first
 *
 *  PARAMS :
 *  lib    Pointer to the library
 *  map    Pointer to the graphic
 *         (fill map->code first!)
 *
 *  RETURN VALUE :
 *      -1 if error, a number >= 0 otherwise
 */

int64_t grlib_add_map( int64_t libid, GRAPH * map ) {
    GRLIB * lib;

    if ( map->code < 1 || map->code > 999 ) lib = syslib;
    else                                    lib = grlib_get( libid );

    if ( !lib ) return -1;
    if ( map->code < 0 ) return -1;

    if ( map->code > 0 ) grlib_unload_map( libid, map->code );

    if ( lib->map_reserved <= map->code ) {
        GRAPH ** lmaps;
        int new_reserved = ( map->code & ~0x003F ) + 64;

        lmaps = ( GRAPH ** ) realloc( lib->maps, sizeof( GRAPH* ) * new_reserved );
        if ( !lmaps ) return -1; // No memory
        lib->maps = lmaps;

        memset( lib->maps + lib->map_reserved, 0, ( new_reserved - lib->map_reserved ) * sizeof( GRAPH * ) );
        lib->map_reserved = new_reserved;
    }

    lib->maps[ map->code ] = map;

    return map->code;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : bitmap_get
 *
 *  Get a bitmap using the DIV syntax (libcode, mapcode)
 *
 *  PARAMS :
 *  libid   Library code or 0 for system/global bitmap
 *  mapcode   Code of the bitmap
 *
 *  RETURN VALUE :
 *      Pointer to the graphic required or NULL if not found
 *
 *  (0, -1) gets the scrbitmap graphic (undocumented!)
 *  (0,  0) gets the background
 *
 */

GRAPH * bitmap_get( int64_t libid, int64_t mapcode ) {
    GRLIB * lib = NULL;
    if ( !libid ) {
        #if 0
        /* Using (0, 0) we can get the background map */
        if ( !mapcode ) return background;
        #endif

        /* Using (0, -1) we can get the screen bitmap (undocumented bug/feature) */
        if ( mapcode == -1 ) {
            if ( scrbitmap && ( scrbitmap->width != scr_width || scrbitmap->height != scr_height ) ) {
                bitmap_destroy( scrbitmap );
                scrbitmap = NULL;
            }

            if ( !scrbitmap ) {
                scrbitmap = bitmap_new( 0, scr_width, scr_height, NULL );
            }

            if ( scrbitmap ) {
                if ( !scrbitmap->surface ) {
                    uint32_t rmask, gmask, bmask, amask;
                    getRGBA_mask( 32, &rmask, &gmask, &bmask, &amask );
                    scrbitmap->surface = SDL_CreateRGBSurface(0, scr_width, scr_height, 32, rmask, gmask, bmask, amask );
                }

                if ( scrbitmap->surface ) {
                    int r = SDL_RenderReadPixels( gRenderer,
                                                  NULL,
                                                  scrbitmap->surface->format->format,
                                                  scrbitmap->surface->pixels,
                                                  scrbitmap->surface->pitch );
                    if ( r ) {
                        printf( "ERROR: %s\n", SDL_GetError() );
                    }
                    return scrbitmap;
                }
            }
            return NULL;
        }
    }

    /* Get the map from the system library
     * (the only one that can have more than 1000 maps)
     */
    if ( mapcode > 999 ) lib = syslib;

    if ( !lib ) lib = grlib_get( libid );

    /* Get the map from a library */

    if ( lib && lib->map_reserved > mapcode && mapcode >= 0 ) return lib->maps[ mapcode ];

    return 0;
}

/* --------------------------------------------------------------------------- */

GRAPH * bitmap_new_syslib( int64_t w, int64_t h ) {
    GRAPH * gr;

    gr = bitmap_new( 0, w, h, NULL );
    if ( !gr ) return NULL;

    gr->code = bitmap_next_code();
    if ( grlib_add_map( 0, gr ) == -1 ) {
        bitmap_destroy( gr );
        return NULL;
    }

    return gr;
}


/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : grlib_init
 *
 *  Create the system library. This should be called at program startup.
 *
 *  PARAMS :
 *  None
 *
 *  RETURN VALUE :
 *      None
 *
 */

void grlib_init() {
    if ( !syslib ) syslib = grlib_create();
}

/* --------------------------------------------------------------------------- */
