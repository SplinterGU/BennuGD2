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
#include <string.h>

#include "bgdrtm.h"

#include "bgddl.h"
#include "libbggfx.h"

/* --------------------------------------------------------------------------- */

int64_t drawing_blend_mode = 0;
CUSTOM_BLENDMODE drawing_custom_blendmode = { 0 };

uint8_t drawing_color_r = 255;
uint8_t drawing_color_g = 255;
uint8_t drawing_color_b = 255;
uint8_t drawing_color_a = 255;

#ifdef USE_SDL2_GPU
float drawing_thickness = 1.0f;
#endif

/* --------------------------------------------------------------------------- */

#ifdef USE_SDL2
    #define DRAW_PREPARE_RENDERER()    BLENDMODE blend_mode = drawing_blend_mode; \
        if ( gr_prepare_renderer( dest, clip, 0, &blend_mode ) ) return; \
        SDL_SetRenderDrawBlendMode( gRenderer, blend_mode ); \
        SDL_SetRenderDrawColor( gRenderer, drawing_color_r, drawing_color_g, drawing_color_b, drawing_color_a )

    #define DRAW_RELEASE_RENDERER() if ( dest ) SDL_SetRenderTarget( gRenderer, NULL )
#endif
#ifdef USE_SDL2_GPU
    #define DRAW_PREPARE_RENDERER()    BLENDMODE blend_mode = drawing_blend_mode; \
        if ( gr_prepare_renderer( dest, clip, 0, &blend_mode ) ) return; \
        if ( blend_mode == BLEND_DISABLED ) { \
            GPU_SetShapeBlending( GPU_FALSE ); \
        } else { \
            GPU_SetShapeBlending( GPU_TRUE ); \
            if ( blend_mode == BLEND_CUSTOM ) { \
                GPU_SetShapeBlendFunction( drawing_custom_blendmode.src_rgb, drawing_custom_blendmode.dst_rgb, drawing_custom_blendmode.src_alpha, drawing_custom_blendmode.dst_alpha ); \
                GPU_SetShapeBlendEquation( drawing_custom_blendmode.eq_rgb, drawing_custom_blendmode.eq_alpha ); \
            } else { \
                GPU_SetShapeBlendMode( blend_mode ); \
            } \
        } \
        GPU_SetLineThickness( drawing_thickness ); \
        SDL_Color color; \
        color.r = drawing_color_r; color.g = drawing_color_g; color.b = drawing_color_b; color.a = drawing_color_a; \
        GPU_Target * target = dest ? dest->tex->target : gRenderer;

    #define DRAW_RELEASE_RENDERER()
#endif


/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : draw_point
 *
 *  Draw a point
 *
 *  PARAMS :
 *      dest            Destination bitmap or NULL for screen
 *      clip            Clipping region or NULL for the whole screen
 *      x, y            Coordinates of the point
 *
 *  RETURN VALUE :
 *      None
 *
 */

void draw_point( GRAPH * dest, REGION * clip, int64_t x, int64_t y ) {

    DRAW_PREPARE_RENDERER();

#ifdef USE_SDL2
    SDL_RenderDrawPoint( gRenderer, x, y );
#endif
#ifdef USE_SDL2_GPU
    // +1 for GPU_Pixel bug???
    GPU_Pixel( target, ( float ) x, ( float ) y + 1, color );
#endif

    DRAW_RELEASE_RENDERER();

}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : draw_line
 *
 *  Draw a line
 *
 *  PARAMS :
 *      dest            Destination bitmap or NULL for screen
 *      clip            Clipping region or NULL for the whole screen
 *      x, y            Coordinates of the first point
 *      x2, y2          Coordinates of the second point
 *
 *  RETURN VALUE :
 *      None
 *
 */

