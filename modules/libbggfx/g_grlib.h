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

#ifndef __G_GRLIB_H
#define __G_GRLIB_H

#define MAXLIBS 1024

typedef struct _grlib {
    GRAPH ** maps;
    int64_t map_reserved;
    char name[ 64 ];
} GRLIB;

extern GRLIB * syslib;

extern int64_t grlib_newid();
extern GRAPH * bitmap_get( int64_t libid, int64_t mapcode );
extern GRLIB * grlib_get( int64_t libid );
extern void grlib_init();
extern int64_t grlib_new();
extern void grlib_destroy( int64_t libid );
extern int64_t grlib_add_map( int64_t libid, GRAPH * map );
extern int64_t grlib_unload_map( int64_t libid, int64_t mapcode );
extern GRAPH * bitmap_new_syslib( int64_t w, int64_t h );

#endif
