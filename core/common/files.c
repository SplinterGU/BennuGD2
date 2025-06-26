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

#ifdef _WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef TARGET_BEOS
#include <posix/assert.h>
#else
#include <assert.h>
#endif

#include <stdint.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "files.h"

#define MAX_POSSIBLE_PATHS  128

#ifdef __ANDROID__
#ifdef __BGDRTM__
#include "SDL.h"
#include "SDL_rwops.h"
#define FILE SDL_RWops
#define fopen(fname, mode) SDL_RWFromFile(fname, mode)
#define fread(ptr, size, nmemb, stream) SDL_RWread(stream, ptr, size, nmemb)
#define fwrite(ptr, size, nmemb, stream) SDL_RWwrite(stream, ptr, size, nmemb)
#define fclose(stream) SDL_RWclose(stream)
#define fseek(stream, offset, whence) SDL_RWseek(stream, offset, whence)
#define ftell(stream) SDL_RWtell(stream)
#define rewind(stream) SDL_RWseek(stream, 0, RW_SEEK_SET)
#define feof(stream) (SDL_RWtell(stream) >= SDL_RWsize(stream))
static char *sdl_fgets(char *s, int size, SDL_RWops *stream) {
    int i = 0;
    while (i < size - 1) {
        char c;
        if (SDL_RWread(stream, &c, 1, 1) != 1) {
            // No más datos
            break;
        }
        s[i++] = c;
        if (c == '\n') {
            break;
        }
    }

    if (i == 0) {
        return NULL; // EOF
    }

    s[i] = '\0';
    return s;
}
#define fgets(ptr, size, stream) sdl_fgets(ptr, size, stream)
#endif
#endif

char * possible_paths[MAX_POSSIBLE_PATHS] = { NULL };

int opened_files = 0;

XFILE * x_file = NULL;
int max_x_files = 0;

int x_files_count = 0;

/* Add new file to PATH */

void xfile_init( int maxfiles ) {
    x_file = ( XFILE * ) calloc( sizeof( XFILE ), maxfiles );
    max_x_files = maxfiles;
}

void file_add_xfile( file * fp, const char * stubname, long offset, char * name, int size ) {
    char * ptr;

    assert( x_files_count < max_x_files );
    assert( fp->type == F_FILE );

    x_file[x_files_count].stubname = strdup( stubname );
    x_file[x_files_count].offset = offset;
    x_file[x_files_count].size = size;
    x_file[x_files_count].name = strdup( name );

    ptr = x_file[x_files_count].name;
    while ( *ptr ) {
        if ( *ptr == '\\' ) *ptr = '/'; /* Unix style */
        ptr++;
    }

    x_files_count++;
}

/* Read a datablock from file */

int file_read( file * fp, void * buffer, int len ) {
    if ( !fp || !len ) return 0;

    if ( fp->type == F_XFILE ) {
        int result;

        if (( len + fp->pos ) > ( fp->xf->offset + fp->xf->size ) ) {
            fp->eof = 1;
            len = fp->xf->size + fp->xf->offset - fp->pos;
        }

        gzseek( fp->gz, fp->pos, SEEK_SET );
        result = gzread( fp->gz, buffer, len );
        fp->error = ( result < len );
        if ( result < 0 ) result = 0;
        fp->pos = gztell( fp->gz );
        return result;
    }

    if ( fp->type == F_GZFILE ) {
        int result = gzread( fp->gz, buffer, len );
        fp->error = ( result < len );
        if ( result < 0 ) result = 0;
        return result;
    }
    return fread( buffer, 1, len, fp->fp );
}

/* Save a unquoted string to a file */

int file_qputs( file * fp, char * buffer ) {
    char dest[1024], * optr;
    const char * ptr;

    ptr = buffer;
    optr = dest;
    while ( *ptr ) {
        if ( optr - dest >= 1019 ) {
            *optr++ = '\\';
            *optr++ = '\n';
            *optr   = 0;
            file_write( fp, dest, optr - dest );
            optr = dest;
        }
        if ( *ptr == '\n' ) {
            *optr++ = '\\';
            *optr++ = 'n';
            ptr++;
            continue;
        }
        if ( *ptr == '\\' ) {
            *optr++ = '\\';
            *optr++ = *ptr++;
            continue;
        }
        *optr++ = *ptr++;
    }
    *optr++ = '\n';
    return file_write( fp, dest, optr - dest );
}

