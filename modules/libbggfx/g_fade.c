/*
 *  Copyright (C) 2006-2016 SplinterGU (Fenix/BennuGD)
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

/* -------------------------------------------------------------------------- */

int fade_on = 0;
int fade_set = 0;

static int fade_to;
static double fade_pos = 100.0;
static int fade_time_pos;
static double fade_time_step;

/* -------------------------------------------------------------------------- */
/* duration = milliseconds                                                    */

void gr_fade_init( int to, int duration ) {
    if ( fade_pos == to ) {
        GLOQWORD( libbggfx, FADING ) = 0;
        fade_on = 0;
        return;
    }

    if ( to > 200 )     to = 200;
    else if ( to < 0 )  to = 0;

    fade_time_step = duration / (double) ( to - fade_pos );

    if ( fade_time_step == 0.0 ) return;

    fade_time_pos = SDL_GetTicks();

    fade_to = to;
    fade_on = 1;

    GLOQWORD( libbggfx, FADING ) = 1;
}

/* -------------------------------------------------------------------------- */

void gr_fade_step() {
    double delta, pos;
    int currtime;

    if ( fade_on ) {
        fade_set = 1;
        GLOQWORD( libbggfx, FADING ) = 1;

        if ( ( delta = ( ( currtime = SDL_GetTicks() ) - fade_time_pos ) / fade_time_step ) != 0 ) {
            pos = fade_pos + delta;
            if ( ( fade_pos < fade_to && pos > fade_to ) || ( fade_pos > fade_to && pos < fade_to ) ) pos = fade_to;
            fade_pos = pos;
            fade_time_pos = currtime;
        }
    }

    if ( fade_set ) {
        if ( !fade_on && fade_pos == 100 ) fade_set = 0;

        SDL_SetRenderDrawBlendMode( gRenderer, SDL_BLENDMODE_BLEND );
        if ( fade_pos < 100 )   SDL_SetRenderDrawColor( gRenderer,   0,   0,   0, ( 100 - fade_pos ) * 255 / 100 );
        else                    SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, ( fade_pos - 100 ) * 255 / 100 );
        SDL_RenderFillRect( gRenderer, NULL );

        if ( fade_pos == fade_to ) {
            GLOQWORD( libbggfx, FADING ) = 0;
            fade_on = 0;
        }
    }
}

/* -------------------------------------------------------------------------- */
