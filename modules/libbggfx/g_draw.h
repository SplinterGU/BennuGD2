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

#ifndef __G_DRAW_H
#define __G_DRAW_H

/* --------------------------------------------------------------------------- */

extern int64_t draw_blend_mode;

extern uint8_t draw_color_r;
extern uint8_t draw_color_g;
extern uint8_t draw_color_b;
extern uint8_t draw_color_a;

/* --------------------------------------------------------------------------- */

extern void draw_point( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t x2, int64_t y2 );
extern void draw_points( GRAPH * dest, REGION * clip, int64_t count, SDL_Point * points );
extern void draw_line( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t x2, int64_t y2 );
extern void draw_lines( GRAPH * dest, REGION * clip, int64_t count, SDL_Point * points );
extern void draw_box( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t w, int64_t h );
extern void draw_rectangle( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t w, int64_t h );
extern void draw_circle( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t r );
extern void draw_fcircle( GRAPH * dest, REGION * clip, int64_t x, int64_t y, int64_t r );
extern void draw_bezier( GRAPH * dest, REGION * clip, int64_t x1, int64_t y1, int64_t x2, int64_t y2, int64_t x3, int64_t y3, int64_t x4, int64_t y4, int64_t level );

/* --------------------------------------------------------------------------- */

#endif
