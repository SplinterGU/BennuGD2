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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bgddl.h"

#include "libmod_sound.h"

#include "files.h"
#include "xstrings.h"

#include "dlvaracc.h"

#include "bgload.h"

/* --------------------------------------------------------------------------- */

static int audio_initialized = 0 ;

/* --------------------------------------------------------------------------- */

#define SOUND_FREQ              0
#define SOUND_MODE              1
#define SOUND_CHANNELS          2

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
/* SDL_RWops Bennu Interface                                                   */
/* --------------------------------------------------------------------------- */

static Sint64 SDLCALL __libmod_sound_size_cb( struct SDL_RWops *context ) {
    return( file_size( context->hidden.unknown.data1 ) );
}

/* --------------------------------------------------------------------------- */

static Sint64 SDLCALL __libmod_sound_seek_cb( struct SDL_RWops *context, Sint64 offset, int whence ) {
    if ( file_seek( context->hidden.unknown.data1, offset, whence ) < 0 ) return ( -1 );
    return( file_pos( context->hidden.unknown.data1 ) );
//    return ( file_seek( context->hidden.unknown.data1, offset, whence ) );
}

/* --------------------------------------------------------------------------- */

static size_t SDLCALL __libmod_sound_read_cb( struct SDL_RWops *context, void *ptr, size_t size, size_t maxnum ) {
    int ret = file_read( context->hidden.unknown.data1, ptr, size * maxnum );
    if ( ret > 0 ) ret /= size;
    return( ret );
}

/* --------------------------------------------------------------------------- */

static size_t SDLCALL __libmod_sound_write_cb( struct SDL_RWops *context, const void *ptr, size_t size, size_t num ) {
    int ret = file_write( context->hidden.unknown.data1, ( void * )ptr, size * num );
    if ( ret > 0 ) ret /= size;
    return( ret );
}

/* --------------------------------------------------------------------------- */

static int SDLCALL __libmod_sound_close_cb( struct SDL_RWops *context ) {
    if ( context ) {
        file_close( context->hidden.unknown.data1 );
        SDL_FreeRW( context );
    }
    return( 0 );
}

/* --------------------------------------------------------------------------- */

static SDL_RWops *SDL_RWFromBGDFP( file *fp ) {
    SDL_RWops *rwops = SDL_AllocRW();
    if ( rwops ) {
        rwops->size = __libmod_sound_size_cb;
        rwops->seek = __libmod_sound_seek_cb;
        rwops->read = __libmod_sound_read_cb;
        rwops->write = __libmod_sound_write_cb;
        rwops->close = __libmod_sound_close_cb;
        rwops->hidden.unknown.data1 = fp;
    }
    return( rwops );
}

/* --------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : sound_quit
 *
 *  Close all the audio set
 *
 *  PARAMS:
 *      no params
 *
 *  RETURN VALUE:
 *
 *  no return
 *
 */

