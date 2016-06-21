/*
 *  Copyright (C) 2006-2016 SplinterGU (Fenix/BennuGD)
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

#include "bgdrtm.h"
#include "bgddl.h"

#include "libbggfx.h"
#include "libmod_gfx.h"

/* --------------------------------------------------------------------------- */

static SDL_PixelFormat * pixformat = NULL;

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_get_rgb( INSTANCE * my, int64_t * params ) {
    Uint8 r, g, b;

    if ( !pixformat ) pixformat = SDL_AllocFormat( SDL_PIXELFORMAT_ARGB8888 );

    SDL_GetRGB( params[0], pixformat, &r, &g, &b ) ;
    *( uint8_t * )(params[1]) = ( uint8_t ) r;
    *( uint8_t * )(params[2]) = ( uint8_t ) g;
    *( uint8_t * )(params[3]) = ( uint8_t ) b;

    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_get_rgba( INSTANCE * my, int64_t * params ) {
    Uint8 r, g, b, a;

    if ( !pixformat ) pixformat = SDL_AllocFormat( SDL_PIXELFORMAT_ARGB8888 );

    SDL_GetRGBA( params[0], pixformat, &r, &g, &b, &a ) ;
    *( uint8_t * )(params[1]) = ( uint8_t ) r;
    *( uint8_t * )(params[2]) = ( uint8_t ) g;
    *( uint8_t * )(params[3]) = ( uint8_t ) b;
    *( uint8_t * )(params[4]) = ( uint8_t ) a;

    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_rgb( INSTANCE * my, int64_t * params ) {
    if ( !pixformat ) pixformat = SDL_AllocFormat( SDL_PIXELFORMAT_ARGB8888 );
    return SDL_MapRGB( pixformat, params[0], params[1], params[2] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_rgba( INSTANCE * my, int64_t * params ) {
    if ( !pixformat ) pixformat = SDL_AllocFormat( SDL_PIXELFORMAT_ARGB8888 );
    return SDL_MapRGBA( pixformat, params[0], params[1], params[2], params[3] );
}
