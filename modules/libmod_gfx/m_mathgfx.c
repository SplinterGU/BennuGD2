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
    int64_t x, y, sx = 1, sy = -1, angle = 0, idx = params[0], size_x, size_y, flags;
    double centerx, centery, dx = 0, dy = 0, px, py;

    b = instance_graph( my ) ;
    if ( !b ) return 0 ;

    if ( !( idx >= 0 && idx < b->ncpoints ) ) return 0 ;

    x = LOCINT64( libmod_gfx, my, COORDX ) ;
    y = LOCINT64( libmod_gfx, my, COORDY ) ;

    if ( !idx ) {
        *( int64_t * )( intptr_t )params[1] = x ;
        *( int64_t * )( intptr_t )params[2] = y ;
        return 1;
    }

    if ( b->cpoints[idx].x == CPOINT_UNDEFINED || b->cpoints[idx].y == CPOINT_UNDEFINED ) return 0;

    RESOLXY( libmod_gfx, my, x, y );

    centerx = LOCINT64( libmod_gfx, my, GRAPHCENTERX ) ;
    centery = LOCINT64( libmod_gfx, my, GRAPHCENTERY ) ;
    if ( centerx == POINT_UNDEFINED || centery == POINT_UNDEFINED ) {
        if ( b->ncpoints && b->cpoints[0].x != CPOINT_UNDEFINED ) {
            centerx = b->cpoints[0].x;
            centery = b->cpoints[0].y;
        } else {
            centerx = b->width / 2.0;
            centery = b->height / 2.0;
        }
    }

    if ( !LOCQWORD( libmod_gfx, my, XGRAPH ) ) angle = LOCINT64( libmod_gfx, my, ANGLE );

    flags = LOCQWORD( libmod_gfx, my, FLAGS );

    if ( flags & B_HMIRROR ) sx = -1;
    if ( flags & B_VMIRROR ) sy = 1;

    size_x = LOCINT64( libmod_gfx, my, GRAPHSIZEX );
    size_y = LOCINT64( libmod_gfx, my, GRAPHSIZEY );
    if ( size_x == 100 && size_y == 100 ) size_x = size_y = LOCINT64( libmod_gfx, my, GRAPHSIZE );

    dx = ( b->cpoints[idx].x - centerx ) * size_x;
    dy = ( b->cpoints[idx].y - centery ) * size_y;

    double cos_angle = cos_deg( angle );
    double sin_angle = sin_deg( angle );

    px = x + ( dx * cos_angle + dy * sin_angle ) * sx / 100.0 ;
    py = y + ( dx * sin_angle - dy * cos_angle ) * sy / 100.0 ;

    int64_t resolution = LOCINT64( libmod_gfx, my, RESOLUTION );

    if ( resolution > 0 ) {
        px *= resolution;
        py *= resolution;
    } else if ( resolution < 0 ) {
        px /= -resolution;
        py /= -resolution;
    }

    *( int64_t * )( intptr_t )params[1] = px ;
    *( int64_t * )( intptr_t )params[2] = py ;

    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_get_real_point2( INSTANCE * my, int64_t * params ) {
    my = instance_get( params[0] );
    if ( !my ) return 0;
    return libmod_gfx_get_real_point( my, &params[1] );
}

/* --------------------------------------------------------------------------- */
