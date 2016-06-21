/*
 *  Copyright (C) 2006-2016 SplinterGU (Fenix/BennuGD)
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

#ifndef __M_MAP_H
#define __M_MAP_H

#include <SDL.h>
#include <SDL_image.h>

#include "bgdrtm.h"
#include "bgddl.h"

extern int64_t libmod_gfx_map_get_pixel( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_map_put_pixel( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_graphic_set( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_graphic_info( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_set_point( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_set_center( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_get_point( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_map_clear( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_map_clear2( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_map_clear3( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_map_clear4( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_new_map( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_new_map_extend( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_map_clone( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_map_put( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_map_xput( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_map_name( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_map_set_name( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_map_exists( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_map_block_copy( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_load_map( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_unload_map( INSTANCE * my, int64_t * params );

extern int64_t gr_load_img( const char * mapname );

#endif
