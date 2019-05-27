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

int gr_get_pixel( GRAPH * gr, int64_t x, int64_t y ) {
    if ( !gr || !gr->surface ) return -1;

    SDL_Surface * surface = gr->surface;

    if ( x < 0 || y < 0 || x >= ( int64_t ) surface->w || y >= ( int64_t ) surface->h ) return -1;

    switch ( surface->format->BitsPerPixel ) {
        case 1:
            return (( *( uint8_t * )( surface->pixels + surface->pitch * y + ( x >> 3 ) ) ) & ( 0x80 >> ( x & 7 ) ) ) ? 1 : 0;

        case 8:
            return *(( uint8_t * ) surface->pixels + surface->pitch * y + x );

        case 16:
            return *( uint16_t * )(( uint8_t * ) surface->pixels + surface->pitch * y + ( x << 1 ) );

        case 32:
            return *( uint32_t * )(( uint8_t * ) surface->pixels + surface->pitch * y + ( x << 2 ) );
    }

    return -1;
}

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
    if ( !gr || !gr->surface ) return;

    SDL_Surface * surface = gr->surface;

    if ( x < 0 || y < 0 || x >= ( int64_t ) surface->w || y >= ( int64_t ) surface->h ) return;

    switch ( surface->format->BitsPerPixel ) {
        case 1:
            if ( color )    *(( uint8_t * ) surface->pixels + surface->pitch * y + ( x >> 3 ) ) |= ( 0x80 >> ( x & 7 ) );
            else            *(( uint8_t * ) surface->pixels + surface->pitch * y + ( x >> 3 ) ) &= ~( 0x80 >> ( x & 7 ) );
            break;

        case 8:
            *(( uint8_t * ) surface->pixels + surface->pitch * y + x ) = color;
            break;

        case 16:
            *( uint16_t * )(( uint8_t * ) surface->pixels + surface->pitch * y + ( x << 1 )) = color;
            break;

        case 32:
            *( uint32_t * ) (( uint8_t * ) surface->pixels + surface->pitch * y + ( x << 2 )) = color;
            break;
    }
}

/* --------------------------------------------------------------------------- */
