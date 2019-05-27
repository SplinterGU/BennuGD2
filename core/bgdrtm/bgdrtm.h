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

#ifndef __BGDRTM_H
#define __BGDRTM_H

#include <stdint.h>
#include <inttypes.h>

#include <commons_defs.h>

#include "arrange.h"
#include "i_procdef.h"
#include "files.h"
#include "dcb.h"
#include "xctype.h"
#include "instance.h"

/* --------------------------------------------------------------------------- */

extern void * globaldata;
extern void * localdata;
extern int64_t local_size;
extern int64_t * localstr;
extern int64_t local_strings;

extern PROCDEF * procs;
extern PROCDEF * mainproc;
extern int64_t procdef_count;

extern char *appname;
extern char *appexename;
extern char *appexepath;
extern char *appexefullpath;

extern int64_t debug;          /* 1 if running in debug mode                    */

/* Trace */
extern int64_t debug_mode;

extern int64_t exit_value;
extern int64_t must_exit;

extern int64_t force_debug;
extern int64_t debug_next;

extern int64_t trace_sentence;
extern INSTANCE * trace_instance;

/* --------------------------------------------------------------------------- */

extern int strncmpi( char * str1, char * str2, int sz );

extern int dcb_load( const char * filename );
extern int dcb_load_from( file * fp, char * dcbname, int offset );

extern char * getid_name( int64_t code );

extern void mnemonic_dump( int64_t i, int64_t param );

/* --------------------------------------------------------------------------- */

extern void bgdrtm_entry( int argc, char * argv[] );
extern void bgdrtm_exit( int n );

/* --------------------------------------------------------------------------- */

#if defined( TARGET_GP2X_WIZ ) || defined( TARGET_CAANOO )

#define TIMER_BASE3 0x1980
#define TIMER_REG(x) __bgdrtm_memregl[(TIMER_BASE3 + x) >> 2]

extern volatile unsigned long *__bgdrtm_memregl;
extern int __bgdrtm_memdev;

extern void bgdrtm_ptimer_init(void);
extern unsigned long bgdrtm_ptimer_get_ticks_us(void);
extern void bgdrtm_ptimer_cleanup(void);

#endif

/* --------------------------------------------------------------------------- */

#endif
