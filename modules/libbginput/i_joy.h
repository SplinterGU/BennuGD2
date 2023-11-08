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
    extern int64_t joy_powerlevel();

    extern void joy_init();
    extern void joy_exit();

    extern void joy_handler();

#endif
