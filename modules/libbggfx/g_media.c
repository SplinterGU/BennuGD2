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

#include "libbggfx.h"

/* --------------------------------------------------------------------------- */

static libvlc_instance_t *libvlc = NULL;

/* --------------------------------------------------------------------------- */

static void *_media_lock( void * data, void **p_pixels ) {
    MEDIA * mh = ( MEDIA * ) data;
#ifdef USE_SDL2
    *p_pixels = mh->graph->surface->pixels;
#endif
#ifdef USE_SDL2_GPU
    *p_pixels = mh->surface->pixels;
#endif
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
    return NULL;
}

/* --------------------------------------------------------------------------- */

static void _media_display( void *data, void *id ) {
    MEDIA * mh = ( MEDIA * ) data;
#ifdef USE_SDL2
    mh->graph->texture_must_update = 1;
#endif
#ifdef USE_SDL2_GPU
    mh->texture_must_update = 1;
#endif
}

/* --------------------------------------------------------------------------- */

static int __media_info( void * what, REGION * bbox, int64_t * z, int64_t * drawme ) {
#ifdef USE_SDL2_GPU
    MEDIA * mh = ( MEDIA * ) what;

    if ( mh->texture_must_update ) {
        GPU_UpdateImage( mh->graph->tex, NULL, mh->surface, NULL );
        mh->texture_must_update = 0;
    }
#endif
    * drawme = 0;
    return 0;
}

/* --------------------------------------------------------------------------- */

void media_init() {
    const char * vlc_argv[] = { "--verbose=-1" };
    if ( !libvlc ) libvlc = libvlc_new( 1, vlc_argv );
}

/* --------------------------------------------------------------------------- */

void media_exit() {
    if ( libvlc ) libvlc_release( libvlc );
    libvlc = NULL;
}

/* --------------------------------------------------------------------------- */

MEDIA * media_load( const char * media, int64_t * graph_id, int w, int h, int timeout ) {
    int vw, vh;

    if ( !libvlc ) media_init();

    MEDIA * mh = calloc(1, sizeof( MEDIA ) );
    if ( !mh ) return NULL;

    mh->m = libvlc_media_new_path( libvlc, media );
    if ( !mh->m ) mh->m = libvlc_media_new_location( libvlc, media );
    if ( !mh->m ) {
        free( mh );
        return NULL;
    }

    mh->mp = libvlc_media_player_new_from_media( mh->m );
    if ( !mh->mp ) {
        libvlc_media_release( mh->m );
        free( mh );
        return NULL;
    }

    libvlc_media_parse_flag_t parse_flag;

    if ( !strcmp( media, "://") ) parse_flag = libvlc_media_parse_network;
    else                          parse_flag = libvlc_media_parse_local;

    if ( libvlc_media_parse_with_options( mh->m, parse_flag, timeout ) ) {
        libvlc_media_release( mh->m );
        libvlc_media_player_release( mh->mp );
        free( mh );
        return NULL;        
    }
    
    libvlc_media_parsed_status_t parse_status;

    while( !( parse_status = libvlc_media_get_parsed_status( mh->m ) ) ) SDL_Delay(0);

    switch( parse_status ) {
        case libvlc_media_parsed_status_skipped:
        case libvlc_media_parsed_status_failed:
        case libvlc_media_parsed_status_timeout:
        default:
            libvlc_media_release( mh->m );
            libvlc_media_player_release( mh->mp );
            free( mh );
            return NULL;
            break;

        case libvlc_media_parsed_status_done:
            {
                if ( libvlc_video_get_size( mh->mp, 0, &vw, &vh ) != -1 ) {
//                    GRAPH * bitmap = NULL;

                    if ( w && h ) {
                        mh->video_width = w;
                        mh->video_height = h;
                    } else if ( !w && !h ) {
                        mh->video_width = vw;
                        mh->video_height = vh;
                    } else if ( w ) {
                        mh->video_width = w;
                        mh->video_height = w * ( ( float ) vh / ( float ) vw );
                    } else {
                        mh->video_height = h;
                        mh->video_width = h * ( ( float ) vw / ( float ) vh );
                    }

                    mh->surface = SDL_CreateRGBSurface(0, mh->video_width, mh->video_height, 32, 0, 0, 0, 0 /* Force alpha to opaque */ );
                    if ( !mh->surface ) {
                        libvlc_media_release( mh->m );
                        libvlc_media_player_release( mh->mp );
                        free( mh );
                        return NULL;
                    }
                    SDL_SetColorKey( mh->surface, SDL_FALSE, 0 );

                    mh->graph = bitmap_new( 0, mh->video_width, mh->video_height, mh->surface );
                    if ( !mh->graph ) {
                        libvlc_media_release( mh->m );
                        libvlc_media_player_release( mh->mp );
                        SDL_FreeSurface( mh->surface );
                        free( mh );
                        return NULL;
                    }

                    *graph_id = ( int64_t ) ( mh->graph->code = bitmap_next_code() );

                    grlib_add_map( 0, mh->graph );

                    libvlc_video_set_callbacks( mh->mp, _media_lock, NULL, _media_display, mh );

                    libvlc_video_set_format( mh->mp, "RV32", mh->video_width, mh->video_height, mh->video_width * 4 );

                    mh->objectid = gr_new_object( INT64_MIN + 10, __media_info, NULL, ( void * ) mh );

                }
                return mh ;
            }
            break;
    }

    return NULL;
}

