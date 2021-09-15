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

/* --------------------------------------------------------------------------- */

#ifndef __M_FPG_H
#define __M_FPG_H

#include "bgdrtm.h"
#include "bgddl.h"

extern int64_t libmod_gfx_load_fpg( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_unload_fpg( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_fpg_exists( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_fpg_add( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_fpg_new( INSTANCE * my, int64_t * params );

#endif
