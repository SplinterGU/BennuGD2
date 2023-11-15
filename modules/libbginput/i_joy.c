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

/* --------------------------------------------------------------------------- */
/* Thanks Sandman for suggest on openjoys at initialization time               */
/* --------------------------------------------------------------------------- */
/* Credits SplinterGU/Sandman 2007-2009                                        */
/* --------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <SDL.h>

#include "bgddl.h"
#include "bgdrtm.h"
#include "xstrings.h"

#include "libbginput.h"

/* --------------------------------------------------------------------------- */

#ifdef TARGET_CAANOO
#include "caanoo/te9_tf9_hybrid_driver.c"

#ifndef ABS
#define ABS(x) (((x) < 0) ? -(x):(x))
#endif

#endif

/* --------------------------------------------------------------------------- */

#define MAX_JOYS    16


typedef struct bindElement
{
    SDL_GameControllerBindType inType;
    union
    {
        int button;

        struct
        {
            int axis;
            int axis_min;
            int axis_max;
            int axis_total;     // max - min
            int threshold;
        } axis;

        struct
        {
            int hat;
            int hat_mask;
        } hat;

    } in;

    SDL_GameControllerBindType outType;
    union
    {
        struct
        {
            int axis_min;
            int axis_max;
            int axis_total;       // max - min
        } axis;

    } out;

    struct bindElement * next;

} bindElement;

typedef struct {
    int instanceID;
    SDL_Joystick* joystick;
    bindElement * mapping[ SDL_CONTROLLER_BUTTON_MAX + SDL_CONTROLLER_AXIS_MAX ];
} JoystickInfo;

static int64_t _selected_joystick = -1;
static JoystickInfo _joystickList[MAX_JOYS];

/* --------------------------------------------------------------------------- */
/* Internals mapping functions                                                 */
/* --------------------------------------------------------------------------- */

/*
 * Given a controller button name and a joystick name, update our mapping structure with it
 */

