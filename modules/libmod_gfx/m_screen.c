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
//        SCROLL_EXTRA_DATA * data;
        scrolldata  * scroll;
        int i;

        int64_t cnumber = LOCQWORD( libmod_gfx, proc, CNUMBER );
        if ( !cnumber ) cnumber = 0xFFFFFFFF ;

        for ( i = 0 ; i < MAX_SCROLLS ; i++ ) {
//            data = &(( SCROLL_EXTRA_DATA * ) &GLOQWORD( libmod_gfx, SCROLLS ) )[ i ] ;
//            scroll = ( scrolldata  * ) ( intptr_t ) data->reserved[0];

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
#if 0
static int get_sdl_flags( int flags ) {
    int sdl_flags = SDL_HWPALETTE;

    sdl_flags |= ( flags & MODE_FULLSCREEN ) ? SDL_FULLSCREEN : 0 ;
    sdl_flags |= ( flags & MODE_DOUBLEBUFFER ) ? SDL_DOUBLEBUF : 0 ;
    sdl_flags |= ( flags & MODE_HARDWARE ) ? SDL_HWSURFACE : SDL_SWSURFACE ;
    sdl_flags |= ( flags & MODE_FRAMELESS ) ? SDL_NOFRAME : 0 ;

    return sdl_flags;
}

/* --------------------------------------------------------------------------- */

/*
Return a pointer to an array of available screen dimensions for the given format and video flags,
sorted largest to smallest.

Returns NULL if there are no dimensions available for a particular format,
or -1 if any dimension is okay for the given format.
*/

int64_t libmod_gfx_list_modes( INSTANCE * my, int64_t * params ) {
    SDL_Rect **modes;
    SDL_PixelFormat vfmt;
    int sdl_flags = get_sdl_flags( params[1] );
    int depth = params[0];
    int i, n;
    static int * available_modes = NULL ;

    if ( !depth ) depth = ( params[1] & MODE_32BITS ) ? 32 : (( params[1] & MODE_16BITS ) ? 16 : 8 );

    vfmt.BitsPerPixel = depth ;

    /* Get available fullscreen/hardware modes */
    modes = SDL_ListModes( params[0] ? &vfmt : NULL, sdl_flags );

    if ( modes == ( SDL_Rect ** ) 0 ) return 0; /* No video modes available for this criteria */
    if ( modes == ( SDL_Rect ** ) -1 ) return -1; /* Any video mode for this criteria */

    n = 0;
    for ( i = 0; modes[i]; ++i ) ++n ;

    available_modes = realloc( available_modes, ( 1 + n ) * sizeof( int ) * 2 );
    if ( !available_modes ) return -2;

    for ( i = 0; modes[i]; ++i ) {
        available_modes[i*2  ] = modes[i]->w;
        available_modes[i*2+1] = modes[i]->h;
    }
    available_modes[i*2  ] = 0;
    available_modes[i*2+1] = 0;

    return ( int )available_modes;
}

/* --------------------------------------------------------------------------- */

/*
   returns 0 if the requested mode is not supported under any bit depth,
   or returns the bits-per-pixel of the closest available
   mode with the given width, height and requested flags

   params:
        height,width,depth,flags

*/

int64_t libmod_gfx_mode_is_ok( INSTANCE * my, int64_t * params ) {
    int sdl_flags = get_sdl_flags( params[3] );
    int depth = params[2];

    if ( !depth ) depth = ( params[3] & MODE_32BITS ) ? 32 : (( params[3] & MODE_16BITS ) ? 16 : 8 );

    return ( SDL_VideoModeOK( params[0], params[1], depth, sdl_flags ) );
}
#endif
