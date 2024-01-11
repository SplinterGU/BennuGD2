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
    GRAPH * graph = bitmap_get( params[ 0 ], params[ 1 ] );
    if ( !graph ) return -1;
    return gr_get_pixel( graph, params[ 2 ], params[ 3 ] );
 }

 /* --------------------------------------------------------------------------- */

int64_t libmod_gfx_map_put_pixel( INSTANCE * my, int64_t * params ) {
    GRAPH * graph = bitmap_get( params[ 0 ], params[ 1 ] );
    if ( !graph ) return 0;
    gr_put_pixel( graph, params[ 2 ], params[ 3 ], params[ 4 ] );
    return 1;
}

/* --------------------------------------------------------------------------- */
/* Map Properties                                                              */
/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_graphic_set( INSTANCE * my, int64_t * params ) {
    GRAPH * graph;

    graph = bitmap_get( params[0], params[1] );
    if ( !graph ) return 0;

    switch ( params[2] ) {
        case G_CENTER_X:     /* g_center_x */
            bitmap_set_cpoint( graph, 0, params[3], ( graph->ncpoints ) ? ( graph->cpoints[0].y ) : ( graph->height / 2 ) );
            return 1;

        case G_CENTER_Y:     /* g_center_y */
            bitmap_set_cpoint( graph, 0, ( graph->ncpoints ) ? ( graph->cpoints[0].x ) : ( graph->width / 2 ), params[3] );
            return 1;
    }
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_graphic_info( INSTANCE * my, int64_t * params ) {
    GRAPH * graph;

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

            case G_DEPTH:           /* g_depth */
                return 32;

        }
        return 0;
    }

    graph = bitmap_get( params[0], params[1] );
    if ( !graph ) return 0;

    switch ( params[2] ) {
        case G_WIDTH:           /* g_wide */
            return graph->width;

        case G_HEIGHT:          /* g_height */
            return graph->height;

//        case G_PITCH:           /* g_pitch */
//            return graph->pitch;

        case G_DEPTH:           /* g_depth */
#ifdef USE_SDL2
            if ( !graph->surface ) return -1;
            return graph->surface->format->BitsPerPixel;
#endif
#ifdef USE_SDL2_GPU
            if ( !graph->tex ) return -1;
            return 32; // Only 32 bpp is supported
#endif
        case G_CENTER_X:        /* g_center_x */
            if ( graph->ncpoints > 0 && graph->cpoints[0].x != CPOINT_UNDEFINED ) return graph->cpoints[0].x;
            return graph->width / 2;

        case G_CENTER_Y:        /* g_center_y */
            if ( graph->ncpoints > 0 && graph->cpoints[0].y != CPOINT_UNDEFINED ) return graph->cpoints[0].y;
            return graph->height / 2;
    }

    return 1;
}

