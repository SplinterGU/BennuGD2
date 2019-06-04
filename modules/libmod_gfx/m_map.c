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
#include <SDL_image.h>

#include <stdlib.h>
#include <string.h>

#include "bgdrtm.h"
#include "bgddl.h"

#include "xstrings.h"

#include "libbggfx.h"
#include "libmod_gfx.h"

#include "file_div.h"

/* --------------------------------------------------------------------------- */
/* Maps                                                                        */
/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_map_get_pixel( INSTANCE * my, int64_t * params ) {
    GRAPH * map = bitmap_get( params[ 0 ], params[ 1 ] );
    if ( !map ) return -1;
    return gr_get_pixel( map, params[ 2 ], params[ 3 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_map_put_pixel( INSTANCE * my, int64_t * params ) {
    GRAPH * map = bitmap_get( params[ 0 ], params[ 1 ] );
    if ( !map ) return 0;
    gr_put_pixel( map, params[ 2 ], params[ 3 ], params[ 4 ] );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_graphic_set( INSTANCE * my, int64_t * params ) {
    GRAPH * map;

    map = bitmap_get( params[0], params[1] );
    if ( !map ) return 0;

    switch ( params[2] ) {
        case 2:     /* g_center_x */
            bitmap_set_cpoint( map, 0, params[3], ( map->ncpoints ) ? ( map->cpoints[0].y ) : ( map->height / 2 ) );
            return 1;

        case 3:     /* g_center_y */
            bitmap_set_cpoint( map, 0, ( map->ncpoints ) ? ( map->cpoints[0].x ) : ( map->width / 2 ), params[3] );
            return 1;
    }
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_graphic_info( INSTANCE * my, int64_t * params ) {
    GRAPH * map;

    if ( !params[0] && !params[1] ) {
        switch ( params[2] ) {
            case G_WIDTH:           /* g_wide */
                return scr_width;

            case G_HEIGHT:          /* g_height */
                return scr_height;

            case G_CENTER_X:        /* g_center_x */
                return scr_width / 2;

            case G_CENTER_Y:        /* g_center_y */
                return scr_height / 2;
        }
        return 0;
    }

    map = bitmap_get( params[0], params[1] );
    if ( !map ) return 0;

    switch ( params[2] ) {
        case G_WIDTH:           /* g_wide */
            return map->width;

        case G_HEIGHT:          /* g_height */
            return map->height;

//        case G_PITCH:           /* g_pitch */
//            return map->pitch;

//        case G_DEPTH:           /* g_depth */
//            return map->format->depth;

        case G_CENTER_X:        /* g_center_x */
            if ( map->ncpoints > 0 )
                if ( map->cpoints[0].x != CPOINT_UNDEFINED )
                    return map->cpoints[0].x;
            return map->width / 2;

        case G_CENTER_Y:        /* g_center_y */
            if ( map->ncpoints > 0 )
                if ( map->cpoints[0].y != CPOINT_UNDEFINED )
                    return map->cpoints[0].y;
            return map->height / 2;
    }

    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_set_point( INSTANCE * my, int64_t * params ) {
    GRAPH * bmp = bitmap_get( params[0], params[1] );
    if ( !bmp || params[2] < 0 || params[2] > 999 ) return -1;
    bitmap_set_cpoint( bmp, ( uint32_t )params[2], params[3], params[4] );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_set_center( INSTANCE * my, int64_t * params ) {
    GRAPH * bmp = bitmap_get( params[0], params[1] );
    if ( !bmp ) return -1;
    bitmap_set_cpoint( bmp, 0, params[2], params[3] );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_get_point( INSTANCE * my, int64_t * params ) {
    GRAPH * bmp;

    bmp = bitmap_get( params[0], params[1] );
    if ( !bmp ) return 0;

    /* Use the center as control point if it is not there */
    if ( params[2] == 0 && ( bmp->ncpoints == 0 || bmp->cpoints[0].x == CPOINT_UNDEFINED ) ) {
        *( int64_t * )params[3] = bmp->width / 2;
        *( int64_t * )params[4] = bmp->height / 2;
        return 1;
    }

    if (( uint64_t )params[2] >= bmp->ncpoints || params[2] < 0 )
        return 0;
    if ( bmp->cpoints[params[2]].x == CPOINT_UNDEFINED && bmp->cpoints[params[2]].y == CPOINT_UNDEFINED )
        return 0;

    *( int64_t * )params[3] = bmp->cpoints[params[2]].x;
    *( int64_t * )params[4] = bmp->cpoints[params[2]].y;
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_map_clear( INSTANCE * my, int64_t * params ) {
    GRAPH *map = bitmap_get( params[0], params[1] );
    if ( map ) gr_clear( map );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_map_clear2( INSTANCE * my, int64_t * params ) {
    GRAPH *map = bitmap_get( params[0], params[1] );
    if ( map ) gr_clear_as( map, params[2] );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_map_clear3( INSTANCE * my, int64_t * params ) {
    GRAPH *map = bitmap_get( params[0], params[1] );
    if ( map ) {
        REGION region;
        region.x = MIN( params[2], params[4] );
        region.y = MIN( params[3], params[5] );
        region.x2 = MAX( params[2], params[4] );
        region.y2 = MAX( params[3], params[5] );
        gr_clear_region( map, &region );
    }
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_map_clear4( INSTANCE * my, int64_t * params ) {
    GRAPH *map = bitmap_get( params[0], params[1] );
    if ( map ) {
        REGION region;
        region.x = MIN( params[2], params[4] );
        region.y = MIN( params[3], params[5] );
        region.x2 = MAX( params[2], params[4] );
        region.y2 = MAX( params[3], params[5] );
        gr_clear_region_as( map, &region, params[6] );
    }
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_new_map( INSTANCE * my, int64_t * params ) {
     GRAPH * map;
     map = bitmap_new_syslib( params[0], params[1] );
     return map ? map->code : 0;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_new_map_extend( INSTANCE * my, int64_t * params ) {
    GRAPH * map;
    map = bitmap_new_syslib( params[0], params[1] );
    if ( map && ( params[2] & B_CLEAR )) gr_clear( map );
    return map ? map->code : 0;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_map_clone( INSTANCE * my, int64_t * params ) {
    GRAPH * origin, * map = NULL;
    origin = bitmap_get( params[0], params[1] );
    if ( origin ) map = bitmap_clone( origin );
    if ( !map ) return 0;
    map->code = bitmap_next_code();
    grlib_add_map( 0, map );
    return map->code;
}

/* --------------------------------------------------------------------------- */
/** MAP_PUT(FILE, GRAPH_DEST, FILE_SRC, GRAPH_SRC, X, Y)
 *  Draws a map into another one
 */

int64_t libmod_gfx_map_put( INSTANCE * my, int64_t * params ) {
    GRAPH * dest = bitmap_get( params[0], params[1] );
    GRAPH * orig = bitmap_get( params[2], params[3] );

    if ( !dest || !orig ) return 0;

    gr_blit( dest, NULL, params[4], params[5], 0, 0, 100, 100, orig, NULL, 255, 255, 255, 255 );
    return 1;
}

/* --------------------------------------------------------------------------- */
/** MAP_XPUT(FILE, GRAPH_DEST, FILE_SRC, GRAPH_SRC, X, Y, SCALEX, SCALEY, ANGLE, SIZE, FLAGS, ALPHA, R, G, B)
 *  Draws a map into another one, with most blitter options including flags and alpha
 */

int64_t libmod_gfx_map_xput( INSTANCE * my, int64_t * params ) {
    GRAPH * dest = bitmap_get( params[0], params[1] );
    GRAPH * orig = bitmap_get( params[2], params[3] );
    if ( !dest || !orig ) return 0;

    gr_blit( dest,       // dest
             NULL,       // clip
             params[4],  // x
             params[5],  // y
             params[9],  // flags
             params[6],  // angle
             params[7],  // scalex
             params[8],  // scaley
             orig,       // orig
             NULL,       // orig_clip
             params[10], // alpha
             params[11], // r
             params[12], // g
             params[13]  // b
         );

    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_map_name( INSTANCE * my, int64_t * params ) {
    GRAPH * map = bitmap_get( params[0], params[1] );
    int64_t result;

    if ( !map ) return 0;
    result = string_new( map->name );
    string_use( result );
    return result;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_map_set_name( INSTANCE * my, int64_t * params ) {
    GRAPH * map = bitmap_get( params[0], params[1] );
    const char * ptr = string_get( params[2] );
    if ( map ) {
        strncpy( map->name, ptr, sizeof( map->name ) );
        map->name[sizeof( map->name )-1] = 0;
    }
    string_discard( params[2] );
    return 0;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_map_exists( INSTANCE * my, int64_t * params ) {
    GRAPH * map = bitmap_get( params[0], params[1] );
    return map == NULL ? 0 : 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_map_block_copy( INSTANCE * my, int64_t * params ) {
    GRAPH * dest, * orig;
    REGION clip;
    int64_t centerx, centery, flag;
    uint64_t x, y, w, h, dx, dy;
    uint8_t alpha, r, g, b;

    if ( !( dest = bitmap_get( params[0], params[1] ) ) ) return 0;
    if ( !( orig = bitmap_get( params[0], params[4] ) ) ) return 0;

    x     = params[5];
    y     = params[6];
    w     = params[7];
    h     = params[8];
    dx    = params[2];
    dy    = params[3];
    flag  = params[9];
    alpha = params[10];
    r     = params[11];
    g     = params[12];
    b     = params[13];

    if ( orig->ncpoints > 0 && orig->cpoints[0].x != CPOINT_UNDEFINED ) {
        centerx = orig->cpoints[0].x;
        centery = orig->cpoints[0].y;
    } else {
        centery = orig->height / 2;
        centerx = orig->width / 2;
    }

    if ( flag & B_HMIRROR ) centerx = orig->width - 1 - centerx;
    if ( flag & B_VMIRROR ) centery = orig->height - 1 - centery;

    if ( x  < 0 ) { dx += x;  w += x;   x = 0; }
    if ( y  < 0 ) { dy += y;  h += y;   y = 0; }
    if ( dx < 0 ) {  x += dx; w += dx; dx = 0; }
    if ( dy < 0 ) {  y += dy; h += dy; dy = 0; }

    if ( x + w  > orig->width  ) w = orig->width  - x;
    if ( y + h  > orig->height ) h = orig->height - y;
    if ( dx + w > dest->width  ) w = dest->width  - dx;
    if ( dy + h > dest->height ) h = dest->height - dy;

    if ( w <= 0 || h <= 0 ) return 0;

    clip.x  = dx;
    clip.y  = dy;
    clip.x2 = dx + w - 1;
    clip.y2 = dy + h - 1;

    gr_blit( dest, &clip, dx - x + centerx, dy - y + centery, flag, 0, 100, 100, orig, NULL, alpha, r, g, b );
    return 1;
}

/* --------------------------------------------------------------------------- */
/* Funciones de carga de nivel superior */

/* ------------------------------------- */
/* Interface SDL_RWops Bennu             */
/* ------------------------------------- */

static Sint64 SDLCALL __RWops_seek_cb( SDL_RWops *context, Sint64 offset, int whence ) {
    if ( file_seek( context->hidden.unknown.data1, offset, whence ) < 0 ) return ( -1 );
    return( file_pos( context->hidden.unknown.data1 ) );
}

static size_t SDLCALL __RWops_read_cb( SDL_RWops *context, void *ptr, size_t size, size_t maxnum ) {
    size_t ret = file_read( context->hidden.unknown.data1, ptr, size * maxnum );
    if ( ret > 0 ) ret /= size;
    return( ret );
}

static size_t SDLCALL __RWops_write_cb( SDL_RWops *context, const void *ptr, size_t size, size_t num ) {
    size_t ret = file_write( context->hidden.unknown.data1, ( void * )ptr, size * num );
    if ( ret > 0 ) ret /= size;
    return( ret );
}

static int SDLCALL __RWops_close_cb( SDL_RWops *context ) {
    if ( context ) {
        file_close( context->hidden.unknown.data1 );
        SDL_FreeRW( context );
    }
    return( 0 );
}

static SDL_RWops *SDL_RWFromBGDFP( file *fp ) {
    SDL_RWops *rwops = SDL_AllocRW();
    if ( rwops != NULL ) {
        rwops->seek = __RWops_seek_cb;
        rwops->read = __RWops_read_cb;
        rwops->write = __RWops_write_cb;
        rwops->close = __RWops_close_cb;
        rwops->hidden.unknown.data1 = fp;
    }
    return( rwops );
}

/* --------------------------------------------------------------------------- */

int64_t gr_load_img( const char * mapname ) {
    int64_t map = gr_load_map( mapname );
    if ( !map ) {
        file *fp;

        if ( !( fp = file_open( mapname, "rb0" ) ) ) return ( 0 );

        SDL_Surface* surface = IMG_Load_RW( SDL_RWFromBGDFP( fp ), 1 );
        if ( !surface ) {
            printf( "Unable to load image %s! SDL_image Error: %s\n", mapname, IMG_GetError() );
            return 0;
        }

        // Set transparent color
        SDL_SetColorKey( surface, SDL_TRUE, 0 );

        GRAPH *gr = bitmap_new( 0, 0, 0, surface );
        SDL_FreeSurface( surface );
        if ( !gr ) return 0;

        // Don't matter the file code, we must force a new code...
        gr->code = bitmap_next_code();

        grlib_add_map( 0, gr );
        return gr->code;
    }

    return map;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_load_map( INSTANCE * my, int64_t * params ) {
    int64_t r = gr_load_img( string_get( params[0] ) );
    string_discard( params[0] );
    return r;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_unload_map( INSTANCE * my, int64_t * params ) {
    return grlib_unload_map( params[0], params[1] );
}

/* ---------------------------------------------------------------------- */

// int64_t libmod_gfx_save_map( INSTANCE * my, int64_t * params )
// {
//     int64_t r = ( int64_t ) gr_save_map( bitmap_get( params[0], params[1] ), ( char * )string_get( params[2] ) );
//     string_discard( params[2] );
//     return r;
// }

/* ---------------------------------------------------------------------- */
