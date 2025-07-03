/*
 *  Copyright (C) SplinterGU (Fenix/BennuGD) (Since 2006)
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


#ifndef __M_SYS_H
#define __M_SYS_H

/* ---------------------------------------------------------------------- */

#ifndef _P_WAIT
#define _P_WAIT     0
#endif

#ifndef _P_NOWAIT
#define _P_NOWAIT   1
#endif

/*
#define _P_OVERLAY  2
#define _OLD_P_OVERLAY  _P_OVERLAY
#define _P_NOWAITO  3
#define _P_DETACH   4
*/

/* ----------------------------------------------------------------- */

extern int64_t libmod_misc_sys_exec( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_sys_getenv( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_sys_get_pref_language( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_sys_set_process_dpi_aware( INSTANCE * my, int64_t * params );

#endif