/* --------------------------------------------------------------------------- */

void media_unload( MEDIA * mh ) {
    if ( mh ) {
        libvlc_media_player_stop( mh->mp );

        if ( mh->objectid ) gr_destroy_object( mh->objectid );

        libvlc_media_slaves_clear( mh->m );
        libvlc_media_release( mh->m );
        libvlc_media_player_release( mh->mp );
        if ( mh->surface ) SDL_FreeSurface( mh->surface );
        if ( mh->graph ) grlib_unload_map( 0, mh->graph->code );

        free( mh );
    }
}

/* --------------------------------------------------------------------------- */

int media_play( MEDIA * mh ) {
    if ( !mh ) return -1;
    return libvlc_media_player_play( mh->mp );
}

/* --------------------------------------------------------------------------- */

void media_pause_action( MEDIA * mh, int do_pause ) {
    if ( !mh ) return;
    libvlc_media_player_set_pause( mh->mp, do_pause );
}

/* --------------------------------------------------------------------------- */

void media_pause( MEDIA * mh ) {
    if ( !mh ) return;
    libvlc_media_player_set_pause( mh->mp, 1 );
}

/* --------------------------------------------------------------------------- */

void media_resume( MEDIA * mh ) {
    if ( !mh ) return;
    libvlc_media_player_set_pause( mh->mp, 0 );
}

/* --------------------------------------------------------------------------- */

void media_stop( MEDIA * mh ) {
    if ( !mh ) return;
    libvlc_media_player_stop( mh->mp );
}

/* --------------------------------------------------------------------------- */

int64_t media_get_time( MEDIA * mh ) {
    if ( !mh ) return -1;
    return ( int64_t ) libvlc_media_player_get_time( mh->mp );
}

/* --------------------------------------------------------------------------- */

void media_set_time( MEDIA * mh, int64_t i_time) {
    if ( !mh ) return;
    libvlc_media_player_set_time( mh->mp, i_time );
}

/* --------------------------------------------------------------------------- */

int64_t media_get_duration( MEDIA * mh ) {
    if ( !mh ) return -1;
    return ( int64_t ) libvlc_media_player_get_length( mh->mp );
}

/* --------------------------------------------------------------------------- */

int media_get_status( MEDIA * mh ) {
    if ( !mh ) return -1;
    int st = libvlc_media_player_get_state( mh->mp );

    switch( st ) {
        case libvlc_NothingSpecial:
            return MEDIA_STATUS_CLOSE;

        case libvlc_Opening:
            return MEDIA_STATUS_OPENING;

        case libvlc_Playing:
            return MEDIA_STATUS_PLAYING;

        case libvlc_Paused:
            return MEDIA_STATUS_PAUSED;

        case libvlc_Stopped:
            return MEDIA_STATUS_STOPPING;

        case libvlc_Ended:
            return MEDIA_STATUS_ENDED;

        case libvlc_Error:
            return MEDIA_STATUS_ERROR;
    }

    return -1;
}

/* --------------------------------------------------------------------------- */

float media_get_rate( MEDIA * mh ) {
    if ( !mh ) return 1.0f;
    return libvlc_media_player_get_rate( mh->mp );
}

/* --------------------------------------------------------------------------- */

int media_set_rate( MEDIA * mh, float rate ) {
    if ( !mh ) return -1;
    return libvlc_media_player_set_rate( mh->mp, rate );
}

/* --------------------------------------------------------------------------- */

void media_next_frame( MEDIA * mh ) {
    if ( !mh ) return;
    libvlc_media_player_next_frame( mh->mp );
}

/* --------------------------------------------------------------------------- */

int media_get_mute( MEDIA * mh ) {
    if ( !mh ) return -1;
    return libvlc_audio_get_mute( mh->mp );
}

/* --------------------------------------------------------------------------- */

void media_set_mute( MEDIA * mh, int status ) {
    if ( !mh ) return;
    libvlc_audio_set_mute( mh->mp, status );
}

