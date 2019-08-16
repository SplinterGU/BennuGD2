/*
 *  Copyright (C) 2006-2019 SplinterGU (Fenix/BennuGD)
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

#include "bgdrtm.h"
#include "bgddl.h"

#include "resolution.h"

#include "m_resolxy.h"

#include "libbggfx.h"
#include "libmod_gfx.h"

#include "fmath.h"

#include "dlvaracc.h"

/* --------------------------------------------------------------------------- */

typedef struct {
    CBOX cbox;
    double  vertices[8];
    BGD_Box limits;
} __cbox_info;

typedef struct {
    int64_t x;
    int64_t y;
    int64_t width;
    int64_t height;
    double  center_x;
    double  center_y;
    int64_t angle;
    int64_t flags;
    double  scale_x;
    double  scale_y;
    int64_t ncboxes;
    __cbox_info * cboxes;
} __obj_col_info;

/* --------------------------------------------------------------------------- */

#define in_radius(x,y,x2,y2,radius)             ((x-x2)*(x-x2)+(y-y2)*(y-y2))<((radius)*(radius))
#define in_radius2(x,y,x2,y2,radius,radius2)    ((x-x2)*(x-x2)+(y-y2)*(y-y2))<((radius+radius2)*(radius+radius2))

#define FREE(a) free(a);a=NULL

/* --------------------------------------------------------------------------- */
// scale must be with decimal points

static inline void __calculate_shape( __obj_col_info * oci )
{
    int i;
    double sx = 1, sy = -1;

    if ( oci->scale_x < 0.0 ) oci->scale_x = 0.0;
    if ( oci->scale_y < 0.0 ) oci->scale_y = 0.0;

    if ( oci->flags & B_HMIRROR ) sx = -1;
    if ( oci->flags & B_VMIRROR ) sy = 1;

    double cos_angle = cos_deg( oci->angle );
    double sin_angle = sin_deg( oci->angle );

    double  lef_x, top_y, rig_x, bot_y,
            delta_x, delta_y;

    for ( i = 0; i < oci->ncboxes; i++ ) {
        switch ( oci->cboxes[i].cbox.shape ) {
            case BITMAP_CB_SHAPE_BOX:
                lef_x = oci->scale_x * ( oci->cboxes[i].cbox.x - oci->center_x );
                rig_x = oci->scale_x * ( oci->cboxes[i].cbox.width - 1 ) + lef_x;
                top_y = oci->scale_y * ( oci->cboxes[i].cbox.y - oci->center_y );
                bot_y = oci->scale_y * ( oci->cboxes[i].cbox.height - 1 ) + top_y;

                oci->cboxes[i].vertices[0] = ( lef_x * cos_angle + top_y * sin_angle ) * sx + oci->x;
                oci->cboxes[i].vertices[1] = ( lef_x * sin_angle - top_y * cos_angle ) * sy + oci->y;
                oci->cboxes[i].vertices[2] = ( rig_x * cos_angle + top_y * sin_angle ) * sx + oci->x;
                oci->cboxes[i].vertices[3] = ( rig_x * sin_angle - top_y * cos_angle ) * sy + oci->y;
                oci->cboxes[i].vertices[4] = ( rig_x * cos_angle + bot_y * sin_angle ) * sx + oci->x;
                oci->cboxes[i].vertices[5] = ( rig_x * sin_angle - bot_y * cos_angle ) * sy + oci->y;
                oci->cboxes[i].vertices[6] = ( lef_x * cos_angle + bot_y * sin_angle ) * sx + oci->x;
                oci->cboxes[i].vertices[7] = ( lef_x * sin_angle - bot_y * cos_angle ) * sy + oci->y;
                break;

            case BITMAP_CB_SHAPE_CIRCLE:
                delta_x = oci->scale_x * ( oci->cboxes[i].cbox.x - oci->center_x );
                delta_y = oci->scale_y * ( oci->cboxes[i].cbox.y - oci->center_y );

                oci->cboxes[i].vertices[0] = ( cos_angle * delta_x + sin_angle * delta_y ) * sx + oci->x;
                oci->cboxes[i].vertices[1] = ( sin_angle * delta_x - cos_angle * delta_y ) * sy + oci->y;
                break;
        }
    }
}

/* --------------------------------------------------------------------------- */

