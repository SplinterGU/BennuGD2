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
#ifndef WIN32
#include <unistd.h>
#else
#include <direct.h>
#endif
#include "bgdrtm.h"
#include "dcb.h"
#include "dirs.h"
#include "files.h"
#include "xstrings.h"

#define SYSPROCS_ONLY_DECLARE
#include "sysprocs.h"

/* --------------------------------------------------------------------------- */

void * globaldata = 0;
void * localdata  = 0;
int64_t local_size = 0;
int64_t * localstr = 0;
int64_t local_strings = 0;

PROCDEF * procs = NULL;
PROCDEF * mainproc = NULL;
int64_t procdef_count = 0;

/* ---------------------------------------------------------------------- */

DCB_SYSPROC_CODE2 * sysproc_code_ref = NULL;

/* ---------------------------------------------------------------------- */

void sysprocs_fixup( void ) {
    SYSPROC * proc = sysprocs;
    DCB_SYSPROC_CODE2 * s = NULL;
    int64_t n;

    while ( proc->func ) {
        proc->code = -1;

        s = sysproc_code_ref;
        for ( n = 0; n < dcb.data.NSysProcsCodes; n++, s++ ) {
            if (
                proc->type == s->Type && proc->params == s->Params &&
                s->Id == getid( proc->name ) && !strcmp( (const char *)s->ParamTypes, proc->paramtypes ) )
            {
                proc->code = s->Code;
                break;
            }
        }
        proc++;
    }
}

/* ---------------------------------------------------------------------- */

PROCDEF * procdef_get( int64_t n ) {
    if ( n >= 0 && n < procdef_count ) return &procs[n];
    return NULL;
}

/* ---------------------------------------------------------------------- */

PROCDEF * procdef_get_by_name( char * name ) {
    int64_t n;
    for ( n = 0; n < procdef_count; n++ ) if ( strcmp( procs[n].name, name ) == 0 ) return &procs[n];
    return NULL;
}

/* ---------------------------------------------------------------------- */

DCB_HEADER dcb;

/* ---------------------------------------------------------------------- */

static char * trim( char * ptr ) {
    char * ostr = ptr, * bptr = ptr;
    while ( *ptr == ' ' || *ptr == '\n' || *ptr == '\r' || *ptr == '\t' ) ptr++;
    while ( *ptr ) *bptr++ = *ptr++;
    while ( bptr > ostr && ( bptr[-1] == ' ' || bptr[-1] == '\n' || bptr[-1] == '\r' || bptr[-1] == '\t' ) ) bptr--;
    *bptr = 0;
    return ( ostr );
}

/* ---------------------------------------------------------------------- */

static int load_file( const char * filename, int n ) {
    char line[2048], ** lines;
    int allocated = 16, count = 0, i;
    file * fp;

    fp = file_open( filename, "r0" );
    if ( !fp ) {
        dcb.sourcefiles[n] = 0;
        dcb.sourcelines[n] = 0;
        dcb.sourcecount[n] = 0;
        return 0;
    }

    lines = ( char ** ) calloc( allocated, sizeof( char* ) );
    if ( !lines ) {
        file_close( fp );
        return -1;
    }

    while ( !file_eof( fp ) ) {
        file_qgets( fp, line, 2048 );
        trim( line );
        if ( allocated == count ) {
            char ** l;
            l = realloc( lines, sizeof( char* ) * ( allocated + 16 ) );
            if ( !l) {
                for ( i = 0; i < count; i++ ) free( lines[ i ] );
                free( lines );
                file_close( fp );
                return -1;
            }
            lines = l;
            allocated += 16;
        }
        lines[count] = strdup( line );
        if ( !lines[count]) {
            for ( i = 0; i < count; i++ ) free( lines[ i ] );
            free( lines );
            file_close( fp );
            return -1;
        }
        count++;
    }
    file_close( fp );

    dcb.sourcefiles[n] = ( uint8_t * ) strdup( filename );
    if ( !dcb.sourcefiles[n] ) {
        for ( i = 0; i < count; i++ ) free( lines[ i ] );
        free( lines );
        file_close( fp );
        return -1;
    }
    dcb.sourcelines[n] = ( uint8_t ** ) lines;
    dcb.sourcecount[n] = count;
    return 1;
}

