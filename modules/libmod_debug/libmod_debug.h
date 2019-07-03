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

#ifndef __LIBMOD_DEBUG_H
#define __LIBMOD_DEBUG_H

/* --------------------------------------------------------------------------- */

#include "bgdrtm.h"
#include "bgddl.h"

#include "libbginput.h"
#include "m_debug.h"

/* --------------------------------------------------------------------------- */

/* Locals */
enum {
    PROCESS_ID = 0,
    FATHER,
    BIGBRO,
    SON,
    STATUS
};

/* Globals */
enum {
    SHIFTSTATUS = 0
};

/* --------------------------------------------------------------------------- */

extern DLVARFIXUP __bgdexport( libmod_debug, locals_fixup )[];
extern DLVARFIXUP __bgdexport( libmod_debug, globals_fixup )[];

extern void __bgdexport( libmod_debug, process_exec_hook )( INSTANCE * r );
extern void __bgdexport( libmod_debug, module_initialize )();
extern void __bgdexport( libmod_debug, module_finalize )();

/* --------------------------------------------------------------------------- */

#endif
