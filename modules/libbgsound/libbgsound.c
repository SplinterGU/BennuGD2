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

#include "libbgsound.h"

/* --------------------------------------------------------------------------- */

static int audio_initialized = 0;

/* --------------------------------------------------------------------------- */

int bg_sound_is_initialized() {
    return audio_initialized;
}

/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/* SDL_RWops Bennu Interface                                                   */
/* --------------------------------------------------------------------------- */

static Sint64 SDLCALL __libbgsound_size_cb( struct SDL_RWops *context ) {
    return( file_size( context->hidden.unknown.data1 ) );
}

static Sint64 SDLCALL __libbgsound_seek_cb( struct SDL_RWops *context, Sint64 offset, int whence ) {
    if ( file_seek( context->hidden.unknown.data1, offset, whence ) < 0 ) return ( -1 );
    return( file_pos( context->hidden.unknown.data1 ) );
}

static size_t SDLCALL __libbgsound_read_cb( struct SDL_RWops *context, void *ptr, size_t size, size_t maxnum ) {
    int ret = file_read( context->hidden.unknown.data1, ptr, size * maxnum );
    if ( ret > 0 ) ret /= size;
    return( ret );
}

static size_t SDLCALL __libbgsound_write_cb( struct SDL_RWops *context, const void *ptr, size_t size, size_t num ) {
    int ret = file_write( context->hidden.unknown.data1, ( void * )ptr, size * num );
    if ( ret > 0 ) ret /= size;
    return( ret );
}

static int SDLCALL __libbgsound_close_cb( struct SDL_RWops *context ) {
    if ( context ) {
        file_close( context->hidden.unknown.data1 );
        SDL_FreeRW( context );
    }
    return( 0 );
}

SDL_RWops *SDL_RWFromBGDFP( file *fp ) {
    SDL_RWops *rwops = SDL_AllocRW();
    if ( rwops ) {
        rwops->size = __libbgsound_size_cb;
        rwops->seek = __libbgsound_seek_cb;
        rwops->read = __libbgsound_read_cb;
        rwops->write = __libbgsound_write_cb;
        rwops->close = __libbgsound_close_cb;
        rwops->hidden.unknown.data1 = fp;
    }
    return( rwops );
}

/* --------------------------------------------------------------------------- */

int bg_sound_init( int freq, int mode, int channels ) {
    bg_sound_quit();

    int audio_rate = freq;
    if ( audio_rate > 44100 )  audio_rate = 48000;
    else if ( audio_rate > 22050 )  audio_rate = 44100;
    else if ( audio_rate > 11025 )  audio_rate = 22050;
    else                            audio_rate = 11025;

    Uint16 audio_format = MIX_DEFAULT_FORMAT;
    int audio_channels = mode;

    if ( Mix_OpenAudio( audio_rate, audio_format, audio_channels, 4096 ) >= 0 ) {
        Mix_AllocateChannels( channels > 32 ? 32 : (channels > 0 ? channels : 32) );
        audio_initialized = 1;
        return 0;
    }

    audio_initialized = 0;
    return -1;
}

void bg_sound_quit() {
    if ( !audio_initialized ) return;
    Mix_CloseAudio();
    audio_initialized = 0;
}

void bg_sound_system_init() {
    if ( !SDL_WasInit( SDL_INIT_AUDIO ) ) SDL_InitSubSystem( SDL_INIT_AUDIO );
    Mix_Init( MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID );
}

void bg_sound_system_finalize() {
    if ( SDL_WasInit( SDL_INIT_AUDIO ) ) SDL_QuitSubSystem( SDL_INIT_AUDIO );
    Mix_Quit();
}

/* Music */

int64_t bg_sound_load_music( const char * filename ) {
    Mix_Music *music = NULL;
    file      *fp;

    if ( !audio_initialized ) return ( 0 );
    if ( !( fp = file_open( filename, "rb" ) ) ) return ( 0 );

    SDL_RWops * rwops = SDL_RWFromBGDFP( fp );
    if ( !rwops ) {
        file_close( fp );
        return( 0 );
    }

    if ( !( music = Mix_LoadMUS_RW( rwops, 1 ) ) ) return ( 0 );
    return (( int64_t )( intptr_t )music );
}

int bg_sound_play_music( int64_t id, int loops ) {
    if ( audio_initialized && id ) return Mix_PlayMusic(( Mix_Music * )( intptr_t )id, loops );
    return( -1 );
}

