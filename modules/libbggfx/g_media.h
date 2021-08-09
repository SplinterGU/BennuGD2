/*
 *  Copyright (C) 2006-2019 SplinterGU (Fenix/BennuGD)
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

#ifndef __MEDIA_H
#define __MEDIA_H

#include <SDL.h>
#ifdef USE_SDL2_GPU
    #include <SDL_gpu.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <vlc/vlc.h>

#include "libbggfx.h"

/* --------------------------------------------------------------------------- */
/*
void * MEDIA_Load( string * media, int * graph_id )
int MEDIA_GetType()
void MEDIA_Play( void * media_id )
void MEDIA_Pause( void * media_id )
void MEDIA_Stop( void * media_id )
void MEDIA_Release( void * media_id )
int MEDIA_getDimensions( void * media_id, POINT_T * dimension )
int MEDIA_GetPosition( void * media_id )
void MEDIA_SetPosition( void * media_id, int position )


    libvlc_instance_t *libvlc;
*/

typedef struct media {
    libvlc_media_t *m;
    libvlc_media_player_t *mp;
    int tracks_count;
    libvlc_media_track_t **tracks;

    int video_width;
    int video_height;

    GRAPH * graph;

#ifdef USE_SDL2_GPU
    SDL_Surface * surface;
    int texture_must_update;
#endif

    int64_t objectid;

} MEDIA;

enum {
    MEDIA_STATUS_CLOSE = 0,
    MEDIA_STATUS_OPENING,
    MEDIA_STATUS_PLAYING,
    MEDIA_STATUS_PAUSED,
    MEDIA_STATUS_STOPPING,
    MEDIA_STATUS_ENDED,
    MEDIA_STATUS_ERROR
};

/* --------------------------------------------------------------------------- */

extern void media_init();
extern void media_exit();

extern MEDIA * media_load( const char * media, int64_t * graph_id, int w, int h, int timeout );
extern void media_unload( MEDIA * mh );

extern int media_play( MEDIA * mh );
extern void media_pause_action( MEDIA * mh, int do_pause );
extern void media_pause( MEDIA * mh );
extern void media_resume( MEDIA * mh );
extern void media_stop( MEDIA * mh );

extern int64_t media_get_time( MEDIA * mh );
extern void media_set_time( MEDIA * mh, int64_t i_time);
extern int64_t media_get_duration( MEDIA * mh );

extern int media_get_status( MEDIA * mh );

extern float media_get_rate( MEDIA * mh );
extern int media_set_rate( MEDIA * mh, float rate );
extern void media_next_frame( MEDIA * mh );

extern int media_get_mute( MEDIA * mh );
extern void media_set_mute( MEDIA * mh, int status );
extern int media_get_volume( MEDIA * mh );
extern int media_set_volume( MEDIA * mh, int volume );

/* --------------------------------------------------------------------------- */

#endif
