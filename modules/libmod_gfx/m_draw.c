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

#include "xstrings.h"

#include "libbggfx.h"
#include "libmod_gfx.h"

#include "dlvaracc.h"

/* --------------------------------------------------------------------------- */

/* Primitives */

enum {
    DRAWOBJ_POINT = 1,
    DRAWOBJ_LINE,
#if ENABLE_MULTIDRAW
    DRAWOBJ_POINTS,
    DRAWOBJ_LINES,
    DRAWOBJ_RECTANGLES,
    DRAWOBJ_RECTANGLES_FILLED,
#endif
    DRAWOBJ_RECTANGLE,
    DRAWOBJ_RECTANGLE_FILLED,
    DRAWOBJ_CIRCLE,
    DRAWOBJ_CIRCLE_FILLED,
    DRAWOBJ_CURVE,
#ifdef USE_SDL2_GPU
    DRAWOBJ_ARC,
    DRAWOBJ_ARC_FILLED,
    DRAWOBJ_ELLIPSE,
    DRAWOBJ_ELLIPSE_FILLED,
    DRAWOBJ_SECTOR,
    DRAWOBJ_SECTOR_FILLED,
    DRAWOBJ_TRIANGLE,
    DRAWOBJ_TRIANGLE_FILLED,
    DRAWOBJ_RECTANGLE_ROUND,
    DRAWOBJ_RECTANGLE_ROUND_FILLED,
    DRAWOBJ_POLYGON,
    DRAWOBJ_POLYGON_FILLED,
    DRAWOBJ_POLYLINE
#endif
};

typedef struct _drawing_object {
    int64_t type;

    int64_t x1;
    int64_t y1;

    int64_t w;
    int64_t h;

    /* [x2, y2], [x3, y3] and [x4, y4] are relatives to [x1, y1] */
    int64_t x2;
    int64_t y2;
    int64_t x3;
    int64_t y3;
    int64_t x4;
    int64_t y4;

    int64_t level;

    int64_t radius;

#ifdef USE_SDL2_GPU
    int64_t rx;
    int64_t ry;

    int64_t inner_radius;
    int64_t outer_radius;

    int64_t degrees;

    int64_t start_angle;
    int64_t end_angle;

    int64_t close_loop;

    float thickness;
#endif

    int64_t z;

    int64_t preallocated:1; // if data is preallocated, user must release it

    int64_t data_size; // count items of objs
    void * data; // objs Rects/Points
                 // For DRAWOBJ_CIRCLE / DRAWOBJ_CIRCLE_FILLED / DRAWOBJ_CURVE is cache

    /* Private */

    int64_t blend_mode;
    CUSTOM_BLENDMODE custom_blendmode;

    uint8_t color_r;
    uint8_t color_g;
    uint8_t color_b;
    uint8_t color_a;

    int64_t id;

    struct _drawing_object * prev;
    struct _drawing_object * next;
} DRAWING_OBJECT;

/* --------------------------------------------------------------------------- */

static DRAWING_OBJECT * drawing_objects = NULL;

static GRAPH * drawing_graph = NULL;
static int64_t drawing_z = 0 ;

/* --------------------------------------------------------------------------- */

/*
 *  FUNCTION : _libmod_gfx_draw_object_info
 *
 *  Internal function used to return information about a primitive object
 *
 *  PARAMS :
 *      dr          Drawing object
 *      brectangle_filled        Pointer to a REGION to be filled with the bounding rectangle_filled
 *
 *  RETURN VALUE :
 *      1 if the primitive changed since last frame
 *
 */

