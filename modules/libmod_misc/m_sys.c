/*
 *  Copyright (C) SplinterGU (Fenix/BennuGD) (Since 2006)
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
#include <time.h>

#include "bgddl.h"
#include "files.h"
#include "xstrings.h"

#include <unistd.h>

#ifndef WIN32
#include <sys/wait.h>
#else
#include <process.h>
#endif

#include "SDL.h"

/* ---------------------------------------------------------------------- */

#include "libmod_misc.h"

/* ---------------------------------------------------------------------- */

int64_t libmod_misc_sys_exec( INSTANCE * my, int64_t * params ) {
    int mode = ( int ) params[0];
    char * filename = ( char * ) string_get( params[1] );
    int argc = ( int ) params[2];
    char ** argv;
    int n = 0, status = -1;
#ifndef WIN32
    pid_t child;
#endif

    // fill argv
    argv = ( char ** ) calloc( argc + 2, sizeof( char * ) );
    argv[0] = filename;
    for ( n = 0; n < argc; n++ )
        argv[n + 1] = ( char * ) string_get((( int64_t * )( intptr_t )( params[3] ) )[n] );

    // Execute program
#ifdef WIN32
    status = spawnvp( mode, filename, ( char * const * )argv );
#elif defined( __SWITCH__ ) || defined( PS3_PPU )
    status = -1;
#else
    if (( child = fork() ) == -1 ) {
        //Error
        status = -1 ;
    } else if ( child == 0 ) {
        execvp( filename, ( char * const * )argv );
        exit(-1);
    } else {
        /* father */
        switch ( mode ) {
            case _P_WAIT:
                if ( waitpid( child, &status, WUNTRACED ) != child )
                    status = -1;
                else
                    status = ( int )(char)WEXITSTATUS(status);
                break;

            case _P_NOWAIT:
                status = child;
                break;
        }
    }
#endif

    // Free resources
    string_discard( params[1] );
    if ( argv ) free( argv );

    return ( ( int64_t ) status ) ;
}

/* ---------------------------------------------------------------------- */

int64_t libmod_misc_sys_getenv( INSTANCE * my, int64_t * params ) {
    char *e ;
    int64_t str ;

    if (( e = getenv( string_get( params[0] ) ) ) ) str = string_new( e ) ;
    else                                            str = string_new( "" ) ;

    string_discard( params[0] ) ;
    string_use( str ) ;
    return str ;
}

/* ----------------------------------------------------------------- */

int64_t libmod_misc_sys_get_pref_language( INSTANCE * my, int64_t * params ) {
	int64_t str;
    SDL_Locale *locales = SDL_GetPreferredLocales();
	
    if (locales == NULL) {
        str = string_new( "" );
    } else {
		if (locales[0].language != NULL) {
			str = string_new( locales[0].language );
		} else {
			str = string_new( "" );
		}
	}

    SDL_free(locales);
	string_use(str);
	return str;
}

/* ----------------------------------------------------------------- */