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
/* Son las variables que se desea acceder.                                     */
/* El interprete completa esta estructura, si la variable existe.              */
/* (usada en tiempo de ejecucion)                                              */

DLVARFIXUP  __bgdexport( libmod_sound, globals_fixup )[] =
{
    /* Nombre de variable global, puntero al dato, tamano del elemento, cantidad de elementos */
    { "sound.sound_freq"        , NULL, -1, -1 },
    { "sound.sound_mode"        , NULL, -1, -1 },
    { "sound.sound_channels"    , NULL, -1, -1 },
    { NULL                      , NULL, -1, -1 }
};

/* --------------------------------------------------------------------------- */
/* Interfaz SDL_RWops Bennu                                                    */
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
    if ( !audio_initialized ) {
        /* Initialize variables: but limit quality to some fixed options */

        int audio_rate = GLOQWORD( libmod_sound, SOUND_FREQ );

        if ( audio_rate > 22050 )       audio_rate = 44100;
        else if ( audio_rate > 11025 )  audio_rate = 22050;
        else                            audio_rate = 11025;

        Uint16 audio_format = AUDIO_S16;
        int audio_channels = GLOQWORD( libmod_sound, SOUND_MODE ) + 1;
        int audio_buffers = 1024 * audio_rate / 22050;

        /* Open the audio device */
        if ( Mix_OpenAudio( audio_rate, audio_format, audio_channels, audio_buffers ) >= 0 ) {
            GLOQWORD( libmod_sound, SOUND_CHANNELS ) <= 32 ? Mix_AllocateChannels( GLOQWORD( libmod_sound, SOUND_CHANNELS ) ) : Mix_AllocateChannels( 32 ) ;
            Mix_QuerySpec( &audio_rate, &audio_format, &audio_channels );
            int audio_mix_channels = Mix_AllocateChannels( -1 ) ;
            GLOQWORD( libmod_sound, SOUND_CHANNELS ) = audio_mix_channels ;

            audio_initialized = 1;
            return 0;
        }
    }

    // fprintf( stderr, "[SOUND] No se pudo inicializar el audio: %s\n", SDL_GetError() ) ;
    audio_initialized = 0;
    return -1 ;
}

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

    //falta por comprobar que todo esté descargado

    Mix_CloseAudio();

    audio_initialized = 0;
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

static int is_fading_music( ) {
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
/* --------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/* Sonido WAV                                                                  */
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

static int  set_sound_volume( int64_t sample, int volume ) {
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
 *  Set the volume for wav playing (0-128) IN CHANNEL
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
/* Sonido                                                                      */
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
 *  FUNCTION : libmod_sound_unload_music2
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

static int64_t libmod_sound_unload_music2( INSTANCE * my, int64_t * params ) {
    int64_t *s = (int64_t *)(intptr_t)(params[0]), r;
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

static int libmod_sound_is_fading_music() {
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
 *  FUNCTION : libmod_sound_load_sound2
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
 *  FUNCTION : libmod_sound_play_sound_channel
 *
 *  Play a WAV
 *
 *  PARAMS:
 *      wav id;
 *      number of loops (-1 infinite loops)
 *      channel (-1 like libmod_sound_play_sound)
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
 *  FUNCTION : libmod_sound_unload_sound2
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

static int64_t libmod_sound_unload_sound2( INSTANCE * my, int64_t * params ) {
    int64_t *s = (int64_t *)(intptr_t)(params[0]), r;
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

static int64_t libmod_sound_set_channel_volume( INSTANCE * my, int64_t * params ) {
    return( set_channel_volume( params[0], params[1] ) );
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

static int64_t libmod_sound_set_music_position( INSTANCE * my, int64_t * params ) {
    return ( Mix_SetMusicPosition( *( double * ) &params[0] ) );
}

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
/* Funciones de inicializacion del modulo/plugin                               */

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
