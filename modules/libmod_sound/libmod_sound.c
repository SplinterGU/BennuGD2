/*
 *  Copyright (C) SplinterGU (Fenix/BennuGD) (Since 2006)
 *  Copyright (C) 2002-2006 Fenix Team (Fenix)
 *  Copyright (C) 1999-2002 José Luis Cebrián Pagüe (Fenix)
 *
 *  This file is part of Bennu Game Development
 *
 *  This software is provided 'as-is', without any express or implied
 *  warranty. In no event will the authors be held liable for any damages
 *  arising from the use of this software.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "bgddl.h"
#include "dlvaracc.h"
#include "xstrings.h"
#include "bgload.h"

#include "libbgsound.h"
#include "libmod_sound.h"

/* --------------------------------------------------------------------------- */

enum {
    SOUND_FREQ = 0,
    SOUND_MODE,
    SOUND_CHANNELS
};

/* --------------------------------------------------------------------------- */
/* These are the variables that are intended to be accessed.                   */
/* The interpreter populates this structure if the variable exists.            */
/* (used at runtime)                                                           */

DLVARFIXUP  __bgdexport( libmod_sound, globals_fixup )[] =
{
    /* Global variable name, pointer to data, size of the element, number of elements */
    { "sound.freq"          , NULL, -1, -1 },
    { "sound.mode"          , NULL, -1, -1 },
    { "sound.channels"      , NULL, -1, -1 },
    { NULL                  , NULL, -1, -1 }
};

/* --------------------------------------------------------------------------- */
/* BennuGD Wrapper Functions                                                   */
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_init
 *
 *  Initializes the audio system
 *
 *  PARAMS:
 *
 *  no params
 *
 *  RETURN VALUE:
 *
 *  -1 if there is any error
 *  0 if all goes ok
 *
 */

