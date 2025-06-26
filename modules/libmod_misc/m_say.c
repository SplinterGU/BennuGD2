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

#include "bgddl.h"
#include "xstrings.h"

#include "libmod_misc.h"

#ifdef __ANDROID__
#include "jni.h"
#include <android/log.h>
#define LOG_TAG "bgdi-native"
#endif

/* ---------------------------------------------------------------------- */

int64_t libmod_misc_say_say( INSTANCE * my, int64_t * params ) {
    /* Show debugging info also in stdout */
#ifdef __ANDROID__
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "%s", string_get( params[0] ) );
#else
    printf( "%s\n", string_get( params[0] ) );
    fflush( stdout );
#endif
    string_discard( params[0] ) ;
    return 1 ;
}

/* ---------------------------------------------------------------------- */

int64_t libmod_misc_say_say_fast( INSTANCE * my, int64_t * params ) {
    /* Show debugging info also in stdout */
#ifdef __ANDROID__
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "%s", string_get( params[0] ) );
#else
    printf( "%s\n", string_get( params[0] ) );
#endif
    string_discard( params[0] ) ;
    return 1 ;
}

/* ----------------------------------------------------------------- */