int bg_sound_fade_music_in( int64_t id, int loops, int ms ) {
    if ( audio_initialized && id ) return( Mix_FadeInMusic(( Mix_Music * )( intptr_t )id, loops, ms ) );
    return( -1 );
}

int bg_sound_fade_music_in_pos( int64_t id, int loops, int ms, double position ) {
    if ( audio_initialized && id ) return( Mix_FadeInMusicPos(( Mix_Music * )( intptr_t )id, loops, ms, position ) );
    return( -1 );
}

int bg_sound_fade_music_off( int ms ) {
    if ( !audio_initialized ) return ( 0 );
    return ( Mix_FadeOutMusic( ms ) );
}

int bg_sound_is_fading_music() {
    if ( audio_initialized ) return Mix_FadingMusic();
    return ( -1 );
}

int bg_sound_unload_music( int64_t id ) {
    if ( audio_initialized && id ) {
        if ( Mix_PlayingMusic() ) Mix_HaltMusic();
        Mix_FreeMusic(( Mix_Music * )( intptr_t )id );
    }
    return ( 0 ) ;
}

int bg_sound_stop_music( void ) {
    if ( audio_initialized ) Mix_HaltMusic();
    return ( 0 ) ;
}

int bg_sound_pause_music( void ) {
    if ( audio_initialized ) Mix_PauseMusic();
    return ( 0 ) ;
}

int bg_sound_is_paused_music( void ) {
    if ( audio_initialized ) return Mix_PausedMusic();
    return ( -1 ) ;
}

int bg_sound_resume_music( void ) {
    if ( audio_initialized ) Mix_ResumeMusic();
    return( 0 ) ;
}

int bg_sound_is_playing_music( void ) {
    if ( !audio_initialized ) return ( 0 );
    return Mix_PlayingMusic();
}

int bg_sound_set_music_volume( int volume ) {
    if ( !audio_initialized ) return ( -1 );
    if ( volume < 0 ) volume = 0;
    if ( volume > 128 ) volume = 128;
    Mix_VolumeMusic( volume );
    return 0;
}

int bg_sound_get_music_volume( int64_t id ) {
    if ( !audio_initialized || !id ) return ( 0 );
#if (SDL_MIXER_VERSION_ATLEAST(2, 6, 0))
    return Mix_GetMusicVolume(( Mix_Music * )( intptr_t )id );
#else
    return -1;
#endif
}

int bg_sound_music_rewind() {
    if ( audio_initialized ) Mix_RewindMusic();
    return 0;
}

int bg_sound_set_music_playback_position( double position ) {
    if ( !audio_initialized ) return ( 0 );
    return ( Mix_SetMusicPosition( position ) );
}

double bg_sound_get_music_playback_position( int64_t id ) {
    if ( !audio_initialized || !id ) return ( 0 );
#if (SDL_MIXER_VERSION_ATLEAST(2, 6, 0))
    return ( Mix_GetMusicPosition(( Mix_Music * )( intptr_t )id ) );
#else
    return -1;
#endif
}

double bg_sound_get_music_duration( int64_t id ) {
    if ( !audio_initialized ) return 0.0;
#if (SDL_MIXER_VERSION_ATLEAST(2, 6, 0))
    return Mix_MusicDuration(( Mix_Music * )( intptr_t )id );
#else
    return -1;
#endif
}

double bg_sound_get_music_loop_start_time( int64_t id ) {
    if ( !audio_initialized ) return 0.0;
#if (SDL_MIXER_VERSION_ATLEAST(2, 6, 0))
    return Mix_GetMusicLoopStartTime(( Mix_Music * )( intptr_t )id );
#else
    return -1;
#endif
}

double bg_sound_get_music_loop_end_time( int64_t id ) {
    if ( !audio_initialized ) return 0.0;
#if (SDL_MIXER_VERSION_ATLEAST(2, 6, 0))
    return Mix_GetMusicLoopEndTime(( Mix_Music * )( intptr_t )id );
#else
    return -1;
#endif
}

double bg_sound_get_music_loop_length_time( int64_t id ) {
    if ( !audio_initialized ) return 0.0;
#if (SDL_MIXER_VERSION_ATLEAST(2, 6, 0))
    return Mix_GetMusicLoopLengthTime(( Mix_Music * )( intptr_t )id );
#else
    return -1;
#endif
}

/* Sound (Chunk) */

