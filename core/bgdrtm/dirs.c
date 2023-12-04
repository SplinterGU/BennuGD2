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

/*
 * FILE        : dirs.c
 * DESCRIPTION : directory functions
 *
 * HISTORY:
 *
 */

#include "bgdrtm.h"
#include "dirs.h"
#include "xstrings.h"

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include <ctype.h>

/* ------------------------------------------------------------------------------------ */
/*
 *  FUNCTION : match
 *
 *  Matches a pattern against a string.
 *
 *  PARAMS:
 *      const char *pattern     The pattern to match.
 *      const char *string      The string to match against.
 *      int ignore_case         Ignore case
 *
 *  RETURN VALUE:
 *      int:                    1 if the pattern matches, 0 otherwise.
 */

static int match(const char *pattern, const char *string, int ignore_case) {
    while (*pattern && *string) {
        if (*pattern == '*') {
            pattern++;
            while (*string) {
                if (match(pattern, string, ignore_case)) return 1;
                string++;
            }
            return *pattern == '\0';
        } else if ( (
                      ( ignore_case && tolower(*pattern) != tolower(*string) ) ||
                      (!ignore_case &&         *pattern != *string           )
                    ) && *pattern != '?') {
            return 0;
        }
        pattern++;
        string++;
    }
    while (*pattern == '*') pattern++;

    return  ( ignore_case && tolower(*pattern) == tolower(*string) ) ||
            (!ignore_case &&         *pattern  == *string          );
}

/* ------------------------------------------------------------------------------------ */
/*
 *  FUNCTION : dir_path_convert
 *
 *  Convert a path to the valid OS format
 *
 *  PARAMS :
 *      char * path:    path to convert
 *
 *  RETURN VALUE :
 *      char *:         converted path
 *
 */

char * dir_path_convert( const char * dir )
{
    char *c, *p ;

    p = strdup( dir ) ;
    if ( !p ) return NULL;

    c = p ;
    /* Convert characters */
    while ( *p )
    {
        if ( *p == PATH_CHAR_ISEP ) *p = PATH_CHAR_SEP ;
        p++ ;
    }
    return c;
}


/* ------------------------------------------------------------------------------------ */
/*
 *  FUNCTION : dir_current
 *
 *  Retrieve current directory
 *
 *  PARAMS :
 *
 *  RETURN VALUE :
 *      STRING ID pointing to a system string with the current dir
 *
 */

char * dir_current( void )
{
    return ( getcwd( NULL, 0 ) ) ;
}

/* ------------------------------------------------------------------------------------ */
/*
 *  FUNCTION : dir_change
 *
 *  Retrieve current directory
 *
 *  PARAMS :
 *      char * dir:     the new current directory
 *
 *  RETURN VALUE :
 *      0           - FAILURE
 *      NON_ZERO    - SUCCESS
 *
 */

int dir_change( const char * dir )
{
    char *c = dir_path_convert( dir ) ;
    if ( !c ) return 0;
    int r = chdir( c ) ;
    free( c ) ;
    return r ;
}

/* ------------------------------------------------------------------------------------ */
/*
 *  FUNCTION : dir_create
 *
 *  Retrieve current directory
 *
 *  PARAMS :
 *      char * dir:     the directory to create
 *
 *  RETURN VALUE :
 *      0           - FAILURE
 *      NON_ZERO    - SUCCESS
 *
 */

int dir_create( const char * dir )
{
    char *c = dir_path_convert( dir ) ;
    if ( !c ) return 0;
#ifdef WIN32
    int r = mkdir( c ) ;
#else
    int r = mkdir( c, 0777 ) ;
#endif
    free( c ) ;
    return r ;
}

/* ------------------------------------------------------------------------------------ */
/*
 *  FUNCTION : dir_delete
 *
 *  Retrieve current directory
 *
 *  PARAMS :
 *      char * dir:     the directory to delete
 *
 *  RETURN VALUE :
 *      0           - FAILURE
 *      NON_ZERO    - SUCCESS
 *
 */

int dir_delete( const char * dir )
{
    char *c = dir_path_convert( dir ) ;
    if ( !c ) return 0;
    int r = rmdir( c ) ;
    free( c ) ;
    return r ;
}