static inline void __normalize_circle( double * vertices, int64_t x, int64_t y, int64_t angle, int64_t flags, double *normalized_vertices ) {
    double cos_angle;
    double sin_angle;
    double x1;
    double y1;

    if ( flags & B_HMIRROR ) angle = -angle;
    if ( flags & B_VMIRROR ) angle = -angle;

    cos_angle = cos_deg( angle );
    sin_angle = sin_deg( angle );

    x1 = *vertices++ - x;
    y1 = *vertices++ - y;

    *normalized_vertices++ = ( x1 * cos_angle - y1 * sin_angle ) + x; // x
    *normalized_vertices++ = ( x1 * sin_angle + y1 * cos_angle ) + y; // y
}

/* --------------------------------------------------------------------------- */

static inline void __normalize_box( double * vertices, int64_t x, int64_t y, int64_t angle, int64_t flags, double *normalized_vertices ) {
    double cos_angle;
    double sin_angle;
    double x1, x2, x3, x4;
    double y1, y2, y3, y4;

    if ( flags & B_HMIRROR ) angle = -angle;
    if ( flags & B_VMIRROR ) angle = -angle;

    cos_angle = cos_deg( angle );
    sin_angle = sin_deg( angle );

    x1 = *vertices++ - x;
    y1 = *vertices++ - y;
    x2 = *vertices++ - x;
    y2 = *vertices++ - y;
    x3 = *vertices++ - x;
    y3 = *vertices++ - y;
    x4 = *vertices++ - x;
    y4 = *vertices++ - y;

    *normalized_vertices++ = ( x1 * cos_angle - y1 * sin_angle ) + x; // x
    *normalized_vertices++ = ( x1 * sin_angle + y1 * cos_angle ) + y; // y
    *normalized_vertices++ = ( x2 * cos_angle - y2 * sin_angle ) + x; // x
    *normalized_vertices++ = ( x2 * sin_angle + y2 * cos_angle ) + y; // y
    *normalized_vertices++ = ( x3 * cos_angle - y3 * sin_angle ) + x; // x
    *normalized_vertices++ = ( x3 * sin_angle + y3 * cos_angle ) + y; // y
    *normalized_vertices++ = ( x4 * cos_angle - y4 * sin_angle ) + x; // x
    *normalized_vertices++ = ( x4 * sin_angle + y4 * cos_angle ) + y; // y
}

/* --------------------------------------------------------------------------- */

static inline void __get_vertices_proyection( double * normalized_vertices, BGD_Box * limits ) {
    limits->x = limits->x2 = *normalized_vertices++;
    limits->y = limits->y2 = *normalized_vertices++;

    if ( limits->x  > *normalized_vertices ) limits->x  = *normalized_vertices;
    if ( limits->x2 < *normalized_vertices ) limits->x2 = *normalized_vertices; normalized_vertices++;
    if ( limits->y  > *normalized_vertices ) limits->y  = *normalized_vertices;
    if ( limits->y2 < *normalized_vertices ) limits->y2 = *normalized_vertices; normalized_vertices++;
    if ( limits->x  > *normalized_vertices ) limits->x  = *normalized_vertices;
    if ( limits->x2 < *normalized_vertices ) limits->x2 = *normalized_vertices; normalized_vertices++;
    if ( limits->y  > *normalized_vertices ) limits->y  = *normalized_vertices;
    if ( limits->y2 < *normalized_vertices ) limits->y2 = *normalized_vertices; normalized_vertices++;
    if ( limits->x  > *normalized_vertices ) limits->x  = *normalized_vertices;
    if ( limits->x2 < *normalized_vertices ) limits->x2 = *normalized_vertices; normalized_vertices++;
    if ( limits->y  > *normalized_vertices ) limits->y  = *normalized_vertices;
    if ( limits->y2 < *normalized_vertices ) limits->y2 = *normalized_vertices; normalized_vertices++;

}

/* --------------------------------------------------------------------------- */