void parseElement(int joyIdx, const char *szGameButton, const char *szJoystickButton)
{
    SDL_GameControllerButton button;
    SDL_GameControllerAxis axis;
    SDL_bool invert_in = 0;
    char half_axis_in = 0;
    char half_axis_out = 0;

    if (*szGameButton == '+' || *szGameButton == '-') {
        half_axis_out = *szGameButton++;
    }

    axis = SDL_GameControllerGetAxisFromString(szGameButton);
    button = SDL_GameControllerGetButtonFromString(szGameButton);

    int element = button;

    bindElement * bind = malloc( sizeof( bindElement ) );
    if ( !bind ) return;

    if (axis != SDL_CONTROLLER_AXIS_INVALID) element += SDL_CONTROLLER_BUTTON_MAX + 1 + axis;

    bind->next = _joystickList[ joyIdx ].mapping[ element ];

    if (axis != SDL_CONTROLLER_AXIS_INVALID) {
        bind->outType = SDL_CONTROLLER_BINDTYPE_AXIS;

        if (axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT || axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT) {
            bind->out.axis.axis_min = 0;
            bind->out.axis.axis_max = SDL_JOYSTICK_AXIS_MAX;
        }
        else
        {
            if (half_axis_out == '+') {
                bind->out.axis.axis_min = 0;
                bind->out.axis.axis_max = SDL_JOYSTICK_AXIS_MAX;
            }
            else
            if (half_axis_out == '-') {
                bind->out.axis.axis_min = 0;
                bind->out.axis.axis_max = SDL_JOYSTICK_AXIS_MIN;
            }
            else
            {
                bind->out.axis.axis_min = SDL_JOYSTICK_AXIS_MIN;
                bind->out.axis.axis_max = SDL_JOYSTICK_AXIS_MAX;
            }
        }
        bind->out.axis.axis_total = bind->out.axis.axis_max - bind->out.axis.axis_min;
    }
    else
    if (button != SDL_CONTROLLER_BUTTON_INVALID) {
        bind->outType = SDL_CONTROLLER_BINDTYPE_BUTTON;
    }
    else
    {
        // Unexpected controller element
        free( bind );
        return;
    }

    if (*szJoystickButton == '+' || *szJoystickButton == '-') {
        half_axis_in = *szJoystickButton++;
    }
    
    if (szJoystickButton[strlen(szJoystickButton) - 1] == '~') {
        invert_in = 1;
    }

    if (szJoystickButton[0] == 'a' && isdigit((unsigned char)szJoystickButton[1])) {
        bind->inType = SDL_CONTROLLER_BINDTYPE_AXIS;
        bind->in.axis.axis = atoi(&szJoystickButton[1]);
        if (half_axis_in == '-') {
            if ( invert_in ) {
                bind->in.axis.axis_min = SDL_JOYSTICK_AXIS_MIN;
                bind->in.axis.axis_max = 0;
            }
            else
            {
                bind->in.axis.axis_min = 0;
                bind->in.axis.axis_max = SDL_JOYSTICK_AXIS_MIN;
            }
        }
        else
        {
            if (half_axis_in == '+') {
                if ( invert_in ) {
                    bind->in.axis.axis_min = SDL_JOYSTICK_AXIS_MAX;
                    bind->in.axis.axis_max = 0;
                }
                else
                {
                    bind->in.axis.axis_min = 0;
                    bind->in.axis.axis_max = SDL_JOYSTICK_AXIS_MAX;
                }
            } else {
                if ( invert_in ) {
                    bind->in.axis.axis_min = SDL_JOYSTICK_AXIS_MAX;
                    bind->in.axis.axis_max = SDL_JOYSTICK_AXIS_MIN;
                }
                else
                {
                    bind->in.axis.axis_min = SDL_JOYSTICK_AXIS_MIN;
                    bind->in.axis.axis_max = SDL_JOYSTICK_AXIS_MAX;
                }
            }
        }
        bind->in.axis.axis_total = bind->in.axis.axis_max - bind->in.axis.axis_min;
        bind->in.axis.threshold = bind->in.axis.axis_min + bind->in.axis.axis_total / 2;
    }
    else
    if (szJoystickButton[0] == 'b' && isdigit((unsigned char)szJoystickButton[1])) {
        bind->inType = SDL_CONTROLLER_BINDTYPE_BUTTON;
        bind->in.button = SDL_atoi(&szJoystickButton[1]);
    }
    else
    if (szJoystickButton[0] == 'h' && isdigit((unsigned char)szJoystickButton[1]) &&
        szJoystickButton[2] == '.' && isdigit((unsigned char)szJoystickButton[3])) {
        int hat = atoi(&szJoystickButton[1]);
        int mask = atoi(&szJoystickButton[3]);
        bind->inType = SDL_CONTROLLER_BINDTYPE_HAT;
        bind->in.hat.hat = hat;
        bind->in.hat.hat_mask = mask;
    }
    else
    {
        // Unexpected joystick element
        free( bind );
        return;
    }

    _joystickList[ joyIdx ].mapping[ element ] = bind;
}

/*
 * Given a controller mapping string, update our mapping object
 */

void ParseMapping(int joyIdx, const char *pchString)
{
    char szGameButton[20];
    char szJoystickButton[20];
    SDL_bool parseButton = 1;

    const char *pchPos = pchString;

    int i;

    // Skip GUID
    pchPos = strchr(pchPos, ',' );
    if ( !pchPos ) return;

    // Skip Name
    pchPos = strchr(pchPos + 1, ',' );
    if ( !pchPos ) return;

    memset( _joystickList[ joyIdx ].mapping, '\0', sizeof( _joystickList[ joyIdx ].mapping ) );

    i = 0;

    szGameButton[0] = '\0';
    szJoystickButton[0] = '\0';

    while (pchPos && *pchPos) {
        switch(*pchPos) {
            case    ':':
                    i = 0;
                    parseButton = 0;
                    break;

            case    ' ':
                    break;

            case    ',':
                    i = 0;
                    parseButton = 1;
                    parseElement( joyIdx, szGameButton, szJoystickButton );
                    szGameButton[0] = '\0';
                    szJoystickButton[0] = '\0';
                    break;

            default:
                    if (parseButton) {
                        if (i >= sizeof(szGameButton)) return; // Button name too large
                        szGameButton[i] = *pchPos;
                        i++;
                        szGameButton[i] = '\0';
                    }
                    else
                    {
                        if (i >= sizeof(szJoystickButton)) return; // Joystick button name too large
                        szJoystickButton[i] = *pchPos;
                        i++;
                        szJoystickButton[i] = '\0';
                    }
                    break;

        }
        pchPos++;
    }

    /* No more values if the string was terminated by a comma. Don't report an error. */
    if (szGameButton[0] || szJoystickButton[0] ) {
        parseElement( joyIdx, szGameButton, szJoystickButton );
    }
}

