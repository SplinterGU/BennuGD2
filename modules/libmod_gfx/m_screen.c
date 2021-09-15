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

#include "dlvaracc.h"

/* --------------------------------------------------------------------------- */
/* Screen                                                                      */
/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_define_region( INSTANCE * my, int64_t * params ) {
    region_define( params[0], params[1], params[2], params[3], params[4] ) ;
    return params[0] ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_out_region( INSTANCE * my, int64_t * params ) {
    INSTANCE * proc = instance_get( params[0] ) ;
    int64_t region = params[1] ;
    REGION bbox ;
    GRAPH * gr ;

    if ( region < 0 || region >= MAX_REGIONS || !proc ) return 0 ;

    gr = instance_graph( proc ) ;
    if ( !gr ) return 0 ;

    instance_get_bbox( proc, gr, &bbox );

    if ( LOCQWORD( libmod_gfx, proc, CTYPE ) == C_SCROLL ) {
        scrolldata  * scroll;
        int i;

        int64_t cnumber = LOCQWORD( libmod_gfx, proc, CNUMBER );
        if ( !cnumber ) cnumber = 0xFFFFFFFF ;

        for ( i = 0 ; i < MAX_SCROLLS ; i++ ) {
            if ( /*scroll &&*/ scrolls[i].active && ( cnumber & ( 1 << i ) ) ) {
                bbox.x  -= scrolls[i].posx0 ;
                bbox.y  -= scrolls[i].posy0 ;
                bbox.x2 -= scrolls[i].posx0 ;
                bbox.y2 -= scrolls[i].posy0 ;
                break;
            }
        }
    }

    return region_is_out( &regions[region], &bbox ) ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_get_screen( INSTANCE * my, int64_t * params ) {
    GRAPH * capture = g_get_screen();
    if ( capture ) return capture->code;
    return 0;
}

/* --------------------------------------------------------------------------- */
/* Funciones de inicializacion y carga                                         */
/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_set_mode( INSTANCE * my, int64_t * params ) {
    return gr_set_mode( params[0], params[1], 0 ) ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_set_mode_extended( INSTANCE * my, int64_t * params ) {
    return gr_set_mode( params[0], params[1], params[2] ) ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_set_fps( INSTANCE * my, int64_t * params ) {
    gr_set_fps( params[0], params[1] ) ;
    return params[0];
}

/* --------------------------------------------------------------------------- */
