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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#include <SDL.h>

#include "bgdrtm.h"

#include "xstrings.h"
#include "bgddl.h"

#include "libmod_misc.h"

#include "dlvaracc.h"


/* --------------------------------------------------------------------------- */
/* Timer                                                                       */

int64_t libmod_misc_get_timer( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) SDL_GetTicks();
}

/* --------------------------------------------------------------------------- */
/* Hora del d�a                                                                */

int64_t libmod_misc_time( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) time( NULL );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_misc_ftime
 *
 *  Returns parts of the date
 *
 *  PARAMS:
 *      no params
 *
 *  RETURN VALUE:
 *      pointer to a string value...
 *
 */

int64_t libmod_misc_ftime( INSTANCE * my, int64_t * params ) {
    char buffer[128], * format, * base;
    struct tm * t;
    int64_t ret;
    time_t tim;

#ifdef _WIN32
    /* aux buffer to make all changes... */
    char aux[128];
    unsigned char pos;
#endif

    format = base = strdup( string_get( params[0] ) );
    string_discard( params[0] );

#ifdef _WIN32
    /* Addapting win32 strftime formats to linux formats */
    /* HEAVY PATCH... :( */
    pos = 0;
    while ( *format && pos < 127 ) {
        switch ( *format ) {
            case '%': /* MIGHT NEED CONVERSION... */
                aux[pos] = *format;
                pos++;
                format++;
                switch ( *format ) {
                    case 'e':
                        aux[pos++] = '#';
                        aux[pos] = 'd';
                        break;
                    case 'l':
                        aux[pos++] = '#';
                        aux[pos] = 'I';
                        break;
                    case 'k':
                        aux[pos++] = '#';
                        aux[pos] = 'H';
                        break;
                    case 'P':
                        aux[pos] = 'p';
                        break;

                    case 'C':
                        aux[pos++] = '%';
                        aux[pos++] = *format;
                        aux[pos++] = '%';
                        aux[pos] = 'Y';
                        break;

                    case 'u':
                        aux[pos++] = '%';
                        aux[pos++] = *format;
                        aux[pos++] = '%';
                        aux[pos] = 'w';
                        break;

                    case '%':   //MUST BE %%%% TO KEEP 2 IN POSTPROCESS
                        aux[pos++] = '%';
                        aux[pos++] = '%';
                        aux[pos] = '%';
                        break;

                    default:
                        aux[pos] = *format;
                        break;
                }
                break;

            default: aux[pos] = *format;
                break;
        }
        format++;
        pos++;
    }
    aux[pos] = 0;
    format = aux;
#endif

    tim = ( time_t ) params[1];
    t = localtime( &tim );
    strftime( buffer, sizeof( buffer ), format, t );

#ifdef _WIN32
    /* win32 postprocess */
    aux[0] = '\0';
    format = buffer;
    pos = 0;
    while ( *format ) {
        switch ( *format ) {
            case '%':
                format++;
                switch ( *format ) {
                    case 'u':
                        format++;
                        if ( *format == '0' ) *format = '7';
                        aux[pos] = *format;
                        break;

                    case 'C':
                        format++;
                        aux[pos++] = *format;
                        format++;
                        aux[pos] = *format;
                        format++;
                        format++;
                        break;

                    default:
                        aux[pos] = *format;
                        break;
                }
                break;

            default:
                aux[pos] = *format;
                break;
        }
        format++;
        pos++;
    }
    aux[pos] = '\0';
    strcpy( buffer, aux );
#endif

    ret = string_new( buffer );
    string_use( ret );

    free( base );

    return ret;
}

/* --------------------------------------------------------------------------- */
/* exports                                                                     */
/* --------------------------------------------------------------------------- */

/* ----------------------------------------------------------------- */
/*
 *  FUNCTION : libmod_misc_advance_timers
 *
 *  Update the value of all global timers
 *
 *  PARAMS :
 *      None
 *
 *  RETURN VALUE :
 *      None
 */

void libmod_misc_advance_timers( void ) {
    static int64_t initial_ticktimer[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    static int64_t ltimer[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}; // -1 to force initial_ticktimer update
    static int64_t last_curr_ticktimer = 0;
    int64_t curr_ticktimer = SDL_GetTicks();
    int64_t * timer, i;

    /* TODO: Here add checking for console_mode, don't advance in this mode */
    timer = ( int64_t * ) ( &GLOQWORD( libmod_misc, TIMER ) );
    if ( system_paused || debugger_show_console ) {
        for ( i = 0; i < 10; i++ ) {
            initial_ticktimer[i] += curr_ticktimer - last_curr_ticktimer;
        }
    } else {
        for ( i = 0; i < 10; i++ ) {
            if ( timer[i] != ltimer[i] ) initial_ticktimer[i] = curr_ticktimer - ( timer[i] * 10 );
            ltimer[i] = timer[i] = ( curr_ticktimer - initial_ticktimer[i] ) / 10;
        }
    }
    last_curr_ticktimer = curr_ticktimer;
}

/* --------------------------------------------------------------------------- */
