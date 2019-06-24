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
/* Gesti�n de regiones                                                         */

REGION regions[ MAX_REGIONS ];

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : region_define
 *
 *  Sets one of the 32 regions visible from the fenix
 *  language to the values given
 *
 *  The region 0 is always equal to the whole screen
 *  and cannot be changed
 *
 *  PARAMS :
 *      n           Number of region to set (1 to MAX_REGIONS - 1)
 *      x, y        Top-Left coordinates
 *      width       Width in pixels
 *      height      Height in pixels
 *
 *  RETURN VALUE :
 *      None
 */

void region_define( int64_t region, int64_t x, int64_t y, int64_t width, int64_t height ) {
    if ( region < 1 || region > MAX_REGIONS - 1 ) return;
#if 1
    regions[ region ].x = x;
    regions[ region ].y = y;
    regions[ region ].x2 = ( x + width ) - 1;
    regions[ region ].y2 = ( y + height ) - 1;
#else
    regions[ region ].x = MAX( x, 0 );
    regions[ region ].y = MAX( y, 0 );
    regions[ region ].x2 = MIN( scr_width, x + width ) - 1;
    regions[ region ].y2 = MIN( scr_height, y + height ) - 1;
#endif
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : region_union
 *
 *  Calculates a region as the intersection of another two
 *
 *  PARAMS :
 *      dest        First region, and the one to contain the result
 *      b           Second region
 *
 *  RETURN VALUE :
 *      None. The result will be stored in the first region
 *      used as parameter
 *
 */

void region_union( REGION * dest, REGION * b ) {
    dest->x = MAX( dest->x, b->x );
    dest->y = MAX( dest->y, b->y );
    dest->y2 = MIN( dest->y2, b->y2 );
    dest->x2 = MIN( dest->x2, b->x2 );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : region_is_empty
 *
 *  Returns TRUE if the region contains no pixels
 *
 *  PARAMS :
 *      region      Region to check
 *
 *  RETURN VALUE :
 *      1 if the region is empty, 0 otherwise
 *
 */

int region_is_empty( REGION * a ) {
    return ( a->x2 < a->x ) || ( a->y2 < a->y );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : region_is_out
 *
 *  Returns TRUE if two regions overlap
 *
 *  PARAMS :
 *      a           First region to check
 *      b           Second region to check
 *
 *  RETURN VALUE :
 *      1 if there is at least one pixel in both regions, 0 otherwise
 *
 */

int region_is_out( REGION * a, REGION * b ) {
    return ( b->x > a->x2 || b->y > a->y2 || b->x2 < a->x || b->y2 < a->y );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : region_new
 *
 *  Create a new region object. Only rectangular regions
 *  are supported in this library.
 *
 *  PARAMS :
 *      x, y        Coordinates of the top-left pixel
 *      width       Width of the region in pixels
 *      height      Height of the region in pixels
 *
 *  RETURN VALUE :
 *      Returns a pointer to the new object
 *
 */

REGION * region_new( int64_t x, int64_t y, int64_t width, int64_t height ) {
    REGION * region = malloc( sizeof( REGION ) );
#if 1
    region->x = x;
    region->y = y;
    region->x2 = ( x + width ) - 1;
    region->y2 = ( y + height ) - 1;
#else
    region->x = MAX( x, 0 );
    region->y = MAX( y, 0 );
    region->x2 = MIN( scr_width, x + width ) - 1;
    region->y2 = MIN( scr_height, y + height ) - 1;
#endif
    return region;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : region_get
 *
 *  Returns one of the MAX_REGIONS default regions visible from the fenix language
 *
 *  PARAMS :
 *      n           Number of the region ( 0 to MAX_REGIONS - 1 )
 *
 *  RETURN VALUE :
 *      Returns the region object
 *
 */

REGION * region_get( int64_t n ) {
    if ( n < 0 || n > MAX_REGIONS - 1 ) return 0;

    return &regions[ n ];
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : region_destroy
 *
 *  Free the memory allocated by a region
 *
 *  PARAMS :
 *      region      Pointer to the region object
 *
 *  RETURN VALUE :
 *      None
 *
 */

void region_destroy( REGION * region ) {
    free( region );
}

/* --------------------------------------------------------------------------- */
