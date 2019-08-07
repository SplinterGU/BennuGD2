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

#include <stdlib.h>

#include "bgdrtm.h"
#include "bgddl.h"

#include "libbggfx.h"
#include "libmod_gfx.h"

#include "fmath.h"
#include "bgload.h"

#include "file_div.h"
#include "m_pathfind.h"

#include "dlvaracc.h"

/* --------------------------------------------------------------------------- */

DLVARFIXUP __bgdexport( libmod_gfx, locals_fixup )[] = {
    /* Nombre de variable local, offset al dato, tama�o del elemento, cantidad de elementos */
    { "__collision_reserved.type_scan"  , NULL, -1, -1 },
    { "__collision_reserved.id_scan"    , NULL, -1, -1 },
    { "__collision_reserved.context"    , NULL, -1, -1 },

    { "id"                              , NULL, -1, -1 },
    { "reserved.process_type"           , NULL, -1, -1 },
    { "reserved.status"                 , NULL, -1, -1 },

    { "ctype"                           , NULL, -1, -1 },
    { "cnumber"                         , NULL, -1, -1 },

    { "x"                               , NULL, -1, -1 },
    { "y"                               , NULL, -1, -1 },

    { "angle"                           , NULL, -1, -1 },
    { "size"                            , NULL, -1, -1 },
    { "size_x"                          , NULL, -1, -1 },
    { "size_y"                          , NULL, -1, -1 },
    { "flags"                           , NULL, -1, -1 },

    { "region"                          , NULL, -1, -1 },
    { "resolution"                      , NULL, -1, -1 },
    { "xgraph"                          , NULL, -1, -1 },

    { "clip_x"                          , NULL, -1, -1 },
    { "clip_y"                          , NULL, -1, -1 },
    { "clip_w"                          , NULL, -1, -1 },
    { "clip_h"                          , NULL, -1, -1 },

    { "cshape"                          , NULL, -1, -1 },
    { "cbox.x"                          , NULL, -1, -1 },
    { "cbox.y"                          , NULL, -1, -1 },
    { "cbox.w"                          , NULL, -1, -1 },
    { "cbox.h"                          , NULL, -1, -1 },

    { "collision_info.collider_cbox"    , NULL, -1, -1 },
    { "collision_info.collided_id"      , NULL, -1, -1 },
    { "collision_info.collided_cbox"    , NULL, -1, -1 },

    { NULL                              , NULL, -1, -1 }
};

/* --------------------------------------------------------------------------- */

DLVARFIXUP __bgdexport( libmod_gfx, globals_fixup )[] = {
    /* Nombre de variable global, puntero al dato, tama�o del elemento, cantidad de elementos */
    { "scroll"                          , NULL, -1, -1 },
    { "mouse.x"                         , NULL, -1, -1 },
    { "mouse.y"                         , NULL, -1, -1 },

    { NULL                              , NULL, -1, -1 }
};

/* --------------------------------------------------------------------------- */

void __bgdexport( libmod_gfx, process_exec_hook )( INSTANCE * r ) {
    #include "m_collision_process_exec_hook.h"
}

/* --------------------------------------------------------------------------- */
/* exports                                                                     */
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/* Funciones de inicializacion del modulo/plugin                               */

void __bgdexport( libmod_gfx, module_initialize )() {
    #include "m_map_initialize.h"
}

/* --------------------------------------------------------------------------- */

void __bgdexport( libmod_gfx, module_finalize )() {
    #include "m_map_finalize.h"
}

/* --------------------------------------------------------------------------- */

#include "libmod_gfx_exports.h"

/* --------------------------------------------------------------------------- */