/*
 * Queries the button input of the joystick
 */

int _joy_query_button( int joyIdx, int button ) {
    int value = 0;

    bindElement * bind = _joystickList[ joyIdx ].mapping[ button ];
    SDL_Joystick * joystick = _joystickList[ joyIdx ].joystick;

    while( bind ) {
        switch ( bind->inType ) {
            case SDL_CONTROLLER_BINDTYPE_BUTTON:
                value = SDL_JoystickGetButton(joystick, bind->in.button );
                break;

            case SDL_CONTROLLER_BINDTYPE_AXIS:
                value = SDL_JoystickGetAxis(joystick, bind->in.axis.axis );
                if (bind->in.axis.axis_min < bind->in.axis.axis_max) {
                    if ( value >= bind->in.axis.axis_min && value <= bind->in.axis.axis_max ) value = value >= bind->in.axis.threshold;
                } else {
                    if ( value >= bind->in.axis.axis_max && value <= bind->in.axis.axis_min ) value = value <= bind->in.axis.threshold;
                }
                break;

            case SDL_CONTROLLER_BINDTYPE_HAT:
                value = ( SDL_JoystickGetHat(joystick, bind->in.hat.hat ) & bind->in.hat.hat_mask ) != 0;
                break;

            default:
                break;

        }
        if ( value ) break;
        bind = bind->next;
    }

    return value;
}


/*
 * Queries the axis input of the joystick
 */

int _joy_query_axis( int joyIdx, int axis ) {
   int value = 0;

    bindElement * bind = _joystickList[ joyIdx ].mapping[ axis ];
    SDL_Joystick * joystick = _joystickList[ joyIdx ].joystick;

    while( bind ) {
        switch ( bind->inType ) {
            case SDL_CONTROLLER_BINDTYPE_BUTTON:
                value = SDL_JoystickGetButton(joystick, bind->in.button );
                if ( value ) value = bind->out.axis.axis_max;
                else         value = bind->out.axis.axis_min;
                break;

            case SDL_CONTROLLER_BINDTYPE_AXIS:
                value = SDL_JoystickGetAxis(joystick, bind->in.axis.axis );
                if ( (value >= bind->in.axis.axis_min && value <= bind->in.axis.axis_max) ||
                     (value >= bind->in.axis.axis_max && value <= bind->in.axis.axis_min) ) {
                    if (bind->in.axis.axis_min != bind->out.axis.axis_min || bind->in.axis.axis_max != bind->out.axis.axis_max) {
                        float normalized_value = (float)(value - bind->in.axis.axis_min) / bind->in.axis.axis_total;
                        value = bind->out.axis.axis_min + (int)(normalized_value * bind->out.axis.axis_total);
                    }
                } else {
                    value = 0;
                }
                break;

            case SDL_CONTROLLER_BINDTYPE_HAT:
                value = ( SDL_JoystickGetHat(joystick, bind->in.hat.hat ) & bind->in.hat.hat_mask ) != 0;
                if ( value ) value = bind->out.axis.axis_max;
                else         value = bind->out.axis.axis_min;
                break;

            default:
                break;

        }
        if ( value ) break;
        bind = bind->next;
    }

    return value;
}
/* --------------------------------------------------------------------------- */
/* Internals mapping functions end                                             */
/* --------------------------------------------------------------------------- */