/* Load a string from a file and unquoted it */

int file_qgets( file * fp, char * buffer, int len ) {
    char * ptr, * result = NULL;
    size_t sz;

    if ( fp->type == F_XFILE ) {
        ptr = result = buffer;

        if ( fp->pos - fp->xf->offset + len - 1 > fp->xf->size ) {
            len = fp->xf->size - ( fp->pos - fp->xf->offset ) - 1;
            fp->eof = 1;
        }

        gzseek( fp->gz, fp->pos, SEEK_SET );
        result = gzgets( fp->gz, buffer, len ) ;
        fp->pos = gztell( fp->gz );
    }
    else if ( fp->type == F_GZFILE ) {
        result = gzgets( fp->gz, buffer, len );
    }
    else {
        result = fgets( buffer, len, fp->fp );
    }

    if ( result == NULL ) {
        buffer[0] = 0;
        return 0;
    }

    ptr = buffer;
    while ( *ptr ) {
        if ( *ptr == '\\' ) {
            if ( ptr[1] == 'n' ) ptr[1] = '\n';
            strcpy( ptr, ptr + 1 );
            ptr++;
            continue;
        }
        if ( *ptr == '\n' ) {
            *ptr = 0;
            break;
        }
        ptr++;
    }

    return strlen( buffer );
}

/* Save a string to file */

int file_puts( file * fp, char * buffer ) {
    return file_write( fp, buffer, strlen( buffer ) );
}

/* Load a string from a file and unquoted it */

int file_gets( file * fp, char * buffer, int len ) {
    char * result = NULL;
    size_t sz;

    if ( fp->type == F_XFILE ) {
        char * ptr = result = buffer;

        if ( fp->pos - fp->xf->offset + len - 1 > fp->xf->size ) {
            len = fp->xf->size - ( fp->pos - fp->xf->offset ) - 1;
            fp->eof = 1;
        }

        gzseek( fp->gz, fp->pos, SEEK_SET );
        result = gzgets( fp->gz, buffer, len );
        fp->pos = gztell( fp->gz );
    }
    else if ( fp->type == F_GZFILE ) {
        result = gzgets( fp->gz, buffer, len );
    }
    else {
        result = fgets( buffer, len, fp->fp );
    }

    if ( result == NULL ) {
        buffer[0] = 0;
        return 0;
    }

    return strlen( buffer );
}

/* Save an int data to a binary file */

int file_writeSint8( file * fp, int8_t * buffer ) {
    return file_write( fp, buffer, sizeof( uint8_t ) );
}

int file_writeUint8( file * fp, uint8_t * buffer ) {
    return file_write( fp, buffer, sizeof( uint8_t ) );
}

int file_writeSint16( file * fp, int16_t * buffer ) {
#if __BYTEORDER == __LIL_ENDIAN
    return file_write( fp, buffer, sizeof( int16_t ) );
#else
    file_write( fp, ( uint8_t * )buffer + 1, 1 );
    return file_write( fp, ( uint8_t * )buffer, 1 );
#endif
}

int file_writeUint16( file * fp, uint16_t * buffer ) {
    return file_writeSint16( fp, ( int16_t * ) buffer );
}

int file_writeSint32( file * fp, int32_t * buffer ) {
#if __BYTEORDER == __LIL_ENDIAN
    return file_write( fp, buffer, sizeof( int32_t ) );
#else
    file_write( fp, ( uint8_t * )buffer + 3, 1 );
    file_write( fp, ( uint8_t * )buffer + 2, 1 );
    file_write( fp, ( uint8_t * )buffer + 1, 1 );
    return file_write( fp, ( uint8_t * )buffer, 1 );
#endif
}

