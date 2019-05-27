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

/* --------------------------------------------------------------------------- */
/* Thanks Sandman for suggest on openjoys at initialization time               */
/* --------------------------------------------------------------------------- */
/* Credits SplinterGU/Sandman 2007-2009                                        */
/* --------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

#define MAX_JOYS    32

static int64_t _max_joys = 0;
static SDL_Joystick * _joysticks[MAX_JOYS];
static int64_t _selected_joystick = -1;

/* --------------------------------------------------------------------------- */
/* joy_num ()                                                               */
/* Returns the number of joysticks present in the system                       */
/* --------------------------------------------------------------------------- */

int64_t joy_num( void ) {
    return _max_joys;
}

/* --------------------------------------------------------------------------- */
/* joy_name (int64_t JOY)                                                       */
/* Returns the name for a given joystick present in the system                 */
/* --------------------------------------------------------------------------- */

int64_t joy_name( int64_t joy ) {
    int64_t result;
    result = string_new( SDL_JoystickName( ( SDL_Joystick * ) joy ) );
    string_use( result );
    return result;
}

/* --------------------------------------------------------------------------- */
/* joy_select (int64_t JOY)                                                     */
/* Returns the selected joystick number                                        */
/* --------------------------------------------------------------------------- */

int64_t joy_select( int64_t joy ) {
    return ( _selected_joystick = joy );
}

/* --------------------------------------------------------------------------- */
/* joy_buttons ()                                                           */
/* Returns the selected joystick total buttons                                 */
/* --------------------------------------------------------------------------- */

int64_t joy_buttons( void ) {
    if ( _selected_joystick >= 0 && _selected_joystick < _max_joys ) {
#ifdef TARGET_CAANOO
        if ( _selected_joystick == 0 ) return 21;
#endif
        return SDL_JoystickNumButtons( _joysticks[ _selected_joystick ] );
    }
    return 0;
}

/* --------------------------------------------------------------------------- */
/* joy_axes ()                                                              */
/* Returns the selected joystick total axes                                    */
/* --------------------------------------------------------------------------- */

int64_t joy_axes( void ) {
    if ( _selected_joystick >= 0 && _selected_joystick < _max_joys ) {
        return SDL_JoystickNumAxes( _joysticks[ _selected_joystick ] );
    }
    return 0;
}

/* --------------------------------------------------------------------------- */
/* joy_get_button ( int64_t button )                                            */
/* Returns the selected joystick state for the given button                    */
/* --------------------------------------------------------------------------- */

int64_t joy_get_button( int64_t button ) {
    if ( _selected_joystick >= 0 && _selected_joystick < _max_joys ) {
#ifdef TARGET_CAANOO
        if ( _selected_joystick == 0 ) {
            int64_t vax;

            switch ( button ) {
                case    1: /* UPLF                  */  return ( SDL_JoystickGetAxis( _joysticks[ 0 ], 1 ) < -16384 && SDL_JoystickGetAxis( _joysticks[ 0 ], 0 ) < -16384 );
                case    3: /* DWLF                  */  return ( SDL_JoystickGetAxis( _joysticks[ 0 ], 1 ) >  16384 && SDL_JoystickGetAxis( _joysticks[ 0 ], 0 ) < -16384 );
                case    5: /* DWRT                  */  return ( SDL_JoystickGetAxis( _joysticks[ 0 ], 1 ) >  16384 && SDL_JoystickGetAxis( _joysticks[ 0 ], 0 ) >  16384 );
                case    7: /* UPRT                  */  return ( SDL_JoystickGetAxis( _joysticks[ 0 ], 1 ) < -16384 && SDL_JoystickGetAxis( _joysticks[ 0 ], 0 ) >  16384 );
                case    0: /* UP                    */  vax = SDL_JoystickGetAxis( _joysticks[ 0 ], 0 ); return ( SDL_JoystickGetAxis( _joysticks[ 0 ], 1 ) < -16384 && ABS( vax ) < 16384 );
                case    4: /* DW                    */  vax = SDL_JoystickGetAxis( _joysticks[ 0 ], 0 ); return ( SDL_JoystickGetAxis( _joysticks[ 0 ], 1 ) >  16384 && ABS( vax ) < 16384 );
                case    2: /* LF                    */  vax = SDL_JoystickGetAxis( _joysticks[ 0 ], 1 ); return ( SDL_JoystickGetAxis( _joysticks[ 0 ], 0 ) < -16384 && ABS( vax ) < 16384 );
                case    6: /* RT                    */  vax = SDL_JoystickGetAxis( _joysticks[ 0 ], 1 ); return ( SDL_JoystickGetAxis( _joysticks[ 0 ], 0 ) >  16384 && ABS( vax ) < 16384 );

                case    8:  /* MENU->HOME           */  return ( SDL_JoystickGetButton( _joysticks[ 0 ], 6 ) );
                case    9:  /* SELECT->HELP-II      */  return ( SDL_JoystickGetButton( _joysticks[ 0 ], 9 ) );
                case    10: /* L                    */  return ( SDL_JoystickGetButton( _joysticks[ 0 ], 4 ) );
                case    11: /* R                    */  return ( SDL_JoystickGetButton( _joysticks[ 0 ], 5 ) );
                case    12: /* A                    */  return ( SDL_JoystickGetButton( _joysticks[ 0 ], 0 ) );
                case    13: /* B                    */  return ( SDL_JoystickGetButton( _joysticks[ 0 ], 2 ) );
                case    14: /* X                    */  return ( SDL_JoystickGetButton( _joysticks[ 0 ], 1 ) );
                case    15: /* Y                    */  return ( SDL_JoystickGetButton( _joysticks[ 0 ], 3 ) );
                case    16: /* VOLUP                */  return ( 0 );
                case    17: /* VOLDOWN              */  return ( 0 );
                case    18: /* CLICK                */  return ( SDL_JoystickGetButton( _joysticks[ 0 ], 10 ) );
                case    19: /* POWER-LOCK  (CAANOO) */  return ( SDL_JoystickGetButton( _joysticks[ 0 ], 7 ) ); /* Only Caanoo */
                case    20: /* HELP-I      (CAANOO) */  return ( SDL_JoystickGetButton( _joysticks[ 0 ], 8 ) ); /* Only Caanoo */
                default:                                return ( 0 );
            }
        }
#endif
        return SDL_JoystickGetButton( _joysticks[ _selected_joystick ], button );
    }
    return 0;
}

