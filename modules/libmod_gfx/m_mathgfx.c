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

    double dx = LOCDOUBLE( libmod_gfx, b, COORDX ) - LOCDOUBLE( libmod_gfx, a, COORDX );
    double dy = LOCDOUBLE( libmod_gfx, b, COORDY ) - LOCDOUBLE( libmod_gfx, a, COORDY );

    int64_t angle ;

    if ( dx == 0 ) return ( dy > 0 ) ? 270000L : 90000L ;

    angle = ( int64_t )( atan( dy / dx ) * 180000.0 / M_PI ) ;

    return ( dx > 0 ) ? -angle : -angle + 180000L ;
}

/* --------------------------------------------------------------------------- */

static inline int64_t __get_distance( INSTANCE * a, INSTANCE * b ) {
    if ( !a || !b ) return -1;

    int64_t x1 = LOCDOUBLE( libmod_gfx, a, COORDX ), y1 = LOCDOUBLE( libmod_gfx, a, COORDY ) ;
    int64_t x2 = LOCDOUBLE( libmod_gfx, b, COORDX ), y2 = LOCDOUBLE( libmod_gfx, b, COORDY ) ;

    int64_t res = LOCINT64( libmod_gfx, a, RESOLUTION ) ;

    RESOLXY( libmod_gfx, a, x1, y1 );
    RESOLXY( libmod_gfx, b, x2, y2 );

    double dx = ( x2 - x1 ) * ( x2 - x1 ) ;
    double dy = ( y2 - y1 ) * ( y2 - y1 ) ;

    double ret = sqrt( dx + dy ) ;

         if ( res > 0 ) ret *= res ;
    else if ( res < 0 ) ret /= -res ;

    return *( int64_t * ) &ret;
}