void draw_line( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t x2, int64_t y2 ) {

    DRAW_PREPARE_RENDERER();

#ifdef USE_SDL2
    SDL_RenderDrawLine( gRenderer, x, y, x2, y2 );
#endif
#ifdef USE_SDL2_GPU
    GPU_Line( target, ( float ) x, ( float ) y, ( float ) x2, ( float ) y2, color );
#endif

    DRAW_RELEASE_RENDERER();

}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : draw_rectangle
 *
 *  Draw a rectangle (non-filled)
 *
 *  PARAMS :
 *      dest            Destination bitmap or NULL for screen
 *      clip            Clipping region or NULL for the whole screen
 *      x, y            Coordinates of the top-left pixel
 *      w               Width in pixels
 *      h               Height in pixels
 *
 *  RETURN VALUE :
 *      None
 *
 */

void draw_rectangle( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t w, int64_t h ) {

    DRAW_PREPARE_RENDERER();

#ifdef USE_SDL2
    SDL_Rect rectangle;

    rectangle.x = x;
    rectangle.y = y;
    rectangle.w = w;
    rectangle.h = h;

    SDL_RenderDrawRect( gRenderer, &rectangle );
#endif
#ifdef USE_SDL2_GPU
    GPU_Rectangle( target, x, y, x + w, y + h, color );
#endif

    DRAW_RELEASE_RENDERER();

}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : draw_rectangle_filled
 *
 *  Draw a filled rectangle
 *
 *  PARAMS :
 *      dest            Destination bitmap or NULL for screen
 *      clip            Clipping region or NULL for the whole screen
 *      x, y            Coordinates of the top-left pixel
 *      w               Width in pixels
 *      h               Height in pixels
 *
 *  RETURN VALUE :
 *      None
 *
 */

void draw_rectangle_filled( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t w, int64_t h ) {

    DRAW_PREPARE_RENDERER();

#ifdef USE_SDL2
    SDL_Rect rectangle;

    rectangle.x = x;
    rectangle.y = y;
    rectangle.w = w;
    rectangle.h = h;

    SDL_RenderFillRect( gRenderer, &rectangle );
#endif
#ifdef USE_SDL2_GPU
    GPU_RectangleFilled( target, x, y, x + w, y + h, color );
#endif

    DRAW_RELEASE_RENDERER();

}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : draw_lines
 *
 *  Draw multiple line
 *
 *  PARAMS :
 *      dest            Destination bitmap or NULL for screen
 *      clip            Clipping region or NULL for the whole screen
 *      count           Number of points
 *      points          Pointer to SDL_Point with points
 *
 *  RETURN VALUE :
 *      None
 *
 */

void draw_lines( GRAPH * dest, REGION * clip, int64_t count, SDL_Point * points ) {

    DRAW_PREPARE_RENDERER();

#ifdef USE_SDL2
    SDL_RenderDrawLines( gRenderer, points, count );
#endif
#ifdef USE_SDL2_GPU
    count--;
    if ( count > 1 ) {
        while( count-- ) {
            GPU_Line( target, ( float ) points[0].x, ( float ) points[0].y, ( float ) points[1].x, ( float ) points[1].y, color );
            points++;
        }
    }
#endif

    DRAW_RELEASE_RENDERER();

}

#if defined(ENABLE_MULTIDRAW) || defined(USE_SDL2)
/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : draw_points
 *
 *  Draw multiple points
 *
 *  PARAMS :
 *      dest            Destination bitmap or NULL for screen
 *      clip            Clipping region or NULL for the whole screen
 *      count           Number of points
 *      points          Pointer to SDL_Point with points
 *
 *  RETURN VALUE :
 *      None
 *
 */

void draw_points( GRAPH * dest, REGION * clip, int64_t count, SDL_Point * points ) {

    DRAW_PREPARE_RENDERER();

#ifdef USE_SDL2
    SDL_RenderDrawPoints( gRenderer, points, count );
#endif
#ifdef USE_SDL2_GPU
    while( count-- ) {
        GPU_Pixel( target, ( float ) points->x, ( float ) points->y, color );
        points++;
    }
#endif

    DRAW_RELEASE_RENDERER();

}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : draw_rectangles
 *
 *  Draw multiple rectangles (non-filled)
 *
 *  PARAMS :
 *      dest            Destination bitmap or NULL for screen
 *      clip            Clipping region or NULL for the whole screen
 *      count           Number of rects
 *      rects           Pointer to SDL_Rect with rects
 *
 *  RETURN VALUE :
 *      None
 *
 */

