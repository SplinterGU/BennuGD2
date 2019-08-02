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

#include "bgdrtm.h"
#include "bgddl.h"

#include "resolution.h"

#include "m_resolxy.h"

#include "libbggfx.h"
#include "libmod_gfx.h"

#include "fmath.h"

#include "dlvaracc.h"

/* --------------------------------------------------------------------------- */
/* Statics                                                                     */
/* --------------------------------------------------------------------------- */

static inline int64_t __get_angle( INSTANCE * a, INSTANCE * b ) {
    if ( !a || !b ) return -1;

    double dx = ( double ) ( LOCINT64( libmod_gfx, b, COORDX ) - LOCINT64( libmod_gfx, a, COORDX ) );
    double dy = ( double ) ( LOCINT64( libmod_gfx, b, COORDY ) - LOCINT64( libmod_gfx, a, COORDY ) );

    int64_t angle ;

    if ( dx == 0 ) return ( dy > 0 ) ? 270000L : 90000L ;

    angle = ( int64_t )( atan( dy / dx ) * 180000.0 / M_PI ) ;

    return ( dx > 0 ) ? -angle : -angle + 180000L ;
}

/* --------------------------------------------------------------------------- */

static inline int64_t __get_distance( INSTANCE * a, INSTANCE * b ) {
    if ( !a || !b ) return -1;

    int64_t x1 = LOCINT64( libmod_gfx, a, COORDX ), y1 = LOCINT64( libmod_gfx, a, COORDY ) ;
    int64_t x2 = LOCINT64( libmod_gfx, b, COORDX ), y2 = LOCINT64( libmod_gfx, b, COORDY ) ;

    int64_t res = LOCINT64( libmod_gfx, a, RESOLUTION ) ;

    RESOLXY( libmod_gfx, a, x1, y1 );
    RESOLXY( libmod_gfx, b, x2, y2 );

    double dx = ( x2 - x1 ) * ( x2 - x1 ) ;
    double dy = ( y2 - y1 ) * ( y2 - y1 ) ;

         if ( res > 0 ) return ( int64_t ) sqrt( dx + dy ) * res ;
    else if ( res < 0 ) return ( int64_t ) sqrt( dx + dy ) / -res ;

    return ( int64_t ) sqrt( dx + dy ) ;
}