static inline void __calculate_box_limits( __obj_col_info * oci ) {
    int i;
    for ( i = 0; i < oci->ncboxes; i++ ) {
        if ( oci->cboxes[i].cbox.shape == BITMAP_CB_SHAPE_BOX ) {
            if ( oci->flags & B_HMIRROR ) {
                oci->cboxes[i].limits.x2 = oci->x + oci->scale_x * ( oci->center_x - oci->cboxes[i].cbox.x );
                oci->cboxes[i].limits.x = oci->cboxes[i].limits.x2 - oci->scale_x * ( oci->cboxes[i].cbox.width - 1 );
            } else {
                oci->cboxes[i].limits.x = oci->x + oci->scale_x * ( oci->cboxes[i].cbox.x - oci->center_x );
                oci->cboxes[i].limits.x2 = oci->cboxes[i].limits.x + oci->scale_x * ( oci->cboxes[i].cbox.width - 1 );
            }

            if ( oci->flags & B_VMIRROR ) {
                oci->cboxes[i].limits.y2 = oci->y + oci->scale_y * ( oci->center_y - oci->cboxes[i].cbox.y );
                oci->cboxes[i].limits.y = oci->cboxes[i].limits.y2 - oci->scale_y * ( oci->cboxes[i].cbox.height - 1 );
            } else {
                oci->cboxes[i].limits.y = oci->y + oci->scale_y * ( oci->cboxes[i].cbox.y - oci->center_y );
                oci->cboxes[i].limits.y2 = oci->cboxes[i].limits.y + oci->scale_y * ( oci->cboxes[i].cbox.height - 1 );
            }
        }
    }
}

/* --------------------------------------------------------------------------- */

static int __get_proc_info(
        INSTANCE * proc,
        __obj_col_info * oci // Box
) {
    GRAPH * graph;

    graph = instance_graph( proc );
    if ( !graph ) return 0;

    oci->scale_x = ( double ) LOCINT64( libmod_gfx, proc, GRAPHSIZEX );
    oci->scale_y = ( double ) LOCINT64( libmod_gfx, proc, GRAPHSIZEY );
    if ( oci->scale_x == 100.0 && oci->scale_y == 100.0 ) oci->scale_x = oci->scale_y = ( double ) LOCINT64( libmod_gfx, proc, GRAPHSIZE );

    oci->scale_x /= 100.0;
    oci->scale_y /= 100.0;

    oci->x = LOCINT64( libmod_gfx, proc, COORDX );
    oci->y = LOCINT64( libmod_gfx, proc, COORDY );

    RESOLXY( libmod_gfx, proc, oci->x, oci->y );

    oci->width  = LOCINT64( libmod_gfx, proc, CLIPW );
    oci->height = LOCINT64( libmod_gfx, proc, CLIPH );

    if ( !oci->width || !oci->height ) {
        oci->width  = graph->width;
        oci->height = graph->height;
    }

    /* Calculate the graphic center */

    oci->center_x = LOCINT64( libmod_gfx, proc, GRAPHCENTERX ) ;
    oci->center_y = LOCINT64( libmod_gfx, proc, GRAPHCENTERY ) ;
    if ( oci->center_x == POINT_UNDEFINED || oci->center_y == POINT_UNDEFINED ) {
        if ( graph->ncpoints && graph->cpoints[0].x != CPOINT_UNDEFINED ) {
            oci->center_x = graph->cpoints[0].x;
            oci->center_y = graph->cpoints[0].y;
        } else {
            oci->center_x = oci->width  / 2.0;
            oci->center_y = oci->height / 2.0;
        }
    }

    oci->angle = LOCINT64( libmod_gfx, proc, ANGLE );
    oci->flags = LOCQWORD( libmod_gfx, proc, FLAGS );

    int i;
    if ( graph->ncboxes ) {
        oci->cboxes = malloc( graph->ncboxes * sizeof( __cbox_info ) );
        if ( !oci->cboxes ) return 0;
        oci->ncboxes = graph->ncboxes;
        for ( i = 0; i < oci->ncboxes; i++ ) oci->cboxes[i].cbox = graph->cboxes[i];
    } else { // No graph cbox defined, use a virtual cbox
        oci->cboxes = malloc( sizeof( __cbox_info ) );
        if ( !oci->cboxes ) return 0;
        oci->ncboxes = 1;
        oci->cboxes->cbox.code   = -1;
        oci->cboxes->cbox.shape  = LOCINT64( libmod_gfx, proc, CSHAPE );
        oci->cboxes->cbox.x      = LOCINT64( libmod_gfx, proc, CBOX_X );
        oci->cboxes->cbox.y      = LOCINT64( libmod_gfx, proc, CBOX_Y );
        oci->cboxes->cbox.width  = LOCINT64( libmod_gfx, proc, CBOX_WIDTH );
        oci->cboxes->cbox.height = LOCINT64( libmod_gfx, proc, CBOX_HEIGHT );
    }

    for ( i = 0; i < oci->ncboxes; i++ ) {
        switch ( oci->cboxes[i].cbox.shape ) {
            case BITMAP_CB_SHAPE_BOX:
            default:
                oci->cboxes[i].cbox.x = oci->cboxes[i].cbox.x == POINT_UNDEFINED ? 0 : oci->cboxes[i].cbox.x;
                oci->cboxes[i].cbox.y = oci->cboxes[i].cbox.y == POINT_UNDEFINED ? 0 : oci->cboxes[i].cbox.y;
                oci->cboxes[i].cbox.width = ( oci->cboxes[i].cbox.width < 1 ) ? oci->width : oci->cboxes[i].cbox.width;
                oci->cboxes[i].cbox.height = ( oci->cboxes[i].cbox.height < 1 ) ? oci->height : oci->cboxes[i].cbox.height;
                break;

            case BITMAP_CB_SHAPE_CIRCLE:
                oci->cboxes[i].cbox.x = oci->cboxes[i].cbox.x == POINT_UNDEFINED ? oci->center_x : oci->cboxes[i].cbox.x;
                oci->cboxes[i].cbox.y = oci->cboxes[i].cbox.y == POINT_UNDEFINED ? oci->center_y : oci->cboxes[i].cbox.y;

                // Width is radius
                if ( oci->cboxes[i].cbox.width < 1 ) {
                    switch ( oci->cboxes[i].cbox.width ) {
                        case BITMAP_CB_CIRCLE_GRAPH_SIZE:
                        case BITMAP_CB_CIRCLE_GRAPH_AVERAGE_SIZE:
                        default:
                            oci->cboxes[i].cbox.width = 0.5 + ( oci->width + oci->height ) / 4.0;
                            break;

                        case BITMAP_CB_CIRCLE_GRAPH_WIDTH:
                            oci->cboxes[i].cbox.width = 0.5 + oci->width / 2.0;
                            break;

                        case BITMAP_CB_CIRCLE_GRAPH_HEIGHT:
                            oci->cboxes[i].cbox.width = 0.5 + oci->height / 2.0;
                            break;

                        case BITMAP_CB_CIRCLE_GRAPH_MIN_SIZE:
                            oci->cboxes[i].cbox.width = 0.5 + MIN( oci->width, oci->height ) / 2.0;
                            break;

                        case BITMAP_CB_CIRCLE_GRAPH_MAX_SIZE:
                            oci->cboxes[i].cbox.width = 0.5 + MAX( oci->width, oci->height ) / 2.0;
                            break;
                    }
                }
                break;
        }
    }

    return 1;

}

