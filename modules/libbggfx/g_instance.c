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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fmath.h>

/* --------------------------------------------------------------------------- */

#include <SDL.h>

#include "bgddl.h"

#include "libbggfx.h"
#include "resolution.h"

#include "sysprocs_st.h"

#include "dlvaracc.h"

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : instance_graph
 *
 *  Returns the instance graphic or NULL if there is none
 *
 *  PARAMS :
 *      i           Pointer to the instance
 *
 *  RETURN VALUE :
 *      Pointer to the graphic or NULL if none
 */

GRAPH * instance_graph( INSTANCE * i ) {
    int64_t * xgraph, c, a;

    if (( xgraph = ( int64_t * ) LOCQWORD( libbggfx, i, XGRAPH ) ) ) { // Get offset of XGRAPH table
        c = *xgraph++;  // Get number of graphs ids in XGRAPH table
        if ( c ) {
            // Normalize ANGLE
            a = LOCINT64( libbggfx, i, ANGLE ) % 360000;
            if ( a < 0 ) a += 360000;

            // Get graph id in XGRAPH table to draw
            c = xgraph[a * c / 360000];

            // If graph id value is negative, then graphic must be mirrored
            if ( c < 0 ) {
                c = -c;
                LOCQWORD( libbggfx, i, XGRAPH_FLAGS ) = B_HMIRROR;
            } else {
                LOCQWORD( libbggfx, i, XGRAPH_FLAGS ) = 0;
            }

            // Get GRAPH * to draw
            return bitmap_get( LOCQWORD( libbggfx, i, FILEID ), c );
        }
    }

    // Get GRAPH * to draw
    if (( c = LOCQWORD( libbggfx, i, GRAPHID ) ) ) {
        return bitmap_get( LOCQWORD( libbggfx, i, FILEID ), c );
    }

    return NULL; // No graph to draw
}

/* ---------------------------------------------------------------------- */

void instance_get_bbox( INSTANCE * i, GRAPH * gr, REGION * dest ) {
    REGION *region;
    int64_t x, y, r, scalex, scaley;
    SDL_Rect *map_clip = NULL, _map_clip;

    r = LOCINT64( libbggfx, i, REGIONID );
    if ( r > 0 && r < 32 ) region = &regions[ r ];
    else                   region = &regions[ 0 ];

    x = LOCINT64( libbggfx, i, COORDX );
    y = LOCINT64( libbggfx, i, COORDY );

    RESOLXY( libbggfx, i, x, y );

    scalex = LOCINT64( libbggfx, i, GRAPHSIZEX );
    scaley = LOCINT64( libbggfx, i, GRAPHSIZEY );

    if ( scalex == 100 && scaley == 100 ) scalex = scaley = LOCINT64( libbggfx, i, GRAPHSIZE );

    _map_clip.w = LOCINT64( libbggfx, i, CLIPW );
    _map_clip.h = LOCINT64( libbggfx, i, CLIPH );

    if ( _map_clip.w && _map_clip.h ) {
        _map_clip.x = LOCINT64( libbggfx, i, CLIPX );
        _map_clip.y = LOCINT64( libbggfx, i, CLIPY );
        map_clip = &_map_clip;
    }

    gr_get_bbox( dest,
                 region,
                 x,
                 y,
                 LOCQWORD( libbggfx, i, FLAGS ) ^ LOCQWORD( libbggfx, i, XGRAPH_FLAGS ),
                 LOCQWORD( libbggfx, i, XGRAPH ) ? 0 : LOCINT64( libbggfx, i, ANGLE ),
                 scalex,
                 scaley,
                 gr,
                 map_clip
               );
}

/* ---------------------------------------------------------------------- */

void draw_instance_at( INSTANCE * i, REGION * region, int64_t x, int64_t y, GRAPH * dest ) {
    GRAPH * map;
    int64_t flags, scalex, scaley;
    SDL_Rect *map_clip = NULL, _map_clip;
    uint8_t alpha, color_r, color_g, color_b;

    if ( !( alpha = LOCBYTE( libbggfx, i, ALPHA ) ) ) return;

    if ( !( map = instance_graph( i ) ) ) return;

    color_r = LOCBYTE( libbggfx, i, COLORR );
    color_g = LOCBYTE( libbggfx, i, COLORG );
    color_b = LOCBYTE( libbggfx, i, COLORB );

    flags = ( LOCQWORD( libbggfx, i, FLAGS ) ^ LOCQWORD( libbggfx, i, XGRAPH_FLAGS ) );

    scalex = LOCINT64( libbggfx, i, GRAPHSIZEX );
    scaley = LOCINT64( libbggfx, i, GRAPHSIZEY );
    if ( scalex == 100 && scaley == 100 ) scalex = scaley = LOCINT64( libbggfx, i, GRAPHSIZE );

    _map_clip.w = LOCINT64( libbggfx, i, CLIPW );
    _map_clip.h = LOCINT64( libbggfx, i, CLIPH );

    if ( _map_clip.w && _map_clip.h ) {
        _map_clip.x = LOCINT64( libbggfx, i, CLIPX );
        _map_clip.y = LOCINT64( libbggfx, i, CLIPY );
        map_clip = &_map_clip;
    }

    gr_blit( dest, region, x, y, flags, LOCQWORD( libbggfx, i, XGRAPH ) ? 0 : LOCINT64( libbggfx, i, ANGLE ), scalex, scaley, map, map_clip, alpha, color_r, color_g, color_b );
}

