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

#include <time.h>

#ifdef WIN32
/* NEEDED FOR LOCALE DETECTION */
#include <windows.h>
#include <windef.h>
#include <winnls.h>
#include "shlwapi.h"
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "bgdc.h"

#include "errors.h"

/* --------------------------------------------------------------------------- */

#define REMOVE_EXT(s) {char *p; if ((p = strrchr(s, '.')) && (p > strrchr(s, '\\') && p > strrchr(s, '/'))) * p = '\0';}

extern int load_file( char * filename );
extern void import_files( char * filename );
extern void add_simple_define( unsigned char * macro, unsigned char *text );
extern int dcb_options;

extern int dcb_load_lib( const char * filename );

/* --------------------------------------------------------------------------- */

char langinfo[64];

extern int64_t debug;
int64_t autodeclare = 1;
int64_t libmode = 0;

static char _main_path[__MAX_PATH] = { 0 };
char * main_path = NULL;

char * appexename       = NULL;
char * appexepath       = NULL;
char * appexefullpath   = NULL;

/* --------------------------------------------------------------------------- */

static char timebuff[11]; /* YYYY/MM/DD or HH:MM:SS */
static char _tmp[128];

/* --------------------------------------------------------------------------- */

int main( int argc, char *argv[] ) {
    time_t curtime;
    struct tm *loctime;
    int value, code;
    char * d, *d1;

    char * sourcefile = 0;
    char basepathname[__MAX_PATH] = "";
    char dcbname[__MAX_PATH] = "";
    char stubname[__MAX_PATH] = "";
    char importname[__MAX_PATH] = "";
    char compilerimport[__MAX_PATH] = "";
    int i, j;
    char *ptr;

    /* get my executable name */
    ptr = argv[0] + strlen( argv[0] );
    while ( ptr > argv[0] && ptr[-1] != '\\' && ptr[-1] != '/' ) ptr-- ;

    appexename = strdup( ptr );

    /* get executable full pathname  */
    appexefullpath = getfullpath( argv[0] );
    if ( ( !strchr( argv[0], '\\' ) && !strchr( argv[0], '/' ) ) ) {
        struct stat st;
        if ( stat( appexefullpath, &st ) || !S_ISREG( st.st_mode ) ) {
            if ( appexename ) {
                char *p = whereis( appexename );
                if ( p ) {
                    char * tmp = calloc( 1, strlen( p ) + strlen( appexename ) + 2 );
                    if ( tmp ) {
                        free( appexefullpath );
                        sprintf( tmp, "%s/%s", p, appexename );
                        appexefullpath = getfullpath( tmp );
                        free( tmp );
                    }
                    free( p );
                }
            }
        }
    }

    if ( appexename ) {
        /* get pathname of executable */
        ptr = strstr( appexefullpath, appexename );
        appexepath = calloc( 1, ptr - appexefullpath + 1 );
        strncpy( appexepath, appexefullpath, ptr - appexefullpath );
    }

    printf( BGDC_VERSION "\n"
            "Bennu Game Development Compiler\n\n"
            MSG_COPYRIGHT "\n" );

    /* Default lang to EN */
    strcpy( langinfo, "EN" );
    /* LANG detect */
#ifdef WIN32
    GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_SABBREVCTRYNAME, langinfo, 64 );
    strlwr( langinfo );
#else
    if ( getenv( "LANG" ) != NULL && strlen( getenv( "LANG" ) ) >= 2 )
        strcpy( langinfo, getenv( "LANG" ) );
