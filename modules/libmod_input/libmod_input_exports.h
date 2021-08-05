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

#ifndef __LIBMOD_INPUT_EXPORTS
#define __LIBMOD_INPUT_EXPORTS

/* ----------------------------------------------------------------- */

#include "bgddl.h"

/* ----------------------------------------------------------------- */

DLSYSFUNCS  __bgdexport( libmod_input, functions_exports )[] = {

    FUNC( "KEY"                 , "I"       , TYPE_INT    , libmod_input_key                          ),
    FUNC( "KEY_UP"              , "I"       , TYPE_INT    , libmod_input_key_up                       ),
    FUNC( "KEY_DOWN"            , "I"       , TYPE_INT    , libmod_input_key_down                     ),

    FUNC( "JOY_AXES"            , ""        , TYPE_INT    , libmod_input_joy_axes                     ),
    FUNC( "JOY_AXES"            , "I"       , TYPE_INT    , libmod_input_joy_axes_specific            ),
    FUNC( "JOY_NUMAXES"         , ""        , TYPE_INT    , libmod_input_joy_axes                     ),
    FUNC( "JOY_NUMAXES"         , "I"       , TYPE_INT    , libmod_input_joy_axes_specific            ),
    FUNC( "JOY_GETAXIS"         , "I"       , TYPE_INT    , libmod_input_joy_get_position             ),
    FUNC( "JOY_GETAXIS"         , "II"      , TYPE_INT    , libmod_input_joy_get_position_specific    ),

    FUNC( "JOY_BUTTONS"         , ""        , TYPE_INT    , libmod_input_joy_buttons                  ),
    FUNC( "JOY_BUTTONS"         , "I"       , TYPE_INT    , libmod_input_joy_buttons_specific         ),

    FUNC( "JOY_NAME"            , "I"       , TYPE_STRING , libmod_input_joy_name                     ),

    FUNC( "JOY_NUMBUTTONS"      , ""        , TYPE_INT    , libmod_input_joy_buttons                  ),
    FUNC( "JOY_NUMBUTTONS"      , "I"       , TYPE_INT    , libmod_input_joy_buttons_specific         ),

    FUNC( "JOY_NUMBER"          , ""        , TYPE_INT    , libmod_input_joy_num                      ),
    FUNC( "JOY_NUMJOYSTICKS"    , ""        , TYPE_INT    , libmod_input_joy_num                      ),

    FUNC( "JOY_SELECT"          , "I"       , TYPE_INT    , libmod_input_joy_select                   ),

    FUNC( "JOY_GETBUTTON"       , "I"       , TYPE_INT    , libmod_input_joy_get_button               ),
    FUNC( "JOY_GETBUTTON"       , "II"      , TYPE_INT    , libmod_input_joy_get_button_specific      ),
    FUNC( "JOY_GETPOSITION"     , "I"       , TYPE_INT    , libmod_input_joy_get_position             ),
    FUNC( "JOY_GETPOSITION"     , "II"      , TYPE_INT    , libmod_input_joy_get_position_specific    ),

    FUNC( "JOY_NUMHATS"         , ""        , TYPE_INT    , libmod_input_joy_hats                     ),
    FUNC( "JOY_NUMHATS"         , "I"       , TYPE_INT    , libmod_input_joy_hats_specific            ),   /* Added by Sandman */
    FUNC( "JOY_NUMBALLS"        , ""        , TYPE_INT    , libmod_input_joy_balls                    ),
    FUNC( "JOY_NUMBALLS"        , "I"       , TYPE_INT    , libmod_input_joy_balls_specific           ),   /* Added by Sandman */
    FUNC( "JOY_GETHAT"          , "I"       , TYPE_INT    , libmod_input_joy_get_hat                  ),
    FUNC( "JOY_GETHAT"          , "II"      , TYPE_INT    , libmod_input_joy_get_hat_specific         ),   /* Added by Sandman */
    FUNC( "JOY_GETBALL"         , "IPP"     , TYPE_INT    , libmod_input_joy_get_ball                 ),
    FUNC( "JOY_GETBALL"         , "IIPP"    , TYPE_INT    , libmod_input_joy_get_ball_specific        ),   /* Added by Sandman */

    FUNC( "JOY_GETACCEL"        , "PPP"     , TYPE_INT    , libmod_input_joy_get_accel                ),
    FUNC( "JOY_GETACCEL"        , "IPPP"    , TYPE_INT    , libmod_input_joy_get_accel_specific       ),

    /* Compatibility */

    FUNC( "NUMBER_JOY"          , ""        , TYPE_INT    , libmod_input_joy_num                      ),
    FUNC( "SELECT_JOY"          , "I"       , TYPE_INT    , libmod_input_joy_select                   ),
    FUNC( "GET_JOY_BUTTON"      , "I"       , TYPE_INT    , libmod_input_joy_get_button               ),
    FUNC( "GET_JOY_BUTTON"      , "II"      , TYPE_INT    , libmod_input_joy_get_button_specific      ),
    FUNC( "GET_JOY_POSITION"    , "I"       , TYPE_INT    , libmod_input_joy_get_position             ),
    FUNC( "GET_JOY_POSITION"    , "II"      , TYPE_INT    , libmod_input_joy_get_position_specific    ),

    FUNC( 0                     , 0         , 0           , 0                                         )
};

/* ----------------------------------------------------------------- */

char * __bgdexport( libmod_input, modules_dependency )[] = {
    "libbginput",
    NULL
};

/* ----------------------------------------------------------------- */

#endif