/* --------------------------------------------------------------------------- */
/* joy_name_specific (int64_t JOY)                                             */
/* Returns the name for a given joystick present in the system                 */
/* --------------------------------------------------------------------------- */

int64_t joy_name_specific( int64_t joy ) {
    int64_t result;
    if ( joy >= 0 && joy < MAX_JOYS && _joystickList[ joy ].joystick ) {
        result = string_new( SDL_JoystickName( _joystickList[ joy ].joystick ) );
    } else {
        result = string_new( "" );
    }
    string_use( result );
    return result;
}

/* --------------------------------------------------------------------------- */
/* joy_buttons_specific (int64_t JOY)                                           */
/* Returns the selected joystick total buttons                                 */
/* --------------------------------------------------------------------------- */

int64_t joy_buttons_specific( int64_t joy ) {
    if ( joy >= 0 && joy < MAX_JOYS && _joystickList[ joy ].joystick ) {
#ifdef TARGET_CAANOO
        if ( joy == 0 ) return 21;
#endif
        return SDL_JoystickNumButtons( _joystickList[ joy ].joystick );
    }
    return 0;
}

/* --------------------------------------------------------------------------- */
/* joy_axes_specific (int64_t JOY)                                              */
/* Returns the selected joystick total axes                                    */
/* --------------------------------------------------------------------------- */

int64_t joy_axes_specific( int64_t joy ) {
    if ( joy >= 0 && joy < MAX_JOYS && _joystickList[ joy ].joystick ) {
        return SDL_JoystickNumAxes( _joystickList[ joy ].joystick );
    }
    return 0;
}

/* --------------------------------------------------------------------------- */
/* joy_get_button_specific (int64_t JOY, int64_t button)                            */
/* Returns the selected joystick state for the given button                    */
/* --------------------------------------------------------------------------- */

