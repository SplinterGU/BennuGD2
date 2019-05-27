/*
 *  Copyright (C) 2006-2016 SplinterGU (Fenix/BennuGD)
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

#ifndef _FMATH_H
#define _FMATH_H

#include <stdint.h>
#include <math.h>

#include "bgdcore.h"

/* Rutinas matemáticas de punto fijo, basadas en Allegro */

#ifndef M_PI
#define M_PI        3.14159265358979323846  /* PI */
#endif

#ifndef ABS
#define ABS(x)  (((x) < 0) ? -(x):(x))
#endif
#ifndef SGN
#define SGN(a)  (((a) < 0) ? -1 : !(a) ? 0 : 1)
#endif

typedef int64_t fixed ;

extern fixed ftofix( double x );
extern double fixtof( fixed x );
extern fixed itofix( int64_t x );
extern int64_t fixtoi( fixed x );
extern int64_t fixceil( fixed x );
extern fixed fixcos( int64_t x );
extern fixed fixsin( int64_t x );
extern fixed fixmul( int64_t x, int64_t y );
extern fixed fixdiv( int64_t x, int64_t y );
extern void init_cos_tables();

#endif
