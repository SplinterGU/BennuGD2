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

#ifndef __LIBMOD_SOUND_EXPORTS
#define __LIBMOD_SOUND_EXPORTS

/* --------------------------------------------------------------------------- */

#include "bgddl.h"
#include "libmod_sound.h"

/* --------------------------------------------------------------------------- */

#if defined(__BGDC__) || !defined(__STATIC__)

/* ----------------------------------------------------------------- */
/* Constants definition (used at compile time)                                 */

DLCONSTANT  __bgdexport( libmod_sound, constants_def )[] = {
    { "MODE_MONO"                   , TYPE_INT, SOUND_MODE_MONO             },
    { "MODE_STEREO"                 , TYPE_INT, SOUND_MODE_STEREO           },
    { "MODE_STEREO_2_1"             , TYPE_INT, SOUND_MODE_STEREO_2_1       },
    { "MODE_SURROUND_4_1"           , TYPE_INT, SOUND_MODE_SURROUND_4_1     },
    { "MODE_SURROUND_5_1"           , TYPE_INT, SOUND_MODE_SURROUND_5_1     },
    { "MODE_SURROUND_7_1"           , TYPE_INT, SOUND_MODE_SURROUND_7_1     },

    { "SOUND_MODE_MONO"             , TYPE_INT, SOUND_MODE_MONO             },
    { "SOUND_MODE_STEREO"           , TYPE_INT, SOUND_MODE_STEREO           },
    { "SOUND_MODE_STEREO_2_1"       , TYPE_INT, SOUND_MODE_STEREO_2_1       },
    { "SOUND_MODE_SURROUND_4_1"     , TYPE_INT, SOUND_MODE_SURROUND_4_1     },
    { "SOUND_MODE_SURROUND_5_1"     , TYPE_INT, SOUND_MODE_SURROUND_5_1     },
    { "SOUND_MODE_SURROUND_7_1"     , TYPE_INT, SOUND_MODE_SURROUND_7_1     },

    { "MUSIC_TYPE_NONE"             , TYPE_INT, MUS_NONE                    },
    { "MUSIC_TYPE_CMD"              , TYPE_INT, MUS_CMD                     },
    { "MUSIC_TYPE_WAV"              , TYPE_INT, MUS_WAV                     },
    { "MUSIC_TYPE_MOD"              , TYPE_INT, MUS_MOD                     },
    { "MUSIC_TYPE_MID"              , TYPE_INT, MUS_MID                     },
    { "MUSIC_TYPE_OGG"              , TYPE_INT, MUS_OGG                     },
    { "MUSIC_TYPE_MP3"              , TYPE_INT, MUS_MP3                     },
    { "MUSIC_TYPE_MP3_MAD_UNUSED"   , TYPE_INT, MUS_MP3_MAD_UNUSED          },
    { "MUSIC_TYPE_FLAC"             , TYPE_INT, MUS_FLAC                    },
    { "MUSIC_TYPE_MODPLUG_UNUSED"   , TYPE_INT, MUS_MODPLUG_UNUSED          },
    { "MUSIC_TYPE_OPU"              , TYPE_INT, MUS_OPUS                    },

    { "ALL_SOUND"                   , TYPE_INT, -1                          },

    { "MIX_NO_FADING"               , TYPE_INT, MIX_NO_FADING               },
    { "MIX_FADING_OUT"              , TYPE_INT, MIX_FADING_OUT              },
    { "MIX_FADING_IN"               , TYPE_INT, MIX_FADING_IN               },

    { NULL                          , 0       , 0                           }
} ;

/* --------------------------------------------------------------------------- */
/* Global variables definition (used at compile time)                          */

char * __bgdexport( libmod_sound, globals_def ) =
    "STRUCT sound\n"
    "   INT freq=" TOSTRING(DEFAULT_SOUND_FREQ) ";\n"
    "   INT mode=" TOSTRING(DEFAULT_SOUND_MODE) ";\n"
    "   INT channels=" TOSTRING(DEFAULT_SOUND_CHANNELS) ";\n"
    "END\n"
    ;
#endif

/* --------------------------------------------------------------------------- */