/* --------------------------------------------------------------------------- */
/* Exportables                                                                 */
/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_advance( INSTANCE * my, int64_t * params ) {
    int64_t angle = LOCINT64( libmod_gfx, my, ANGLE ) ;
    LOCINT64( libmod_gfx, my, COORDX ) += ( int64_t ) ( cos_deg( angle ) * ( double ) params[0] );
    LOCINT64( libmod_gfx, my, COORDY ) -= ( int64_t ) ( sin_deg( angle ) * ( double ) params[0] );
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_xadvance( INSTANCE * my, int64_t * params ) {
    int64_t angle = params[0] ;
    LOCINT64( libmod_gfx, my, COORDX ) += ( int64_t ) ( cos_deg( angle ) * ( double ) params[1] );
    LOCINT64( libmod_gfx, my, COORDY ) -= ( int64_t ) ( sin_deg( angle ) * ( double ) params[1] );
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_get_angle( INSTANCE * my, int64_t * params ) {
    INSTANCE * b = instance_get( params[0] ) ;
    return __get_angle( my, b );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_get_angle2( INSTANCE * my, int64_t * params ) {
    INSTANCE * a = instance_get( params[0] );
    INSTANCE * b = instance_get( params[1] );
    return __get_angle( a, b );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_get_dist( INSTANCE * my, int64_t * params ) {
    INSTANCE * b = instance_get( params[0] );
    return __get_distance( my, b );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_get_dist2( INSTANCE * my, int64_t * params ) {
    INSTANCE * a = instance_get( params[0] );
    INSTANCE * b = instance_get( params[1] );
    return __get_distance( a, b );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_get_real_point( INSTANCE * my, int64_t * params ) {
    GRAPH * b ;
    int64_t x, y, r, centerx, centery, px = 0, py = 0, rx = 0, ry = 0 ;
    int64_t _angle = 0, angle = 0;

    b = instance_graph( my ) ;
    if ( !b )  return 0 ;

    /* Point 0 is the graphic center, but it may be not defined */
    if ( params[0] == 0 && ( b->ncpoints == 0 || b->cpoints[0].x == CPOINT_UNDEFINED ) ) {
        if ( b->ncpoints == 0 ) {
            bitmap_add_cpoint( b, b->width / 2, b->height / 2 );
        } else {
            b->cpoints[0].x = b->width / 2;
            b->cpoints[0].y = b->height / 2;
        }
    } else
        if (( uint64_t )params[0] >= b->ncpoints || params[0] < 0)
            return 0 ;

    if ( b->cpoints[params[0]].x == CPOINT_UNDEFINED && b->cpoints[params[0]].y == CPOINT_UNDEFINED )
        return 0;

    r = LOCINT64( libmod_gfx, my, REGIONID ) ;
    if ( r < 0 || r >= MAX_REGIONS ) r = 0 ;

    if ( b->cpoints[0].x != CPOINT_UNDEFINED && b->cpoints[0].y != CPOINT_UNDEFINED ) {
        centerx = b->cpoints[0].x ;
        centery = b->cpoints[0].y ;
    } else {
        centerx = b->width / 2 ;
        centery = b->height / 2 ;
    }

    if ( LOCINT64( libmod_gfx, my, ANGLE ) != 0 && !LOCQWORD( libmod_gfx, my, XGRAPH ) ) {
        _angle = angle = LOCINT64( libmod_gfx, my, ANGLE ) ;
    }

    if ( params[0] > 0 ) {
        px = b->cpoints[params[0]].x - centerx ;
        py = b->cpoints[params[0]].y - centery ;

        if ( LOCQWORD( libmod_gfx, my, FLAGS ) & B_HMIRROR ) {
            if (_angle) {
                py = (centery - b->cpoints[params[0]].y) ;
                angle = 90000 + ( 90000 - angle );
            } else
                px = (centerx - b->cpoints[params[0]].x) ;
        }

        if ( LOCQWORD( libmod_gfx, my, FLAGS ) & B_VMIRROR ) {
            if (_angle) {
                px = (centerx - b->cpoints[params[0]].x) ;
                angle = 360000 + ( 180000 - angle );
            } else
                py = (centery - b->cpoints[params[0]].y) ;
        }
    }

    if ( LOCINT64( libmod_gfx, my, GRAPHSIZEX ) == 100 && LOCINT64( libmod_gfx, my, GRAPHSIZEY ) == 100 ) {
        if ( LOCINT64( libmod_gfx, my, GRAPHSIZE ) > 0 ) {
            // Corrected a bug from the casting that rounded to 0
            px = ( int64_t ) ( px * ( LOCINT64( libmod_gfx, my, GRAPHSIZE ) / 100.0 ) ) ;
            py = ( int64_t ) ( py * ( LOCINT64( libmod_gfx, my, GRAPHSIZE ) / 100.0 ) ) ;
        }
    } else {
        // Adding size_x/size_y control
        if ( LOCINT64( libmod_gfx, my, GRAPHSIZEX ) > 0 )
            px = ( int64_t ) ( px * ( LOCINT64( libmod_gfx, my, GRAPHSIZEX ) / 100.0 ) ) ;

        if ( LOCINT64( libmod_gfx, my, GRAPHSIZEY ) > 0 )
            py = ( int64_t ) ( py * ( LOCINT64( libmod_gfx, my, GRAPHSIZEY ) / 100.0 ) ) ;
    }

    if ( angle ) {
        double cos_angle = cos_deg( -angle );
        double sin_angle = sin_deg( -angle );

        rx = ( int64_t ) ( ( double )px * cos_angle - ( double )py * sin_angle ) ;
        ry = ( int64_t ) ( ( double )px * sin_angle + ( double )py * cos_angle ) ;

        px = rx ;
        py = ry ;
    }

    x = LOCINT64( libmod_gfx, my, COORDX ) ;
    y = LOCINT64( libmod_gfx, my, COORDY ) ;

    RESOLXY( libmod_gfx, my, x, y );

    rx = x + px ;
    ry = y + py ;

    if ( LOCINT64( libmod_gfx, my, RESOLUTION ) > 0 ) {
        rx *= LOCINT64( libmod_gfx, my, RESOLUTION );
        ry *= LOCINT64( libmod_gfx, my, RESOLUTION );
    } else if ( LOCINT64( libmod_gfx, my, RESOLUTION ) < 0 ) {
        rx /= -LOCINT64( libmod_gfx, my, RESOLUTION );
        ry /= -LOCINT64( libmod_gfx, my, RESOLUTION );
    }

    *( int64_t * )( intptr_t )params[1] = rx ;
    *( int64_t * )( intptr_t )params[2] = ry ;

    return 1 ;
}

/* --------------------------------------------------------------------------- */
