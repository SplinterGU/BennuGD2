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

#ifndef __I_PROCDEF_ST_H
#define __I_PROCDEF_ST_H

#include "instance_st.h"

/* Process definitions, more simple that in the compiler */

typedef struct _procdef {
	uint8_t * pridata;
	uint8_t * pubdata;

	int64_t * code;

	int64_t exitcode;
	int64_t errorcode;

	int64_t * strings;
	int64_t * pubstrings;

	int64_t private_size;
	int64_t public_size;

	int64_t code_size;

	int64_t string_count;
	int64_t pubstring_count;

	int64_t params;
	int64_t id;
	int64_t type;
	int64_t flags;
	char * name;

    int64_t breakpoint;
} PROCDEF;

#define PROC_USES_FRAME 	0x01
#define PROC_USES_LOCALS	0x02
#define PROC_FUNCTION   	0x04
#define PROC_USES_PUBLICS   0x08

/* System functions */

typedef int64_t SYSFUNC (INSTANCE *, int64_t *);
typedef struct _sysproc {
	int64_t code;
	char * name;
	char * paramtypes;
	int64_t type;
	int64_t params;
	SYSFUNC * func;
	int64_t id;
} SYSPROC;

#endif
