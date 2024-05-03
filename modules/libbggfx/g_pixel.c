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

#include <stdio.h>

#include "libbggfx.h"

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : gr_get_pixel
 *
 *  Read a pixel from a bitmap
 *
 *  PARAMS :
 *      dest            Destination bitmap
 *      x, y            Pixel coordinates
 *
 *  RETURN VALUE :
 *      1, 8 or 16-bit integer with the pixel value
 *
 */
#if 0
int64_t gr_get_pixel( GRAPH * gr, int64_t x, int64_t y ) {
    if ( !gr ) return -1;

    if ( x < 0 || y < 0 ) return -1;

    if ( gr_create_image_for_graph( gr ) ) return -1;

    if ( !gr->tex ) return -1;

#ifdef USE_SDL2
    if ( x >= ( int64_t ) gr->width || y >= ( int64_t ) gr->height ) return -1;
#endif
#ifdef USE_SDL2_GPU
    if ( x >= ( int64_t ) gr->tex->w || y >= ( int64_t ) gr->tex->h ) return -1;
#endif

#ifdef USE_SDL2
    SDL_Texture* auxTexture = SDL_CreateTexture( gRenderer, gPixelFormat->format, SDL_TEXTUREACCESS_TARGET, 1, 1 );
    SDL_SetRenderTarget( gRenderer, auxTexture );

    SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0 );
    SDL_RenderClear( gRenderer );

    SDL_SetTextureBlendMode( auxTexture, SDL_BLENDMODE_NONE );

    SDL_Rect sourceRect = { x, y, 1, 1 };
    SDL_RenderCopy(gRenderer, gr->tex, &sourceRect, NULL);

    Uint32 pixelColor;

    SDL_RenderReadPixels( gRenderer, NULL, gPixelFormat->format, &pixelColor, sizeof( Uint32 ) );

    SDL_DestroyTexture( auxTexture );

    SDL_SetRenderTarget( gRenderer, NULL );

    return pixelColor;
#endif
#ifdef USE_SDL2_GPU
    if ( !gr->tex->target ) GPU_LoadTarget( gr->tex );
    SDL_Color c = GPU_GetPixel( gr->tex->target, ( Sint16 ) x, ( Sint16 ) y );
    return SDL_MapRGBA( gPixelFormat, c.r, c.g, c.b, c.a );
#endif
}
#else

int64_t gr_get_pixel( GRAPH * gr, int64_t x, int64_t y ) {

    if ( !gr ) return -1;

    if ( x < 0 || y < 0 ) return -1;

    if ( gr_create_image_for_graph( gr ) ) return -1;

    if ( !gr->tex ) return -1;

    bitmap_update_surface( gr );

    if ( x >= ( int64_t ) gr->surface->w || y >= ( int64_t ) gr->surface->h ) return -1;

    return *( uint32_t * ) ( ( ( uint8_t * ) gr->surface->pixels ) + ( y * gr->surface->pitch + x * 4 ) );
}
#endif

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : gr_put_pixel
 *
 *  Paint a pixel with no clipping whatsoever, except by
 *  the bitmap's dimensions
 *
 *  PARAMS :
 *      dest            Destination bitmap
 *      x, y            Pixel coordinates
 *      color           1, 8 or 16-bit pixel value
 *
 *  RETURN VALUE :
 *      None
 *
 */

void gr_put_pixel( GRAPH * gr, int64_t x, int64_t y, int64_t color ) {

    if ( !gr ) return;

    if ( x < 0 || y < 0 ) return;

    if ( gr_create_image_for_graph( gr ) ) return;

#ifdef USE_SDL2
    if ( x >= ( int64_t ) gr->width || y >= ( int64_t ) gr->height ) return;
#endif
#ifdef USE_SDL2_GPU
    if ( x >= ( int64_t ) gr->tex->w || y >= ( int64_t ) gr->tex->h ) return;
#endif

#ifdef USE_SDL2
    SDL_Color c;
    SDL_SetRenderTarget( gRenderer, gr->tex );
    SDL_GetRGBA( color, gPixelFormat, &c.r, &c.g, &c.b, &c.a ) ;
    SDL_SetRenderDrawColor( gRenderer, c.r, c.g, c.b, c.a );
    SDL_Rect rect = { x, y, 1, 1 };
    SDL_RenderFillRect( gRenderer, &rect );
    SDL_SetRenderTarget( gRenderer, NULL );
#endif
#ifdef USE_SDL2_GPU
    SDL_Color c;
    SDL_GetRGBA( color, gPixelFormat, &c.r, &c.g, &c.b, &c.a ) ;
    GPU_Pixel( gr->tex->target, ( float ) x, ( float ) y, c );

    gr->dirty = 1;
#endif
}

/* --------------------------------------------------------------------------- */
