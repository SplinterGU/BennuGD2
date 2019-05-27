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

#include <SDL.h>
#include <SDL_syswm.h>

#include <stdlib.h>

#include "bgdrtm.h"
#include "bgddl.h"

#include "xstrings.h"

#include "libbggfx.h"
#include "libmod_gfx.h"

#include "dlvaracc.h"

/* --------------------------------------------------------------------------- */
/* Window Manager                                                              */
/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_set_title( INSTANCE * my, int64_t * params ) {
    gr_set_caption( ( char * )string_get( params[0] ) ) ;
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_set_icon( INSTANCE * my, int64_t * params ) {
    gr_set_icon( bitmap_get( params[0], params[1] ) );
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_hide_window( INSTANCE * my, int64_t * params ) {
    SDL_HideWindow( gWindow );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_show_window( INSTANCE * my, int64_t * params ) {
    SDL_ShowWindow( gWindow );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_minimize( INSTANCE * my, int64_t * params ) {
    SDL_MinimizeWindow( gWindow );
    return 1;
}

/* ---------------------------------------------------------------------- */

int64_t libmod_gfx_maximize( INSTANCE * my, int64_t * params ) {
    SDL_MaximizeWindow( gWindow );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_raise_window( INSTANCE * my, int64_t * params ) {
    SDL_RaiseWindow( gWindow );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_restore_window( INSTANCE * my, int64_t * params ) {
    SDL_RestoreWindow( gWindow );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_move_window( INSTANCE * my, int64_t * params ) {
    if ( full_screen ) return 0;

    SDL_SetWindowPosition( gWindow,
                           params[0],   // horizontal position
                           params[1]    // vertical position
                         );
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_get_window_pos( INSTANCE * my, int64_t * params ) {
    if ( full_screen ) return -1;

    int x, y;

    SDL_GetWindowPosition( gWindow, &x, &y );

    if ( params[0] ) *(( int64_t * )( params[0] ) ) = x;
    if ( params[1] ) *(( int64_t * )( params[1] ) ) = y;

    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_get_window_size( INSTANCE * my, int64_t * params ) {
    int w, h;

    SDL_GetWindowSize( gWindow, &w, &h );
    if ( params[0] ) *(( int64_t * )( params[0] ) ) = w;
    if ( params[1] ) *(( int64_t * )( params[1] ) ) = h;

    SDL_GetWindowMaximumSize( gWindow, &w, &h );
    if ( params[2] ) *(( int64_t * )( params[2] ) ) = w;
    if ( params[3] ) *(( int64_t * )( params[3] ) ) = h;

    SDL_GetWindowMinimumSize( gWindow, &w, &h );
    if ( params[4] ) *(( int64_t * )( params[4] ) ) = w;
    if ( params[5] ) *(( int64_t * )( params[5] ) ) = h;

    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_get_desktop_size( INSTANCE * my, int64_t * params ) {
    SDL_DisplayMode mode;

    if ( SDL_GetDesktopDisplayMode( 0, &mode ) < 0 ) return -1;

    if ( params[0] ) *(( int64_t * )( params[0] ) ) = mode.w;
    if ( params[1] ) *(( int64_t * )( params[1] ) ) = mode.h;

    return 1 ;
}

/* --------------------------------------------------------------------------- */
