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

int64_t drawing_blend_mode = B_NOCOLORKEY;

uint8_t drawing_color_r = 255;
uint8_t drawing_color_g = 255;
uint8_t drawing_color_b = 255;
uint8_t drawing_color_a = 255;

/* --------------------------------------------------------------------------- */

#define DRAW_PREPARE_RENDERER()    BLENDMODE blend_mode; \
    if ( gr_prepare_renderer( dest, clip, drawing_blend_mode, &blend_mode ) ) return; \
    SDL_SetRenderDrawBlendMode( gRenderer, blend_mode ); \
    SDL_SetRenderDrawColor( gRenderer, drawing_color_r, drawing_color_g, drawing_color_b, drawing_color_a )

#define DRAW_RELEASE_RENDERER() if ( dest ) SDL_SetRenderTarget( gRenderer, NULL )

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

    SDL_RenderDrawPoint( gRenderer, x, y );

    DRAW_RELEASE_RENDERER();

}

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

    SDL_RenderDrawPoints( gRenderer, points, count );

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

    SDL_RenderDrawLine( gRenderer, x, y, x2, y2 );

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

    SDL_RenderDrawLines( gRenderer, points, count );

    DRAW_RELEASE_RENDERER();

}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : draw_box
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

void draw_box( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t w, int64_t h ) {

    DRAW_PREPARE_RENDERER();

    SDL_Rect rectangle;

    rectangle.x = x;
    rectangle.y = y;
    rectangle.w = w;
    rectangle.h = h;

    SDL_RenderFillRect( gRenderer, &rectangle );

    DRAW_RELEASE_RENDERER();

}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : draw_boxes
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

void draw_boxes( GRAPH * dest, REGION * clip, int64_t count, SDL_Rect * rects ) {

    DRAW_PREPARE_RENDERER();

    SDL_RenderFillRects( gRenderer, rects, count );

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

    SDL_Rect rectangle;

    rectangle.x = x;
    rectangle.y = y;
    rectangle.w = w;
    rectangle.h = h;

    SDL_RenderDrawRect( gRenderer, &rectangle );

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

    SDL_RenderDrawRects( gRenderer, rects, count );

    DRAW_RELEASE_RENDERER();

}

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

}

/* --------------------------------------------------------------------------- */

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

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : draw_fcircle
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

void draw_fcircle( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t r, int64_t * cache_size, void ** cache ) {

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
        /* a, b, c are the coefficient of the polynom in t defining the parametric curve */
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

        points = malloc( sizeof( SDL_Point ) * n * 2 );
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
