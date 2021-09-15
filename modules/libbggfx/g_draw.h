/*
 *  Copyright (C) SplinterGU (Fenix/BennuGD) (Since 2006)
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

#ifndef __G_DRAW_H
#define __G_DRAW_H

/* --------------------------------------------------------------------------- */

extern int64_t drawing_blend_mode;
extern CUSTOM_BLENDMODE drawing_custom_blendmode;

extern uint8_t drawing_color_r;
extern uint8_t drawing_color_g;
extern uint8_t drawing_color_b;
extern uint8_t drawing_color_a;

#ifdef USE_SDL2_GPU
extern float drawing_thickness;
#endif

/* --------------------------------------------------------------------------- */

extern void draw_point( GRAPH * dest, REGION * clip, int64_t x, int64_t y );
extern void draw_line( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t x2, int64_t y2 );
extern void draw_rectangle( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t w, int64_t h );
extern void draw_rectangle_filled( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t w, int64_t h );
#if ENABLE_MULTIDRAW
extern void draw_points( GRAPH * dest, REGION * clip, int64_t count, SDL_Point * points );
extern void draw_lines( GRAPH * dest, REGION * clip, int64_t count, SDL_Point * points );
extern void draw_rectangles( GRAPH * dest, REGION * clip, int64_t count, SDL_Rect * rects );
extern void draw_rectangles_filled( GRAPH * dest, REGION * clip, int64_t count, SDL_Rect * rects );
#endif
extern void draw_circle( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t r, int64_t * cache_size, void ** cache );
extern void draw_circle_filled( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t r, int64_t * cache_size, void ** cache );
extern void draw_bezier( GRAPH * dest, REGION * clip, int64_t x1, int64_t y1, int64_t x2, int64_t y2, int64_t x3, int64_t y3, int64_t x4, int64_t y4, int64_t level, int64_t * cache_size, void ** cache );

#ifdef USE_SDL2_GPU
extern void draw_arc( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t r, int64_t start_angle, int64_t end_angle );
extern void draw_arc_filled( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t r, int64_t start_angle, int64_t end_angle );
extern void draw_ellipse( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t rx, int64_t ry, int64_t degrees );
extern void draw_ellipse_filled( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t rx, int64_t ry, int64_t degrees );
extern void draw_sector( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t inner_radius, int64_t outer_radius, int64_t start_angle, int64_t end_angle );
extern void draw_sector_filled( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t inner_radius, int64_t outer_radius, int64_t start_angle, int64_t end_angle );
extern void draw_triangle( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t x2, int64_t y2, int64_t x3, int64_t y3 );
extern void draw_triangle_filled( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t x2, int64_t y2, int64_t x3, int64_t y3 );
extern void draw_rectangle_round( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t x2, int64_t y2, int64_t r );
extern void draw_rectangle_round_filled( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t x2, int64_t y2, int64_t r );
extern void draw_polygon( GRAPH * dest, REGION * clip, int nun_vertices, float * vertices );
extern void draw_polygon_filled( GRAPH * dest, REGION * clip, int nun_vertices, float * vertices );
extern void draw_polyline( GRAPH * dest, REGION * clip, int nun_vertices, float * vertices, int close_loop );
#endif

/* --------------------------------------------------------------------------- */

#endif