/* ------------------------------------------------------------------------------------ */
/*
 *  FUNCTION : dir_deletefile
 *
 *  Remove a given file
 *
 *  PARAMS :
 *      char * filename: the file to delete
 *
 *  RETURN VALUE :
 *      0           - FAILURE
 *      NON_ZERO    - SUCCESS
 *
 */

int dir_deletefile( const char * filename )
{
    char *c = dir_path_convert( filename ) ;
    if ( !c ) return 0;
    int r = unlink( c ) ;
    free( c ) ;
    return ( r == -1 ) ? 0 : 1 ;
}

/* ------------------------------------------------------------------------------------ */

__DIR_ST * dir_open( const char * path )
{
    __DIR_ST * hDir = malloc( sizeof( __DIR_ST ) );
    if ( !hDir ) return NULL;

    hDir->path = strdup( path );
    if ( !hDir->path )
    {
        free ( hDir );
        return NULL;
    }

#ifdef _WIN32
    hDir->handle = FindFirstFile( hDir->path, &hDir->data );
    hDir->eod = ( hDir->handle == INVALID_HANDLE_VALUE );

    if ( !hDir->handle )
    {
        free( hDir->path );
        free( hDir );
        return NULL;
    }

#elif defined(USE_OPENDIR)
    hDir->dirname = strdup( path );
    if ( !hDir->dirname ) {
        free( hDir->path );
        free( hDir );
        return NULL;
    }

    char * fptr = hDir->dirname;
    while ( fptr = strchr( fptr, '\\') ) *fptr++ == '/';

    int is_dir = 0;

    if ( *hDir->dirname ) {
        struct stat s;
        stat( hDir->dirname, &s );

        is_dir = S_ISDIR( s.st_mode );
    }

    if ( is_dir || !*hDir->dirname ) {
        hDir->pattern = strdup("*");
    } else {
        fptr = strrchr( hDir->dirname, '/' );
        if ( fptr ) {
            if ( fptr == hDir->dirname ) {
                hDir->pattern = strdup(fptr+1);
                fptr[1] = '\0';
            } else {
                *fptr++ = '\0';
                hDir->pattern = strdup(fptr);
            }
        }
        else {
            if ( !*hDir->dirname ) {
                hDir->pattern = strdup( "*" );
            } else {
                hDir->pattern = strdup( hDir->dirname );
                hDir->dirname[0] = '.';
                hDir->dirname[1] = '\0';
            }
            if ( !hDir->pattern ) {
                free( hDir->dirname );
                free( hDir->path );
                free( hDir );
                return NULL;
            }
        }
    }

    if ( !*hDir->dirname ) {
        free( hDir->dirname );
        hDir->dirname = strdup(".");
    }

    /* Convert '*.*' to '*' */
    if ( strlen( hDir->pattern ) > 2 && !strcmp( hDir->pattern, "*.*" ) ) hDir->pattern[1] = '\0';

    hDir->hdir = opendir( hDir->dirname );
    if ( !hDir->hdir )
    {
        free( hDir->dirname );
        free( hDir->pattern );
        free( hDir->path );
        free( hDir );
        return NULL;
    }
#else
    const char * ptr = hDir->path;
    char * fptr;
    int r;

    hDir->pattern = malloc( strlen( path ) * 4 );
    if ( !hDir->pattern )
    {
        free ( hDir->path );
        free ( hDir );
        return NULL;
    }

    /* Clean the path creating a case-insensitive match pattern */

    fptr = hDir->pattern;
    while ( *ptr )
    {
        if ( *ptr == '\\' )
        {
            *fptr++ = '/';
        }
        else if ( *ptr >= 'a' && *ptr <= 'z' )
        {
            *fptr++ = '[';
            *fptr++ = *ptr;
            *fptr++ = toupper( *ptr );
            *fptr++ = ']';
        }
        else if ( *ptr >= 'A' && *ptr <= 'Z' )
        {
            *fptr++ = '[';
            *fptr++ = tolower( *ptr );
            *fptr++ = *ptr;
            *fptr++ = ']';
        }
        else
            *fptr++ = *ptr;
        ptr++;
    }
    *fptr = 0;

    /* Convert '*.*' to '*' */
    if ( fptr > hDir->pattern + 2 && fptr[ -1 ] == '*' && fptr[ -2 ] == '.' && fptr[ -3 ] == '*' ) fptr[ -2 ] = 0;

#ifdef GLOB_PERIOD
    r = glob( hDir->pattern, GLOB_ERR | GLOB_PERIOD | GLOB_NOSORT, NULL, &hDir->globd );
#else //  defined(__APPLE__) || defined(TARGET_BEOS) || ( defined(__APPLE__) && defined(__MACH__) )
    r = glob( hDir->pattern, GLOB_ERR | GLOB_NOSORT, NULL, &hDir->globd );
#endif

    if ( r )
    {
        free( hDir->pattern );
        free( hDir->path );
        free( hDir );
        return NULL;
    }

    hDir->currFile = 0;
#endif

    return hDir;
}