int64_t joy_get_button_specific( int64_t joy, int64_t button ) {
    if ( joy >= 0 && joy < MAX_JOYS && _joystickList[ joy ].joystick ) {
        if ( button >= JOY_BUTTON_A && button < JOY_BUTTON_MAX ) {
            return _joy_query_button( joy, button - 0x100 );
        }
        else
#ifdef TARGET_CAANOO
        if ( button >= 0 && ( ( joy == 0 && button <= 21 ) || ( joy != 0 && button <= SDL_JoystickNumButtons( _joystickList[ joy ].joystick ) ) ) )
#else
        if ( button >= 0 && button <= SDL_JoystickNumButtons( _joystickList[ joy ].joystick ) )
#endif
        {
#ifdef TARGET_CAANOO
            if ( joy == 0 ) {
                int64_t vax;

                switch ( button ) {
                    case    1: /* UPLF                  */  return ( SDL_JoystickGetAxis( _joystickList[ 0 ], 1 ) < -16384 && SDL_JoystickGetAxis( _joystickList[ 0 ], 0 ) < -16384 );
                    case    3: /* DWLF                  */  return ( SDL_JoystickGetAxis( _joystickList[ 0 ], 1 ) >  16384 && SDL_JoystickGetAxis( _joystickList[ 0 ], 0 ) < -16384 );
                    case    5: /* DWRT                  */  return ( SDL_JoystickGetAxis( _joystickList[ 0 ], 1 ) >  16384 && SDL_JoystickGetAxis( _joystickList[ 0 ], 0 ) >  16384 );
                    case    7: /* UPRT                  */  return ( SDL_JoystickGetAxis( _joystickList[ 0 ], 1 ) < -16384 && SDL_JoystickGetAxis( _joystickList[ 0 ], 0 ) >  16384 );
                    case    0: /* UP                    */  vax = SDL_JoystickGetAxis( _joystickList[ 0 ], 0 ); return ( SDL_JoystickGetAxis( _joystickList[ 0 ], 1 ) < -16384 && ABS( vax ) < 16384 );
                    case    4: /* DW                    */  vax = SDL_JoystickGetAxis( _joystickList[ 0 ], 0 ); return ( SDL_JoystickGetAxis( _joystickList[ 0 ], 1 ) >  16384 && ABS( vax ) < 16384 );
                    case    2: /* LF                    */  vax = SDL_JoystickGetAxis( _joystickList[ 0 ], 1 ); return ( SDL_JoystickGetAxis( _joystickList[ 0 ], 0 ) < -16384 && ABS( vax ) < 16384 );
                    case    6: /* RT                    */  vax = SDL_JoystickGetAxis( _joystickList[ 0 ], 1 ); return ( SDL_JoystickGetAxis( _joystickList[ 0 ], 0 ) >  16384 && ABS( vax ) < 16384 );

                    case    8:  /* MENU->HOME           */  return ( SDL_JoystickGetButton( _joystickList[ 0 ], 6 ) );
                    case    9:  /* SELECT->HELP-II      */  return ( SDL_JoystickGetButton( _joystickList[ 0 ], 9 ) );
                    case    10: /* L                    */  return ( SDL_JoystickGetButton( _joystickList[ 0 ], 4 ) );
                    case    11: /* R                    */  return ( SDL_JoystickGetButton( _joystickList[ 0 ], 5 ) );
                    case    12: /* A                    */  return ( SDL_JoystickGetButton( _joystickList[ 0 ], 0 ) );
                    case    13: /* B                    */  return ( SDL_JoystickGetButton( _joystickList[ 0 ], 2 ) );
                    case    14: /* X                    */  return ( SDL_JoystickGetButton( _joystickList[ 0 ], 1 ) );
                    case    15: /* Y                    */  return ( SDL_JoystickGetButton( _joystickList[ 0 ], 3 ) );
                    case    16: /* VOLUP                */  return ( 0 );
                    case    17: /* VOLDOWN              */  return ( 0 );
                    case    18: /* CLICK                */  return ( SDL_JoystickGetButton( _joystickList[ 0 ], 10 ) );
                    case    19: /* POWER-LOCK  (CAANOO) */  return ( SDL_JoystickGetButton( _joystickList[ 0 ], 7 ) ); /* Only Caanoo */
                    case    20: /* HELP-I      (CAANOO) */  return ( SDL_JoystickGetButton( _joystickList[ 0 ], 8 ) ); /* Only Caanoo */
                    default:                                return ( 0 );
                }
            }
#endif
            return SDL_JoystickGetButton( _joystickList[ joy ].joystick, button );
        }
    }
    return 0;
}

/* --------------------------------------------------------------------------- */
/* joy_get_position_specific (int64_t JOY, int64_t axis)                            */
/* Returns the selected joystick state for the given axis                      */
/* --------------------------------------------------------------------------- */

int64_t joy_get_position_specific( int64_t joy, int64_t axis ) {
    if ( joy >= 0 && joy < MAX_JOYS && _joystickList[ joy ].joystick ) {
        if ( axis >= JOY_AXIS_LEFTX && axis < JOY_AXIS_MAX ) {
            return _joy_query_axis( joy, axis - JOY_BUTTON_A );
        } else
        if ( axis >= 0 && axis <= SDL_JoystickNumAxes( _joystickList[ joy ].joystick ) ) {
            return SDL_JoystickGetAxis( _joystickList[ joy ].joystick, axis );
        }
    }
    return 0;
}

/* --------------------------------------------------------------------------- */
/* Added by Sandman */
/* --------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------- */
/* joy_hats_specific (int64_t JOY)                                              */
/* Returns the total number of POV hats of the specified joystick              */
/* --------------------------------------------------------------------------- */

int64_t joy_hats_specific( int64_t joy ) {
    if ( joy >= 0 && joy < MAX_JOYS && _joystickList[ joy ].joystick ) {
        return SDL_JoystickNumHats( _joystickList[ joy ].joystick );
    }
    return 0;
}

/* --------------------------------------------------------------------------- */
/* joy_balls_specific (int64_t JOY)                                             */
/* Returns the total number of balls of the specified joystick                 */
/* --------------------------------------------------------------------------- */

