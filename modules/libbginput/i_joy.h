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

#ifndef __I_JOY_H
    #define __I_JOY_H

    #define JOY_BUTTON_A                ( 0x100 + SDL_CONTROLLER_BUTTON_A )
    #define JOY_BUTTON_B                ( 0x100 + SDL_CONTROLLER_BUTTON_B )
    #define JOY_BUTTON_X                ( 0x100 + SDL_CONTROLLER_BUTTON_X )
    #define JOY_BUTTON_Y                ( 0x100 + SDL_CONTROLLER_BUTTON_Y )
    #define JOY_BUTTON_BACK             ( 0x100 + SDL_CONTROLLER_BUTTON_BACK )
    #define JOY_BUTTON_GUIDE            ( 0x100 + SDL_CONTROLLER_BUTTON_GUIDE )
    #define JOY_BUTTON_START            ( 0x100 + SDL_CONTROLLER_BUTTON_START )
    #define JOY_BUTTON_LEFTSTICK        ( 0x100 + SDL_CONTROLLER_BUTTON_LEFTSTICK )
    #define JOY_BUTTON_RIGHTSTICK       ( 0x100 + SDL_CONTROLLER_BUTTON_RIGHTSTICK )
    #define JOY_BUTTON_LEFTSHOULDER     ( 0x100 + SDL_CONTROLLER_BUTTON_LEFTSHOULDER )
    #define JOY_BUTTON_RIGHTSHOULDER    ( 0x100 + SDL_CONTROLLER_BUTTON_RIGHTSHOULDER )
    #define JOY_BUTTON_DPAD_UP          ( 0x100 + SDL_CONTROLLER_BUTTON_DPAD_UP )
    #define JOY_BUTTON_DPAD_DOWN        ( 0x100 + SDL_CONTROLLER_BUTTON_DPAD_DOWN )
    #define JOY_BUTTON_DPAD_LEFT        ( 0x100 + SDL_CONTROLLER_BUTTON_DPAD_LEFT )
    #define JOY_BUTTON_DPAD_RIGHT       ( 0x100 + SDL_CONTROLLER_BUTTON_DPAD_RIGHT )
    #define JOY_BUTTON_MISC1            ( 0x100 + SDL_CONTROLLER_BUTTON_MISC1 )    /* Xbox Series X share button PS5 microphone button Nintendo Switch Pro capture button Amazon Luna microphone button */
    #define JOY_BUTTON_PADDLE1          ( 0x100 + SDL_CONTROLLER_BUTTON_PADDLE1 )  /* Xbox Elite paddle P1 (upper left facing the back) */
    #define JOY_BUTTON_PADDLE2          ( 0x100 + SDL_CONTROLLER_BUTTON_PADDLE2 )  /* Xbox Elite paddle P3 (upper right facing the back) */
    #define JOY_BUTTON_PADDLE3          ( 0x100 + SDL_CONTROLLER_BUTTON_PADDLE3 )  /* Xbox Elite paddle P2 (lower left facing the back) */
    #define JOY_BUTTON_PADDLE4          ( 0x100 + SDL_CONTROLLER_BUTTON_PADDLE4 )  /* Xbox Elite paddle P4 (lower right facing the back) */
    #define JOY_BUTTON_TOUCHPAD         ( 0x100 + SDL_CONTROLLER_BUTTON_TOUCHPAD ) /* PS4/PS5 touchpad button */
    #define JOY_BUTTON_MAX              ( 0x100 + SDL_CONTROLLER_BUTTON_MAX )

    #define JOY_AXIS_LEFTX              ( 0x100 + SDL_CONTROLLER_BUTTON_MAX + SDL_CONTROLLER_AXIS_LEFTX )
    #define JOY_AXIS_LEFTY              ( 0x100 + SDL_CONTROLLER_BUTTON_MAX + SDL_CONTROLLER_AXIS_LEFTY )
    #define JOY_AXIS_RIGHTX             ( 0x100 + SDL_CONTROLLER_BUTTON_MAX + SDL_CONTROLLER_AXIS_RIGHTX )
    #define JOY_AXIS_RIGHTY             ( 0x100 + SDL_CONTROLLER_BUTTON_MAX + SDL_CONTROLLER_AXIS_RIGHTY )
    #define JOY_AXIS_TRIGGERLEFT        ( 0x100 + SDL_CONTROLLER_BUTTON_MAX + SDL_CONTROLLER_AXIS_TRIGGERLEFT )
    #define JOY_AXIS_TRIGGERRIGHT       ( 0x100 + SDL_CONTROLLER_BUTTON_MAX + SDL_CONTROLLER_AXIS_TRIGGERRIGHT )
    #define JOY_AXIS_MAX                ( 0x100 + SDL_CONTROLLER_BUTTON_MAX + SDL_CONTROLLER_AXIS_MAX )

    extern int64_t joy_num( void );
    extern int64_t joy_name();
    extern int64_t joy_select( int64_t joy );
    extern int64_t joy_buttons( void );
    extern int64_t joy_axes( void );
    extern int64_t joy_get_button( int64_t button );
    extern int64_t joy_get_position( int64_t axis );
    extern int64_t joy_hats( void );
    extern int64_t joy_balls( void );
    extern int64_t joy_get_hat( int64_t hat );
    extern int64_t joy_get_ball( int64_t ball, int64_t * dx, int64_t * dy );
    extern int64_t joy_powerlevel();
    extern int64_t joy_is_attached();

    extern int64_t joy_name_specific( int64_t joy );
    extern int64_t joy_buttons_specific( int64_t joy );
    extern int64_t joy_axes_specific( int64_t joy );
    extern int64_t joy_get_button_specific( int64_t joy, int64_t button );
    extern int64_t joy_get_position_specific( int64_t joy, int64_t axis );
    extern int64_t joy_hats_specific( int64_t joy );
    extern int64_t joy_balls_specific( int64_t joy );
    extern int64_t joy_get_hat_specific( int64_t joy, int64_t hat );
    extern int64_t joy_get_ball_specific( int64_t joy, int64_t ball, int64_t * dx, int64_t * dy );
    extern int64_t joy_get_accel( int64_t * x, int64_t * y, int64_t * z );
    extern int64_t joy_get_accel_specific( int64_t joy, int64_t * x, int64_t * y, int64_t * z );
    extern int64_t joy_powerlevel_specific( int64_t joy );
    extern int64_t joy_is_attached_specific( int64_t joy );

    extern void process_joy_events();

    extern void joy_init();
    extern void joy_exit();

#endif
