/*
 *  Copyright (C) 2006-2016 SplinterGU (Fenix/BennuGD)
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

#ifndef __LIBMOD_SOUND_EXPORTS
#define __LIBMOD_SOUND_EXPORTS

/* --------------------------------------------------------------------------- */

#include "bgddl.h"

/* --------------------------------------------------------------------------- */

#if defined(__BGDC__) || !defined(__STATIC__)

/* ----------------------------------------------------------------- */
/* Definicion de constantes (usada en tiempo de compilacion)                   */

DLCONSTANT  __bgdexport( libmod_sound, constants_def )[] = {
    { "MODE_MONO"   , TYPE_INT64, 0  },
    { "MODE_STEREO" , TYPE_INT64, 1  },
    { "ALL_SOUND"   , TYPE_INT64, -1 },
    { NULL          , 0         , 0  }
} ;

/* --------------------------------------------------------------------------- */
/* Definicion de variables globales (usada en tiempo de compilacion)           */

char * __bgdexport( libmod_sound, globals_def ) =
    "sound_freq = 22050;\n"
    "sound_mode = MODE_STEREO;\n"
    "sound_channels = 8;\n";

#endif

/* --------------------------------------------------------------------------- */

DLSYSFUNCS  __bgdexport( libmod_sound, functions_exports )[] = {
    FUNC( "SOUND_INIT"              , ""     , TYPE_INT64 , libmod_sound_init                 ),
    FUNC( "SOUND_CLOSE"             , ""     , TYPE_INT64 , libmod_sound_close                ),

    FUNC( "SONG_LOAD"               , "S"    , TYPE_INT64 , libmod_sound_load_song            ),
    FUNC( "SONG_LOAD"               , "SP"   , TYPE_INT64 , libmod_sound_bgload_song          ),
    FUNC( "SONG_UNLOAD"             , "Q"    , TYPE_INT64 , libmod_sound_unload_song          ),
    FUNC( "SONG_UNLOAD"             , "P"    , TYPE_INT64 , libmod_sound_unload_song2         ),

    FUNC( "SONG_PLAY"               , "QQ"   , TYPE_INT64 , libmod_sound_play_song            ),
    FUNC( "SONG_STOP"               , ""     , TYPE_INT64 , libmod_sound_stop_song            ),
    FUNC( "SONG_PAUSE"              , ""     , TYPE_INT64 , libmod_sound_pause_song           ),
    FUNC( "SONG_RESUME"             , ""     , TYPE_INT64 , libmod_sound_resume_song          ),

    FUNC( "SONG_SET_VOLUME"         , "Q"    , TYPE_INT64 , libmod_sound_set_song_volume      ),
    FUNC( "SONG_IS_PLAYING"         , ""     , TYPE_INT64 , libmod_sound_is_playing_song      ),

    FUNC( "WAV_LOAD"                , "S"    , TYPE_INT64 , libmod_sound_load_wav             ),
    FUNC( "WAV_LOAD"                , "SP"   , TYPE_INT64 , libmod_sound_bgload_wav           ),
    FUNC( "WAV_UNLOAD"              , "Q"    , TYPE_INT64 , libmod_sound_unload_wav           ),
    FUNC( "WAV_UNLOAD"              , "P"    , TYPE_INT64 , libmod_sound_unload_wav2          ),
    FUNC( "WAV_PLAY"                , "QQ"   , TYPE_INT64 , libmod_sound_play_wav             ),
    FUNC( "WAV_PLAY"                , "QQQ"  , TYPE_INT64 , libmod_sound_play_wav_channel     ),
    FUNC( "WAV_STOP"                , "Q"    , TYPE_INT64 , libmod_sound_stop_wav             ),
    FUNC( "WAV_PAUSE"               , "Q"    , TYPE_INT64 , libmod_sound_pause_wav            ),
    FUNC( "WAV_RESUME"              , "Q"    , TYPE_INT64 , libmod_sound_resume_wav           ),
    FUNC( "WAV_IS_PLAYING"          , "Q"    , TYPE_INT64 , libmod_sound_is_playing_wav       ),

    FUNC( "MUSIC_FADE_IN"           , "QQQ"  , TYPE_INT64 , libmod_sound_fade_music_in        ),
    FUNC( "MUSIC_FADE_OFF"          , "Q"    , TYPE_INT64 , libmod_sound_fade_music_off       ),

    FUNC( "MUSIC_SET_POSITION"      , "D"    , TYPE_INT64 , libmod_sound_set_music_position   ),

    FUNC( "VOLUME_SET"              , "QQ"   , TYPE_INT64 , libmod_sound_set_wav_volume       ),
    FUNC( "VOLUME_SET_CHANNEL"      , "QQ"   , TYPE_INT64 , libmod_sound_set_channel_volume   ),

    FUNC( "SET_PANNING"             , "QQQ"  , TYPE_INT64 , libmod_sound_set_panning          ),
    FUNC( "SET_POSITION"            , "QQQ"  , TYPE_INT64 , libmod_sound_set_position         ),
    FUNC( "SET_DISTANCE"            , "QQ"   , TYPE_INT64 , libmod_sound_set_distance         ),

    FUNC( "RESERVE_CHANNELS"        , "Q"    , TYPE_INT64 , libmod_sound_reserve_channels     ),
    FUNC( "REVERSE_STEREO"          , "QQ"   , TYPE_INT64 , libmod_sound_reverse_stereo       ),

    FUNC( 0                         , 0      , 0          , 0                                 )
};

/* --------------------------------------------------------------------------- */

#endif
