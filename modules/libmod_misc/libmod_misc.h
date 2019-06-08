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

#ifndef __LIBMOD_MISC_H
#define __LIBMOD_MISC_H

/* ----------------------------------------------------------------- */

#include "bgddl.h"

#include "b_crypt.h"

#include "m_crypt.h"
#include "m_dir.h"
#include "m_file.h"
#include "m_math.h"
#include "m_mem.h"
#include "m_proc.h"
#include "m_rand.h"
#include "m_regex.h"
#include "m_say.h"
#include "m_sort.h"
#include "m_string.h"
#include "m_sys.h"
#include "m_time.h"

/* ----------------------------------------------------------------- */

enum {
    PROCESS_ID = 0,
    PROCESS_TYPE,
    STATUS,
    ID_SCAN,
    TYPE_SCAN,
    _CONTEXT,
    SIGNAL_ACTION
} ;

enum {
    FILE_PATH = 0,
    FILE_NAME,
    FILE_DIRECTORY,
    FILE_HIDDEN,
    FILE_READONLY,
    FILE_SIZE,
    FILE_CREATED,
    FILE_MODIFIED,
    FILE_ACCESSED,
    FILE_STATECHG,

    TIMER,

    REGEX_REG

} ;

/* ----------------------------------------------------------------- */

extern DLVARFIXUP __bgdexport( libmod_misc, globals_fixup)[];
extern DLVARFIXUP __bgdexport( libmod_misc, locals_fixup )[];

/* ----------------------------------------------------------------- */

#endif