/* --------------------------------------------------------------------------- */
/* joy_get_position ( int64_t axis )                                            */
/* Returns the selected joystick state for the given axis                      */
/* --------------------------------------------------------------------------- */

int64_t joy_get_position( int64_t axis ) {
    if ( _selected_joystick >= 0 && _selected_joystick < _max_joys ) {
        return SDL_JoystickGetAxis( _joysticks[ _selected_joystick ], axis );
    }
    return 0;
}

/* --------------------------------------------------------------------------- */
/* joy_hats ()                                                              */
/* Returns the total number of POV hats of the current selected joystick       */
/* --------------------------------------------------------------------------- */

int64_t joy_hats( void ) {
    if ( _selected_joystick >= 0 && _selected_joystick < _max_joys ) {
        return SDL_JoystickNumHats( _joysticks[ _selected_joystick ] );
    }
    return 0;
}

/* --------------------------------------------------------------------------- */
/* joy_balls ()                                                             */
/* Returns the total number of balls of the current selected joystick          */
/* --------------------------------------------------------------------------- */

int64_t joy_balls( void ) {
    if ( _selected_joystick >= 0 && _selected_joystick < _max_joys ) {
        return SDL_JoystickNumBalls( _joysticks[ _selected_joystick ] );
    }
    return 0;
}

/* --------------------------------------------------------------------------- */
/* joy_get_hat (int64_t HAT)                                                    */
/* Returns the state of the specfied hat on the current selected joystick      */
/* --------------------------------------------------------------------------- */

int64_t joy_get_hat( int64_t hat ) {
    if ( _selected_joystick >= 0 && _selected_joystick < _max_joys ) {
        if ( hat >= 0 && hat <= SDL_JoystickNumHats( _joysticks[ _selected_joystick ] ) ) {
            return SDL_JoystickGetHat( _joysticks[ _selected_joystick ], hat );
        }
    }
    return 0;
}

/* --------------------------------------------------------------------------- */
/* joy_get_ball (int64_t BALL, int64_t* dx, int64_t* dy)                                */
/* Returns the state of the specfied ball on the current selected joystick     */
/* --------------------------------------------------------------------------- */

int64_t joy_get_ball( int64_t ball, int64_t * dx, int64_t * dy ) {
    if ( _selected_joystick >= 0 && _selected_joystick < _max_joys ) {
        if ( ball >= 0 && ball <= SDL_JoystickNumBalls( _joysticks[ball] ) ) {
            int _dx, _dy;
            int64_t r = SDL_JoystickGetBall( _joysticks[ _selected_joystick ], ball, &_dx, &_dy );
            * dx = ( int64_t ) _dx;
            * dy = ( int64_t ) _dy;
            return  r;
        }
    }
    return -1;
}

/* --------------------------------------------------------------------------- */

int64_t joy_get_accel( int64_t * x, int64_t * y, int64_t * z ) {
#ifdef TARGET_CAANOO
    if ( _selected_joystick == 0 ) {
        KIONIX_ACCEL_read_LPF_g( x, y, z );
    }
    return 0;
#else
    return -1;
#endif
}

