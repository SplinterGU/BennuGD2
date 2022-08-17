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

#ifdef LIBVLC_ENABLED

#include "bgdrtm.h"
#include "bgddl.h"

#include "libbggfx.h"
#include "libmod_gfx.h"

#include "xstrings.h"

#include "dlvaracc.h"

/* --------------------------------------------------------------------------- */
/* MEDIA                                                                       */
/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_load( INSTANCE * my, int64_t * params ) {
    int64_t ret;
    const char * media = string_get( params[0] );
    ret = ( int64_t ) ( intptr_t ) media_load( media, ( int64_t * ) ( params[1] ), 0, 0, 0 );
    string_discard( params[0] );
    return ret;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_load2( INSTANCE * my, int64_t * params ) {
    int64_t ret;
    const char * media = string_get( params[0] );
    ret = ( int64_t ) ( intptr_t ) media_load( media, ( int64_t * ) ( params[1] ), 0, 0, params[2] );
    string_discard( params[0] );
    return ret;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_load3( INSTANCE * my, int64_t * params ) {
    int64_t ret;
    const char * media = string_get( params[0] );
    ret = ( int64_t ) ( intptr_t ) media_load( media, ( int64_t * ) ( params[1] ), params[2], params[3], 0 );
    string_discard( params[0] );
    return ret;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_load4( INSTANCE * my, int64_t * params ) {
    int64_t ret;
    const char * media = string_get( params[0] );
    ret = ( int64_t ) ( intptr_t ) media_load( media, ( int64_t * ) ( params[1] ), params[2], params[3], params[4] );
    string_discard( params[0] );
    return ret;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_unload( INSTANCE * my, int64_t * params ) {
    media_unload( ( MEDIA * ) ( params[0] ) );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_play( INSTANCE * my, int64_t * params ) {
    return media_play( ( MEDIA * ) ( params[0] ) );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_pause_action( INSTANCE * my, int64_t * params ) {
    media_pause_action( ( MEDIA * ) ( params[0] ), params[1] );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_pause( INSTANCE * my, int64_t * params ) {
    media_pause( ( MEDIA * ) ( params[0] ) );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_resume( INSTANCE * my, int64_t * params ) {
    media_resume( ( MEDIA * ) ( params[0] ) );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_stop( INSTANCE * my, int64_t * params ) {
    media_stop( ( MEDIA * ) ( params[0] ) );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_get_time( INSTANCE * my, int64_t * params ) {
    return media_get_time( ( MEDIA * ) ( params[0] ) );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_set_time( INSTANCE * my, int64_t * params ) {
    media_set_time( ( MEDIA * ) ( params[0] ), params[1] );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_get_duration( INSTANCE * my, int64_t * params ) {
    return media_get_duration( ( MEDIA * ) ( params[0] ) );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_get_status( INSTANCE * my, int64_t * params ) {
    return media_get_status( ( MEDIA * ) ( params[0] ) );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_get_rate( INSTANCE * my, int64_t * params ) {
    float res =  media_get_rate( ( MEDIA * ) ( params[0] ) );
    return *(( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_set_rate( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) media_set_rate( ( MEDIA * ) ( params[0] ), *( float * ) &params[1] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_next_frame( INSTANCE * my, int64_t * params ) {
    media_next_frame( ( MEDIA * ) ( params[0] ) );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_get_mute( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) media_get_mute(( MEDIA * ) ( params[0] ) );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_set_mute( INSTANCE * my, int64_t * params ) {
    media_set_mute( ( MEDIA * ) ( params[0] ), params[1] );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_get_volume( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) media_get_volume( ( MEDIA * ) ( params[0] ) );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_set_volume( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) media_set_volume( ( MEDIA * ) ( params[0] ), params[1] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_get_track( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) media_get_track( ( MEDIA * ) ( params[0] ) );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_set_track( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) media_set_track( ( MEDIA * ) ( params[0] ), params[1] );
}

/* --------------------------------------------------------------------------- */
/* Subtitles                                                                   */
/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_add_subtitle( INSTANCE * my, int64_t * params ) {
    const char * uri = string_get( params[1] );
    int64_t ret = ( int64_t ) media_add_subtitle( ( MEDIA * ) ( params[0] ), uri );
    string_discard( params[1] );
    return ret;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_get_subtitle( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) media_get_subtitle( ( MEDIA * ) ( params[0] ) );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_set_subtitle( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) media_set_subtitle( ( MEDIA * ) ( params[0] ), params[1] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_get_subtitle_delay( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) media_get_subtitle( ( MEDIA * ) ( params[0] ) );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_set_subtitle_delay( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) media_set_subtitle( ( MEDIA * ) ( params[0] ), params[1] );
}

/* --------------------------------------------------------------------------- */
/* Audio                                                                       */
/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_add_audio( INSTANCE * my, int64_t * params ) {
    const char * uri = string_get( params[1] );
    int64_t ret = ( int64_t ) media_add_audio( ( MEDIA * ) ( params[0] ), uri );
    string_discard( params[1] );
    return ret;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_get_audio( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) media_get_audio( ( MEDIA * ) ( params[0] ) );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_set_audio( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) media_set_audio( ( MEDIA * ) ( params[0] ), params[1] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_get_audio_delay( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) media_get_audio_delay( ( MEDIA * ) ( params[0] ) );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_set_audio_delay( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) media_set_audio_delay( ( MEDIA * ) ( params[0] ), params[1] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_get_audio_channel( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) media_get_audio_channel( ( MEDIA * ) ( params[0] ) );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_set_audio_channel( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) media_set_audio_channel( ( MEDIA * ) ( params[0] ), params[1] );
}

/* --------------------------------------------------------------------------- */
/* Tracks                                                                      */
/* --------------------------------------------------------------------------- */

typedef struct libmod_gfx_media_track_t {
    int64_t id;
    int64_t type;
    int64_t language;
    int64_t description;
} libmod_gfx_media_track_t;

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_get_track_list( INSTANCE * my, int64_t * params ) {

    if ( !params[1] ) return -1;

    media_track_t *tracks = NULL;

    int tracks_count = media_get_track_list( ( MEDIA * ) ( params[0] ), &tracks );

    if ( tracks_count < 1 ) return 0;

    libmod_gfx_media_track_t * ts = ( libmod_gfx_media_track_t * ) calloc( tracks_count, sizeof( libmod_gfx_media_track_t ) );

    if ( !ts ) {
        media_track_list_release( &tracks, tracks_count );
        return 0;
    }

    int i;

    for ( i = 0; i < tracks_count; i++ ) {
        ts[i].id = tracks[i].id;
        ts[i].type = tracks[i].type;
        ts[i].language = tracks[i].language ? string_new( tracks[i].language ) : string_new("");
        ts[i].description = tracks[i].description ? string_new( tracks[i].description ) : string_new("");
        string_use( ts[i].language );
        string_use( ts[i].description );
    }

    media_track_list_release( &tracks, tracks_count );

    if ( params[1] ) *(( int64_t * )( intptr_t )( params[1] ) ) = ( int64_t ) ts;

    return tracks_count;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_track_list_release( INSTANCE * my, int64_t * params ) {

    if ( !params[0] || !params[1] ) return 0;

    int tracks_count = params[1];

    if ( tracks_count < 1 ) return 0;

    libmod_gfx_media_track_t ** tracks = ( ( libmod_gfx_media_track_t ** ) ( intptr_t ) ( params[0] ));

    int i;
    for ( i = 0; i < tracks_count; i++ ) {
        string_discard( (*tracks)[i].language );
        string_discard( (*tracks)[i].description );
    }

    free( *tracks );
    *tracks = NULL;

    return 1;
}

/* --------------------------------------------------------------------------- */
/* Chapters                                                                    */
/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_get_chapter( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) media_get_chapter( ( MEDIA * ) ( params[0] ) );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_set_chapter( INSTANCE * my, int64_t * params ) {
    media_set_chapter( ( MEDIA * ) ( params[0] ), params[1] );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_get_chapter_count( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) media_get_chapter_count( ( MEDIA * ) ( params[0] ) );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_prev_chapter( INSTANCE * my, int64_t * params ) {
    media_prev_chapter( ( MEDIA * ) ( params[0] ) );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_next_chapter( INSTANCE * my, int64_t * params ) {
    media_next_chapter( ( MEDIA * ) ( params[0] ) );
    return 1;
}

/* --------------------------------------------------------------------------- */

typedef struct libmod_gfx_media_chapter_t {
    int64_t time_offset;
    int64_t duration;
    int64_t name;
} libmod_gfx_media_chapter_t;

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_get_chapter_list( INSTANCE * my, int64_t * params ) {

    if ( !params[1] ) return -1;

    media_chapter_t *chapters = NULL;

    int chapters_count = media_get_chapter_list( ( MEDIA * ) ( params[0] ), &chapters );

    if ( chapters_count < 1 ) return 0;

    libmod_gfx_media_chapter_t * cs = ( libmod_gfx_media_chapter_t * ) calloc( chapters_count, sizeof( libmod_gfx_media_chapter_t ) );

    if ( !cs ) {
        media_chapter_list_release( &chapters, chapters_count );
        return 0;
    }

    int i;

    for ( i = 0; i < chapters_count; i++ ) {
        cs[i].time_offset = chapters[i].time_offset;
        cs[i].duration = chapters[i].duration;
        cs[i].name = chapters[i].name ? string_new( chapters[i].name ) : string_new(""); string_use( cs[i].name );
    }

    media_chapter_list_release( &chapters, chapters_count );

    if ( params[1] ) *(( int64_t * )( intptr_t )( params[1] ) ) = ( int64_t ) cs;

    return chapters_count;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_chapter_list_release( INSTANCE * my, int64_t * params ) {

    if ( !params[0] || !params[1] ) return 0;

    int chapters_count = params[1];

    if ( chapters_count < 1 ) return 0;

    libmod_gfx_media_chapter_t ** chapters = ( ( libmod_gfx_media_chapter_t ** ) ( intptr_t ) ( params[0] ));

    int i;
    for ( i = 0; i < chapters_count; i++ ) {
        string_discard( (*chapters)[i].name );
    }

    free( *chapters );
    *chapters = NULL;

    return 1;
}

/* --------------------------------------------------------------------------- */

#endif