/* --------------------------------------------------------------------------- */
/* Control Points                                                              */
/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_set_point( INSTANCE * my, int64_t * params ) {
    GRAPH * graph = bitmap_get( params[0], params[1] );
    if ( !graph || params[2] < 0 || params[2] > 999 ) return -1;
    bitmap_set_cpoint( graph, ( uint32_t )params[2], params[3], params[4] );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_set_center( INSTANCE * my, int64_t * params ) {
    GRAPH * graph = bitmap_get( params[0], params[1] );
    if ( !graph ) return -1;
    bitmap_set_cpoint( graph, 0, params[2], params[3] );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_get_point( INSTANCE * my, int64_t * params ) {
    GRAPH * graph;

    graph = bitmap_get( params[0], params[1] );
    if ( !graph ) return 0;

    /* Use the center as control point if it is not there */
    if ( params[2] == 0 && ( graph->ncpoints == 0 || graph->cpoints[0].x == CPOINT_UNDEFINED ) ) {
        *( int64_t * )( intptr_t )params[3] = graph->width / 2;
        *( int64_t * )( intptr_t )params[4] = graph->height / 2;
        return 1;
    }

    if (( uint64_t )params[2] >= graph->ncpoints || params[2] < 0 ) return 0;

    if ( graph->cpoints[params[2]].x == CPOINT_UNDEFINED && graph->cpoints[params[2]].y == CPOINT_UNDEFINED ) return 0;

    *( int64_t * )( intptr_t )params[3] = graph->cpoints[params[2]].x;
    *( int64_t * )( intptr_t )params[4] = graph->cpoints[params[2]].y;
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_get_point_total( INSTANCE * my, int64_t * params ) {
    GRAPH * graph;

    graph = bitmap_get( params[0], params[1] );
    if ( !graph ) return 0;

    return graph->ncpoints;
}

/* --------------------------------------------------------------------------- */
/* Control Box                                                                 */
/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_set_box( INSTANCE * my, int64_t * params ) {
    GRAPH * graph;

    graph = bitmap_get( params[0], params[1] );
    if ( !graph ) return 0;

    bitmap_set_cbox( graph, params[2], params[3], params[4], params[5], params[6], params[7] );

    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_get_box( INSTANCE * my, int64_t * params ) {
    GRAPH * graph;

    graph = bitmap_get( params[0], params[1] );
    if ( !graph ) return 0;

    CBOX * cbox = bitmap_get_cbox( graph, params[2] );
    if ( !cbox ) return 0;

    if ( params[3] ) *( int64_t * )( intptr_t )params[3] = cbox->shape;
    if ( params[4] ) *( int64_t * )( intptr_t )params[4] = cbox->x;
    if ( params[5] ) *( int64_t * )( intptr_t )params[5] = cbox->y;
    if ( params[6] ) *( int64_t * )( intptr_t )params[6] = cbox->width;
    if ( params[7] ) *( int64_t * )( intptr_t )params[7] = cbox->height;

    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_get_box_by_pos( INSTANCE * my, int64_t * params ) {
    GRAPH * graph;

    graph = bitmap_get( params[0], params[1] );
    if ( !graph ) return 0;

    CBOX * cbox = bitmap_get_cbox_by_pos( graph, params[2] );
    if ( !cbox ) return 0;

    if ( params[3] ) *( int64_t * )( intptr_t )params[3] = cbox->code;
    if ( params[4] ) *( int64_t * )( intptr_t )params[4] = cbox->shape;
    if ( params[5] ) *( int64_t * )( intptr_t )params[5] = cbox->x;
    if ( params[6] ) *( int64_t * )( intptr_t )params[6] = cbox->y;
    if ( params[7] ) *( int64_t * )( intptr_t )params[7] = cbox->width;
    if ( params[8] ) *( int64_t * )( intptr_t )params[8] = cbox->height;

    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_set_box2( INSTANCE * my, int64_t * params ) {
    GRAPH * graph;

    graph = instance_graph( my );
    if ( !graph ) return 0;

    bitmap_set_cbox( graph, params[0], params[1], params[2], params[3], params[4], params[5] );

    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_get_box2( INSTANCE * my, int64_t * params ) {
    GRAPH * graph;

    graph = instance_graph( my );
    if ( !graph ) return 0;

    CBOX * cbox = bitmap_get_cbox( graph, params[0] );
    if ( !cbox ) return 0;

    if ( params[1] ) *( int64_t * )( intptr_t )params[1] = cbox->shape;
    if ( params[2] ) *( int64_t * )( intptr_t )params[2] = cbox->x;
    if ( params[3] ) *( int64_t * )( intptr_t )params[3] = cbox->y;
    if ( params[4] ) *( int64_t * )( intptr_t )params[4] = cbox->width;
    if ( params[5] ) *( int64_t * )( intptr_t )params[5] = cbox->height;

    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_get_box_by_pos2( INSTANCE * my, int64_t * params ) {
    GRAPH * graph;

    graph = instance_graph( my );
    if ( !graph ) return 0;

    CBOX * cbox = bitmap_get_cbox_by_pos( graph, params[0] );
    if ( !cbox ) return 0;

    if ( params[1] ) *( int64_t * )( intptr_t )params[1] = cbox->code;
    if ( params[2] ) *( int64_t * )( intptr_t )params[2] = cbox->shape;
    if ( params[3] ) *( int64_t * )( intptr_t )params[3] = cbox->x;
    if ( params[4] ) *( int64_t * )( intptr_t )params[4] = cbox->y;
    if ( params[5] ) *( int64_t * )( intptr_t )params[5] = cbox->width;
    if ( params[6] ) *( int64_t * )( intptr_t )params[6] = cbox->height;

    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_remove_box( INSTANCE * my, int64_t * params ) {
    GRAPH * graph;

    graph = bitmap_get( params[0], params[1] );
    if ( !graph ) return 0;

    bitmap_remove_cbox( graph, params[2] );

    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_remove_box2( INSTANCE * my, int64_t * params ) {
    GRAPH * graph;

    graph = instance_graph( my );
    if ( !graph ) return 0;

    bitmap_remove_cbox( graph, params[1] );

    return 1;
}

/* --------------------------------------------------------------------------- */
/* Map                                                                         */
/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_map_clear( INSTANCE * my, int64_t * params ) {
    GRAPH *graph = bitmap_get( params[0], params[1] );
    if ( graph ) gr_clear( graph );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_map_clear2( INSTANCE * my, int64_t * params ) {
    GRAPH *graph = bitmap_get( params[0], params[1] );
    if ( graph ) gr_clear_as( graph, params[2] );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_map_clear3( INSTANCE * my, int64_t * params ) {
    GRAPH *graph = bitmap_get( params[0], params[1] );
    if ( graph ) {
        REGION region;
        region.x = MIN( params[2], params[4] );
        region.y = MIN( params[3], params[5] );
        region.x2 = MAX( params[2], params[4] );
        region.y2 = MAX( params[3], params[5] );
        gr_clear_region( graph, &region );
    }
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_map_clear4( INSTANCE * my, int64_t * params ) {
    GRAPH *graph = bitmap_get( params[0], params[1] );
    if ( graph ) {
        REGION region;
        region.x = MIN( params[2], params[4] );
        region.y = MIN( params[3], params[5] );
        region.x2 = MAX( params[2], params[4] );
        region.y2 = MAX( params[3], params[5] );
        gr_clear_region_as( graph, &region, params[6] );
    }
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_new_map( INSTANCE * my, int64_t * params ) {
     GRAPH * graph;
     graph = bitmap_new_syslib( params[0], params[1] );
     return graph ? graph->code : 0;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_new_map_extend( INSTANCE * my, int64_t * params ) {
    GRAPH * graph;
    graph = bitmap_new_syslib( params[0], params[1] );
    if ( graph && ( params[2] & B_CLEAR )) gr_clear( graph );
    return graph ? graph->code : 0;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_map_clone( INSTANCE * my, int64_t * params ) {
    GRAPH * origin, * graph = NULL;
    origin = bitmap_get( params[0], params[1] );
    if ( origin ) graph = bitmap_clone( origin );
    if ( !graph ) return 0;
    graph->code = bitmap_next_code();
    grlib_add_map( 0, graph );
    return graph->code;
}

/* --------------------------------------------------------------------------- */
/** MAP_PUT(FILE, GRAPH_DEST, FILE_SRC, GRAPH_SRC, X, Y)
 *  Draws a graph into another one
 */

int64_t libmod_gfx_map_put( INSTANCE * my, int64_t * params ) {
    GRAPH * dest = bitmap_get( params[0], params[1] );
    GRAPH * orig = bitmap_get( params[2], params[3] );

    if ( !dest || !orig ) return 0;

    shader_deactivate();

    gr_blit(    dest,
                NULL,
                params[4],
                params[5],
                0,
                0,
                100.0,
                100.0,
                POINT_UNDEFINED,
                POINT_UNDEFINED,
                orig,
                NULL,
                255,
                255,
                255,
                255,
                BLEND_DISABLED,
                NULL
            );
    return 1;
}

/* --------------------------------------------------------------------------- */
/** MAP_PUT(FILE, GRAPH_DEST, FILE_SRC, GRAPH_SRC, X, Y, ANGLE, SCALEX, SCALEY, FLAGS, R, G, B, ALPHA)
 *  Draws a graph into another one, with most blitter options including flags and alpha
 */

int64_t libmod_gfx_map_put2( INSTANCE * my, int64_t * params ) {
    GRAPH * dest = bitmap_get( params[0], params[1] );
    GRAPH * orig = bitmap_get( params[2], params[3] );
    if ( !dest || !orig ) return 0;

    shader_deactivate();

    gr_blit( dest,       // dest
             NULL,       // clip
             params[4],  // x
             params[5],  // y
             params[9],  // flags
             params[6],  // angle
             params[7],  // scalex
             params[8],  // scaley
             POINT_UNDEFINED, // centerx
             POINT_UNDEFINED, // centery
             orig,       // orig
             NULL,       // orig_clip
             params[13], // alpha
             params[10], // r
             params[11], // g
             params[12], // b
             BLEND_DISABLED,
             NULL
         );

    return 1;
}

/* --------------------------------------------------------------------------- */
/** MAP_PUT(FILE, GRAPH_DEST, FILE_SRC, GRAPH_SRC, X, Y, ANGLE, SCALEX, SCALEY, FLAGS, R, G, B, ALPHA, BLEND_MODE)
 *  Draws a graph into another one, with most blitter options including flags and alpha
 */

int64_t libmod_gfx_map_put3( INSTANCE * my, int64_t * params ) {
    GRAPH * dest = bitmap_get( params[0], params[1] );
    GRAPH * orig = bitmap_get( params[2], params[3] );
    if ( !dest || !orig ) return 0;

    shader_deactivate();

    gr_blit( dest,       // dest
             NULL,       // clip
             params[4],  // x
             params[5],  // y
             params[9],  // flags
             params[6],  // angle
             params[7],  // scalex
             params[8],  // scaley
             POINT_UNDEFINED, // centerx
             POINT_UNDEFINED, // centery
             orig,       // orig
             NULL,       // orig_clip
             params[13], // alpha
             params[10], // r
             params[11], // g
             params[12], // b
             params[14], // blend_mode
             NULL
         );

    return 1;
}

/* --------------------------------------------------------------------------- */
/** MAP_PUT(FILE, GRAPH_DEST, FILE_SRC, GRAPH_SRC, X, Y, ANGLE, SCALEX, SCALEY, FLAGS, R, G, B, ALPHA, BLEND_MODE, SRC_RGB, DST_RGB, SRC_ALPHA, DST_ALPHA, EQUATION_RGB, EQUATION_ALPHA)
 *  Draws a graph into another one, with most blitter options including flags and alpha
 */

int64_t libmod_gfx_map_put4( INSTANCE * my, int64_t * params ) {
    GRAPH * dest = bitmap_get( params[0], params[1] );
    GRAPH * orig = bitmap_get( params[2], params[3] );
    if ( !dest || !orig ) return 0;

    CUSTOM_BLENDMODE custom_blendmode;

    custom_blendmode.src_rgb    = params[15];
    custom_blendmode.dst_rgb    = params[16];
    custom_blendmode.src_alpha  = params[17];
    custom_blendmode.dst_alpha  = params[18];
    custom_blendmode.eq_rgb     = params[19];
    custom_blendmode.eq_alpha   = params[20];

    shader_deactivate();

    gr_blit( dest,       // dest
             NULL,       // clip
             params[4],  // x
             params[5],  // y
             params[9],  // flags
             params[6],  // angle
             params[7],  // scalex
             params[8],  // scaley
             POINT_UNDEFINED, // centerx
             POINT_UNDEFINED, // centery
             orig,       // orig
             NULL,       // orig_clip
             params[13], // alpha
             params[10], // r
             params[11], // g
             params[12], // b
             params[14], // blend_mode
             &custom_blendmode
         );

    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_map_name( INSTANCE * my, int64_t * params ) {
    GRAPH * graph = bitmap_get( params[0], params[1] );
    int64_t result;

    if ( !graph ) return 0;
    result = string_new( graph->name );
    string_use( result );
    return result;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_map_set_name( INSTANCE * my, int64_t * params ) {
    GRAPH * graph = bitmap_get( params[0], params[1] );
    const char * ptr = string_get( params[2] );
    if ( graph ) {
        strncpy( graph->name, ptr, sizeof( graph->name ) );
        graph->name[sizeof( graph->name )-1] = 0;
    }
    string_discard( params[2] );
    return 0;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_map_exists( INSTANCE * my, int64_t * params ) {
    GRAPH * graph = bitmap_get( params[0], params[1] );
    return graph == NULL ? 0 : 1;
}

/* --------------------------------------------------------------------------- */

static int64_t __libmod_gfx_map_block_copy(
    int64_t dest_file,
    int64_t dest_graph,
    int64_t dest_x,
    int64_t dest_y,
    int64_t file,
    int64_t graph,
    int64_t x,
    int64_t y,
    int64_t w,
    int64_t h,
    int64_t flags,
    uint8_t r,
    uint8_t g,
    uint8_t b,
    uint8_t alpha,
    int64_t blend_mode,
    CUSTOM_BLENDMODE * custom_blendmode
 ) {
    GRAPH * dest, * orig;
    REGION clip;
    int64_t centerx, centery;

    if ( !( dest = bitmap_get( dest_file, dest_graph ) ) ) return 0;
    if ( !( orig = bitmap_get( file, graph ) ) ) return 0;

    if ( orig->ncpoints > 0 && orig->cpoints[0].x != CPOINT_UNDEFINED ) {
        centerx = orig->cpoints[0].x;
        centery = orig->cpoints[0].y;
    } else {
        centery = orig->height / 2;
        centerx = orig->width / 2;
    }

    if ( flags & B_HMIRROR ) centerx = orig->width - 1 - centerx;
    if ( flags & B_VMIRROR ) centery = orig->height - 1 - centery;

    if ( x      < 0 ) { dest_x += x; w += x;      x = 0; }
    if ( y      < 0 ) { dest_y += y; h += y;      y = 0; }
    if ( dest_x < 0 ) { x += dest_x; w += dest_x; dest_x = 0; }
    if ( dest_y < 0 ) { y += dest_y; h += dest_y; dest_y = 0; }

    if ( x + w  > orig->width  ) w = orig->width  - x;
    if ( y + h  > orig->height ) h = orig->height - y;
    if ( dest_x + w > dest->width  ) w = dest->width  - dest_x;
    if ( dest_y + h > dest->height ) h = dest->height - dest_y;

    if ( w <= 0 || h <= 0 ) return 0;

    clip.x  = dest_x;
    clip.y  = dest_y;
    clip.x2 = dest_x + w - 1;
    clip.y2 = dest_y + h - 1;

    shader_deactivate();

    gr_blit(    dest,
                &clip,
                dest_x - x + centerx,
                dest_y - y + centery,
                flags,
                0,
                100.0,
                100.0,
                POINT_UNDEFINED,
                POINT_UNDEFINED,
                orig,
                NULL,
                alpha,
                r,
                g,
                b,
                blend_mode,
                custom_blendmode
            );
    return 1;
}

/* --------------------------------------------------------------------------- */
/** MAP_BLOCK_COPY(FILE_DST, GRAPH_DST, X_DST, Y_DST, FILE_SRC, GRAPH_SRC, X_SRC, Y_SRC, W, H, FLAGS, R, G, B, ALPHA, BLEND_MODE, SRC_RGB, DST_RGB, SRC_ALPHA, DST_ALPHA, EQUATION_RGB, EQUATION_ALPHA)
*/

int64_t libmod_gfx_map_block_copy( INSTANCE * my, int64_t * params ) {
    return __libmod_gfx_map_block_copy(
        params[0],
        params[1],
        params[2],
        params[3],
        params[4],
        params[5],
        params[6],
        params[7],
        params[8],
        params[9],
        params[10],
        255,
        255,
        255,
        255,
        BLEND_DISABLED,
        NULL
    );
}

/* --------------------------------------------------------------------------- */
/** MAP_BLOCK_COPY(FILE_DST, GRAPH_DST, X_DST, Y_DST, FILE_SRC, GRAPH_SRC, X_SRC, Y_SRC, W, H, FLAGS, R, G, B, ALPHA )
*/

int64_t libmod_gfx_map_block_copy2( INSTANCE * my, int64_t * params ) {
    return __libmod_gfx_map_block_copy(
        params[0],
        params[1],
        params[2],
        params[3],
        params[4],
        params[5],
        params[6],
        params[7],
        params[8],
        params[9],
        params[10],
        params[11],
        params[12],
        params[13],
        params[14],
        BLEND_DISABLED,
        NULL
    );
}

/* --------------------------------------------------------------------------- */
/** MAP_BLOCK_COPY(FILE_DST, GRAPH_DST, X_DST, Y_DST, FILE_SRC, GRAPH_SRC, X_SRC, Y_SRC, W, H, FLAGS, R, G, B, ALPHA, BLEND_MODE)
*/

int64_t libmod_gfx_map_block_copy3( INSTANCE * my, int64_t * params ) {
    return __libmod_gfx_map_block_copy(
        params[0],
        params[1],
        params[2],
        params[3],
        params[4],
        params[5],
        params[6],
        params[7],
        params[8],
        params[9],
        params[10],
        params[11],
        params[12],
        params[13],
        params[14],
        params[15],
        NULL
    );
}

/* --------------------------------------------------------------------------- */
/** MAP_BLOCK_COPY(FILE_DST, GRAPH_DST, X_DST, Y_DST, FILE_SRC, GRAPH_SRC, X_SRC, Y_SRC, W, H, FLAGS, R, G, B, ALPHA, BLEND_MODE, SRC_RGB, DST_RGB, SRC_ALPHA, DST_ALPHA, EQUATION_RGB, EQUATION_ALPHA)
*/

int64_t libmod_gfx_map_block_copy4( INSTANCE * my, int64_t * params ) {
    CUSTOM_BLENDMODE custom_blendmode;

    custom_blendmode.src_rgb    = params[16];
    custom_blendmode.dst_rgb    = params[17];
    custom_blendmode.src_alpha  = params[18];
    custom_blendmode.dst_alpha  = params[19];
    custom_blendmode.eq_rgb     = params[20];
    custom_blendmode.eq_alpha   = params[21];

    return __libmod_gfx_map_block_copy(
        params[0],
        params[1],
        params[2],
        params[3],
        params[4],
        params[5],
        params[6],
        params[7],
        params[8],
        params[9],
        params[10],
        params[11],
        params[12],
        params[13],
        params[14],
        params[15],
        &custom_blendmode
    );
}

/* --------------------------------------------------------------------------- */
/** MAP_BLOCK_COPY(FILE_DST, GRAPH_DST, X_DST, Y_DST, GRAPH_SRC, X_SRC, Y_SRC, W, H, FLAGS)
*/

int64_t libmod_gfx_map_block_copy5( INSTANCE * my, int64_t * params ) {
    return __libmod_gfx_map_block_copy(
        params[0],
        params[1],
        params[2],
        params[3],
        params[0],
        params[4],
        params[5],
        params[6],
        params[7],
        params[8],
        params[9],
        255,
        255,
        255,
        255,
        BLEND_DISABLED,
        NULL
    );
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
    int64_t graph = gr_load_map( mapname );
    if ( !graph ) {
        file *fp;

        if ( !( fp = file_open( mapname, "rb0" ) ) ) return ( 0 );

        SDL_Surface* surface = IMG_Load_RW( SDL_RWFromBGDFP( fp ), 1 );
        if ( !surface ) {
            printf( "Unable to load image %s! SDL_image Error: %s\n", mapname, IMG_GetError() );
            return 0;
        }

        // Set transparent color
//        SDL_SetColorKey( surface, SDL_TRUE, 0 );

        GRAPH *gr = bitmap_new( 0, 0, 0, surface );
        SDL_FreeSurface( surface );
        if ( !gr ) return 0;

        // Don't matter the file code, we must force a new code...
        gr->code = bitmap_next_code();

        grlib_add_map( 0, gr );
        return gr->code;
    }

    return graph;
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
/*
extern DECLSPEC int SDLCALL IMG_SavePNG(SDL_Surface *surface, const char *file);
extern DECLSPEC int SDLCALL IMG_SavePNG_RW(SDL_Surface *surface, SDL_RWops *dst, int freedst);
*/
int64_t libmod_gfx_map_save( INSTANCE * my, int64_t * params )
{
    GRAPH * gr = bitmap_get( params[0], params[1] );
    int64_t r;

    if ( !gr ) {
        string_discard( params[2] );
        return -1;
    }
#ifdef USE_SDL2
    if ( !gr->surface )
#endif
#ifdef USE_SDL2_GPU
    if ( !gr->tex )
#endif
    {
        string_discard( params[2] );
        return -1;
    }
#ifdef USE_SDL2
    r = ( int64_t ) IMG_SavePNG( gr->surface, ( char * )string_get( params[2] ) );
#endif
#ifdef USE_SDL2_GPU
    r = ( int64_t ) GPU_SaveImage( gr->tex, ( char * )string_get( params[2] ), GPU_FILE_AUTO );
#endif
    string_discard( params[2] );
    return r;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_set_texture_quality( INSTANCE * my, int64_t * params ) {
    switch( params[0] ) {
        case Q_NEAREST:
        default:
#ifdef USE_SDL2
            return SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "nearest" );
#endif
#ifdef USE_SDL2_GPU
            gr_filter_mode  = GPU_FILTER_NEAREST;
            break;
#endif

        case Q_LINEAR:
#ifdef USE_SDL2
            return SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "linear" );
#endif

        case Q_BEST:
#ifdef USE_SDL2
            return SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "best" );
#endif
#ifdef USE_SDL2_GPU
            gr_filter_mode  = GPU_FILTER_LINEAR;
            break;
#endif

#ifdef USE_SDL2_GPU
        case Q_MIPMAP:
            gr_filter_mode  = GPU_FILTER_LINEAR_MIPMAP;
            break;
#endif

    }
    return -1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_set_palette( INSTANCE * my, int64_t * params ) {
#ifdef USE_SDL2
    GRAPH * graph = bitmap_get( params[0], params[1] );
    int firstcolor = params[2], ncolors = params[3];
    SDL_Color * colors = ( SDL_Color * ) ( intptr_t ) params[4];

    if ( !graph->surface ) return -1;

    if ( graph->surface->format->BitsPerPixel != 8 ) return -2; // Not an 8-bit surface

    SDL_Palette * palette = SDL_AllocPalette( 256 );
    if ( !palette ) return -1;

    if ( SDL_MUSTLOCK( graph->surface ) ) {
        SDL_LockSurface( graph->surface );
        SDL_SetPaletteColors( palette, graph->surface->format->palette->colors, 0, 255 );
        SDL_UnlockSurface( graph->surface );
    } else {
        SDL_SetPaletteColors( palette, graph->surface->format->palette->colors, 0, 255 );
    }

    SDL_SetPaletteColors( palette, colors, firstcolor, ncolors );
    SDL_SetSurfacePalette( graph->surface, palette );
    SDL_FreePalette( palette );

    graph->texture_must_update = 1;
#endif
    return 0;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_get_palette( INSTANCE * my, int64_t * params ) {
#ifdef USE_SDL2
    GRAPH * graph = bitmap_get( params[0], params[1] );
    int firstcolor = params[2], ncolors = params[3], i;
    SDL_Color * colors = ( SDL_Color * ) ( intptr_t ) params[4];

    if ( !graph->surface ) return -1;

    if ( graph->surface->format->BitsPerPixel != 8 ) return -2; // Not an 8-bit surface

    if ( SDL_MUSTLOCK( graph->surface ) ) {
        SDL_LockSurface( graph->surface );
        for ( i = 0; ncolors--; i++ ) colors[ i ] = graph->surface->format->palette->colors[ firstcolor + i ];
        SDL_UnlockSurface( graph->surface );
    } else {
        for ( i = 0; ncolors--; i++ ) colors[ i ] = graph->surface->format->palette->colors[ firstcolor + i ];
    }
#endif
    return 0;
}

/* --------------------------------------------------------------------------- */