/* --------------------------------------------------------------------------- */

static int __get_mouse_info( __obj_col_info * oci ) {

    oci->scale_x = 1.0;
    oci->scale_y = 1.0;

    oci->x = GLOINT64( libmod_gfx, MOUSEX );
    oci->y = GLOINT64( libmod_gfx, MOUSEY );

    oci->width  = 1;
    oci->height = 1;

    /* Calculate the graphic center */

    oci->center_x = 0;
    oci->center_y = 0;

    oci->angle = 0;
    oci->flags = 0;

    oci->cboxes = malloc( sizeof( __cbox_info ) );
    if ( !oci->cboxes ) return 0;
    oci->ncboxes = 1;
    oci->cboxes->cbox.code   = -1;
    oci->cboxes->cbox.shape  = BITMAP_CB_SHAPE_BOX;
    oci->cboxes->cbox.x      = 0;
    oci->cboxes->cbox.y      = 0;
    oci->cboxes->cbox.width  = 1;
    oci->cboxes->cbox.height = 1;

    return 1;
}

/* --------------------------------------------------------------------------- */

static inline int __is_collision_box_circle( BGD_Box * limits, int64_t radius, double * normalized_vertices ) {
    double closest_x, closest_y;

    // Find the unrotated closest x point from center of unrotated circle
    if ( normalized_vertices[0] < limits->x )       closest_x = limits->x;
    else if ( normalized_vertices[0] > limits->x2 ) closest_x = limits->x2;
    else                                            closest_x = normalized_vertices[0];

    // Find the unrotated closest y point from center of unrotated circle
    if ( normalized_vertices[1] < limits->y )       closest_y = limits->y;
    else if ( normalized_vertices[1] > limits->y2 ) closest_y = limits->y2;
    else                                            closest_y = normalized_vertices[1];

    return in_radius( closest_x, closest_y, normalized_vertices[0], normalized_vertices[1], radius );

}

