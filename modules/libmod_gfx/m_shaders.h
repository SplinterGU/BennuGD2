/*
 *  Copyright (C) SplinterGU (Fenix/BennuGD) (Since 2006)
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

#ifndef __M_SHADERS_H
#define __M_SHADERS_H

#include "bgddl.h"

extern int64_t libmod_gfx_shader_get_language( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_shader_get_min_version( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_shader_get_max_version( INSTANCE * my, int64_t * params );

extern int64_t libmod_gfx_shader_create( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_shader_free(  INSTANCE * my, int64_t * params );

#if 0
extern int64_t libmod_gfx_shader_activate( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_shader_deactivate( INSTANCE * my, int64_t * params );
#endif

extern int64_t libmod_gfx_shader_getattributelocation( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_shader_getuniformlocation( INSTANCE * my, int64_t * params );

extern int64_t libmod_gfx_shader_create_parameters( INSTANCE * my, int64_t * params );

extern int64_t libmod_gfx_shader_free_parameters( INSTANCE * my, int64_t * params );

extern int64_t libmod_gfx_shader_setparam( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_shader_setparam_float( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_shader_setparam_image( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_shader_setparam_vector( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_shader_setparam_matrix( INSTANCE * my, int64_t * params );

#endif
