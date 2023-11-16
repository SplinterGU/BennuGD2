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
#include <math.h>
#include <time.h>

#include "bgddl.h"
#include "bgdrtm.h"
#include "xstrings.h"
#include "xctype.h"

#include "libbginput.h"

/* --------------------------------------------------------------------------- */

static int64_t libmod_input_key( INSTANCE * my, int64_t * params ) {
    return ( get_key( params[0] ) );
}

/* --------------------------------------------------------------------------- */

static int64_t libmod_input_key_up( INSTANCE * my, int64_t * params ) {
    return ( get_key_up( params[0] ) );
}

/* --------------------------------------------------------------------------- */

static int64_t libmod_input_key_down( INSTANCE * my, int64_t * params ) {
    return ( get_key_down( params[0] ) );
}

/* --------------------------------------------------------------------------- */
/* Thanks Sandman for suggest on openjoys at initialization time               */
/* --------------------------------------------------------------------------- */
/* Credits SplinterGU/Sandman 2007-2009                                        */
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/**
 * JOY_NUMBER()
 * JOY_NUMJOYSTICKS()
 * NUMBER_JOY()
 *
 * Returns the number of joysticks present in the system
 *
 **/

static int64_t libmod_input_joy_num( INSTANCE * my, int64_t * params ) {
    return ( joy_num() );
}

/* --------------------------------------------------------------------------- */
/**
 * JOY_NAME ()
 *
 * Returns the name for the selected joystick
 *
 **/

static int64_t libmod_input_joy_name( INSTANCE * my, int64_t * params ) {
    char * name = joy_name();
    int64_t result;
    if ( !name ) result = string_new("");
    else         result = string_new(name);
    string_use(result);
    return ( result );
}

/* --------------------------------------------------------------------------- */
/**
 * JOY_NAME (int JOY)
 *
 * Returns the name for a given joystick present in the system
 *
 **/

static int64_t libmod_input_joy_name_specific( INSTANCE * my, int64_t * params ) {
    char * name = joy_name_specific( params[0] );
    int64_t result;
    if ( !name ) result = string_new("");
    else         result = string_new(name);
    string_use(result);
    return ( result );
}

/* --------------------------------------------------------------------------- */
/**
 * JOY_SELECT (int JOY)
 * SELECT_JOY(int JOY)
 *
 * Returns the selected joystick number
 *
 **/

static int64_t libmod_input_joy_select( INSTANCE * my, int64_t * params ) {
    return ( joy_select( params[0] ) );
}

/* --------------------------------------------------------------------------- */
/**
 * JOY_BUTTONS ()
 * JOY_NUMBUTTONS ()
 *
 * Returns the selected joystick total buttons
 *
 **/

static int64_t libmod_input_joy_buttons( INSTANCE * my, int64_t * params ) {
    return ( joy_buttons() );
}

/* --------------------------------------------------------------------------- */
/**
 * JOY_AXES ()
 * JOY_NUMAXES()
 *
 * Returns the selected joystick total axes
 *
 **/

static int64_t libmod_input_joy_axes( INSTANCE * my, int64_t * params ) {
    return ( joy_axes() );
}

/* --------------------------------------------------------------------------- */
/**
 * JOY_GETBUTTON (int button)
 * GET_JOY_BUTTON (int button)
 *
 * Returns the selected joystick state for the given button
 *
 **/

static int64_t libmod_input_joy_get_button( INSTANCE * my, int64_t * params ) {
    return ( joy_get_button( params[0] ) );
}

/* --------------------------------------------------------------------------- */
/**
 * JOY_GETAXIS (int axis)
 * JOY_GETPOSITION (int axis)
 * GET_JOY_POSITION (int axis)
 *
 * Returns the selected joystick state for the given axis
 *
 **/

static int64_t libmod_input_joy_get_position( INSTANCE * my, int64_t * params ) {
    return ( joy_get_position( params[0] ) );
}

