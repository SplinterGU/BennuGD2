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

#ifndef __VARSPACE_H
#define __VARSPACE_H

#include <stdint.h>

#include "typedef.h"

/* A VARSPACE is a variable definition zone */

typedef struct _varspace {
	struct _variable * vars;
	int64_t	size;
	int64_t	count;
	int64_t	reserved;
	int64_t	last_offset;
	int64_t * stringvars;
	int64_t	stringvar_reserved;
	int64_t	stringvar_count;
} VARSPACE;

typedef struct _variable {
	TYPEDEF type;
	int64_t	code;
	int64_t	offset;
} VARIABLE;

extern VARSPACE * varspace_new();
extern void       varspace_alloc(VARSPACE * n, int64_t count);
extern void       varspace_init(VARSPACE * n);
extern void       varspace_add(VARSPACE * n, VARIABLE v);
extern VARIABLE * varspace_search(VARSPACE * n, int64_t code);
extern void       varspace_dump(VARSPACE * n, int64_t indent);
extern void       varspace_destroy(VARSPACE * n);
extern void	      varspace_varstring(VARSPACE * n, int64_t offset);

/* Global and local data */

extern VARSPACE global;
extern VARSPACE local;

#endif