/* ------------------------------------------------------------------------------------ */

void dir_close ( __DIR_ST * hDir )
{
    if ( !hDir ) return;

    free ( hDir->path );

#ifdef _WIN32
    FindClose( hDir->handle );
#elif defined USE_OPENDIR
    closedir( hDir->hdir );
    free( hDir->dirname );
    free( hDir->pattern );
#else
    globfree( &hDir->globd );
    free( hDir->pattern );
#endif

    free ( hDir );
}

/* ------------------------------------------------------------------------------------ */

__DIR_FILEINFO_ST * dir_read( __DIR_ST * hDir )
{
    if ( !hDir ) return NULL;

    char realpath[__MAX_PATH];
    char * ptr ;

#ifdef _WIN32
    SYSTEMTIME time;

    if ( hDir->eod ) return NULL;

    /* Fill the FILEINFO struct */
    strcpy( realpath, hDir->path );
    ptr = realpath + strlen( realpath );
    while ( ptr >= realpath )
    {
        if ( *ptr == '\\' || *ptr == '/' )
        {
            ptr[ 1 ] = 0;
            break;
        }
        ptr--;
    }

    memset( &hDir->info, '\0', sizeof( hDir->info ) );

    strcat( realpath, hDir->data.cFileName );
    GetFullPathName( realpath, __MAX_PATH, hDir->info.fullpath, &ptr );
    if ( ptr ) * ptr = '\0';

    strcpy ( hDir->info.filename, hDir->data.cFileName );

    hDir->info.attributes    = (( hDir->data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) ? DIR_FI_ATTR_DIRECTORY : 0 ) |
                               (( hDir->data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN    ) ? DIR_FI_ATTR_HIDDEN    : 0 ) |
                               (( hDir->data.dwFileAttributes & FILE_ATTRIBUTE_READONLY  ) ? DIR_FI_ATTR_READONLY  : 0 );

    hDir->info.size          = hDir->data.nFileSizeLow;

    /* Format and store the creation time */
    FileTimeToSystemTime( &hDir->data.ftCreationTime, &time );

    hDir->info.crtime.tm_sec    = time.wSecond;
    hDir->info.crtime.tm_min    = time.wMinute;
    hDir->info.crtime.tm_hour   = time.wHour;
    hDir->info.crtime.tm_mday   = time.wDay;
    hDir->info.crtime.tm_mon    = time.wMonth - 1;
    hDir->info.crtime.tm_year   = time.wYear - 1900;
    hDir->info.crtime.tm_wday   = time.wDayOfWeek;
    hDir->info.crtime.tm_yday   = time.wMonth;
    hDir->info.crtime.tm_isdst  = -1;

    /* Format and store the last write time */
    FileTimeToSystemTime( &hDir->data.ftLastWriteTime, &time );

    hDir->info.mtime.tm_sec     = time.wSecond;
    hDir->info.mtime.tm_min     = time.wMinute;
    hDir->info.mtime.tm_hour    = time.wHour;
    hDir->info.mtime.tm_mday    = time.wDay;
    hDir->info.mtime.tm_mon     = time.wMonth - 1;
    hDir->info.mtime.tm_year    = time.wYear - 1900;
    hDir->info.mtime.tm_wday    = time.wDayOfWeek;
    hDir->info.mtime.tm_yday    = time.wMonth;
    hDir->info.mtime.tm_isdst   = -1;

    /* Format and store the last access time */
    FileTimeToSystemTime( &hDir->data.ftLastAccessTime, &time );

    hDir->info.atime.tm_sec     = time.wSecond;
    hDir->info.atime.tm_min     = time.wMinute;
    hDir->info.atime.tm_hour    = time.wHour;
    hDir->info.atime.tm_mday    = time.wDay;
    hDir->info.atime.tm_mon     = time.wMonth - 1;
    hDir->info.atime.tm_year    = time.wYear - 1900;
    hDir->info.atime.tm_wday    = time.wDayOfWeek;
    hDir->info.atime.tm_yday    = time.wMonth;
    hDir->info.atime.tm_isdst   = -1;

    /* Continue last search */
    if (!FindNextFile( hDir->handle, &hDir->data )) hDir->eod = 1;

#elif defined USE_OPENDIR
    struct stat s;
    struct dirent *entry;
    
    while( ( entry = readdir( hDir->hdir ) ) && !match( hDir->pattern, entry->d_name, 1 ) );

    if ( !entry ) return NULL;

    memset( &hDir->info, '\0', sizeof( hDir->info ) );

    strcpy ( hDir->info.filename, entry->d_name );

    if ( hDir->dirname ) {
        strcpy( hDir->info.fullpath, hDir->dirname );
        strcat( hDir->info.fullpath, "/" );
        strcat( hDir->info.fullpath, entry->d_name );
    }

    stat( hDir->info.fullpath, &s );

    hDir->info.attributes    = (( S_ISDIR( s.st_mode )          ) ? DIR_FI_ATTR_DIRECTORY : 0 ) |
                               (( hDir->info.filename[0] == '.' ) ? DIR_FI_ATTR_HIDDEN    : 0 ) |
                               (( !( s.st_mode & 0444 )         ) ? DIR_FI_ATTR_READONLY  : 0 );

    hDir->info.size          = s.st_size;

    hDir->info.mtime    = *localtime( &s.st_mtime ) ;
    hDir->info.atime    = *localtime( &s.st_atime ) ;
    hDir->info.ctime    = *localtime( &s.st_ctime ) ;

#else
    struct stat s;

    if ( hDir->currFile == hDir->globd.gl_pathc ) return NULL;

    memset( &hDir->info, '\0', sizeof( hDir->info ) );

    stat( hDir->globd.gl_pathv[ hDir->currFile ], &s );

    ptr = strrchr( hDir->globd.gl_pathv[ hDir->currFile ], '/' );
    if ( !ptr )
    {
        strcpy ( hDir->info.filename, hDir->globd.gl_pathv[ hDir->currFile ] );
        strcpy ( hDir->info.fullpath, getcwd( realpath, sizeof( realpath ) ) );
    }
    else
    {
        strcpy ( hDir->info.filename, ptr + 1 );
        if ( hDir->globd.gl_pathv[ hDir->currFile ][0] == '/' )
        {
            strcpy ( hDir->info.fullpath, hDir->globd.gl_pathv[ hDir->currFile ] );
            hDir->info.fullpath[ ptr - hDir->globd.gl_pathv[ hDir->currFile ] + 1 ] = '\0';
        }
        else
        {
            strcpy ( hDir->info.fullpath, getcwd( realpath, sizeof( realpath ) ) );
            strcat ( hDir->info.fullpath, "/" );
            strcat ( hDir->info.fullpath, hDir->globd.gl_pathv[ hDir->currFile ] );
            ptr = strrchr( hDir->info.fullpath, '/' );
            *(ptr + 1) = '\0';
        }
    }

    hDir->info.attributes    = (( S_ISDIR( s.st_mode )          ) ? DIR_FI_ATTR_DIRECTORY : 0 ) |
                               (( hDir->info.filename[0] == '.' ) ? DIR_FI_ATTR_HIDDEN    : 0 ) |
                               (( !( s.st_mode & 0444 )         ) ? DIR_FI_ATTR_READONLY  : 0 );

    hDir->info.size          = s.st_size;

    hDir->info.mtime    = *localtime( &s.st_mtime ) ;
    hDir->info.atime    = *localtime( &s.st_atime ) ;
    hDir->info.ctime    = *localtime( &s.st_ctime ) ;

    hDir->currFile++;

#endif

    return ( &hDir->info );
}

/* ------------------------------------------------------------------------------------ */
