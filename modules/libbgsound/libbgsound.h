/*
 *  Copyright (C) SplinterGU (Fenix/BennuGD) (Since 2006)
 *
 *  This file is part of Bennu Game Development
 *
 *  This software is provided 'as-is', without any express or implied
 *  warranty. In no event will the authors be held liable for any damages
 *  arising from the use of this software.
 */

#ifndef __LIBBGSOUND_H
#define __LIBBGSOUND_H

#include <stdint.h>
#ifndef __BGDC__
#include <SDL.h>
#include <SDL_mixer.h>
#endif
#include "files.h" // For file* support

#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------------------------------------------------- */

enum {
    BG_SOUND_MODE_MONO           = 1,
    BG_SOUND_MODE_STEREO         = 2,
    BG_SOUND_MODE_STEREO_2_1     = 3,
    BG_SOUND_MODE_SURROUND_4_1   = 5,
    BG_SOUND_MODE_SURROUND_5_1   = 6,
    BG_SOUND_MODE_SURROUND_7_1   = 8
};

/* --------------------------------------------------------------------------- */

extern int bg_sound_init( int freq, int mode, int channels );
extern void bg_sound_quit();
extern int bg_sound_is_initialized();

extern void bg_sound_system_init();
extern void bg_sound_system_finalize();

/* Music functions */
extern int64_t bg_sound_load_music( const char * filename );
extern int bg_sound_play_music( int64_t id, int loops );
extern int bg_sound_fade_music_in( int64_t id, int loops, int ms );
extern int bg_sound_fade_music_in_pos( int64_t id, int loops, int ms, double position );
extern int bg_sound_fade_music_off( int ms );
extern int bg_sound_is_fading_music();
extern int bg_sound_unload_music( int64_t id );
extern int bg_sound_stop_music( void );
extern int bg_sound_pause_music( void );
extern int bg_sound_is_paused_music( void );
extern int bg_sound_resume_music( void );
extern int bg_sound_is_playing_music( void );
extern int bg_sound_set_music_volume( int volume );
extern int bg_sound_get_music_volume( int64_t id );
extern int bg_sound_music_rewind();
extern int bg_sound_set_music_playback_position( double position );
extern double bg_sound_get_music_playback_position( int64_t id );
extern double bg_sound_get_music_duration( int64_t id );
extern double bg_sound_get_music_loop_start_time( int64_t id );
extern double bg_sound_get_music_loop_end_time( int64_t id );
extern double bg_sound_get_music_loop_length_time( int64_t id );

/* Sound (Chunk) functions */
extern int64_t bg_sound_load_sound( const char * filename );
extern int bg_sound_play_sound( int64_t id, int loops, int channel );
extern int bg_sound_play_sound_timed( int64_t id, int loops, int channel, int ticks );
extern int bg_sound_fade_sound_in( int64_t id, int loops, int ms, int channel );
extern int bg_sound_fade_sound_in_timed( int64_t id, int loops, int ms, int channel, int ticks );
extern int bg_sound_is_fading_channel( int channel );
extern int bg_sound_unload_sound( int64_t id );
extern int bg_sound_stop_sound( int channel );
extern int bg_sound_pause_sound( int channel );
extern int bg_sound_is_paused_sound( int channel );
extern int bg_sound_resume_sound( int channel );
extern int bg_sound_is_playing_sound( int channel );
extern int bg_sound_fade_channel_off( int channel, int ms );
extern int bg_sound_set_sound_volume( int64_t sample, int volume );
extern int bg_sound_set_channel_volume( int channel, int volume );

/* Advanced functions */
extern int bg_sound_reserve_channels( int channels );
extern int bg_sound_set_panning( int channel, int left, int right );
extern int bg_sound_set_position( int channel, int angle, int dist );
extern int bg_sound_set_distance( int channel, int dist );
extern int bg_sound_reverse_stereo( int channel, int flip );
extern int bg_sound_set_master_volume( int volume );
extern int64_t bg_sound_expire_channel( int64_t channel, int64_t ticks );

/* Group functions */
extern int64_t bg_sound_group_channel( int64_t channel, int tag );
extern int64_t bg_sound_group_channels( int64_t from, int64_t to, int tag );
extern int64_t bg_sound_group_available( int tag );
extern int64_t bg_sound_group_count( int tag );
extern int64_t bg_sound_group_oldest( int tag );
extern int64_t bg_sound_group_newer( int tag );
extern int64_t bg_sound_group_stop( int tag );
extern int64_t bg_sound_group_fade_out( int tag, int ms );

/* Decoders and metadata */
extern int bg_sound_get_num_music_decoders();
extern const char * bg_sound_get_music_decoder( int index );
extern int bg_sound_has_music_decoder( const char * name );
extern int bg_sound_get_music_type( int64_t id );
extern const char * bg_sound_get_music_title( int64_t id );
extern const char * bg_sound_get_music_title_tag( int64_t id );
extern const char * bg_sound_get_music_artist_tag( int64_t id );
extern const char * bg_sound_get_music_album_tag( int64_t id );
extern const char * bg_sound_get_music_copyright_tag( int64_t id );

/* Internal bridge */
extern SDL_RWops * SDL_RWFromBGDFP( file * fp );

#ifdef __cplusplus
}
#endif

#endif
