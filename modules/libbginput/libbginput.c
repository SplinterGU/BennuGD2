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

#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>

#include "xctype.h"
#include "bgddl.h"
#include "dlvaracc.h"

#include "libbginput.h"

/* ---------------------------------------------------------------------- */
/* Son las variables que se desea acceder.                           */
/* El interprete completa esta estructura, si la variable existe.    */
/* (usada en tiempo de ejecucion)                                    */

DLVARFIXUP  __bgdexport( libbginput, globals_fixup )[] = {
    /* Nombre de variable global, puntero al dato, tama�o del elemento, cantidad de elementos */
    { "keyboard.shift_status"                   , NULL, -1, -1 },
    { "keyboard.ascii"                          , NULL, -1, -1 },
    { "keyboard.scan_code"                      , NULL, -1, -1 },

    { "mouse.x"                                 , NULL, -1, -1 },
    { "mouse.y"                                 , NULL, -1, -1 },
    { "mouse.z"                                 , NULL, -1, -1 },
    { "mouse.file"                              , NULL, -1, -1 },
    { "mouse.graph"                             , NULL, -1, -1 },
    { "mouse.angle"                             , NULL, -1, -1 },
    { "mouse.size"                              , NULL, -1, -1 },
    { "mouse.size_x"                            , NULL, -1, -1 },
    { "mouse.size_y"                            , NULL, -1, -1 },
    { "mouse.flags"                             , NULL, -1, -1 },
    { "mouse.region"                            , NULL, -1, -1 },
    { "mouse.left"                              , NULL, -1, -1 },
    { "mouse.middle"                            , NULL, -1, -1 },
    { "mouse.right"                             , NULL, -1, -1 },
    { "mouse.x1"                                , NULL, -1, -1 },
    { "mouse.x2"                                , NULL, -1, -1 },
    { "mouse.wheelup"                           , NULL, -1, -1 },
    { "mouse.wheeldown"                         , NULL, -1, -1 },
    { "mouse.wheelright"                        , NULL, -1, -1 },
    { "mouse.wheelleft"                         , NULL, -1, -1 },
    { "mouse.clip.x"                            , NULL, -1, -1 },
    { "mouse.clip.y"                            , NULL, -1, -1 },
    { "mouse.clip.w"                            , NULL, -1, -1 },
    { "mouse.clip.h"                            , NULL, -1, -1 },
    { "mouse.center.x"                          , NULL, -1, -1 },
    { "mouse.center.y"                          , NULL, -1, -1 },
    { "mouse.alpha"                             , NULL, -1, -1 },
    { "mouse.color_r"                           , NULL, -1, -1 },
    { "mouse.color_g"                           , NULL, -1, -1 },
    { "mouse.color_b"                           , NULL, -1, -1 },
    { "mouse.blendmode"                         , NULL, -1, -1 },
    { "mouse.custom_blendmode"                  , NULL, -1, -1 },
    { "mouse.custom_blendmode.src_rgb"          , NULL, -1, -1 },
    { "mouse.custom_blendmode.dst_rgb"          , NULL, -1, -1 },
    { "mouse.custom_blendmode.src_alpha"        , NULL, -1, -1 },
    { "mouse.custom_blendmode.dst_alpha"        , NULL, -1, -1 },
    { "mouse.custom_blendmode.equation_rgb"     , NULL, -1, -1 },
    { "mouse.custom_blendmode.equation_alpha"   , NULL, -1, -1 },
    { "mouse.shader"                            , NULL, -1, -1 },
    { "mouse.shader_params"                     , NULL, -1, -1 },

    { "touches"                                 , NULL, -1, -1 },

    { "sensors"                                 , NULL, -1, -1 },

    { NULL                                      , NULL, -1, -1 }
};

/* ---------------------------------------------------------------------- */

/* Bigest priority first execute
   Lowest priority last execute */

HOOK __bgdexport( libbginput, handler_hooks )[] =
{
    { 4900, process_key_events      },
    { 4890, process_mouse_events    },
    { 4880, process_touch_events    },
    { 4870, process_joy_events      },
    { 4860, process_sensors_events  },
    {    0, NULL                    }
} ;

/* ---------------------------------------------------------------------- */
/* Funciones de inicializacion del modulo/plugin                     */

void __bgdexport( libbginput, module_initialize )() {
    key_init();
    mouse_init();
    touch_init();
    joy_init();
    sensors_init();
}

/* ---------------------------------------------------------------------- */

void __bgdexport( libbginput, module_finalize )() {
    sensors_exit();
    joy_exit();
//    touch_exit();
//    mouse_exit();
    key_exit();
}

/* --------------------------------------------------------------------------- */
/* exports                                                                     */
/* --------------------------------------------------------------------------- */

#include "libbginput_exports.h"

/* --------------------------------------------------------------------------- */
