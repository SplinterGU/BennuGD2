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

#ifndef __LIBBGFGX_H
#define __LIBBGFGX_H

/* --------------------------------------------------------------------------- */

#define C_SCREEN            0

/* --------------------------------------------------------------------------- */

#ifdef __LIBBGFGX

/* --------------------------------------------------------------------------- */

/* Globals */

enum {
    GRAPH_MODE = 0,
    FULL_SCREEN,
    SCALE_RESOLUTION,
    SCALE_RESOLUTION_ASPECTRATIO,
    FPS,
    SPEED_GAUGE,
    FRAME_TIME,
    FADING,

    EXIT_STATUS,
    WINDOW_STATUS,
    FOCUS_STATUS,
    MOUSE_STATUS,

    TEXTZ,
    TEXT_FLAGS,
    TEXT_ALPHA,
    TEXT_COLORR,
    TEXT_COLORG,
    TEXT_COLORB,

    /* backgound */
    BACKGROUND_FILE,
    BACKGROUND_GRAPH,
    BACKGROUND_COLORR,
    BACKGROUND_COLORG,
    BACKGROUND_COLORB,
    BACKGROUND_FLAGS,
    BACKGROUND_ANGLE,
    BACKGROUND_SIZE,
    BACKGROUND_SIZEX,
    BACKGROUND_SIZEY,

    /*Scroll*/
    SCROLLS
};


enum {
    CTYPE = 0,
    CNUMBER,
    COORDX,
    COORDY,
    COORDZ,
    FILEID,
    GRAPHID,
    GRAPHSIZE,
    ANGLE,
    FLAGS,
    REGIONID,
    RESOLUTION,
    GRAPHSIZEX,
    GRAPHSIZEY,
    XGRAPH,
    OBJECTID,
    GRAPHPTR,
    XGRAPH_FLAGS,
    STATUS,
    PROCESS_ID,
    RENDER_FILEID,
    RENDER_GRAPHID,
    CLIPX,
    CLIPY,
    CLIPW,
    CLIPH,
    ALPHA,
    COLORR,
    COLORG,
    COLORB
};

/* --------------------------------------------------------------------------- */

#endif

/* --------------------------------------------------------------------------- */

#include "bgdcore.h"
#include "g_bitmap.h"
#include "g_blit.h"
#include "g_grlib.h"
#include "g_instance.h"
#include "g_object.h"
#include "g_video.h"
#include "g_regions.h"
#include "g_frame.h"
#include "g_wm.h"
#include "g_font.h"
#include "g_text.h"
#include "g_clear.h"
#include "g_pixel.h"
#include "g_fade.h"
#include "g_scroll.h"

/* --------------------------------------------------------------------------- */

extern DLVARFIXUP __bgdexport( libbggfx, globals_fixup )[];
extern DLVARFIXUP __bgdexport( libbggfx, locals_fixup )[];

/* --------------------------------------------------------------------------- */

#endif