int64_t joy_balls_specific( int64_t joy ) {
    if ( joy >= 0 && joy < MAX_JOYS && _joystickList[ joy ].joystick ) {
        return SDL_JoystickNumBalls( _joystickList[ joy ].joystick );
    }
    return 0;
}

/* --------------------------------------------------------------------------- */
/* joy_get_hat_specific (int64_t JOY, int64_t HAT)                                  */
/* Returns the state of the specfied hat on the specified joystick             */
/* --------------------------------------------------------------------------- */

int64_t joy_get_hat_specific( int64_t joy, int64_t hat ) {
    if ( joy >= 0 && joy < MAX_JOYS && _joystickList[ joy ].joystick ) {
        if ( hat >= 0 && hat <= SDL_JoystickNumHats( _joystickList[ joy ].joystick ) ) {
            return SDL_JoystickGetHat( _joystickList[ joy ].joystick, hat );
        }
    }
    return 0;
}

/* --------------------------------------------------------------------------- */
/* joy_get_ball_specific (int64_t JOY, int64_t BALL, int64_t* dx, int64_t* dy)              */
/* Returns the state of the specfied ball on the specified joystick            */
/* --------------------------------------------------------------------------- */

int64_t joy_get_ball_specific( int64_t joy, int64_t ball, int64_t * dx, int64_t * dy ) {
    if ( joy >= 0 && joy < MAX_JOYS && _joystickList[ joy ].joystick ) {
        if ( ball >= 0 && ball <= SDL_JoystickNumBalls( _joystickList[ joy ].joystick ) ) {
            int _dx, _dy;
            int64_t r = SDL_JoystickGetBall( _joystickList[ joy ].joystick, ball, &_dx, &_dy );
            * dx = ( int64_t ) _dx;
            * dy = ( int64_t ) _dy;
            return r;
        }
    }
    return -1;
}

/* --------------------------------------------------------------------------- */

int64_t joy_get_accel_specific( int64_t joy, int64_t * x, int64_t * y, int64_t * z ) {
#ifdef TARGET_CAANOO
    if ( joy == 0 ) {
        KIONIX_ACCEL_read_LPF_g( x, y, z );
        return 0;
    }
#endif
    return -1;
}

/* --------------------------------------------------------------------------- */
/* joy_powerlevel_specific (int64_t JOY)                                       */
/* Returns the joystick battery level                                          */
/* --------------------------------------------------------------------------- */

int64_t joy_powerlevel_specific( int64_t joy ) {
    if ( joy >= 0 && joy < MAX_JOYS && _joystickList[ joy ].joystick ) {
        return SDL_JoystickCurrentPowerLevel( _joystickList[ joy ].joystick );
    }
    return 0;
}

/* --------------------------------------------------------------------------- */
/* joy_is_attached_specific (int64_t JOY)                                      */
/* Returns if joystick is attached                                             */
/* --------------------------------------------------------------------------- */

int64_t joy_is_attached_specific( int64_t joy ) {
    if ( joy < 0 || joy >= MAX_JOYS || !_joystickList[ joy ].joystick ) return 0;
    return ( SDL_JoystickGetAttached( _joystickList[ joy ].joystick ) );
}

/* --------------------------------------------------------------------------- */
/* joy_num ()                                                               */
/* Returns the number of joysticks present in the system                       */
/* --------------------------------------------------------------------------- */

int64_t joy_num( void ) {
    return SDL_NumJoysticks();
}

/* --------------------------------------------------------------------------- */
/* joy_select (int64_t JOY)                                                     */
/* Returns the selected joystick number                                        */
/* --------------------------------------------------------------------------- */

int64_t joy_select( int64_t joy ) {
    if ( joy < 0 || joy >= MAX_JOYS ) return -1;
    return ( _selected_joystick = joy );
}

/* --------------------------------------------------------------------------- */
/* joy_name ()                                                                 */
/* Returns the name for the selected joystick                                  */
/* --------------------------------------------------------------------------- */

int64_t joy_name( void ) {
    return joy_name_specific( _selected_joystick );
}

/* --------------------------------------------------------------------------- */
/* joy_buttons ()                                                           */
/* Returns the selected joystick total buttons                                 */
/* --------------------------------------------------------------------------- */