DLSYSFUNCS  __bgdexport( libmod_sound, functions_exports )[] = {
    FUNC( "SOUNDSYS_INIT"               , ""            , TYPE_INT      , libmod_sound_init                                 ),
    FUNC( "SOUNDSYS_QUIT"               , ""            , TYPE_INT      , libmod_sound_quit                                 ),
    FUNC( "MUSIC_LOAD"                  , "S"           , TYPE_INT      , libmod_sound_load_music                           ),
    FUNC( "MUSIC_LOAD"                  , "SP"          , TYPE_INT      , libmod_sound_bgload_music                         ),
    FUNC( "MUSIC_UNLOAD"                , "I"           , TYPE_INT      , libmod_sound_unload_music                         ),
    FUNC( "MUSIC_UNLOAD"                , "P"           , TYPE_INT      , libmod_sound_unload_music_and_free                ),
    FUNC( "MUSIC_PLAY"                  , "II"          , TYPE_INT      , libmod_sound_play_music                           ),
    FUNC( "MUSIC_PLAY"                  , "I"           , TYPE_INT      , libmod_sound_play_music_looped                    ),
    FUNC( "MUSIC_STOP"                  , ""            , TYPE_INT      , libmod_sound_stop_music                           ),
    FUNC( "MUSIC_PAUSE"                 , ""            , TYPE_INT      , libmod_sound_pause_music                          ),
    FUNC( "MUSIC_RESUME"                , ""            , TYPE_INT      , libmod_sound_resume_music                         ),
    FUNC( "MUSIC_REWIND"                , ""            , TYPE_INT      , libmod_sound_music_rewind                         ),
    FUNC( "MUSIC_FADE_IN"               , "III"         , TYPE_INT      , libmod_sound_fade_music_in                        ),
    FUNC( "MUSIC_FADE_IN"               , "IIID"        , TYPE_INT      , libmod_sound_fade_music_in_pos                    ),
    FUNC( "MUSIC_FADE_OFF"              , "I"           , TYPE_INT      , libmod_sound_fade_music_off                       ),
    FUNC( "MUSIC_IS_PLAYING"            , ""            , TYPE_INT      , libmod_sound_is_playing_music                     ),
    FUNC( "MUSIC_IS_PAUSED"             , ""            , TYPE_INT      , libmod_sound_is_paused_music                      ),
    FUNC( "MUSIC_IS_FADING"             , ""            , TYPE_INT      , libmod_sound_is_fading_music                      ),
    FUNC( "MUSIC_SET_VOLUME"            , "I"           , TYPE_INT      , libmod_sound_set_music_volume                     ),
    FUNC( "MUSIC_GET_VOLUME"            , "I"           , TYPE_INT      , libmod_sound_get_music_volume                     ),
    FUNC( "MUSIC_SET_PLAYBACK_POSITION" , "D"           , TYPE_INT      , libmod_sound_set_music_playback_position          ),
    FUNC( "MUSIC_SET_TIME_POSITION"     , "D"           , TYPE_INT      , libmod_sound_set_music_playback_position          ),
    FUNC( "MUSIC_GET_PLAYBACK_POSITION" , "I"           , TYPE_DOUBLE   , libmod_sound_get_music_playback_position          ),
    FUNC( "MUSIC_GET_TIME_POSITION"     , "I"           , TYPE_DOUBLE   , libmod_sound_get_music_playback_position          ),
    FUNC( "MUSIC_GET_DURATION"          , "I"           , TYPE_DOUBLE   , libmod_sound_get_music_duration                   ),
    FUNC( "MUSIC_GET_DURATION"          , ""            , TYPE_DOUBLE   , libmod_sound_get_music_duration_current           ),
    FUNC( "MUSIC_GET_LOOP_START"        , "I"           , TYPE_DOUBLE   , libmod_sound_get_music_loop_start_time            ),
    FUNC( "MUSIC_GET_LOOP_START"        , ""            , TYPE_DOUBLE   , libmod_sound_get_music_loop_start_time_current    ),
    FUNC( "MUSIC_GET_LOOP_END"          , "I"           , TYPE_DOUBLE   , libmod_sound_get_music_loop_end_time              ),
    FUNC( "MUSIC_GET_LOOP_END"          , ""            , TYPE_DOUBLE   , libmod_sound_get_music_loop_end_time_current      ),
    FUNC( "MUSIC_GET_LOOP_LEN"          , "I"           , TYPE_DOUBLE   , libmod_sound_get_music_loop_length_time           ),
    FUNC( "MUSIC_GET_LOOP_LEN"          , ""            , TYPE_DOUBLE   , libmod_sound_get_music_loop_length_time_current   ),
    FUNC( "SOUND_LOAD"                  , "S"           , TYPE_INT      , libmod_sound_load_sound                           ),
    FUNC( "SOUND_LOAD"                  , "SP"          , TYPE_INT      , libmod_sound_bgload_sound                         ),
    FUNC( "SOUND_UNLOAD"                , "I"           , TYPE_INT      , libmod_sound_unload_sound                         ),
    FUNC( "SOUND_UNLOAD"                , "P"           , TYPE_INT      , libmod_sound_unload_sound_and_free                ),
    FUNC( "SOUND_PLAY"                  , "II"          , TYPE_INT      , libmod_sound_play_sound                           ),
    FUNC( "SOUND_PLAY"                  , "I"           , TYPE_INT      , libmod_sound_play_sound_once                      ),
    FUNC( "SOUND_PLAY"                  , "III"         , TYPE_INT      , libmod_sound_play_sound_channel                   ),
    FUNC( "SOUND_PLAY"                  , "IIII"        , TYPE_INT      , libmod_sound_play_sound_timed                     ),
    FUNC( "SOUND_STOP"                  , "I"           , TYPE_INT      , libmod_sound_stop_sound                           ),
    FUNC( "SOUND_PAUSE"                 , "I"           , TYPE_INT      , libmod_sound_pause_sound                          ),
    FUNC( "SOUND_RESUME"                , "I"           , TYPE_INT      , libmod_sound_resume_sound                         ),
    FUNC( "SOUND_FADE_IN"               , "IIII"        , TYPE_INT      , libmod_sound_fade_sound_in                        ),
    FUNC( "SOUND_FADE_IN"               , "IIIII"       , TYPE_INT      , libmod_sound_fade_sound_in_timed                  ),
    FUNC( "SOUND_FADE_OFF"              , "II"          , TYPE_INT      , libmod_sound_fade_channel_off                     ),
    FUNC( "SOUND_IS_PLAYING"            , "I"           , TYPE_INT      , libmod_sound_is_playing_sound                     ),
    FUNC( "SOUND_IS_PAUSED"             , "I"           , TYPE_INT      , libmod_sound_is_paused_sound                      ),
    FUNC( "SOUND_IS_FADING"             , "I"           , TYPE_INT      , libmod_sound_is_fading_channel                    ),
    FUNC( "SOUND_SET_VOLUME"            , "II"          , TYPE_INT      , libmod_sound_set_sound_volume                     ),
    FUNC( "SOUND_SET_VOLUME"            , "I"           , TYPE_INT      , libmod_sound_set_sound_volume_all                 ),
    FUNC( "SOUND_SET_LOCATION"          , "III"         , TYPE_INT      , libmod_sound_set_position                         ),
    FUNC( "SOUND_SET_SPATIAL_POSITION"  , "III"         , TYPE_INT      , libmod_sound_set_position                         ),
    FUNC( "SET_POSITION"                , "III"         , TYPE_INT      , libmod_sound_set_position                         ),
    FUNC( "CHANNEL_SET_EXPIRE"          , "II"          , TYPE_INT      , libmod_sound_expire_channel                       ),
    FUNC( "CHANNEL_SET_EXPIRE"          , "I"           , TYPE_INT      , libmod_sound_expire_channel_all                   ),
    FUNC( "CHANNEL_SET_VOLUME"          , "II"          , TYPE_INT      , libmod_sound_set_channel_volume                   ),
    FUNC( "CHANNEL_SET_VOLUME"          , "I"           , TYPE_INT      , libmod_sound_set_channel_volume_all               ),
    FUNC( "SET_MASTER_VOLUME"           , "I"           , TYPE_INT      , libmod_sound_set_master_volume                    ),
    FUNC( "CHANNEL_SET_PANNING"         , "III"         , TYPE_INT      , libmod_sound_set_panning                          ),
    FUNC( "SET_PANNING"                 , "III"         , TYPE_INT      , libmod_sound_set_panning                          ),
    FUNC( "CHANNEL_SET_DISTANCE"        , "II"          , TYPE_INT      , libmod_sound_set_distance                         ),
    FUNC( "SET_DISTANCE"                , "II"          , TYPE_INT      , libmod_sound_set_distance                         ),
    FUNC( "RESERVE_CHANNELS"            , "I"           , TYPE_INT      , libmod_sound_reserve_channels                     ),
    FUNC( "REVERSE_STEREO"              , "II"          , TYPE_INT      , libmod_sound_reverse_stereo                       ),
    FUNC( "GET_NUM_MUSIC_DECODERS"      , ""            , TYPE_INT      , libmod_sound_get_num_music_decoders               ),
    FUNC( "GET_MUSIC_DECODER"           , "I"           , TYPE_STRING   , libmod_sound_get_music_decoder                    ),
    FUNC( "HAS_MUSIC_DECODER"           , "S"           , TYPE_INT      , libmod_sound_has_music_decoder                    ),
    FUNC( "MUSIC_GET_TYPE"              , "I"           , TYPE_INT      , libmod_sound_get_music_type                       ),
    FUNC( "MUSIC_GET_TITLE"             , "I"           , TYPE_STRING   , libmod_sound_get_music_title                      ),
    FUNC( "MUSIC_GET_TITLE_TAG"         , "I"           , TYPE_STRING   , libmod_sound_get_music_title_tag                  ),
    FUNC( "MUSIC_GET_ARTIST_TAG"        , "I"           , TYPE_STRING   , libmod_sound_get_music_artist_tag                 ),
    FUNC( "MUSIC_GET_ALBUM_TAG"         , "I"           , TYPE_STRING   , libmod_sound_get_music_album_tag                  ),
    FUNC( "MUSIC_GET_COPYRIGHT_TAG"     , "I"           , TYPE_STRING   , libmod_sound_get_music_copyright_tag              ),
    FUNC( "MUSIC_GET_TYPE"              , ""            , TYPE_INT      , libmod_sound_get_music_type_current               ),
    FUNC( "MUSIC_GET_TITLE"             , ""            , TYPE_STRING   , libmod_sound_get_music_title_current              ),
    FUNC( "MUSIC_GET_TITLE_TAG"         , ""            , TYPE_STRING   , libmod_sound_get_music_title_tag_current          ),
    FUNC( "MUSIC_GET_ARTIST_TAG"        , ""            , TYPE_STRING   , libmod_sound_get_music_artist_tag_current         ),
    FUNC( "MUSIC_GET_ALBUM_TAG"         , ""            , TYPE_STRING   , libmod_sound_get_music_album_tag_current          ),
    FUNC( "MUSIC_GET_COPYRIGHT_TAG"     , ""            , TYPE_STRING   , libmod_sound_get_music_copyright_tag_current      ),
    FUNC( "SOUND_GROUP_CHANNEL"         , "II"          , TYPE_INT      , libmod_sound_group_channel                        ),
    FUNC( "SOUND_GROUP_CHANNELS"        , "III"         , TYPE_INT      , libmod_sound_group_channels                       ),
    FUNC( "SOUND_GROUP_AVAILABLE"       , "I"           , TYPE_INT      , libmod_sound_group_available                      ),
    FUNC( "SOUND_GROUP_COUNT"           , "I"           , TYPE_INT      , libmod_sound_group_count                          ),
    FUNC( "SOUND_GROUP_OLDEST"          , "I"           , TYPE_INT      , libmod_sound_group_oldest                         ),
    FUNC( "SOUND_GROUP_NEWER"           , "I"           , TYPE_INT      , libmod_sound_group_newer                          ),
    FUNC( "SOUND_GROUP_HALT"            , "I"           , TYPE_INT      , libmod_sound_halt_group                           ),
    FUNC( "SOUND_GROUP_FADE_OUT"        , "II"          , TYPE_INT      , libmod_sound_fade_out_group                       ),

    FUNC( 0                             , 0             , 0             , 0                                                 )
};

/* --------------------------------------------------------------------------- */

#endif