void draw_rectangles( GRAPH * dest, REGION * clip, int64_t count, SDL_Rect * rects ) {

    DRAW_PREPARE_RENDERER();

#ifdef USE_SDL2
    SDL_RenderDrawRects( gRenderer, rects, count );
#endif
#ifdef USE_SDL2_GPU
    while( count-- ) {
        GPU_Rectangle( target, rects->x, rects->y, rects->x + rects->w, rects->y + rects->h, color );
        rects++;
    }
#endif

    DRAW_RELEASE_RENDERER();

}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : draw_rectangles_filled
 *
 *  Draw multiple filled rectangles
 *
 *  PARAMS :
 *      dest            Destination bitmap or NULL for screen
 *      clip            Clipping region or NULL for the whole screen
 *      count           Number of rects
 *      rects           Pointer to SDL_Rect with rects
 *
 *  RETURN VALUE :
 *      None
 *
 */

void draw_rectangles_filled( GRAPH * dest, REGION * clip, int64_t count, SDL_Rect * rects ) {

    DRAW_PREPARE_RENDERER();

#ifdef USE_SDL2
    SDL_RenderFillRects( gRenderer, rects, count );
#endif
#ifdef USE_SDL2_GPU
    while( count-- ) {
        GPU_RectangleFilled( target, rects->x, rects->y, rects->x + rects->w, rects->y + rects->h, color );
        rects++;
    }
#endif

    DRAW_RELEASE_RENDERER();

}
#endif
/* --------------------------------------------------------------------------- */

#define setPoint(_x,_y) { \
    points[count].x = _x; \
    points[count++].y = _y; \
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : draw_circle
 *
 *  Draw a circle
 *
 *  PARAMS :
 *      dest            Destination bitmap or NULL for screen
 *      clip            Clipping region or NULL for the whole screen
 *      x, y            Coordinates of the center
 *      r               Radius, in pixels
 *
 *  RETURN VALUE :
 *      None
 *
 */

void draw_circle( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t r, int64_t * cache_size, void ** cache ) {

#ifdef USE_SDL2
    SDL_Point * points;
    int free_on_exit = 1;
    int64_t count = 0;

    if ( !cache || !*cache ) {
        int64_t cx = 0, cy = r;
        int64_t df = 1 - r, de = 3, dse = -2 * r + 5;

        points = malloc( sizeof( SDL_Point ) * r * 8 );

        if ( !points ) return;

        do {
            setPoint( x - cx, y - cy );
            if ( cx ) setPoint( x + cx, y - cy );

            if ( cy ) {
                setPoint( x - cx, y + cy );
                if ( cx ) setPoint( x + cx, y + cy );
            }

            if ( cx != cy ) {
                setPoint( x - cy, y - cx );
                if ( cy ) setPoint( x + cy, y - cx );
            }

            if ( cx && cy != cx ) {
                setPoint( x - cy, y + cx );
                if ( cy ) setPoint( x + cy, y + cx );
            }

            cx++ ;
            if ( df < 0 ) df += de, de += 2, dse += 2 ;
            else df += dse, de += 2, dse += 4, cy-- ;
        } while ( cx <= cy ) ;

        if ( cache ) {
            * cache = points;
            if ( cache_size ) * cache_size = count;
            free_on_exit = 0;
        }

    } else {
        points = ( SDL_Point * ) * cache;
        count = * cache_size;
        free_on_exit = 0;
    }

    draw_points( dest, clip, count, points );

    if ( free_on_exit ) free( points );
#endif
#ifdef USE_SDL2_GPU
    DRAW_PREPARE_RENDERER();

    GPU_Circle( target, ( float ) x, ( float ) y, ( float ) r, color);

    DRAW_RELEASE_RENDERER();
#endif
}

/* --------------------------------------------------------------------------- */

