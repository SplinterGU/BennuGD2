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

#ifndef __BITMAP_H
#define __BITMAP_H

#include <SDL.h>

/* --------------------------------------------------------------------------- */

// Access

#define BITMAP_SURFACE_ONLY         1
#define BITMAP_TEXTURE_STATIC       2           // SDL_TEXTUREACCESS_STATIC
#define BITMAP_TEXTURE_STREAMING    4           // SDL_TEXTUREACCESS_STREAMING
#define BITMAP_TEXTURE_TARGET       8           // SDL_TEXTUREACCESS_TARGET

#define CPOINT_UNDEFINED            32767       /* It's enough if X is set to this value */

/* --------------------------------------------------------------------------- */

typedef SDL_Point CPOINT;
typedef struct _bitmap {
    int64_t         code;           /* Identifier of the graphic (in the graphic library) */
    char            name[ 64 ];     /* Name of the graphic */

    uint64_t        width;          /* Width of a bitmap frame in pixels */
    uint64_t        height;         /* Height of a bitmap frame in pixels */

    uint64_t        ncpoints;       /* Number of control points */
    CPOINT          * cpoints;      /* Pointer to the control points ([0] = center) */

    int64_t         type;           /* surface/texture type */
    SDL_Surface     * surface;
    SDL_Texture     * texture;

    int64_t         nsegments;
    struct {
        int64_t         offx, offy;
        int64_t         w, h;
        SDL_Texture     * texture;
    } * segments;
} GRAPH;

/* --------------------------------------------------------------------------- */

extern void getRGBA_mask( int bpp, uint32_t * rmask, uint32_t * gmask, uint32_t * bmask, uint32_t * amask );
extern GRAPH * bitmap_new( int64_t code, int64_t width, int64_t height, SDL_Surface * surface );
extern GRAPH * bitmap_clone( GRAPH * map );
extern void bitmap_destroy( GRAPH * map );
extern void bitmap_add_cpoint( GRAPH * map, int64_t x, int64_t y );
extern void bitmap_set_cpoint( GRAPH * map, uint64_t point, int64_t x, int64_t y );
extern int64_t bitmap_next_code();

/* --------------------------------------------------------------------------- */

#endif