/* ---------------------------------------------------------------------- */

int dcb_load( const char * filename ) {
    file * fp;

    /* check for existence of the DCB FILE */
    if ( !file_exists( filename ) ) return 0;

    fp = file_open( filename, "rb" );
    if ( !fp ) {
        fprintf( stderr, "ERROR: Runtime error - Could not open file (%s)\n", filename );
        exit( 1 );
    }

    return dcb_load_from( fp, filename, 0 );
}

/* ---------------------------------------------------------------------- */

void arrange_varspace_data( DCB_VAR * basevar, int nvars, char * basedata ) {
    unsigned int n;

    /* Search for a variable */
    for ( n = 0; n < nvars; n++ ) {
        switch ( basevar[ n ].Type.BaseType[ 0 ] ) {
            case TYPE_ARRAY:
            {
                int count = 1;
                int i = 0;
                for ( i = 0; i < MAX_TYPECHUNKS; i++ ) {
                    if ( basevar[ n ].Type.BaseType[ i ] != TYPE_ARRAY ) break;
                    count *= basevar[ n ].Type.Count[ i ];
                }

                switch( basevar[ n ].Type.BaseType[ i ] ) {
                    case TYPE_QWORD:
                    case TYPE_INT:
                    case TYPE_DOUBLE:
                    case TYPE_POINTER:
                    case TYPE_STRING:
                        ARRANGE_QWORDS( ( uint8_t* )basedata + basevar[ n ].Offset, count );
                        break;

                    case TYPE_DWORD:
                    case TYPE_INT32:
                    case TYPE_FLOAT:
                        ARRANGE_DWORDS( ( uint8_t* )basedata + basevar[ n ].Offset, count );
                        break;

                    case TYPE_WORD:
                    case TYPE_SHORT:
                        ARRANGE_WORDS( ( uint8_t* )basedata + basevar[ n ].Offset, count );
                        break;

                    case TYPE_BYTE:
                    case TYPE_SBYTE:
                    case TYPE_CHAR:
                        break;

                    case TYPE_STRUCT:
                    {
                        uint8_t * data = ( uint8_t* )basedata + basevar[ n ].Offset;
                        int64_t tsize = typedef_size( basevar[ n ].Type ) / count;
                        for ( int m = 0; m < count; m++ ) {
                            arrange_varspace_data( dcb.varspace_vars[ basevar[ n ].Type.Members ], dcb.varspace[ basevar[ n ].Type.Members ].NVars, data );
                            data += tsize;
                        }
                        break;
                    }

                }
                break;
            }

            case TYPE_QWORD:
            case TYPE_INT:
            case TYPE_DOUBLE:
            case TYPE_POINTER:
            case TYPE_STRING:
                ARRANGE_QWORD( ( uint8_t* )basedata + basevar[ n ].Offset );
                break;

            case TYPE_DWORD:
            case TYPE_INT32:
            case TYPE_FLOAT:
                ARRANGE_DWORD( ( uint8_t* )basedata + basevar[ n ].Offset );
                break;

            case TYPE_WORD:
            case TYPE_SHORT:
                ARRANGE_WORD( ( uint8_t* )basedata + basevar[ n ].Offset );
                break;

            case TYPE_BYTE:
            case TYPE_SBYTE:
            case TYPE_CHAR:
                break;

            case TYPE_STRUCT:
                arrange_varspace_data( dcb.varspace_vars[ basevar[ n ].Type.Members ], dcb.varspace[ basevar[ n ].Type.Members ].NVars, ( uint8_t* )basedata + basevar[ n ].Offset );
                break;

            default:
                break;
        }
    }
}

/* ---------------------------------------------------------------------- */