int64_t joy_buttons( void ) {
    return joy_buttons_specific( _selected_joystick );
}

/* --------------------------------------------------------------------------- */
/* joy_axes ()                                                              */
/* Returns the selected joystick total axes                                    */
/* --------------------------------------------------------------------------- */

int64_t joy_axes( void ) {
    return joy_axes_specific( _selected_joystick );
}

/* --------------------------------------------------------------------------- */
/* joy_get_button ( int64_t button )                                            */
/* Returns the selected joystick state for the given button                    */
/* --------------------------------------------------------------------------- */

int64_t joy_get_button( int64_t button ) {
    return joy_get_button_specific( _selected_joystick, button );
}

/* --------------------------------------------------------------------------- */
/* joy_get_position ( int64_t axis )                                            */
/* Returns the selected joystick state for the given axis                      */
/* --------------------------------------------------------------------------- */

int64_t joy_get_position( int64_t axis ) {
    return joy_get_position_specific( _selected_joystick, axis );
}

/* --------------------------------------------------------------------------- */
/* joy_hats ()                                                              */
/* Returns the total number of POV hats of the current selected joystick       */
/* --------------------------------------------------------------------------- */

int64_t joy_hats( void ) {
    return joy_hats_specific( _selected_joystick );
}

/* --------------------------------------------------------------------------- */
/* joy_balls ()                                                             */
/* Returns the total number of balls of the current selected joystick          */
/* --------------------------------------------------------------------------- */

int64_t joy_balls( void ) {
    return joy_balls_specific( _selected_joystick );
}

/* --------------------------------------------------------------------------- */
/* joy_get_hat (int64_t HAT)                                                    */
/* Returns the state of the specfied hat on the current selected joystick      */
/* --------------------------------------------------------------------------- */

int64_t joy_get_hat( int64_t hat ) {
    return joy_get_hat_specific( _selected_joystick, hat );
}

/* --------------------------------------------------------------------------- */
/* joy_get_ball (int64_t BALL, int64_t* dx, int64_t* dy)                                */
/* Returns the state of the specfied ball on the current selected joystick     */
/* --------------------------------------------------------------------------- */

int64_t joy_get_ball( int64_t ball, int64_t * dx, int64_t * dy ) {
    return joy_get_ball_specific( _selected_joystick, ball, dx, dy );
}

/* --------------------------------------------------------------------------- */

int64_t joy_get_accel( int64_t * x, int64_t * y, int64_t * z ) {
    return joy_get_accel_specific( _selected_joystick, x, y, z );
}

/* --------------------------------------------------------------------------- */
/* joy_powerlevel ()                                                           */
/* Returns the joystick battery level                                          */
/* --------------------------------------------------------------------------- */

int64_t joy_powerlevel() {
    return joy_powerlevel_specific( _selected_joystick );
}

/* --------------------------------------------------------------------------- */
/* joy_is_attached ()                                                          */
/* Returns if selected joystick is attached                                    */
/* --------------------------------------------------------------------------- */

int64_t joy_is_attached() {
    return joy_is_attached_specific( _selected_joystick);
}

/* --------------------------------------------------------------------------- */
/* Funciones de inicializacion del modulo/plugin                               */
/* --------------------------------------------------------------------------- */

