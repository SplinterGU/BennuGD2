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

#ifndef __REGIONS_H
#define __REGIONS_H

/* --------------------------------------------------------------------------- */

#include "g_region.h"

/* --------------------------------------------------------------------------- */

#define MAX_REGIONS 256

/* --------------------------------------------------------------------------- */

extern REGION regions[ MAX_REGIONS ];

/* --------------------------------------------------------------------------- */

extern void region_define( int64_t region, int64_t x, int64_t y, int64_t width, int64_t height );
extern void region_union( REGION * a, REGION * b );
extern int region_is_empty( REGION * a );
extern int region_is_out( REGION * a, REGION * b );
extern REGION * region_new( int64_t x, int64_t y, int64_t width, int64_t height );
extern void region_destroy( REGION * );
extern REGION * region_get( int64_t n );

/* --------------------------------------------------------------------------- */

#endif