/* --------------------------------------------------------------------------- */

int media_get_volume( MEDIA * mh ) {
    if ( !mh ) return -1;
    return libvlc_audio_get_volume( mh->mp );
}

/* --------------------------------------------------------------------------- */

int media_set_volume( MEDIA * mh, int volume ) {
    if ( !mh ) return -1;
    return libvlc_audio_set_volume( mh->mp, volume );
}

/* --------------------------------------------------------------------------- */

int media_get_track( MEDIA * mh ) {
    if ( !mh ) return -1;
    return libvlc_video_get_track( mh->mp );
}

/* --------------------------------------------------------------------------- */

int media_set_track( MEDIA * mh, int i_track ) {
    if ( !mh ) return -1;
    return libvlc_video_set_track( mh->mp, i_track );
}

/* --------------------------------------------------------------------------- */
/* Slaves                                                                      */
/* --------------------------------------------------------------------------- */

static int __get_slave_id_by_uri( MEDIA * mh, const char * uri, libvlc_media_slave_type_t type ) {
    int res = 0, i;
    if ( !mh ) return 0;
    libvlc_media_slave_t **slaves_list = NULL;
    unsigned int slaves = libvlc_media_slaves_get( mh->m, &slaves_list );
    for ( i = 0; i < slaves; i++ ) if ( slaves_list[i]->i_type == type && ( res = ( !strcmp( slaves_list[i]->psz_uri, uri ) ) ) ) break;
    libvlc_media_slaves_release( slaves_list, slaves );
    return res;
}

/* --------------------------------------------------------------------------- */

static int __media_add_slave( MEDIA * mh, const char * uri, libvlc_media_slave_type_t type ) {
    int res = 0;

    if ( !mh ) return -1;
    // TODO: save subtitle list

    char * resource = ( char * ) uri;

    if ( strcmp( uri, "://") ) { 
        resource = malloc( strlen ( uri ) + 8 /*strlen( "file://" ) + 1*/ );
        strcpy( resource, "file://" );
        strcat( resource, uri );
    }

    if ( !__get_slave_id_by_uri( mh, resource, type ) ) res = libvlc_media_player_add_slave( mh->mp, type, resource, true );

    if ( resource != uri ) free( resource );

    return res;
}

/* --------------------------------------------------------------------------- */
/* Subtitles                                                                   */
/* --------------------------------------------------------------------------- */

int media_add_subtitle( MEDIA * mh, const char * uri ) {
    return __media_add_slave( mh, uri, libvlc_media_slave_type_subtitle );
}

/* --------------------------------------------------------------------------- */

int media_get_subtitle( MEDIA * mh ) {
    if ( !mh ) return -1;
    return libvlc_video_get_spu( mh->mp );
}

/* --------------------------------------------------------------------------- */

int media_set_subtitle( MEDIA * mh, int id ) {
    if ( !mh ) return -1;
    return libvlc_video_set_spu( mh->mp, id );
}

/* --------------------------------------------------------------------------- */

int64_t media_get_subtitle_delay( MEDIA * mh ) {
    if ( !mh ) return -1;
    return libvlc_video_get_spu_delay( mh->mp );
}

/* --------------------------------------------------------------------------- */

int media_set_subtitle_delay( MEDIA * mh, int64_t d ) {
    if ( !mh ) return -1;
    return libvlc_video_set_spu_delay( mh->mp, d );
}

/* --------------------------------------------------------------------------- */
/* Audio                                                                       */
/* --------------------------------------------------------------------------- */

int media_add_audio( MEDIA * mh, const char * uri ) {
    return __media_add_slave( mh, uri, libvlc_media_slave_type_audio );
}

/* --------------------------------------------------------------------------- */

int media_get_audio( MEDIA * mh ) {
    if ( !mh ) return -1;
    return libvlc_audio_get_track( mh->mp );
}

/* --------------------------------------------------------------------------- */

int media_set_audio( MEDIA * mh, int id ) {
    if ( !mh ) return -1;
    return libvlc_audio_set_track( mh->mp, id );
}

/* --------------------------------------------------------------------------- */

int64_t media_get_audio_delay( MEDIA * mh ) {
    if ( !mh ) return -1;
    return libvlc_audio_get_delay( mh->mp );
}

/* --------------------------------------------------------------------------- */

int media_set_audio_delay( MEDIA * mh, int64_t d ) {
    if ( !mh ) return -1;
    return libvlc_audio_set_delay( mh->mp, d );
}

/* --------------------------------------------------------------------------- */

int media_get_audio_channel( MEDIA * mh ) {
    if ( !mh ) return -1;
    return libvlc_audio_get_channel( mh->mp );
}

/* --------------------------------------------------------------------------- */