/* --------------------------------------------------------------------------- */
/**
 * JOY_NUMHATS ()
 *
 * Returns the total number of POV hats of the current selected joystick
 *
 **/

static int64_t libmod_input_joy_hats( INSTANCE * my, int64_t * params ) {
    return ( joy_hats() );
}

/* --------------------------------------------------------------------------- */
/**
 * JOY_NUMBALLS ()
 *
 * Returns the total number of balls of the current selected joystick
 *
 **/

static int64_t libmod_input_joy_balls( INSTANCE * my, int64_t * params ) {
    return ( joy_balls() );
}

/* --------------------------------------------------------------------------- */
/**
 * JOY_GETHAT (int HAT)
 *
 * Returns the state of the specfied hat on the current selected joystick
 *
 **/

static int64_t libmod_input_joy_get_hat( INSTANCE * my, int64_t * params ) {
    return ( joy_get_hat( params[0] ) );
}

/* --------------------------------------------------------------------------- */
/**
 * JOY_GETBALL (int BALL, int64_t * dx, int64_t * dy)
 *
 * Returns the state of the specfied ball on the current selected joystick
 *
 **/

static int64_t libmod_input_joy_get_ball( INSTANCE * my, int64_t * params ) {
    return ( joy_get_ball( params[0], ( int64_t * )( intptr_t )params[1], ( int64_t * )( intptr_t )params[2] ) );
}

/* --------------------------------------------------------------------------- */

static int64_t libmod_input_joy_get_accel( INSTANCE * my, int64_t * params ) {
    return ( joy_get_accel( ( int64_t * ) ( intptr_t ) params[0], ( int64_t * ) ( intptr_t ) params[1], ( int64_t * ) ( intptr_t ) params[2] ) );
}

/* --------------------------------------------------------------------------- */
/**
 * JOY_IS_ATTACHED ()
 *
 * Returns if the current selected joystick is attached
 *
 **/

static int64_t libmod_input_joy_is_attached( INSTANCE * my, int64_t * params ) {
    return joy_is_attached();
}

/* --------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/**
 * JOY_BUTTONS (int JOY)
 * JOY_NUMBUTTONS (int JOY)
 *
 * Returns the selected joystick total buttons
 *
 **/

static int64_t libmod_input_joy_buttons_specific( INSTANCE * my, int64_t * params ) {
    return ( joy_buttons_specific( params[0] ) );
}

/* --------------------------------------------------------------------------- */
/**
 * JOY_AXES (int JOY)
 * JOY_NUMAXES (int JOY)
 *
 * Returns the selected joystick total axes
 *
 **/

static int64_t libmod_input_joy_axes_specific( INSTANCE * my, int64_t * params ) {
    return ( joy_axes_specific( params[0] ) );
}

/* --------------------------------------------------------------------------- */
/**
 * JOY_GETBUTTON (int JOY, int button)
 * GET_JOY_BUTTON (int JOY, int button)
 *
 * Returns the selected joystick state for the given button
 *
 **/

static int64_t libmod_input_joy_get_button_specific( INSTANCE * my, int64_t * params ) {
    return ( joy_get_button_specific( params[0], params[1] ) );
}

/* --------------------------------------------------------------------------- */
/**
 * JOY_GETAXIS (int JOY, int axis)
 * JOY_GETPOSITION (int JOY, int axis)
 * GET_JOY_POSITION (int JOY, int axis)
 *
 * Returns the selected joystick state for the given axis
 *
 **/

static int64_t libmod_input_joy_get_position_specific( INSTANCE * my, int64_t * params ) {
    return ( joy_get_position_specific( params[0], params[1] ) );
}

/* --------------------------------------------------------------------------- */
/* Added by Sandman */
/* --------------------------------------------------------------------------- */
/**
 * JOY_NUMHATS (int JOY)
 *
 * Returns the total number of POV hats of the specified joystick
 *
 **/