int file_writeUint32( file * fp, uint32_t * buffer ) {
    return file_writeSint32( fp, ( int32_t * )buffer );
}

int file_writeSint64( file * fp, int64_t * buffer ) {
#if __BYTEORDER == __LIL_ENDIAN
    return file_write( fp, buffer, sizeof( int64_t ) );
#else
    file_write( fp, ( uint8_t * )buffer + 7, 1 );
    file_write( fp, ( uint8_t * )buffer + 6, 1 );
    file_write( fp, ( uint8_t * )buffer + 5, 1 );
    file_write( fp, ( uint8_t * )buffer + 4, 1 );
    file_write( fp, ( uint8_t * )buffer + 3, 1 );
    file_write( fp, ( uint8_t * )buffer + 2, 1 );
    file_write( fp, ( uint8_t * )buffer + 1, 1 );
    return file_write( fp, ( uint8_t * )buffer, 1 );
#endif
}

int file_writeUint64( file * fp, uint64_t * buffer ) {
    return file_writeSint64( fp, ( int64_t * )buffer );
}

/* Save an array to a binary file */

int file_writeSint8A( file * fp, int8_t * buffer, int n ) {
    return file_write( fp, buffer, n );
}

int file_writeUint8A( file * fp, uint8_t * buffer, int n ) {
    return file_write( fp, buffer, n );
}

int file_writeSint16A( file * fp, int16_t * buffer, int n ) {
#if __BYTEORDER == __LIL_ENDIAN
    return file_write( fp, buffer, n << 1 ) >> 1;
#else
    int i;
    for ( i = 0; i < n; i++ )
        if ( !file_writeSint16( fp, ( int16_t * )buffer++ ) ) break;
    return i;
#endif
}

int file_writeUint16A( file * fp, uint16_t * buffer, int n ) {
    return file_writeSint16A( fp, ( int16_t * )buffer, n );
}

int file_writeSint32A( file * fp, int32_t * buffer, int n ) {
#if __BYTEORDER == __LIL_ENDIAN
    return file_write( fp, buffer, n << 2 ) >> 2;
#else
    int i;
    for ( i = 0; i < n; i++ )
        if ( !file_writeSint32( fp, ( int32_t * )buffer++ ) ) break;
    return i;
#endif
}

int file_writeUint32A( file * fp, uint32_t * buffer, int n ) {
    return file_writeSint32A( fp, ( int32_t * )buffer, n );
}

int file_writeSint64A( file * fp, int64_t * buffer, int n ) {
#if __BYTEORDER == __LIL_ENDIAN
    return file_write( fp, buffer, n << 3 ) >> 3;
#else
    int i;
    for ( i = 0; i < n; i++ )
        if ( !file_writeSint64( fp, ( int64_t * )buffer++ ) ) break;
    return i;
#endif
}

int file_writeUint64A( file * fp, uint64_t * buffer, int n ) {
    return file_writeSint64A( fp, ( int64_t * )buffer, n );
}

/* Read an int data from a binary file */

int file_readSint8( file * fp, int8_t * buffer ) {
    return file_read( fp, buffer, sizeof( int8_t ) );
}

int file_readUint8( file * fp, uint8_t * buffer ) {
    return file_read( fp, buffer, sizeof( int8_t ) );
}

int file_readSint16( file * fp, int16_t * buffer ) {
#if __BYTEORDER == __LIL_ENDIAN
    return file_read( fp, buffer, sizeof( int16_t ) );
#else
    file_read( fp, ( uint8_t * )buffer + 1, 1 );
    return file_read( fp, ( uint8_t * )buffer, 1 );
#endif
}

int file_readUint16( file * fp, uint16_t * buffer ) {
    return file_readSint16( fp, (int16_t *)buffer );
}

int file_readSint32( file * fp, int32_t * buffer ) {
#if __BYTEORDER == __LIL_ENDIAN
    return file_read( fp, buffer, sizeof( int32_t ) );
#else
    file_read( fp, ( uint8_t * )buffer + 3, 1 );
    file_read( fp, ( uint8_t * )buffer + 2, 1 );
    file_read( fp, ( uint8_t * )buffer + 1, 1 );
    return file_read( fp, ( uint8_t * )buffer, 1 );
#endif
}

