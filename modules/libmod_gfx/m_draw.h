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

#ifndef __M_DRAW_H
#define __M_DRAW_H

#include "bgdrtm.h"
#include "bgddl.h"

/* --------------------------------------------------------------------------- */

extern int64_t libmod_gfx_draw_drawing_color( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_drawing_rgba( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_drawing_color2( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_drawing_rgba2( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_drawing_blend_mode( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_drawing_blend_mode2( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_drawing_blend_mode3( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_drawing_blend_mode4( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_drawing_z_get( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_drawing_z( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_drawing_z2( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_drawing_map( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_delete_drawing( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_move_drawing( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_getcoords( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_point( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_line( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_rectangle( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_rectangle_filled( INSTANCE * my, int64_t * params );
#if ENABLE_MULTIDRAW
extern int64_t libmod_gfx_draw_points( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_lines( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_rectangles( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_rectangles_filled( INSTANCE * my, int64_t * params );
#endif
extern int64_t libmod_gfx_draw_circle( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_circle_filled( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_bezier( INSTANCE * my, int64_t * params );

#ifdef USE_SDL2_GPU
extern int64_t libmod_gfx_draw_arc( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_arc_filled( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_ellipse( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_ellipse_filled( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_sector( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_sector_filled( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_triangle( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_triangle_filled( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_rectangle_round( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_rectangle_round_filled( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_polygon( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_polygon_filled( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_polyline( INSTANCE * my, int64_t * params );

extern int64_t libmod_gfx_draw_set_thickness( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_set_thickness2( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_get_thickness( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_get_thickness2( INSTANCE * my, int64_t * params );
#endif

#define EXPORT_DRWFN_COLOR(fn) extern int64_t libmod_gfx_draw_##fn##_color( INSTANCE * my, int64_t * params );

EXPORT_DRWFN_COLOR(point)
EXPORT_DRWFN_COLOR(line)
EXPORT_DRWFN_COLOR(rectangle)
EXPORT_DRWFN_COLOR(rectangle_filled)
#if ENABLE_MULTIDRAW
EXPORT_DRWFN_COLOR(points)
EXPORT_DRWFN_COLOR(lines)
EXPORT_DRWFN_COLOR(rectangles_filled)
EXPORT_DRWFN_COLOR(rectangles)
#endif
EXPORT_DRWFN_COLOR(circle)
EXPORT_DRWFN_COLOR(circle_filled)
EXPORT_DRWFN_COLOR(bezier)
#ifdef USE_SDL2_GPU
EXPORT_DRWFN_COLOR(arc)
EXPORT_DRWFN_COLOR(arc_filled)
EXPORT_DRWFN_COLOR(ellipse)
EXPORT_DRWFN_COLOR(ellipse_filled)
EXPORT_DRWFN_COLOR(sector)
EXPORT_DRWFN_COLOR(sector_filled)
EXPORT_DRWFN_COLOR(triangle)
EXPORT_DRWFN_COLOR(triangle_filled)
EXPORT_DRWFN_COLOR(rectangle_round)
EXPORT_DRWFN_COLOR(rectangle_round_filled)
EXPORT_DRWFN_COLOR(polygon)
EXPORT_DRWFN_COLOR(polygon_filled)
EXPORT_DRWFN_COLOR(polyline)
#endif

#undef EXPORT_DRWFN_COLOR

#define DRWFN_COLOR(fn) libmod_gfx_draw_##fn##_color

/* ----------------------------------------------------------------- */

#endif
