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

#ifndef __G_BLIT_H
#define __G_BLIT_H

#include <bgddl.h>
#include <g_bitmap.h>
#include <g_regions.h>

/* --------------------------------------------------------------------------- */
/* Flags for gr_blit                                                           */

#define B_HMIRROR       0x0001
#define B_VMIRROR       0x0002
#define B_ABLEND        0x0010
#define B_SBLEND        0x0020
#define B_MBLEND        0x0040
#define B_NOCOLORKEY    0x0080

/* --------------------------------------------------------------------------- */

extern int gr_prepare_renderer( GRAPH * dest, REGION * clip, int64_t flags, SDL_BlendMode * blend_mode );
extern void gr_blit( GRAPH * dest, REGION * clip, int64_t scrx, int64_t scry, int64_t flags, int64_t angle, int64_t scalex, int64_t scaley, GRAPH * gr, SDL_Rect * gr_clip, uint8_t alpha, uint8_t color_r, uint8_t color_g, uint8_t color_b );
extern void gr_get_bbox( REGION * dest, REGION * clip, int64_t x, int64_t y, int64_t flags, int64_t angle, int64_t scalex, int64_t scaley, GRAPH * gr, SDL_Rect * map_clip );

/* --------------------------------------------------------------------------- */

#endif
