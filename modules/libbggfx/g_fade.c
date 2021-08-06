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

#include <string.h>
#include <stdlib.h>

#include "bgdrtm.h"

#include "bgddl.h"
#include "dlvaracc.h"

#include "libbggfx.h"

#include "fmath.h"

/* -------------------------------------------------------------------------- */

int fade_on = 0;
int fade_set = 0;

static int fade_to_r = 0;
static int fade_to_g = 0;
static int fade_to_b = 0;
static int fade_to_a = 0;

static double fade_pos_r = 0.0;
static double fade_pos_g = 0.0;
static double fade_pos_b = 0.0;
static double fade_pos_a = 0.0;

static int fade_time_pos;
static double fade_time_step;

int fade_region = 0;

/* -------------------------------------------------------------------------- */
/* duration = milliseconds                                                    */

void gr_fade_init( int r, int g, int b, int a, int duration, int region ) {

    if ( ( int ) fade_pos_a == 0 ) fade_pos_r = fade_pos_g = fade_pos_b = 0.0;

    double  dif_r = ( double ) r - fade_pos_r,
            dif_g = ( double ) g - fade_pos_g,
            dif_b = ( double ) b - fade_pos_b,
            dif_a = ( double ) a - fade_pos_a;

    double max = MAX( ABS( dif_r ), MAX( ABS( dif_g ), MAX( ABS( dif_b ), ABS( dif_a ) ) ) );

    if ( ( int ) max > 0 ) fade_time_step = duration / max; else fade_time_step = 0.0;

    fade_time_pos = SDL_GetTicks();

    fade_to_r = r;
    fade_to_g = g;
    fade_to_b = b;
    fade_to_a = a;

    fade_on = 1;

    fade_region = region;

    GLOQWORD( libbggfx, FADING ) = 1;
}

/* -------------------------------------------------------------------------- */

static void gr_fade_step() {
    double delta;
    int currtime = SDL_GetTicks();

    if ( fade_on ) {
        fade_set = 1;
        GLOQWORD( libbggfx, FADING ) = 1;

        if ( system_paused ) fade_time_pos = currtime;

        if ( fade_time_step == 0.0 ) {
            fade_pos_r = fade_to_r;
            fade_pos_g = fade_to_g;
            fade_pos_b = fade_to_b;
            fade_pos_a = fade_to_a;

        } else if ( ( delta = ( currtime - fade_time_pos ) / fade_time_step ) != 0 ) {
            if ( fade_pos_r < fade_to_r ) {
                fade_pos_r = fade_pos_r + delta; if ( fade_pos_r > fade_to_r ) fade_pos_r = fade_to_r;
            } else if ( fade_pos_r > fade_to_r ) {
                fade_pos_r = fade_pos_r - delta; if ( fade_pos_r < fade_to_r ) fade_pos_r = fade_to_r;
            }
            if ( fade_pos_g < fade_to_g ) {
                fade_pos_g = fade_pos_g + delta; if ( fade_pos_g > fade_to_g ) fade_pos_g = fade_to_g;
            } else if ( fade_pos_g > fade_to_g ) {
                fade_pos_g = fade_pos_g - delta; if ( fade_pos_g < fade_to_g ) fade_pos_g = fade_to_g;
            }
            if ( fade_pos_b < fade_to_b ) {
                fade_pos_b = fade_pos_b + delta; if ( fade_pos_b > fade_to_b ) fade_pos_b = fade_to_b;
            } else if ( fade_pos_b > fade_to_b ) {
                fade_pos_b = fade_pos_b - delta; if ( fade_pos_b < fade_to_b ) fade_pos_b = fade_to_b;
            }
            if ( fade_pos_a < fade_to_a ) {
                fade_pos_a = fade_pos_a + delta; if ( fade_pos_a > fade_to_a ) fade_pos_a = fade_to_a;
            } else if ( fade_pos_a > fade_to_a ) {
                fade_pos_a = fade_pos_a - delta; if ( fade_pos_a < fade_to_a ) fade_pos_a = fade_to_a;
            }
            fade_time_pos = currtime;

        }
    }

    if ( fade_set ) {
        REGION * region = region_get( fade_region );

        if ( !fade_on && ( int ) fade_pos_a == 0 ) fade_set = 0;

        shader_deactivate();

#ifdef USE_SDL2
        SDL_SetRenderDrawBlendMode( gRenderer, SDL_BLENDMODE_BLEND );
        SDL_SetRenderDrawColor( gRenderer, fade_pos_r, fade_pos_g, fade_pos_b, fade_pos_a );

        SDL_Rect r;
        if ( region ) {
            r.x = region->x;
            r.y = region->y;
            r.w = region->x2 - region->x + 1;
            r.h = region->y2 - region->y + 1;
        }

        SDL_RenderFillRect( gRenderer, !region ? NULL : &r );
#endif
#ifdef USE_SDL2_GPU
        SDL_Color color;
        color.r = fade_pos_r; color.g = fade_pos_g; color.b = fade_pos_b; color.a = fade_pos_a;

        GPU_SetShapeBlending( GPU_TRUE );
        if ( region )   GPU_RectangleFilled( gRenderer, region->x, region->y, region->x2 + 1, region->y2 + 1, color );
        else            GPU_RectangleFilled( gRenderer, 0.0, 0.0, gRenderer->w, gRenderer->h, color );
        GPU_SetShapeBlending( GPU_FALSE );
#endif

        if ( ( int ) fade_pos_r == fade_to_r && ( int ) fade_pos_g == fade_to_g && ( int ) fade_pos_b == fade_to_b && ( int ) fade_pos_a == fade_to_a ) {
            GLOQWORD( libbggfx, FADING ) = 0;
            fade_on = 0;
        }
    }
}

/* --------------------------------------------------------------------------- */

static int __gr_fade_info( void * what, REGION * bbox, int64_t * z, int64_t * drawme ) {
    * drawme = ( fade_on || fade_set );
    return 0;
}

/* --------------------------------------------------------------------------- */

static void __gr_fade_draw( void * what, REGION * clip ) {
    gr_fade_step();
}

/* --------------------------------------------------------------------------- */

void gr_fade_initalize() {
    gr_new_object(  INT64_MIN + 1, ( OBJ_INFO * ) __gr_fade_info, ( OBJ_DRAW * ) __gr_fade_draw, ( void * ) 0 );
}

/* --------------------------------------------------------------------------- */