int64_t bg_sound_load_sound( const char * filename ) {
    Mix_Chunk *sound = NULL;
    file      *fp;

    if ( !audio_initialized ) return ( 0 );
    if ( !( fp = file_open( filename, "rb" ) ) ) return ( 0 );

    SDL_RWops * rwops = SDL_RWFromBGDFP( fp );
    if ( !rwops ) {
        file_close( fp );
        return( 0 );
    }

    if ( !( sound = Mix_LoadWAV_RW( rwops, 1 ) ) ) return( 0 );
    return (( int64_t ) ( intptr_t ) sound );
}

int bg_sound_play_sound( int64_t id, int loops, int channel ) {
    if ( audio_initialized && id ) return ( ( int ) Mix_PlayChannel( channel, ( Mix_Chunk * )( intptr_t )id, loops ) );
    return ( -1 );
}

int bg_sound_play_sound_timed( int64_t id, int loops, int channel, int ticks ) {
    if ( audio_initialized && id ) return ( ( int ) Mix_PlayChannelTimed( channel, ( Mix_Chunk * )( intptr_t )id, loops, ticks ) );
    return ( -1 );
}

int bg_sound_fade_sound_in( int64_t id, int loops, int ms, int channel ) {
    if ( audio_initialized && id ) return( Mix_FadeInChannel( channel, ( Mix_Chunk * )( intptr_t )id, loops, ms ) );
    return( -1 );
}

int bg_sound_fade_sound_in_timed( int64_t id, int loops, int ms, int channel, int ticks ) {
    if ( audio_initialized && id ) return( Mix_FadeInChannelTimed( channel, ( Mix_Chunk * )( intptr_t )id, loops, ms, ticks ) );
    return( -1 );
}

int bg_sound_is_fading_channel( int channel ) {
    if ( audio_initialized ) return Mix_FadingChannel( channel );
    return( -1 );
}

int bg_sound_unload_sound( int64_t id ) {
    if ( audio_initialized && id ) Mix_FreeChunk(( Mix_Chunk * )( intptr_t )id );
    return ( 0 );
}

int bg_sound_stop_sound( int channel ) {
    if ( audio_initialized && Mix_Playing( channel ) ) return( Mix_HaltChannel( channel ) );
    return ( -1 ) ;
}

int bg_sound_pause_sound( int channel ) {
    if ( audio_initialized && Mix_Playing( channel ) ) {
        Mix_Pause( channel );
        return ( 0 ) ;
    }
    return ( -1 ) ;
}

int bg_sound_is_paused_sound( int channel ) {
    if ( audio_initialized ) return Mix_Paused( channel );
    return ( -1 ) ;
}

int bg_sound_resume_sound( int channel ) {
    if ( audio_initialized && Mix_Playing( channel ) ) {
        Mix_Resume( channel );
        return ( 0 ) ;
    }
    return ( -1 ) ;
}

int bg_sound_is_playing_sound( int channel ) {
    if ( audio_initialized ) return( Mix_Playing( channel ) );
    return ( 0 );
}

int bg_sound_fade_channel_off( int channel, int ms ) {
    if ( audio_initialized ) return( Mix_FadeOutChannel( channel, ms ) );
    return( -1 );
}

int bg_sound_set_sound_volume( int64_t sample, int volume ) {
    if ( !audio_initialized ) return ( -1 );
    if ( volume < 0 ) volume = 0;
    if ( volume > 128 ) volume = 128;
    if ( sample ) return( Mix_VolumeChunk(( Mix_Chunk * )( intptr_t )sample, volume ) );
    return -1 ;
}

int bg_sound_set_channel_volume( int channel, int volume ) {
    if ( !audio_initialized ) return ( -1 );
    if ( volume < 0 ) volume = 0;
    if ( volume > 128 ) volume = 128;
    return( Mix_Volume( channel, volume ) );
}

int bg_sound_reserve_channels( int channels ) {
    if ( !audio_initialized ) return ( -1 );
    return Mix_ReserveChannels( channels );
}

int bg_sound_set_panning( int channel, int left, int right ) {
    if ( audio_initialized && Mix_Playing( channel ) ) {
        Mix_SetPanning( channel, ( Uint8 )left, ( Uint8 )right );
        return ( 0 ) ;
    }
    return ( -1 ) ;
}

int bg_sound_set_position( int channel, int angle, int dist ) {
    if ( audio_initialized && Mix_Playing( channel ) ) {
        Mix_SetPosition( channel, ( Sint16 )angle, ( Uint8 )dist );
        return ( 0 ) ;
    }
    return ( -1 ) ;
}

