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

#ifndef __MEDIA_H
#define __MEDIA_H

#ifdef LIBVLC_ENABLED

#include <SDL.h>
#ifdef USE_SDL2_GPU
    #include <SDL_gpu.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <vlc/vlc.h>

#include "libbggfx.h"

/* --------------------------------------------------------------------------- */

enum {
    MEDIA_STATUS_CLOSE = 0,
    MEDIA_STATUS_OPENING,
    MEDIA_STATUS_PLAYING,
    MEDIA_STATUS_PAUSED,
    MEDIA_STATUS_STOPPING,
    MEDIA_STATUS_ENDED,
    MEDIA_STATUS_ERROR
};

enum {
    MEDIA_TRACK_UNKNOWN = -1,
    MEDIA_TRACK_AUDIO,
    MEDIA_TRACK_VIDEO,
    MEDIA_TRACK_SUBTITLE
};

typedef struct media {
    libvlc_media_t *m;
    libvlc_media_player_t *mp;

    int video_width;
    int video_height;

    GRAPH * graph;

#if defined(USE_SDL2_GPU) || defined(USE_SDL2)
    SDL_Surface * surface;
    int texture_must_update;
#endif

    int64_t objectid;

    // Used for interaction with debbuger && system paused
    int in_hold_state;
    int is_paused;
} MEDIA;

typedef struct media_track_t {
    int id;
    int type;
    char * language;
    char * description;
} media_track_t;

typedef struct media_chapter_t {
    int64_t time_offset;
    int64_t duration;
    char *name;
} media_chapter_t;

/* --------------------------------------------------------------------------- */
/* General                                                                     */
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

extern int media_get_track( MEDIA * mh );
extern int media_set_track( MEDIA * mh, int i_track );

/* --------------------------------------------------------------------------- */
/* Subtitles                                                                   */
/* --------------------------------------------------------------------------- */

extern int media_add_subtitle( MEDIA * mh, const char * uri );
extern int media_get_subtitle( MEDIA * mh );
extern int media_set_subtitle( MEDIA * mh, int id );
extern int64_t media_get_subtitle_delay( MEDIA * mh );
extern int media_set_subtitle_delay( MEDIA * mh, int64_t d );

/* --------------------------------------------------------------------------- */
/* Audio                                                                       */
/* --------------------------------------------------------------------------- */

extern int media_add_audio( MEDIA * mh, const char * uri );
extern int media_get_audio( MEDIA * mh );
extern int media_set_audio( MEDIA * mh, int id );
extern int64_t media_get_audio_delay( MEDIA * mh );
extern int media_set_audio_delay( MEDIA * mh, int64_t d );
extern int media_get_audio_channel( MEDIA * mh );
extern int media_set_audio_channel( MEDIA * mh, int channel );

/* --------------------------------------------------------------------------- */
/* Tracks                                                                      */
/* --------------------------------------------------------------------------- */

extern int media_get_track_list( MEDIA * mh, media_track_t **tracks );
extern void media_track_list_release( media_track_t **tracks, int count );

/* --------------------------------------------------------------------------- */
/* Chapters                                                                    */
/* --------------------------------------------------------------------------- */

extern int media_get_chapter( MEDIA * mh );
extern void media_set_chapter( MEDIA * mh, int i_chapter );
extern int media_get_chapter_count( MEDIA * mh );
extern void media_prev_chapter( MEDIA * mh );
extern void media_next_chapter( MEDIA * mh );
extern int media_get_chapter_list( MEDIA * mh, media_chapter_t **chapters );
extern void media_chapter_list_release( media_chapter_t **chapters, int count );

/* --------------------------------------------------------------------------- */

#endif

#endif
