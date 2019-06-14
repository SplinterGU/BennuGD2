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
extern int64_t libmod_gfx_draw_drawing_color_id( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_drawing_rgba_id( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_drawing_blend_mode( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_drawing_blend_mode_id( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_drawing_z( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_drawing_z_id( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_drawing_map( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_delete_drawing( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_move_drawing( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_point( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_points( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_line( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_lines( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_box( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_boxes( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_rect( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_rects( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_circle( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_fcircle( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_draw_bezier( INSTANCE * my, int64_t * params );

#define EXPORT_DRWFN_COLOR(fn) extern int64_t libmod_gfx_draw_##fn##_color( INSTANCE * my, int64_t * params );

EXPORT_DRWFN_COLOR(point)
EXPORT_DRWFN_COLOR(points)
EXPORT_DRWFN_COLOR(line)
EXPORT_DRWFN_COLOR(lines)
EXPORT_DRWFN_COLOR(box)
EXPORT_DRWFN_COLOR(boxes)
EXPORT_DRWFN_COLOR(rect)
EXPORT_DRWFN_COLOR(rects)
EXPORT_DRWFN_COLOR(circle)
EXPORT_DRWFN_COLOR(fcircle)
EXPORT_DRWFN_COLOR(bezier)

#undef EXPORT_DRWFN_COLOR

#define DRWFN_COLOR(fn) libmod_gfx_draw_##fn##_color

/* ----------------------------------------------------------------- */

#endif
