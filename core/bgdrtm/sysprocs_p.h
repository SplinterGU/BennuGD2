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

/*
 * FILE        : sysprocs_p.h
 * DESCRIPTION : Defines prototipes for internal system functions
 */

#ifndef __SYSPROCS_P_H
#define __SYSPROCS_P_H

#include "sysprocs_st.h"

/* ---------------------------------------------------------------------- */

extern char * sysproc_name( int64_t code ) ;

/* ---------------------------------------------------------------------- */

extern HOOK * handler_hook_list ;
extern int64_t handler_hook_allocated ;
extern int64_t handler_hook_count ;

extern INSTANCE_HOOK * instance_pre_execute_hook_list ;
extern int64_t instance_pre_execute_hook_allocated ;
extern int64_t instance_pre_execute_hook_count ;

extern INSTANCE_HOOK * instance_pos_execute_hook_list ;
extern int64_t instance_pos_execute_hook_allocated ;
extern int64_t instance_pos_execute_hook_count ;

extern INSTANCE_HOOK * instance_create_hook_list ;
extern int64_t instance_create_hook_allocated ;
extern int64_t instance_create_hook_count ;

extern INSTANCE_HOOK * instance_destroy_hook_list ;
extern int64_t instance_destroy_hook_allocated ;
extern int64_t instance_destroy_hook_count ;

extern INSTANCE_HOOK * process_exec_hook_list ;
extern int64_t process_exec_hook_allocated ;
extern int64_t process_exec_hook_count ;

extern FN_HOOK * module_initialize_list ;
extern int64_t module_initialize_allocated ;
extern int64_t module_initialize_count ;

extern FN_HOOK * module_finalize_list ;
extern int64_t module_finalize_allocated ;
extern int64_t module_finalize_count ;

/* ---------------------------------------------------------------------- */

#endif
