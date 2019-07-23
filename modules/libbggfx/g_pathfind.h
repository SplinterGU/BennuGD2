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

#ifndef __G_PATHFIND_H
#define __G_PATHFIND_H

#include "bgdrtm.h"
#include "bgddl.h"

/* --------------------------------------------------------------------------- */

typedef struct _node {
    int x, y;
    int64_t f, g, h;
    int walkable;
    int opened;
    int closed;
    struct _node * parent;
    struct _node * next, * prev;
} NODE;

typedef struct {
    NODE * matrix;
    int w, h;
} GRID;

/* --------------------------------------------------------------------------- */

#define PF_DIAG     1

/* --------------------------------------------------------------------------- */

enum {
    PF_HEURISTIC_MANHATTAN = 0,
    PF_HEURISTIC_EUCLIDEAN,
    PF_HEURISTIC_OCTILE,
    PF_HEURISTIC_CHEBYSHEV
};

/* --------------------------------------------------------------------------- */

extern GRID * path_new( GRAPH * gr );
extern int64_t * path_find( GRID * grid, int startX, int startY, int endX, int endY, int options, int weight, int heuristic );
extern void path_destroy( GRID * grid );

/* --------------------------------------------------------------------------- */

#endif
