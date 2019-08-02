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

#ifndef __LIBMOD_GFX_H
#define __LIBMOD_GFX_H

/* --------------------------------------------------------------------------- */

#include "bgdrtm.h"
#include "bgddl.h"

#include "file_div.h"
#include "m_bgload.h"
#include "m_collision.h"
#include "m_fade.h"
#include "m_fnt.h"
#include "m_fpg.h"
#include "m_map.h"
#include "m_mathgfx.h"
#include "m_pathfind.h"
#include "m_rgba.h"
#include "m_screen.h"
#include "m_scroll.h"
#include "m_text.h"
#include "m_wm.h"
#include "m_draw.h"

/* --------------------------------------------------------------------------- */

#define G_WIDE          0
#define G_WIDTH         0
#define G_HEIGHT        1
#define G_CENTER_X      2
#define G_X_CENTER      2
#define G_CENTER_Y      3
#define G_Y_CENTER      3
#define G_PITCH         4
#define G_DEPTH         5

#define B_CLEAR         0x00000001

/* Texture quality */
enum {
    Q_NEAREST = 0,
    Q_LINEAR,
    Q_BEST,
#ifdef USE_SDL2_GPU
    Q_MIPMAP,
#endif
};

/* --------------------------------------------------------------------------- */

/* Locals */
enum {
    COLLISION_RESERVED_TYPE_SCAN = 0,
    COLLISION_RESERVED_ID_SCAN,
    COLLISION_RESERVED_CONTEXT,
    PROCESS_ID,
    PROCESS_TYPE,
    STATUS,
    CTYPE,
    CNUMBER,
    COORDX,
    COORDY,
    ANGLE,
    GRAPHSIZE,
    GRAPHSIZEX,
    GRAPHSIZEY,
    FLAGS,
    REGIONID,
    RESOLUTION,
    XGRAPH,
    CLIPX,
    CLIPY,
    CLIPW,
    CLIPH,
    CSHAPE,
    CBOX_X,
    CBOX_Y,
    CBOX_WIDTH,
    CBOX_HEIGHT
};

/* Globals */
enum {
    SCROLLS = 0,
    MOUSEX,
    MOUSEY
};

/* --------------------------------------------------------------------------- */

extern DLVARFIXUP __bgdexport( libmod_gfx, locals_fixup )[];
extern DLVARFIXUP __bgdexport( libmod_gfx, globals_fixup )[];

extern void __bgdexport( libmod_gfx, process_exec_hook )( INSTANCE * r );
extern void __bgdexport( libmod_gfx, module_initialize )();
extern void __bgdexport( libmod_gfx, module_finalize )();

/* --------------------------------------------------------------------------- */

#endif
