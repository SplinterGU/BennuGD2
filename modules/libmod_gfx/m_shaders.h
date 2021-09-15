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

#include "bgdrtm.h"
#include "bgddl.h"

extern int64_t libmod_gfx_shader_create( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_shader_free(  INSTANCE * my, int64_t * params );

extern int64_t libmod_gfx_shader_activate( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_shader_deactivate( INSTANCE * my, int64_t * params );

extern int64_t libmod_gfx_shader_getattributelocation( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_shader_getuniformlocation( INSTANCE * my, int64_t * params );

extern int64_t libmod_gfx_shader_setshaderimage( INSTANCE * my, int64_t * params );

extern int64_t libmod_gfx_shader_setattributei( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_shader_setattributeiv( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_shader_setattributeui( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_shader_setattributeuiv( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_shader_setattributef( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_shader_setattributefv( INSTANCE * my, int64_t * params );

extern int64_t libmod_gfx_shader_setuniformi( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_shader_setuniformiv( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_shader_setuniform2iv( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_shader_setuniform3iv( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_shader_setuniform4iv( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_shader_setuniformui( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_shader_setuniformuiv( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_shader_setuniform2uiv( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_shader_setuniform3uiv( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_shader_setuniform4uiv( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_shader_setuniformf( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_shader_setuniformfv( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_shader_setuniform2fv( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_shader_setuniform3fv( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_shader_setuniform4fv( INSTANCE * my, int64_t * params );

extern int64_t libmod_gfx_shader_setuniformmatrix( INSTANCE * my, int64_t * params );

#endif