/* --------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/* joy_buttons_specific (int64_t JOY)                                           */
/* Returns the selected joystick total buttons                                 */
/* --------------------------------------------------------------------------- */

int64_t joy_buttons_specific( int64_t joy ) {
    if ( joy >= 0 && joy < _max_joys ) {
#ifdef TARGET_CAANOO
        if ( joy == 0 ) return 21;
#endif
        return SDL_JoystickNumButtons( _joysticks[ joy ] );
    }
    return 0;
}

/* --------------------------------------------------------------------------- */
/* joy_axes_specific (int64_t JOY)                                              */
/* Returns the selected joystick total axes                                    */
/* --------------------------------------------------------------------------- */

int64_t joy_axes_specific( int64_t joy ) {
    if ( joy >= 0 && joy < _max_joys ) {
        return SDL_JoystickNumAxes( _joysticks[ joy ] );
    }
    return 0;
}

/* --------------------------------------------------------------------------- */
/* joy_get_button_specific (int64_t JOY, int64_t button)                            */
/* Returns the selected joystick state for the given button                    */
/* --------------------------------------------------------------------------- */

int64_t joy_get_button_specific( int64_t joy, int64_t button ) {
    if ( joy >= 0 && joy < _max_joys ) {
#ifdef TARGET_CAANOO
        if ( button >= 0 && ( ( joy == 0 && button <= 21 ) || ( joy != 0 && SDL_JoystickNumButtons( _joysticks[ joy ] ) ) ) )
#else
        if ( button >= 0 && button <= SDL_JoystickNumButtons( _joysticks[ joy ] ) )
#endif
        {
#ifdef TARGET_CAANOO
            if ( joy == 0 ) {
                int64_t vax;

                switch ( button ) {
                    case    1: /* UPLF                  */  return ( SDL_JoystickGetAxis( _joysticks[ 0 ], 1 ) < -16384 && SDL_JoystickGetAxis( _joysticks[ 0 ], 0 ) < -16384 );
                    case    3: /* DWLF                  */  return ( SDL_JoystickGetAxis( _joysticks[ 0 ], 1 ) >  16384 && SDL_JoystickGetAxis( _joysticks[ 0 ], 0 ) < -16384 );
                    case    5: /* DWRT                  */  return ( SDL_JoystickGetAxis( _joysticks[ 0 ], 1 ) >  16384 && SDL_JoystickGetAxis( _joysticks[ 0 ], 0 ) >  16384 );
                    case    7: /* UPRT                  */  return ( SDL_JoystickGetAxis( _joysticks[ 0 ], 1 ) < -16384 && SDL_JoystickGetAxis( _joysticks[ 0 ], 0 ) >  16384 );
                    case    0: /* UP                    */  vax = SDL_JoystickGetAxis( _joysticks[ 0 ], 0 ); return ( SDL_JoystickGetAxis( _joysticks[ 0 ], 1 ) < -16384 && ABS( vax ) < 16384 );
                    case    4: /* DW                    */  vax = SDL_JoystickGetAxis( _joysticks[ 0 ], 0 ); return ( SDL_JoystickGetAxis( _joysticks[ 0 ], 1 ) >  16384 && ABS( vax ) < 16384 );
                    case    2: /* LF                    */  vax = SDL_JoystickGetAxis( _joysticks[ 0 ], 1 ); return ( SDL_JoystickGetAxis( _joysticks[ 0 ], 0 ) < -16384 && ABS( vax ) < 16384 );
                    case    6: /* RT                    */  vax = SDL_JoystickGetAxis( _joysticks[ 0 ], 1 ); return ( SDL_JoystickGetAxis( _joysticks[ 0 ], 0 ) >  16384 && ABS( vax ) < 16384 );

                    case    8:  /* MENU->HOME           */  return ( SDL_JoystickGetButton( _joysticks[ 0 ], 6 ) );
                    case    9:  /* SELECT->HELP-II      */  return ( SDL_JoystickGetButton( _joysticks[ 0 ], 9 ) );
                    case    10: /* L                    */  return ( SDL_JoystickGetButton( _joysticks[ 0 ], 4 ) );
                    case    11: /* R                    */  return ( SDL_JoystickGetButton( _joysticks[ 0 ], 5 ) );
                    case    12: /* A                    */  return ( SDL_JoystickGetButton( _joysticks[ 0 ], 0 ) );
                    case    13: /* B                    */  return ( SDL_JoystickGetButton( _joysticks[ 0 ], 2 ) );
                    case    14: /* X                    */  return ( SDL_JoystickGetButton( _joysticks[ 0 ], 1 ) );
                    case    15: /* Y                    */  return ( SDL_JoystickGetButton( _joysticks[ 0 ], 3 ) );
                    case    16: /* VOLUP                */  return ( 0 );
                    case    17: /* VOLDOWN              */  return ( 0 );
                    case    18: /* CLICK                */  return ( SDL_JoystickGetButton( _joysticks[ 0 ], 10 ) );
                    case    19: /* POWER-LOCK  (CAANOO) */  return ( SDL_JoystickGetButton( _joysticks[ 0 ], 7 ) ); /* Only Caanoo */
                    case    20: /* HELP-I      (CAANOO) */  return ( SDL_JoystickGetButton( _joysticks[ 0 ], 8 ) ); /* Only Caanoo */
                    default:                                return ( 0 );
                }
            }
#endif
            return SDL_JoystickGetButton( _joysticks[ joy ], button );
        }
    }
    return 0;
}

