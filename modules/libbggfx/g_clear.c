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

    if ( !dest->surface ) {
        dest->surface = SDL_CreateRGBSurface(0, dest->width, dest->height, gPixelFormat->BitsPerPixel, gPixelFormat->Rmask, gPixelFormat->Gmask, gPixelFormat->Bmask, gPixelFormat->Amask );
        if ( !dest->surface ) return;
//        SDL_SetColorKey( dest->surface, SDL_TRUE, 0 );
    }

    memset( dest->surface->pixels, '\0', dest->height * dest->surface->pitch );

    dest->texture_must_update = 1;
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

    if ( !dest->surface ) {
        dest->surface = SDL_CreateRGBSurface( 0, dest->width, dest->height, gPixelFormat->BitsPerPixel, gPixelFormat->Rmask, gPixelFormat->Gmask, gPixelFormat->Bmask, gPixelFormat->Amask );
        if ( !dest->surface ) return;
//        SDL_SetColorKey( dest->surface, SDL_TRUE, 0 );
    }

    if ( !color ) {
        gr_clear( dest );
        return;
    }

    switch ( dest->surface->format->BitsPerPixel ) {
        case 1: {
            int c = color ? 0xFF : 0 ;
            memset( dest->surface->pixels, c, dest->surface->pitch * dest->height ) ;
            break;
        }

        case 8: {
            memset( dest->surface->pixels, color, dest->surface->pitch * dest->height ) ;
            break;
        }

        case 16: {
            uint8_t * data = dest->surface->pixels ;
            int16_t * ptr ;
            int n, y ;
            y = dest->height;
            while ( y-- ) {
                ptr = ( int16_t * ) data;
                n = dest->width;
                while ( n-- ) * ptr++ = color ;
                data += dest->surface->pitch;
            }
            break;
        }

        case 32: {
            uint8_t * data = dest->surface->pixels ;
            uint32_t * ptr ;
            int n, y ;
            y = dest->height;
            while ( y-- ) {
                ptr = ( uint32_t * ) data;
                n = dest->width;
                while ( n-- ) * ptr++ = color ;
                data += dest->surface->pitch;
            }
            break;
        }
    }

    dest->texture_must_update = 1;
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

    if ( !dest->surface ) {
        dest->surface = SDL_CreateRGBSurface( 0, dest->width, dest->height, gPixelFormat->BitsPerPixel, gPixelFormat->Rmask, gPixelFormat->Gmask, gPixelFormat->Bmask, gPixelFormat->Amask );
        if ( !dest->surface ) return;
//        SDL_SetColorKey( dest->surface, SDL_TRUE, 0 );
    }

    if ( !region ) {
        x = 0 ;
        y = 0 ;
        w = dest->width - 1 ;
        h = dest->height - 1 ;
    } else {
        x = MAX( MIN( region->x, region->x2 ), 0 ) ;
        y = MAX( MIN( region->y, region->y2 ), 0 ) ;
        w = MIN( MAX( region->x, region->x2 ), dest->width - 1 ) - x ;
        h = MIN( MAX( region->y, region->y2 ), dest->height - 1 ) - y ;
    }

    if ( x > dest->width || region->y > dest->height ) return;
    if ( ( x + w ) < 0 || ( y + h ) < 0 ) return;

    switch ( dest->surface->format->BitsPerPixel ) {
        case 1: {
            uint8_t * mem, * pmem = ( (uint8_t *) dest->surface->pixels ) + dest->surface->pitch * y;
            int w2, h2;

            if ( color ) {
                for ( h2 = 0; h2 < h; h2++ ) {
                    mem = pmem;
                    for ( w2 = 0; w2 < w; w2++ ) * ( mem + ( ( x + w2 ) >> 3 ) ) |= ( 0x80 >> ( ( x + w2 ) & 7 ) );
                    pmem += dest->surface->pitch;
                }
            } else {
                for ( h2 = 0; h2 < h; h2++ ) {
                    mem = pmem;
                    for ( w2 = 0; w2 < w; w2++ ) * ( mem + ( ( x + w2 ) >> 3 ) ) &= ~( 0x80 >> ( ( x + w2 ) & 7 ) );
                    pmem += dest->surface->pitch;
                }
            }
            dest->texture_must_update = 1;
            break;
        }

        case 8: {
            uint8_t * mem, * pmem = ( (uint8_t *) dest->surface->pixels ) + dest->surface->pitch * y + x;
            int w2, h2;

            for ( h2 = 0; h2 < h; h2++ ) {
                mem = pmem;
                for ( w2 = 0; w2 < w; w2++ ) {
                    *mem++ = color;
                }
                pmem += dest->surface->pitch;
            }
            dest->texture_must_update = 1;
            break;
        }

        case 16: {
            uint8_t * pmem = ( (uint8_t *) dest->surface->pixels ) + dest->surface->pitch * y + x * 2;
            uint16_t * mem;
            int w2, h2;

            for ( h2 = 0; h2 < h; h2++ ) {
                mem = ( uint16_t * ) pmem;
                for ( w2 = 0; w2 < w; w2++ ) {
                    *mem++ = color;
                }
                pmem += dest->surface->pitch;
            }
            dest->texture_must_update = 1;
            break;
        }

        case 32: {
            uint8_t * pmem = ( (uint8_t *) dest->surface->pixels ) + dest->surface->pitch * y + x * 4;
            uint32_t * mem;
            int w2, h2;

            for ( h2 = 0; h2 < h; h2++ ) {
                mem = ( uint32_t * ) pmem;
                for ( w2 = 0; w2 < w; w2++ ) {
                    *mem++ = color;
                }
                pmem += dest->surface->pitch;
            }
            dest->texture_must_update = 1;
            break;
        }

        default:
            return;
    }

    dest->texture_must_update = 1;

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