/* --------------------------------------------------------------------------- */

static inline int __is_collision_circle_circle( int64_t radiusA, double * verticesA, int64_t radiusB, double * verticesB ) {

    return in_radius2( verticesA[0], verticesA[1], verticesB[0], verticesB[1], radiusA, radiusB );
}

/* --------------------------------------------------------------------------- */
/*  ociA is precalculated, is collider */

static inline int __check_collision( __obj_col_info * ociA, __obj_col_info * ociB, int64_t * cbox_code ) {
    int i, ii, collisionA, collisionB;
    int64_t shapeA, shapeB;
    BGD_Box proyectionA, proyectionB;
    double normalized_verticesA[8], normalized_verticesB[8], normalized_verticesC[8];

    // Get real vertices
    __calculate_shape( ociB );
    __calculate_box_limits( ociB );

    for ( i = 0; i < ociA->ncboxes; i++ ) {
        shapeA = ociA->cboxes[i].cbox.shape;
        switch ( shapeA ) {
            case BITMAP_CB_SHAPE_BOX:
                __normalize_box( ociA->cboxes[i].vertices, ociB->x, ociB->y, ociB->angle, ociB->flags, normalized_verticesA ); // Normalize to Box axis
                __get_vertices_proyection( normalized_verticesA, &proyectionA ); // Get limits
                break;

            case BITMAP_CB_SHAPE_CIRCLE:
                __normalize_circle( ociA->cboxes[i].vertices, ociB->x, ociB->y, ociB->angle, ociB->flags, normalized_verticesA ); // Normalize to Box axis
                break;
        }

        /* ***** Iterate ***** */
        /* A vs each B */
        for ( ii = 0; ii < ociB->ncboxes; ii++ ) {
            collisionA = collisionB = 0;
            shapeB = ociB->cboxes[ii].cbox.shape;
            switch ( shapeB ) {
                case BITMAP_CB_SHAPE_BOX:
                    switch ( shapeA ) {
                        case BITMAP_CB_SHAPE_BOX:
                            collisionA = !( proyectionA.x > ociB->cboxes[ii].limits.x2 || proyectionA.x2 < ociB->cboxes[ii].limits.x || proyectionA.y > ociB->cboxes[ii].limits.y2 || proyectionA.y2 < ociB->cboxes[ii].limits.y );
                            // If not collisionA then don't need check for second box
                            if ( collisionA ) {
                                __normalize_box( ociB->cboxes[ii].vertices, ociA->x, ociA->y, ociA->angle, ociA->flags, normalized_verticesB ); // Normalize to Box axis
                                __get_vertices_proyection( normalized_verticesB, &proyectionB ); // Get limits
                                collisionB = !( proyectionB.x > ociA->cboxes[i].limits.x2 || proyectionB.x2 < ociA->cboxes[i].limits.x || proyectionB.y > ociA->cboxes[i].limits.y2 || proyectionB.y2 < ociA->cboxes[i].limits.y );
                            }
                            break;

                        case BITMAP_CB_SHAPE_CIRCLE:
                            collisionB = __is_collision_box_circle( &ociB->cboxes[ii].limits, ociA->cboxes[i].cbox.width * ociA->scale_x, normalized_verticesA );
                            break;
                    }
                    break;

                case BITMAP_CB_SHAPE_CIRCLE:
                    switch ( shapeA ) {
                        case BITMAP_CB_SHAPE_BOX:
                            __normalize_circle( ociB->cboxes[ii].vertices, ociA->x, ociA->y, ociA->angle, ociA->flags, normalized_verticesB );
                            collisionB = __is_collision_box_circle( &ociA->cboxes[i].limits, ociB->cboxes[ii].cbox.width * ociB->scale_x, normalized_verticesB ); // Normalize to Box axis
                            break;

                        case BITMAP_CB_SHAPE_CIRCLE:
                            collisionB = __is_collision_circle_circle( ociA->cboxes[i].cbox.width * ociA->scale_x, ociA->cboxes[i].vertices, ociB->cboxes[ii].cbox.width * ociB->scale_x, ociB->cboxes[ii].vertices );
                            break;
                    }
                    break;
            }

            if ( collisionB ) {
                cbox_code[0] = ociA->cboxes[i].cbox.code;
                cbox_code[1] = ociB->cboxes[ii].cbox.code;
                return 1;
            }
        }
    }

    return 0;
}