int file_readUint32( file * fp, uint32_t * buffer ) {
    return file_readSint32( fp, ( int32_t * )buffer );
}

int file_readSint64( file * fp, int64_t * buffer ) {
#if __BYTEORDER == __LIL_ENDIAN
    return file_read( fp, buffer, sizeof( int64_t ) );
#else
    file_read( fp, ( uint8_t * )buffer + 7, 1 );
    file_read( fp, ( uint8_t * )buffer + 6, 1 );
    file_read( fp, ( uint8_t * )buffer + 5, 1 );
    file_read( fp, ( uint8_t * )buffer + 4, 1 );
    file_read( fp, ( uint8_t * )buffer + 3, 1 );
    file_read( fp, ( uint8_t * )buffer + 2, 1 );
    file_read( fp, ( uint8_t * )buffer + 1, 1 );
    return file_read( fp, ( uint8_t * )buffer, 1 );
#endif
}

int file_readUint64( file * fp, uint64_t * buffer ) {
    return file_readSint64( fp, ( int64_t * )buffer );
}

/* Read an array from a binary file */

int file_readSint8A( file * fp, int8_t * buffer, int n ) {
    return file_read( fp, buffer, n );
}

int file_readUint8A( file * fp, uint8_t * buffer, int n ) {
    return file_read( fp, buffer, n );
}

int file_readSint16A( file * fp, int16_t * buffer, int n ) {
#if __BYTEORDER == __LIL_ENDIAN
    return file_read( fp, buffer, n << 1 ) >> 1;
#else
    int i;
    for ( i = 0; i < n; i++ )
        if ( !file_readSint16( fp, ( int16_t * )buffer++ ) ) break;
    return i;
#endif
}

int file_readUint16A( file * fp, uint16_t * buffer, int n ) {
    return file_readSint16A( fp, ( int16_t * )buffer, n );
}

int file_readSint32A( file * fp, int32_t * buffer, int n ) {
#if __BYTEORDER == __LIL_ENDIAN
    return file_read( fp, buffer, n << 2 ) >> 2;
#else
    int i;
    for ( i = 0; i < n; i++ )
        if ( !file_readSint32( fp, ( int32_t * )buffer++ ) ) break;
    return i;
#endif
}

int file_readUint32A( file * fp, uint32_t * buffer, int n ) {
    return file_readSint32A( fp, ( int32_t * )buffer, n );
}

int file_readSint64A( file * fp, int64_t * buffer, int n ) {
#if __BYTEORDER == __LIL_ENDIAN
    return file_read( fp, buffer, n << 3 ) >> 3;
#else
    int i;
    for ( i = 0; i < n; i++ )
        if ( !file_readSint64( fp, ( int64_t * )buffer++ ) ) break;
    return i;
#endif
}

int file_readUint64A( file * fp, uint64_t * buffer, int n ) {
    return file_readSint64A( fp, ( int64_t * )buffer, n );
}

/* Write a datablock to a file */

int file_write( file * fp, void * buffer, int len ) {
    if ( fp->type == F_XFILE ) return 0; // this don't must happen

    if ( fp->type == F_GZFILE ) {
        int result = gzwrite( fp->gz, buffer, len );
        fp->error = ( result < len );
        if ( result < 0 ) result = 0;
        return result;
    }

    return fwrite( buffer, 1, len, fp->fp );
}

/* Return file size */

long file_size( file * fp ) {
    long pos, size;

    if ( !fp ) return 0;

    if ( fp->type == F_XFILE ) return fp->xf->size;

    pos = file_pos( fp );
    file_seek(fp, 0, SEEK_END );
    size = file_pos( fp );

    file_seek(fp, pos, SEEK_SET );

    return size;
}

/* Get current file pointer position */

