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

#ifndef __LIBBGINPUT_H
#define __LIBBGINPUT_H

/* ---------------------------------------------------------------------- */

#include "i_key.h"
#include "i_mouse.h"
#include "i_joy.h"
#include "i_touch.h"
#include "i_sensors.h"

/* ---------------------------------------------------------------------- */

#ifdef __LIBBGINPUT

#if !defined(__BGDC__)

enum {
    SHIFTSTATUS = 0,
    ASCII,
    SCANCODE,

    MOUSEX,
    MOUSEY,
    MOUSEZ,
    MOUSEFILE,
    MOUSEGRAPH,
    MOUSEANGLE,
    MOUSESIZE,
    MOUSESIZEX,
    MOUSESIZEY,
    MOUSEFLAGS,
    MOUSEREGION,
    MOUSELEFT,
    MOUSEMIDDLE,
    MOUSERIGHT,
    MOUSEX1,
    MOUSEX2,
    MOUSEWHEELUP,
    MOUSEWHEELDOWN,
    MOUSEWHEELRIGHT,
    MOUSEWHEELLEFT,
    MOUSECLIPX,
    MOUSECLIPY,
    MOUSECLIPW,
    MOUSECLIPH,
    MOUSECENTERX,
    MOUSECENTERY,
    MOUSEALPHA,
    MOUSECOLOR_R,
    MOUSECOLOR_G,
    MOUSECOLOR_B,
    MOUSE_BLEND_MODE,
    MOUSE_CUSTOM_BLEND_MODE,
    MOUSE_SRC_RGB,
    MOUSE_DST_RGB,
    MOUSE_SRC_ALPHA,
    MOUSE_DST_ALPHA,
    MOUSE_EQUATION_RGB,
    MOUSE_EQUATION_ALPHA,
    MOUSE_SHADER_ID,
    MOUSE_SHADER_PARAMS,

    TOUCHES,

    SENSORS
};

#endif

/* --------------------------------------------------------------------------- */

extern DLVARFIXUP __bgdexport( libbginput, globals_fixup )[];
// extern DLVARFIXUP __bgdexport( libbginput, locals_fixup )[];

#endif

/* ---------------------------------------------------------------------- */

#endif