static int64_t libmod_input_joy_hats_specific( INSTANCE * my, int64_t * params ) {
    return ( joy_hats_specific( params[0] ) );
}

/* --------------------------------------------------------------------------- */
/**
 * JOY_NUMBALLS (int JOY)
 *
 * Returns the total number of balls of the specified joystick
 *
 **/

static int64_t libmod_input_joy_balls_specific( INSTANCE * my, int64_t * params ) {
    return ( joy_balls_specific( params[0] ) );
}

/* --------------------------------------------------------------------------- */
/**
 * JOY_GETHAT (int JOY, int HAT)
 *
 * Returns the state of the specfied hat on the specified joystick
 *
 **/

static int64_t libmod_input_joy_get_hat_specific( INSTANCE * my, int64_t * params ) {
    return ( joy_get_hat_specific( params[0], params[1] ) );
}

/* --------------------------------------------------------------------------- */
/**
 * JOY_GETBALL (int JOY, int BALL, int64_t * dx, int64_t * dy)
 *
 * Returns the state of the specfied ball on the specified joystick
 *
 **/

static int64_t libmod_input_joy_get_ball_specific( INSTANCE * my, int64_t * params ) {
    return ( joy_get_ball_specific( params[0], params[1], ( int64_t * ) ( intptr_t ) params[2], ( int64_t * ) ( intptr_t ) params[3] ) );
}

/* --------------------------------------------------------------------------- */

static int64_t libmod_input_joy_get_accel_specific( INSTANCE * my, int64_t * params ) {
    return ( joy_get_accel_specific( params[0], ( int64_t * ) ( intptr_t ) params[1], ( int64_t * ) ( intptr_t ) params[2], ( int64_t * ) ( intptr_t ) params[3] ) );
}

/* --------------------------------------------------------------------------- */
/**
 * JOY_IS_ATTACHED (int JOY)
 *
 * Returns if the specified joystick is attached
 *
 **/

static int64_t libmod_input_joy_is_attached_specific( INSTANCE * my, int64_t * params ) {
    return joy_is_attached_specific( params[0] );
}





static int64_t libmod_input_joy_query_specific( INSTANCE * my, int64_t * params ) {
    return joy_query_specific( params[0], params[1] );
}

/* --------------------------------------------------------------------------- */
/**
 * JOY_SET(int joy, int element, int arg1, int arg2)
 * 
 * Sets specific information for the specified joystick, with options for 2 arguments.
 *
 **/

static int64_t libmod_input_joy_set_specific2( INSTANCE * my, int64_t * params ) {
    int64_t arg2;
#ifdef JOY_SEND_EFFECT_ENABLED
    if ( params[1] == JOY_SET_SEND_EFFECT ) {
        arg2 = (int64_t) string_get( params[2] );
    } else {
#endif
        arg2 = params[2];
#ifdef JOY_SEND_EFFECT_ENABLED
    }
#endif

    return joy_set_specific( params[0], params[1], arg2, params[3], 0 );
}

/* --------------------------------------------------------------------------- */
/**
 * JOY_SET(int joy, int element, int arg1, int arg2, int arg3)
 * 
 * Sets specific information for the specified joystick, with options for 3 arguments.
 *
 **/

static int64_t libmod_input_joy_set_specific3( INSTANCE * my, int64_t * params ) {
    int64_t arg2;
#ifdef JOY_SEND_EFFECT_ENABLED
    if ( params[1] == JOY_SET_SEND_EFFECT ) {
        arg2 = (int64_t) string_get( params[2] );
    } else {
#endif
        arg2 = params[2];
#ifdef JOY_SEND_EFFECT_ENABLED
    }
#endif

    return joy_set_specific( params[0], params[1], arg2, params[3], params[4] );
}

/* --------------------------------------------------------------------------- */
/* exports                                                                     */
/* --------------------------------------------------------------------------- */

#include "libmod_input_exports.h"

/* --------------------------------------------------------------------------- */