static int _libmod_gfx_draw_object_info( void * what, REGION * brectangle_filled, int64_t * z, int64_t * drawme ) {

    REGION newclip = { 0 };
    int64_t minx, miny, maxx, maxy;

    DRAWING_OBJECT * dr = ( DRAWING_OBJECT * ) what;
    if ( !dr ) return 1;

    * drawme = 1;

    * z = dr->z;

    if ( !brectangle_filled ) return 1;

    switch ( dr->type ) {
#ifdef USE_SDL2_GPU
        case DRAWOBJ_ARC:
        case DRAWOBJ_ARC_FILLED:
            newclip.x = dr->x1 - dr->radius + 1;
            newclip.y = dr->y1 - dr->radius + 1;
            newclip.x2 = dr->x1 + dr->radius - 1;
            newclip.y2 = dr->y1 + dr->radius - 1;
            break;

        case DRAWOBJ_ELLIPSE:
        case DRAWOBJ_ELLIPSE_FILLED:
            newclip.x = dr->x1 - dr->rx;
            newclip.y = dr->y1 - dr->ry;
            newclip.x2 = dr->x1 + dr->rx;
            newclip.y2 = dr->y1 + dr->ry;
            break;

        case DRAWOBJ_SECTOR:
        case DRAWOBJ_SECTOR_FILLED:
            newclip.x = dr->x1 - dr->outer_radius + 1;
            newclip.y = dr->y1 - dr->outer_radius + 1;
            newclip.x2 = dr->x1 + dr->outer_radius - 1;
            newclip.y2 = dr->y1 + dr->outer_radius - 1;
            break;

        case DRAWOBJ_TRIANGLE:
        case DRAWOBJ_TRIANGLE_FILLED:
            newclip.x = MIN( dr->x1, MIN( dr->x1 + dr->x2, dr->x1 + dr->x3 ) );
            newclip.y = MIN( dr->y1, MIN( dr->y1 + dr->y2, dr->y1 + dr->y3 ) );
            newclip.x2 = MAX( dr->x1, MAX( dr->x1 + dr->x2, dr->x1 + dr->x3 ) );
            newclip.y2 = MAX( dr->y1, MAX( dr->y1 + dr->y2, dr->y1 + dr->y3 ) );
            break;

        case DRAWOBJ_RECTANGLE_ROUND:
        case DRAWOBJ_RECTANGLE_ROUND_FILLED:
            newclip.x = dr->x1;
            newclip.y = dr->y1;
            newclip.x2 = dr->x1 + dr->w - 1;
            newclip.y2 = dr->y1 + dr->h - 1;
            break;

        case DRAWOBJ_POLYGON:
        case DRAWOBJ_POLYGON_FILLED:
        case DRAWOBJ_POLYLINE:
            break;

#endif

        case DRAWOBJ_CIRCLE:
        case DRAWOBJ_CIRCLE_FILLED:
            newclip.x = dr->x1 - dr->radius;
            newclip.y = dr->y1 - dr->radius;
            newclip.x2 = dr->x1 + dr->radius;
            newclip.y2 = dr->y1 + dr->radius;
            break;

        case DRAWOBJ_CURVE:
            newclip.x = dr->x1;
            newclip.y = dr->y1;
            newclip.x2 = dr->x1 + dr->x4;
            newclip.y2 = dr->y1 + dr->y4;
            break;

        case DRAWOBJ_POINT:
            newclip.x = dr->x1;
            newclip.y = dr->y1;
            newclip.x2 = dr->x1;
            newclip.y2 = dr->y1;
            break;

        case DRAWOBJ_LINE:
            newclip.x = dr->x1;
            newclip.y = dr->y1;
            newclip.x2 = dr->x1 + dr->x2;
            newclip.y2 = dr->y1 + dr->y2;
            break;

#if ENABLE_MULTIDRAW
        // Maybe can be calculated
        case DRAWOBJ_POINTS:
        case DRAWOBJ_LINES:
        case DRAWOBJ_RECTANGLES:
        case DRAWOBJ_RECTANGLES_FILLED:
            newclip.x = 0;
            newclip.y = 0;
            newclip.x2 = scr_width;
            newclip.y2 = scr_height;
            break;
#endif
        case DRAWOBJ_RECTANGLE:
        case DRAWOBJ_RECTANGLE_FILLED:
        default:
            newclip.x = dr->x1;
            newclip.y = dr->y1;
            newclip.x2 = dr->x1 + dr->w - 1;
            newclip.y2 = dr->y1 + dr->h - 1;
            break;

    }

    minx = newclip.x;
    miny = newclip.y;
    maxx = newclip.x2;
    maxy = newclip.y2;

    if ( minx > maxx ) {
        minx = newclip.x2;
        maxx = newclip.x;
    }

    if ( miny > maxy ) {
        miny = newclip.y2;
        maxy = newclip.y;
    }

    newclip.x = minx;
    newclip.y = miny;
    newclip.x2 = maxx;
    newclip.y2 = maxy;

    if ( newclip.x != brectangle_filled->x || newclip.y != brectangle_filled->y || newclip.x2 != brectangle_filled->x2 || newclip.y2 != brectangle_filled->y2 ) {
        * brectangle_filled = newclip;
        return 0;
    }

    return 1;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : _libmod_gfx_draw_object_draw
 *
 *  Internal function used to draw a primitive object
 *
 *  PARAMS :
 *      dr          Drawing object
 *
 *  RETURN VALUE :
 *      None
 *
 */

static void _libmod_gfx_draw_object_draw( void * what, REGION * clip ) {
    DRAWING_OBJECT * dr = ( DRAWING_OBJECT * ) what;
    if ( !dr ) return;

    uint8_t old_drawing_color_r = drawing_color_r;
    uint8_t old_drawing_color_g = drawing_color_g;
    uint8_t old_drawing_color_b = drawing_color_b;
    uint8_t old_drawing_color_a = drawing_color_a;

    int64_t old_drawing_blend_mode = drawing_blend_mode;
    CUSTOM_BLENDMODE old_drawing_custom_blendmode = drawing_custom_blendmode;

#ifdef USE_SDL2_GPU
    float old_drawing_thickness = drawing_thickness;
#endif

    drawing_color_r = dr->color_r;
    drawing_color_g = dr->color_g;
    drawing_color_b = dr->color_b;
    drawing_color_a = dr->color_a;
    drawing_blend_mode = dr->blend_mode;
    drawing_custom_blendmode = dr->custom_blendmode;

#ifdef USE_SDL2_GPU
    drawing_thickness = dr->thickness;
#endif

    switch ( dr->type ) {
        case DRAWOBJ_POINT:
            draw_point( NULL, clip, dr->x1, dr->y1 );
            break;

        case DRAWOBJ_LINE:
            draw_line( NULL, clip, dr->x1, dr->y1, dr->x1 + dr->x2, dr->y1 + dr->y2 );
            break;

        case DRAWOBJ_RECTANGLE:
            draw_rectangle( NULL, clip, dr->x1, dr->y1, dr->w, dr->h );
            break;

        case DRAWOBJ_RECTANGLE_FILLED:
            draw_rectangle_filled( NULL, clip, dr->x1, dr->y1, dr->w, dr->h );
            break;

#if ENABLE_MULTIDRAW
        case DRAWOBJ_POINTS:
            draw_points( NULL, clip, dr->data_size, ( SDL_Point * ) dr->data );
            break;

        case DRAWOBJ_LINES:
            draw_lines( NULL, clip, dr->data_size, ( SDL_Point * ) dr->data );
            break;

        case DRAWOBJ_RECTANGLES:
            draw_rectangles( NULL, clip, dr->data_size, ( SDL_Rect * ) dr->data );
            break;

        case DRAWOBJ_RECTANGLES_FILLED:
            draw_rectangles_filled( NULL, clip, dr->data_size, ( SDL_Rect * ) dr->data );
            break;
#endif
        case DRAWOBJ_CIRCLE:
            draw_circle( NULL, clip, dr->x1, dr->y1, dr->radius, &dr->data_size, &dr->data );
            break;

        case DRAWOBJ_CIRCLE_FILLED:
            draw_circle_filled( NULL, clip, dr->x1, dr->y1, dr->radius, &dr->data_size, &dr->data );
            break;

        case DRAWOBJ_CURVE:
            draw_bezier( NULL, clip, dr->x1, dr->y1, dr->x1 + dr->x2, dr->y1 + dr->y2, dr->x1 + dr->x3, dr->y1 + dr->y3, dr->x1 + dr->x4, dr->y1 + dr->y4, dr->level, &dr->data_size, &dr->data );
            break;

#ifdef USE_SDL2_GPU
        case DRAWOBJ_ARC:
            draw_arc( NULL, clip, dr->x1, dr->y1, dr->radius, dr->start_angle, dr->end_angle ) ;
            break;

        case DRAWOBJ_ARC_FILLED:
            draw_arc_filled( NULL, clip, dr->x1, dr->y1, dr->radius, dr->start_angle, dr->end_angle ) ;
            break;

        case DRAWOBJ_ELLIPSE:
            draw_ellipse( NULL, clip, dr->x1, dr->y1, dr->rx, dr->ry, dr->degrees );
            break;

        case DRAWOBJ_ELLIPSE_FILLED:
            draw_ellipse_filled( NULL, clip, dr->x1, dr->y1, dr->rx, dr->ry, dr->degrees );
            break;

        case DRAWOBJ_SECTOR:
            draw_sector( NULL, clip, dr->x1, dr->y1, dr->inner_radius, dr->outer_radius, dr->start_angle, dr->end_angle ) ;
            break;

        case DRAWOBJ_SECTOR_FILLED:
            draw_sector_filled( NULL, clip, dr->x1, dr->y1, dr->inner_radius, dr->outer_radius, dr->start_angle, dr->end_angle ) ;
            break;

        case DRAWOBJ_TRIANGLE:
            draw_triangle( NULL, clip, dr->x1, dr->y1, dr->x1 + dr->x2, dr->y1 + dr->y2, dr->x1 + dr->x3, dr->y1 + dr->y3 ) ;
            break;

        case DRAWOBJ_TRIANGLE_FILLED:
            draw_triangle_filled( NULL, clip, dr->x1, dr->y1, dr->x1 + dr->x2, dr->y1 + dr->y2, dr->x1 + dr->x3, dr->y1 + dr->y3 ) ;
            break;

        case DRAWOBJ_RECTANGLE_ROUND:
            draw_rectangle_round( NULL, clip, dr->x1, dr->y1, dr->w, dr->h, dr->radius ) ;
            break;

        case DRAWOBJ_RECTANGLE_ROUND_FILLED:
            draw_rectangle_round_filled( NULL, clip, dr->x1, dr->y1, dr->w, dr->h, dr->radius ) ;
            break;

        case DRAWOBJ_POLYGON:
            draw_polygon( NULL, clip, dr->data_size, ( float * ) dr->data ) ;
            break;

        case DRAWOBJ_POLYGON_FILLED:
            draw_polygon_filled( NULL, clip, dr->data_size, ( float * ) dr->data ) ;
            break;

        case DRAWOBJ_POLYLINE:
            draw_polyline( NULL, clip, dr->data_size, ( float * ) dr->data, dr->close_loop ) ;
            break;

#endif
    }

#ifdef USE_SDL2_GPU
    drawing_thickness = old_drawing_thickness;
#endif

    drawing_custom_blendmode = old_drawing_custom_blendmode;

    drawing_color_r = old_drawing_color_r;
    drawing_color_g = old_drawing_color_g;
    drawing_color_b = old_drawing_color_b;
    drawing_color_a = old_drawing_color_a;

    drawing_blend_mode = old_drawing_blend_mode;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : _libmod_gfx_draw_object_new
 *
 *  Create a new on-screen drawing object
 *
 *  PARAMS :
 *      drawing         Object type and coordinates
 *      z               Z Coordinate
 *
 *  RETURN VALUE :
 *      An integer identifier, representing the new object, or -1 if error
 *
 */

static int64_t _libmod_gfx_draw_object_new( DRAWING_OBJECT * dr, int64_t z ) {

    if ( !dr ) return -1;

    /* Fill the struct and register the new object */

    if ( drawing_objects ) drawing_objects->prev = dr ;

    dr->prev = NULL;
    dr->next = drawing_objects;

    dr->color_r = drawing_color_r;
    dr->color_g = drawing_color_g;
    dr->color_b = drawing_color_b;
    dr->color_a = drawing_color_a;
    dr->blend_mode = drawing_blend_mode;
    dr->z = drawing_z;

#ifdef USE_SDL2_GPU
    dr->thickness = drawing_thickness;
#endif

    dr->preallocated = 0;

    switch ( dr->type ) {
#if ENABLE_MULTIDRAW
        case DRAWOBJ_POINTS:
        case DRAWOBJ_LINES:
        case DRAWOBJ_RECTANGLES:
        case DRAWOBJ_RECTANGLES_FILLED:
#endif
#ifdef USE_SDL2_GPU
        case DRAWOBJ_POLYGON:
        case DRAWOBJ_POLYGON_FILLED:
        case DRAWOBJ_POLYLINE:
            dr->preallocated = 1;
#endif
        case DRAWOBJ_CIRCLE:
        case DRAWOBJ_CIRCLE_FILLED:
        case DRAWOBJ_CURVE:
            break;

        default:
            dr->data_size = 0;
            dr->data = NULL;
            break;

    }

    dr->id = gr_new_object( z, ( OBJ_INFO * ) _libmod_gfx_draw_object_info, ( OBJ_DRAW * ) _libmod_gfx_draw_object_draw, ( void * ) dr );

    drawing_objects = dr;

    return ( int64_t ) ( intptr_t ) dr;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : _libmod_gfx_draw_object_destroy
 *
 *  Destroy a new on-screen drawing object
 *
 *  PARAMS :
 *      id              Object id returned by _libmod_gfx_draw_object_new
 *                      or 0 to destroy every object
 *
 *  RETURN VALUE :
 *      None
 *
 */

static void _libmod_gfx_draw_object_destroy( int64_t id )
{
    DRAWING_OBJECT * dr = ( DRAWING_OBJECT * ) ( intptr_t ) id, * next;
    int64_t destroyall = 0;

    if ( !dr ) {
        dr = drawing_objects;
        destroyall = 1;
    }

    while ( dr ) {
        next = dr->next;

        if ( dr->next ) dr->next->prev = dr->prev;
        if ( dr->prev ) dr->prev->next = dr->next;

        if ( !dr->preallocated ) {
            free( dr->data );
            dr->data = NULL;
        }

        gr_destroy_object( dr->id );

        if ( drawing_objects == dr ) drawing_objects = dr->next;

        free( dr );

        if ( !destroyall ) break;

        dr = next;
    }
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : _libmod_gfx_draw_object_move
 *
 *  Move an on-screen drawing object to a new coordinates,
 *  relative to the first point in the primitive
 *
 *  PARAMS :
 *      id              Object id returned by _libmod_gfx_draw_object_new
 *      x, y            New coordinates
 *
 *  RETURN VALUE :
 *      None
 *
 */

static void _libmod_gfx_draw_object_move( int64_t id, int64_t x, int64_t y ) {
    DRAWING_OBJECT * dr = ( DRAWING_OBJECT * ) ( intptr_t ) id;

    if ( dr ) {
        int64_t incx = x - dr->x1;
        int64_t incy = y - dr->y1;
        int64_t i;

        dr->x1 += incx;
        dr->y1 += incy;

        switch ( dr->type ) {
            case DRAWOBJ_POINT:
                break;

            case DRAWOBJ_LINE:
                break;

            case DRAWOBJ_RECTANGLE:
            case DRAWOBJ_RECTANGLE_FILLED:
                break;

#if ENABLE_MULTIDRAW
            case DRAWOBJ_POINTS:
            case DRAWOBJ_LINES:
#endif
#ifdef USE_SDL2
            case DRAWOBJ_CIRCLE:
            case DRAWOBJ_CIRCLE_FILLED:
#endif
            case DRAWOBJ_CURVE:
                if ( dr->data ) {
                    SDL_Point * p = ( SDL_Point * ) dr->data;
                    for ( i = 0; i < dr->data_size; i++, p++ ) {
                        p->x += incx, p->y += incy;
                    }
                }
                break;

#ifdef USE_SDL2_GPU
            case DRAWOBJ_CIRCLE:
            case DRAWOBJ_CIRCLE_FILLED:
                break;
#endif

#if ENABLE_MULTIDRAW
            case DRAWOBJ_RECTANGLES:
            case DRAWOBJ_RECTANGLES_FILLED:
                if ( dr->data ) {
                    SDL_Rect * p = ( SDL_Rect * ) dr->data;
                    for ( i = 0; i < dr->data_size; i++, p++ ) {
                        p->x += incx, p->y += incy;
                    }
                }
                break;
#endif

#ifdef USE_SDL2_GPU
        case DRAWOBJ_ARC:
        case DRAWOBJ_ARC_FILLED:
        case DRAWOBJ_ELLIPSE:
        case DRAWOBJ_ELLIPSE_FILLED:
        case DRAWOBJ_SECTOR:
        case DRAWOBJ_SECTOR_FILLED:
        case DRAWOBJ_RECTANGLE_ROUND:
        case DRAWOBJ_RECTANGLE_ROUND_FILLED:
            break;

        case DRAWOBJ_TRIANGLE:
        case DRAWOBJ_TRIANGLE_FILLED:
            break;

        case DRAWOBJ_POLYGON:
        case DRAWOBJ_POLYGON_FILLED:
        case DRAWOBJ_POLYLINE:
            if ( dr->data ) {
                float * p = ( float * ) dr->data;
                for ( i = 0; i < dr->data_size; i++, p++ ) {
                    *p++ += ( float ) incx;
                    *p += ( float ) incy;
                }
            }
            break;

#endif
        }
    }
}

/* --------------------------------------------------------------------------- */

static void __get_rgba( int64_t color, uint8_t * r, uint8_t * g, uint8_t * b, uint8_t * a ) {
    SDL_GetRGBA( color, gPixelFormat, r, g, b, a ) ;
}

/* --------------------------------------------------------------------------- */
/* Exportable functions                                                        */
/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_drawing_color( INSTANCE * my, int64_t * params ) {
    __get_rgba( params[0], &drawing_color_r, &drawing_color_g, &drawing_color_b, &drawing_color_a ) ;
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_drawing_rgba( INSTANCE * my, int64_t * params ) {
    drawing_color_r = params[0];
    drawing_color_g = params[1];
    drawing_color_b = params[2];
    drawing_color_a = params[3];
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_drawing_color2( INSTANCE * my, int64_t * params ) {
    DRAWING_OBJECT * dr = ( DRAWING_OBJECT * ) ( intptr_t ) params[0];
    if ( !dr ) return -1;
    __get_rgba( params[1], &dr->color_r, &dr->color_g, &dr->color_b, &dr->color_a ) ;
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_drawing_rgba2( INSTANCE * my, int64_t * params ) {
    DRAWING_OBJECT * dr = ( DRAWING_OBJECT * ) ( intptr_t ) params[0];
    if ( !dr ) return -1;
    dr->color_r = params[1];
    dr->color_g = params[2];
    dr->color_b = params[3];
    dr->color_a = params[4];
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_drawing_blend_mode( INSTANCE * my, int64_t * params ) {
    drawing_blend_mode = params[0];
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_drawing_blend_mode2( INSTANCE * my, int64_t * params ) {
    DRAWING_OBJECT * dr = ( DRAWING_OBJECT * ) ( intptr_t ) params[0];
    if ( !dr ) return -1;
    dr->blend_mode = params[1];
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_drawing_blend_mode3( INSTANCE * my, int64_t * params ) {
    drawing_blend_mode = BLEND_CUSTOM;
    drawing_custom_blendmode.src_rgb    = params[0];
    drawing_custom_blendmode.dst_rgb    = params[1];
    drawing_custom_blendmode.src_alpha  = params[2];
    drawing_custom_blendmode.dst_alpha  = params[3];
    drawing_custom_blendmode.eq_rgb     = params[4];
    drawing_custom_blendmode.eq_alpha   = params[5];
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_drawing_blend_mode4( INSTANCE * my, int64_t * params ) {
    DRAWING_OBJECT * dr = ( DRAWING_OBJECT * ) ( intptr_t ) params[0];
    if ( !dr ) return -1;
    drawing_blend_mode = BLEND_CUSTOM;
    drawing_custom_blendmode.src_rgb    = params[1];
    drawing_custom_blendmode.dst_rgb    = params[2];
    drawing_custom_blendmode.src_alpha  = params[3];
    drawing_custom_blendmode.dst_alpha  = params[4];
    drawing_custom_blendmode.eq_rgb     = params[5];
    drawing_custom_blendmode.eq_alpha   = params[6];
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_drawing_z_get( INSTANCE * my, int64_t * params ) {
    return drawing_z;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_drawing_z( INSTANCE * my, int64_t * params ) {
    drawing_z = params[ 0 ];
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_drawing_z2( INSTANCE * my, int64_t * params ) {
    DRAWING_OBJECT * dr = ( DRAWING_OBJECT * ) ( intptr_t ) params[0];
    if ( !dr ) return -1;
    dr->z = params[1];
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_drawing_map( INSTANCE * my, int64_t * params ) {
    drawing_graph = bitmap_get( params[ 0 ], params[ 1 ] ) ;
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_delete_drawing( INSTANCE * my, int64_t * params ) {
    _libmod_gfx_draw_object_destroy( params[ 0 ] );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_move_drawing( INSTANCE * my, int64_t * params ) {
    _libmod_gfx_draw_object_move( params[ 0 ], params[ 1 ], params[ 2 ] );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_getcoords( INSTANCE * my, int64_t * params ) {
    DRAWING_OBJECT * dr = ( DRAWING_OBJECT * ) ( intptr_t ) params[0];
    if ( !dr ) return -1;
    if ( params[1] ) *( int64_t * )( intptr_t )(params[1]) = ( uint8_t ) dr->x1;
    if ( params[2] ) *( int64_t * )( intptr_t )(params[2]) = ( uint8_t ) dr->y1;
    if ( params[3] ) *( int64_t * )( intptr_t )(params[3]) = ( uint8_t ) dr->z;
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_point( INSTANCE * my, int64_t * params ) {
    if ( !drawing_graph ) {
        DRAWING_OBJECT * dr = malloc( sizeof( DRAWING_OBJECT ) );
        if ( !dr ) return -1;

        dr->type = DRAWOBJ_POINT;
        dr->x1 = params[ 0 ];
        dr->y1 = params[ 1 ];
        return _libmod_gfx_draw_object_new( dr, drawing_z );
    }

    draw_point( drawing_graph, 0, params[ 0 ], params[ 1 ] );
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_line( INSTANCE * my, int64_t * params ) {
    if ( !drawing_graph ) {
        DRAWING_OBJECT * dr = malloc( sizeof( DRAWING_OBJECT ) );
        if ( !dr ) return -1;

        dr->type = DRAWOBJ_LINE;
        dr->x1 = params[ 0 ];
        dr->y1 = params[ 1 ];
        dr->x2 = params[ 2 ] - dr->x1 + 1;
        dr->y2 = params[ 3 ] - dr->y1 + 1;
        return _libmod_gfx_draw_object_new( dr, drawing_z );
    }

    draw_line( drawing_graph, 0, params[ 0 ], params[ 1 ], params[ 2 ], params[ 3 ] );
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_rectangle_filled( INSTANCE * my, int64_t * params ) {
    if ( !drawing_graph ) {
        DRAWING_OBJECT * dr = malloc( sizeof( DRAWING_OBJECT ) );
        if ( !dr ) return -1;

        dr->type = DRAWOBJ_RECTANGLE_FILLED;
        dr->x1 = params[ 0 ];
        dr->y1 = params[ 1 ];
        dr->w = params[ 2 ];
        dr->h = params[ 3 ];
        return _libmod_gfx_draw_object_new( dr, drawing_z );
    }

    draw_rectangle_filled( drawing_graph, 0, params[ 0 ], params[ 1 ], params[ 2 ], params[ 3 ] ) ;
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_rectangle( INSTANCE * my, int64_t * params ) {
    if ( !drawing_graph ) {
        DRAWING_OBJECT * dr = malloc( sizeof( DRAWING_OBJECT ) );
        if ( !dr ) return -1;

        dr->type = DRAWOBJ_RECTANGLE;
        dr->x1 = params[ 0 ];
        dr->y1 = params[ 1 ];
        dr->w = params[ 2 ];
        dr->h = params[ 3 ];
        return _libmod_gfx_draw_object_new( dr, drawing_z );
    }

    draw_rectangle( drawing_graph, 0, params[ 0 ], params[ 1 ], params[ 2 ], params[ 3 ] ) ;
    return 1 ;
}

#if ENABLE_MULTIDRAW
/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_points( INSTANCE * my, int64_t * params ) {
    if ( !drawing_graph ) {
        DRAWING_OBJECT * dr = malloc( sizeof( DRAWING_OBJECT ) );
        if ( !dr ) return -1;

        dr->type = DRAWOBJ_POINTS;
        dr->data_size = params[ 0 ];
        dr->data = ( void * ) ( intptr_t ) params[ 1 ];
        return _libmod_gfx_draw_object_new( dr, drawing_z );
    }

    draw_points( drawing_graph, 0, params[ 0 ], ( void * ) ( intptr_t ) params[ 1 ] );
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_lines( INSTANCE * my, int64_t * params ) {
    if ( !drawing_graph ) {
        DRAWING_OBJECT * dr = malloc( sizeof( DRAWING_OBJECT ) );
        if ( !dr ) return -1;

        dr->type = DRAWOBJ_LINES;
        dr->data_size = params[ 0 ];
        dr->data = ( void * ) ( intptr_t ) params[ 1 ];
        return _libmod_gfx_draw_object_new( dr, drawing_z );
    }

    draw_lines( drawing_graph, 0, params[ 0 ], ( void * ) ( intptr_t ) params[ 1 ] );
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_rectangles( INSTANCE * my, int64_t * params ) {
    if ( !drawing_graph ) {
        DRAWING_OBJECT * dr = malloc( sizeof( DRAWING_OBJECT ) );
        if ( !dr ) return -1;

        dr->type = DRAWOBJ_RECTANGLES;
        dr->data_size = params[ 0 ];
        dr->data = ( void * ) ( intptr_t ) params[ 1 ];
        return _libmod_gfx_draw_object_new( dr, drawing_z );
    }

    draw_rectangles( drawing_graph, 0, params[ 0 ], ( void * ) ( intptr_t ) params[ 1 ] ) ;
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_rectangles_filled( INSTANCE * my, int64_t * params ) {
    if ( !drawing_graph ) {
        DRAWING_OBJECT * dr = malloc( sizeof( DRAWING_OBJECT ) );
        if ( !dr ) return -1;

        dr->type = DRAWOBJ_RECTANGLES_FILLED;
        dr->data_size = params[ 0 ];
        dr->data = ( void * ) ( intptr_t ) params[ 1 ];
        return _libmod_gfx_draw_object_new( dr, drawing_z );
    }

    draw_rectangles_filled( drawing_graph, 0, params[ 0 ], ( void * ) ( intptr_t ) params[ 1 ] ) ;
    return 1 ;
}
#endif

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_circle( INSTANCE * my, int64_t * params ) {
    if ( !drawing_graph ) {
        DRAWING_OBJECT * dr = malloc( sizeof( DRAWING_OBJECT ) );
        if ( !dr ) return -1;

        dr->type = DRAWOBJ_CIRCLE;
        dr->x1 = params[ 0 ];
        dr->y1 = params[ 1 ];
        dr->radius = params[ 2 ];

        dr->data_size = 0;
        dr->data = NULL;
        return _libmod_gfx_draw_object_new( dr, drawing_z );
    }

    draw_circle( drawing_graph, 0, params[ 0 ], params[ 1 ], params[ 2 ], NULL, NULL ) ;
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_circle_filled( INSTANCE * my, int64_t * params ) {
    if ( !drawing_graph ) {
        DRAWING_OBJECT * dr = malloc( sizeof( DRAWING_OBJECT ) );
        if ( !dr ) return -1;

        dr->type = DRAWOBJ_CIRCLE_FILLED;
        dr->x1 = params[ 0 ];
        dr->y1 = params[ 1 ];
        dr->radius = params[ 2 ];

        dr->data_size = 0;
        dr->data = NULL;
        return _libmod_gfx_draw_object_new( dr, drawing_z );
    }

    draw_circle_filled( drawing_graph, 0, params[ 0 ], params[ 1 ], params[ 2 ], NULL, NULL ) ;
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_bezier( INSTANCE * my, int64_t * params ) {
    if ( !drawing_graph ) {
        DRAWING_OBJECT * dr = malloc( sizeof( DRAWING_OBJECT ) );
        if ( !dr ) return -1;

        dr->type = DRAWOBJ_CURVE;
        dr->x1 = params[ 0 ];
        dr->y1 = params[ 1 ];
        dr->x2 = params[ 2 ] - dr->x1 + 1;
        dr->y2 = params[ 3 ] - dr->y1 + 1;
        dr->x3 = params[ 4 ] - dr->x1 + 1;
        dr->y3 = params[ 5 ] - dr->y1 + 1;
        dr->x4 = params[ 6 ] - dr->x1 + 1;
        dr->y4 = params[ 7 ] - dr->y1 + 1;
        dr->level = params[ 8 ];

        dr->data_size = 0;
        dr->data = NULL;
        return _libmod_gfx_draw_object_new( dr, drawing_z );
    }

    draw_bezier( drawing_graph, 0, params[ 0 ], params[ 1 ], params[ 2 ], params[ 3 ], params[ 4 ], params[ 5 ], params[ 6 ], params[ 7 ], params[ 8 ], NULL, NULL );
    return 1;
}

/* --------------------------------------------------------------------------- */

#ifdef USE_SDL2_GPU

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_arc( INSTANCE * my, int64_t * params ) {
    if ( !drawing_graph ) {
        DRAWING_OBJECT * dr = malloc( sizeof( DRAWING_OBJECT ) );
        if ( !dr ) return -1;

        dr->type = DRAWOBJ_ARC;
        dr->x1 = params[ 0 ];
        dr->y1 = params[ 1 ];
        dr->radius = params[ 2 ];
        dr->start_angle = params[ 3 ];
        dr->end_angle = params[ 4 ];

        dr->data_size = 0;
        dr->data = NULL;
        return _libmod_gfx_draw_object_new( dr, drawing_z );
    }

    draw_arc( drawing_graph, 0, params[ 0 ], params[ 1 ], params[ 2 ], params[ 3 ], params[ 4 ] ) ;
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_arc_filled( INSTANCE * my, int64_t * params ) {
    if ( !drawing_graph ) {
        DRAWING_OBJECT * dr = malloc( sizeof( DRAWING_OBJECT ) );
        if ( !dr ) return -1;

        dr->type = DRAWOBJ_ARC_FILLED;
        dr->x1 = params[ 0 ];
        dr->y1 = params[ 1 ];
        dr->radius = params[ 2 ];
        dr->start_angle = params[ 3 ];
        dr->end_angle = params[ 4 ];

        dr->data_size = 0;
        dr->data = NULL;
        return _libmod_gfx_draw_object_new( dr, drawing_z );
    }

    draw_arc_filled( drawing_graph, 0, params[ 0 ], params[ 1 ], params[ 2 ], params[ 3 ], params[ 4 ] ) ;
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_ellipse( INSTANCE * my, int64_t * params ) {
    if ( !drawing_graph ) {
        DRAWING_OBJECT * dr = malloc( sizeof( DRAWING_OBJECT ) );
        if ( !dr ) return -1;

        dr->type = DRAWOBJ_ELLIPSE;
        dr->x1 = params[ 0 ];
        dr->y1 = params[ 1 ];
        dr->rx = params[ 2 ];
        dr->ry = params[ 3 ];
        dr->degrees = params[ 4 ];

        dr->data_size = 0;
        dr->data = NULL;
        return _libmod_gfx_draw_object_new( dr, drawing_z );
    }

    draw_ellipse( drawing_graph, 0, params[ 0 ], params[ 1 ], params[ 2 ], params[ 3 ], params[ 4 ] ) ;
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_ellipse_filled( INSTANCE * my, int64_t * params ) {
    if ( !drawing_graph ) {
        DRAWING_OBJECT * dr = malloc( sizeof( DRAWING_OBJECT ) );
        if ( !dr ) return -1;

        dr->type = DRAWOBJ_ELLIPSE_FILLED;
        dr->x1 = params[ 0 ];
        dr->y1 = params[ 1 ];
        dr->rx = params[ 2 ];
        dr->ry = params[ 3 ];
        dr->degrees = params[ 4 ];

        dr->data_size = 0;
        dr->data = NULL;
        return _libmod_gfx_draw_object_new( dr, drawing_z );
    }

    draw_ellipse_filled( drawing_graph, 0, params[ 0 ], params[ 1 ], params[ 2 ], params[ 3 ], params[ 4 ] ) ;
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_sector( INSTANCE * my, int64_t * params ) {
    if ( !drawing_graph ) {
        DRAWING_OBJECT * dr = malloc( sizeof( DRAWING_OBJECT ) );
        if ( !dr ) return -1;

        dr->type = DRAWOBJ_SECTOR;
        dr->x1 = params[ 0 ];
        dr->y1 = params[ 1 ];
        dr->inner_radius = params[ 2 ];
        dr->outer_radius = params[ 3 ];
        dr->start_angle = params[ 4 ];
        dr->end_angle = params[ 5 ];

        dr->data_size = 0;
        dr->data = NULL;
        return _libmod_gfx_draw_object_new( dr, drawing_z );
    }

    draw_sector( drawing_graph, 0, params[ 0 ], params[ 1 ], params[ 2 ], params[ 3 ], params[ 4 ], params[ 5 ] ) ;
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_sector_filled( INSTANCE * my, int64_t * params ) {
    if ( !drawing_graph ) {
        DRAWING_OBJECT * dr = malloc( sizeof( DRAWING_OBJECT ) );
        if ( !dr ) return -1;

        dr->type = DRAWOBJ_SECTOR_FILLED;
        dr->x1 = params[ 0 ];
        dr->y1 = params[ 1 ];
        dr->inner_radius = params[ 2 ];
        dr->outer_radius = params[ 3 ];
        dr->start_angle = params[ 4 ];
        dr->end_angle = params[ 5 ];

        dr->data_size = 0;
        dr->data = NULL;
        return _libmod_gfx_draw_object_new( dr, drawing_z );
    }

    draw_sector_filled( drawing_graph, 0, params[ 0 ], params[ 1 ], params[ 2 ], params[ 3 ], params[ 4 ], params[ 5 ] ) ;
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_triangle( INSTANCE * my, int64_t * params ) {
    if ( !drawing_graph ) {
        DRAWING_OBJECT * dr = malloc( sizeof( DRAWING_OBJECT ) );
        if ( !dr ) return -1;

        dr->type = DRAWOBJ_TRIANGLE;
        dr->x1 = params[ 0 ];
        dr->y1 = params[ 1 ];
        dr->x2 = params[ 2 ] - dr->x1 + 1;
        dr->y2 = params[ 3 ] - dr->y1 + 1;
        dr->x3 = params[ 4 ] - dr->x1 + 1;
        dr->y3 = params[ 5 ] - dr->y1 + 1;

        dr->data_size = 0;
        dr->data = NULL;
        return _libmod_gfx_draw_object_new( dr, drawing_z );
    }

    draw_triangle( drawing_graph, 0, params[ 0 ], params[ 1 ], params[ 2 ], params[ 3 ], params[ 4 ], params[ 5 ] ) ;
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_triangle_filled( INSTANCE * my, int64_t * params ) {
    if ( !drawing_graph ) {
        DRAWING_OBJECT * dr = malloc( sizeof( DRAWING_OBJECT ) );
        if ( !dr ) return -1;

        dr->type = DRAWOBJ_TRIANGLE_FILLED;
        dr->x1 = params[ 0 ];
        dr->y1 = params[ 1 ];
        dr->x2 = params[ 2 ] - dr->x1 + 1;
        dr->y2 = params[ 3 ] - dr->y1 + 1;
        dr->x3 = params[ 4 ] - dr->x1 + 1;
        dr->y3 = params[ 5 ] - dr->y1 + 1;

        dr->data_size = 0;
        dr->data = NULL;
        return _libmod_gfx_draw_object_new( dr, drawing_z );
    }

    draw_triangle_filled( drawing_graph, 0, params[ 0 ], params[ 1 ], params[ 2 ], params[ 3 ], params[ 4 ], params[ 5 ] ) ;
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_rectangle_round( INSTANCE * my, int64_t * params ) {
    if ( !drawing_graph ) {
        DRAWING_OBJECT * dr = malloc( sizeof( DRAWING_OBJECT ) );
        if ( !dr ) return -1;

        dr->type = DRAWOBJ_RECTANGLE_ROUND;
        dr->x1 = params[ 0 ];
        dr->y1 = params[ 1 ];
        dr->w = params[ 2 ];
        dr->h = params[ 3 ];
        dr->radius = params[ 4 ];

        dr->data_size = 0;
        dr->data = NULL;
        return _libmod_gfx_draw_object_new( dr, drawing_z );
    }

    draw_rectangle_round( drawing_graph, 0, params[ 0 ], params[ 1 ], params[ 2 ], params[ 3 ], params[ 4 ] ) ;
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_rectangle_round_filled( INSTANCE * my, int64_t * params ) {
    if ( !drawing_graph ) {
        DRAWING_OBJECT * dr = malloc( sizeof( DRAWING_OBJECT ) );
        if ( !dr ) return -1;

        dr->type = DRAWOBJ_RECTANGLE_ROUND_FILLED;
        dr->x1 = params[ 0 ];
        dr->y1 = params[ 1 ];
        dr->w = params[ 2 ];
        dr->h = params[ 3 ];
        dr->radius = params[ 4 ];

        dr->data_size = 0;
        dr->data = NULL;
        return _libmod_gfx_draw_object_new( dr, drawing_z );
    }

    draw_rectangle_round_filled( drawing_graph, 0, params[ 0 ], params[ 1 ], params[ 2 ], params[ 3 ], params[ 4 ] ) ;
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_polygon( INSTANCE * my, int64_t * params ) {
    if ( !drawing_graph ) {
        DRAWING_OBJECT * dr = malloc( sizeof( DRAWING_OBJECT ) );
        if ( !dr ) return -1;

        dr->type = DRAWOBJ_POLYGON;
        dr->data_size = params[ 0 ];
        dr->data = ( void * ) ( intptr_t ) params[ 1 ];
        if ( dr->data_size > 0 ) {
            float * data = ( void * ) ( intptr_t ) params[ 1 ];
            dr->x1 = data[0];
            dr->y1 = data[1];
        }
        return _libmod_gfx_draw_object_new( dr, drawing_z );
    }

    draw_polygon( drawing_graph, 0, params[ 0 ], ( void * ) ( intptr_t ) params[ 1 ] ) ;
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_polygon_filled( INSTANCE * my, int64_t * params ) {
    if ( !drawing_graph ) {
        DRAWING_OBJECT * dr = malloc( sizeof( DRAWING_OBJECT ) );
        if ( !dr ) return -1;

        dr->type = DRAWOBJ_POLYGON_FILLED;
        dr->data_size = params[ 0 ];
        dr->data = ( void * ) ( intptr_t ) params[ 1 ];
        if ( dr->data_size > 0 ) {
            float * data = ( void * ) ( intptr_t ) params[ 1 ];
            dr->x1 = data[0];
            dr->y1 = data[1];
        }
        return _libmod_gfx_draw_object_new( dr, drawing_z );
    }

    draw_polygon_filled( drawing_graph, 0, params[ 0 ], ( void * ) ( intptr_t ) params[ 1 ] ) ;
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_polyline( INSTANCE * my, int64_t * params ) {
    if ( !drawing_graph ) {
        DRAWING_OBJECT * dr = malloc( sizeof( DRAWING_OBJECT ) );
        if ( !dr ) return -1;

        dr->type = DRAWOBJ_POLYLINE;
        dr->data_size = params[ 0 ];
        dr->data = ( void * ) ( intptr_t ) params[ 1 ];
        dr->close_loop = params[ 2 ];
        if ( dr->data_size > 0 ) {
            float * data = ( void * ) ( intptr_t ) params[ 1 ];
            dr->x1 = data[0];
            dr->y1 = data[1];
        }
        return _libmod_gfx_draw_object_new( dr, drawing_z );
    }

    draw_polyline( drawing_graph, 0, params[ 0 ], ( void * ) ( intptr_t ) params[ 1 ], params[ 2 ] ) ;
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_set_thickness( INSTANCE * my, int64_t * params ) {
    float old = drawing_thickness;
    drawing_thickness = *( float * ) ( intptr_t ) &params[0];
    return *( int64_t * ) ( intptr_t ) &old ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_set_thickness2( INSTANCE * my, int64_t * params ) {
    float old = 1.0f;
    DRAWING_OBJECT * dr = ( DRAWING_OBJECT * ) ( intptr_t ) params[0];
    if ( dr ) {
        old = dr->thickness;
        dr->thickness = *( float * ) ( intptr_t ) &params[1];
    }
    return *( int64_t * ) ( intptr_t ) &old ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_get_thickness( INSTANCE * my, int64_t * params ) {
    return *( int64_t * ) ( intptr_t ) &drawing_thickness ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_get_thickness2( INSTANCE * my, int64_t * params ) {
    float val = 1.0f;
    DRAWING_OBJECT * dr = ( DRAWING_OBJECT * ) ( intptr_t ) params[0];
    if ( dr ) val = dr->thickness;
    return *( int64_t * ) ( intptr_t ) &val ;
}

/* --------------------------------------------------------------------------- */

#endif

/* --------------------------------------------------------------------------- */

#undef DRWFN_COLOR
#define DRWFN_COLOR(fn,params_color) \
int64_t libmod_gfx_draw_##fn##_color( INSTANCE * my, int64_t * params ) { \
     \
    uint8_t old_drawing_color_r = drawing_color_r; \
    uint8_t old_drawing_color_g = drawing_color_g; \
    uint8_t old_drawing_color_b = drawing_color_b; \
    uint8_t old_drawing_color_a = drawing_color_a; \
 \
    __get_rgba( params[params_color], &drawing_color_r, &drawing_color_g, &drawing_color_b, &drawing_color_a ); \
 \
    int64_t result = libmod_gfx_draw_##fn( my, params ); \
 \
    drawing_color_r = old_drawing_color_r; \
    drawing_color_g = old_drawing_color_g; \
    drawing_color_b = old_drawing_color_b; \
    drawing_color_a = old_drawing_color_a; \
 \
    return result ; \
}

/* ----------------------------------------------------------------- */

DRWFN_COLOR(point,2)
DRWFN_COLOR(line,4)
DRWFN_COLOR(rectangle,4)
DRWFN_COLOR(rectangle_filled,4)
#if ENABLE_MULTIDRAW
DRWFN_COLOR(points,2)
DRWFN_COLOR(lines,2)
DRWFN_COLOR(rectangles,2)
DRWFN_COLOR(rectangles_filled,2)
#endif
DRWFN_COLOR(circle,3)
DRWFN_COLOR(circle_filled,3)
DRWFN_COLOR(bezier,9)

#ifdef USE_SDL2_GPU
DRWFN_COLOR(arc,5)
DRWFN_COLOR(arc_filled,5)
DRWFN_COLOR(ellipse,5)
DRWFN_COLOR(ellipse_filled,5)
DRWFN_COLOR(sector,6)
DRWFN_COLOR(sector_filled,6)
DRWFN_COLOR(triangle,6)
DRWFN_COLOR(triangle_filled,6)
DRWFN_COLOR(rectangle_round,5)
DRWFN_COLOR(rectangle_round_filled,5)
DRWFN_COLOR(polygon,2)
DRWFN_COLOR(polygon_filled,2)
DRWFN_COLOR(polyline,3)
#endif

/* ----------------------------------------------------------------- */
