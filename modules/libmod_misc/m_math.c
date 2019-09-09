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

#define _GNU_SOURCE

#include "fmath.h"

#include <math.h>
#include <stdio.h>

#include "bgddl.h"
#include "libmod_misc.h"

/* --------------------------------------------------------------------------- */
/* Funciones matemáticas */
/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_abs( INSTANCE * my, int64_t * params ) {
    double num = *( double * ) &params[0];
    double res = ( num < 0 ) ? -num : num;
    return *(( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_pow( INSTANCE * my, int64_t * params ) {
    double res = pow( *( double * ) &params[0], *( double * ) &params[1] );
    return *(( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_sqrt( INSTANCE * my, int64_t * params ) {
    double res = sqrt( *( double * ) &params[0] );
    return *(( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_cos( INSTANCE * my, int64_t * params ) {
    double res = cos_deg( *( double * ) &params[0] );
    return *(( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_sin( INSTANCE * my, int64_t * params ) {
    double res = sin_deg( *( double * ) &params[0] );
    return *(( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_tan( INSTANCE * my, int64_t * params ) {
    double param = *( double * ) &params[0];
    double res = tan( param * M_PI / 180000.0 );
    return *(( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_acos( INSTANCE * my, int64_t * params )
{
    double param = *( double * ) &params[0];
    double res = acos(( double )param ) * 180000.0 / M_PI;
    return *(( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_asin( INSTANCE * my, int64_t * params ) {
    double param = *( double * ) &params[0];
    double res = asin(( double )param ) * 180000.0 / M_PI;
    return *(( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_atan( INSTANCE * my, int64_t * params ) {
    double param = *( double * ) &params[0];
    double res = atan(( double )param ) * 180000.0 / M_PI;
    return *(( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_atan2( INSTANCE * my, int64_t * params ) {
    double param1 = *( double * ) &params[0],
           param2 = *( double * ) &params[1];
    double res = atan2(( double )param1, ( double )param2 ) * 180000.0 / M_PI;
    return *(( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_isinf( INSTANCE * my, int64_t * params ) {
    double param = *( double * ) &params[0];
    return isinf( param );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_isnan( INSTANCE * my, int64_t * params ) {
    double param = *( double * ) &params[0];
    return isnan( param );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_finite( INSTANCE * my, int64_t * params ) {
    double param = *( double * ) &params[0];
    return finite ( param );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_fget_angle( INSTANCE * my, int64_t * params ) {
    double dx = *( double * ) &params[2] - *( double * ) &params[0];
    double dy = *( double * ) &params[3] - *( double * ) &params[1];
    int64_t angle;

    if ( dx == 0 ) return ( dy > 0 ) ? 270000L : 90000L;

    angle = ( int64_t )( atan( dy / dx ) * 180000.0 / M_PI );

    return ( dx > 0 ) ? -angle : -angle + 180000L;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_fget_dist( INSTANCE * my, int64_t * params ) {
    double dx = *( double * ) &params[2] - *( double * ) &params[0];
    double dy = *( double * ) &params[3] - *( double * ) &params[1];
    double res = sqrt( dx*dx + dy*dy );
    return *( int64_t * ) &res;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_near_angle( INSTANCE * my, int64_t * params ) {
    int64_t angle = params[0],
            dest  = params[1],
            incr  = params[2];

    if ( angle < dest && dest - angle > 180000L ) angle += 360000L;
    if ( angle > dest && angle - dest > 180000L ) angle -= 360000L;

    if ( angle < dest ) {
        angle += incr;
        if ( angle > dest ) angle = dest;
    } else {
        angle -= incr;
        if ( angle < dest ) angle = dest;
    }

    if ( angle < 0 ) return angle + 360000L;
    if ( angle >= 360000L ) return angle - 360000L;
    return angle;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_get_distx( INSTANCE * my, int64_t * params ) {
    double res = ( *( double * ) &params[1] * cos_deg( params[0] ) );
    return *( int64_t * ) &res;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_get_disty( INSTANCE * my, int64_t * params ) {
    double res = ( *( double * ) &params[1] * -sin_deg( params[0] ) );
    return *( int64_t * ) &res;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_clamp( INSTANCE * my, int64_t * params ) {
    int64_t value = params[0], min = params[1], max = params[2];
    return ( value < min ? min : value > max ? max : value );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_clampf( INSTANCE * my, int64_t * params ) {
    double value = *( double * ) &params[0], min = *( double * ) &params[1], max = *( double * ) &params[2];
    double res = ( value < min ? min : value > max ? max : value );
    return * (( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_max( INSTANCE * my, int64_t * params ) {
    double a = *( double * ) &params[0], b = *( double * ) &params[1];
    double res = a > b ? a : b > a ? b : a;
    return * (( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_min( INSTANCE * my, int64_t * params ) {
    double a = *( double * ) &params[0], b = *( double * ) &params[1];
    double res = a < b ? a : b < a ? b : a;
    return * (( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_sgn( INSTANCE * my, int64_t * params ) {
    double a = *( double * ) &params[0];
    double res = a < 0.0 ? -1.0 : a > 0.0 ? 1.0 : 0.0;
    return * (( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_sgn2( INSTANCE * my, int64_t * params ) {
    return params[0] < 0 ? -1 : params[0] > 0 ? 1 : 0;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_round( INSTANCE * my, int64_t * params ) {
    double a = *( double * ) &params[0];
    double res = round( a );
    return * (( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_floor( INSTANCE * my, int64_t * params ) {
    double a = *( double * ) &params[0];
    double res = floor( a );
    return * (( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_ceil( INSTANCE * my, int64_t * params ) {
    double a = *( double * ) &params[0];
    double res = ceil( a );
    return * (( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_trunc( INSTANCE * my, int64_t * params ) {
    double a = *( double * ) &params[0];
    double res = trunc( a );
    return * (( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_frac( INSTANCE * my, int64_t * params ) {
    double a = *( double * ) &params[0];
    double res = a - floor( a );
    return * (( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_decimal( INSTANCE * my, int64_t * params ) {
    double a = *( double * ) &params[0];
    double res = a - trunc( a );
    return * (( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */
