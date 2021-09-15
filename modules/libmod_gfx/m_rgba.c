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

#include <SDL.h>

#include "bgdrtm.h"
#include "bgddl.h"

#include "libbggfx.h"
#include "libmod_gfx.h"

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_get_rgb( INSTANCE * my, int64_t * params ) {
    Uint8 r, g, b;

    SDL_GetRGB( params[0], gPixelFormat, &r, &g, &b ) ;
    *( uint8_t * )( intptr_t )(params[1]) = ( uint8_t ) r;
    *( uint8_t * )( intptr_t )(params[2]) = ( uint8_t ) g;
    *( uint8_t * )( intptr_t )(params[3]) = ( uint8_t ) b;

    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_get_rgba( INSTANCE * my, int64_t * params ) {
    Uint8 r, g, b, a;

    SDL_GetRGBA( params[0], gPixelFormat, &r, &g, &b, &a ) ;
    *( uint8_t * )( intptr_t )(params[1]) = ( uint8_t ) r;
    *( uint8_t * )( intptr_t )(params[2]) = ( uint8_t ) g;
    *( uint8_t * )( intptr_t )(params[3]) = ( uint8_t ) b;
    *( uint8_t * )( intptr_t )(params[4]) = ( uint8_t ) a;

    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_get_rgb_map( INSTANCE * my, int64_t * params ) {
    Uint8 r, g, b;
#ifdef USE_SDL2
    GRAPH * map = bitmap_get( params[0], params[1] ) ;
    if ( !map->surface ) return 0;
    SDL_GetRGB( params[2], map->surface->format, &r, &g, &b ) ;
#endif
#ifdef USE_SDL2_GPU
    SDL_GetRGB( params[2], gPixelFormat, &r, &g, &b ) ;
#endif
    *( uint8_t * )( intptr_t )(params[3]) = ( uint8_t ) r;
    *( uint8_t * )( intptr_t )(params[4]) = ( uint8_t ) g;
    *( uint8_t * )( intptr_t )(params[5]) = ( uint8_t ) b;

    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_get_rgba_map( INSTANCE * my, int64_t * params ) {
    Uint8 r, g, b, a;
#ifdef USE_SDL2
    GRAPH * map = bitmap_get( params[0], params[1] ) ;
    if ( !map->surface ) return 0;
    SDL_GetRGBA( params[2], map->surface->format, &r, &g, &b, &a ) ;
#endif
#ifdef USE_SDL2_GPU
    SDL_GetRGBA( params[2], gPixelFormat, &r, &g, &b, &a ) ;
#endif
    *( uint8_t * )( intptr_t )(params[3]) = ( uint8_t ) r;
    *( uint8_t * )( intptr_t )(params[4]) = ( uint8_t ) g;
    *( uint8_t * )( intptr_t )(params[5]) = ( uint8_t ) b;
    *( uint8_t * )( intptr_t )(params[6]) = ( uint8_t ) a;
    return 1 ;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_rgb( INSTANCE * my, int64_t * params ) {
    return SDL_MapRGB( gPixelFormat, params[0], params[1], params[2] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_rgba( INSTANCE * my, int64_t * params ) {
    return SDL_MapRGBA( gPixelFormat, params[0], params[1], params[2], params[3] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_rgb_map( INSTANCE * my, int64_t * params ) {
#ifdef USE_SDL2
    GRAPH * map = bitmap_get( params[0], params[1] ) ;
    if ( !map->surface ) return 0;
    return SDL_MapRGB( map->surface->format, params[2], params[3], params[4] );
#endif
#ifdef USE_SDL2_GPU
    return SDL_MapRGB( gPixelFormat, params[2], params[3], params[4] );
#endif
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_rgba_map( INSTANCE * my, int64_t * params ) {
#ifdef USE_SDL2
    GRAPH * map = bitmap_get( params[0], params[1] ) ;
    if ( !map->surface ) return 0;
    return SDL_MapRGBA( map->surface->format, params[2], params[3], params[4], params[5] );
#endif
#ifdef USE_SDL2_GPU
    return SDL_MapRGBA( gPixelFormat, params[2], params[3], params[4], params[5] );
#endif
}

/* --------------------------------------------------------------------------- */
