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

#include "bgdrtm.h"
#include "bgddl.h"

#include "xstrings.h"

#include "libbggfx.h"
#include "libmod_gfx.h"

#include "file_div.h"

/* ---------------------------------------------------------------------- */

/* FPG functions */

int64_t libmod_gfx_load_fpg( INSTANCE * my, int64_t * params ) {
    int64_t r;
    r = gr_load_fpg( string_get( params[0] ) ) ;
    string_discard( params[0] ) ;
    return r ;
}

/* --------------------------------------------------------------------------- */

// int64_t libmod_gfx_save_fpg( INSTANCE * my, int64_t * params )
// {
//     int r;
//     r = gr_save_fpg( params[0], string_get( params[1] ) ) ;
//     string_discard( params[1] ) ;
//     return r ;
// }

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_unload_fpg( INSTANCE * my, int64_t * params ) {
    grlib_destroy( params[0] ) ;
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_fpg_exists( INSTANCE * my, int64_t * params ) {
    GRLIB * lib = grlib_get( params[0] );
    return lib == NULL ? 0 : 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_fpg_add( INSTANCE * my, int64_t * params ) {
    GRAPH * orig = bitmap_get( params[2], params[3] );
    GRAPH * dest ;

    if ( orig == NULL ) return 0;
    dest = bitmap_clone( orig ) ;
    dest->code = params[1] ;

    return grlib_add_map( params[0], dest );
}

 /* --------------------------------------------------------------------------- */

int64_t libmod_gfx_fpg_new( INSTANCE * my, int64_t * params ) {
    return grlib_new();
}

/* --------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------- */
