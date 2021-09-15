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

/* --------------------------------------------------------------------------- */

#include <stdlib.h>

#include "bgdrtm.h"
#include "bgddl.h"

#include "libbggfx.h"
#include "libmod_gfx.h"

#include "m_pathfind.h"

/* --------------------------------------------------------------------------- */
/* Funciones de búsqueda de caminos */

int64_t libmod_gfx_path_new( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) ( intptr_t ) path_new( bitmap_get( ( int ) params[0], ( int ) params[1] ) );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_path_destroy( INSTANCE * my, int64_t * params ) {
    path_destroy( ( GRID * ) ( intptr_t ) params[0] );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_path_find( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) ( intptr_t ) path_find( ( GRID * ) ( intptr_t ) params[0], ( int ) params[1], ( int ) params[2], ( int ) params[3], ( int ) params[4], ( int ) params[5], 1, PF_HEURISTIC_MANHATTAN );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_path_find2( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) ( intptr_t ) path_find( ( GRID * ) ( intptr_t ) params[0], ( int ) params[1], ( int ) params[2], ( int ) params[3], ( int ) params[4], ( int ) params[5], ( int ) params[6], ( int ) params[7] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_path_free_results( INSTANCE * my, int64_t * params ) {
    free( ( void * ) ( intptr_t ) params[0] );
    return 1;
}

/* --------------------------------------------------------------------------- */