/* --------------------------------------------------------------------------- */
/* joy_get_position_specific (int64_t JOY, int64_t axis)                            */
/* Returns the selected joystick state for the given axis                      */
/* --------------------------------------------------------------------------- */

int64_t joy_get_position_specific( int64_t joy, int64_t axis ) {
    if ( joy >= 0 && joy < _max_joys ) {
        if ( axis >= 0 && axis <= SDL_JoystickNumAxes( _joysticks[ joy ] ) ) {
            return SDL_JoystickGetAxis( _joysticks[ joy ], axis );
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
    if ( joy >= 0 && joy < _max_joys ) {
        return SDL_JoystickNumHats( _joysticks[ joy ] );
    }
    return 0;
}

/* --------------------------------------------------------------------------- */
/* joy_balls_specific (int64_t JOY)                                             */
/* Returns the total number of balls of the specified joystick                 */
/* --------------------------------------------------------------------------- */

int64_t joy_balls_specific( int64_t joy ) {
    if ( joy >= 0 && joy < _max_joys ) {
        return SDL_JoystickNumBalls( _joysticks[ joy ] );
    }
    return 0;
}

/* --------------------------------------------------------------------------- */
/* joy_get_hat_specific (int64_t JOY, int64_t HAT)                                  */
/* Returns the state of the specfied hat on the specified joystick             */
/* --------------------------------------------------------------------------- */

int64_t joy_get_hat_specific( int64_t joy, int64_t hat ) {
    if ( joy >= 0 && joy < _max_joys ) {
        if ( hat >= 0 && hat <= SDL_JoystickNumHats( _joysticks[ joy ] ) ) {
            return SDL_JoystickGetHat( _joysticks[ joy ], hat );
        }
    }
    return 0;
}

/* --------------------------------------------------------------------------- */
/* joy_get_ball_specific (int64_t JOY, int64_t BALL, int64_t* dx, int64_t* dy)              */
/* Returns the state of the specfied ball on the specified joystick            */
/* --------------------------------------------------------------------------- */

int64_t joy_get_ball_specific( int64_t joy, int64_t ball, int64_t * dx, int64_t * dy ) {
    if ( joy >= 0 && joy < _max_joys ) {
        if ( ball >= 0 && ball <= SDL_JoystickNumBalls( _joysticks[ joy ] ) ) {
            int _dx, _dy;
            int64_t r = SDL_JoystickGetBall( _joysticks[ joy ], ball, &_dx, &_dy );
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
/* Funciones de inicializacion del modulo/plugin                               */
/* --------------------------------------------------------------------------- */

void joy_init() {
    int64_t i;

    if ( !SDL_WasInit( SDL_INIT_JOYSTICK ) ) {
        SDL_InitSubSystem( SDL_INIT_JOYSTICK );
        SDL_JoystickEventState( SDL_ENABLE );
    }

    /* Open all joysticks */
    if (( _max_joys = SDL_NumJoysticks() ) > MAX_JOYS ) {
        printf( "[JOY] Warning: maximum number of joysticks exceeded (%i>%i)", ( int ) _max_joys, MAX_JOYS );
        _max_joys = MAX_JOYS;
    }

    for ( i = 0; i < _max_joys; i++ ) {
        _joysticks[i] = SDL_JoystickOpen( i );
        if ( !_joysticks[ i ] ) printf( "[JOY] Failed to open joystick '%i'", ( int ) i );
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

void joy_exit() {
    int64_t i;

    #ifdef TARGET_CAANOO
    KIONIX_ACCEL_deinit();
    #endif

    for ( i = 0; i < _max_joys; i++ )
        if ( _joysticks[ i ] ) SDL_JoystickClose( _joysticks[ i ] );

    if ( SDL_WasInit( SDL_INIT_JOYSTICK ) ) SDL_QuitSubSystem( SDL_INIT_JOYSTICK );
}

/* ----------------------------------------------------------------- */