#ifdef USE_SDL2
static int _sort_circle_points(const void * a, const void * b ) {

    SDL_Point * _a = ( SDL_Point *) a;
    SDL_Point * _b = ( SDL_Point *) b;

    if ( _a->y != _b->y ) return _a->y - _b->y;
    if ( _a->x != _b->x ) {
        if ( _a->y & 1 ) return _b->x - _a->x;
        return _a->x - _b->x;
    }
    return 0;

}
#endif

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : draw_circle_filled
 *
 *  Draw a filled circle
 *
 *  PARAMS :
 *      dest            Destination bitmap or NULL for screen
 *      clip            Clipping region or NULL for the whole screen
 *      x, y            Coordinates of the center
 *      r               Radius, in pixels
 *
 *  RETURN VALUE :
 *      None
 *
 */

void draw_circle_filled( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t r, int64_t * cache_size, void ** cache ) {
#ifdef USE_SDL2
    SDL_Point * points;
    int free_on_exit = 1;
    int64_t count = 0;

    if ( !cache || !*cache ) {
        int64_t cx = 0, cy = r;
        int64_t df = 1 - r, de = 3, dse = -2 * r + 5;

        points = malloc( sizeof( SDL_Point ) * r * 8 );

        if ( !points ) return;

        do {
            if ( cx != cy ) {
                setPoint(x - cy, y - cx);
                setPoint(x - cy + cy * 2, y - cx);
                if ( cx ) {
                    setPoint(x - cy, y + cx);
                    setPoint(x - cy + cy * 2, y + cx);
                }
            }
            if ( df < 0 ) {
                df += de, de += 2, dse += 2 ;
            } else {
                df += dse, de += 2, dse += 4;
                setPoint(x - cx, y - cy);
                setPoint(x - cx + cx * 2, y - cy);
                if ( cy ) {
                    setPoint(x - cx, y + cy);
                    setPoint(x - cx + cx *2, y + cy);
                }
                cy-- ;
            }
            cx++ ;
        }
        while ( cx <= cy ) ;

        if ( cache ) {
            * cache = points;
            if ( cache_size ) * cache_size = count;
            free_on_exit = 0;
        }

    } else {
        points = ( SDL_Point * ) * cache;
        count = * cache_size;
        free_on_exit = 0;
    }

    draw_lines( dest, clip, count, points );

    if ( free_on_exit ) free( points );
#endif
#ifdef USE_SDL2_GPU
    DRAW_PREPARE_RENDERER();

    GPU_CircleFilled( target, ( float ) x, ( float ) y, ( float ) r, color);

    DRAW_RELEASE_RENDERER();
#endif
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : draw_bezier
 *
 *  Draw a bezier curve
 *
 *  PARAMS :
 *      dest            Destination bitmap or NULL for screen
 *      clip            Clipping region or NULL for the whole screen
 *      params          Pointer to an integer array with the parameters:
 *
 *          x1, y1
 *          x2, y2
 *          x3, y3
 *          x4, y4      Curve points
 *          level       Curve smoothness (1 to 15, 15 is more)
 *
 *  RETURN VALUE :
 *      None
 *
 */

void draw_bezier( GRAPH * dest, REGION * clip, int64_t x1, int64_t y1, int64_t x2, int64_t y2, int64_t x3, int64_t y3, int64_t x4, int64_t y4, int64_t level, int64_t * cache_size, void ** cache ) {

    SDL_Point * points;
    int free_on_exit = 1;
    int64_t count = 0;

    if ( !cache || !*cache ) {
        double x = ( double ) x1, y = ( double ) y1;
        double xp = x, yp = y;
        double delta;
        double dx, d2x, d3x;
        double dy, d2y, d3y;
        double a, b, c;
        int64_t i;
        int64_t n = 1;

        /* Compute number of iterations */

        if ( level < 1 ) level = 1;
        if ( level >= 15 ) level = 15;
        while ( level-- > 0 ) n *= 2;
        delta = 1.0f / ( double ) n;

        /* Compute finite differences */
        /* a, b, c are the coefficient of the polynom in target defining the parametric curve */
        /* The computation is done independently for x and y */

        a = ( double )( -x1 + 3 * x2 - 3 * x3 + x4 );
        b = ( double )( 3 * x1 - 6 * x2 + 3 * x3 );
        c = ( double )( -3 * x1 + 3 * x2 );

        d3x = 6 * a * delta * delta * delta;
        d2x = d3x + 2 * b * delta * delta;
        dx = a * delta * delta * delta + b * delta * delta + c * delta;

        a = ( double )( -y1 + 3 * y2 - 3 * y3 + y4 );
        b = ( double )( 3 * y1 - 6 * y2 + 3 * y3 );
        c = ( double )( -3 * y1 + 3 * y2 );

        d3y = 6 * a * delta * delta * delta;
        d2y = d3y + 2 * b * delta * delta;
        dy = a * delta * delta * delta + b * delta * delta + c * delta;

        points = malloc( sizeof( SDL_Point ) * ( n + 1 ) /* * 2 */ );
        if ( !points ) return;

        for ( i = 0; i < n; i++ ) {
            x += dx;
            dx += d2x;
            d2x += d3x;
            y += dy;
            dy += d2y;
            d2y += d3y;
            if (( int64_t )( xp ) != ( int64_t )( x ) || ( int64_t )( yp ) != ( int64_t )( y ) ) {
                if ( count == 0 ) setPoint( ( int64_t ) xp, ( int64_t ) yp );
                setPoint( ( int64_t ) x, ( int64_t ) y );
            }
            xp = x;
            yp = y;
        }

        if ( cache ) {
            * cache = points;
            if ( cache_size ) * cache_size = count;
            free_on_exit = 0;
        }

    } else {
        points = ( SDL_Point * ) * cache;
        count = * cache_size;
        free_on_exit = 0;
    }

    draw_lines( dest, clip, count, points );

    if ( free_on_exit ) free( points );

}

/* --------------------------------------------------------------------------- */

#ifdef USE_SDL2_GPU

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : draw_arc
 *
 *  Draw a arc
 *
 *  PARAMS :
 *      dest            Destination bitmap or NULL for screen
 *      clip            Clipping region or NULL for the whole screen
 *      x, y            Coordinates of the top-left pixel
 *      r               Radius
 *      start_angle     Start angle
 *      end_angle       End angle
 *
 *  RETURN VALUE :
 *      None
 *
 */

void draw_arc( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t r, int64_t start_angle, int64_t end_angle ) {

    DRAW_PREPARE_RENDERER();

    GPU_Arc( target, ( float ) x, ( float ) y, ( float ) r, start_angle / 1000.0, end_angle / 1000.0, color );

    DRAW_RELEASE_RENDERER();

}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : draw_arc_filled
 *
 *  Draw a filled arc
 *
 *  PARAMS :
 *      dest            Destination bitmap or NULL for screen
 *      clip            Clipping region or NULL for the whole screen
 *      x, y            Coordinates of the top-left pixel
 *      r               Radius
 *      start_angle     Start angle
 *      end_angle       End angle
 *
 *  RETURN VALUE :
 *      None
 *
 */

void draw_arc_filled( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t r, int64_t start_angle, int64_t end_angle ) {

    DRAW_PREPARE_RENDERER();

    GPU_ArcFilled( target, ( float ) x, ( float ) y, ( float ) r, start_angle / 1000.0, end_angle / 1000.0, color );

    DRAW_RELEASE_RENDERER();

}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : draw_ellipse
 *
 *  Draw a ellipse
 *
 *  PARAMS :
 *      dest            Destination bitmap or NULL for screen
 *      clip            Clipping region or NULL for the whole screen
 *      x, y            Coordinates of the top-left pixel
 *      rx              X radius of ellipse
 *      ry              Y radius of ellipse
 *      degrees         The angle to rotate the ellipse
 *
 *  RETURN VALUE :
 *      None
 *
 */

void draw_ellipse( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t rx, int64_t ry, int64_t degrees ) {

    DRAW_PREPARE_RENDERER();

    GPU_Ellipse( target, ( float ) x, ( float ) y, ( float ) rx, ( float ) ry, ( float ) degrees, color );

    DRAW_RELEASE_RENDERER();

}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : draw_ellipse_filled
 *
 *  Draw a filled ellipse
 *
 *  PARAMS :
 *      dest            Destination bitmap or NULL for screen
 *      clip            Clipping region or NULL for the whole screen
 *      x, y            Coordinates of the top-left pixel
 *      rx              X radius of ellipse
 *      ry              Y radius of ellipse
 *      degrees         The angle to rotate the ellipse
 *
 *  RETURN VALUE :
 *      None
 *
 */

void draw_ellipse_filled( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t rx, int64_t ry, int64_t degrees ) {

    DRAW_PREPARE_RENDERER();

    GPU_EllipseFilled( target, ( float ) x, ( float ) y, ( float ) rx, ( float ) ry, ( float ) degrees, color );

    DRAW_RELEASE_RENDERER();

}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : draw_sector
 *
 *  Draw a sector
 *
 *  PARAMS :
 *      dest            Destination bitmap or NULL for screen
 *      clip            Clipping region or NULL for the whole screen
 *      x, y            Coordinates of the top-left pixel
 *      inner_radius    The inner radius of the ring
 *      outer_radius    The outer radius of the ring
 *      start_angle     The angle to start from, in degrees.  Measured clockwise from the positive x-axis.
 *      end_angle       The angle to end at, in degrees.  Measured clockwise from the positive x-axis.
 *
 *  RETURN VALUE :
 *      None
 *
 */

void draw_sector( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t inner_radius, int64_t outer_radius, int64_t start_angle, int64_t end_angle ) {

    DRAW_PREPARE_RENDERER();

    GPU_Sector( target, ( float ) x, ( float ) y, ( float ) inner_radius, ( float ) outer_radius, start_angle / 1000.0, end_angle / 1000.0, color );

    DRAW_RELEASE_RENDERER();

}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : draw_sector_filled
 *
 *  Draw a filled sector
 *
 *  PARAMS :
 *      dest            Destination bitmap or NULL for screen
 *      clip            Clipping region or NULL for the whole screen
 *      x, y            Coordinates of the top-left pixel
 *      inner_radius    The inner radius of the ring
 *      outer_radius    The outer radius of the ring
 *      start_angle     The angle to start from, in degrees.  Measured clockwise from the positive x-axis.
 *      end_angle       The angle to end at, in degrees.  Measured clockwise from the positive x-axis.
 *
 *  RETURN VALUE :
 *      None
 *
 */

void draw_sector_filled( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t inner_radius, int64_t outer_radius, int64_t start_angle, int64_t end_angle ) {

    DRAW_PREPARE_RENDERER();

    GPU_SectorFilled( target, ( float ) x, ( float ) y, ( float ) inner_radius, ( float ) outer_radius, start_angle / 1000.0, end_angle / 1000.0, color );

    DRAW_RELEASE_RENDERER();

}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : draw_triangle
 *
 *  Draw a triangle
 *
 *  PARAMS :
 *      dest            Destination bitmap or NULL for screen
 *      clip            Clipping region or NULL for the whole screen
 *      x, y            Coordinates of the top-left pixel
 *      inner_radius    The inner radius of the ring
 *      outer_radius    The outer radius of the ring
 *      start_angle     The angle to start from, in degrees.  Measured clockwise from the positive x-axis.
 *      end_angle       The angle to end at, in degrees.  Measured clockwise from the positive x-axis.
 *
 *  RETURN VALUE :
 *      None
 *
 */

void draw_triangle( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t x2, int64_t y2, int64_t x3, int64_t y3 ) {

    DRAW_PREPARE_RENDERER();

    GPU_Tri( target, ( float ) x, ( float ) y, ( float ) x2, ( float ) y2, ( float ) x3, ( float ) y3, color );

    DRAW_RELEASE_RENDERER();

}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : draw_triangle_filled
 *
 *  Draw a filled triangle
 *
 *  PARAMS :
 *      dest            Destination bitmap or NULL for screen
 *      clip            Clipping region or NULL for the whole screen
 *      x, y            Coordinates of the top-left pixel
 *      inner_radius    The inner radius of the ring
 *      outer_radius    The outer radius of the ring
 *      start_angle     The angle to start from, in degrees.  Measured clockwise from the positive x-axis.
 *      end_angle       The angle to end at, in degrees.  Measured clockwise from the positive x-axis.
 *
 *  RETURN VALUE :
 *      None
 *
 */

void draw_triangle_filled( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t x2, int64_t y2, int64_t x3, int64_t y3 ) {

    DRAW_PREPARE_RENDERER();

    GPU_TriFilled( target, ( float ) x, ( float ) y, ( float ) x2, ( float ) y2, ( float ) x3, ( float ) y3, color );

    DRAW_RELEASE_RENDERER();

}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : draw_rectangle_round
 *
 *  Draw a rectangle_round (non-filled)
 *
 *  PARAMS :
 *      dest            Destination bitmap or NULL for screen
 *      clip            Clipping region or NULL for the whole screen
 *      x, y            Coordinates of the top-left pixel
 *      w               Width in pixels
 *      h               Height in pixels
 *      r               The radius of the corners
 *
 *  RETURN VALUE :
 *      None
 *
 */

void draw_rectangle_round( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t w, int64_t h, int64_t r ) {

    DRAW_PREPARE_RENDERER();

    GPU_RectangleRound( target, x, y, x + w, y + h, r, color );

    DRAW_RELEASE_RENDERER();

}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : draw_rectangle_round_filled
 *
 *  Draw a filled rectangle_round
 *
 *  PARAMS :
 *      dest            Destination bitmap or NULL for screen
 *      clip            Clipping region or NULL for the whole screen
 *      x, y            Coordinates of the top-left pixel
 *      w               Width in pixels
 *      h               Height in pixels
 *      r               The radius of the corners
 *
 *  RETURN VALUE :
 *      None
 *
 */

void draw_rectangle_round_filled( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t w, int64_t h, int64_t r ) {

    DRAW_PREPARE_RENDERER();

    GPU_RectangleRoundFilled( target, x, y, x + w, y + h, r, color );

    DRAW_RELEASE_RENDERER();

}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : draw_polygon
 *
 *  Draw a polygon (non-filled)
 *
 *  PARAMS :
 *      dest            Destination bitmap or NULL for screen
 *      clip            Clipping region or NULL for the whole screen
 *      num_vertices    Number of vertices (x and y pairs)
 *      vertices        An array of vertex positions stored as interlaced x and y coords, e.g. {x1, y1, x2, y2, ...} *
 *
 *  RETURN VALUE :
 *      None
 *
 */

void draw_polygon( GRAPH * dest, REGION * clip, int nun_vertices, float * vertices ) {

    DRAW_PREPARE_RENDERER();

    GPU_Polygon( target, nun_vertices, vertices, color );

    DRAW_RELEASE_RENDERER();

}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : draw_polygon_filled
 *
 *  Draw a filled polygon
 *
 *  PARAMS :
 *      dest            Destination bitmap or NULL for screen
 *      clip            Clipping region or NULL for the whole screen
 *      num_vertices    Number of vertices (x and y pairs)
 *      vertices        An array of vertex positions stored as interlaced x and y coords, e.g. {x1, y1, x2, y2, ...} *
 *  RETURN VALUE :
 *      None
 *
 */

void draw_polygon_filled( GRAPH * dest, REGION * clip, int nun_vertices, float * vertices ) {

    DRAW_PREPARE_RENDERER();

    GPU_PolygonFilled( target, nun_vertices, vertices, color );

    DRAW_RELEASE_RENDERER();

}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : draw_polyline
 *
 *  Draw a polyline shape
 *
 *  PARAMS :
 *      dest            Destination bitmap or NULL for screen
 *      clip            Clipping region or NULL for the whole screen
 *      num_vertices    Number of vertices (x and y pairs)
 *      vertices        An array of vertex positions stored as interlaced x and y coords, e.g. {x1, y1, x2, y2, ...}
 *      close_loop      Make a closed polygon by drawing a line at the end back to the start point
 *
 *  RETURN VALUE :
 *      None
 *
 */

void draw_polyline( GRAPH * dest, REGION * clip, int nun_vertices, float * vertices, int close_loop ) {

    DRAW_PREPARE_RENDERER();

    GPU_Polyline( target, nun_vertices, vertices, color, close_loop );

    DRAW_RELEASE_RENDERER();

}

/* --------------------------------------------------------------------------- */

#endif