void joy_init() {
    int64_t i;

    if ( !SDL_WasInit( SDL_INIT_JOYSTICK ) ) {
        SDL_InitSubSystem( SDL_INIT_JOYSTICK );
        SDL_JoystickEventState( SDL_ENABLE );
    }

    int max_joys = SDL_NumJoysticks();

    /* Open all joysticks */
    if ( max_joys > MAX_JOYS ) {
        printf( "[JOY] Warning: maximum number of joysticks exceeded (%i>%i)", ( int ) max_joys, MAX_JOYS );
        max_joys = MAX_JOYS;
    }

    for ( i = 0; i < max_joys; i++ ) {
        _joystickList[i].joystick = SDL_JoystickOpen( i );
        if ( _joystickList[ i ].joystick ) {
            _joystickList[i].instanceID = SDL_JoystickInstanceID( _joystickList[i].joystick );
            ParseMapping(i, SDL_GameControllerMappingForGUID(SDL_JoystickGetGUID(_joystickList[i].joystick)));
        } else {
            printf( "[JOY] Failed to open joystick '%i'", ( int ) i );
        }
    }

    for ( ; i < MAX_JOYS; i++ ) {
        _joystickList[i].joystick = NULL;
        _joystickList[i].instanceID = -1;
    }

    SDL_JoystickUpdate();

#ifdef TARGET_CAANOO
    KIONIX_ACCEL_init();

    if ( KIONIX_ACCEL_get_device_type() != DEVICE_TYPE_KIONIX_KXTF9 ) KIONIX_ACCEL_deinit();

    KXTF9_set_G_range(2);
    KXTF9_set_resolution(12);
    KXTF9_set_lpf_odr(400);

    KIONIX_ACCEL_enable_outputs();
#endif
}

/* ----------------------------------------------------------------- */

void joy_exit() {
    int64_t i;

    #ifdef TARGET_CAANOO
    KIONIX_ACCEL_deinit();
    #endif

    for ( i = 0; i < MAX_JOYS; i++ )
        if ( _joystickList[ i ].joystick ) SDL_JoystickClose( _joystickList[ i ].joystick );

    if ( SDL_WasInit( SDL_INIT_JOYSTICK ) ) SDL_QuitSubSystem( SDL_INIT_JOYSTICK );
}

/* ----------------------------------------------------------------- */

void process_joy_events() {
    int events_processed = 0;

    SDL_Event e;

#if 0
    /* Joystick events */
    SDL_JOYAXISMOTION  = 0x600, /**< Joystick axis motion */
    SDL_JOYBALLMOTION,          /**< Joystick trackball motion */
    SDL_JOYHATMOTION,           /**< Joystick hat position change */
    SDL_JOYBUTTONDOWN,          /**< Joystick button pressed */
    SDL_JOYBUTTONUP,            /**< Joystick button released */
    SDL_JOYDEVICEADDED,         /**< A new joystick has been inserted into the system */
    SDL_JOYDEVICEREMOVED,       /**< An opened joystick has been removed */
    SDL_JOYBATTERYUPDATED,      /**< Joystick battery level change */
#endif

    while (SDL_PeepEvents(&e, 1, SDL_GETEVENT, SDL_JOYDEVICEADDED, SDL_JOYDEVICEREMOVED) > 0) {
        if (e.type == SDL_JOYDEVICEADDED) {
            int64_t device_index = e.jdevice.which;
            for (int j = 0; j < MAX_JOYS; j++) {
                // Add joy to list
                if (_joystickList[j].instanceID == -1) {
                    _joystickList[j].joystick = SDL_JoystickOpen(device_index);
                    if (_joystickList[j].joystick) {
                        _joystickList[j].instanceID = SDL_JoystickInstanceID(_joystickList[j].joystick); // instanceID;
                        ParseMapping(j, SDL_GameControllerMappingForGUID(SDL_JoystickGetGUID(_joystickList[j].joystick)));
                    }
                    break;
                }
            }
        } else if (e.type == SDL_JOYDEVICEREMOVED) {
            int64_t instanceID = e.jdevice.which;
            for (int j = 0; j < MAX_JOYS; j++) {
                // Remove joy from list
                if (_joystickList[j].instanceID == instanceID) {
                    SDL_JoystickClose(_joystickList[j].joystick);
                    _joystickList[j].instanceID = -1;
                    _joystickList[j].joystick = NULL;

                    // Free mapping
                    for ( int i = 0; i < SDL_CONTROLLER_BUTTON_MAX + SDL_CONTROLLER_AXIS_MAX; i++ ) {
                        bindElement * b = _joystickList[ j ].mapping[ i ], * bn;
                        while( b ) {
                            bn = b->next;
                            free( b );
                            b = bn;
                        }
                    }
                    break;
                }
            }
        }
    }
}