/* --------------------------------------------------------------------------- */
/* Exportables                                                                 */
/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_advance( INSTANCE * my, int64_t * params ) {
    int64_t angle = LOCINT64( libmod_gfx, my, ANGLE ) ;
    LOCDOUBLE( libmod_gfx, my, COORDX ) += cos_deg( angle ) * ( *( double * ) &params[0] );
    LOCDOUBLE( libmod_gfx, my, COORDY ) -= sin_deg( angle ) * ( *( double * ) &params[0] );
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_xadvance( INSTANCE * my, int64_t * params ) {
    int64_t angle = params[0] ;
    LOCDOUBLE( libmod_gfx, my, COORDX ) += cos_deg( angle ) * ( *( double * ) &params[1] );
    LOCDOUBLE( libmod_gfx, my, COORDY ) -= sin_deg( angle ) * ( *( double * ) &params[1] );
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

static inline int64_t __get_real_point( INSTANCE * my, int64_t * params, GRAPH * b, int64_t point_x, int64_t point_y ) {
    int64_t sx = 1, sy = -1, angle = 0, flags;
    double x, y, centerx, centery, dx = 0, dy = 0, px, py, size_x, size_y;

    x = LOCDOUBLE( libmod_gfx, my, COORDX ) ;
    y = LOCDOUBLE( libmod_gfx, my, COORDY ) ;

    RESOLXY( libmod_gfx, my, x, y );

    centerx = LOCDOUBLE( libmod_gfx, my, GRAPHCENTERX ) ;
    centery = LOCDOUBLE( libmod_gfx, my, GRAPHCENTERY ) ;
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

    size_x = LOCDOUBLE( libmod_gfx, my, GRAPHSIZEX );
    size_y = LOCDOUBLE( libmod_gfx, my, GRAPHSIZEY );
    if ( size_x == 100.0 && size_y == 100.0 ) size_x = size_y = LOCDOUBLE( libmod_gfx, my, GRAPHSIZE );

    dx = ( point_x - centerx ) * size_x;
    dy = ( point_y - centery ) * size_y;

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

    *( double * )( intptr_t )params[0] = px ;
    *( double * )( intptr_t )params[1] = py ;

    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_get_real_point( INSTANCE * my, int64_t * params ) {
    GRAPH * b ;
    int64_t idx = params[0];

    b = instance_graph( my ) ;
    if ( !b ) return 0 ;

    if ( !( idx >= 0 && idx < b->ncpoints ) ) return 0 ;

    if ( !idx ) {
        *( double * )( intptr_t )params[1] = LOCDOUBLE( libmod_gfx, my, COORDX ) ;
        *( double * )( intptr_t )params[2] = LOCDOUBLE( libmod_gfx, my, COORDY ) ;
        return 1;
    }

    if ( b->cpoints[idx].x == CPOINT_UNDEFINED || b->cpoints[idx].y == CPOINT_UNDEFINED ) return 0;

    return __get_real_point( my, &params[1], b, b->cpoints[idx].x, b->cpoints[idx].y );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_get_real_point2( INSTANCE * my, int64_t * params ) {
    my = instance_get( params[0] );
    if ( !my ) return 0;
    return libmod_gfx_get_real_point( my, &params[1] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_get_real_point3( INSTANCE * my, int64_t * params ) {
    GRAPH * b;

    my = instance_get( params[0] );
    if ( !my ) return 0;

    b = instance_graph( my ) ;
    if ( !b ) return 0 ;

    return __get_real_point( my, &params[3], b, params[1], params[1] );
}

/* --------------------------------------------------------------------------- */

static inline int64_t __get_real_box_vertex( GRAPH * graph, INSTANCE * my, int64_t * params ) {
    int64_t width = 0, height = 0;
    double x = 0.0, y = 0.0, center_x = 0.0, center_y = 0.0;
    CBOX vcbox = {0};

    CBOX * cbox = &vcbox;
    if ( params[0] == -1 ) {
        cbox->code   = -1;
        cbox->shape  = LOCINT64( libmod_gfx, my, CSHAPE );
        cbox->x      = LOCINT64( libmod_gfx, my, CBOX_X );
        cbox->y      = LOCINT64( libmod_gfx, my, CBOX_Y );
        cbox->width  = LOCINT64( libmod_gfx, my, CBOX_WIDTH );
        cbox->height = LOCINT64( libmod_gfx, my, CBOX_HEIGHT );
    }
    else
        if ( !( cbox = bitmap_get_cbox( graph, params[0] ) ) ) return 0;


    /* scale */

    double scale_x = LOCDOUBLE( libmod_gfx, my, GRAPHSIZEX ) / 100.0;
    double scale_y = LOCDOUBLE( libmod_gfx, my, GRAPHSIZEY ) / 100.0;
    if ( scale_x == 1.0 && scale_y == 1.0 ) scale_x = scale_y = LOCDOUBLE( libmod_gfx, my, GRAPHSIZE ) / 100.0;

    if ( scale_x < 0.0 ) scale_x = 0.0;
    if ( scale_y < 0.0 ) scale_y = 0.0;

    /* Calculate center */

    int64_t * radius = ( int64_t * ) ( intptr_t ) params[2];

    center_x = LOCDOUBLE( libmod_gfx, my, GRAPHCENTERX ) ;
    center_y = LOCDOUBLE( libmod_gfx, my, GRAPHCENTERY ) ;
    if ( center_x == POINT_UNDEFINED || center_y == POINT_UNDEFINED ) {
        if ( !graph->ncpoints || graph->cpoints[0].x == CPOINT_UNDEFINED ) {
            center_x = graph->width / 2.0;
            center_y = graph->height / 2.0;
        } else {
            center_x = graph->cpoints[0].x;
            center_y = graph->cpoints[0].y;
        }
    }

    /* Calculate dimensions */

    switch ( cbox->shape ) {
        case BITMAP_CB_SHAPE_BOX:
        default:
            x = cbox->x == POINT_UNDEFINED ? 0 : cbox->x;
            y = cbox->y == POINT_UNDEFINED ? 0 : cbox->y;
            width = ( cbox->width < 1 ) ? graph->width : cbox->width;
            height = ( cbox->height < 1 ) ? graph->height : cbox->height;
            break;

        case BITMAP_CB_SHAPE_CIRCLE:
            x = cbox->x == POINT_UNDEFINED ? center_x : cbox->x;
            y = cbox->y == POINT_UNDEFINED ? center_y : cbox->y;

            width = cbox->width;

            // Width is radius
            if ( cbox->width < 1 ) {
                switch ( cbox->width ) {
                    case BITMAP_CB_CIRCLE_GRAPH_SIZE:
                    case BITMAP_CB_CIRCLE_GRAPH_AVERAGE_SIZE:
                    default:
                        width = 0.5 + ( graph->width + graph->height ) / 4.0;
                        break;

                    case BITMAP_CB_CIRCLE_GRAPH_WIDTH:
                        width = 0.5 + graph->width / 2.0;
                        break;

                    case BITMAP_CB_CIRCLE_GRAPH_HEIGHT:
                        width = 0.5 + graph->height / 2.0;
                        break;

                    case BITMAP_CB_CIRCLE_GRAPH_MIN_SIZE:
                        width = 0.5 + MIN( graph->width, graph->height ) / 2.0;
                        break;

                    case BITMAP_CB_CIRCLE_GRAPH_MAX_SIZE:
                        width = 0.5 + MAX( graph->width, graph->height ) / 2.0;
                        break;
                }
            }
            * radius = width * scale_x;
            break;
    }

    /* Calculate coords */

    double x0 = LOCDOUBLE( libmod_gfx, my, COORDX );
    double y0 = LOCDOUBLE( libmod_gfx, my, COORDY );
    int64_t angle = LOCINT64( libmod_gfx, my, ANGLE );

    int64_t flags = LOCQWORD( libmod_gfx, my, FLAGS );

    int64_t resolution = LOCINT64( libmod_gfx, my, RESOLUTION );

    double sx = ( flags & B_HMIRROR ) ? -1 :  1;
    double sy = ( flags & B_VMIRROR ) ?  1 : -1;

    double cos_angle = cos_deg( angle );
    double sin_angle = sin_deg( angle );

    double * vertices = ( double * ) ( intptr_t ) params[1];

    double  lef_x, top_y, rig_x, bot_y, delta_x, delta_y;

    switch ( cbox->shape ) {
        case BITMAP_CB_SHAPE_BOX:
        default:
            lef_x = scale_x * ( x - center_x );
            rig_x = scale_x * ( width - 1 ) + lef_x;
            top_y = scale_y * ( y - center_y );
            bot_y = scale_y * ( height - 1 ) + top_y;
            vertices[0] = ( lef_x * cos_angle + top_y * sin_angle ) * sx + x0;
            vertices[1] = ( lef_x * sin_angle - top_y * cos_angle ) * sy + y0;
            vertices[2] = ( rig_x * cos_angle + top_y * sin_angle ) * sx + x0;
            vertices[3] = ( rig_x * sin_angle - top_y * cos_angle ) * sy + y0;
            vertices[4] = ( rig_x * cos_angle + bot_y * sin_angle ) * sx + x0;
            vertices[5] = ( rig_x * sin_angle - bot_y * cos_angle ) * sy + y0;
            vertices[6] = ( lef_x * cos_angle + bot_y * sin_angle ) * sx + x0;
            vertices[7] = ( lef_x * sin_angle - bot_y * cos_angle ) * sy + y0;

            if ( resolution != 1 ) {
                if ( resolution > 0 ) {
                    vertices[0] *= resolution;
                    vertices[1] *= resolution;
                    vertices[2] *= resolution;
                    vertices[3] *= resolution;
                    vertices[4] *= resolution;
                    vertices[5] *= resolution;
                    vertices[6] *= resolution;
                    vertices[7] *= resolution;
                } else if ( resolution < 0 ) {
                    vertices[0] /= -resolution;
                    vertices[1] /= -resolution;
                    vertices[2] /= -resolution;
                    vertices[3] /= -resolution;
                    vertices[4] /= -resolution;
                    vertices[5] /= -resolution;
                    vertices[6] /= -resolution;
                    vertices[7] /= -resolution;
                }
            }
            break;

        case BITMAP_CB_SHAPE_CIRCLE:
            delta_x = scale_x * ( x - center_x );
            delta_y = scale_y * ( y - center_y );
            vertices[0] = ( cos_angle * delta_x + sin_angle * delta_y ) * sx + x0;
            vertices[1] = ( sin_angle * delta_x - cos_angle * delta_y ) * sy + y0;
            if ( resolution != 1 ) {
                if ( resolution > 0 ) {
                    vertices[0] *= resolution;
                    vertices[1] *= resolution;
                } else if ( resolution < 0 ) {
                    vertices[0] /= -resolution;
                    vertices[1] /= -resolution;
                }
            }
            break;
    }

    return 1;

}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_get_real_box_vertex( INSTANCE * my, int64_t * params ) {
    GRAPH * graph;

    graph = bitmap_get( params[0], params[1] );
    if ( !graph ) return 0;

    return __get_real_box_vertex( graph, my, &params[2] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_get_real_box_vertex2( INSTANCE * my, int64_t * params ) {
    my = instance_get( params[0] );
    if ( !my ) return 0;
    return libmod_gfx_get_real_box_vertex( my, &params[1] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_get_real_box_vertex3( INSTANCE * my, int64_t * params ) {
    GRAPH * graph = instance_graph( my );
    if ( !graph ) return 0;
    return __get_real_box_vertex( graph, my, params );
}

/* --------------------------------------------------------------------------- */