DCB_VAR * read_and_arrange_varspace( file * fp, int count ) {
    int n, n1;
    DCB_VAR * vars = ( DCB_VAR * ) calloc( count, sizeof( DCB_VAR ) );
    if ( !vars ) {
        fprintf( stderr, "ERROR: Runtime error - %s: out of memory\n", __FUNCTION__ );
        exit(2);
    }

    for ( n = 0; n < count; n++ ) {
        file_read( fp, &vars[n], sizeof( DCB_VAR ) );
        ARRANGE_QWORD( &vars[n].ID );
        ARRANGE_QWORD( &vars[n].Offset );
        for ( n1 = 0; n1 < MAX_TYPECHUNKS; n1++ ) ARRANGE_QWORD( &vars[n].Type.Count[n1] );
        ARRANGE_QWORD( &vars[n].Type.Members );
    }

    return vars;
}

/* ---------------------------------------------------------------------- */

void remove_parent_refs(char *path) {
    char *src = path, *dst = path;

    while (*src) {
        // If we see "../", skip it
        if (src[0] == '.' && src[1] == '.' && (src[2] == '/' || src[2] == '\\')) {
            src += 3;
        } else {
            *dst++ = *src++;
        }
    }
    *dst = '\0';
}

/* ---------------------------------------------------------------------- */


