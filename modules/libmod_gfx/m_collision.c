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
    double region_radius;
} __proc_col_info;

/* --------------------------------------------------------------------------- */

#define in_radius(x,y,x2,y2,radius)             ((x-x2)*(x-x2)+(y-y2)*(y-y2))<((radius)*(radius))
#define in_radius2(x,y,x2,y2,radius,radius2)    ((x-x2)*(x-x2)+(y-y2)*(y-y2))<((radius+radius2)*(radius+radius2))

/* --------------------------------------------------------------------------- */
// scale must be with decimal points

static /* inline */ void __calculate_shape( __proc_col_info * pci )
{
    int i;
    double sx = 1, sy = -1;

    if ( pci->scale_x < 0.0 ) pci->scale_x = 0.0;
    if ( pci->scale_y < 0.0 ) pci->scale_y = 0.0;

    if ( pci->flags & B_HMIRROR ) sx = -1;
    if ( pci->flags & B_VMIRROR ) sy = 1;

    double cos_angle = cos_deg( pci->angle );
    double sin_angle = sin_deg( pci->angle );

    double  lef_x, top_y, rig_x, bot_y,
            delta_x, delta_y;

    for ( i = 0; i < pci->ncboxes; i++ ) {
        switch ( pci->cboxes[i].cbox.shape ) {
            case BITMAP_CB_SHAPE_BOX:
                lef_x = pci->scale_x * -( pci->center_x + pci->cboxes[i].cbox.x );
                rig_x = pci->scale_x * pci->cboxes[i].cbox.width + lef_x;
                top_y = pci->scale_y * -( pci->center_y + pci->cboxes[i].cbox.y );
                bot_y = pci->scale_y * pci->cboxes[i].cbox.height + top_y;

                pci->cboxes[i].vertices[0] = ( lef_x * cos_angle + top_y * sin_angle ) * sx + pci->x;
                pci->cboxes[i].vertices[1] = ( lef_x * sin_angle - top_y * cos_angle ) * sy + pci->y;
                pci->cboxes[i].vertices[2] = ( rig_x * cos_angle + top_y * sin_angle ) * sx + pci->x;
                pci->cboxes[i].vertices[3] = ( rig_x * sin_angle - top_y * cos_angle ) * sy + pci->y;
                pci->cboxes[i].vertices[4] = ( rig_x * cos_angle + bot_y * sin_angle ) * sx + pci->x;
                pci->cboxes[i].vertices[5] = ( rig_x * sin_angle - bot_y * cos_angle ) * sy + pci->y;
                pci->cboxes[i].vertices[6] = ( lef_x * cos_angle + bot_y * sin_angle ) * sx + pci->x;
                pci->cboxes[i].vertices[7] = ( lef_x * sin_angle - bot_y * cos_angle ) * sy + pci->y;
                break;

            case BITMAP_CB_SHAPE_CIRCLE:
                delta_x = pci->scale_x * ( pci->center_x + pci->cboxes[i].cbox.x );
                delta_y = pci->scale_y * ( pci->center_y + pci->cboxes[i].cbox.y );

                pci->cboxes[i].vertices[0] = ( cos_angle * delta_x + sin_angle * delta_y ) * sx + pci->x;
                pci->cboxes[i].vertices[1] = ( sin_angle * delta_x - cos_angle * delta_y ) * sy + pci->y;
                break;
        }
    }
}

/* --------------------------------------------------------------------------- */

