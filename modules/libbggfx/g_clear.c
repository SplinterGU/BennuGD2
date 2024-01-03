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

#include "libbggfx.h"

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : gr_clear
 *
 *  Clear a bitmap (paint all pixels as 0 [transparent])
 *
 *  PARAMS :
 *      dest            Bitmap to clear
 *
 *  RETURN VALUE :
 *      None
 *
 */

void gr_clear( GRAPH * dest ) {
    if ( !dest ) return;
    if ( gr_create_image_for_graph( dest ) ) return;

#ifdef USE_SDL2
    SDL_SetRenderTarget( gRenderer, dest->tex );
    SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 255 );
    SDL_RenderClear( gRenderer );
#endif
#ifdef USE_SDL2_GPU
    GPU_Clear( dest->tex->target );
#endif
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : gr_clear_as
 *
 *  Clear a bitmap (paint all pixels as the given color)
 *
 *  PARAMS :
 *      dest            Bitmap to clear
 *      color           32bit color value
 *
 *  RETURN VALUE :
 *      None
 *
 */

void gr_clear_as( GRAPH * dest, int color ) {
    if ( !dest ) return;
    if ( gr_create_image_for_graph( dest ) ) return;

#ifdef USE_SDL2
    Uint8 r, g, b, a;
    SDL_SetRenderTarget( gRenderer, dest->tex );
    SDL_GetRGBA( color, gPixelFormat, &r, &g, &b, &a );
    SDL_SetRenderDrawColor( gRenderer, r, g, b, a );
    SDL_Rect eraseRect = { 0, 0, dest->width, dest->height };
    SDL_SetRenderDrawBlendMode( gRenderer, SDL_BLENDMODE_NONE );
    SDL_RenderFillRect( gRenderer, &eraseRect );
#endif
#ifdef USE_SDL2_GPU
    Uint8 r, g, b, a;
    SDL_GetRGBA( color, gPixelFormat, &r, &g, &b, &a );
    GPU_ClearRGBA( dest->tex->target, r, g, b, a );
#endif
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : gr_clear_region_as
 *
 *  Clear a region bitmap (paint all pixels as 0 [transparent])
 *
 *  PARAMS :
 *      dest            Bitmap to clear
 *      region          Region to clear or NULL for the whole screen
 *      color           32-bit color value
 *
 *  RETURN VALUE :
 *      None
 *
 */

void gr_clear_region_as( GRAPH * dest, REGION * region, int color ) {
    int x, y, w, h;

    if ( !dest ) return;

    if ( !region ) {
        x = 0 ;
        y = 0 ;
        w = dest->width;
        h = dest->height;
    } else {
        x = MAX( MIN( region->x, region->x2 ), 0 ) ;
        y = MAX( MIN( region->y, region->y2 ), 0 ) ;
        w = MIN( MAX( region->x, region->x2 ) + 1, dest->width ) - x ;
        h = MIN( MAX( region->y, region->y2 ) + 1, dest->height ) - y ;
    }

    if ( x > dest->width || y > dest->height ) return;
    if ( ( x + w ) < 0 || ( y + h ) < 0 ) return;

    if ( gr_create_image_for_graph( dest ) ) return;

#ifdef USE_SDL2
    Uint8 r, g, b, a;
    SDL_SetRenderTarget( gRenderer, dest->tex );
    SDL_GetRGBA( color, gPixelFormat, &r, &g, &b, &a );
    SDL_SetRenderDrawColor( gRenderer, r, g, b, a );
    SDL_Rect eraseRect = { x, y, w, h };
    SDL_SetRenderDrawBlendMode( gRenderer, SDL_BLENDMODE_NONE );
    SDL_RenderFillRect( gRenderer, &eraseRect );
#endif
#ifdef USE_SDL2_GPU
    Uint8 r, g, b, a;
    SDL_GetRGBA( color, gPixelFormat, &r, &g, &b, &a );
    GPU_SetClip( dest->tex->target, x, y, w, h );
    GPU_ClearRGBA( dest->tex->target, r, g, b, a );
    GPU_UnsetClip( dest->tex->target );
#endif
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : gr_clear_region
 *
 *  Clear a region bitmap (paint all pixels as 0 [transparent])
 *
 *  PARAMS :
 *      dest            Bitmap to clear
 *      region          Region to clear or NULL for the whole screen
 *
 *  RETURN VALUE :
 *      None
 *
 */

void gr_clear_region( GRAPH * dest, REGION * region ) {
    gr_clear_region_as( dest, region, 0 );
}

/* --------------------------------------------------------------------------- */