static int64_t libmod_sound_init( INSTANCE * my, int64_t * params ) {
    int freq = GLOQWORD( libmod_sound, SOUND_FREQ );
    int mode = GLOQWORD( libmod_sound, SOUND_MODE );
    int channels = GLOQWORD( libmod_sound, SOUND_CHANNELS );
    return ( int64_t ) bg_sound_init( freq, mode, channels );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_quit
 *
 *  Close all audio subsystem
 *
 *  PARAMS:
 *
 *  no params
 *
 *  RETURN VALUE:
 *
 *  0
 *
 */

static int64_t libmod_sound_quit( INSTANCE * my, int64_t * params ) {
    bg_sound_quit();
    return 0;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_load_music
 *
 *  Load a MOD from a file
 *
 *  PARAMS:
 *      file name
 *
 *  RETURN VALUE:
 *
 *      mod id
 *
 */

static int64_t libmod_sound_load_music( INSTANCE * my, int64_t * params ) {
    int64_t var;
    const char * filename ;

    if ( !bg_sound_is_initialized() && libmod_sound_init( NULL, NULL ) == -1 ) return 0;

    if ( !( filename = string_get( params[0] ) ) ) return ( 0 ) ;

    var = bg_sound_load_music( filename );
    string_discard( params[0] );

    return ( var );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_bgload_music
 *
 *  Load a MOD from a file
 *
 *  PARAMS:
 *      file name
 *      pointer mod id
 *
 *  RETURN VALUE:
 *
 *
 */

static int64_t libmod_sound_bgload_music( INSTANCE * my, int64_t * params ) {
    bgload( ( int64_t ( * )( void * ) )bg_sound_load_music, params );
    return 0;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_play_music
 *
 *  Play a MOD
 *
 *  PARAMS:
 *      mod id;
 *      number of loops (-1 infinite loops)
 *
 *  RETURN VALUE:
 *
 *  -1 if there is any error
 *  0 if all goes ok
 *
 */

static int64_t libmod_sound_play_music( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_play_music( params[0], ( int )params[1] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_play_music_looped
 *
 *  Play a MOD looped
 *
 *  PARAMS:
 *      mod id;
 *
 *  RETURN VALUE:
 *
 *  -1 if there is any error
 *  0 if all goes ok
 *
 */

static int64_t libmod_sound_play_music_looped( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_play_music( params[0], -1 );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_unload_music
 *
 *  Frees the resources from a MOD and unloads it
 *
 *  PARAMS:
 *      mod id;
 *
 *  RETURN VALUE:
 *
 *  -1 if there is any error
 *  0 if all goes ok
 *
 */

static int64_t libmod_sound_unload_music( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_unload_music( params[0] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_unload_music_and_free
 *
 *  Frees the resources from a MOD and unloads it
 *
 *  PARAMS:
 *      mod *id;
 *
 *  RETURN VALUE:
 *
 *  -1 if there is any error
 *  0 if all goes ok
 *
 */

static int64_t libmod_sound_unload_music_and_free( INSTANCE * my, int64_t * params ) {
    int64_t *s = ( int64_t * )( intptr_t )( params[0] ), r;
    if ( !s || *s == -1LL ) return -1;
    r = ( int64_t ) bg_sound_unload_music( *s );
    *s = 0LL;
    return( r );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_stop_music
 *
 *  Stop the play of a mod
 *
 *  PARAMS:
 *
 *  no params
 *
 *  RETURN VALUE:
 *
 *  -1 if there is any error
 *  0 if all goes ok
 *
 */

static int64_t libmod_sound_stop_music( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_stop_music();
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_pause_music
 *
 *  Pause the mod in curse, you can resume it after
 *
 *  PARAMS:
 *
 *  no params
 *
 *  RETURN VALUE:
 *
 *  -1 if there is any error
 *  0 if all goes ok
 *
 */

static int64_t libmod_sound_pause_music( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_pause_music();
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_is_paused_music
 *
 *  Query is music is paused
 *
 *  PARAMS:
 *
 *  no params
 *
 *  RETURN VALUE:
 *
 *  -1 if there is any error
 *  0 if all goes ok
 *
 */

static int64_t libmod_sound_is_paused_music( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_is_paused_music();
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_resume_music
 *
 *  Resume the mod, paused before
 *
 *  PARAMS:
 *
 *  no params
 *
 *  RETURN VALUE:
 *
 *  -1 if there is any error
 *  0 if all goes ok
 *
 */

static int64_t libmod_sound_resume_music( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_resume_music();
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_is_playing_music
 *
 *  Check if there is any mod playing
 *
 *  PARAMS:
 *
 *  no params
 *
 *  RETURN VALUE:
 *
 *  -1 if there is any error
 *  TRUE OR FALSE if there is no error
 *
 */

static int64_t libmod_sound_is_playing_music( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_is_playing_music();
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_fade_music_in
 *
 *  Fade in a music from its beginning
 *
 *  PARAMS:
 *      mod id;
 *      number of loops (-1 infinite loops);
 *      time to fade in in milliseconds
 *
 *  RETURN VALUE:
 *
 *  -1 if there is any error
 *  0 if all goes ok
 *
 */

static int64_t libmod_sound_fade_music_in( INSTANCE * my, int64_t * params ) {
    if ( !bg_sound_is_initialized() && libmod_sound_init( NULL, NULL ) == -1 ) return -1;
    return ( int64_t ) bg_sound_fade_music_in( params[0], ( int )params[1], ( int )params[2] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_fade_music_in_pos
 *
 *  Fade in a music from a specified position
 *
 *  PARAMS:
 *      mod id;
 *      number of loops (-1 infinite loops);
 *      time to fade in in milliseconds;
 *      start position in seconds
 *
 *  RETURN VALUE:
 *
 *  -1 if there is any error
 *  0 if all goes ok
 *
 */

static int64_t libmod_sound_fade_music_in_pos( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_fade_music_in_pos( params[0], ( int )params[1], ( int )params[2], *( double * ) &params[3] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_fade_music_off
 *
 *  Fade out the music
 *
 *  PARAMS:
 *
 *  int milliseconds
 *
 *  RETURN VALUE:
 *
 *  -1 if there is any error
 *  0 if all goes ok
 *
 */

static int64_t libmod_sound_fade_music_off( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_fade_music_off( ( int )params[0] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_is_fading_music
 *
 *  Query is music if fading
 *
 *  PARAMS:
 *
 *  RETURN VALUE:
 *
 * -1 if there is any error
 *
 */

static int64_t libmod_sound_is_fading_music( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_is_fading_music();
}

/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_set_music_volume
 *
 *  Set the volume for mod playing (0-128)
 *
 *  PARAMS:
 *
 *  int volume
 *
 *  RETURN VALUE:
 *
 *  -1 if there is any error
 *  0 if there is no error
 *
 */

static int64_t libmod_sound_set_music_volume( INSTANCE * my, int64_t * params ) {
    if ( !bg_sound_is_initialized() && libmod_sound_init( NULL, NULL ) == -1 ) return -1;
    return ( int64_t ) bg_sound_set_music_volume( ( int )params[0] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_get_music_volume
 *
 *  Get the current volume value for a music object
 *
 *  PARAMS:
 *
 *      mod pointer
 *
 *  RETURN VALUE:
 *
 *      between 0 - MIN_MAX_VOLUME (128)
 *
 */

static int64_t libmod_sound_get_music_volume( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_get_music_volume( params[0] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_music_rewind
 *
 *  Rewind a music
 *
 *  PARAMS:
 *
 *  RETURN VALUE:
 *
 *  0
 *
 */

static int64_t libmod_sound_music_rewind( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_music_rewind();
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_set_music_playback_position
 *
 *  Set the current position in the music stream, in seconds.
 *
 *  To convert from milliseconds, divide by 1000.0.
 *
 *   This function is only implemented for MOD music formats ( set pattern order
 *   number ) and for WAV, OGG, FLAC, MP3, and MODPLUG music at the moment.
 *
 *  PARAMS:
 *
 *  position    time in seconds with decimals.
 *
 *  RETURN VALUE:
 *
 *  -1 if there is any error
 *  0 if there is no error
 *
 */

static int64_t libmod_sound_set_music_playback_position( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_set_music_playback_position( *( double * ) &params[0] );
}


/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_get_music_playback_position
 *
 *  Get the current position in the music stream, in seconds.
 *
 *  To convert from milliseconds, divide by 1000.0.
 *
 *   This function is only implemented for MOD music formats ( set pattern order
 *   number ) and for WAV, OGG, FLAC, MP3, and MODPLUG music at the moment.
 *
 *  PARAMS:
 *
 *  mod id
 *
 *  RETURN VALUE:
 *
 *  -1 if there is any error
 *  other value position
 *
 */

static int64_t libmod_sound_get_music_playback_position( INSTANCE * my, int64_t * params ) {
    double res = bg_sound_get_music_playback_position( params[0] );
    return *( ( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_get_music_duration
 *
 *  Get a music object's duration, in seconds.
 *
 *  To convert to milliseconds, multiply by 1000.0.
 *
 *  PARAMS:
 *
 *  music       pointer to Mix_Music structure
 *              if 0, returns duration of current playing music.
 *
 *  RETURN VALUE:
 *
 *  -1 if there is any error
 *  other value     return duration in seconds
 *
 */

static int64_t libmod_sound_get_music_duration( INSTANCE * my, int64_t * params ) {
    double res = bg_sound_get_music_duration( params[0] );
    return *( ( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_get_music_duration_current
 *
 *  Get a music object's duration, in seconds.
 *
 *  To convert to milliseconds, multiply by 1000.0.
 *
 *  PARAMS:
 *
 *  none
 *
 *  RETURN VALUE:
 *
 *  -1 if there is any error
 *  other value     return duration in seconds
 *
 */

static int64_t libmod_sound_get_music_duration_current( INSTANCE * my, int64_t * params ) {
    double res = bg_sound_get_music_duration( 0 );
    return *( ( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_get_music_loop_start_time
 *
 *  Get the start time of the loop in the music stream, in seconds.
 *
 *  PARAMS:
 *
 *  id          identifier for the music
 *
 *  RETURN VALUE:
 *
 *  -1.0 if there is any error or if loop start time is not available
 *  other value loop start time in seconds
 *
 */

static int64_t libmod_sound_get_music_loop_start_time( INSTANCE * my, int64_t * params ) {
    double res = bg_sound_get_music_loop_start_time( params[0] );
    return *( ( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_get_music_loop_start_time_current
 *
 *  Get the start time of the loop in the music stream, in seconds.
 *
 *  PARAMS:
 *
 *  none
 *
 *  RETURN VALUE:
 *
 *  -1.0 if there is any error or if loop start time is not available
 *  other value loop start time in seconds
 *
 */

static int64_t libmod_sound_get_music_loop_start_time_current( INSTANCE * my, int64_t * params ) {
    double res = bg_sound_get_music_loop_start_time( 0 );
    return *( ( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_get_music_loop_end_time
 *
 *  Get the end time of the loop in the music stream, in seconds.
 *
 *  PARAMS:
 *
 *  id          identifier for the music
 *
 *  RETURN VALUE:
 *
 *  -1.0 if there is any error or if loop end time is not available
 *  other value loop end time in seconds
 *
 */

static int64_t libmod_sound_get_music_loop_end_time( INSTANCE * my, int64_t * params ) {
    double res = bg_sound_get_music_loop_end_time( params[0] );
    return *( ( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_get_music_loop_end_time_current
 *
 *  Get the end time of the loop in the music stream, in seconds.
 *
 *  PARAMS:
 *
 *  none
 *
 *  RETURN VALUE:
 *
 *  -1.0 if there is any error or if loop end time is not available
 *  other value loop end time in seconds
 *
 */

static int64_t libmod_sound_get_music_loop_end_time_current( INSTANCE * my, int64_t * params ) {
    double res = bg_sound_get_music_loop_end_time( 0 );
    return *( ( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_get_music_loop_length_time
 *
 *  Get the length of the loop in the music stream, in seconds.
 *
 *  PARAMS:
 *
 *  id          identifier for the music
 *
 *  RETURN VALUE:
 *
 *  -1.0 if there is any error or if loop length time is not available
 *  other value loop length time in seconds
 *
 */

static int64_t libmod_sound_get_music_loop_length_time( INSTANCE * my, int64_t * params ) {
    double res = bg_sound_get_music_loop_length_time( params[0] );
    return *( ( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_get_music_loop_length_time_current
 *
 *  Get the length of the loop in the music stream, in seconds.
 *
 *  PARAMS:
 *
 *  none
 *
 *  RETURN VALUE:
 *
 *  -1.0 if there is any error or if loop length time is not available
 *  other value loop length time in seconds
 *
 */

static int64_t libmod_sound_get_music_loop_length_time_current( INSTANCE * my, int64_t * params ) {
    double res = bg_sound_get_music_loop_length_time( 0 );
    return *( ( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_load_sound
 *
 *  Load a WAV from a file
 *
 *  PARAMS:
 *      file name
 *
 *  RETURN VALUE:
 *
 *  wav pointer
 *
 */

static int64_t libmod_sound_load_sound( INSTANCE * my, int64_t * params ) {
    int64_t var;
    const char * filename ;

    if ( !bg_sound_is_initialized() && libmod_sound_init( NULL, NULL ) == -1 ) return 0;

    if ( !( filename = string_get( params[0] ) ) ) return ( 0 ) ;
    var = bg_sound_load_sound( filename );
    string_discard( params[0] );
    return ( var );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_bgload_sound
 *
 *  Load a WAV from a file
 *
 *  PARAMS:
 *      file name
 *      pointer wav id
 *
 *  RETURN VALUE:
 *
 *
 */

static int64_t libmod_sound_bgload_sound( INSTANCE * my, int64_t * params ) {
    bgload( ( int64_t ( * )( void * ) )bg_sound_load_sound, params );
    return 0;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_unload_sound
 *
 *  Frees the resources from a WAV and unloads it
 *
 *  PARAMS:
 *      wav id
 *
 *  RETURN VALUE:
 *
 *  -1 if there is any error
 *  0 if all goes ok
 *
 */

static int64_t libmod_sound_unload_sound( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_unload_sound( params[0] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_unload_sound_and_free
 *
 *  Frees the resources from a WAV and unloads it
 *
 *  PARAMS:
 *      wav *id
 *
 *  RETURN VALUE:
 *
 *  -1 if there is any error
 *  0 if all goes ok
 *
 */

static int64_t libmod_sound_unload_sound_and_free( INSTANCE * my, int64_t * params ) {
    int64_t *s = ( int64_t * )( intptr_t )( params[0] ), r;
    if ( !s || *s == -1LL ) return -1;
    r = ( int64_t ) bg_sound_unload_sound( *s );
    *s = 0LL;
    return( r );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_play_sound
 *
 *  Play a WAV
 *
 *  PARAMS:
 *      wav id;
 *      number of loops (-1 infinite loops)
 *
 *  RETURN VALUE:
 *
 *  channel number
 *
 */

static int64_t libmod_sound_play_sound( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_play_sound( params[0], ( int )params[1], -1 );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_play_sound_once
 *
 *  Play a WAV once
 *
 *  PARAMS:
 *      wav id;
 *
 *  RETURN VALUE:
 *
 *  channel number
 *
 */

static int64_t libmod_sound_play_sound_once( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_play_sound( params[0], 0, -1 );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_play_sound_channel
 *
 *  Play a WAV on a channel
 *
 *  PARAMS:
 *      wav id;
 *      number of loops (-1 infinite loops);
 *      channel number (-1 first free channel)
 *
 *  RETURN VALUE:
 *
 *  channel number
 *
 */

static int64_t libmod_sound_play_sound_channel( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_play_sound( params[0], ( int )params[1], ( int )params[2] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_play_sound_timed
 *
 *  Play a WAV on a channel for a fixed time
 *
 *  PARAMS:
 *      wav id;
 *      number of loops (-1 infinite loops);
 *      channel number (-1 first free channel)
 *      time to play sound in milliseconds
 *
 *  RETURN VALUE:
 *
 *  channel number
 *
 */

static int64_t libmod_sound_play_sound_timed( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_play_sound_timed( params[0], ( int )params[1], ( int )params[2], ( int )params[3] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_stop_sound
 *
 *  Stop a sound playing on a channel
 *
 *  PARAMS:
 *      channel number
 *
 *  RETURN VALUE:
 *
 *  0
 *
 */

static int64_t libmod_sound_stop_sound( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_stop_sound( ( int )params[0] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_pause_sound
 *
 *  Pause a sound playing on a channel
 *
 *  PARAMS:
 *      channel number
 *
 *  RETURN VALUE:
 *
 *  0
 *
 */

static int64_t libmod_sound_pause_sound( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_pause_sound( ( int )params[0] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_resume_sound
 *
 *  Resume a sound playing on a channel
 *
 *  PARAMS:
 *      channel number
 *
 *  RETURN VALUE:
 *
 *  0
 *
 */

static int64_t libmod_sound_resume_sound( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_resume_sound( ( int )params[0] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_is_playing_sound
 *
 *  Is a sound playing on a channel?
 *
 *  PARAMS:
 *      channel number
 *
 *  RETURN VALUE:
 *
 *  1 if playing, 0 if not
 *
 */

static int64_t libmod_sound_is_playing_sound( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_is_playing_sound( ( int )params[0] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_is_paused_sound
 *
 *  Is a sound paused on a channel?
 *
 *  PARAMS:
 *      channel number
 *
 *  RETURN VALUE:
 *
 *  1 if paused, 0 if not
 *
 */

static int64_t libmod_sound_is_paused_sound( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_is_paused_sound( ( int )params[0] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_is_fading_channel
 *
 *  Is a sound fading on a channel?
 *
 *  PARAMS:
 *      channel number
 *
 *  RETURN VALUE:
 *
 *  1 if fading, 0 if not
 *
 */

static int64_t libmod_sound_is_fading_channel( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_is_fading_channel( ( int )params[0] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_fade_sound_in
 *
 *  Play a WAV on a channel and fade it in
 *
 *  PARAMS:
 *      wav id;
 *      number of loops (-1 infinite loops);
 *      time to fade in in milliseconds
 *      channel number (-1 first free channel)
 *
 *  RETURN VALUE:
 *
 *  channel number
 *
 */

static int64_t libmod_sound_fade_sound_in( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_fade_sound_in( params[0], ( int )params[1], ( int )params[2], ( int )params[3] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_fade_sound_in_timed
 *
 *  Play a WAV on a channel and fade it in for a fixed time
 *
 *  PARAMS:
 *      wav id;
 *      number of loops (-1 infinite loops);
 *      time to fade in in milliseconds
 *      channel number (-1 first free channel)
 *      time to play sound in milliseconds
 *
 *  RETURN VALUE:
 *
 *  channel number
 *
 */

static int64_t libmod_sound_fade_sound_in_timed( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_fade_sound_in_timed( params[0], ( int )params[1], ( int )params[2], ( int )params[3], ( int )params[4] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_fade_channel_off
 *
 *  Fade out a sound on a channel over a fixed time and stop it
 *
 *  PARAMS:
 *      channel number
 *      time to fade out in milliseconds
 *
 *  RETURN VALUE:
 *
 *  0
 *
 */

static int64_t libmod_sound_fade_channel_off( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_fade_channel_off( ( int )params[0], ( int )params[1] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_set_sound_volume
 *
 *  Set the volume for a sound
 *
 *  PARAMS:
 *      wav id
 *      volume (0-128)
 *
 *  RETURN VALUE:
 *
 *  0
 *
 */

static int64_t libmod_sound_set_sound_volume( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_set_sound_volume( params[0], ( int )params[1] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_set_sound_volume_all
 *
 *  Set the volume for all playing sound channels
 *
 *  PARAMS:
 *      volume (0-128)
 *
 *  RETURN VALUE:
 *
 *  -1
 *
 */

static int64_t libmod_sound_set_sound_volume_all( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_set_channel_volume( -1, ( int )params[0] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_set_channel_volume
 *
 *  Set the volume for a channel
 *
 *  PARAMS:
 *      channel number
 *      volume (0-128)
 *
 *  RETURN VALUE:
 *
 *  0
 *
 */

static int64_t libmod_sound_set_channel_volume( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_set_channel_volume( ( int )params[0], ( int )params[1] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_set_channel_volume_all
 *
 *  Set the volume for all playing sound channels
 *
 *  PARAMS:
 *      volume (0-128)
 *
 *  RETURN VALUE:
 *
 *  -1
 *
 */

static int64_t libmod_sound_set_channel_volume_all( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_set_channel_volume( -1, ( int )params[0] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_set_master_volume
 *
 *  Set the volume for all playing sound channels
 *
 *  PARAMS:
 *      volume (0-128)
 *
 *  RETURN VALUE:
 *
 *  0
 *
 */

static int64_t libmod_sound_set_master_volume( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_set_master_volume( ( int )params[0] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_reserve_channels
 *
 *  Reserves some channels so they are not used as free channels
 *
 *  PARAMS:
 *      number of channels to reserve
 *
 *  RETURN VALUE:
 *
 *  number of reserved channels
 *
 */

static int64_t libmod_sound_reserve_channels( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_reserve_channels( ( int )params[0] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_set_panning
 *
 *  Set the panning for a channel
 *
 *  PARAMS:
 *      channel number
 *      left volume (0-255)
 *      right volume (0-255)
 *
 *  RETURN VALUE:
 *
 *  0
 *
 */

static int64_t libmod_sound_set_panning( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_set_panning( ( int )params[0], ( int )params[1], ( int )params[2] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_set_position
 *
 *  Set the position for a channel
 *
 *  PARAMS:
 *      channel number
 *      angle (0-360)
 *      distance (0-255)
 *
 *  RETURN VALUE:
 *
 *  0
 *
 */

static int64_t libmod_sound_set_position( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_set_position( ( int )params[0], ( int )params[1], ( int )params[2] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_set_distance
 *
 *  Set the distance for a channel
 *
 *  PARAMS:
 *      channel number
 *      distance (0-255)
 *
 *  RETURN VALUE:
 *
 *  0
 *
 */

static int64_t libmod_sound_set_distance( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_set_distance( ( int)params[0], ( int )params[1] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_reverse_stereo
 *
 *  Reverse stereo for a channel
 *
 *  PARAMS:
 *      channel number
 *      flip (1 or 0)
 *
 *  RETURN VALUE:
 *
 *  0
 *
 */

static int64_t libmod_sound_reverse_stereo( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_reverse_stereo( ( int )params[0], ( int )params[1] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_expire_channel
 *
 *  Tells a channel to stop playing after a certain time
 *
 *  PARAMS:
 *      channel number
 *      ticks in milliseconds
 *
 *  RETURN VALUE:
 *
 *  0
 *
 */

static int64_t libmod_sound_expire_channel( INSTANCE * my, int64_t * params ) {
    return bg_sound_expire_channel( params[0], params[1] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_expire_channel_all
 *
 *  Tells all channels to stop playing after a certain time
 *
 *  PARAMS:
 *      ticks in milliseconds
 *
 *  RETURN VALUE:
 *
 *  -1
 *
 */

static int64_t libmod_sound_expire_channel_all( INSTANCE * my, int64_t * params ) {
    return bg_sound_expire_channel( -1, params[0] );
}

/* Decoders */

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_get_num_music_decoders
 *
 *  Get the number of available music decoders.
 *
 *  PARAMS:
 *
 *  my          an INSTANCE pointer
 *  params      pointer to an array of integers
 *
 *  RETURN VALUE:
 *
 *  the number of available music decoders
 *
 */

static int64_t libmod_sound_get_num_music_decoders( INSTANCE * my, int64_t * params ) {
    if ( !bg_sound_is_initialized() && libmod_sound_init( NULL, NULL ) == -1 ) return -1;
    return ( int64_t ) bg_sound_get_num_music_decoders();
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_get_music_decoder
 *
 *  Get the name of the music decoder at the specified index.
 *
 *  PARAMS:
 *
 *  my          an INSTANCE pointer
 *  params      pointer to an array of integers
 *
 *  RETURN VALUE:
 *
 *  the name of the music decoder at the specified index
 *
 */

static int64_t libmod_sound_get_music_decoder( INSTANCE * my, int64_t * params ) {
    if ( !bg_sound_is_initialized() && libmod_sound_init( NULL, NULL ) == -1 ) return 0;
    const char * name = bg_sound_get_music_decoder( ( int )params[0] );
    int64_t r = string_new( ( char * )name );
    string_use( r );
    return r;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_has_music_decoder
 *
 *  Check if a music decoder is available by its name.
 *
 *  PARAMS:
 *
 *  my          an INSTANCE pointer
 *  params      pointer to an array of integers
 *
 *  RETURN VALUE:
 *
 *  0 if the audio is not initialized or if the decoder is not available
 *  1 if the decoder is available
 *
 */

static int64_t libmod_sound_has_music_decoder( INSTANCE * my, int64_t * params ) {
    if ( !bg_sound_is_initialized() && libmod_sound_init( NULL, NULL ) == -1 ) return 0;
    const char * name = string_get( params[0] );
    int64_t r = 0;
    if ( name ) {
        r = ( int64_t ) bg_sound_has_music_decoder( name );
        string_discard( params[0] );
    }
    return r;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_get_music_type
 *
 *  Get the type of music.
 *
 *  PARAMS:
 *
 *  my          an INSTANCE pointer
 *  params      pointer to an array of integers
 *
 *  RETURN VALUE:
 *
 *  the type of the music
 *
 */

static int64_t libmod_sound_get_music_type( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_get_music_type( params[0] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_get_music_type_current
 *
 *  Get the type of music current playing.
 *
 *  PARAMS:
 *
 *  my          an INSTANCE pointer
 *  params      pointer to an array of integers
 *
 *  RETURN VALUE:
 *
 *  the type of the music
 *
 */

static int64_t libmod_sound_get_music_type_current( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) bg_sound_get_music_type( 0 );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_get_music_title
 *
 *  Get the title of the music.
 *
 *  PARAMS:
 *
 *  my          an INSTANCE pointer
 *  params      pointer to an array of integers
 *
 *  RETURN VALUE:
 *
 *  the title of the music
 *
 */

static int64_t libmod_sound_get_music_title( INSTANCE * my, int64_t * params ) {
    const char * name = bg_sound_get_music_title( params[0] );
    int64_t r = string_new( ( char * )name );
    string_use( r );
    return r;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_get_music_title_current
 *
 *  Get the title of the music current playing.
 *
 *  PARAMS:
 *
 *  my          an INSTANCE pointer
 *  params      pointer to an array of integers
 *
 *  RETURN VALUE:
 *
 *  the title of the music
 *
 */

static int64_t libmod_sound_get_music_title_current( INSTANCE * my, int64_t * params ) {
    const char * name = bg_sound_get_music_title( 0 );
    int64_t r = string_new( ( char * )name );
    string_use( r );
    return r;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_get_music_title_tag
 *
 *  Get the title tag of the music.
 *
 *  PARAMS:
 *
 *  my          an INSTANCE pointer
 *  params      pointer to an array of integers
 *
 *  RETURN VALUE:
 *
 *  the title tag of the music
 *
 */

static int64_t libmod_sound_get_music_title_tag( INSTANCE * my, int64_t * params ) {
    const char * name = bg_sound_get_music_title_tag( params[0] );
    int64_t r = string_new( ( char * )name );
    string_use( r );
    return r;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_get_music_title_tag_current
 *
 *  Get the title tag of the music current playing.
 *
 *  PARAMS:
 *
 *  my          an INSTANCE pointer
 *  params      pointer to an array of integers
 *
 *  RETURN VALUE:
 *
 *  the title tag of the music
 *
 */

static int64_t libmod_sound_get_music_title_tag_current( INSTANCE * my, int64_t * params ) {
    const char * name = bg_sound_get_music_title_tag( 0 );
    int64_t r = string_new( ( char * )name );
    string_use( r );
    return r;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_get_music_artist_tag
 *
 *  Get the artist tag of the music.
 *
 *  PARAMS:
 *
 *  my          an INSTANCE pointer
 *  params      pointer to an array of integers
 *
 *  RETURN VALUE:
 *
 *  the artist tag of the music
 *
 */

static int64_t libmod_sound_get_music_artist_tag( INSTANCE * my, int64_t * params ) {
    const char * name = bg_sound_get_music_artist_tag( params[0] );
    int64_t r = string_new( ( char * )name );
    string_use( r );
    return r;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_get_music_artist_tag_current
 *
 *  Get the artist tag of the music current playing.
 *
 *  PARAMS:
 *
 *  my          an INSTANCE pointer
 *  params      pointer to an array of integers
 *
 *  RETURN VALUE:
 *
 *  the artist tag of the music
 *
 */

static int64_t libmod_sound_get_music_artist_tag_current( INSTANCE * my, int64_t * params ) {
    const char * name = bg_sound_get_music_artist_tag( 0 );
    int64_t r = string_new( ( char * )name );
    string_use( r );
    return r;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_get_music_album_tag
 *
 *  Get the album tag of the music.
 *
 *  PARAMS:
 *
 *  my          an INSTANCE pointer
 *  params      pointer to an array of integers
 *
 *  RETURN VALUE:
 *
 *  the album tag of the music
 *
 */

static int64_t libmod_sound_get_music_album_tag( INSTANCE * my, int64_t * params ) {
    const char * name = bg_sound_get_music_album_tag( params[0] );
    int64_t r = string_new( ( char * )name );
    string_use( r );
    return r;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_get_music_album_tag_current
 *
 *  Get the album tag of the music current playing.
 *
 *  PARAMS:
 *
 *  my          an INSTANCE pointer
 *  params      pointer to an array of integers
 *
 *  RETURN VALUE:
 *
 *  the album tag of the music
 *
 */

static int64_t libmod_sound_get_music_album_tag_current( INSTANCE * my, int64_t * params ) {
    const char * name = bg_sound_get_music_album_tag( 0 );
    int64_t r = string_new( ( char * )name );
    string_use( r );
    return r;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_get_music_copyright_tag
 *
 *  Get the copyright tag of the music.
 *
 *  PARAMS:
 *
 *  my          an INSTANCE pointer
 *  params      pointer to an array of integers
 *
 *  RETURN VALUE:
 *
 *  the copyright tag of the music
 *
 */

static int64_t libmod_sound_get_music_copyright_tag( INSTANCE * my, int64_t * params ) {
    const char * name = bg_sound_get_music_copyright_tag( params[0] );
    int64_t r = string_new( ( char * )name );
    string_use( r );
    return r;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_get_music_copyright_tag_current
 *
 *  Get the copyright tag of the music current playing.
 *
 *  PARAMS:
 *
 *  my          an INSTANCE pointer
 *  params      pointer to an array of integers
 *
 *  RETURN VALUE:
 *
 *  the copyright tag of the music
 *
 */

static int64_t libmod_sound_get_music_copyright_tag_current( INSTANCE * my, int64_t * params ) {
    const char * name = bg_sound_get_music_copyright_tag( 0 );
    int64_t r = string_new( ( char * )name );
    string_use( r );
    return r;
}

/* Groups */

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_group_channel
 *
 *  Assign a tag to a channel.
 *
 *  A tag is an arbitrary number that can be assigned to several mixer channels,
 *  to form groups of channels.
 *
 *  If tag is -1, the tag is removed.
 *
 *  PARAMS:
 *
 *  my          an INSTANCE pointer
 *  params      pointer to an array of integers containing channel and tag
 *
 *  RETURN VALUE:
 *
 *  1 if successful, 0 on failure
 *
 */

static int64_t libmod_sound_group_channel( INSTANCE * my, int64_t * params ) {
    return bg_sound_group_channel( params[0], ( int )params[1] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_group_channels
 *
 *  Assign a tag to a range of channels.
 *
 *  A tag is an arbitrary number that can be assigned to several mixer channels,
 *  to form groups of channels.
 *
 *  If tag is -1, the tag is removed.
 *
 *  Existing tags in the range will be replaced. Any mixer channel can only
 *  have one tag per channel.
 *
 *  You may not specify MAX_CHANNEL_POST for a channel.
 *
 *  Note that this returns success and failure in the _opposite_ way from
 *  Mix_GroupChannel(). We regret the API design mistake.
 *
 *  PARAMS:
 *
 *  my          an INSTANCE pointer
 *  params      pointer to an array of integers containing from, to, and tag
 *
 *  RETURN VALUE:
 *
 *  0 if successful, negative on error
 *
 */

static int64_t libmod_sound_group_channels( INSTANCE * my, int64_t * params ) {
    return bg_sound_group_channels( params[0], params[1], ( int )params[2] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_group_available
 *
 *  Finds the first available channel in a group of channels.
 *
 *  A tag is an arbitrary number that can be assigned to several mixer channels,
 *  to form groups of channels.
 *
 *  This function searches all channels with a specified tag, and returns the
 *  channel number of the first one it finds that is currently unused.
 *
 *  If no channels with the specified tag are unused, this function returns -1.
 *
 *  PARAMS:
 *
 *  my          an INSTANCE pointer
 *  params      pointer to an integer containing the tag
 *
 *  RETURN VALUE:
 *
 *  first available channel, or -1 if none are available.
 *
 */

static int64_t libmod_sound_group_available( INSTANCE * my, int64_t * params ) {
    return bg_sound_group_available( ( int )params[0] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_group_count
 *
 *  Returns the number of channels in a group.
 *
 *  If tag is -1, this will return the total number of channels allocated,
 *  regardless of what their tag might be.
 *
 *  PARAMS:
 *
 *  my          an INSTANCE pointer
 *  params      pointer to an integer containing the tag
 *
 *  RETURN VALUE:
 *
 *  the number of channels assigned the specified tag.
 *
 */

static int64_t libmod_sound_group_count( INSTANCE * my, int64_t * params ) {
    return bg_sound_group_count( ( int )params[0] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_group_oldest
 *
 *  Find the "oldest" sample playing in a group of channels.
 *
 *  Specifically, this function returns the channel number that is assigned the
 *  specified tag, is currently playing, and has the lowest start time, based
 *  on the value of SDL_GetTicks() when the channel started playing.
 *
 *  If no channel with this tag is currently playing, this function returns -1.
 *
 *  PARAMS:
 *
 *  my          an INSTANCE pointer
 *  params      pointer to an integer containing the tag
 *
 *  RETURN VALUE:
 *
 *  the "oldest" sample playing in a group of channels
 *
 */

static int64_t libmod_sound_group_oldest( INSTANCE * my, int64_t * params ) {
    return bg_sound_group_oldest( ( int )params[0] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_group_newer
 *
 *  Find the "most recent" sample playing in a group of channels.
 *
 *  Specifically, this function returns the channel number that is assigned the
 *  specified tag, is currently playing, and has the highest start time, based
 *  on the value of SDL_GetTicks() when the channel started playing.
 *
 *  If no channel with this tag is currently playing, this function returns -1.
 *
 *  PARAMS:
 *
 *  my          an INSTANCE pointer
 *  params      pointer to an integer containing the tag
 *
 *  RETURN VALUE:
 *
 *  the "most recent" sample playing in a group of channels
 *
 */

static int64_t libmod_sound_group_newer( INSTANCE * my, int64_t * params ) {
    return bg_sound_group_newer( ( int )params[0] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_halt_group
 *
 *  Halt playing of a group of channels by arbitrary tag.
 *
 *  This will stop further playback on all channels with a specific tag, until
 *  a new chunk is started there.
 *
 *  A tag is an arbitrary number that can be assigned to several mixer channels,
 *  to form groups of channels.
 *
 *  The default tag for a channel is -1.
 *
 *  Any halted channels will have any currently-registered effects
 *  deregistered, and will call any callback specified by Mix_ChannelFinished()
 *  before this function returns.
 *
 *  PARAMS:
 *
 *  my      an INSTANCE pointer
 *  params  pointer to an array of integers containing the tag
 *
 *  RETURN VALUE:
 *
 *  zero, whether any channels were halted or not.
 *
 */

static int64_t libmod_sound_halt_group( INSTANCE * my, int64_t * params ) {
    return bg_sound_group_stop( ( int )params[0] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_fade_out_group
 *
 *  Halt a playing group of channels by arbitrary tag, after fading them out
 *  for a specified time.
 *
 *  This will begin fading a group of channels with a specific tag from their
 *  current volumes to silence over `ms` milliseconds. After that time, those
 *  channels are halted.
 *
 *  A tag is an arbitrary number that can be assigned to several mixer channels,
 *  to form groups of channels.
 *
 *  The default tag for a channel is -1.
 *
 *  Any halted channels will have any currently-registered effects
 *  deregistered, and will call any callback specified by Mix_ChannelFinished()
 *  once the halt occurs.
 *
 *  A fading channel will change its volume progressively, as if Mix_Volume()
 *  was called on it ( which is to say: you probably shouldn't call Mix_Volume()
 *  on a fading channel ).
 *
 *  Note that this function does not block for the number of milliseconds
 *  requested; it just schedules the group to fade and notes the time for the
 *  mixer to manage later, and returns immediately.
 *
 *  PARAMS:
 *
 *  my      an INSTANCE pointer
 *  params  pointer to an array of integers containing the tag and ms
 *
 *  RETURN VALUE:
 *
 *  the number of channels that were scheduled for fading.
 *
 */

static int64_t libmod_sound_fade_out_group( INSTANCE * my, int64_t * params ) {
    return bg_sound_group_fade_out( ( int )params[0], ( int )params[1] );
}

/* --------------------------------------------------------------------------- */
/* Module/Plugin Initialization Functions                                      */

void __bgdexport( libmod_sound, module_initialize )() {
    bg_sound_system_init();
}

/* --------------------------------------------------------------------------- */

void __bgdexport( libmod_sound, module_finalize )() {
    bg_sound_system_finalize();
}

/* --------------------------------------------------------------------------- */

#include "libmod_sound_exports.h"