long file_pos( file * fp ) {
    if ( fp->type == F_XFILE ) return fp->pos - fp->xf->offset;

    if ( fp->type == F_GZFILE ) return gztell( fp->gz );

    return ftell( fp->fp );
}

int file_flush( file * fp ) {
    if ( fp->type == F_XFILE ) return 0;

    if ( fp->type == F_GZFILE ) return 0;

    return fflush( fp->fp );
}

/* Set current file pointer position */

int file_seek( file * fp, long pos, int where ) {
    assert( fp );
    if ( fp->type == F_XFILE ) {
        if ( where == SEEK_END )        pos = fp->xf->size - pos + 1;
        else if ( where == SEEK_CUR )   pos += ( fp->pos - fp->xf->offset );

        if ( fp->xf->size < pos ) pos = fp->xf->size;

        if ( pos < 0 ) pos = 0;

        fp->pos = pos + fp->xf->offset;
        return pos;
    }

    if ( fp->type == F_GZFILE ) {
        assert( fp->gz );
        if ( where == SEEK_END ) {
            char buffer[1024];
            while ( !file_eof( fp ) ) file_read( fp, buffer, sizeof(buffer) );
            fp->error = 0;
            pos += file_pos( fp );
            if ( pos < 0 ) pos = 0;
            where = SEEK_SET;
        }
        return gzseek( fp->gz, pos, where );
    }

    assert( fp->fp );
    return fseek( fp->fp, pos, where );
}

void file_rewind( file * fp ) {
    fp->error = 0;

    switch ( fp->type ) {
        case F_XFILE:
            fp->pos = fp->xf->offset;
            break;

        case F_GZFILE:
            gzrewind( fp->gz );
            break;

        default:
            rewind( fp->fp );
    }
}

/* Open file */

static int open_raw( file * f, const char * filename, const char * mode ) {
    char    _mode[5];
    char    *p;

    if ( !strchr( mode, '0' ) ) {
        f->type = F_GZFILE;
        f->gz = gzopen( filename, mode );
        f->eof  = 0;
        if ( f->gz ) return 1;
    }

    p = _mode;
    while ( *mode ) {
        if ( *mode != '0' ) {
            *p = *mode;
            p++;
        }
        mode++;
    }
    *p = '\0';

    f->eof  = 0;
    f->type = F_FILE;
    f->fp = fopen( filename, _mode );
    if ( f->fp ) return 1;
    return 0;
}

file * file_open( const char * filename, char * mode ) {
    char work [__MAX_PATH];
    char here [__MAX_PATH];

    char * name = NULL;
    char * p, c;
    int i;

    file * f;

    f = ( file * ) calloc( 1, sizeof( file ) );
    assert( f );

    p = f->name;
    while ( *filename ) {
        *p++ = *filename++;
        if ( p[-1] == '\\' ) p[-1] = '/'; /* Unix style */
    }
    p[0] = '\0';

    filename = f->name;

    if ( open_raw( f, filename, mode ) ) {
        opened_files++;
        return f;
    }

    /* if real file don't exists in disk */
    if (  strchr( mode, 'r' ) &&  strchr( mode, 'b' ) &&  /* Only read-only files */
         !strchr( mode, '+' ) && !strchr( mode, 'w' ) )
    {
        for ( i = 0; i < x_files_count; i++ ) {
            if ( !strcmp( filename, x_file[i].name ) ) {
                f->eof  = 0;
                f->pos  = x_file[i].offset;
                f->type = F_XFILE;
                f->xf   = &x_file[i];
                f->gz = gzopen( x_file[i].stubname, "rb" );

                opened_files++;
                return f;
            }
        }
    }

    p = name = work;
    while (( c = *filename ) ) {
        if ( c == '\\' || c == '/' ) {
            c = 0;
            name = p + 1;
        }
        *p++ = c;
        filename++;
    }
    *p = '\0';

    /* Use file extension for search in a directory named as it (for example: FPG dir for .FPG files) */
    if ( strchr( name, '.' ) ) {
        strcpy( here, strrchr( name, '.' ) + 1 );
        strcat( here, PATH_SEP );
        strcat( here, name );
        if ( open_raw( f, here, mode ) ) {
            opened_files++;
            return f;
        }
    }

    for ( i = 0; possible_paths[i]; i++ ) {
        strcpy( here, possible_paths[i] );
        strcat( here, name );
        if ( open_raw( f, here, mode ) ) {
            opened_files++;
            return f;
        }
    }

    free( f );
    return 0;
}

