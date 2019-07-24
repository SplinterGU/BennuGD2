/*
 *  Copyright (C) 2006-2019 SplinterGU (Fenix/BennuGD)
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
    { "MODE_MONO"       , TYPE_INT, 0               },
    { "MODE_STEREO"     , TYPE_INT, 1               },
    { "ALL_SOUND"       , TYPE_INT, -1              },

    { "MIX_NO_FADING"   , TYPE_INT, MIX_NO_FADING   },
    { "MIX_FADING_OUT"  , TYPE_INT, MIX_FADING_OUT  },
    { "MIX_FADING_IN"   , TYPE_INT, MIX_FADING_IN   },

    { NULL              , 0       , 0               }
} ;

/* --------------------------------------------------------------------------- */
/* Definicion de variables globales (usada en tiempo de compilacion)           */

char * __bgdexport( libmod_sound, globals_def ) =
    "STRUCT sound\n"
    "   INT sound_freq=22050;\n"
    "   INT sound_mode=MODE_STEREO;\n"
    "   INT sound_channels=8;\n"
    "END\n"
    ;

#endif

/* --------------------------------------------------------------------------- */

DLSYSFUNCS  __bgdexport( libmod_sound, functions_exports )[] = {
    FUNC( "SOUNDSYS_INIT"           , ""     , TYPE_INT , libmod_sound_init                 ),
    FUNC( "SOUNDSYS_QUIT"           , ""     , TYPE_INT , libmod_sound_quit                 ),

    FUNC( "MUSIC_LOAD"              , "S"    , TYPE_INT , libmod_sound_load_music           ),
    FUNC( "MUSIC_LOAD"              , "SP"   , TYPE_INT , libmod_sound_bgload_music         ),
    FUNC( "MUSIC_UNLOAD"            , "I"    , TYPE_INT , libmod_sound_unload_music         ),
    FUNC( "MUSIC_UNLOAD"            , "P"    , TYPE_INT , libmod_sound_unload_music2        ),

    FUNC( "MUSIC_PLAY"              , "II"   , TYPE_INT , libmod_sound_play_music           ),
    FUNC( "MUSIC_STOP"              , ""     , TYPE_INT , libmod_sound_stop_music           ),
    FUNC( "MUSIC_PAUSE"             , ""     , TYPE_INT , libmod_sound_pause_music          ),
    FUNC( "MUSIC_RESUME"            , ""     , TYPE_INT , libmod_sound_resume_music         ),
    FUNC( "MUSIC_REWIND"            , ""     , TYPE_INT , libmod_sound_music_rewind         ),

    FUNC( "MUSIC_FADE_IN"           , "III"  , TYPE_INT , libmod_sound_fade_music_in        ),
    FUNC( "MUSIC_FADE_IN"           , "IIID" , TYPE_INT , libmod_sound_fade_music_in_pos    ),
    FUNC( "MUSIC_FADE_OFF"          , "I"    , TYPE_INT , libmod_sound_fade_music_off       ),

    FUNC( "MUSIC_IS_PLAYING"        , ""     , TYPE_INT , libmod_sound_is_playing_music     ),
    FUNC( "MUSIC_IS_PAUSED"         , ""     , TYPE_INT , libmod_sound_is_paused_music      ),
    FUNC( "MUSIC_IS_FADING"         , ""     , TYPE_INT , libmod_sound_is_fading_music      ),

    FUNC( "MUSIC_SET_VOLUME"        , "I"    , TYPE_INT , libmod_sound_set_music_volume     ),
    FUNC( "MUSIC_SET_POSITION"      , "D"    , TYPE_INT , libmod_sound_set_music_position   ),

    FUNC( "SOUND_LOAD"              , "S"    , TYPE_INT , libmod_sound_load_sound           ),
    FUNC( "SOUND_LOAD"              , "SP"   , TYPE_INT , libmod_sound_bgload_sound         ),
    FUNC( "SOUND_UNLOAD"            , "I"    , TYPE_INT , libmod_sound_unload_sound         ),
    FUNC( "SOUND_UNLOAD"            , "P"    , TYPE_INT , libmod_sound_unload_sound2        ),
    FUNC( "SOUND_PLAY"              , "II"   , TYPE_INT , libmod_sound_play_sound           ),
    FUNC( "SOUND_PLAY"              , "III"  , TYPE_INT , libmod_sound_play_sound_channel   ),
    FUNC( "SOUND_PLAY"              , "IIII" , TYPE_INT , libmod_sound_play_sound_timed     ),
    FUNC( "SOUND_STOP"              , "I"    , TYPE_INT , libmod_sound_stop_sound           ),
    FUNC( "SOUND_PAUSE"             , "I"    , TYPE_INT , libmod_sound_pause_sound          ),
    FUNC( "SOUND_RESUME"            , "I"    , TYPE_INT , libmod_sound_resume_sound         ),

    FUNC( "SOUND_FADE_IN"           , "IIII" , TYPE_INT , libmod_sound_fade_sound_in        ),
    FUNC( "SOUND_FADE_IN"           , "IIIII", TYPE_INT , libmod_sound_fade_sound_in_timed  ),
    FUNC( "SOUND_FADE_OFF"          , "II"   , TYPE_INT , libmod_sound_fade_channel_off     ),

    FUNC( "SOUND_IS_PLAYING"        , "I"    , TYPE_INT , libmod_sound_is_playing_sound     ),
    FUNC( "SOUND_IS_PAUSED"         , "I"    , TYPE_INT , libmod_sound_is_paused_sound      ),
    FUNC( "SOUND_IS_FADING"         , "I"    , TYPE_INT , libmod_sound_is_fading_channel    ),

    FUNC( "SOUND_SET_VOLUME"        , "II"   , TYPE_INT , libmod_sound_set_sound_volume     ),
    FUNC( "CHANNEL_SET_VOLUME"      , "II"   , TYPE_INT , libmod_sound_set_channel_volume   ),

    FUNC( "SET_PANNING"             , "III"  , TYPE_INT , libmod_sound_set_panning          ),
    FUNC( "SET_POSITION"            , "III"  , TYPE_INT , libmod_sound_set_position         ),
    FUNC( "SET_DISTANCE"            , "II"   , TYPE_INT , libmod_sound_set_distance         ),

    FUNC( "RESERVE_CHANNELS"        , "I"    , TYPE_INT , libmod_sound_reserve_channels     ),
    FUNC( "REVERSE_STEREO"          , "II"   , TYPE_INT , libmod_sound_reverse_stereo       ),

    FUNC( 0                         , 0      , 0        , 0                                 )
};

/* --------------------------------------------------------------------------- */

#endif
