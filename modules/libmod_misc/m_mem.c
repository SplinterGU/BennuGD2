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

#include <stdint.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* WIN32 INCLUDES */
#ifdef WIN32
#include <windows.h>
#include <winbase.h>
#include <windef.h>
#endif

/* BeOS INCLUDES */
#ifdef TARGET_BEOS
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <ctype.h>
#include <be/kernel/OS.h>
#endif

/* LINUX INCLUDES */
#ifdef __linux__
#include <unistd.h>
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <ctype.h>
#define KERNELC_V_1 2
#define KERNELC_V_2 3
#define KERNELC_V_3 16
#endif

/* Mac OS X INCLUDES */
#ifdef TARGET_MAC
#include <unistd.h>
#include <sys/utsname.h>
#include <ctype.h>
#endif

/* BSD INCLUDES */
#ifdef TARGET_BSD
#include <unistd.h>
#include <sys/utsname.h>
#include <ctype.h>
#endif

#include "bgddl.h"

#include "libmod_misc.h"

/*
 * Dynamic memory
 */

/* Linux utility function */

#ifdef __linux__
static int kernel_version_type( void ) {
    struct utsname sysinf;
    int kernel_v[3];
    int i, t, fv = 0;

    if ( uname( &sysinf ) == -1 )
        return -1;

    bzero(( int* )kernel_v, sizeof( int )*3 );

    for ( i = 0, t = 0; i <= 2; i++ ) {
        if ( sysinf.release[t] ) {
            kernel_v[i] = atoi( &sysinf.release[t] );
            while ( sysinf.release[++t] && sysinf.release[t] != '.' )
               ;
            t++;
        }
    }

    if ( !fv && kernel_v[0] > KERNELC_V_1 ) fv = 1;
    if ( !fv && kernel_v[0] < KERNELC_V_1 ) fv = 2;
    if ( !fv && kernel_v[1] > KERNELC_V_2 ) fv = 1;
    if ( !fv && kernel_v[1] < KERNELC_V_2 ) fv = 2;
    if ( !fv && kernel_v[2] > KERNELC_V_3 ) fv = 1;
    if ( !fv && kernel_v[2] < KERNELC_V_3 ) fv = 2;

    return fv;
}
#endif

/* MEMORY_FREE()
 *  Returns the number of free bytes (physycal memory only)
 *  This value is intended only for informational purposes
 *  and may or may not be an approximation.
 */

int64_t libmod_misc_mem_memory_free( INSTANCE * my, int64_t * params ) {
#ifdef WIN32
    MEMORYSTATUS mem;
    GlobalMemoryStatus( &mem );
    return mem.dwAvailPhys;

#elif defined(TARGET_BEOS)
    system_info info;
    get_system_info( &info );
    return B_PAGE_SIZE * ( info.max_pages - info.used_pages );

#elif !defined(TARGET_MAC) && !defined(TARGET_WII)
    /* Linux and other Unix (?) */
    struct sysinfo meminf;
    int fv;

    if ( sysinfo( &meminf ) == -1 ) return -1;

    if ( !( fv = kernel_version_type() ) ) return -1;

    if ( fv == 1 )
        return meminf.freeram * meminf.mem_unit;
    else
        return meminf.freeram;

    return -1;

#else
    return 0; //TODO

#endif
}

/* MEMORY_TOTAL();
 *  Return total number of bytes of physical memory
 */

int64_t libmod_misc_mem_memory_total( INSTANCE * my, int64_t * params ) {
#ifdef WIN32
    MEMORYSTATUS mem;
    GlobalMemoryStatus( &mem );
    return mem.dwTotalPhys;

#elif defined(TARGET_BEOS)
    system_info info;
    get_system_info( &info );
    return  B_PAGE_SIZE * ( info.max_pages );

#elif !defined(TARGET_MAC) && !defined(TARGET_WII)
    /* Linux and other Unix (?) */
    struct sysinfo meminf;
    int fv;

    if ( sysinfo( &meminf ) == -1 ) return -1;

    if ( !( fv = kernel_version_type() ) ) return -1;

    if ( fv == 1 )
        return meminf.totalram * meminf.mem_unit;
    else
        return meminf.totalram;

    return -1;

#else
    return 0; //TODO

#endif
}

int64_t libmod_misc_mem_memcmp( INSTANCE * my, int64_t * params ) {
    return ( memcmp(( void * )params[0], ( void * )params[1], params[2] ) );
}

int64_t libmod_misc_mem_memmove( INSTANCE * my, int64_t * params ) {
    memmove(( void * )params[0], ( void * )params[1], params[2] );
    return 1;
}

int64_t libmod_misc_mem_memcopy( INSTANCE * my, int64_t * params ) {
    memcpy(( void * )params[0], ( void * )params[1], params[2] );
    return 1;
}

int64_t libmod_misc_mem_memset( INSTANCE * my, int64_t * params ) {
    memset(( void * )params[0], params[1], params[2] );
    return 1;
}

int64_t libmod_misc_mem_memsetw( INSTANCE * my, int64_t * params ) {
    uint16_t * ptr = ( uint16_t * )params[0];
    const uint16_t b = params[1];
    int n;

    for ( n = params[2]; n; n-- ) *ptr++ = b;
    return 1;
}

int64_t libmod_misc_mem_memseti( INSTANCE * my, int64_t * params ) {
    uint32_t * ptr = ( uint32_t * )params[0];
    const uint32_t b = params[1];
    int n;

    for ( n = params[2]; n; n-- ) *ptr++ = b;
    return 1;
}

int64_t libmod_misc_mem_memsetq( INSTANCE * my, int64_t * params ) {
    uint64_t * ptr = ( uint64_t * )params[0];
    const uint64_t b = params[1];
    int n;

    for ( n = params[2]; n; n-- ) *ptr++ = b;
    return 1;
}

int64_t libmod_misc_mem_calloc( INSTANCE * my, int64_t * params ) {
    return (( int64_t ) calloc( params[0], params[1] ) );
}

int64_t libmod_misc_mem_alloc( INSTANCE * my, int64_t * params ) {
    return (( int64_t ) malloc( params[0] ) );
}

int64_t libmod_misc_mem_realloc( INSTANCE * my, int64_t * params ) {
    return (( int64_t )realloc(( void * )params[0], params[1] ) );
}

int64_t libmod_misc_mem_free( INSTANCE * my, int64_t * params ) {
    free(( void * )params[0] );
    return 1;
}

/* ----------------------------------------------------------------- */