/* --------------------------------------------------------------------------- */
/* Rutinas gráficas de alto nivel */

void draw_instance( void * what, REGION * clip ) {
    INSTANCE * i = ( void * ) what;
    GRAPH * map, * map_dst = NULL;
    int64_t flags, scalex, scaley, x, y, r;
    REGION region;
    SDL_Rect *map_clip = NULL, _map_clip;
    uint8_t alpha, color_r, color_g, color_b, c;

    if ( !( alpha = LOCBYTE( libbggfx, i, ALPHA ) ) ) return;

    if ( !( map = ( GRAPH * ) LOCQWORD( libbggfx, i, GRAPHPTR ) ) ) return;

    // Get GRAPH * target, if exists
    if (( c = LOCQWORD( libbggfx, i, RENDER_GRAPHID ) ) ) {
        map_dst = bitmap_get( LOCQWORD( libbggfx, i, RENDER_FILEID ), c );
    }

    flags = ( LOCQWORD( libbggfx, i, FLAGS ) ^ LOCQWORD( libbggfx, i, XGRAPH_FLAGS ) );

    color_r = LOCBYTE( libbggfx, i, COLORR );
    color_g = LOCBYTE( libbggfx, i, COLORG );
    color_b = LOCBYTE( libbggfx, i, COLORB );

    scalex = LOCINT64( libbggfx, i, GRAPHSIZEX );
    scaley = LOCINT64( libbggfx, i, GRAPHSIZEY );
    if ( scalex == 100 && scaley == 100 ) scalex = scaley = LOCINT64( libbggfx, i, GRAPHSIZE );

    x = LOCINT64( libbggfx, i, COORDX );
    y = LOCINT64( libbggfx, i, COORDY );

    RESOLXY( libbggfx, i, x, y );

    r = LOCINT64( libbggfx, i, REGIONID );
    if ( r > 0 && r < 32 ) {
        region = regions[ r ];
    } else if ( map_dst ) {
        region.x = region.y = 0;
        region.x2 = map_dst->width;
        region.y2 = map_dst->height;
    } else {
        region = regions[ 0 ];
    }

    if ( clip ) region_union( &region, clip );

    _map_clip.w = LOCINT64( libbggfx, i, CLIPW );
    _map_clip.h = LOCINT64( libbggfx, i, CLIPH );

    if ( _map_clip.w && _map_clip.h ) {
        _map_clip.x = LOCINT64( libbggfx, i, CLIPX );
        _map_clip.y = LOCINT64( libbggfx, i, CLIPY );
        map_clip = &_map_clip;
    }

    gr_blit( map_dst, &region, x, y, flags, LOCQWORD( libbggfx, i, XGRAPH ) ? 0 : LOCINT64( libbggfx, i, ANGLE ), scalex, scaley, map, map_clip, alpha, color_r, color_g, color_b );

}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : draw_instance_info
 *
 *  Compares the internal position variables of the instance with its
 *  currents values, and returns 1 if there is any difference. Used
 *  to detect changes in a visible process's aspect or position.
 *
 *  PARAMS :
 *      i           Pointer to the instance
 *
 *  RETURN VALUE :
 *      1 if there is any change, 0 otherwise
 */

int draw_instance_info( void * what, REGION * region, int64_t * z, int64_t * drawme ) {
    INSTANCE * i = ( INSTANCE * ) what;
    GRAPH * graph;

    if ( drawme ) * drawme = 0;

    LOCQWORD( libbggfx, i, GRAPHPTR ) = ( int64_t )( graph = instance_graph( i ) );
    if ( !graph ) return 0;

    int status;

    /* Update key */
    * z = LOCINT64( libbggfx, i, COORDZ );

    /* Si tiene grafico o xgraph o (ctype == 0 y esta corriendo o congelado) */

    if ( LOCQWORD( libbggfx, i, CTYPE ) == C_SCREEN && ( ( status = LOCQWORD( libbggfx, i, STATUS ) & ~STATUS_WAITING_MASK ) == STATUS_RUNNING || status == STATUS_FROZEN ) ) * drawme = 1;

    return 1;
}
