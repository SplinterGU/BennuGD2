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
#include <string.h>
#include <math.h>
#include <time.h>

#include "bgddl.h"

#include "xctype.h"

#include "libmod_debug.h"

/* --------------------------------------------------------------------------- */

DLVARFIXUP __bgdexport( libmod_debug, locals_fixup )[] = {

    { "id"                              , NULL, -1, -1 },
    { "father"                          , NULL, -1, -1 },
    { "bigbro"                          , NULL, -1, -1 },
    { "son"                             , NULL, -1, -1 },
    { "reserved.status"                 , NULL, -1, -1 },

    { NULL                              , NULL, -1, -1 }
};

/* --------------------------------------------------------------------------- */

DLVARFIXUP __bgdexport( libmod_debug, globals_fixup )[] = {
    /* Nombre de variable global, puntero al dato, tama�o del elemento, cantidad de elementos */
    { "keyboard.shift_status"           , NULL, -1, -1 },

    { NULL                              , NULL, -1, -1 }
};

/* --------------------------------------------------------------------------- */
/* exports                                                                     */
/* --------------------------------------------------------------------------- */

/* Bigest priority first execute
   Lowest priority last execute */

HOOK __bgdexport( libmod_debug, handler_hooks )[] = {
    { 1000, m_debug_frame_complete_hook     },

    {    0, NULL                            }
} ;

/* --------------------------------------------------------------------------- */

void __bgdexport( libmod_debug, process_exec_hook )( INSTANCE * r ) {
    m_debug_process_exec_hook( r );

}

/* --------------------------------------------------------------------------- */

void __bgdexport( libmod_debug, module_initialize )() {
    m_debug_init();
}

/* --------------------------------------------------------------------------- */

void __bgdexport( libmod_debug, module_finalize )() {
}

#include "libmod_debug_exports.h"

/* --------------------------------------------------------------------------- */
