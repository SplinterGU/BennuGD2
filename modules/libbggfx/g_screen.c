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

#include "g_bitmap.h"
#include "g_grlib.h"

#include "libbggfx.h"

/* --------------------------------------------------------------------------- */

GRAPH * g_get_screen( void ) {
    SDL_Surface * surface = NULL;
    GRAPH * bitmap = NULL;
#ifdef USE_SDL2
    surface = SDL_CreateRGBSurface(0, renderer_width, renderer_height, gPixelFormat->BitsPerPixel, gPixelFormat->Rmask, gPixelFormat->Gmask, gPixelFormat->Bmask, 0 /* Force alpha to opaque */ );
    SDL_SetColorKey( surface, SDL_FALSE, 0 );

    if ( surface ) {
        SDL_Rect rect;

        rect.x = rect.y = 0;
        rect.w = renderer_width;
        rect.h = renderer_height;

        int r = SDL_RenderReadPixels( gRenderer,
                                      &rect,
                                      0, /* format */
                                      surface->pixels,
                                      surface->pitch );
    }
#endif
#ifdef USE_SDL2_GPU
    surface = GPU_CopySurfaceFromTarget( gRenderer );
    /* Convert for remove alpha channel */
    SDL_Surface * surfaceRGB = NULL;
    SDL_PixelFormat * fmt = SDL_AllocFormat( SDL_PIXELFORMAT_RGB888 );
    surfaceRGB = SDL_ConvertSurface( surface, fmt, 0 );
    SDL_FreeSurface( surface );
    SDL_FreeFormat( fmt );
    surface = surfaceRGB;
#endif
    if ( surface ) {
        bitmap = bitmap_new( 0, 0, 0, surface );
        SDL_FreeSurface( surface );
        if ( bitmap ) {
            bitmap->code = bitmap_next_code();
            grlib_add_map( 0, bitmap );
            return bitmap ;
        }
    }
    return NULL;

}

/* --------------------------------------------------------------------------- */
