/*
 *  Copyright (C) 2006-2019 SplinterGU (Fenix/BennuGD)
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

 /* ---------------------------------------------------------------------- */

#include <SDL.h>

#include "xctype.h"
#include "bgddl.h"
#include "dlvaracc.h"

/* ---------------------------------------------------------------------- */

#include "libmod_misc.h"

/* ---------------------------------------------------------------------- */
/* Son las variables que se desea acceder.                                */
/* El interprete completa esta estructura, si la variable existe.         */
/* (usada en tiempo de ejecucion)                                         */
DLVARFIXUP __bgdexport( libmod_misc, locals_fixup )[]  = {
    /* Nombre de variable local, offset al dato, tamano del elemento, cantidad de elementos */
    { "id"                                  , NULL, -1, -1 },
    { "reserved.process_type"               , NULL, -1, -1 },
    { "reserved.status"                     , NULL, -1, -1 },
    { "__proccess_reserved.id_scan"         , NULL, -1, -1 },
    { "__proccess_reserved.type_scan"       , NULL, -1, -1 },
    { "__proccess_reserved.context"         , NULL, -1, -1 },
    { "__proccess_reserved.signal_action"   , NULL, -1, -1 },

    { NULL                                  , NULL, -1, -1 }
};

/* ---------------------------------------------------------------------- */
/* Son las variables que se desea acceder.                                */
/* El interprete completa esta estructura, si la variable existe.         */
/* (usada en tiempo de ejecucion)                                         */

DLVARFIXUP __bgdexport( libmod_misc, globals_fixup)[] = {
    /* Nombre de variable global, puntero al dato, tamaño del elemento, cantidad de elementos */
    { "fileinfo.path"           , NULL, -1, -1 },
    { "fileinfo.name"           , NULL, -1, -1 },
    { "fileinfo.directory"      , NULL, -1, -1 },
    { "fileinfo.hidden"         , NULL, -1, -1 },
    { "fileinfo.readonly"       , NULL, -1, -1 },
    { "fileinfo.size"           , NULL, -1, -1 },
    { "fileinfo.created"        , NULL, -1, -1 },
    { "fileinfo.modified"       , NULL, -1, -1 },
    { "fileinfo.accessed"       , NULL, -1, -1 },
    { "fileinfo.statechg"       , NULL, -1, -1 },

    { "timer"                   , NULL, -1, -1 },

    { "regex_reg"               , NULL, -1, -1 },

    { NULL                      , NULL, -1, -1 }
};

/* ---------------------------------------------------------------------- */

/* ----------------------------------------------------------------- */

/* Bigest priority first execute
   Lowest priority last execute */

HOOK __bgdexport( libmod_misc, handler_hooks )[] = {
    { 100, libmod_misc_advance_timers   },
    {   0, NULL                         }
} ;

/* --------------------------------------------------------------------------- */

void __bgdexport( libmod_misc, module_initialize )() {
#ifndef TARGET_DINGUX_A320
    if ( !SDL_WasInit( SDL_INIT_TIMER ) ) SDL_InitSubSystem( SDL_INIT_TIMER );
#endif
}

/* --------------------------------------------------------------------------- */

void __bgdexport( libmod_misc, module_finalize )() {
#ifndef TARGET_DINGUX_A320
    if ( SDL_WasInit( SDL_INIT_TIMER ) ) SDL_QuitSubSystem( SDL_INIT_TIMER );
#endif
}

/* ----------------------------------------------------------------- */

void __bgdexport( libmod_misc, process_exec_hook )( INSTANCE * r ) {
    LOCQWORD( libmod_misc, r, TYPE_SCAN ) = 0;
    LOCQWORD( libmod_misc, r, ID_SCAN ) = 0;
    LOCQWORD( libmod_misc, r, CONTEXT ) = 0;
}

/* ---------------------------------------------------------------------- */
/* exports                                                                */
/* ---------------------------------------------------------------------- */

#include "libmod_misc_exports.h"

/* ---------------------------------------------------------------------- */