int bg_sound_set_distance( int channel, int dist ) {
    if ( audio_initialized && Mix_Playing( channel ) ) {
        Mix_SetDistance( channel, ( Uint8 )dist );
        return ( 0 ) ;
    }
    return ( -1 ) ;
}

int bg_sound_reverse_stereo( int channel, int flip ) {
    if ( audio_initialized && Mix_Playing( channel ) ) {
        Mix_SetReverseStereo( channel, flip );
        return ( 0 ) ;
    }
    return ( -1 ) ;
}

int bg_sound_set_master_volume( int volume ) {
    if ( !audio_initialized ) return ( -1 );
    if ( volume < 0 ) volume = 0;
    if ( volume > 128 ) volume = 128;
#ifndef __SWITCH__
    Mix_MasterVolume( volume );
#endif
    return 0;
}

int64_t bg_sound_expire_channel( int64_t channel, int64_t ticks ) {
    if ( !audio_initialized ) return -1;
    return Mix_ExpireChannel( ( int )channel, ( int )ticks );
}

int64_t bg_sound_group_channel( int64_t channel, int tag ) {
    if ( !audio_initialized ) return -1;
    return Mix_GroupChannel( ( int )channel, tag );
}

int64_t bg_sound_group_channels( int64_t from, int64_t to, int tag ) {
    if ( !audio_initialized ) return -1;
    return Mix_GroupChannels( ( int )from, ( int )to, tag );
}

int64_t bg_sound_group_available( int tag ) {
    if ( !audio_initialized ) return -1;
    return Mix_GroupAvailable( tag );
}

int64_t bg_sound_group_count( int tag ) {
    if ( !audio_initialized ) return -1;
    return Mix_GroupCount( tag );
}

int64_t bg_sound_group_oldest( int tag ) {
    if ( !audio_initialized ) return -1;
    return Mix_GroupOldest( tag );
}

int64_t bg_sound_group_newer( int tag ) {
    if ( !audio_initialized ) return -1;
    return Mix_GroupNewer( tag );
}

int64_t bg_sound_group_stop( int tag ) {
    if ( !audio_initialized ) return -1;
    return Mix_HaltGroup( tag );
}

int64_t bg_sound_group_fade_out( int tag, int ms ) {
    if ( !audio_initialized ) return -1;
    return Mix_FadeOutGroup( tag, ms );
}

/* Decoders and metadata */

int bg_sound_get_num_music_decoders() {
    return Mix_GetNumMusicDecoders();
}

const char * bg_sound_get_music_decoder( int index ) {
    return Mix_GetMusicDecoder( index );
}

int bg_sound_has_music_decoder( const char * name ) {
#if (SDL_MIXER_VERSION_ATLEAST(2, 6, 0))
    return Mix_HasMusicDecoder( name );
#else
    return 0;
#endif
}

int bg_sound_get_music_type( int64_t id ) {
    return ( int )Mix_GetMusicType(( Mix_Music * )( intptr_t )id );
}

const char * bg_sound_get_music_title( int64_t id ) {
#if (SDL_MIXER_VERSION_ATLEAST(2, 6, 0))
    const char * name = Mix_GetMusicTitle(( Mix_Music * )( intptr_t )id );
    return name ? name : "";
#else
    return "";
#endif
}

const char * bg_sound_get_music_title_tag( int64_t id ) {
#if (SDL_MIXER_VERSION_ATLEAST(2, 6, 0))
    const char * name = Mix_GetMusicTitleTag(( Mix_Music * )( intptr_t )id );
    return name ? name : "";
#else
    return "";
#endif
}

const char * bg_sound_get_music_artist_tag( int64_t id ) {
#if (SDL_MIXER_VERSION_ATLEAST(2, 6, 0))
    const char * name = Mix_GetMusicArtistTag(( Mix_Music * )( intptr_t )id );
    return name ? name : "";
#else
    return "";
#endif
}

const char * bg_sound_get_music_album_tag( int64_t id ) {
#if (SDL_MIXER_VERSION_ATLEAST(2, 6, 0))
    const char * name = Mix_GetMusicAlbumTag(( Mix_Music * )( intptr_t )id );
    return name ? name : "";
#else
    return "";
#endif
}

const char * bg_sound_get_music_copyright_tag( int64_t id ) {
#if (SDL_MIXER_VERSION_ATLEAST(2, 6, 0))
    const char * name = Mix_GetMusicCopyrightTag(( Mix_Music * )( intptr_t )id );
    return name ? name : "";
#else
    return "";
#endif
}
