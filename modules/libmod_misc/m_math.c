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

#define _GNU_SOURCE

#include "fmath.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "bgddl.h"
#include "libmod_misc.h"

/* --------------------------------------------------------------------------- */

#define DBL_EPSILON 0.000001f

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
//    double res = sin_deg( *( double * ) &params[0] )/cos_deg( *( double * ) &params[0] );
    double param = *( double * ) &params[0];
    double res = tan( param * M_PI / 180000.0 );
    return *(( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_acos( INSTANCE * my, int64_t * params ) {
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
    return finite( param );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_fget_angle( INSTANCE * my, int64_t * params ) {
    double dx = *( double * ) &params[2] - *( double * ) &params[0];
    double dy = *( double * ) &params[3] - *( double * ) &params[1];
    int64_t angle;

    if ( dx == 0 ) return ( dy > 0 ) ? 270000L : 90000L;

    angle = ( int64_t )( atan( dy / dx ) * 180000.0 / M_PI );

    return ( dx > 0 ) ? ( dy < 0 ) ? -angle : 360000L - angle : 180000L - angle;
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
    double value = *( double * ) &params[0], v1 = *( double * ) &params[1], v2 = *( double * ) &params[2];
    double res = ( value < v1 ? v1 : value > v2 ? v2 : value );
    return * (( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_between( INSTANCE * my, int64_t * params ) {
    double value = *( double * ) &params[0], lim1 = *( double * ) &params[1], lim2 = *( double * ) &params[2];
    return ( value >= MIN(lim1,lim2) && value <= MAX(lim1,lim2) );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_min( INSTANCE * my, int64_t * params ) {
    double param1 = *( double * ) &params[0], param2 = *( double * ) &params[1];
    double res = param1 < param2 ? param1 : param2;
    return * (( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_max( INSTANCE * my, int64_t * params ) {
    double param1 = *( double * ) &params[0], param2 = *( double * ) &params[1];
    double res = param1 > param2 ? param1 : param2;
    return * (( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_sgn( INSTANCE * my, int64_t * params ) {
    double param = *( double * ) &params[0];
    double res = param < 0.0 ? -1.0 : param > 0.0 ? 1.0 : 0.0;
    return * (( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_sgn2( INSTANCE * my, int64_t * params ) {
    return params[0] < 0 ? -1 : params[0] > 0 ? 1 : 0;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_round( INSTANCE * my, int64_t * params ) {
    double param = *( double * ) &params[0];
    double res = round( param );
    return * (( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_floor( INSTANCE * my, int64_t * params ) {
    double param = *( double * ) &params[0];
    double res = floor( param );
    return * (( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_ceil( INSTANCE * my, int64_t * params ) {
    double param = *( double * ) &params[0];
    double res = ceil( param );
    return * (( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_trunc( INSTANCE * my, int64_t * params ) {
    double param = *( double * ) &params[0];
    double res = trunc( param );
    return * (( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_frac( INSTANCE * my, int64_t * params ) {
    double param = *( double * ) &params[0];
    double res = param - floor( param );
    return * (( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_decimal( INSTANCE * my, int64_t * params ) {
    double param = *( double * ) &params[0];
    double res = param - trunc( param );
    return * (( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */

/*
    ecuacion de la recta

    "y = m*X + base"

    m1 = ( y2 - y1 ) / ( x2 - x1 )
    m2 = ( y4 - y3 ) / ( x4 - x3 )

    b1 = y1 - x1 * m1
    b2 = y3 - x3 * m2

    y = b1 + m1 * x
    y = b2 + m2 * x

    entonces

    b1 + m1 * x = b2 + m2 * x

    m1 * x - m2 * x = b2 - b1
    ( m1 - m2 ) * x = b2 - b1

    final:

    x0 = ( b2 - b1 ) / ( m1 - m2 )
    y0 = b1 + m1 * x0
*/

int64_t libmod_misc_math_intersect( INSTANCE * my, int64_t * params ) {
    double  x1 = *( double * ) &params[0],
            y1 = *( double * ) &params[1],
            x2 = *( double * ) &params[2],
            y2 = *( double * ) &params[3],
            x3 = *( double * ) &params[4],
            y3 = *( double * ) &params[5],
            x4 = *( double * ) &params[6],
            y4 = *( double * ) &params[7];

    double m1, m2, b1, b2, x0, y0;

    if ( x1 == x2 && x3 == x4 ) return 0;

    m1 = ( y2 - y1 ) / ( x2 - x1 );
    m2 = ( y4 - y3 ) / ( x4 - x3 );

    b1 = y1 - x1 * m1;
    b2 = y3 - x3 * m2;

    if ( x1 == x2 ) {
        x0 = x1;
        y0 = b2 + m2 * x0;
    } else if ( x3 == x4 ) {
        x0 = x3;
        y0 = b1 + m1 * x0;
    } else {
        x0 = ( b2 - b1 ) / ( m1 - m2 );
        y0 = b1 + m1 * x0;
    }

    if ( !( x0 > MAX(x1,x2) + DBL_EPSILON || x0 < MIN(x1,x2) - DBL_EPSILON || y0 > MAX(y1,y2) + DBL_EPSILON || y0 + 0.01f < MIN(y1,y2) - DBL_EPSILON ||
            x0 > MAX(x3,x4) + DBL_EPSILON || x0 < MIN(x3,x4) - DBL_EPSILON || y0 > MAX(y3,y4) + DBL_EPSILON || y0 + 0.01f < MIN(y3,y4) - DBL_EPSILON ) ) {
        * ( int64_t * ) params[8] = * ( int64_t * ) &x0;
        * ( int64_t * ) params[9] = * ( int64_t * ) &y0;
        return 1;
    }

    return 0;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_intersect_line_circle( INSTANCE * my, int64_t * params ) {
    double  x1 = *( double * ) &params[0],
            y1 = *( double * ) &params[1],
            x2 = *( double * ) &params[2],
            y2 = *( double * ) &params[3],
            cx = *( double * ) &params[4],
            cy = *( double * ) &params[5],
            r = *( double * ) &params[6];

    double m1, b1, x0_1, y0_1, x0_2, y0_2;
    double min_x, min_y, max_x, max_y;

    m1 = ( y2 - y1 ) / ( x2 - x1 );
    b1 = y1 - x1 * m1;

    if ( x1 == x2 ) {
        x0_1 = x1;
        x0_2 = x1;
        y0_1 = cy + sqrt( r * r - ( x0_1 - cx ) * ( x0_1 - cx ) );
        y0_2 = cy - sqrt( r * r - ( x0_2 - cx ) * ( x0_2 - cx ) );
    } else {
        x0_1 = ( cx + sqrt( m1 * ( cx * ( ( cy + cy ) - ( b1 + b1 ) - cx * m1 ) + m1 * r * r ) - ( cy - b1 ) * ( cy - b1 ) + r * r ) + m1 * ( cy - b1 ) ) / ( m1 * m1 + 1 );
        x0_2 = ( cx - sqrt( m1 * ( cx * ( ( cy + cy ) - ( b1 + b1 ) - cx * m1 ) + m1 * r * r ) - ( cy - b1 ) * ( cy - b1 ) + r * r ) + m1 * ( cy - b1 ) ) / ( m1 * m1 + 1 );
        y0_1 = b1 + m1 * x0_1;
        y0_2 = b1 + m1 * x0_2;
    }

    int nret = 0;

    min_x = MIN(x1,x2) - DBL_EPSILON;
    min_y = MIN(y1,y2) - DBL_EPSILON;
    max_x = MAX(x1,x2) + DBL_EPSILON;
    max_y = MAX(y1,y2) + DBL_EPSILON;

    if ( finite(x0_1) && finite(y0_1) && !( x0_1 > max_x || x0_1 < min_x || y0_1 > max_y || y0_1 < min_y ) ) {
        * ( int64_t * ) params[7] = * ( int64_t * ) &x0_1;
        * ( int64_t * ) params[8] = * ( int64_t * ) &y0_1;
        nret++;
    }

    if ( finite(x0_2) && finite(y0_2) && !( x0_2 > max_x || x0_2 < min_x || y0_2 > max_y || y0_2 < min_y ) ) {
        if ( nret ) {
            * ( int64_t * ) params[9]  = * ( int64_t * ) &x0_2;
            * ( int64_t * ) params[10] = * ( int64_t * ) &y0_2;
        } else {
            * ( int64_t * ) params[7] = * ( int64_t * ) &x0_2;
            * ( int64_t * ) params[8] = * ( int64_t * ) &y0_2;
        }
        nret++;
    }
    return nret;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_intersect_circle( INSTANCE * my, int64_t * params ) {
    double  cx1 = *( double * ) &params[0],
            cy1 = *( double * ) &params[1],
            cx2 = *( double * ) &params[2],
            cy2 = *( double * ) &params[3],
            r1 = *( double * ) &params[4],
            r2 = *( double * ) &params[5];

    double acos_a, x0, y0;

    double dx = cx2 - cx1;
    double dy = cy2 - cy1;

    // get distance
    double dist = sqrt( dx * dx + dy * dy );

    // get angle
    int64_t angle;

    if ( dx == 0.0 ) angle = ( dy > 0.0 ) ? 270000L : 90000L;
    else {
        angle = ( int64_t )( atan( dy / dx ) * 180000.0 / M_PI );
        angle = ( dx > 0.0 ) ? ( dy < 0.0 ) ? -angle : 360000L - angle : 180000L - angle;
    }

    acos_a = acos( ( r1 * r1 + dist * dist - r2 * r2 ) / ( ( r1 + r1 ) * dist ) );

    if ( finite( acos_a ) ) {
        acos_a *= 180000.0 / M_PI;

        x0 = cx1 + cos_deg( angle + acos_a ) * r1;
        y0 = cy1 - sin_deg( angle + acos_a ) * r1;
        * ( int64_t * ) params[6] = * ( int64_t * ) &x0;
        * ( int64_t * ) params[7] = * ( int64_t * ) &y0;

        if ( acos_a == 0.0 ) return 1;

        x0 = cx1 + cos_deg( angle - acos_a ) * r1;
        y0 = cy1 - sin_deg( angle - acos_a ) * r1;
        * ( int64_t * ) params[8] = * ( int64_t * ) &x0;
        * ( int64_t * ) params[9] = * ( int64_t * ) &y0;

        return 2;
    }

    return 0;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_normal_projection( INSTANCE * my, int64_t * params ) {
    double  x1 = *( double * ) &params[0],
            y1 = *( double * ) &params[1],
            x2 = *( double * ) &params[2],
            y2 = *( double * ) &params[3],
            px = *( double * ) &params[4],
            py = *( double * ) &params[5];

    double m1, m2, b1, b2, x0, y0 = y1;

    if ( x1 == x2 ) {
        x0 = px;
        y0 = y1;
    } else if ( y1 == y2 ) {
        x0 = x1;
    } else {
        m1 = ( y2 - y1 ) / ( x2 - x1 );
        m2 = -1.0 / m1;
        b1 = y1 - x1 * m2;
        b2 = py - px * m1;
        x0 = ( b2 - b1 ) / ( m2 - m1 );
        y0 = b2 + m1 * x0;
    }

    * ( int64_t * ) ( params[6] ) = * ( int64_t * ) &x0;
    * ( int64_t * ) ( params[7] ) = * ( int64_t * ) &y0;
/*
    double dx =   ( px - x0 ) * ( px - x0 ) + ( py - y0 ) * ( py - y0 ),
           s  = ( ( px - x0 ) + ( py - y0 ) ) < 0 ? -1 : 1;

    double dist = sqrt( dx ) * s;
*/
    double dist = sqrt( ( px - x0 ) * ( px - x0 ) + ( py - y0 ) * ( py - y0 ) );

    return * ( int64_t * ) &dist;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_math_orthogonal_projection( INSTANCE * my, int64_t * params ) {
    double  x1 = *( double * ) &params[0],
            y1 = *( double * ) &params[1],
            x2 = *( double * ) &params[2],
            y2 = *( double * ) &params[3],
            px = *( double * ) &params[4],
            py = *( double * ) &params[5];

    double m1, m2, b1, b2, x0, y0;

    if ( x1 == x2 ) {
        x0 = x1;
        y0 = py;
    } else if ( y1 == y2 ) {
        x0 = px;
        y0 = y1;
    } else {
        m1 = ( y2 - y1 ) / ( x2 - x1 );
        m2 = -1.0 / m1;
        b1 = y1 - x1 * m1;
        b2 = py - px * m2;
        x0 = ( b2 - b1 ) / ( m1 - m2 );
        y0 = b2 + m2 * x0;
    }

    * ( int64_t * ) params[6] = * ( int64_t * ) &x0;
    * ( int64_t * ) params[7] = * ( int64_t * ) &y0;
/*
    double dx =   ( px - x0 ) * ( px - x0 ) + ( py - y0 ) * ( py - y0 ),
           s  = ( ( px - x0 ) + ( py - y0 ) ) < 0 ? -1 : 1;

    double dist = sqrt( dx ) * s;
*/
    double dist = sqrt( ( px - x0 ) * ( px - x0 ) + ( py - y0 ) * ( py - y0 ) );

    return * ( int64_t * ) &dist;
}

/* --------------------------------------------------------------------------- */