static void sound_quit() {
    if ( !audio_initialized ) return;

    // Still need to verify that everything is unloaded

    Mix_CloseAudio();

    audio_initialized = 0;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : sound_init
 *
 *  Set the SDL_Mixer library
 *
 *  PARAMS:
 *      no params
 *
 *  RETURN VALUE:
 *
 *  no return
 *
 */

static int sound_init() {
    sound_quit();
    
    /* Initialize variables: but limit quality to some fixed options */
    
    int audio_rate = GLOQWORD( libmod_sound, SOUND_FREQ );

         if ( audio_rate > 44100 )  audio_rate = 48000;
    else if ( audio_rate > 22050 )  audio_rate = 44100;
    else if ( audio_rate > 11025 )  audio_rate = 22050;
    else                            audio_rate = 11025;

    Uint16 audio_format = MIX_DEFAULT_FORMAT;
    int audio_channels = GLOQWORD( libmod_sound, SOUND_MODE );

    switch ( audio_channels ) {
        case SOUND_MODE_MONO:           // Mono
        case SOUND_MODE_STEREO:         // Stereo
        case SOUND_MODE_STEREO_2_1:     // Stereo 2.1
        case SOUND_MODE_SURROUND_4_1:   // Surround 4.0 (quadraphonic)
        case SOUND_MODE_SURROUND_5_1:   // Surround 5.1
        case SOUND_MODE_SURROUND_7_1:   // Surround 7.1
            break;

        default:
            audio_channels = SOUND_MODE_STEREO;
            break;
    }

    /* Open the audio device */
    if ( !Mix_OpenAudio( audio_rate, audio_format, audio_channels, 4096 ) ) {
        GLOQWORD( libmod_sound, SOUND_CHANNELS ) <= 32 ? Mix_AllocateChannels( GLOQWORD( libmod_sound, SOUND_CHANNELS ) ) : Mix_AllocateChannels( 32 ) ;
        GLOQWORD( libmod_sound, SOUND_CHANNELS ) = Mix_AllocateChannels( -1 ) ; // Get current audio mix channels
#if 0
        Mix_QuerySpec( &audio_rate, &audio_format, &audio_channels );
        printf("Opened audio at %d Hz %d bit%s %s (%d channel)\n", audio_rate,
            (audio_format&0xFF),
            (SDL_AUDIO_ISFLOAT(audio_format) ? " (float)" : ""),
            (audio_channels > 3) ? "surround" :
            (audio_channels > 1) ? "stereo" : "mono",
            audio_channels);
#endif
        audio_initialized = 1;
        return 0;
    }

#if 0
    fprintf( stderr, "[SOUND] Could not initialize audio: %s\n", SDL_GetError() ) ;
#endif

    audio_initialized = 0;
    return -1 ;
}

/* --------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/* Sonido MOD y OGG                                                            */
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : load_music
 *
 *  Load a MOD/OGG from a file
 *
 *  PARAMS:
 *      file name
 *
 *  RETURN VALUE:
 *
 *  mod pointer
 *
 */

static int64_t load_music( const char * filename ) {
    Mix_Music *music = NULL;
    file      *fp;

    if ( !audio_initialized && sound_init() ) return ( 0 );

    if ( !( fp = file_open( filename, "rb0" ) ) ) return ( 0 );

    SDL_RWops * rwops = SDL_RWFromBGDFP( fp );
    if ( !rwops ) {
        file_close( fp );
        return( 0 );
    }

    // Don't need free rwops, SDL will do
    if ( !( music = Mix_LoadMUS_RW( rwops, 1 ) ) ) return ( 0 );

    return (( int64_t )( intptr_t )music );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : play_music
 *
 *  Play a MOD/OGG
 *
 *  PARAMS:
 *      mod pointer
 *      number of loops (-1 infinite loops)
 *
 *  RETURN VALUE:
 *
 * -1 if there is any error
 *
 */

static int play_music( int64_t id, int loops ) {
    if ( audio_initialized && id ) {
        int result = Mix_PlayMusic(( Mix_Music * )( intptr_t )id, loops );
        // if ( result == -1 ) fprintf( stderr, "%s\n", Mix_GetError() );
        return result;
    }
    // fprintf( stderr, "Play music called with invalid handle\n" );
    return( -1 );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : fade_music_in
 *
 *  Play a MOD/OGG fading in it
 *
 *  PARAMS:
 *      mod pointer
 *      number of loops (-1 infinite loops)
 *      ms  microsends of fading
 *
 *  RETURN VALUE:
 *
 * -1 if there is any error
 *
 */

static int fade_music_in( int64_t id, int loops, int ms ) {
    if ( audio_initialized && id ) return( Mix_FadeInMusic(( Mix_Music * )( intptr_t )id, loops, ms ) );
    return( -1 );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : fade_music_in_pos
 *
 *  Play a MOD/OGG fading in it
 *
 *  PARAMS:
 *      mod pointer
 *      number of loops (-1 infinite loops)
 *      ms  microsends of fading
 *      position position in seconds
 *
 *  RETURN VALUE:
 *
 * -1 if there is any error
 *
 */

static int fade_music_in_pos( int64_t id, int loops, int ms, double position ) {
    if ( audio_initialized && id ) return( Mix_FadeInMusicPos(( Mix_Music * )( intptr_t )id, loops, ms, position ) );
    return( -1 );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : fade_music_off
 *
 *  Stop the play of a mod
 *
 *  PARAMS:
 *
 *  ms  microsends of fading
 *
 *  RETURN VALUE:
 *
 * -1 if there is any error
 *
 */

static int fade_music_off( int ms ) {
    if ( !audio_initialized ) return ( 0 );
    return ( Mix_FadeOutMusic( ms ) );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : is_fading_music
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

static int is_fading_music() {
    if ( audio_initialized ) return Mix_FadingMusic();
    return ( -1 );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : unload_music
 *
 *  Play a MOD
 *
 *  PARAMS:
 *
 *  mod id
 *
 *  RETURN VALUE:
 *
 * -1 if there is any error
 *
 */

static int unload_music( int64_t id ) {
    if ( audio_initialized && id ) {
        if ( Mix_PlayingMusic() ) Mix_HaltMusic();
        Mix_FreeMusic(( Mix_Music * )( intptr_t )id );
    }
    return ( 0 ) ;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : stop_music
 *
 *  Stop the play of a mod
 *
 *  PARAMS:
 *
 *  no params
 *
 *  RETURN VALUE:
 *
 * -1 if there is any error
 *
 */

static int stop_music( void ) {
    if ( audio_initialized ) Mix_HaltMusic();
    return ( 0 ) ;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : pause_music
 *
 *  Pause the mod in curse, you can resume it after
 *
 *  PARAMS:
 *
 *  no params
 *
 *  RETURN VALUE:
 *
 * -1 if there is any error
 *
 */

static int pause_music( void ) {
    if ( audio_initialized ) Mix_PauseMusic();
    return ( 0 ) ;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : is_paused_music
 *
 *  Query is music is paused
 *
 *  PARAMS:
 *
 *  no params
 *
 *  RETURN VALUE:
 *
 * -1 if there is any error
 *
 */

static int is_paused_music( void ) {
    if ( audio_initialized ) return Mix_PausedMusic();
    return ( -1 ) ;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : resume_music
 *
 *  Resume the mod, paused before
 *
 *  PARAMS:
 *
 *  no params
 *
 *  RETURN VALUE:
 *
 * -1 if there is any error
 *
 */

static int resume_music( void ) {
    if ( audio_initialized ) Mix_ResumeMusic();
    return( 0 ) ;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : is_playing_music
 *
 *  Check if there is any mod playing
 *
 *  PARAMS:
 *
 *  no params
 *
 *  RETURN VALUE:
 *
 * -1 if there is any error
 *  TRUE OR FALSE if there is no error
 *
 */

static int is_playing_music( void ) {
    if ( !audio_initialized ) return ( 0 );
    return Mix_PlayingMusic();
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : set_music_volume
 *
 *  Set the volume for mod playing (0-128)
 *
 *  PARAMS:
 *
 *  int volume
 *
 *  RETURN VALUE:
 *
 * -1 if there is any error
 *  0 if there is no error
 *
 */

static int set_music_volume( int volume ) {
    if ( !audio_initialized && sound_init() ) return ( -1 );

    if ( volume < 0 ) volume = 0;
    if ( volume > 128 ) volume = 128;

    Mix_VolumeMusic( volume );
    return 0;
}


/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : get_music_volume
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

static int get_music_volume( int64_t id ) {
    if ( !audio_initialized || !id ) return ( 0 );
#if !(SDL_MIXER_VERSION_ATLEAST(2, 6, 0))
    return -1;
#else
    return Mix_GetMusicVolume(( Mix_Music * )( intptr_t )id );
#endif
}


/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : music_rewind
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

static int music_rewind() {
    if ( audio_initialized ) Mix_RewindMusic();
    return 0;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : set_music_playback_position
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

static int set_music_playback_position( double position ) {
    if ( !audio_initialized ) return ( 0 );
    return ( Mix_SetMusicPosition( position ) );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : get_music_playback_position
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

static double get_music_playback_position( int64_t id ) {
    if ( !audio_initialized || !id ) return ( 0 );
#if !(SDL_MIXER_VERSION_ATLEAST(2, 6, 0))
    return -1;
#else
    return ( Mix_GetMusicPosition(( Mix_Music * )( intptr_t )id ) );
#endif
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : get_music_duration
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
static double get_music_duration( int64_t id ) {
    if ( !audio_initialized ) return 0.0;
#if !(SDL_MIXER_VERSION_ATLEAST(2, 6, 0))
    return -1;
#else
    return Mix_MusicDuration(( Mix_Music * )( intptr_t )id );
#endif
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : get_music_loop_start_time
 *
 *  Get the start time of the loop in the music stream, in seconds.
 *
 *  PARAMS:
 *
 *  music       pointer to Mix_Music structure
 *              if 0, returns duration of current playing music.
 *
 *  RETURN VALUE:
 *
 *  -1.0 if there is any error or if loop start time is not available
 *  other value loop start time in seconds
 *
 */

static double get_music_loop_start_time( int64_t id ) {
    if ( !audio_initialized ) return 0.0;
#if !(SDL_MIXER_VERSION_ATLEAST(2, 6, 0))
    return -1;
#else
    return Mix_GetMusicLoopStartTime(( Mix_Music * )( intptr_t )id );
#endif
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : get_music_loop_end_time
 *
 *  Get the end time of the loop in the music stream, in seconds.
 *
 *  PARAMS:
 *
 *  music       pointer to Mix_Music structure
 *              if 0, returns duration of current playing music.
 *
 *  RETURN VALUE:
 *
 *  -1.0 if there is any error or if loop end time is not available
 *  other value loop end time in seconds
 *
 */

static double get_music_loop_end_time( int64_t id ) {
    if ( !audio_initialized ) return 0.0;
#if !(SDL_MIXER_VERSION_ATLEAST(2, 6, 0))
    return -1;
#else
    return Mix_GetMusicLoopEndTime(( Mix_Music * )( intptr_t )id );
#endif
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : get_music_loop_length_time
 *
 *  Get the length of the loop in the music stream, in seconds.
 *
 *  PARAMS:
 *
 *  music       pointer to Mix_Music structure
 *              if 0, returns duration of current playing music.
 *
 *  RETURN VALUE:
 *
 *  -1.0 if there is any error or if loop length time is not available
 *  other value loop length time in seconds
 *
 */

static double get_music_loop_length_time( int64_t id ) {
    if ( !audio_initialized ) return 0.0;
#if !(SDL_MIXER_VERSION_ATLEAST(2, 6, 0))
    return -1;
#else
    return Mix_GetMusicLoopLengthTime(( Mix_Music * )( intptr_t )id );
#endif
}


/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : get_num_music_decoders
 *
 *  Get the number of available music decoders.
 *
 *  RETURN VALUE:
 *
 *  -1 if there is any error or if the audio is not initialized
 *  other value     the number of available music decoders
 *
 */
static int64_t get_num_music_decoders(  ) {
    if ( !audio_initialized && sound_init() ) return -1;
    return Mix_GetNumMusicDecoders(  );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : get_music_decoder
 *
 *  Get the name of the music decoder at the specified index.
 *
 *  PARAMS:
 *
 *  index       index of the music decoder to query
 *
 *  RETURN VALUE:
 *
 *  NULL if there is any error or if the audio is not initialized
 *  other value     the name of the music decoder at the specified index
 *
 */
static const char *get_music_decoder( int index ) {
    if ( !audio_initialized ) return NULL;
    return Mix_GetMusicDecoder( index );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : has_music_decoder
 *
 *  Check if a music decoder is available by its name.
 *
 *  PARAMS:
 *
 *  name        the decoder name to query
 *
 *  RETURN VALUE:
 *
 *  0 if the audio is not initialized or if the decoder is not available
 *  1 if the decoder is available
 *
 */
static int has_music_decoder( const char *name ) {
    if ( !audio_initialized ) return 0;
#if !(SDL_MIXER_VERSION_ATLEAST(2, 6, 0))
    return 0;
#else
    return ( int )Mix_HasMusicDecoder( name );
#endif
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : get_music_type
 *
 *  Get the type of music.
 *
 *  PARAMS:
 *
 *  id          ID of the music
 *
 *  RETURN VALUE:
 *
 *  -1 if there is any error or if the audio is not initialized
 *  other value     the type of the music
 *
 */
static int get_music_type( int64_t id ) {
    if ( !audio_initialized ) return -1;
    return ( int )Mix_GetMusicType(( Mix_Music * )( intptr_t )id );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : get_music_title
 *
 *  Get the title of the music.
 *
 *  PARAMS:
 *
 *  id          ID of the music
 *
 *  RETURN VALUE:
 *
 *  NULL if there is any error or if the audio is not initialized
 *  other value     the title of the music
 *
 */
static const char *get_music_title( int64_t id ) {
    if ( !audio_initialized ) return NULL;
#if !(SDL_MIXER_VERSION_ATLEAST(2, 6, 0))
    return "";
#else
    return Mix_GetMusicTitle(( Mix_Music * )( intptr_t )id );
#endif
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : get_music_title_tag
 *
 *  Get the title tag of the music.
 *
 *  PARAMS:
 *
 *  id          ID of the music
 *
 *  RETURN VALUE:
 *
 *  the title tag of the music
 *
 */
static const char *get_music_title_tag( int64_t id ) {
#if !(SDL_MIXER_VERSION_ATLEAST(2, 6, 0))
    return "";
#else
    return Mix_GetMusicTitleTag(( Mix_Music * )( intptr_t )id );
#endif
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : get_music_artist_tag
 *
 *  Get the artist tag of the music.
 *
 *  PARAMS:
 *
 *  id          ID of the music
 *
 *  RETURN VALUE:
 *
 *  the artist tag of the music
 *
 */
static const char *get_music_artist_tag( int64_t id ) {
#if !(SDL_MIXER_VERSION_ATLEAST(2, 6, 0))
    return "";
#else
    return Mix_GetMusicArtistTag(( Mix_Music * )( intptr_t )id );
#endif
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : get_music_album_tag
 *
 *  Get the album tag of the music.
 *
 *  PARAMS:
 *
 *  id          ID of the music
 *
 *  RETURN VALUE:
 *
 *  the album tag of the music
 *
 */
static const char *get_music_album_tag( int64_t id ) {
#if !(SDL_MIXER_VERSION_ATLEAST(2, 6, 0))
    return "";
#else
    return Mix_GetMusicAlbumTag(( Mix_Music * )( intptr_t )id );
#endif
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : get_music_copyright_tag
 *
 *  Get the copyright tag of the music.
 *
 *  PARAMS:
 *
 *  id          ID of the music
 *
 *  RETURN VALUE:
 *
 *  the copyright tag of the music
 *
 */
static const char *get_music_copyright_tag( int64_t id ) {
#if !(SDL_MIXER_VERSION_ATLEAST(2, 6, 0))
    return "";
#else
    return Mix_GetMusicCopyrightTag(( Mix_Music * )( intptr_t )id );
#endif
}

/* --------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/* WAV                                                                         */
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : load_sound
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

static int64_t load_sound( const char * filename ) {
    Mix_Chunk *sound = NULL;
    file      *fp;

    if ( !audio_initialized && sound_init() ) return ( 0 );

    if ( !( fp = file_open( filename, "rb0" ) ) ) return ( 0 );

    SDL_RWops * rwops = SDL_RWFromBGDFP( fp );
    if ( !rwops ) {
        file_close( fp );
        return( 0 );
    }

    // Don't need free rwops, SDL will do
    if ( !( sound = Mix_LoadWAV_RW( rwops, 1 ) ) ) return( 0 );

    return (( int64_t ) ( intptr_t ) sound );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : play_sound
 *
 *  Play a WAV
 *
 *  PARAMS:
 *      wav pointer;
 *      number of loops (-1 infinite loops)
 *      channel (-1 any channel)
 *
 *  RETURN VALUE:
 *
 * -1 if there is any error
 *  else channel where the music plays
 *
 */

static int play_sound( int64_t id, int loops, int channel ) {
    if ( audio_initialized && id ) return ( ( int ) Mix_PlayChannel( channel, ( Mix_Chunk * )( intptr_t )id, loops ) );
    return ( -1 );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : play_sound_timed
 *
 *  Play a WAV with timeout
 *
 *  PARAMS:
 *      wav pointer;
 *      number of loops (-1 infinite loops)
 *      channel (-1 any channel)
 *      ticks timeout in milliseconds ( -1 no timeout )
 *
 *  RETURN VALUE:
 *
 * -1 if there is any error
 *  else channel where the music plays
 *
 */

static int play_sound_timed( int64_t id, int loops, int channel, int ticks ) {
    if ( audio_initialized && id ) return ( ( int ) Mix_PlayChannelTimed( channel, ( Mix_Chunk * )( intptr_t )id, loops, ticks ) );
    return ( -1 );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : fade_sound_in
 *
 *  Play a WAV fading in it
 *
 *  PARAMS:
 *      wav pointer;
 *      number of loops (-1 infinite loops)
 *      ms  microsends of fading
 *      channel (-1 any channel)
 *
 *  RETURN VALUE:
 *
 * -1 if there is any error
 *
 */

static int fade_sound_in( int64_t id, int loops, int ms, int channel ) {
    if ( audio_initialized && id ) return( Mix_FadeInChannel( channel, ( Mix_Chunk * )( intptr_t )id, loops, ms ) );
    return( -1 );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : fade_sound_in_timed
 *
 *  Play a WAV fading in it with timeout
 *
 *  PARAMS:
 *      wav pointer;
 *      number of loops (-1 infinite loops)
 *      ms  microsends of fading
 *      channel (-1 any channel)
 *      ticks timeout in milliseconds ( -1 no timeout )
 *
 *  RETURN VALUE:
 *
 * -1 if there is any error
 *
 */

static int fade_sound_in_timed( int64_t id, int loops, int ms, int channel, int ticks ) {
    if ( audio_initialized && id ) return( Mix_FadeInChannelTimed( channel, ( Mix_Chunk * )( intptr_t )id, loops, ms, ticks ) );
    return( -1 );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : is_fading_channel
 *
 *  Query if a channel is fading
 *
 *  PARAMS:
 *      channel id
 *
 *  RETURN VALUE:
 *
 * -1 if there is any error
 *
 */

static int is_fading_channel( int channel ) {
    if ( audio_initialized ) return Mix_FadingChannel( channel );
    return( -1 );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : unload_sound
 *
 *  Frees the resources from a wav, unloading it
 *
 *  PARAMS:
 *
 *  wav pointer
 *
 *  RETURN VALUE:
 *
 * -1 if there is any error
 *
 */

static int unload_sound( int64_t id ) {
    if ( audio_initialized && id ) Mix_FreeChunk(( Mix_Chunk * )( intptr_t )id );
    return ( 0 );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : stop_sound
 *
 *  Stop a wav playing
 *
 *  PARAMS:
 *
 *  int channel
 *
 *  RETURN VALUE:
 *
 * -1 if there is any error
 *
 */

static int stop_sound( int channel ) {
    if ( audio_initialized && Mix_Playing( channel ) ) return( Mix_HaltChannel( channel ) );
    return ( -1 ) ;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : pause_sound
 *
 *  Pause a wav playing, you can resume it after
 *
 *  PARAMS:
 *
 *  int channel
 *
 *  RETURN VALUE:
 *
 * -1 if there is any error
 *
 */

static int pause_sound( int channel ) {
    if ( audio_initialized && Mix_Playing( channel ) ) {
        Mix_Pause( channel );
        return ( 0 ) ;
    }
    return ( -1 ) ;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : is_paused_sound
 *
 *  Query if channel is paused
 *
 *  PARAMS:
 *
 *  int channel
 *
 *  RETURN VALUE:
 *
 * -1 if there is any error
 *
 */

static int is_paused_sound( int channel ) {
    if ( audio_initialized ) return Mix_Paused( channel );
    return ( -1 ) ;
}


/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : resume_sound
 *
 *  Resume a wav playing, paused before
 *
 *  PARAMS:
 *
 *  int channel
 *
 *  RETURN VALUE:
 *
 * -1 if there is any error
 *
 */

static int resume_sound( int channel ) {
    if ( audio_initialized && Mix_Playing( channel ) ) {
        Mix_Resume( channel );
        return ( 0 ) ;
    }
    return ( -1 ) ;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : is_playing_sound
 *
 *  Check a wav playing
 *
 *  PARAMS:
 *
 *  int channel
 *
 *  RETURN VALUE:
 *
 * -1 if there is any error
 *  TRUE OR FALSE if there is no error
 *
 */

static int is_playing_sound( int channel ) {
    if ( audio_initialized ) return( Mix_Playing( channel ) );
    return ( 0 );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : fade_channel_off
 *
 *  Fade out channel
 *
 *  PARAMS:
 *      channel id
 *      ms  microsends of fading
 *
 *  RETURN VALUE:
 *
 * -1 if there is any error
 *
 */

static int fade_channel_off( int channel, int ms ) {
    if ( audio_initialized ) return( Mix_FadeOutChannel( channel, ms ) );
    return( -1 );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : set_sound_volume
 *
 *  Set the volume for wav playing (0-128) IN SAMPLE
 *
 *  PARAMS:
 *
 *  channel id
 *  int volume
 *
 *  RETURN VALUE:
 *
 * -1 if there is any error
 *
 */

static int set_sound_volume( int64_t sample, int volume ) {
    if ( !audio_initialized ) return ( -1 );

    if ( volume < 0 ) volume = 0;
    if ( volume > 128 ) volume = 128;

    if ( sample ) return( Mix_VolumeChunk(( Mix_Chunk * )( intptr_t )sample, volume ) );

    return -1 ;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : set_channel_volume
 *
 *  Set the volume for a channel (0-128)
 *
 *  PARAMS:
 *
 *  channel id
 *  int volume
 *
 *  RETURN VALUE:
 *
 * -1 if there is any error
 *
 */

static int set_channel_volume( int channel, int volume ) {
    if ( !audio_initialized && sound_init() ) return ( -1 );

    if ( volume < 0 ) volume = 0;
    if ( volume > 128 ) volume = 128;

    return( Mix_Volume( channel, volume ) );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : reserve_channels
 *
 *  Reserve the first channels (0 -> n-1) for the application, i.e. don't allocate
 *  them dynamically to the next sample if requested with a -1 value below.
 *
 *  PARAMS:
 *  number of channels to reserve.
 *
 *  RETURN VALUE:
 *  number of reserved channels.
 * -1 if there is any error
 *
 */

static int reserve_channels( int channeles ) {
    if ( !audio_initialized && sound_init() ) return ( -1 );
    return Mix_ReserveChannels( channeles );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : set_panning
 *
 *  Set the panning for a wav channel
 *
 *  PARAMS:
 *
 *  channel
 *  left volume (0-255)
 *  right volume (0-255)
 *
 */

static int set_panning( int channel, int left, int right ) {
    if ( !audio_initialized && sound_init() ) return ( -1 );

    if ( Mix_Playing( channel ) ) {
        Mix_SetPanning( channel, ( Uint8 )left, ( Uint8 )right );
        return ( 0 ) ;
    }
    return ( -1 ) ;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : set_position
 *
 *  Set the position of a channel. (angle) is an integer from 0 to 360
 *
 *  PARAMS:
 *
 *  channel
 *  angle (0-360)
 *  distance (0-255)
 *
 */

static int set_position( int channel, int angle, int dist ) {
    if ( !audio_initialized && sound_init() ) return ( -1 );

    if ( Mix_Playing( channel ) ) {
        Mix_SetPosition( channel, ( Sint16 )angle, ( Uint8 )dist );
        return ( 0 ) ;
    }
    return ( -1 ) ;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : set_distance
 *
 *  Set the "distance" of a channel. (distance) is an integer from 0 to 255
 *  that specifies the location of the sound in relation to the listener.
 *
 *  PARAMS:
 *
 *  channel
 *
 *  distance (0-255)
 *
 */

static int set_distance( int channel, int dist ) {
    if ( !audio_initialized && sound_init() ) return ( -1 );

    if ( Mix_Playing( channel ) ) {
        Mix_SetDistance( channel, ( Uint8 )dist );
        return ( 0 ) ;
    }

    return ( -1 ) ;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : reverse_stereo
 *
 *  Causes a channel to reverse its stereo.
 *
 *  PARAMS:
 *
 *  channel
 *  flip  0 normal  != reverse
 *
 */

static int reverse_stereo( int channel, int flip ) {
    if ( !audio_initialized && sound_init() ) return ( -1 );

    if ( Mix_Playing( channel ) ) {
        Mix_SetReverseStereo( channel, flip );
        return ( 0 ) ;
    }

    return ( -1 ) ;
}


/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : set_master_volume
 *
 *  Set the master volume for all channels.
 *
 *  PARAMS:
 *
 *  int volume
 *
 *  RETURN VALUE:
 *
 * -1 if there is any error
 *  0 if there is no error
 *
 */

static int set_master_volume( int volume ) {
    if ( !audio_initialized && sound_init() ) return ( -1 );

    if ( volume < 0 ) volume = 0;
    if ( volume > 128 ) volume = 128;

#ifndef __SWITCH__
    Mix_MasterVolume( volume );
#endif
    return 0;
}



/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : expire_channel
 *
 *  Change the expiration delay for a particular channel.
 *
 *  The channel will halt after the 'ticks' milliseconds have elapsed, or
 *  remove the expiration if 'ticks' is -1.
 *
 *  This overrides the value passed to the fourth parameter of
 *  play_sound_timed().
 *
 *  Specifying a channel of -1 will set an expiration for _all_ channels.
 *
 *  Any halted channels will have any currently-registered effects
 *  deregistered, and will call any callback specified by Mix_ChannelFinished()
 *  once the halt occurs.
 *
 *  Note that this function does not block for the number of ticks requested;
 *  it just schedules the chunk to expire and notes the time for the mixer to
 *  manage later, and returns immediately.
 *
 *  PARAMS:
 *
 *  channel     the channel to change the expiration time on.
 *  ticks       number of milliseconds from now to let channel play before
 *              halting, -1 to not halt.
 *
 *  RETURN VALUE:
 *
 *  the number of channels that changed expirations.
 *
 */
static int64_t expire_channel( int64_t channel, int64_t ticks ) {
    if ( !audio_initialized && sound_init() ) return -1;
    return Mix_ExpireChannel( ( int )channel, ( int )ticks );
}

/* --------------------------------------------------------------------------- */
/* Channel grouping functions                                                  */
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : group_channel
 *
 *  Assign a tag to a channel.
 *
 *  A tag is an arbitrary number that can be assigned to several mixer channels,
 *  to form groups of channels.
 *
 *  If 'tag' is -1, the tag is removed ( actually -1 is the tag used to
 *  represent the group of all the channels ).
 *
 *  This function replaces the requested channel's current tag; you may only
 *  have one tag per channel.
 *
 *  You may not specify MAX_CHANNEL_POST for a channel.
 *
 *  PARAMS:
 *
 *  which   the channel to set the tag on.
 *  tag     an arbitrary value to assign a channel.
 *
 *  RETURN VALUE:
 *
 *  non-zero on success, zero on error ( no such channel ).
 *
 */
static int64_t group_channel( int64_t channel, int tag ) {
    if ( !audio_initialized && sound_init() ) return -1;
    return Mix_GroupChannel( ( int )channel, tag );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : group_channels
 *
 *  Assign several consecutive channels to the same tag.
 *
 *  A tag is an arbitrary number that can be assigned to several mixer channels,
 *  to form groups of channels.
 *
 *  If 'tag' is -1, the tag is removed ( actually -1 is the tag used to
 *  represent the group of all the channels ).
 *
 *  This function replaces the requested channels' current tags; you may only
 *  have one tag per channel.
 *
 *  You may not specify MAX_CHANNEL_POST for a channel.
 *
 *  Note that this returns success and failure in the _opposite_ way from
 *  Mix_GroupChannel(). We regret the API design mistake.
 *
 *  PARAMS:
 *
 *  from    the first channel to set the tag on.
 *  to      the last channel to set the tag on, inclusive.
 *  tag     an arbitrary value to assign a channel.
 *
 *  RETURN VALUE:
 *
 *  0 if successful, negative on error
 *
 */
static int64_t group_channels( int64_t from, int64_t to, int tag ) {
    if ( !audio_initialized && sound_init() ) return -1;
    return Mix_GroupChannels( ( int )from, ( int )to, tag );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : group_available
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
 *  tag an arbitrary value, assigned to channels, to search for.
 *
 *  RETURN VALUE:
 *
 *  first available channel, or -1 if none are available.
 *
 */
static int64_t group_available( int tag ) {
    if ( !audio_initialized ) return -1;
    return Mix_GroupAvailable( tag );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : group_count
 *
 *  Returns the number of channels in a group.
 *
 *  If tag is -1, this will return the total number of channels allocated,
 *  regardless of what their tag might be.
 *
 *  PARAMS:
 *
 *  tag an arbitrary value, assigned to channels, to search for.
 *
 *  RETURN VALUE:
 *
 *  the number of channels assigned the specified tag.
 *
 */
static int64_t group_count( int tag ) {
    if ( !audio_initialized ) return -1;
    return Mix_GroupCount( tag );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : group_oldest
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
 *  tag an arbitrary value, assigned to channels, to search through.
 *
 *  RETURN VALUE:
 *
 *  the "oldest" sample playing in a group of channels
 *
 */
static int64_t group_oldest( int tag ) {
    if ( !audio_initialized ) return -1;
    return Mix_GroupOldest( tag );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : group_newer
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
 *  tag an arbitrary value, assigned to channels, to search through.
 *
 *  RETURN VALUE:
 *
 *  the "most recent" sample playing in a group of channels
 *
 */
static int64_t group_newer( int tag ) {
    if ( !audio_initialized ) return -1;
    return Mix_GroupNewer( tag );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : halt_group
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
 *  tag     an arbitrary value, assigned to channels, to search for.
 *
 *  RETURN VALUE:
 *
 *  zero, whether any channels were halted or not.
 *
 */
static int64_t halt_group( int64_t tag ) {
    if ( !audio_initialized ) return -1;
    return Mix_HaltGroup( ( int )tag );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : fade_out_group
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
 *  tag     an arbitrary value, assigned to channels, to search for.
 *  ms      number of milliseconds to fade before halting the group.
 *
 *  RETURN VALUE:
 *
 *  the number of channels that were scheduled for fading.
 *
 */
static int64_t fade_out_group( int64_t tag, int64_t ms ) {
    if ( !audio_initialized ) return -1;
    return Mix_FadeOutGroup( ( int )tag, ( int )ms );
}



/* --------------------------------------------------------------------------- */
/* Sound Modules Functions                                                     */
/* --------------------------------------------------------------------------- */

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

    if ( !( filename = string_get( params[0] ) ) ) return ( 0 ) ;

    var = load_music( filename );
    string_discard( params[0] );

    return ( var );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_load_music2
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
    bgload( load_music, params );
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
    if ( params[0] == -1LL ) return -1; // check for !params[0] in internal function
    return( play_music( params[0], params[1] ) );
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
    if ( params[0] == -1LL ) return -1; // check for !params[0] in internal function
    return( play_music( params[0], -1 ) );
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
    if ( params[0] == -1LL ) return -1; // check for !params[0] in internal function
    return( unload_music( params[0] ) );
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
    if ( !s || *s == -1LL ) return -1; // check for !*s in internal function
    r = unload_music( *s );
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
    return( stop_music() );
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
    return( pause_music() );
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
    return( is_paused_music() );
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
    return( resume_music() );
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
    return ( is_playing_music() );
}

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
    return ( set_music_volume( params[0] ) );
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
    return ( get_music_volume( params[0] ) );
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
    return music_rewind();
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
    return ( set_music_playback_position( *( double * ) &params[0] ) );
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
    if ( params[0] == -1LL ) return -1; // check for !params[0] in internal function
    double res = get_music_playback_position( params[0] ) ;
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
    if ( params[0] == -1LL ) return -1; // check for !params[0] in internal function
    double res = get_music_duration( params[0] ) ;
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
    double res = get_music_duration( 0 ) ;
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
    if ( params[0] == -1LL ) return -1; // check for !params[0] in internal function
    double start_time = get_music_loop_start_time( params[0] );
    return *( ( int64_t * )&start_time );
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
    double start_time = get_music_loop_start_time( 0 );
    return *( ( int64_t * )&start_time );
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
    if ( params[0] == -1LL ) return -1; // check for !params[0] in internal function
    double end_time = get_music_loop_end_time( params[0] );
    return *( ( int64_t * )&end_time );
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
    double end_time = get_music_loop_end_time( 0 );
    return *( ( int64_t * )&end_time );
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
    if ( params[0] == -1LL ) return -1; // check for !params[0] in internal function
    double length_time = get_music_loop_length_time( params[0] );
    return *( ( int64_t * )&length_time );
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
    double length_time = get_music_loop_length_time( 0 );
    return *( ( int64_t * )&length_time );
}

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
static int64_t libmod_sound_get_num_music_decoders( INSTANCE *my, int64_t *params ) {
    return get_num_music_decoders(  );
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
static int64_t libmod_sound_get_music_decoder( INSTANCE *my, int64_t *params ) {
    const char *decoder_name = get_music_decoder( params[0] );
    int64_t r = string_new( ( char * )decoder_name );
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
static int64_t libmod_sound_has_music_decoder( INSTANCE *my, int64_t *params ) {
    const char *decoder_name;
    if ( !( decoder_name = string_get( params[0] ) ) ) return 0;
    int available = has_music_decoder( decoder_name );
    string_discard( params[0] );
    return ( int64_t )available;
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
static int64_t libmod_sound_get_music_type( INSTANCE *my, int64_t *params ) {
    return get_music_type( params[0] );
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
static int64_t libmod_sound_get_music_title( INSTANCE *my, int64_t *params ) {
    const char *title = get_music_title( params[0] );
    int64_t r = string_new( ( char * )title );
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
static int64_t libmod_sound_get_music_title_tag( INSTANCE *my, int64_t *params ) {
    const char *title_tag = get_music_title_tag( params[0] );
    int64_t r = string_new( ( char * )title_tag );
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
static int64_t libmod_sound_get_music_artist_tag( INSTANCE *my, int64_t *params ) {
    const char *artist_tag = get_music_artist_tag( params[0] );
    int64_t r = string_new( ( char * )artist_tag );
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
static int64_t libmod_sound_get_music_album_tag( INSTANCE *my, int64_t *params ) {
    const char *album_tag = get_music_album_tag( params[0] );
    int64_t r = string_new( ( char * )album_tag );
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
static int64_t libmod_sound_get_music_copyright_tag( INSTANCE *my, int64_t *params ) {
    const char *copyright_tag = get_music_copyright_tag( params[0] );
    int64_t r = string_new( ( char * )copyright_tag );
    string_use( r );
    return r;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_get_music_type_current
 *
 *  Get the type of the currently playing music.
 *
 *  PARAMS:
 *
 *  my          an INSTANCE pointer
 *  params      pointer to an array of integers
 *
 *  RETURN VALUE:
 *
 *  the type of the currently playing music
 *
 */
static int64_t libmod_sound_get_music_type_current( INSTANCE *my, int64_t *params ) {
    return get_music_type( 0 );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_get_music_title_current
 *
 *  Get the title of the currently playing music.
 *
 *  PARAMS:
 *
 *  my          an INSTANCE pointer
 *  params      pointer to an array of integers
 *
 *  RETURN VALUE:
 *
 *  the title of the currently playing music
 *
 */
static int64_t libmod_sound_get_music_title_current( INSTANCE *my, int64_t *params ) {
    const char *title = get_music_title( 0 );
    int64_t r = string_new( ( char * )title );
    string_use( r );
    return r;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_get_music_title_tag_current
 *
 *  Get the title tag of the currently playing music.
 *
 *  PARAMS:
 *
 *  my          an INSTANCE pointer
 *  params      pointer to an array of integers
 *
 *  RETURN VALUE:
 *
 *  the title tag of the currently playing music
 *
 */
static int64_t libmod_sound_get_music_title_tag_current( INSTANCE *my, int64_t *params ) {
    const char *title_tag = get_music_title_tag( 0 );
    int64_t r = string_new( ( char * )title_tag );
    string_use( r );
    return r;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_get_music_artist_tag_current
 *
 *  Get the artist tag of the currently playing music.
 *
 *  PARAMS:
 *
 *  my          an INSTANCE pointer
 *  params      pointer to an array of integers
 *
 *  RETURN VALUE:
 *
 *  the artist tag of the currently playing music
 *
 */
static int64_t libmod_sound_get_music_artist_tag_current( INSTANCE *my, int64_t *params ) {
    const char *artist_tag = get_music_artist_tag( 0 );
    int64_t r = string_new( ( char * )artist_tag );
    string_use( r );
    return r;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_get_music_album_tag_current
 *
 *  Get the album tag of the currently playing music.
 *
 *  PARAMS:
 *
 *  my          an INSTANCE pointer
 *  params      pointer to an array of integers
 *
 *  RETURN VALUE:
 *
 *  the album tag of the currently playing music
 *
 */
static int64_t libmod_sound_get_music_album_tag_current( INSTANCE *my, int64_t *params ) {
    const char *album_tag = get_music_album_tag( 0 );
    int64_t r = string_new( ( char * )album_tag );
    string_use( r );
    return r;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_get_music_copyright_tag_current
 *
 *  Get the copyright tag of the currently playing music.
 *
 *  PARAMS:
 *
 *  my          an INSTANCE pointer
 *  params      pointer to an array of integers
 *
 *  RETURN VALUE:
 *
 *  the copyright tag of the currently playing music
 *
 */
static int64_t libmod_sound_get_music_copyright_tag_current( INSTANCE *my, int64_t *params ) {
    const char *copyright_tag = get_music_copyright_tag( 0 );
    int64_t r = string_new( ( char * )copyright_tag );
    string_use( r );
    return r;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_fade_music_in
 *
 *  Play a MOD/OGG fading in it
 *
 *  PARAMS:
 *      mod pointer
 *      number of loops (-1 infinite loops)
 *      ms  microsends of fading
 *
 *  RETURN VALUE:
 *
 *  -1 if there is any error
 *
 */

static int64_t libmod_sound_fade_music_in( INSTANCE * my, int64_t * params ) {
    if ( params[0] == -1LL ) return -1; // check for !params[0] in internal function
    return ( fade_music_in( params[0], params[1], params[2] ) );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_fade_music_in_pos
 *
 *  Play a MOD/OGG fading in it
 *
 *  PARAMS:
 *      mod pointer
 *      number of loops (-1 infinite loops)
 *      ms  microsends of fading
 *      position position in seconds
 *
 *  RETURN VALUE:
 *
 *  -1 if there is any error
 *
 */

static int64_t libmod_sound_fade_music_in_pos( INSTANCE * my, int64_t * params ) {
    if ( params[0] == -1LL ) return -1; // check for !params[0] in internal function
    return ( fade_music_in_pos( params[0], params[1], params[2], *( double * ) &params[3] ) );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_fade_music_off
 *
 *  Stop the play of a mod
 *
 *  PARAMS:
 *
 *  ms  microsends of fading
 *
 *  RETURN VALUE:
 *
 *  -1 if there is any error
 *
 */

static int64_t libmod_sound_fade_music_off( INSTANCE * my, int64_t * params ) {
    return ( fade_music_off( params[0] ) );
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

static int libmod_sound_is_fading_music( INSTANCE * my, int64_t * params ) {
    return is_fading_music();
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
 *      wav id
 *
 */

static int64_t libmod_sound_load_sound( INSTANCE * my, int64_t * params ) {
    int64_t var;
    const char * filename ;

    if ( !( filename = string_get( params[0] ) ) ) return ( 0 ) ;

    var = ( int64_t )( intptr_t ) load_sound( filename );
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
    bgload( load_sound, params );
    return 0;
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
 *  -1 if there is any error
 *  0 if all goes ok
 *
 */

static int64_t libmod_sound_play_sound( INSTANCE * my, int64_t * params ) {
    if ( params[0] == -1LL ) return -1; // check for !params[0] in internal function
    return( play_sound( params[0], params[1], -1 ) );
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
 *  -1 if there is any error
 *  0 if all goes ok
 *
 */

static int64_t libmod_sound_play_sound_once( INSTANCE * my, int64_t * params ) {
    if ( params[0] == -1LL ) return -1; // check for !params[0] in internal function
    return( play_sound( params[0], 0, -1 ) );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_play_sound_channel
 *
 *  Play a WAV
 *
 *  PARAMS:
 *      wav id;
 *      number of loops (-1 infinite loops)
 *      channel ( -1 like libmod_sound_play_sound )
 *
 *  RETURN VALUE:
 *
 *  -1 if there is any error
 *  0 if all goes ok
 *
 */

static int64_t libmod_sound_play_sound_channel( INSTANCE * my, int64_t * params ) {
    if ( params[0] == -1LL ) return -1; // check for !params[0] in internal function
    return( play_sound( params[0], params[1], params[2] ) );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_play_sound_timed
 *
 *  Play a WAV with timeout
 *
 *  PARAMS:
 *      wav pointer;
 *      number of loops (-1 infinite loops)
 *      channel (-1 any channel)
 *      ticks timeout in milliseconds ( -1 no timeout )
 *
 *  RETURN VALUE:
 *
 * -1 if there is any error
 *  else channel where the music plays
 *
 */

static int64_t libmod_sound_play_sound_timed( INSTANCE * my, int64_t * params ) {
    if ( params[0] == -1LL ) return -1; // check for !params[0] in internal function
    return( play_sound_timed( params[0], params[1], params[2], params[3] ) );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_fade_sound_in
 *
 *  Play a WAV fading in it
 *
 *  PARAMS:
 *      wav pointer;
 *      number of loops (-1 infinite loops)
 *      ms  microsends of fading
 *      channel (-1 any channel)
 *
 *  RETURN VALUE:
 *
 * -1 if there is any error
 *
 */

static int64_t libmod_sound_fade_sound_in( INSTANCE * my, int64_t * params ) {
    if ( params[0] == -1LL ) return -1; // check for !params[0] in internal function
    return( fade_sound_in( params[0], params[1], params[2], params[3] ) );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_fade_sound_in_timed
 *
 *  Play a WAV fading in it with timeout
 *
 *  PARAMS:
 *      wav pointer;
 *      number of loops (-1 infinite loops)
 *      ms  microsends of fading
 *      channel (-1 any channel)
 *      ticks timeout in milliseconds ( -1 no timeout )
 *
 *  RETURN VALUE:
 *
 * -1 if there is any error
 *
 */

static int64_t libmod_sound_fade_sound_in_timed( INSTANCE * my, int64_t * params ) {
    if ( params[0] == -1LL ) return -1; // check for !params[0] in internal function
    return( fade_sound_in_timed( params[0], params[1], params[2], params[3], params[4] ) );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_is_fading_channel
 *
 *  Query if a channel is fading
 *
 *  PARAMS:
 *      channel id
 *
 *  RETURN VALUE:
 *
 * -1 if there is any error
 *
 */

static int64_t libmod_sound_is_fading_channel( INSTANCE * my, int64_t * params ) {
    if ( params[0] == -1LL ) return -1; // check for !params[0] in internal function
    return( is_fading_channel( params[0] ) );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_unload_sound
 *
 *  Frees the resources from a wav, unloading it
 *
 *  PARAMS:
 *
 *  mod id
 *
 *  RETURN VALUE:
 *
 *  -1 if there is any error
 *  0 if all goes ok
 *
 */

static int64_t libmod_sound_unload_sound( INSTANCE * my, int64_t * params ) {
    if ( params[0] == -1LL ) return -1; // check for !params[0] in internal function
    return( unload_sound( params[0] ) );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_unload_sound_and_free
 *
 *  Frees the resources from a wav, unloading it
 *
 *  PARAMS:
 *
 *  mod *id
 *
 *  RETURN VALUE:
 *
 *  -1 if there is any error
 *  0 if all goes ok
 *
 */

static int64_t libmod_sound_unload_sound_and_free( INSTANCE * my, int64_t * params ) {
    int64_t *s = ( int64_t * )( intptr_t )( params[0] ), r;
    if ( !s || *s == -1LL ) return -1; // check for !*s in internal function
    r = unload_sound( *s );
    *s = 0LL;
    return( r );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_stop_sound
 *
 *  Stop a wav playing
 *
 *  PARAMS:
 *
 *  channel
 *
 *  RETURN VALUE:
 *
 *  -1 if there is any error
 *  0 if all goes ok
 *
 */

static int64_t libmod_sound_stop_sound( INSTANCE * my, int64_t * params ) {
    return( stop_sound( params[0] ) );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_pause_sound
 *
 *  Pause a wav playing, you can resume it after
 *
 *  PARAMS:
 *
 *  channel
 *
 *  RETURN VALUE:
 *
 *  -1 if there is any error
 *  0 if all goes ok
 *
 */

static int64_t libmod_sound_pause_sound( INSTANCE * my, int64_t * params ) {
    return ( pause_sound( params[0] ) );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_is_paused_sound
 *
 *  Query if channel is paused
 *
 *  PARAMS:
 *
 *  int channel
 *
 *  RETURN VALUE:
 *
 * -1 if there is any error
 *
 */

static int64_t libmod_sound_is_paused_sound( INSTANCE * my, int64_t * params ) {
    return ( is_paused_sound( params[0] ) );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : resume_sound
 *
 *  Resume a wav playing, paused before
 *
 *  PARAMS:
 *
 *  channel
 *
 *  RETURN VALUE:
 *
 *  -1 if there is any error
 *  0 if all goes ok
 *
 */

static int64_t libmod_sound_resume_sound( INSTANCE * my, int64_t * params ) {
    return ( resume_sound( params[0] ) );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : is_playing_sound
 *
 *  Check a wav playing
 *
 *  PARAMS:
 *
 *  channel
 *
 *  RETURN VALUE:
 *
 *  -1 if there is any error
 *  TRUE OR FALSE if there is no error
 *
 */

static int64_t libmod_sound_is_playing_sound( INSTANCE * my, int64_t * params ) {
    return ( is_playing_sound( params[0] ) );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_fade_channel_off
 *
 *  Fade out channel
 *
 *  PARAMS:
 *      channel id
 *      ms  microsends of fading
 *
 *  RETURN VALUE:
 *
 * -1 if there is any error
 *
 */

static int64_t libmod_sound_fade_channel_off( INSTANCE * my, int64_t * params ) {
    return ( fade_channel_off( params[0], params[1] ) );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_set_channel_volume
 *
 *  Set the volume for a channel (0-128)
 *
 *  PARAMS:
 *
 *  channel
 *  int volume
 *
 *  RETURN VALUE:
 *
 *  -1 if there is any error
 *  0 if there is no error
 *
 */

static int64_t libmod_sound_set_channel_volume( INSTANCE * my, int64_t * params ) {
    return( set_channel_volume( params[0], params[1] ) );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_set_channel_volume
 *
 *  Set the volume for all channels (0-128)
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

static int64_t libmod_sound_set_channel_volume_all( INSTANCE * my, int64_t * params ) {
    return( set_channel_volume( -1, params[1] ) );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_reserve_channels
 *
 *  Reserve the first channels (0 -> n-1) for the application, i.e. don't allocate
 *  them dynamically to the next sample if requested with a -1 value below.
 *
 *  PARAMS:
 *  number of channels to reserve.
 *
 *  RETURN VALUE:
 *  number of reserved channels.
 *  -1 if there is any error
 *
 */

static int64_t libmod_sound_reserve_channels( INSTANCE * my, int64_t * params ) {
    return ( reserve_channels( params[0] ) );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_set_sound_volume
 *
 *  Set the volume for a wav playing (0-128)
 *
 *  PARAMS:
 *
 *  channel
 *  int volume
 *
 *  RETURN VALUE:
 *
 *  -1 if there is any error
 *  0 if there is no error
 *
 */

static int64_t libmod_sound_set_sound_volume( INSTANCE * my, int64_t * params ) {
    return( set_sound_volume( params[0], params[1] ) );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_set_sound_volume_all
 *
 *  Set the volume for all channels (0-128)
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

static int64_t libmod_sound_set_sound_volume_all( INSTANCE * my, int64_t * params ) {
    return( set_sound_volume( -1, params[1] ) );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_set_panning
 *
 *  Set the panning for a wav channel
 *
 *  PARAMS:
 *
 *  channel
 *  left volume (0-255)
 *  right volume (0-255)
 *
 */

static int64_t libmod_sound_set_panning( INSTANCE * my, int64_t * params ) {
    return( set_panning( params[0], params[1], params[2] ) );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_set_position
 *
 *  Set the position of a channel. (angle) is an integer from 0 to 360
 *
 *  PARAMS:
 *
 *  channel
 *  angle (0-360)
 *  distance (0-255)
 *
 */

static int64_t libmod_sound_set_position( INSTANCE * my, int64_t * params ) {
    return( set_position( params[0], params[1], params[2] ) );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_set_distance
 *
 *  Set the "distance" of a channel. (distance) is an integer from 0 to 255
 *  that specifies the location of the sound in relation to the listener.
 *
 *  PARAMS:
 *
 *  channel
 *
 *  distance (0-255)
 *
 */

static int64_t libmod_sound_set_distance( INSTANCE * my, int64_t * params ) {
    return( set_distance( params[0], params[1] ) );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_reverse_stereo
 *
 *  Causes a channel to reverse its stereo.
 *
 *  PARAMS:
 *
 *  channel
 *
 *  flip 0 normal != reverse
 *
 */

static int64_t libmod_sound_reverse_stereo( INSTANCE * my, int64_t * params ) {
    return( reverse_stereo( params[0], params[1] ) );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_set_master_volume
 *
 *  Set the master volume for all channels.
 *
 *  PARAMS:
 *
 *  int volume
 *
 *  RETURN VALUE:
 *
 * -1 if there is any error
 *  0 if there is no error
 *
 */

static int64_t libmod_sound_set_master_volume( INSTANCE * my, int64_t * params ) {
    return ( set_master_volume( params[0] ) );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_expire_channel
 *
 *  Change the expiration delay for a particular channel.
 *
 *  The channel will halt after the 'ticks' milliseconds have elapsed, or
 *  remove the expiration if 'ticks' is -1.
 *
 *  This overrides the value passed to the fourth parameter of 
 *  libmod_sound_play_sound_timed().
 *
 *  Specifying a channel of -1 will set an expiration for _all_ channels.
 *
 *  Any halted channels will have any currently-registered effects
 *  deregistered, and will call any callback specified by Mix_ChannelFinished()
 *  once the halt occurs.
 *
 *  Note that this function does not block for the number of ticks requested;
 *  it just schedules the chunk to expire and notes the time for the mixer to
 *  manage later, and returns immediately.
 *
 *  PARAMS:
 *
 *  my          an INSTANCE pointer
 *  params      pointer to an array of integers containing the channel and ticks
 *
 *  RETURN VALUE:
 *
 *  the number of channels that changed expirations.
 *
 */
static int64_t libmod_sound_expire_channel( INSTANCE *my, int64_t *params ) {
    return Mix_ExpireChannel( ( int )params[0], ( int )params[1] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_expire_channel_all
 *
 *  Change the expiration delay for all channel.
 *
 *  The channel will halt after the 'ticks' milliseconds have elapsed, or
 *  remove the expiration if 'ticks' is -1.
 *
 *  This overrides the value passed to the fourth parameter of 
 *  libmod_sound_play_sound_timed().
 *
 *  Any halted channels will have any currently-registered effects
 *  deregistered, and will call any callback specified by Mix_ChannelFinished()
 *  once the halt occurs.
 *
 *  Note that this function does not block for the number of ticks requested;
 *  it just schedules the chunk to expire and notes the time for the mixer to
 *  manage later, and returns immediately.
 *
 *  PARAMS:
 *
 *  my          an INSTANCE pointer
 *  params      pointer to an array of integers containing the ticks
 *
 *  RETURN VALUE:
 *
 *  the number of channels that changed expirations.
 *
 */
static int64_t libmod_sound_expire_channel_all( INSTANCE *my, int64_t *params ) {
    return expire_channel( -1, params[0] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_group_channel
 *
 *  Assign a tag to a channel.
 *
 *  A tag is an arbitrary number that can be assigned to several mixer channels,
 *  to form groups of channels.
 *
 *  If 'tag' is -1, the tag is removed ( actually -1 is the tag used to
 *  represent the group of all the channels ).
 *
 *  This function replaces the requested channel's current tag; you may only
 *  have one tag per channel.
 *
 *  You may not specify MAX_CHANNEL_POST for a channel.
 *
 *  PARAMS:
 *
 *  my          an INSTANCE pointer
 *  params      pointer to an array of integers containing the channel and tag
 *
 *  RETURN VALUE:
 *
 *  non-zero on success, zero on error ( no such channel ).
 *
 */
static int64_t libmod_sound_group_channel( INSTANCE *my, int64_t *params ) {
    return group_channel( params[0], params[1] );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_sound_group_channels
 *
 *  Assign several consecutive channels to the same tag.
 *
 *  A tag is an arbitrary number that can be assigned to several mixer channels,
 *  to form groups of channels.
 *
 *  If 'tag' is -1, the tag is removed ( actually -1 is the tag used to
 *  represent the group of all the channels ).
 *
 *  This function replaces the requested channels' current tags; you may only
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
static int64_t libmod_sound_group_channels( INSTANCE *my, int64_t *params ) {
    return group_channels( params[0], params[1], params[2] );
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
static int64_t libmod_sound_group_available( INSTANCE *my, int64_t *params ) {
    return group_available( params[0] );
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
static int64_t libmod_sound_group_count( INSTANCE *my, int64_t *params ) {
    return group_count( params[0] );
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
static int64_t libmod_sound_group_oldest( INSTANCE *my, int64_t *params ) {
    return group_oldest( params[0] );
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
static int64_t libmod_sound_group_newer( INSTANCE *my, int64_t *params ) {
    return group_newer( params[0] );
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
static int64_t libmod_sound_halt_group( INSTANCE *my, int64_t *params ) {
    return halt_group( params[0] );
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
static int64_t libmod_sound_fade_out_group( INSTANCE *my, int64_t *params ) {
    return fade_out_group( params[0], params[1] );
}

/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/* Init/Quit                                                                   */
/* --------------------------------------------------------------------------- */

static int64_t libmod_sound_init( INSTANCE * my, int64_t * params ) {
    return( sound_init() );
}

/* --------------------------------------------------------------------------- */

static int64_t libmod_sound_quit( INSTANCE * my, int64_t * params ) {
    sound_quit();
    return( 0 );
}

/* --------------------------------------------------------------------------- */
/* Module/Plugin Initialization Functions                                      */

void  __bgdexport( libmod_sound, module_initialize )() {
    if ( !SDL_WasInit( SDL_INIT_AUDIO ) ) SDL_InitSubSystem( SDL_INIT_AUDIO );
    Mix_Init( MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID );
}

/* --------------------------------------------------------------------------- */

void __bgdexport( libmod_sound, module_finalize )() {
    if ( SDL_WasInit( SDL_INIT_AUDIO ) ) SDL_QuitSubSystem( SDL_INIT_AUDIO );
    Mix_Quit();
}

/* --------------------------------------------------------------------------- */
/* exports                                                                     */
/* --------------------------------------------------------------------------- */

#include "libmod_sound_exports.h"

/* --------------------------------------------------------------------------- */


/*
TODO (?):

extern DECLSPEC int SDLCALL Mix_SetSoundFonts( const char *paths );
extern DECLSPEC const char* SDLCALL Mix_GetSoundFonts( void );
extern DECLSPEC int SDLCALL Mix_EachSoundFont( int ( SDLCALL *function )( const char*, void* ), void *data );
extern DECLSPEC int SDLCALL Mix_SetTimidityCfg( const char *path );
extern DECLSPEC const char* SDLCALL Mix_GetTimidityCfg( void );
*/
