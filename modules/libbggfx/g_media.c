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

#include <SDL.h>
#ifdef USE_SDL2_GPU
    #include <SDL_gpu.h>
#endif
#include <stdlib.h>
#include <string.h>

#include "bgdrtm.h"
#include "bgddl.h"
#include "dlvaracc.h"

#include "libbggfx.h"

/* --------------------------------------------------------------------------- */

static int __media_info( void * what, REGION * bbox, int64_t * z, int64_t * drawme ) {
/*
    int hold = debugger_show_console || system_paused;
    if ( hold != mh->in_hold_state ) {
        if ( hold ) {
            // Need pause
            mh->is_paused = media_get_status( mh ) == MEDIA_STATUS_PAUSED;
            if ( !mh->is_paused ) media_pause( mh );
        } else {
            // Need resume
            if ( !mh->is_paused ) media_resume( mh );
        }
        mh->in_hold_state = hold;
    }
*/

    MEDIA * mh = ( MEDIA * ) what;

    int ret = thr_update( mh->m );
    if ( ret || mh->texture_must_update ) {
#ifdef USE_SDL2_GPU
        GPU_UpdateImage( mh->graph->tex, NULL, mh->surface, NULL );
#endif
        mh->texture_must_update = 0;
    }
    * drawme = 0;
    return 0;
}

/* --------------------------------------------------------------------------- */

void media_init() {
}

/* --------------------------------------------------------------------------- */

void media_exit() {
}

/* --------------------------------------------------------------------------- */

MEDIA * media_load( const char * media, int64_t * graph_id, int timeout ) {
    int vw, vh;

    if ( !media ) return NULL;

    MEDIA * mh = calloc(1, sizeof( MEDIA ) );
    if ( !mh ) return NULL;

    mh->media = strdup( media );
    if ( !mh->media ) {
        free( mh );
        return NULL;
    }

    mh->m = thr_open( mh->media, mh->timeout);
    if ( !mh->m ) {
        free( mh->media );
        free( mh );
        return NULL;
    }

    if ( thr_get_video_size( mh->m, &vw, &vh ) != -1 ) {
        mh->video_width = vw;
        mh->video_height = vh;

//        mh->surface = SDL_CreateRGBSurface(0, mh->video_width, mh->video_height, 32, 0xff, 0xff00, 0xff0000, 0 /* Force alpha to opaque */ );
        mh->surface = SDL_CreateRGBSurface(0, mh->video_width, mh->video_height, 32, 0, 0, 0, 0 /* Force alpha to opaque */ );
        if ( !mh->surface ) {
            thr_close( mh->m );
            free( mh->media );
            free( mh );
            return NULL;
        }
        SDL_SetColorKey( mh->surface, SDL_FALSE, 0 );

        mh->graph = bitmap_new( 0, mh->video_width, mh->video_height, mh->surface );
        if ( !mh->graph ) {
            thr_close( mh->m );
            SDL_FreeSurface( mh->surface );
            free( mh->media );
            free( mh );
            return NULL;
        }

        *graph_id = ( int64_t ) ( mh->graph->code = bitmap_next_code() );

        grlib_add_map( 0, mh->graph );

        mh->objectid = gr_new_object( INT64_MIN + 10, __media_info, NULL, ( void * ) mh );

        thr_set_shadow_surface( mh->m, mh->surface );
    }

    return mh ;
}

/* --------------------------------------------------------------------------- */

void media_unload( MEDIA * mh ) {
    if ( mh ) {
        if ( mh->objectid ) gr_destroy_object( mh->objectid );

        thr_close( mh->m );
        if ( mh->surface ) SDL_FreeSurface( mh->surface );
        if ( mh->graph ) grlib_unload_map( 0, mh->graph->code );

        free( mh->media );
        free( mh );
    }
}

/* --------------------------------------------------------------------------- */

int media_play( MEDIA * mh ) {
    if ( !mh ) return -1;
    if ( thr_get_state( mh->m ) == MEDIA_STATUS_ENDED ) {
        thr_close( mh->m );
        mh->m = NULL;
    }
    if ( !mh->m ) {
        mh->m = thr_open( mh->media, mh->timeout);
        if ( !mh->m ) return -1;
        thr_set_shadow_surface( mh->m, mh->surface );
    }
    thr_pause( mh->m, 0 );
    return 0;
}

/* --------------------------------------------------------------------------- */

void media_pause_action( MEDIA * mh, int do_pause ) {
    if ( !mh ) return;
    thr_pause( mh->m, do_pause );
}

/* --------------------------------------------------------------------------- */

void media_pause( MEDIA * mh ) {
    if ( !mh ) return;
    thr_pause( mh->m, 1 );
}

/* --------------------------------------------------------------------------- */

void media_resume( MEDIA * mh ) {
    if ( !mh ) return;
    thr_pause( mh->m, 0 );
}

/* --------------------------------------------------------------------------- */

void media_stop( MEDIA * mh ) {
    if ( !mh ) return;
    if ( mh->m ) thr_close( mh->m );
    mh->m = NULL;
}

/* --------------------------------------------------------------------------- */

int64_t media_get_time( MEDIA * mh ) {
    if ( !mh ) return -1;
    return ( int64_t ) thr_get_time( mh->m );
}

/* --------------------------------------------------------------------------- */

int64_t media_get_duration( MEDIA * mh ) {
    if ( !mh ) return -1;
    return ( int64_t ) -1;
}

/* --------------------------------------------------------------------------- */

int media_get_status( MEDIA * mh ) {
    if ( !mh ) return MEDIA_STATUS_ERROR;
    if ( !mh->m ) return MEDIA_STATUS_STOPPED;
    return thr_get_state( mh->m );
}

/* --------------------------------------------------------------------------- */

int media_get_mute( MEDIA * mh ) {
    if ( !mh ) return -1;
    return thr_get_mute( mh->m );
}

/* --------------------------------------------------------------------------- */

void media_set_mute( MEDIA * mh, int status ) {
    if ( !mh ) return;
    thr_set_mute( mh->m, status );
}

/* --------------------------------------------------------------------------- */

int media_get_volume( MEDIA * mh ) {
    if ( !mh ) return -1;
    return thr_get_volume( mh->m );
}

/* --------------------------------------------------------------------------- */

int media_set_volume( MEDIA * mh, int volume ) {
    if ( !mh ) return -1;
    return thr_set_volume( mh->m, volume );
}

/* --------------------------------------------------------------------------- */