int media_set_audio_channel( MEDIA * mh, int channel ) {
    if ( !mh ) return -1;
    return libvlc_audio_set_channel( mh->mp, channel );
}

/* --------------------------------------------------------------------------- */
/* Tracks                                                                      */
/* --------------------------------------------------------------------------- */

int media_get_track_list( MEDIA * mh, media_track_t **tracks ) {

    if ( !tracks ) return 0;

    libvlc_media_track_t **tl = NULL;

    int tracks_count = libvlc_media_tracks_get( mh->m, &tl );

    if ( tracks_count > 0 ) {
        media_track_t *t = ( media_track_t * ) calloc( tracks_count, sizeof( media_track_t ) );

        if ( !t ) {
            libvlc_media_tracks_release( tl, tracks_count );
            return 0;
        }

        int i;

        for ( i = 0; i < tracks_count; i++ ) {
            t[i].id = tl[i]->i_id;
            t[i].description = tl[i]->psz_description ? strdup( tl[i]->psz_description ) : NULL;
            t[i].language = tl[i]->psz_language ? strdup( tl[i]->psz_language ) : NULL;

            switch( tl[i]->i_type ) {
                case libvlc_track_unknown:
                    t[i].type = MEDIA_TRACK_UNKNOWN;
                    break;

                case libvlc_track_audio:
                    t[i].type = MEDIA_TRACK_AUDIO;
                    break;

                case libvlc_track_video:
                    t[i].type = MEDIA_TRACK_VIDEO;
                    break;

                case libvlc_track_text:
                    t[i].type = MEDIA_TRACK_SUBTITLE;
                    break;
            }
        }
        libvlc_media_tracks_release( tl, tracks_count );
        *tracks = t;
    }

    return tracks_count;
}

/* --------------------------------------------------------------------------- */

void media_track_list_release( media_track_t **tracks, int count ) {
    if ( tracks && *tracks ) {
        int i;
        for ( i = 0; i < count; i++ ) {
            if ( (*tracks)[i].description ) free( (*tracks)[i].description );
            if ( (*tracks)[i].language ) free( (*tracks)[i].language );
        }
        free( *tracks );
        *tracks = NULL;
    }
}

/* --------------------------------------------------------------------------- */
/* Chapters                                                                    */
/* --------------------------------------------------------------------------- */

int media_get_chapter( MEDIA * mh ) {
    if ( !mh ) return -1;
    return libvlc_media_player_get_chapter( mh->mp );
}

/* --------------------------------------------------------------------------- */

void media_set_chapter( MEDIA * mh, int i_chapter ) {
    if ( !mh ) return;
    libvlc_media_player_set_chapter( mh->mp, i_chapter );
}

/* --------------------------------------------------------------------------- */

int media_get_chapter_count( MEDIA * mh ) {
    if ( !mh ) return -1;
    return libvlc_media_player_get_chapter_count( mh->mp );
}

/* --------------------------------------------------------------------------- */

void media_prev_chapter( MEDIA * mh ) {
    if ( !mh ) return;
    libvlc_media_player_previous_chapter( mh->mp );
}

/* --------------------------------------------------------------------------- */

void media_next_chapter( MEDIA * mh ) {
    if ( !mh ) return;
    libvlc_media_player_next_chapter( mh->mp );
}

/* --------------------------------------------------------------------------- */

int media_get_chapter_list( MEDIA * mh, media_chapter_t **chapters ) {

    if ( !chapters ) return 0;

    libvlc_chapter_description_t **cd = NULL;

    int chapters_count = libvlc_media_player_get_full_chapter_descriptions( mh->mp, -1, &cd );

    if ( chapters_count > 0 ) {
        media_chapter_t *c = ( media_chapter_t * ) calloc( chapters_count, sizeof( media_chapter_t ) );

        if ( !c ) {
            libvlc_chapter_descriptions_release( cd, chapters_count );
            return 0;
        }

        int i;

        for ( i = 0; i < chapters_count; i++ ) {
            c[i].time_offset = cd[i]->i_time_offset;
            c[i].duration = cd[i]->i_duration;
            c[i].name = cd[i]->psz_name ? strdup( cd[i]->psz_name ) : NULL;
        }
        libvlc_chapter_descriptions_release( cd, chapters_count );
        *chapters = c;
    }

    return chapters_count;
}

/* --------------------------------------------------------------------------- */

void media_chapter_list_release( media_chapter_t **chapters, int count ) {
    if ( chapters && *chapters ) {
        int i;
        for ( i = 0; i < count; i++ ) {
            if ( (*chapters)[i].name ) free( (*chapters)[i].name );
        }
        free( *chapters );
        *chapters = NULL;
    }
}

/* --------------------------------------------------------------------------- */
