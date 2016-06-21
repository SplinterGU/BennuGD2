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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "dirs.h"

/* WIN32 INCLUDES */
#ifdef WIN32
#include <windows.h>
#include <winbase.h>
#include <windef.h>
#else
#include <unistd.h>
#include <sys/utsname.h>
/* BeOS INCLUDES */
#ifdef TARGET_BEOS
#include <sys/types.h>
#endif
#include <sys/stat.h>
#include <glob.h>
#endif

#include "bgddl.h"
#include "dlvaracc.h"
#include "files.h"
#include "xstrings.h"

#include "libmod_misc.h"

/* ----------------------------------------------------------------- */

/* ----------------------------------------------------------------- */
/* DIRECTORY FUNCTIONS */

int64_t libmod_misc_dir_cd( INSTANCE * my, int64_t * params ) {
    char * d = dir_current() ;
    int r = string_new( d ) ;
    string_use( r ) ;
    if ( d ) free( d ) ;
    return r ;
}

int64_t libmod_misc_dir_chdir( INSTANCE * my, int64_t * params ) {
    const char * d = string_get( params[ 0 ] ) ;
    int64_t ret = dir_change( d ) ;
    string_discard( params[ 0 ] ) ;
    return ( ret ) ;
}

int64_t libmod_misc_dir_mkdir( INSTANCE * my, int64_t * params ) {
    const char * d = string_get( params[ 0 ] ) ;
    int64_t ret = dir_create( d ) ;
    string_discard( params[ 0 ] ) ;
    return ( ret ) ;
}

int64_t libmod_misc_dir_rmdir( INSTANCE * my, int64_t * params ) {
    const char * d = string_get( params[ 0 ] ) ;
    int64_t ret = dir_delete( d );
    string_discard( params[ 0 ] ) ;
    return ( ret ) ;
}

int64_t libmod_misc_dir_rm( INSTANCE * my, int64_t * params ) {
    const char * d = string_get( params[ 0 ] ) ;
    int64_t ret = dir_deletefile( d );
    string_discard( params[ 0 ] ) ;
    return ( ret ) ;
}

int64_t __moddir_read(__DIR_ST * dh ) {
    __DIR_FILEINFO_ST * dif;
    char buffer[ 20 ];
    int64_t result;

    dif = dir_read( dh );
    if ( !dif ) {
        result = string_new( "" );
        string_use( result );
        return ( result );
    }

    /* discard previous strings values */
    string_discard( GLOQWORD( libmod_misc, FILE_NAME ) );
    string_discard( GLOQWORD( libmod_misc, FILE_PATH ) );
    string_discard( GLOQWORD( libmod_misc, FILE_CREATED ) );
    string_discard( GLOQWORD( libmod_misc, FILE_MODIFIED ) );
    string_discard( GLOQWORD( libmod_misc, FILE_ACCESSED ) );
    string_discard( GLOQWORD( libmod_misc, FILE_STATECHG ) );

    GLOQWORD( libmod_misc, FILE_NAME        ) = string_new( dif->filename ); string_use( GLOQWORD( libmod_misc, FILE_NAME ) );
    GLOQWORD( libmod_misc, FILE_PATH        ) = string_new( dif->fullpath ); string_use( GLOQWORD( libmod_misc, FILE_PATH ) );

    GLOQWORD( libmod_misc, FILE_DIRECTORY   ) = dif->attributes & DIR_FI_ATTR_DIRECTORY ? 1 : 0;
    GLOQWORD( libmod_misc, FILE_HIDDEN      ) = dif->attributes & DIR_FI_ATTR_HIDDEN    ? 1 : 0;
    GLOQWORD( libmod_misc, FILE_READONLY    ) = dif->attributes & DIR_FI_ATTR_READONLY  ? 1 : 0;
    GLOQWORD( libmod_misc, FILE_SIZE        ) = dif->size;

    /* Store file times */
#ifdef _WIN32
    strftime( buffer, 20, "%d/%m/%Y %H:%M:S", &dif->mtime );
    GLOQWORD( libmod_misc, FILE_CREATED     ) = string_new( buffer ); string_use( GLOQWORD( libmod_misc, FILE_CREATED  ) );
#else
    GLOQWORD( libmod_misc, FILE_CREATED     ) = string_new( "" ); string_use( GLOQWORD( libmod_misc, FILE_CREATED  ) );
#endif

    strftime( buffer, 20, "%d/%m/%Y %H:%M:S", &dif->crtime );
    GLOQWORD( libmod_misc, FILE_MODIFIED    ) = string_new( buffer ); string_use( GLOQWORD( libmod_misc, FILE_MODIFIED ) );

    strftime( buffer, 20, "%d/%m/%Y %H:%M:S", &dif->atime );
    GLOQWORD( libmod_misc, FILE_ACCESSED    ) = string_new( buffer ); string_use( GLOQWORD( libmod_misc, FILE_ACCESSED ) );

#ifndef _WIN32
    strftime( buffer, 20, "%d/%m/%Y %H:%M:S", &dif->ctime );
    GLOQWORD( libmod_misc, FILE_STATECHG    ) = string_new( buffer ); string_use( GLOQWORD( libmod_misc, FILE_STATECHG ) );
#else
    GLOQWORD( libmod_misc, FILE_STATECHG    ) = string_new( "" ); string_use( GLOQWORD( libmod_misc, FILE_STATECHG ) );
#endif

    /* Return */
    result = GLOQWORD( libmod_misc, FILE_NAME );
    string_use( result );
    return result;
}

/*  string GLOB (STRING path)
 *
 *  Given a path with wildcards ('*' or '?' characters), returns the first
 *  file that matches and, in every next call, all matching files found
 *  until no more files exists. It then returns NIL.
 */

int64_t libmod_misc_dir_glob( INSTANCE * my, int64_t * params ) {
    const char * path = string_get( params[ 0 ] );
    static __DIR_ST * dh = NULL;
    int64_t result;

    if ( dh && strcmp( dh->path, path ) ) {
        dir_close( dh );
        dh = NULL;
    }

    if ( !dh ) dh = dir_open( path );

    string_discard( params[ 0 ] );

    if ( !dh ) {
        result = string_new( "" );
        string_use( result );
        return ( result );
    }

    return ( __moddir_read( dh ) ) ;
}

/*  int DIROPEN (STRING path)
 *
 *  Open a dir for read it, returns handle id.
 *  return 0 if fail.
 */

int64_t libmod_misc_dir_open( INSTANCE * my, int64_t * params ) {
    int64_t result = ( int64_t ) dir_open( string_get( params[ 0 ] ) );
    string_discard( params[ 0 ] );
    return result;
}

/*  int DIRCLOSE (INT handle)
 */

int64_t libmod_misc_dir_close( INSTANCE * my, int64_t * params ) {
    if ( params[ 0 ] ) dir_close ( ( __DIR_ST * ) params[ 0 ] ) ;
    return 1;
}

/*  string DIRREAD (INT handle)
 *
 *  Given a path with wildcards ('*' or '?' characters), returns the first
 *  file that matches and, in every next call, all matching files found
 *  until no more files exists. It then returns NIL.
 */

int64_t libmod_misc_dir_read( INSTANCE * my, int64_t * params ) {
    return ( __moddir_read((__DIR_ST *) params[ 0 ] ) ) ;
}

/* ----------------------------------------------------------------- */