static /* inline */ void __normalize_circle( double * vertices, int64_t x, int64_t y, int64_t angle, int64_t flags, double *normalized_vertices ) {
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

static /* inline */ void __normalize_box( double * vertices, int64_t x, int64_t y, int64_t angle, int64_t flags, double *normalized_vertices ) {
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

static /* inline */ void __get_vertices_proyection( double * normalized_vertices, BGD_Box * limits ) {
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

static /* inline */ void __calculate_box_limits( __proc_col_info * pci ) {
    int i;

    for ( i = 0; i < pci->ncboxes; i++ ) {
        if ( pci->cboxes[i].cbox.shape == BITMAP_CB_SHAPE_BOX ) {
            if ( pci->flags & B_HMIRROR ) {
                pci->cboxes[i].limits.x2 = pci->x + pci->scale_x * ( pci->center_x + pci->cboxes[i].cbox.x );
                pci->cboxes[i].limits.x = pci->cboxes[i].limits.x2 - pci->scale_x * pci->cboxes[i].cbox.width;
            } else {
                pci->cboxes[i].limits.x = pci->x - pci->scale_x * ( pci->center_x + pci->cboxes[i].cbox.x );
                pci->cboxes[i].limits.x2 = pci->cboxes[i].limits.x + pci->scale_x * pci->cboxes[i].cbox.width;
            }

            if ( pci->flags & B_VMIRROR ) {
                pci->cboxes[i].limits.y2 = pci->y + pci->scale_y * ( pci->center_y + pci->cboxes[i].cbox.y );
                pci->cboxes[i].limits.y = pci->cboxes[i].limits.y2 - pci->scale_y * pci->cboxes[i].cbox.height;
            } else {
                pci->cboxes[i].limits.y = pci->y - pci->scale_y * ( pci->center_y + pci->cboxes[i].cbox.y );
                pci->cboxes[i].limits.y2 = pci->cboxes[i].limits.y + pci->scale_y * pci->cboxes[i].cbox.height;
            }
        }
    }
}

/* --------------------------------------------------------------------------- */

static int __get_proc_info(
        INSTANCE * proc,
        __proc_col_info * pci // Box
) {
    GRAPH * graph;

    graph = instance_graph( proc );
    if ( !graph ) return 0;

    pci->scale_x = LOCINT64( libmod_gfx, proc, GRAPHSIZEX );
    pci->scale_y = LOCINT64( libmod_gfx, proc, GRAPHSIZEY );
    if ( pci->scale_x == 100 && pci->scale_y == 100 ) pci->scale_x = pci->scale_y = LOCINT64( libmod_gfx, proc, GRAPHSIZE );

    pci->scale_x /= 100.0;
    pci->scale_y /= 100.0;

    pci->x = LOCINT64( libmod_gfx, proc, COORDX );
    pci->y = LOCINT64( libmod_gfx, proc, COORDY );

    RESOLXY( libmod_gfx, proc, pci->x, pci->y );

    pci->width  = LOCINT64( libmod_gfx, proc, CLIPW );
    pci->height = LOCINT64( libmod_gfx, proc, CLIPH );

    if ( !pci->width || !pci->height ) {
        pci->width  = graph->width;
        pci->height = graph->height;
    }

    /* Calculate the graphic center */

    if ( graph->ncpoints && graph->cpoints[0].x != CPOINT_UNDEFINED ) {
        pci->center_x = graph->cpoints[0].x;
        pci->center_y = graph->cpoints[0].y;
    } else {
        pci->center_x = pci->width  / 2.0;
        pci->center_y = pci->height / 2.0;
    }

    pci->angle = LOCINT64( libmod_gfx, proc, ANGLE );
    pci->flags = LOCQWORD( libmod_gfx, proc, FLAGS );

    int i;
    if ( graph->ncboxes ) {
        pci->cboxes = malloc( graph->ncboxes * sizeof( __cbox_info ) );
        if ( !pci->cboxes ) return 0;
        pci->ncboxes = graph->ncboxes;
        for ( i = 0; i < pci->ncboxes; i++ ) {
            pci->cboxes[i].cbox = graph->cboxes[i];
if ( pci->cboxes[i].cbox.code != -1 )
        printf("* %"PRId64" %s %"PRId64" %"PRId64" %"PRId64" %"PRId64"\n",
            pci->cboxes[i].cbox.code,
            pci->cboxes[i].cbox.shape == BITMAP_CB_SHAPE_BOX ? "BOX" : "CIRCLE" ,
            pci->cboxes[i].cbox.x,
            pci->cboxes[i].cbox.y,
            pci->cboxes[i].cbox.width,
            pci->cboxes[i].cbox.height
            );
        }
    } else { // No graph cbox defined, use a virtual cbox
        pci->cboxes = malloc( sizeof( __cbox_info ) );
        if ( !pci->cboxes ) return 0;
        pci->ncboxes = 1;
        pci->cboxes->cbox.code   = -1;
        pci->cboxes->cbox.shape  = LOCINT64( libmod_gfx, proc, CSHAPE );
        pci->cboxes->cbox.x      = LOCINT64( libmod_gfx, proc, CBOX_X );
        pci->cboxes->cbox.y      = LOCINT64( libmod_gfx, proc, CBOX_Y );
        pci->cboxes->cbox.width  = LOCINT64( libmod_gfx, proc, CBOX_WIDTH );
        pci->cboxes->cbox.height = LOCINT64( libmod_gfx, proc, CBOX_HEIGHT );
    }

    int m = 0;
    pci->region_radius = 0;
    double scale = MAX( pci->scale_x, pci->scale_y );

    for ( i = 0; i < pci->ncboxes; i++ ) {
        switch ( pci->cboxes[i].cbox.shape ) {
            case BITMAP_CB_SHAPE_BOX:
            default:
                pci->cboxes[i].cbox.x = pci->cboxes[i].cbox.x == POINT_UNDEFINED ? 0 : pci->cboxes[i].cbox.x;
                pci->cboxes[i].cbox.y = pci->cboxes[i].cbox.y == POINT_UNDEFINED ? 0 : pci->cboxes[i].cbox.y;
                pci->cboxes[i].cbox.width = ( pci->cboxes[i].cbox.width < 1 ) ? pci->width : pci->cboxes[i].cbox.width;
                pci->cboxes[i].cbox.height = ( pci->cboxes[i].cbox.height < 1 ) ? pci->height : pci->cboxes[i].cbox.height;
                m = MAX( pci->cboxes[i].cbox.x + pci->cboxes[i].cbox.width, pci->cboxes[i].cbox.y + pci->cboxes[i].cbox.height );
                break;

            case BITMAP_CB_SHAPE_CIRCLE:
                pci->cboxes[i].cbox.x = pci->cboxes[i].cbox.x == POINT_UNDEFINED ? pci->center_x : pci->cboxes[i].cbox.x;
                pci->cboxes[i].cbox.y = pci->cboxes[i].cbox.y == POINT_UNDEFINED ? pci->center_y : pci->cboxes[i].cbox.y;

                // Width is radius
                if ( pci->cboxes[i].cbox.width < 1 ) {
                    switch ( pci->cboxes[i].cbox.width ) {
                        case BITMAP_CB_CIRCLE_GRAPH_SIZE:
                        case BITMAP_CB_CIRCLE_GRAPH_AVERAGE_SIZE:
                        default:
                            pci->cboxes[i].cbox.width = ( pci->width + pci->height ) / 4;
                            break;

                        case BITMAP_CB_CIRCLE_GRAPH_WIDTH:
                            pci->cboxes[i].cbox.width = pci->width / 2;
                            break;

                        case BITMAP_CB_CIRCLE_GRAPH_HEIGHT:
                            pci->cboxes[i].cbox.width = pci->height / 2;
                            break;

                        case BITMAP_CB_CIRCLE_GRAPH_MIN_SIZE:
                            pci->cboxes[i].cbox.width = MIN( pci->width, pci->height ) / 2;
                            break;

                        case BITMAP_CB_CIRCLE_GRAPH_MAX_SIZE:
                            pci->cboxes[i].cbox.width = MAX( pci->width, pci->height ) / 2;
                            break;
                    }
                }
                m = MAX( pci->cboxes[i].cbox.x + pci->cboxes[i].cbox.width, pci->cboxes[i].cbox.y + pci->cboxes[i].cbox.width );
                break;
        }
        pci->region_radius = MAX( pci->region_radius, m * scale );
if ( pci->cboxes[i].cbox.code != -1 )
        printf("** %"PRId64" %s %"PRId64" %"PRId64" %"PRId64" %"PRId64"\n",
            pci->cboxes[i].cbox.code,
            pci->cboxes[i].cbox.shape == BITMAP_CB_SHAPE_BOX ? "BOX" : "CIRCLE" ,
            pci->cboxes[i].cbox.x,
            pci->cboxes[i].cbox.y,
            pci->cboxes[i].cbox.width,
            pci->cboxes[i].cbox.height
            );

    }
if ( pci->cboxes[0].cbox.code != -1 )
    printf("** %f\n", pci->region_radius);
    return 1;

}

/* --------------------------------------------------------------------------- */

static /* inline */ int __is_collision_box_circle( BGD_Box * limits, int64_t radius, double * normalized_vertices ) {
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
/*
    double dx, dy;

    dx = ( closest_x - normalized_vertices[0] );
    dy = ( closest_y - normalized_vertices[1] );

    return ( dx * dx ) + ( dy * dy ) < radius * radius;
*/
}

/* --------------------------------------------------------------------------- */
/*  pciA is precalculated, is collider */

static /* inline */ int __check_collision( __proc_col_info * pciA, __proc_col_info * pciB ) {
    int i, ii, collision1, collision2;
    BGD_Box proyection;
    int64_t shapeA, shapeB;

    double normalized_vertices[8];

    // Get real vertices
    __calculate_shape( pciB );
    __calculate_box_limits( pciB );

    for ( i = 0; i < pciB->ncboxes; i++ ) {
        shapeB = pciB->cboxes[i].cbox.shape;
        switch ( shapeB ) {
            case BITMAP_CB_SHAPE_BOX:
                // Normalize to Box axis
                __normalize_box( pciB->cboxes[i].vertices, pciA->x, pciA->y, pciA->angle, pciA->flags, normalized_vertices );

                // Get limits
                __get_vertices_proyection( normalized_vertices, &proyection );
                break;

            case BITMAP_CB_SHAPE_CIRCLE:
                // Normalize to Box axis
                __normalize_circle( pciB->cboxes[i].vertices, pciA->x, pciA->y, pciA->angle, pciA->flags, normalized_vertices );
                break;
        }

        /* ***** Iterate ***** */

        for ( ii = 0; ii < pciA->ncboxes; ii++ ) {
            collision1 = 0;
            shapeA = pciA->cboxes[ii].cbox.shape;
            switch ( shapeA ) {
                case BITMAP_CB_SHAPE_BOX:
                    switch ( shapeB ) {
                        case BITMAP_CB_SHAPE_BOX:
                            collision1 = !( proyection.x > pciA->cboxes[ii].limits.x2 || proyection.x2 < pciA->cboxes[ii].limits.x || proyection.y > pciA->cboxes[ii].limits.y2 || proyection.y2 < pciA->cboxes[ii].limits.y );
                            break;

                        case BITMAP_CB_SHAPE_CIRCLE:
                            collision1 = __is_collision_box_circle( &pciA->cboxes[ii].limits, pciB->cboxes[i].cbox.width * pciB->scale_x, normalized_vertices );
                            break;
                    }
                    break;

                case BITMAP_CB_SHAPE_CIRCLE:
                    switch ( shapeB ) {
                        case BITMAP_CB_SHAPE_BOX:
                            // Normalize to Box axis
                            __normalize_circle( pciA->cboxes[ii].vertices, pciB->x, pciB->y, pciB->angle, pciB->flags, normalized_vertices );
                            collision1 = __is_collision_box_circle( &pciB->cboxes[i].limits, pciA->cboxes[ii].cbox.width * pciA->scale_x, normalized_vertices );
                            break;

                        case BITMAP_CB_SHAPE_CIRCLE:
                            break;
                    }
                    break;
            }

            if ( !collision1 ) continue; // Don't need check for second box if 1 box collision fail

            /* ***** 2nd Box ***** */

            collision2 = 0;

            switch ( shapeA ) {
                case BITMAP_CB_SHAPE_BOX:
                    switch ( shapeB ) {
                        case BITMAP_CB_SHAPE_BOX:
                            // Normalize to Box axis
                            __normalize_box( pciA->cboxes[ii].vertices, pciB->x, pciB->y, pciB->angle, pciB->flags, normalized_vertices );

                            // Get limits
                            __get_vertices_proyection( normalized_vertices, &proyection );

                            collision2 = !( proyection.x > pciB->cboxes[i].limits.x2 || proyection.x2 < pciB->cboxes[i].limits.x || proyection.y > pciB->cboxes[i].limits.y2 || proyection.y2 < pciB->cboxes[i].limits.y );
                            break;

                        case BITMAP_CB_SHAPE_CIRCLE:
                            // Normalize to Box axis
                            __normalize_circle( pciA->cboxes[i].vertices, pciB->x, pciB->y, pciB->angle, pciB->flags, normalized_vertices );
                            break;
                    }
                    break;

                case BITMAP_CB_SHAPE_CIRCLE:
                    switch ( shapeB ) {
                        case BITMAP_CB_SHAPE_BOX:
                            break;

                        case BITMAP_CB_SHAPE_CIRCLE:
                            break;
                    }
                    break;
            }

            if ( collision1 && collision2 ) return 1;
        }
    }
    return 0;
}

/* --------------------------------------------------------------------------- */

static int64_t __collision( INSTANCE * my, int64_t id ) {
//    int ( *colfunc )( INSTANCE *, REGION *, INSTANCE * );
    INSTANCE * ptr, ** ctx;
  __proc_col_info pci, pciB;
  int collision = 0;

//    if ( id == -1 ) return ( check_collision_with_mouse( my, colltype ) ) ? 1 : 0;

    if ( !__get_proc_info( my, &pci ) ) return 0;

    // Get real vertices
    __calculate_shape( &pci );
    __calculate_box_limits( &pci );

    int64_t ctype = LOCQWORD( libmod_gfx, my, CTYPE );

    /* Checks only for a single instance */
    if ( id >= FIRST_INSTANCE_ID ) {
        ptr = instance_get( id );
        if ( ptr && ctype == LOCQWORD( libmod_gfx, ptr, CTYPE ) && LOCQWORD( libmod_gfx, ptr, STATUS ) & ( STATUS_RUNNING | STATUS_FROZEN ) && ptr != my ) {
            collision = 0;
            if ( __get_proc_info( ptr, &pciB ) ) {
                collision = in_radius2( pci.x, pci.y, pciB.x, pciB.y, pci.region_radius, pciB.region_radius ) && __check_collision( &pci, &pciB ) ;
                free( pciB.cboxes );
            }
        }
        free( pci.cboxes );
        return ( collision ) ? LOCQWORD( libmod_gfx, ptr, PROCESS_ID ) : 0;
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
                collision = 0;
                if ( __get_proc_info( ptr, &pciB ) ) {
                    collision = in_radius2( pci.x, pci.y, pciB.x, pciB.y, pci.region_radius, pciB.region_radius ) && __check_collision( &pci, &pciB ) ;
                    free( pciB.cboxes );
                }
            }
            if ( collision ) {
                LOCQWORD( libmod_gfx, my, COLLISION_RESERVED_ID_SCAN ) = LOCQWORD( libmod_gfx, ptr, PROCESS_ID );
                free( pci.cboxes );
                return LOCQWORD( libmod_gfx, ptr, PROCESS_ID );
            }
            ptr = ptr->next;
        }
        LOCQWORD( libmod_gfx, my, COLLISION_RESERVED_ID_SCAN ) = 0;
        free( pci.cboxes );
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
            collision = 0;
            if ( __get_proc_info( ptr, &pciB ) ) {
                collision = in_radius2( pci.x, pci.y, pciB.x, pciB.y, pci.region_radius, pciB.region_radius ) && __check_collision( &pci, &pciB ) ;
                free( pciB.cboxes );
            }
        }
        if ( collision ) {
            LOCQWORD( libmod_gfx, my, COLLISION_RESERVED_ID_SCAN ) = LOCQWORD( libmod_gfx, ptr, PROCESS_ID );
            free( pci.cboxes );
            return LOCQWORD( libmod_gfx, ptr, PROCESS_ID );
        }
    }

    LOCQWORD( libmod_gfx, my, COLLISION_RESERVED_TYPE_SCAN ) = 0;
    free( pci.cboxes );
    return 0;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_collision( INSTANCE * my, int64_t * params ) {
    return __collision( my, params[ 0 ] );
}

/* --------------------------------------------------------------------------- */
