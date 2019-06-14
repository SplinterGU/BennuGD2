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
#define DRAWOBJ_POINT       1
#define DRAWOBJ_POINTS      2
#define DRAWOBJ_LINE        3
#define DRAWOBJ_LINES       4
#define DRAWOBJ_RECT        5
#define DRAWOBJ_RECTS       6
#define DRAWOBJ_BOX         7
#define DRAWOBJ_BOXES       8
#define DRAWOBJ_CIRCLE      9
#define DRAWOBJ_FCIRCLE     10
#define DRAWOBJ_CURVE       11

typedef struct _drawing_object {
    int64_t type;

    int64_t x1;
    int64_t y1;

    int64_t w;
    int64_t h;

    int64_t x2;
    int64_t y2;
    int64_t x3;
    int64_t y3;
    int64_t x4;
    int64_t y4;

    int64_t level;

    int64_t radius;

    int64_t z;

    int64_t preallocated:1; // if data is preallocated, user must release it

    int64_t data_size; // count items of objs
    void * data; // objs Rects/Points
                 // For DRAWOBJ_CIRCLE / DRAWOBJ_FCIRCLE / DRAWOBJ_CURVE is cache

    /* Private */

    int64_t blend_mode;

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
static int64_t drawing_z = -256 ;

/* --------------------------------------------------------------------------- */

/*
 *  FUNCTION : _libmod_gfx_draw_object_info
 *
 *  Internal function used to return information about a primitive object
 *
 *  PARAMS :
 *      dr          Drawing object
 *      bbox        Pointer to a REGION to be filled with the bounding box
 *
 *  RETURN VALUE :
 *      1 if the primitive changed since last frame
 *
 */

static int _libmod_gfx_draw_object_info( void * what, REGION * bbox, int64_t * z, int64_t * drawme ) {

    REGION newclip;
    int64_t minx, miny, maxx, maxy;

    DRAWING_OBJECT * dr = ( DRAWING_OBJECT * ) what;
    if ( !dr ) return 1;

    * drawme = 1;

    * z = dr->z;

    if ( !bbox ) return 1;

    switch ( dr->type ) {
        case DRAWOBJ_CIRCLE:
        case DRAWOBJ_FCIRCLE:
            newclip.x = dr->x1 - dr->radius;
            newclip.y = dr->y1 - dr->radius;
            newclip.x2 = dr->x1 + dr->radius;
            newclip.y2 = dr->y1 + dr->radius;
            break;

        case DRAWOBJ_CURVE:
            newclip.x = dr->x1;
            newclip.y = dr->y1;
            newclip.x2 = dr->x4;
            newclip.y2 = dr->y4;
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
            newclip.x2 = dr->x2;
            newclip.y2 = dr->y2;
            break;

        // Maybe can be calculated
        case DRAWOBJ_POINTS:
        case DRAWOBJ_LINES:
        case DRAWOBJ_RECTS:
        case DRAWOBJ_BOXES:
            newclip.x = 0;
            newclip.y = 0;
            newclip.x2 = scr_width;
            newclip.y2 = scr_height;
            break;

        case DRAWOBJ_RECT:
        case DRAWOBJ_BOX:
        default:
            newclip.x = dr->x1;
            newclip.y = dr->y1;
            newclip.x2 = dr->x1 + dr->w;
            newclip.y2 = dr->y1 + dr->h;
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

    if ( newclip.x != bbox->x || newclip.y != bbox->y || newclip.x2 != bbox->x2 || newclip.y2 != bbox->y2 ) {
        * bbox = newclip;
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

    drawing_color_r = dr->color_r;
    drawing_color_g = dr->color_g;
    drawing_color_b = dr->color_b;
    drawing_color_a = dr->color_a;
    drawing_blend_mode = dr->blend_mode;

    switch ( dr->type ) {
        case DRAWOBJ_POINT:
            draw_point( NULL, clip, dr->x1, dr->y1 );
            break;

        case DRAWOBJ_LINE:
            draw_line( NULL, clip, dr->x1, dr->y1, dr->x2, dr->y2 );
            break;

        case DRAWOBJ_RECT:
            draw_rectangle( NULL, clip, dr->x1, dr->y1, dr->w, dr->h );
            break;

        case DRAWOBJ_BOX:
            draw_box( NULL, clip, dr->x1, dr->y1, dr->w, dr->h );
            break;

        case DRAWOBJ_POINTS:
            draw_points( NULL, clip, dr->data_size, ( SDL_Point * ) dr->data );
            break;

        case DRAWOBJ_LINES:
            draw_lines( NULL, clip, dr->data_size, ( SDL_Point * ) dr->data );
            break;

        case DRAWOBJ_RECTS:
            draw_rectangles( NULL, clip, dr->data_size, ( SDL_Rect * ) dr->data );
            break;

        case DRAWOBJ_BOXES:
            draw_boxes( NULL, clip, dr->data_size, ( SDL_Rect * ) dr->data );
            break;

        case DRAWOBJ_CIRCLE:
            draw_circle( NULL, clip, dr->x1, dr->y1, dr->radius, &dr->data_size, &dr->data );
            break;

        case DRAWOBJ_FCIRCLE:
            draw_fcircle( NULL, clip, dr->x1, dr->y1, dr->radius, &dr->data_size, &dr->data );
            break;

        case DRAWOBJ_CURVE:
            draw_bezier( NULL, clip, dr->x1, dr->y1, dr->x2, dr->y2, dr->x3, dr->y3, dr->x4, dr->y4, dr->level, &dr->data_size, &dr->data );
            break;
    }

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

    dr->preallocated = 0;

    switch ( dr->type ) {
        case DRAWOBJ_POINTS:
        case DRAWOBJ_LINES:
        case DRAWOBJ_RECTS:
        case DRAWOBJ_BOXES:
            dr->preallocated = 1;

        case DRAWOBJ_CIRCLE:
        case DRAWOBJ_FCIRCLE:
        case DRAWOBJ_CURVE:
            break;

        default:
            dr->data_size = 0;
            dr->data = NULL;
            break;

    }

    dr->id = gr_new_object( z, ( OBJ_INFO * ) _libmod_gfx_draw_object_info, ( OBJ_DRAW * ) _libmod_gfx_draw_object_draw, ( void * ) dr );

    drawing_objects = dr;

    return ( int64_t ) dr;
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
    DRAWING_OBJECT * dr = ( DRAWING_OBJECT * ) id, * next;
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
    DRAWING_OBJECT * dr = ( DRAWING_OBJECT * ) id;

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
                dr->x2 += incx;
                dr->y2 += incy;
                break;

            case DRAWOBJ_RECT:
            case DRAWOBJ_BOX:
                break;

            case DRAWOBJ_POINTS:
            case DRAWOBJ_LINES:
            case DRAWOBJ_CIRCLE:
            case DRAWOBJ_FCIRCLE:
            case DRAWOBJ_CURVE:
                if ( dr->data ) {
                    SDL_Point * p = ( SDL_Point * ) dr->data;
                    for ( i = 0; i < dr->data_size; i++, p++ ) {
                        p->x += incx, p->y += incy;
                    }
                }
                break;

            case DRAWOBJ_RECTS:
            case DRAWOBJ_BOXES:
                if ( dr->data ) {
                    SDL_Rect * p = ( SDL_Rect * ) dr->data;
                    for ( i = 0; i < dr->data_size; i++, p++ ) {
                        p->x += incx, p->y += incy;
                    }
                }
                break;
        }
    }
}

/* --------------------------------------------------------------------------- */

static SDL_PixelFormat * pixformat = NULL;

static void __get_rgba( int64_t color, uint8_t * r, uint8_t * g, uint8_t * b, uint8_t * a ) {
    if ( !pixformat ) pixformat = SDL_AllocFormat( SDL_PIXELFORMAT_ARGB8888 );
    SDL_GetRGBA( color, pixformat, r, g, b, a ) ;
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

int64_t libmod_gfx_draw_drawing_color_id( INSTANCE * my, int64_t * params ) {
    DRAWING_OBJECT * dr = ( DRAWING_OBJECT * ) params[0];
    if ( !dr ) return -1;
    __get_rgba( params[1], &dr->color_r, &dr->color_g, &dr->color_b, &dr->color_a ) ;
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_drawing_rgba_id( INSTANCE * my, int64_t * params ) {
    DRAWING_OBJECT * dr = ( DRAWING_OBJECT * ) params[0];
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

int64_t libmod_gfx_draw_drawing_blend_mode_id( INSTANCE * my, int64_t * params ) {
    DRAWING_OBJECT * dr = ( DRAWING_OBJECT * ) params[0];
    if ( !dr ) return -1;
    dr->blend_mode = params[1];
    return 1 ;
}


/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_drawing_z( INSTANCE * my, int64_t * params ) {
    drawing_z = params[ 0 ];
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_drawing_z_id( INSTANCE * my, int64_t * params ) {
    DRAWING_OBJECT * dr = ( DRAWING_OBJECT * ) params[0];
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

int64_t libmod_gfx_draw_points( INSTANCE * my, int64_t * params ) {
    if ( !drawing_graph ) {
        DRAWING_OBJECT * dr = malloc( sizeof( DRAWING_OBJECT ) );
        if ( !dr ) return -1;

        dr->type = DRAWOBJ_POINTS;
        dr->data_size = params[ 0 ];
        dr->data = ( void * ) params[ 1 ];;
        return _libmod_gfx_draw_object_new( dr, drawing_z );
    }

    draw_points( drawing_graph, 0, params[ 0 ], ( void * ) params[ 1 ] );
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
        dr->x2 = params[ 2 ];
        dr->y2 = params[ 3 ];
        return _libmod_gfx_draw_object_new( dr, drawing_z );
    }

    draw_line( drawing_graph, 0, params[ 0 ], params[ 1 ], params[ 2 ], params[ 3 ] );
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_lines( INSTANCE * my, int64_t * params ) {
    if ( !drawing_graph ) {
        DRAWING_OBJECT * dr = malloc( sizeof( DRAWING_OBJECT ) );
        if ( !dr ) return -1;

        dr->type = DRAWOBJ_LINES;
        dr->data_size = params[ 0 ];
        dr->data = ( void * ) params[ 1 ];;
        return _libmod_gfx_draw_object_new( dr, drawing_z );
    }

    draw_lines( drawing_graph, 0, params[ 0 ], ( void * ) params[ 1 ] );
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_box( INSTANCE * my, int64_t * params ) {
    if ( !drawing_graph ) {
        DRAWING_OBJECT * dr = malloc( sizeof( DRAWING_OBJECT ) );
        if ( !dr ) return -1;

        dr->type = DRAWOBJ_BOX;
        dr->x1 = params[ 0 ];
        dr->y1 = params[ 1 ];
        dr->w = params[ 2 ];
        dr->h = params[ 3 ];
        return _libmod_gfx_draw_object_new( dr, drawing_z );
    }

    draw_box( drawing_graph, 0, params[ 0 ], params[ 1 ], params[ 2 ], params[ 3 ] ) ;
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_boxes( INSTANCE * my, int64_t * params ) {
    if ( !drawing_graph ) {
        DRAWING_OBJECT * dr = malloc( sizeof( DRAWING_OBJECT ) );
        if ( !dr ) return -1;

        dr->type = DRAWOBJ_BOXES;
        dr->data_size = params[ 0 ];
        dr->data = ( void * ) params[ 1 ];;
        return _libmod_gfx_draw_object_new( dr, drawing_z );
    }

    draw_boxes( drawing_graph, 0, params[ 0 ], ( void * ) params[ 1 ] ) ;
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_rect( INSTANCE * my, int64_t * params ) {
    if ( !drawing_graph ) {
        DRAWING_OBJECT * dr = malloc( sizeof( DRAWING_OBJECT ) );
        if ( !dr ) return -1;

        dr->type = DRAWOBJ_RECT;
        dr->x1 = params[ 0 ];
        dr->y1 = params[ 1 ];
        dr->w = params[ 2 ];
        dr->h = params[ 3 ];
        return _libmod_gfx_draw_object_new( dr, drawing_z );
    }

    draw_rectangle( drawing_graph, 0, params[ 0 ], params[ 1 ], params[ 2 ], params[ 3 ] ) ;
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_draw_rects( INSTANCE * my, int64_t * params ) {
    if ( !drawing_graph ) {
        DRAWING_OBJECT * dr = malloc( sizeof( DRAWING_OBJECT ) );
        if ( !dr ) return -1;

        dr->type = DRAWOBJ_RECTS;
        dr->data_size = params[ 0 ];
        dr->data = ( void * ) params[ 1 ];;
        return _libmod_gfx_draw_object_new( dr, drawing_z );
    }

    draw_rectangles( drawing_graph, 0, params[ 0 ], ( void * ) params[ 1 ] ) ;
    return 1 ;
}

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

int64_t libmod_gfx_draw_fcircle( INSTANCE * my, int64_t * params ) {
    if ( !drawing_graph ) {
        DRAWING_OBJECT * dr = malloc( sizeof( DRAWING_OBJECT ) );
        if ( !dr ) return -1;

        dr->type = DRAWOBJ_FCIRCLE;
        dr->x1 = params[ 0 ];
        dr->y1 = params[ 1 ];
        dr->radius = params[ 2 ];

        dr->data_size = 0;
        dr->data = NULL;
        return _libmod_gfx_draw_object_new( dr, drawing_z );
    }

    draw_fcircle( drawing_graph, 0, params[ 0 ], params[ 1 ], params[ 2 ], NULL, NULL ) ;
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
        dr->x2 = params[ 2 ];
        dr->y2 = params[ 3 ];
        dr->x3 = params[ 4 ];
        dr->y3 = params[ 5 ];
        dr->x4 = params[ 6 ];
        dr->y4 = params[ 7 ];
        dr->level = params[ 8 ];

        dr->data_size = 0;
        dr->data = NULL;
        return _libmod_gfx_draw_object_new( dr, drawing_z );
    }

    draw_bezier( drawing_graph, 0, params[ 0 ], params[ 1 ], params[ 2 ], params[ 3 ], params[ 4 ], params[ 5 ], params[ 6 ], params[ 7 ], params[ 8 ], NULL, NULL );
    return 1;
}

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
DRWFN_COLOR(points,2)
DRWFN_COLOR(line,4)
DRWFN_COLOR(lines,2)
DRWFN_COLOR(box,4)
DRWFN_COLOR(boxes,2)
DRWFN_COLOR(rect,4)
DRWFN_COLOR(rects,2)
DRWFN_COLOR(circle,3)
DRWFN_COLOR(fcircle,3)
DRWFN_COLOR(bezier,9)

/* ----------------------------------------------------------------- */