/* --------------------------------------------------------------------------- */

static int64_t __collision( INSTANCE * my, int64_t id ) {
    INSTANCE * ptr, ** ctx;
    __obj_col_info * oci, * ociB;
    int collision = 0;
    int64_t cbox_code[2];

    oci = malloc(sizeof(__obj_col_info));
    if ( !oci ) return 0;

    ociB = malloc(sizeof(__obj_col_info));
    if ( !ociB ) {
        FREE( oci );
        return 0;
    }

    if ( !__get_proc_info( my, oci ) ) {
        FREE( oci );
        FREE( ociB );
        return 0;
    }

    // Get real vertices
    __calculate_shape( oci );
    __calculate_box_limits( oci );

    /* Checks collision with mouse */
    if ( id == -1 ) {
        collision = 0;
        if ( __get_mouse_info( ociB ) ) {
            if ( LOCQWORD( libmod_gfx, my, CTYPE ) == C_SCROLL ) {
                int i;

                int64_t cnumber = LOCQWORD( libmod_gfx, my, CNUMBER );
                if ( !cnumber ) cnumber = 0xffffffff;

                for ( i = 0; i < MAX_SCROLLS && !collision; i++ ) {
                    if ( scrolls[i].active && ( cnumber & ( 1 << i ) ) ) {
                        REGION * r = scrolls[i].region;

                        ociB->x += scrolls[i].posx0 + r->x;
                        ociB->y += scrolls[i].posy0 + r->y;

                        collision = __check_collision( oci, ociB, cbox_code ) ;

                        ociB->x -= scrolls[i].posx0 + r->x;
                        ociB->y -= scrolls[i].posy0 + r->y;
                    }
                }
                FREE( ociB->cboxes ); FREE( ociB );
                FREE( oci->cboxes ); FREE( oci );
                if ( collision ) {
                    LOCINT64( libmod_gfx, my, COLLIDER_CBOX ) = -1;
                    LOCINT64( libmod_gfx, my, COLLIDED_ID   ) = -1;
                    LOCINT64( libmod_gfx, my, COLLIDED_CBOX ) = cbox_code[1];
                    return 1;
                }
                return 0;
            }
            collision = __check_collision( oci, ociB, cbox_code ) ;
            FREE( ociB->cboxes );
        }
        FREE( ociB );
        free( oci->cboxes ); free( oci );
        if ( collision ) {
            LOCINT64( libmod_gfx, my, COLLIDER_CBOX ) = -1;
            LOCINT64( libmod_gfx, my, COLLIDED_ID   ) = -1;
            LOCINT64( libmod_gfx, my, COLLIDED_CBOX ) = cbox_code[1];
            return 1;
        }
        return 0;
    }

    int64_t ctype = LOCQWORD( libmod_gfx, my, CTYPE );

    /* Checks only for a single instance */
    if ( id >= FIRST_INSTANCE_ID ) {
        ptr = instance_get( id );
        if ( ptr && ctype == LOCQWORD( libmod_gfx, ptr, CTYPE ) && LOCQWORD( libmod_gfx, ptr, STATUS ) & ( STATUS_RUNNING | STATUS_FROZEN ) && ptr != my ) {
            if ( __get_proc_info( ptr, ociB ) ) {
                collision = __check_collision( oci, ociB, cbox_code ) ;
                FREE( ociB->cboxes );
                if ( collision ) {
                    FREE( ociB );
                    FREE( oci->cboxes ); FREE( oci );
                    LOCINT64( libmod_gfx, my, COLLIDER_CBOX ) = cbox_code[0];
                    LOCINT64( libmod_gfx, my, COLLIDED_ID   ) = LOCQWORD( libmod_gfx, ptr, PROCESS_ID );
                    LOCINT64( libmod_gfx, my, COLLIDED_CBOX ) = cbox_code[1];
                    return LOCQWORD( libmod_gfx, ptr, PROCESS_ID );;
                }
            }
        }
        FREE( ociB );
        FREE( oci->cboxes ); FREE( oci );
        return 0;
    }

    /* we must use full list of instances or get types from it */
    ptr = first_instance;

    if ( !id ) {
        int p;
        LOCQWORD( libmod_gfx, my, COLLISION_RESERVED_TYPE_SCAN ) = 0;
        if ( ( p = LOCQWORD( libmod_gfx, my, COLLISION_RESERVED_ID_SCAN ) ) ) {
            ptr = instance_get( p );
            if ( ptr ) ptr = ptr->next;
        }

        while ( ptr ) {
            if ( ctype == LOCQWORD( libmod_gfx, ptr, CTYPE ) && LOCQWORD( libmod_gfx, ptr, STATUS ) & ( STATUS_RUNNING | STATUS_FROZEN ) && ptr != my ) {
                if ( __get_proc_info( ptr, ociB ) ) {
                    collision = __check_collision( oci, ociB, cbox_code ) ;
                    FREE( ociB->cboxes );
                    if ( collision ) {
                        FREE( ociB );
                        FREE( oci->cboxes ); FREE( oci );
                        LOCQWORD( libmod_gfx, my, COLLISION_RESERVED_ID_SCAN ) = LOCQWORD( libmod_gfx, ptr, PROCESS_ID );
                        LOCINT64( libmod_gfx, my, COLLIDER_CBOX ) = cbox_code[0];
                        LOCINT64( libmod_gfx, my, COLLIDED_ID   ) = LOCQWORD( libmod_gfx, ptr, PROCESS_ID );
                        LOCINT64( libmod_gfx, my, COLLIDED_CBOX ) = cbox_code[1];
                        return LOCQWORD( libmod_gfx, ptr, PROCESS_ID );;
                    }
                }
            }
            ptr = ptr->next;
        }
        FREE( ociB );
        FREE( oci->cboxes ); FREE( oci );
        LOCQWORD( libmod_gfx, my, COLLISION_RESERVED_ID_SCAN ) = 0;
        return 0;
    }

    LOCQWORD( libmod_gfx, my, COLLISION_RESERVED_ID_SCAN ) = 0;
    /* Check if already in scan by type and we reach limit */
    ctx = ( INSTANCE ** ) LOCADDR( libmod_gfx, my, COLLISION_RESERVED_CONTEXT );
    if ( LOCQWORD( libmod_gfx, my, COLLISION_RESERVED_TYPE_SCAN ) != id ) { /* Check if type change from last call */
        *ctx = NULL;
        LOCQWORD( libmod_gfx, my, COLLISION_RESERVED_TYPE_SCAN ) = id;
    }

    while ( ( ptr = instance_get_by_type( id, ctx ) ) ) {
        if ( ctype == LOCQWORD( libmod_gfx, ptr, CTYPE ) && LOCQWORD( libmod_gfx, ptr, STATUS ) & ( STATUS_RUNNING | STATUS_FROZEN ) && ptr != my ) {
            if ( __get_proc_info( ptr, ociB ) ) {
                collision = __check_collision( oci, ociB, cbox_code ) ;
                FREE( ociB->cboxes );
                if ( collision ) {
                    FREE( ociB );
                    FREE( oci->cboxes ); FREE( oci );
                    LOCQWORD( libmod_gfx, my, COLLISION_RESERVED_ID_SCAN ) = LOCQWORD( libmod_gfx, ptr, PROCESS_ID );
                    LOCINT64( libmod_gfx, my, COLLIDER_CBOX ) = cbox_code[0];
                    LOCINT64( libmod_gfx, my, COLLIDED_ID   ) = LOCQWORD( libmod_gfx, ptr, PROCESS_ID );
                    LOCINT64( libmod_gfx, my, COLLIDED_CBOX ) = cbox_code[1];
                    return LOCQWORD( libmod_gfx, ptr, PROCESS_ID );;
                }
            }
        }
    }
    FREE( ociB );
    FREE( oci->cboxes ); FREE( oci );
    LOCQWORD( libmod_gfx, my, COLLISION_RESERVED_TYPE_SCAN ) = 0;
    return 0;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_collision( INSTANCE * my, int64_t * params ) {
    return __collision( my, params[ 0 ] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_collision2( INSTANCE * my, int64_t * params ) {
    my = instance_get( params[ 0 ] );
    if ( !my ) return 0;
    return __collision( my, params[ 1 ] );
}

/* --------------------------------------------------------------------------- */
