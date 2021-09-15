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

#include "bgdrtm.h"
#include "bgddl.h"

#include "libbggfx.h"
#include "libmod_gfx.h"

#include "bgload.h"

/* --------------------------------------------------------------------------- */
/**
   int LOAD_FPG(STRING FICHERO, INT POINTER VARIABLE)
   Loads fpg file FICHERO on a separate thread
   VARIABLE is -2 while waiting, -1 on error, >=0 otherwise
 **/

int64_t libmod_gfx_bgload_fpg( INSTANCE * my, int64_t * params ) {
    bgload( gr_load_fpg, params );
    return 0 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_bgload_map( INSTANCE * my, int64_t * params ) {
    bgload( gr_load_img, params );
    return 0 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_bgload_fnt( INSTANCE * my, int64_t * params ) {
    bgload( gr_font_load, params );
    return 0 ;
}

/* --------------------------------------------------------------------------- */

/*
int64_t libmod_gfx_bgload_bdf( INSTANCE * my, int64_t * params ) {
    bgload( gr_load_bdf, params ) ;
    return 0 ;
}
*/

/* --------------------------------------------------------------------------- */