#endif
    langinfo[2] = 0;

    srand( time( NULL ) );

    /* build error messages list */
    err_buildErrorTable();

    init_c_type();
    identifier_init();
    constants_init();
    string_init();
    compile_init();

    mainproc = procdef_new( procdef_getid(), identifier_search_or_add( "MAIN" ) ) ;

    /* Init vars */

    char tmp_version[ 32 ];
    sprintf( tmp_version, "\"%s\"", VERSION );
    add_simple_define( "COMPILER_VERSION", tmp_version );
    add_simple_define( "__VERSION__", tmp_version );

    curtime = time( NULL ); /* Get the current time. */
    loctime = localtime( &curtime ); /* Convert it to local time representation. */

    strftime( timebuff, sizeof( timebuff ), "%Y/%m/%d", loctime );
    value = string_new( timebuff );
    code = identifier_search_or_add( "__DATE__" ) ;
    constants_add( code, typedef_new( TYPE_STRING ), value ) ;

    strftime( timebuff, sizeof( timebuff ), "%H:%M:%S", loctime );
    value = string_new( timebuff );
    code = identifier_search_or_add( "__TIME__" ) ;
    constants_add( code, typedef_new( TYPE_STRING ), value ) ;

    strcpy( _tmp, VERSION );
                d = strchr( _tmp, '.' ); *d = '\0'; add_simple_define( "__BGD__", _tmp );
    d1 = d + 1; d = strchr(   d1, '.' ); *d = '\0'; add_simple_define( "__BGD_MINOR__", d1 );
    d1 = d + 1;                                     add_simple_define( "__BGD_PATCHLEVEL__", d1 );

    memset( &dcb, 0, sizeof( dcb ) );

    core_init();
    sysproc_init();

    /* Get command line parameters */

    for ( i = 1 ; i < argc && argv[i]; i++ ) {
        if ( argv[i][0] == '-' ) {
            if ( !strcmp( argv[i], "--pedantic" ) ) {
                autodeclare = 0 ;
                continue;
            }

            if ( !strcmp( argv[i], "--libmode" ) ) {
                libmode = 1 ;
                continue;
            }

            j = 1;
            while ( argv[i][j] ) {
                if ( argv[i][j] == 'd' ) {
                    if ( argv[i][j + 1] >= '0' && argv[i][j + 1] <= '9' ) {
                        debug = atoi( &argv[i][j + 1] );
                    } else {
                        debug++;
                    }
                }

                if ( argv[i][j] == 'o' ) {
                    if ( argv[i][j + 1] ) {
                        strncpy( dcbname, &argv[i][j + 1], sizeof( dcbname ) );
                    } else if ( argv[i + 1] && argv[i + 1][0] != '-' ) {
                        strncpy( dcbname, argv[++i], sizeof( dcbname ) );
                    }
                    break;
                }

                if ( argv[i][j] == 'a' ) autoinclude = 1;

                if ( argv[i][j] == 'g' ) dcb_options |= DCB_DEBUG;

                if ( argv[i][j] == 'z' ) dcb_options |= DCB_COMPRESSED;

                if ( argv[i][j] == 'p' ) autodeclare = 0 ;

                if ( argv[i][j] == 's' ) {
                    /* -s "stub": Use a stub */

                    if ( argv[i][j + 1] ) {
                        strncpy( stubname, &argv[i][j + 1], __MAX_PATH );
                    } else if ( argv[i + 1] && argv[i + 1][0] != '-' ) {
                        strncpy( stubname, argv[++i], __MAX_PATH );
                    }
                    break;
                }

                if ( argv[i][j] == 'f' ) {
                    /* -f "file": Embed a file to the DCB */
                    if ( argv[i][j + 1] ) {
                        dcb_add_file( &argv[i][j + 1] );
                    } else {
                        while ( argv[i + 1] ) {
                            if ( argv[i + 1][0] == '-' ) break;
                            dcb_add_file( argv[i + 1] );
                            i++;
                        }
                    }
                    break;
                }

                if ( argv[i][j] == 'i' ) {
                    /* -i "path": add a file to the path for include files */

                    if ( argv[i][j + 1] == 0 ) {
                        if ( i == argc - 1 ) {
                            printf( MSG_DIRECTORY_MISSING "\n" );
                            exit( 1 );
                        }
                        file_addp( argv[i + 1] );
                        i++;
                        break;
                    }
                    file_addp( &argv[i][j + 1] );
                    break;
                }

                if ( argv[i][j] == 'l' ) {
                    /* -lLANG:  Set the language for errors and messages */

                    if ( argv[i][j + 1] == 0 ) {
                        if ( i != argc - 1 ) {
                            strcpy( langinfo, argv[i + 1] );
                        }
                        i++;
                        break;
                    }
                    strcpy( langinfo, &argv[i][j + 1] );
                    break;
                }

                if ( argv[i][j] == 'D' ) {
                    char * macro = NULL ;
                    char * text = NULL ;

                    /* -D<macro>=<text> */

                    if ( argv[i][j + 1] ) {
                        macro = strdup( &argv[i][j + 1] );
                    } else {
                        if ( argv[i + 1][0] == '-' ) break;
                        macro = strdup( argv[i + 1] );
                        i++;
                    }

                    if (( text = strchr( macro, '=' ) ) ) {
                        * text = '\0';
                        text++;
                    } else {
                        text = "";
                    }

                    add_simple_define( macro, text );
                    free( macro );
                    break;
                }

                if ( argv[i][j] == 'C' ) {
                    if ( argv[i][j + 1] == 'a' ) autodeclare = 1 ;
                    break;
                }

                if ( argv[i][j] == 'L' ) {
                    int r = 1;
                    char * f;
                    if ( argv[i][j + 1] ) {
                        r = dcb_load_lib( ( f = &argv[i][j + 1] ) );
                    } else if ( argv[i + 1] && argv[i + 1][0] != '-' ) {
                        r = dcb_load_lib( ( f = argv[i + 1] ) );
                        i++;
                    }

                    switch ( r ) {
                        case    0:
                                fprintf( stderr, MSG_DCB_ERROR "\n", f, DCB_VERSION >> 8 ) ;
                                exit( -1 );

                        case    -1:
                                fprintf( stderr, MSG_DCBL_DCB_VERSION_ERROR "\n", f ) ;
                                exit( -1 );
                    }
                    break;
                }

                j++;
            }
        } else {
            char * p, * pathend = NULL;

            sourcefile = argv[i];
            main_path = _main_path;
            strcpy( main_path, sourcefile );
            p = main_path;
            while ( p && *p ) {
                if ( *p == ':' || *p == '\\' || *p == '/' ) pathend = p;
                p++;
            }
            if ( pathend ) {
                *( pathend + 1 ) = '\0';
                file_addp( main_path );
            } else {
                main_path = getcwd(main_path, __MAX_PATH);
                strcat(main_path, PATH_SEP);
            }

            /* Files names */

            strcpy( basepathname, sourcefile );
            REMOVE_EXT( basepathname );

            /* Default compiler imports */
            strcpy( compilerimport, argv[0] );
#ifdef WIN32
            REMOVE_EXT( compilerimport );
#endif
            strcat( compilerimport, ".imp" );
            import_files( compilerimport );
            strcat( compilerimport, "ort" ); /* name.import */
            import_files( compilerimport );

            /* Project imports */
            strcpy( importname, basepathname ); strcat( importname, ".imp" );
            import_files( importname );

            strcat( importname, "ort" ); /* name.import */
            import_files( importname );

            /* Load Main Source File */
            load_file( sourcefile );

            if ( !dcbname[0] ) {
                strcpy( dcbname, basepathname ); strcat( dcbname, !libmode ? ".dcb" : ".dcl" );
            }
        }
    }

    if ( !sourcefile ) {
        printf( MSG_USAGE
                MSG_OPTION_D
                MSG_OPTIONS
                MSG_LICENSE, argv[0] );
        return 1;
    }

    compile_program();

    if ( stubname[0] != 0 ) {
        if ( !file_exists( stubname ) ) {
#ifdef WIN32
            char exepath[__MAX_PATH];
            GetModuleFileName( NULL, exepath, sizeof( exepath ) );

            ptr = exepath + strlen( exepath );
            while ( ptr > exepath && *ptr != '\\' && *ptr != '/' ) ptr--;
            if ( *ptr == '\\' || *ptr == '/' ) ptr++;
            if ( ptr > exepath ) {
                strcat( exepath, "\\" );
                memmove( stubname + strlen( exepath ), stubname, strlen( stubname ) + 1 );
                memcpy( stubname, exepath, strlen( exepath ) );
            }
#else
            ptr = argv[0] + strlen( argv[0] );
            while ( ptr > argv[0] && *ptr != '\\' && *ptr != '/' ) ptr--;
            if ( *ptr == '\\' || *ptr == '/' ) ptr++;
            if ( ptr > argv[0] ) {
                memmove( stubname + ( ptr - argv[0] ), stubname, strlen( stubname ) + 1 );
                memcpy( stubname, argv[0], ptr - argv[0] );
            }
#endif
            if ( !file_exists( stubname ) ) {
#ifdef WIN32
                strcat( stubname, ".exe" );
                if ( !file_exists( stubname ) ) {
#endif
                    compile_error( "Can't open stub file %s", stubname );
#ifdef WIN32
                    return 1;
                }
#endif
            }
        }

        REMOVE_EXT( dcbname );
#ifdef WIN32
        strcat( dcbname, ".exe" );
#endif
        dcb_save( dcbname, dcb_options, stubname );
    } else {
        dcb_save( dcbname, dcb_options, NULL );
    }

    /* unload Modules */
    free_imports();

    /* destroy error messages list */
    err_destroyErrorTable();

    return 0;
}

/* --------------------------------------------------------------------------- */
