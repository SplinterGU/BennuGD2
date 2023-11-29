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

#ifndef __M_MATH_H
#define __M_MATH_H

extern int64_t libmod_misc_math_abs( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_exp( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_log( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_log10( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_pow( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_sqrt( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_cos( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_sin( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_tan( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_acos( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_asin( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_atan( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_atan2( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_isinf( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_isnan( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_finite( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_fget_angle( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_fget_dist( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_near_angle( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_get_distx( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_get_disty( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_clamp_double( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_clamp( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_between_double( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_between( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_towards_double( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_towards( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_wrap( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_lerp( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_invert_lerp( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_check_range_double( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_check_range( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_remap_double( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_remap( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_normalize_double( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_normalize( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_min( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_max( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_sgn( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_sgn2( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_round( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_floor( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_ceil( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_trunc( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_frac( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_decimal( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_rad( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_deg( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_intersect( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_intersect_line_circle( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_intersect_circle( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_normal_projection( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_math_orthogonal_projection( INSTANCE * my, int64_t * params );

#endif
