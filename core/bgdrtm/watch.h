/*
 *  Copyright (C) 2019 SplinterGU (Fenix/BennuGD)
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

#ifndef __WATCH_H
#define __WATCH_H

/* --------------------------------------------------------------------------- */

typedef struct {
    int size;
    void * original;
    void * verify;
} watch;

/* --------------------------------------------------------------------------- */

extern watch * watch_create( void * element, int size );
extern void watch_destroy( watch * t );
extern int watch_reset( watch * t, void * element );
extern int watch_set_default( watch * t, void * element );
extern int watch_set( watch * t, void * element, void * value );
extern int watch_test( watch * t, void * element );

/* --------------------------------------------------------------------------- */

#endif