/* Close file */

void file_close( file * fp ) {
    if ( fp == NULL ) return;
    if ( fp->type == F_FILE ) fclose( fp->fp );
    if ( fp->type == F_GZFILE || fp->type == F_XFILE ) gzclose( fp->gz );

    opened_files--;
    free( fp );
}

/* Add a new dir to PATH */

void file_addp( const char * path ) {
    char truepath[__MAX_PATH];
    int n;

    if ( !path || !*path ) return;

    strcpy( truepath, path );

    for ( n = 0; truepath[n]; n++ ) if ( truepath[n] == '\\' ) truepath[n] = '/';
    if ( truepath[strlen( truepath )-1] != '/' ) strcat( truepath, "/" );

    for ( n = 0; n < MAX_POSSIBLE_PATHS - 2 && possible_paths[n]; n++ );

    possible_paths[n] = strdup( truepath );
    possible_paths[n+1] = NULL;
}

/* --- */

int file_remove( const char * filename ) {
    return ( remove( filename ) );
}

/* --- */

int file_move( const char * source_file, const char * target_file ) {
    return ( rename( source_file, target_file ) );
}

/* Check for file exists */

int file_exists( const char * filename ) {
    file * fp;

    fp = file_open( filename, "rb" );
    if ( fp )
    {
        file_close( fp );
        return 1;
    }
    return 0;
}

/* Check for file end is reached */

int file_eof( file * fp ) {
    if ( fp->type == F_XFILE ) {
        return fp->eof ? 1 : 0;
    }

    if ( fp->type == F_GZFILE ) {
        if ( fp->error ) return 1;
        return gzeof( fp->gz ) ? 1 : 0;
    }

    return feof( fp->fp ) ? 1 : 0;
}

/* ------------------------------------------------------------------------------------ */

char * getfullpath( char *rel_path ) {
    char fullpath[ __MAX_PATH ] = "";
#ifdef _WIN32
    char * fpath = NULL;
    DWORD sz = GetFullPathName( rel_path, sizeof( fullpath ), fullpath, NULL );
    if ( sz > sizeof( fullpath ) ) {
        fpath = malloc( sz + 1 );
        if ( fpath ) {
            if ( GetFullPathName( rel_path, sz, fpath, NULL ) ) return fpath;
            free( fpath );
        }
        return NULL;
    }
    if ( sz ) return strdup( fullpath );
    return NULL;
#else
    char * r = realpath( rel_path, fullpath );
    (void) &r; // avoid compiler warning
    if ( !*fullpath ) return NULL;
    return strdup( fullpath );
#endif
}

/* ------------------------------------------------------------------------------------ */

#ifdef _WIN32
    #define ENV_PATH_SEP    ';'
#else
    #define ENV_PATH_SEP    ':'
#endif

char * whereis( const char *file ) {
    char * path = getenv( "PATH" ), *pact = path, *p;
    char fullname[ __MAX_PATH ];

    while ( pact && *pact ) {
        struct stat st;

        if ( ( p = strchr( pact, ENV_PATH_SEP ) ) ) *p = '\0';
        sprintf( fullname, "%s%s%s", pact, ( pact[ strlen( pact ) - 1 ] == ENV_PATH_SEP ) ? "" : PATH_SEP, file );

        if ( !stat( fullname, &st ) && S_ISREG( st.st_mode ) ) {
            pact = strdup( pact );
            if ( p ) *p = ENV_PATH_SEP;
            return ( pact );
        }

        if ( !p ) break;

        *p = ENV_PATH_SEP;
        pact = p + 1;
    }

    return NULL;
}

/* ------------------------------------------------------------------------------------ */
