/*
 *  Copyright (C) SplinterGU (Fenix/BennuGD) (Since 2006)
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

#ifndef __G_BASE_H
#define __G_BASE_H


#include <SDL.h>
#ifdef USE_SDL2_GPU
    #include <SDL_gpu.h>
#endif

/* --------------------------------------------------------------------------- */

/* Color interno BennuGD2: 0xRRGGBBAA */
#define BGDCOLOR_R(c) (((uint32_t)(c) >> 24) & 0xff)
#define BGDCOLOR_G(c) (((uint32_t)(c) >> 16) & 0xff)
#define BGDCOLOR_B(c) (((uint32_t)(c) >>  8) & 0xff)
#define BGDCOLOR_A(c) ( (uint32_t)(c)        & 0xff)

#define BGDCOLOR(r,g,b,a) \
    ((((uint32_t)(r)) << 24) | (((uint32_t)(g)) << 16) | \
     (((uint32_t)(b)) <<  8) |  ((uint32_t)(a)))

/* --------------------------------------------------------------------------- */

#ifdef USE_SDL2
    typedef SDL_Rect    BGD_Rect;
#endif

#ifdef USE_SDL2_GPU
    typedef GPU_Rect    BGD_Rect;
#endif

typedef SDL_Point       BGD_Point;

typedef struct {
    double x;
    double y;
    double x2;
    double y2;
} BGD_Box;

/* --------------------------------------------------------------------------- */

#endif