int dcb_load_from( file * fp, const char * filename, int offset ) {
    unsigned int n;
    uint64_t size;

    /* Read dcb contents */

    file_seek( fp, offset, SEEK_SET );
    file_read( fp, &dcb, sizeof( DCB_HEADER_DATA ) );

    ARRANGE_QWORD( &dcb.data.Version );
    ARRANGE_QWORD( &dcb.data.NProcs );
    ARRANGE_QWORD( &dcb.data.NFiles );
    ARRANGE_QWORD( &dcb.data.NID );
    ARRANGE_QWORD( &dcb.data.NStrings );
    ARRANGE_QWORD( &dcb.data.NLocVars );
    ARRANGE_QWORD( &dcb.data.NLocStrings );
    ARRANGE_QWORD( &dcb.data.NGloVars );

    ARRANGE_QWORD( &dcb.data.NVarSpaces );

    ARRANGE_QWORD( &dcb.data.SGlobal );
    ARRANGE_QWORD( &dcb.data.SLocal );
    ARRANGE_QWORD( &dcb.data.SText );

    ARRANGE_QWORD( &dcb.data.NImports );

    ARRANGE_QWORD( &dcb.data.NSourceFiles );

    ARRANGE_QWORD( &dcb.data.NSysProcsCodes );

    ARRANGE_QWORD( &dcb.data.OProcsTab );
    ARRANGE_QWORD( &dcb.data.OID );
    ARRANGE_QWORD( &dcb.data.OStrings );
    ARRANGE_QWORD( &dcb.data.OText );
    ARRANGE_QWORD( &dcb.data.OGlobal );
    ARRANGE_QWORD( &dcb.data.OGloVars );
    ARRANGE_QWORD( &dcb.data.OLocal );
    ARRANGE_QWORD( &dcb.data.OLocVars );
    ARRANGE_QWORD( &dcb.data.OLocStrings );
    ARRANGE_QWORD( &dcb.data.OVarSpaces );
    ARRANGE_QWORD( &dcb.data.OFilesTab );
    ARRANGE_QWORD( &dcb.data.OImports );

    ARRANGE_QWORD( &dcb.data.OSourceFiles );
    ARRANGE_QWORD( &dcb.data.OSysProcsCodes );

    if ( memcmp( dcb.data.Header, DCB_MAGIC, sizeof( DCB_MAGIC ) - 1 ) != 0 || dcb.data.Version < 0x0800 ) return 0;

    globaldata = calloc( dcb.data.SGlobal + 8, 1 );
    localdata  = calloc( dcb.data.SLocal + 8, 1 );
    localstr   = ( int64_t * ) calloc( dcb.data.NLocStrings + 8, sizeof( int64_t ) );
    dcb.proc   = ( DCB_PROC * ) calloc(( 1 + dcb.data.NProcs ), sizeof( DCB_PROC ) );
    procs      = ( PROCDEF * ) calloc(( 1 + dcb.data.NProcs ), sizeof( PROCDEF ) );

    if ( !globaldata || !localdata || !localstr || !dcb.proc || !procs ) {
        fprintf( stderr, "ERROR: Runtime error - %s: out of memory\n", __FUNCTION__ );
        exit(2);
    }

    procdef_count = dcb.data.NProcs;
    local_size    = dcb.data.SLocal;
    local_strings = dcb.data.NLocStrings;

    /* Retrieves global data areas */

    file_seek( fp, offset + dcb.data.OGlobal, SEEK_SET );
    file_read( fp, globaldata, dcb.data.SGlobal );         /* **** */

    file_seek( fp, offset + dcb.data.OLocal, SEEK_SET );
    file_read( fp, localdata, dcb.data.SLocal );           /* **** */

    if ( dcb.data.NLocStrings ) {
        file_seek( fp, offset + dcb.data.OLocStrings, SEEK_SET );
        file_readUint64A( fp, (uint64_t *)localstr, dcb.data.NLocStrings );
    }

    file_seek( fp, offset + dcb.data.OProcsTab, SEEK_SET );
    for ( n = 0; n < dcb.data.NProcs; n++ ) {
        file_read( fp, &dcb.proc[n], sizeof( DCB_PROC_DATA ) );

        ARRANGE_QWORD( &dcb.proc[n].data.ID );
        ARRANGE_QWORD( &dcb.proc[n].data.Flags );
        ARRANGE_QWORD( &dcb.proc[n].data.NParams );

        ARRANGE_QWORD( &dcb.proc[n].data.NPriVars );
        ARRANGE_QWORD( &dcb.proc[n].data.NPriStrings );

        ARRANGE_QWORD( &dcb.proc[n].data.NPubVars );
        ARRANGE_QWORD( &dcb.proc[n].data.NPubStrings );

        ARRANGE_QWORD( &dcb.proc[n].data.NSentences );

        ARRANGE_QWORD( &dcb.proc[n].data.SPrivate );
        ARRANGE_QWORD( &dcb.proc[n].data.SPublic );

        ARRANGE_QWORD( &dcb.proc[n].data.SCode );

        ARRANGE_QWORD( &dcb.proc[n].data.OExitCode );
        ARRANGE_QWORD( &dcb.proc[n].data.OErrorCode );

        ARRANGE_QWORD( &dcb.proc[n].data.OSentences );

        ARRANGE_QWORD( &dcb.proc[n].data.OPriVars );
        ARRANGE_QWORD( &dcb.proc[n].data.OPriStrings );
        ARRANGE_QWORD( &dcb.proc[n].data.OPrivate );

        ARRANGE_QWORD( &dcb.proc[n].data.OPubVars );
        ARRANGE_QWORD( &dcb.proc[n].data.OPubStrings );
        ARRANGE_QWORD( &dcb.proc[n].data.OPublic );

        ARRANGE_QWORD( &dcb.proc[n].data.OCode );
    }

    /* Retrieves strings */

    dcb.data.OStrings += offset;
    dcb.data.OText += offset;

    string_load( fp, dcb.data.OStrings, dcb.data.OText, dcb.data.NStrings, dcb.data.SText );

    /* Retrieves included files */

    if ( dcb.data.NFiles ) {
        DCB_FILE dcbfile;
        char fname[__MAX_PATH];

        xfile_init( dcb.data.NFiles );
        file_seek( fp, offset + dcb.data.OFilesTab, SEEK_SET );
        for ( n = 0; n < dcb.data.NFiles; n++ ) {
            file_read( fp, &dcbfile, sizeof( DCB_FILE ) );

            ARRANGE_QWORD( &dcbfile.SName );
            ARRANGE_QWORD( &dcbfile.SFile );
            ARRANGE_QWORD( &dcbfile.OFile );

            file_read( fp, &fname, dcbfile.SName );
            remove_parent_refs(fname);
            file_add_xfile( fp, filename, offset + dcbfile.OFile, fname, dcbfile.SFile );
        }
    }

    /* Retrieves the imports */

    if ( dcb.data.NImports ) {
        dcb.imports = ( uint64_t * )calloc( dcb.data.NImports, sizeof( uint64_t ) );
        if ( !dcb.imports ) {
            fprintf( stderr, "ERROR: Runtime error - %s: out of memory\n", __FUNCTION__ );
            exit(2);
        }
        file_seek( fp, offset + dcb.data.OImports, SEEK_SET );
        file_readUint64A( fp, dcb.imports, dcb.data.NImports );
    }

    /* Retrieves debugging data */

    if ( dcb.data.NID ) {
        dcb.id = ( DCB_ID * ) calloc( dcb.data.NID, sizeof( DCB_ID ) );
        if ( !dcb.id ) {
            fprintf( stderr, "ERROR: Runtime error - %s: out of memory\n", __FUNCTION__ );
            exit(2);
        }
        file_seek( fp, offset + dcb.data.OID, SEEK_SET );

        for ( n = 0; n < dcb.data.NID; n++ ) {
            file_read( fp, &dcb.id[n], sizeof( DCB_ID ) );
            ARRANGE_QWORD( &dcb.id[n].Code );
        }
    }

    if ( dcb.data.NGloVars ) {
        file_seek( fp, offset + dcb.data.OGloVars, SEEK_SET );
        dcb.glovar = read_and_arrange_varspace( fp, dcb.data.NGloVars );
    }

    if ( dcb.data.NLocVars ) {
        file_seek( fp, offset + dcb.data.OLocVars, SEEK_SET );
        dcb.locvar = read_and_arrange_varspace( fp, dcb.data.NLocVars );
    }

    if ( dcb.data.NVarSpaces ) {
        dcb.varspace = ( DCB_VARSPACE * ) calloc( dcb.data.NVarSpaces, sizeof( DCB_VARSPACE ) );
        dcb.varspace_vars = ( DCB_VAR ** ) calloc( dcb.data.NVarSpaces, sizeof( DCB_VAR * ) );

        if ( !dcb.varspace || !dcb.varspace_vars ) {
            fprintf( stderr, "ERROR: Runtime error - %s: out of memory\n", __FUNCTION__ );
            exit(2);
        }

        file_seek( fp, offset + dcb.data.OVarSpaces, SEEK_SET );

        for ( n = 0; n < dcb.data.NVarSpaces; n++ ) {
            file_read( fp, &dcb.varspace[n], sizeof( DCB_VARSPACE ) );
            ARRANGE_QWORD( &dcb.varspace[n].NVars );
            ARRANGE_QWORD( &dcb.varspace[n].OVars );
        }

        for ( n = 0; n < dcb.data.NVarSpaces; n++ ) {
            dcb.varspace_vars[n] = 0;
            if ( !dcb.varspace[n].NVars ) continue;
            file_seek( fp, offset + dcb.varspace[n].OVars, SEEK_SET );
            dcb.varspace_vars[n] = read_and_arrange_varspace( fp, dcb.varspace[n].NVars );
        }
    }

    if ( dcb.data.NGloVars ) arrange_varspace_data( dcb.glovar, dcb.data.NGloVars, globaldata );
    if ( dcb.data.NLocVars ) arrange_varspace_data( dcb.locvar, dcb.data.NLocVars, localdata );

    if ( dcb.data.NSourceFiles ) {
        char fname[__MAX_PATH];

        dcb.sourcecount = ( uint64_t * ) calloc( dcb.data.NSourceFiles, sizeof( uint64_t ) );
        dcb.sourcelines = ( uint8_t *** ) calloc( dcb.data.NSourceFiles, sizeof( uint8_t ** ) );
        dcb.sourcefiles = ( uint8_t ** ) calloc( dcb.data.NSourceFiles, sizeof( uint8_t * ) );

        if ( !dcb.sourcecount || !dcb.sourcelines || !dcb.sourcefiles ) {
            fprintf( stderr, "ERROR: Runtime error - %s: out of memory\n", __FUNCTION__ );
            exit(2);
        }

        file_seek( fp, offset + dcb.data.OSourceFiles, SEEK_SET );
        for ( n = 0; n < dcb.data.NSourceFiles; n++ ) {
            file_readUint64( fp, &size );
            file_read( fp, fname, size );
            switch ( load_file( fname, n ) ) {
                case 0:
                    fprintf( stdout, "WARNING: Runtime warning - file not found (%s)\n", fname );
                    break;

                case -1:
                    fprintf( stdout, "ERROR: Runtime error - no enough memory for load (%s)\n", fname );
                    exit(2);
                    break;
            }
        }
    }

    /* Retrieves processes */

    for ( n = 0; n < dcb.data.NProcs; n++ ) {
        procs[n].params             = dcb.proc[n].data.NParams;
        procs[n].string_count       = dcb.proc[n].data.NPriStrings;
        procs[n].pubstring_count    = dcb.proc[n].data.NPubStrings;
        procs[n].private_size       = dcb.proc[n].data.SPrivate;
        procs[n].public_size        = dcb.proc[n].data.SPublic;
        procs[n].code_size          = dcb.proc[n].data.SCode;
        procs[n].id                 = dcb.proc[n].data.ID;
        procs[n].flags              = dcb.proc[n].data.Flags;
        procs[n].type               = n;
        procs[n].name               = getid_name( procs[n].id );
        procs[n].breakpoint         = 0;

        if ( dcb.proc[n].data.SPrivate ) {
            procs[n].pridata = ( uint8_t * )calloc( dcb.proc[n].data.SPrivate, sizeof( uint8_t ) ); /* Size in bytes */
            if ( !procs[n].pridata ) {
                fprintf( stderr, "ERROR: Runtime error - %s: out of memory\n", __FUNCTION__ );
                exit(2);
            }
            file_seek( fp, offset + dcb.proc[n].data.OPrivate, SEEK_SET );
            file_read( fp, procs[n].pridata, dcb.proc[n].data.SPrivate );      /* *** */
        }

        if ( dcb.proc[n].data.SPublic ) {
            procs[n].pubdata = ( uint8_t * )calloc( dcb.proc[n].data.SPublic, sizeof( uint8_t ) ); /* Size in bytes */
            if ( !procs[n].pubdata ) {
                fprintf( stderr, "ERROR: Runtime error - %s: out of memory\n", __FUNCTION__ );
                exit(2);
            }
            file_seek( fp, offset + dcb.proc[n].data.OPublic, SEEK_SET );
            file_read( fp, procs[n].pubdata, dcb.proc[n].data.SPublic );       /* *** */
        }

        if ( dcb.proc[n].data.SCode ) {
            procs[n].code = ( int64_t * ) calloc( dcb.proc[n].data.SCode / sizeof( int64_t ), sizeof( int64_t ) ); /* Size in bytes */
            if ( !procs[n].code ) {
                fprintf( stderr, "ERROR: Runtime error - %s: out of memory\n", __FUNCTION__ );
                exit(2);
            }
            file_seek( fp, offset + dcb.proc[n].data.OCode, SEEK_SET );
            file_readUint64A( fp, (uint64_t *)procs[n].code, dcb.proc[n].data.SCode / sizeof(uint64_t) );

            if ( dcb.proc[n].data.OExitCode )   procs[n].exitcode = dcb.proc[n].data.OExitCode;
            else                                procs[n].exitcode = 0;

            if ( dcb.proc[n].data.OErrorCode )  procs[n].errorcode = dcb.proc[n].data.OErrorCode;
            else                                procs[n].errorcode = 0;
        }

        if ( dcb.proc[n].data.NPriStrings ) {
            procs[n].strings = ( int64_t * )calloc( dcb.proc[n].data.NPriStrings, sizeof( int64_t ) );
            if ( !procs[n].strings ) {
                fprintf( stderr, "ERROR: Runtime error - %s: out of memory\n", __FUNCTION__ );
                exit(2);
            }
            file_seek( fp, offset + dcb.proc[n].data.OPriStrings, SEEK_SET );
            file_readUint64A( fp, (uint64_t *)procs[n].strings, dcb.proc[n].data.NPriStrings );
        }

        if ( dcb.proc[n].data.NPubStrings ) {
            procs[n].pubstrings = ( int64_t * )calloc( dcb.proc[n].data.NPubStrings, sizeof( int64_t ) );
            if ( !procs[n].pubstrings ) {
                fprintf( stderr, "ERROR: Runtime error - %s: out of memory\n", __FUNCTION__ );
                exit(2);
            }
            file_seek( fp, offset + dcb.proc[n].data.OPubStrings, SEEK_SET );
            file_readUint64A( fp, (uint64_t *)procs[n].pubstrings, dcb.proc[n].data.NPubStrings );
        }

        if ( dcb.proc[n].data.NPriVars ) {
            file_seek( fp, offset + dcb.proc[n].data.OPriVars, SEEK_SET );
            dcb.proc[n].privar = read_and_arrange_varspace( fp, dcb.proc[n].data.NPriVars );
            arrange_varspace_data( dcb.proc[n].privar, dcb.proc[n].data.NPriVars, procs[n].pridata );
        }

        if ( dcb.proc[n].data.NPubVars ) {
            file_seek( fp, offset + dcb.proc[n].data.OPubVars, SEEK_SET );
            dcb.proc[n].pubvar = read_and_arrange_varspace( fp, dcb.proc[n].data.NPubVars );
            arrange_varspace_data( dcb.proc[n].pubvar, dcb.proc[n].data.NPubVars, procs[n].pubdata );
        }
    }

    /* Retrieves the fixup table for system procedures */

    sysproc_code_ref = calloc( dcb.data.NSysProcsCodes, sizeof( DCB_SYSPROC_CODE2 ) );
    if ( !sysproc_code_ref ) {
        fprintf( stderr, "ERROR: Runtime error - %s: out of memory\n", __FUNCTION__ );
        exit(2);
    }
    file_seek( fp, offset + dcb.data.OSysProcsCodes, SEEK_SET );
    for ( n = 0; n < dcb.data.NSysProcsCodes; n++ ) {
        DCB_SYSPROC_CODE sdcb;
        file_read( fp, &sdcb, sizeof( DCB_SYSPROC_CODE ) );

        ARRANGE_QWORD( &sdcb.Id );
        ARRANGE_DWORD( &sdcb.Type );
        ARRANGE_QWORD( &sdcb.Params );
        ARRANGE_QWORD( &sdcb.Code );

        sysproc_code_ref[n].Id = sdcb.Id;
        sysproc_code_ref[n].Type = sdcb.Type;
        sysproc_code_ref[n].Params = sdcb.Params;
        sysproc_code_ref[n].Code = sdcb.Code;
        sysproc_code_ref[n].ParamTypes = ( uint8_t * ) calloc( sdcb.Params + 1, sizeof( uint8_t ) );

        if ( !sysproc_code_ref[n].ParamTypes ) {
            fprintf( stderr, "ERROR: Runtime error - %s: out of memory\n", __FUNCTION__ );
            exit(2);
        }
        if ( sdcb.Params ) file_read( fp, sysproc_code_ref[n].ParamTypes, sdcb.Params );
    }

    sysprocs_fixup();

    mainproc = procdef_get_by_name( "MAIN" );

    return 1;
}

/* ---------------------------------------------------------------------- */

char * getid_name( int64_t code ) {
    int n;
    for ( n = 0; n < dcb.data.NID; n++ ) if ( dcb.id[n].Code == code ) return (char *)dcb.id[n].Name;
    return "(?)";
}

/* ---------------------------------------------------------------------- */

int64_t getid( char * name ) {
    int n;
    for ( n = 0; n < dcb.data.NID; n++ ) if ( strcmp( (const char *)dcb.id[n].Name, name ) == 0 ) return dcb.id[n].Code;
    return -1;
}

/* ---------------------------------------------------------------------- */
