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

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "bgddl.h"

#include <SDL.h>

#include "libbggfx.h"

#include "dlvaracc.h"

/* --------------------------------------------------------------------------- */

#define FPS_INTIAL_VALUE    25
#define FPS_INTIAL_SKIP     2

/* --------------------------------------------------------------------------- */

int64_t fps_value = FPS_INTIAL_VALUE;
int64_t max_jump = FPS_INTIAL_SKIP;
double frame_ms = 1000.0 / FPS_INTIAL_VALUE; /* 40.0; */

uint64_t frames_count = 0;
int64_t last_frame_ticks = 0;
int64_t jump = 0;

int64_t FPS_count = 0;
int64_t FPS_init = 0;

int64_t FPS_count_sync = 0;
int64_t FPS_init_sync = 0;

double ticks_per_frame = 0.0;
double fps_partial = 0.0;

/* --------------------------------------------------------------------------- */
/* Inicializaci�n y controles de tiempo                                        */
/* --------------------------------------------------------------------------- */

/*
 *  FUNCTION : gr_set_fps
 *
 *  Change the game fps and frameskip values
 *
 *  PARAMS :
 *      fps         New number of frames per second
 *      jump        New value of maximum frameskip
 *
 *  RETURN VALUE :
 *      None
 */

void gr_set_fps( int64_t fps, int64_t skip ) {
    if ( fps == fps_value && skip == max_jump ) return;

    frame_ms = fps ? 1000.0 / ( double ) fps : 0.0;
    max_jump = skip;
    fps_value = ( int64_t ) fps;

    FPS_init_sync = FPS_init = 0;
    FPS_count_sync = FPS_count = 0;

    jump = 0;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : gr_wait_frame
 *
 *  Wait for the next frame start.
 *
 *  PARAMS :
 *      None
 *
 *  RETURN VALUE :
 *      None
 */

void gr_wait_frame() {
    int64_t frame_ticks;

    GLOQWORD( libbggfx, FRAMES_COUNT ) = ++frames_count;

    /* -------------- */

    /* Tomo Tick actual */
#if defined(TARGET_GP2X_WIZ) || defined(TARGET_CAANOO)
    frame_ticks = bgdrtm_ptimer_get_ticks_us() / 1000L;
#else
    frame_ticks = SDL_GetTicks();
#endif
    if ( !FPS_init_sync ) {
#if defined(TARGET_GP2X_WIZ) || defined(TARGET_CAANOO)
        FPS_init_sync = FPS_init = bgdrtm_ptimer_get_ticks_us() / 1000L;
#else
        FPS_init_sync = FPS_init = SDL_GetTicks();
#endif
        FPS_count_sync = FPS_count = 0;
        jump = 0;

        /* Tiempo inicial del nuevo frame */
        last_frame_ticks = frame_ticks;

        return;
    }

    /* Tiempo transcurrido total del ejecucion del ultimo frame (Frame time en ms) */
    * ( double * ) &GLOQWORD( libbggfx, FRAME_TIME ) = ( frame_ticks - last_frame_ticks ) / 1000.0f;

    /* -------------- */

    FPS_count++;

    /* -------------- */

    if ( fps_value ) {
        FPS_count_sync++;

        ticks_per_frame = ( ( double ) ( frame_ticks - FPS_init_sync ) ) / ( double ) FPS_count_sync;
        fps_partial = 1000.0 / ticks_per_frame;

        if ( fps_partial == fps_value ) {
            FPS_init_sync = frame_ticks;
            FPS_count_sync = 0;
            jump = 0;
        }
        else if ( fps_partial > fps_value ) {
            int32_t delay = FPS_count_sync * frame_ms - ( frame_ticks - FPS_init_sync );

            if ( delay > 0 ) {
#if defined(TARGET_GP2X_WIZ) || defined(TARGET_CAANOO)
            {
                    unsigned long ta = bgdrtm_ptimer_get_ticks_us(), te = ta + delay * 1000;
                    if ( ta > te ) while ( bgdrtm_ptimer_get_ticks_us() > te );
                    while ( bgdrtm_ptimer_get_ticks_us() < te );
                }
#else
                SDL_Delay( delay );
#endif
                /* Reajust after delay */
#if defined(TARGET_GP2X_WIZ) || defined(TARGET_CAANOO)
                frame_ticks = bgdrtm_ptimer_get_ticks_us() / 1000L;
#else
                frame_ticks = SDL_GetTicks();
#endif
                ticks_per_frame = ( ( double ) ( frame_ticks - FPS_init_sync ) ) / ( double ) FPS_count_sync;
                fps_partial = 1000.0 / ticks_per_frame;
            }

            jump = 0;
        } else {
            if ( jump < max_jump ) /* Como no me alcanza el tiempo, voy a hacer skip */
                jump++; /* No dibujar el frame */
            else {
                FPS_init_sync = frame_ticks;
                FPS_count_sync = 0;
                jump = 0;
            }
        }
    }

    /* Si paso 1 segundo o mas desde la ultima lectura */
    if ( frame_ticks - FPS_init >= 1000 ) {
        if ( fps_value ) {
            GLOQWORD( libbggfx, SPEED_GAUGE ) = FPS_count /*fps_partial*/ * 100.0 / fps_value;
        } else {
            GLOQWORD( libbggfx, SPEED_GAUGE ) = 100;
        }

        GLOQWORD( libbggfx, FPS ) = FPS_count;

        FPS_init = frame_ticks;
        FPS_count = 0;
    }

    /* Tiempo inicial del nuevo frame */
    last_frame_ticks = frame_ticks;
}

/* --------------------------------------------------------------------------- */

void gr_draw_frame() {
    if ( jump ) return;

    /* Set Viewport */
//    SDL_RenderSetViewport( gRenderer, NULL );

    /* Clear screen */
#ifdef USE_NATIVE_SDL2
    SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
    SDL_RenderClear( gRenderer );
#else
    GPU_Clear( gRenderer );
#endif

    GRAPH * background = NULL;
    int64_t background_graph = GLOQWORD( libbggfx, BACKGROUND_GRAPH );

    /* Put background */
    if ( background_graph && ( background = bitmap_get( GLOQWORD( libbggfx, BACKGROUND_FILE ), background_graph ) ) ) {
        int64_t sizex = GLOINT64( libbggfx, BACKGROUND_SIZEX ),
                sizey = GLOINT64( libbggfx, BACKGROUND_SIZEY );
        if ( sizex == 100 && sizey == 100 ) sizex = sizey = GLOINT64( libbggfx, BACKGROUND_SIZE );
        gr_blit( NULL,
                 NULL,
                 scr_width / 2,
                 scr_height / 2,
                 GLOQWORD( libbggfx, BACKGROUND_FLAGS ),
                 GLOQWORD( libbggfx, BACKGROUND_ANGLE ),
                 sizex,
                 sizey,
                 background,
                 NULL,
                 255,
                 GLOBYTE( libbggfx, BACKGROUND_COLORR ),
                 GLOBYTE( libbggfx, BACKGROUND_COLORG ),
                 GLOBYTE( libbggfx, BACKGROUND_COLORB )
                );
    }

    /* Update the object list */
    gr_update_objects();

    /* Dump everything */
    gr_draw_objects();

    if ( fade_on || fade_set ) gr_fade_step();

    //Update screen
#ifdef USE_NATIVE_SDL2
    SDL_RenderPresent( gRenderer );
    SDL_RenderSetClipRect( gRenderer, NULL );
#else
    GPU_Flip( gRenderer );
#endif
}

/* --------------------------------------------------------------------------- */

void frame_init() {
#ifndef TARGET_DINGUX_A320
    if ( !SDL_WasInit( SDL_INIT_TIMER ) ) SDL_InitSubSystem( SDL_INIT_TIMER );
#endif
}

/* --------------------------------------------------------------------------- */

void frame_exit() {
#ifndef TARGET_DINGUX_A320
    if ( SDL_WasInit( SDL_INIT_TIMER ) ) SDL_QuitSubSystem( SDL_INIT_TIMER );
#endif
}

/* --------------------------------------------------------------------------- */
