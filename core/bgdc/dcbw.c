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

#include <stdlib.h>

#ifdef TARGET_BEOS
#include <posix/assert.h>
#else
#include <assert.h>
#endif

#include <stdio.h>
#include <string.h>

#include "bgdc.h"
#include "dcb.h"

#define SYSPROCS_ONLY_DECLARE
#include "sysprocs.h"

#define FREEM(m)    free(m); (m)=NULL;

/* dirty declare external vars */

extern char * string_mem;
extern int64_t * string_offset;
extern int64_t string_count, string_used;
extern int64_t procdef_count;
extern int64_t identifier_count;
extern PROCDEF ** procs;

extern int64_t debug;

/* List of files to include in DCB file managment */

DCB_FILE * dcb_files = 0;
int64_t dcb_ef_alloc = 0;
int64_t dcb_filecount = 0;
char ** dcb_fullname = 0;
int64_t dcb_options = 0;

void dcb_add_file( const char * filename ) {
    file * fp;
    long   size;
    int64_t i;
    char buffer[1024];

    if ( filename[0] == '@' ) {
        fp = file_open( filename + 1, "rb" );
        if ( !fp ) return;
        while ( !file_eof( fp ) ) {
            char *p;
            file_qgets( fp, buffer, sizeof( buffer ) );
            if ( ( p = strchr( buffer, '\n' ) ) ) *p = '\0';
            if ( ( p = strchr( buffer, '\r' ) ) ) *p = '\0';
            if ( buffer[0] == '#' || !buffer[0] ) continue;
            dcb_add_file( buffer );
        }
        file_close( fp );
        return;
    }

    /* Dinamyc libraries aren't included in the stub */
/*
    if ( strlen( filename ) > 4 && !strncmp( filename + strlen( filename ) - 4, ".dll", 4 ) )   return;
    if ( strlen( filename ) > 3 && !strncmp( filename + strlen( filename ) - 3, ".so", 3 ) )    return;
    if ( strlen( filename ) > 6 && !strncmp( filename + strlen( filename ) - 6, ".dylib", 6 ) ) return;
*/
    for ( i = 0; filename[i]; i++ ) if ( filename[i] == '\\' ) buffer[i] = '/'; else buffer[i] = filename[i];
    buffer[i] = '\0';

    filename = ( const char * ) buffer;

    fp = file_open( filename, "rb" );
    if ( !fp ) return;
    size = file_size( fp );
    file_close( fp );

    if ( dcb_ef_alloc == dcb_filecount ) {
        dcb_files = ( DCB_FILE * )realloc( dcb_files, sizeof( DCB_FILE ) * ( dcb_ef_alloc += 16 ) );
        if ( !dcb_files ) compile_error( "dcb_add_file: out of memory" );
    }

    /* file is already included ? */

    for ( i = 0; i < dcb_filecount; i++ )
        if ( strcmp( filename, (const char *)dcb_files[i].Name ) == 0 ) return;

    dcb_files[dcb_filecount].Name = (uint8_t *) strdup( filename );
    dcb_files[dcb_filecount].SFile = size;

    dcb_filecount++;
}

/* Hack for set ID's to varspaces */

VARSPACE ** dcb_orig_varspace = 0;
int64_t dcb_varspaces = 0;

static int dcb_varspace( VARSPACE * v ) {
    int64_t n, result;

    for ( n = 0; n < dcb_varspaces; n++ )
        if ( dcb_orig_varspace[n] == v ) return n;

    dcb.varspace = ( DCB_VARSPACE * ) realloc( dcb.varspace, sizeof( DCB_VARSPACE ) * ( dcb_varspaces + 1 ) );
    dcb_orig_varspace = ( VARSPACE ** ) realloc( dcb_orig_varspace, sizeof( VARSPACE * ) * ( dcb_varspaces + 1 ) );

    dcb_orig_varspace[dcb_varspaces] = v;

    dcb.varspace[dcb_varspaces].NVars = v->count;   ARRANGE_QWORD( &dcb.varspace[dcb_varspaces].NVars ); /* Used Only for dcb */
    result = dcb_varspaces++;

    for ( n = 0; n < v->count; n++ )
        if ( v->vars[n].type.varspace ) dcb_varspace( v->vars[n].type.varspace );

    return result;
}

/* TYPEDEF converts (used in compile time) */

void dcb_settype( DCB_TYPEDEF * d, TYPEDEF * t ) {
    int64_t n;

    for ( n = 0; n < MAX_TYPECHUNKS; n++ ) {
        d->BaseType[n] = t->chunk[n].type;          /* 8 bits */
        d->Count   [n] = t->chunk[n].count;
    }

    if ( t->varspace ) d->Members = dcb_varspace( t->varspace );
    else               d->Members = NO_MEMBERS;
}

/* TYPEDEF converts (used in save time) */

void dcb_prepare_type( DCB_TYPEDEF * d, TYPEDEF * t ) {
    int64_t n;

    for ( n = 0; n < MAX_TYPECHUNKS; n++ ) {
        d->BaseType[n] = t->chunk[n].type;          /* 8 bits */
        d->Count   [n] = t->chunk[n].count;         ARRANGE_QWORD( &d->Count[n] );
    }

    if ( t->varspace ) d->Members = dcb_varspace( t->varspace );
    else               d->Members = NO_MEMBERS;

    ARRANGE_QWORD( &d->Members );
}

/* Make DCB file (see dcb.h) */

DCB_HEADER dcb;

int dcb_save( const char * filename, int options, const char * stubname ) {
    long offset, stubsize = 0;
    uint64_t n, i, size;
    identifier * id;
    void * stubdata;
    file * fp;

    int64_t NPriVars = 0,
            NPubVars = 0,
            SCode    = 0,
            SPrivate = 0,
            SPublic  = 0,
            OFilesTab = 0;

    DCB_SYSPROC_CODE sdcb;
    SYSPROC * s;
    int64_t NSysProcs = 0;

    fp = file_open( filename, "wb0" );
    if ( !fp ) {
        fprintf( stdout, "ERROR: can't open %s\n", filename );
        return 0;
    }

    /* Write the stub file */
    if ( stubname ) {
        file * stub = file_open( stubname, "rb0" );

        if ( !stub ) {
            fprintf( stdout, "ERROR: can't open %s\n", stubname );
            return 0;
        }

        stubsize = file_size( stub );
        stubdata = calloc( stubsize, sizeof( char ) );
        if ( !stubdata ) {
            fprintf( stdout, "ERROR: Stub %s too big\n", stubname );
            return 0;
        }
        if ( file_read( stub, stubdata, stubsize ) != stubsize ) {
            fprintf( stdout, "ERROR: error reading stub %s\n", stubname );
            return 0;
        }
        file_write( fp, stubdata, stubsize );
        if ( stubsize & 15 ) {
            file_write( fp, stubdata, 16 - ( stubsize & 15 ) );
            stubsize += 16 - ( stubsize & 15 );
        }
        free( stubdata );
        file_close( stub );
    }

    /* Refresh sysprocs list */

    for ( s = sysprocs; s->name; s++ ) {
        if ( s->id == 0 ) s->id = ( int64_t ) identifier_search_or_add( s->name );
        NSysProcs++;
    }

    /* We can asume that dcb.varspace is already filled */

    /* Splinter, fix to save neccessary info only */
    if ( !( options & DCB_DEBUG ) ) n_files = 0;

    /* 1. Fill header */

    if ( libmode ) memcpy( dcb.data.Header, DCL_MAGIC, sizeof( DCL_MAGIC ) - 1 );
    else           memcpy( dcb.data.Header, DCB_MAGIC, sizeof( DCB_MAGIC ) - 1 );

    dcb.data.Version        = DCB_VERSION;                                      ARRANGE_QWORD( &dcb.data.Version );

    dcb.data.NProcs         = procdef_count;                                    ARRANGE_QWORD( &dcb.data.NProcs );
    dcb.data.NFiles         = dcb_filecount;                                    ARRANGE_QWORD( &dcb.data.NFiles );
    dcb.data.NID            = identifier_count;                                 ARRANGE_QWORD( &dcb.data.NID );
    dcb.data.NStrings       = string_count;                                     ARRANGE_QWORD( &dcb.data.NStrings );
    dcb.data.NLocVars       = local.count;                                      ARRANGE_QWORD( &dcb.data.NLocVars );
    dcb.data.NLocStrings    = local.stringvar_count;                            ARRANGE_QWORD( &dcb.data.NLocStrings );
    dcb.data.NGloVars       = global.count;                                     ARRANGE_QWORD( &dcb.data.NGloVars );

    dcb.data.SGlobal        = globaldata->current;                              ARRANGE_QWORD( &dcb.data.SGlobal );
    dcb.data.SLocal         = localdata->current;                               ARRANGE_QWORD( &dcb.data.SLocal );
    dcb.data.SText          = string_used;                                      ARRANGE_QWORD( &dcb.data.SText );

    dcb.data.NImports       = nimports;                                         ARRANGE_QWORD( &dcb.data.NImports );

    dcb.data.NSourceFiles   = n_files;                                          ARRANGE_QWORD( &dcb.data.NSourceFiles );

    dcb.data.NSysProcsCodes = NSysProcs;                                        ARRANGE_QWORD( &dcb.data.NSysProcsCodes );

    /* 2. Build process table */

    dcb.proc = ( DCB_PROC * ) calloc( procdef_count, sizeof( DCB_PROC ) );

    for ( n = 0; n < procdef_count; n++ ) {
        dcb.proc[n].data.ID          = procs[n]->identifier;                    ARRANGE_QWORD( &dcb.proc[n].data.ID );
        dcb.proc[n].data.Flags       = procs[n]->flags;                         ARRANGE_QWORD( &dcb.proc[n].data.Flags );
        dcb.proc[n].data.NParams     = procs[n]->params;                        ARRANGE_QWORD( &dcb.proc[n].data.NParams );

        dcb.proc[n].data.NPriVars    = procs[n]->privars->count;                ARRANGE_QWORD( &dcb.proc[n].data.NPriVars );
        dcb.proc[n].data.NPriStrings = procs[n]->privars->stringvar_count;      ARRANGE_QWORD( &dcb.proc[n].data.NPriStrings );

        dcb.proc[n].data.NPubVars    = procs[n]->pubvars->count;                ARRANGE_QWORD( &dcb.proc[n].data.NPubVars );
        dcb.proc[n].data.NPubStrings = procs[n]->pubvars->stringvar_count;      ARRANGE_QWORD( &dcb.proc[n].data.NPubStrings );

        dcb.proc[n].data.NSentences  = procs[n]->sentence_count;                ARRANGE_QWORD( &dcb.proc[n].data.NSentences );

        dcb.proc[n].data.SPrivate    = procs[n]->pridata->current;              ARRANGE_QWORD( &dcb.proc[n].data.SPrivate );
        dcb.proc[n].data.SPublic     = procs[n]->pubdata->current;              ARRANGE_QWORD( &dcb.proc[n].data.SPublic );

        dcb.proc[n].data.SCode       = procs[n]->code.current * sizeof( uint64_t ); ARRANGE_QWORD( &dcb.proc[n].data.SCode );

        dcb.proc[n].data.OExitCode   = procs[n]->exitcode ;                     ARRANGE_QWORD( &dcb.proc[n].data.OExitCode );
        dcb.proc[n].data.OErrorCode  = procs[n]->errorcode ;                    ARRANGE_QWORD( &dcb.proc[n].data.OErrorCode );

        SCode    += procs[n]->code.current * 8;

        NPriVars += procs[n]->privars->count;
        NPubVars += procs[n]->pubvars->count;

        SPrivate += procs[n]->pridata->current;
        SPublic  += procs[n]->pubdata->current;

        dcb.proc[n].sentence = ( DCB_SENTENCE * ) calloc( procs[n]->sentence_count, sizeof( DCB_SENTENCE ) );

        for ( i = 0; i < procs[n]->sentence_count; i++ ) {
            dcb.proc[n].sentence[i].NFile = 0;                                  ARRANGE_QWORD( &dcb.proc[n].sentence[i].NFile );
            dcb.proc[n].sentence[i].NLine = procs[n]->sentences[i].line;        ARRANGE_QWORD( &dcb.proc[n].sentence[i].NLine );
            dcb.proc[n].sentence[i].NCol  = procs[n]->sentences[i].col;         ARRANGE_QWORD( &dcb.proc[n].sentence[i].NCol );
            dcb.proc[n].sentence[i].OCode = procs[n]->sentences[i].offset;      ARRANGE_QWORD( &dcb.proc[n].sentence[i].OCode );
        }

        /* Splinter, tipos de parametros */

        dcb.proc[n].privar = ( DCB_VAR * ) calloc( procs[n]->privars->count, sizeof( DCB_VAR ) );
        for ( i = 0; i < procs[n]->privars->count; i++ ) {
            dcb_prepare_type( &dcb.proc[n].privar[i].Type, &procs[n]->privars->vars[i].type );
            dcb.proc[n].privar[i].ID     = procs[n]->privars->vars[i].code;     ARRANGE_QWORD( &dcb.proc[n].privar[i].ID );
            dcb.proc[n].privar[i].Offset = procs[n]->privars->vars[i].offset;   ARRANGE_QWORD( &dcb.proc[n].privar[i].Offset );
        }

        dcb.proc[n].pubvar = ( DCB_VAR * ) calloc( procs[n]->pubvars->count, sizeof( DCB_VAR ) );
        for ( i = 0; i < procs[n]->pubvars->count; i++ ) {
            dcb_prepare_type( &dcb.proc[n].pubvar[i].Type, &procs[n]->pubvars->vars[i].type );
            dcb.proc[n].pubvar[i].ID     = procs[n]->pubvars->vars[i].code;     ARRANGE_QWORD( &dcb.proc[n].pubvar[i].ID );
            dcb.proc[n].pubvar[i].Offset = procs[n]->pubvars->vars[i].offset;   ARRANGE_QWORD( &dcb.proc[n].pubvar[i].Offset );
        }
    }

    /* 3. Build global tables */

    dcb.id = ( DCB_ID * ) calloc( identifier_count, sizeof( DCB_ID ) );

    id = identifier_first();

    for ( n = 0; n < identifier_count; n++ ) {
        assert( id != 0 );
        strncpy( (char *)dcb.id[n].Name, id->name, sizeof( dcb.id[n].Name ) );
        dcb.id[n].Code = id->code;                                              ARRANGE_QWORD( &dcb.id[n].Code );
        id = identifier_next( id );
    }

    dcb.glovar = ( DCB_VAR * ) calloc( global.count, sizeof( DCB_VAR ) );
    dcb.locvar = ( DCB_VAR * ) calloc( local.count, sizeof( DCB_VAR ) );

    for ( n = 0; n < global.count; n++ ) {
        dcb_prepare_type( &dcb.glovar[n].Type, &global.vars[n].type );
        dcb.glovar[n].ID     = global.vars[n].code;                             ARRANGE_QWORD( &dcb.glovar[n].ID );
        dcb.glovar[n].Offset = global.vars[n].offset;                           ARRANGE_QWORD( &dcb.glovar[n].Offset );
    }

    for ( n = 0; n < local.count; n++ ) {
        dcb_prepare_type( &dcb.locvar[n].Type, &local.vars[n].type );
        dcb.locvar[n].ID     = local.vars[n].code;                              ARRANGE_QWORD( &dcb.locvar[n].ID );
        dcb.locvar[n].Offset = local.vars[n].offset;                            ARRANGE_QWORD( &dcb.locvar[n].Offset );
    }

    dcb.data.NVarSpaces = dcb_varspaces;

    dcb.file = dcb_files;

    /* 4. Calculate offsets */

    offset = sizeof( DCB_HEADER_DATA );

    dcb.data.OProcsTab      = offset; offset += sizeof( DCB_PROC_DATA ) * procdef_count;    ARRANGE_QWORD( &dcb.data.OProcsTab );
    dcb.data.OStrings       = offset; offset += sizeof( uint64_t ) * string_count;          ARRANGE_QWORD( &dcb.data.OStrings );
    dcb.data.OGloVars       = offset; offset += sizeof( DCB_VAR ) * global.count;           ARRANGE_QWORD( &dcb.data.OGloVars );
    dcb.data.OLocVars       = offset; offset += sizeof( DCB_VAR ) * local.count;            ARRANGE_QWORD( &dcb.data.OLocVars );
    dcb.data.OLocStrings    = offset; offset += sizeof( uint64_t ) * local.stringvar_count; ARRANGE_QWORD( &dcb.data.OLocStrings );
    dcb.data.OID            = offset; offset += sizeof( DCB_ID ) * identifier_count;        ARRANGE_QWORD( &dcb.data.OID );
    dcb.data.OVarSpaces     = offset; offset += sizeof( DCB_VARSPACE ) * dcb_varspaces;     ARRANGE_QWORD( &dcb.data.OVarSpaces );
    dcb.data.OText          = offset; offset += string_used;                                ARRANGE_QWORD( &dcb.data.OText );
    dcb.data.OImports       = offset; offset += sizeof( uint64_t ) * nimports;              ARRANGE_QWORD( &dcb.data.OImports );
    dcb.data.OGlobal        = offset; offset += globaldata->current;                        ARRANGE_QWORD( &dcb.data.OGlobal );
    dcb.data.OLocal         = offset; offset += localdata->current;                         ARRANGE_QWORD( &dcb.data.OLocal );

    dcb.data.OSourceFiles   = offset;                                                       ARRANGE_QWORD( &dcb.data.OSourceFiles );

    /* sources */
    for ( n = 0; n < n_files; n++ )
        offset += sizeof( uint64_t ) + strlen( files[n] ) + 1;

    dcb.data.OSysProcsCodes = offset;                                                       ARRANGE_QWORD( &dcb.data.OSysProcsCodes );
    for ( s = sysprocs; s->name; s++ ) {
        offset += sizeof( DCB_SYSPROC_CODE );
        offset += s->params;
    }

    for ( n = 0; n < dcb_varspaces; n++ ) {
        dcb.varspace[n].OVars = offset;                                                     ARRANGE_QWORD( &dcb.varspace[n].OVars );
        offset += sizeof( DCB_VAR ) * dcb_orig_varspace[n]->count;
    }

    for ( n = 0; n < procdef_count; n++ ) {
        dcb.proc[n].data.OSentences  = offset; offset += sizeof( DCB_SENTENCE ) * procs[n]->sentence_count;     ARRANGE_QWORD( &dcb.proc[n].data.OSentences );

        /* Private */
        dcb.proc[n].data.OPriVars    = offset; offset += sizeof( DCB_VAR ) * procs[n]->privars->count;          ARRANGE_QWORD( &dcb.proc[n].data.OPriVars );
        dcb.proc[n].data.OPriStrings = offset; offset += 8 * procs[n]->privars->stringvar_count;                ARRANGE_QWORD( &dcb.proc[n].data.OPriStrings );
        dcb.proc[n].data.OPrivate    = offset; offset += procs[n]->pridata->current;                            ARRANGE_QWORD( &dcb.proc[n].data.OPrivate );

        /* Publics */
        dcb.proc[n].data.OPubVars    = offset; offset += sizeof( DCB_VAR ) * procs[n]->pubvars->count;          ARRANGE_QWORD( &dcb.proc[n].data.OPubVars );
        dcb.proc[n].data.OPubStrings = offset; offset += 8 * procs[n]->pubvars->stringvar_count;                ARRANGE_QWORD( &dcb.proc[n].data.OPubStrings );
        dcb.proc[n].data.OPublic     = offset; offset += procs[n]->pubdata->current;                            ARRANGE_QWORD( &dcb.proc[n].data.OPublic );

        /* Code */
        dcb.proc[n].data.OCode       = offset; offset += procs[n]->code.current * 8;                            ARRANGE_QWORD( &dcb.proc[n].data.OCode );
    }

    /* Archivos incluidos */
    OFilesTab = offset;
    dcb.data.OFilesTab   = OFilesTab;                                                       ARRANGE_QWORD( &dcb.data.OFilesTab );

    /* FullPathName */
    dcb_fullname = calloc( dcb_filecount, sizeof( char * ) );

    /* Cada uno de los archivos incluidos */
    for ( n = 0; n < dcb_filecount; n++ ) {
        dcb_fullname[n] = (char *)dcb.file[n].Name;                       /* Guardo el Name, porque lo destruyo en SName (es un union) */
        dcb.file[n].SName = strlen( (const char *)dcb.file[n].Name ) + 1; /* Todavia no hago el ARRANGE de este dato, lo hago luego cuando lo voy a grabar */
        offset += sizeof( DCB_FILE ) + dcb.file[n].SName;
    }

    /* Data de los archivos incluidos */
    for ( n = 0; n < dcb_filecount; n++ ) {
        dcb.file[n].OFile = offset;                                                         ARRANGE_QWORD( &dcb.file[n].OFile );
        offset += dcb.file[n].SFile;                                                        ARRANGE_QWORD( &dcb.file[n].SFile );
    }

    /* 5. Guardar todo en disco ordenadamente */

    file_write( fp, &dcb, sizeof( DCB_HEADER_DATA ) );

    for ( n = 0; n < procdef_count; n++ )
        file_write( fp, &dcb.proc[n], sizeof( DCB_PROC_DATA ) );

    file_writeUint64A( fp, (uint64_t *)string_offset, string_count );
    file_write( fp, dcb.glovar, sizeof( DCB_VAR ) * global.count );         /* Ya procesado el byteorder */
    file_write( fp, dcb.locvar, sizeof( DCB_VAR ) * local.count );          /* Ya procesado el byteorder */
    file_writeUint64A( fp, (uint64_t *)local.stringvars, local.stringvar_count );
    file_write( fp, dcb.id, sizeof( DCB_ID ) * identifier_count );          /* Ya procesado el byteorder */
    file_write( fp, dcb.varspace, sizeof( DCB_VARSPACE ) * dcb_varspaces ); /* Ya procesado el byteorder */
    file_write( fp, string_mem, string_used );                              /* No necesita byteorder */
    file_writeUint64A( fp, (uint64_t *)imports, nimports );
    file_write( fp, globaldata->bytes, globaldata->current );   /* ****** */
    file_write( fp, localdata->bytes, localdata->current );     /* ****** */

    if ( dcb_options & DCB_DEBUG ) {
        for ( n = 0; n < n_files; n++ ) {
            size = strlen( files[n] ) + 1;
            file_writeUint64( fp, &size );
            file_write( fp, files[n], size );
        }
    }

    for ( s = sysprocs; s->name; s++ ) {
        int64_t l = s->params;
        sdcb.Id = s->id;                                                                    ARRANGE_QWORD( &sdcb.Id );
        sdcb.Type = s->type;                                                                ARRANGE_QWORD( &sdcb.Type );
        sdcb.Params = l;                                                                    ARRANGE_QWORD( &sdcb.Params );
        sdcb.Code = s->code;                                                                ARRANGE_QWORD( &sdcb.Code );
        file_write( fp, &sdcb, sizeof( DCB_SYSPROC_CODE ) );
        file_write( fp, s->paramtypes, l );
    }

    for ( n = 0; n < dcb_varspaces; n++ ) {
        VARIABLE * var;
        DCB_VAR v;

        memset (&v, '\0', sizeof(v));

        var = &dcb_orig_varspace[n]->vars[0];

        for ( i = 0; i < dcb_orig_varspace[n]->count; i++, var++ ) {
            dcb_prepare_type( &v.Type, &var->type );
            v.ID     = var->code;                                                           ARRANGE_QWORD( &v.ID );
            v.Offset = var->offset;                                                         ARRANGE_QWORD( &v.Offset );
            file_write( fp, &v, sizeof( DCB_VAR ) );
        }
    }

    for ( n = 0; n < procdef_count; n++ ) {
        file_write( fp, dcb.proc[n].sentence, sizeof( DCB_SENTENCE ) * procs[n]->sentence_count );  /* Ya procesado el byteorder */

        /* Privadas */
        file_write( fp, dcb.proc[n].privar, sizeof( DCB_VAR ) * procs[n]->privars->count );         /* Ya procesado el byteorder */
        file_writeUint64A( fp, (uint64_t *)procs[n]->privars->stringvars, procs[n]->privars->stringvar_count );
        file_write( fp, procs[n]->pridata->bytes, procs[n]->pridata->current );                     /* ****** */

        /* Publicas */
        file_write( fp, dcb.proc[n].pubvar, sizeof( DCB_VAR ) * procs[n]->pubvars->count );         /* Ya procesado el byteorder */
        file_writeUint64A( fp, (uint64_t *)procs[n]->pubvars->stringvars, procs[n]->pubvars->stringvar_count );
        file_write( fp, procs[n]->pubdata->bytes, procs[n]->pubdata->current );                     /* ****** */

        /* Code */
        file_writeUint64A( fp, (uint64_t *)procs[n]->code.data, procs[n]->code.current );
    }

    /* Cada uno de los archivos incluidos */
    for ( n = 0; n < dcb_filecount; n++ ) {
        int64_t siz = dcb.file[n].SName;
        ARRANGE_QWORD( &dcb.file[n].SName );
        ARRANGE_QWORD( &dcb.file[n].SFile );
        ARRANGE_QWORD( &dcb.file[n].OFile );
        file_write( fp, &dcb.file[n], sizeof( DCB_FILE ) );
        file_write( fp, dcb_fullname[n], siz );
    }

    for ( n = 0; n < dcb_filecount; n++ ) {
        char buffer[8192];
        file * fp_r = file_open( dcb_fullname[n], "rb" );
        int64_t chunk_size, siz = 0;

        assert( fp_r );
        while ( !file_eof( fp_r ) ) {
            siz += chunk_size = file_read( fp_r, buffer, 8192 );
            file_write( fp, buffer, chunk_size );
            if ( chunk_size < 8192 ) break;
        }
        file_close( fp_r );
    }

    /* Write the stub signature */

    if ( stubname != NULL ) {
        dcb_signature dcb_signature;

        /* Voy al final del archivo */

        strcpy( dcb_signature.magic, DCB_STUB_MAGIC );
        dcb_signature.dcb_offset = ( int64_t )stubsize;

        ARRANGE_QWORD( &dcb_signature.dcb_offset );

        file_write( fp, &dcb_signature, sizeof( dcb_signature ) );
    }

    file_close( fp );

    /* 6. Mostrar estadísticas */

    printf( "\nFile %s compiled (%ld bytes):\n\n", filename, offset );
    printf( "  Processes              %16" PRId64 "\n", procdef_count );
    printf( "  Global data            %16" PRId64 " bytes\n", globaldata->current );
    printf( "  Local data             %16" PRId64 " bytes\n", localdata->current );
    printf( "  Private data           %16" PRId64 " bytes\n", SPrivate );
    printf( "  Public data            %16" PRId64 " bytes\n", SPublic );
    printf( "  Code                   %16" PRId64 " bytes\n", SCode );
    printf( "  System processes       %16" PRId64 "\n", NSysProcs );

    printf( "  Globals vars           %16" PRId64 "\n", global.count );
    printf( "  Locals vars            %16" PRId64 "\n", local.count );
    printf( "  Private vars           %16" PRId64 "\n", NPriVars );
    printf( "  Publics vars           %16" PRId64 "\n", NPubVars );
    printf( "  Identifiers            %16" PRId64 "\n", identifier_count );
    printf( "  Structs                %16" PRId64 "\n", dcb_varspaces );
    printf( "  Strings                %16" PRId64 " (%" PRId64 " bytes)\n", string_count, string_used );

    if ( dcb_filecount )
        printf( "  Files added            %8" PRId64 " (%" PRId64 " bytes)\n", dcb_filecount, offset - OFilesTab );

    printf( "\n" );

    return 1;
}

/* ---------------------------------------------------------------------------
 * load_dcb as library
 * --------------------------------------------------------------------------- */

int64_t * newid = NULL;
int64_t * stringid = NULL;
int64_t * fileid = NULL;

/* --------------------------------------------------------------------------- */

void * glodata  = NULL ;
void * locdata  = NULL ;

DCB_SYSPROC_CODE2 * sysproc_code_ref = NULL ;

/* ---------------------------------------------------------------------- */

typedef struct {
    int64_t offstart;
    int64_t offend;
    int64_t offnew;
} range;

range * glovaroffs = NULL;
range * locvaroffs = NULL;

VARSPACE * varspaces = NULL;

/* ---------------------------------------------------------------------- */

void dcb_varspaces_load( file * fp, VARSPACE * vs, VARSPACE * vs_array, int count ) {
    int n, m;
    DCB_VAR vars;
    VARIABLE var;
    TYPEDEF type;

    for ( n = 0; n < count; n++ ) {
        file_read( fp, &vars, sizeof( DCB_VAR ) ) ;
        ARRANGE_QWORD( &vars.ID );
        ARRANGE_QWORD( &vars.Offset );
        for ( m = 0; m < MAX_TYPECHUNKS; m++ ) ARRANGE_QWORD( &vars.Type.Count[m] );
        ARRANGE_QWORD( &vars.Type.Members );

        type.depth = 0;
        for ( m = 0; m < MAX_TYPECHUNKS; m++ ) {
            type.chunk[m].type = vars.Type.BaseType[m];
            type.chunk[m].count = vars.Type.Count[m];
            if ( type.chunk[m].type != TYPE_UNDEFINED ) type.depth++;
        }

        if ( vars.Type.Members != NO_MEMBERS ) type.varspace = &vs_array[vars.Type.Members];
        else                                   type.varspace = NULL;

        var.code = newid[vars.ID];
        var.type = type;
        var.offset = vars.Offset;
        varspace_add( vs, var );
    }
}

/* ---------------------------------------------------------------------- */

void dcb_vars_load( file * fp, VARSPACE * vs, segment * data, char * data_source, int count, range ** offs, int totalsize ) {
    int n, m;
    DCB_VAR * vars = calloc( count, sizeof( DCB_VAR ) );
    VARIABLE var, * v;
    TYPEDEF type;

    if ( offs ) *offs = calloc( count, sizeof( range ) );

    for ( n = 0; n < count; n++ ) {
        file_read( fp, &vars[n], sizeof( DCB_VAR ) ) ;
        ARRANGE_QWORD( &vars[n].ID );
        ARRANGE_QWORD( &vars[n].Offset );
        for ( m = 0; m < MAX_TYPECHUNKS; m++ ) ARRANGE_QWORD( &vars[n].Type.Count[m] );
        ARRANGE_QWORD( &vars[n].Type.Members );
    }

    for ( n = 0; n < count; n++ ) {
        type.depth = 0;

        for ( m = 0; m < MAX_TYPECHUNKS; m++ ) {
            type.chunk[m].type = vars[n].Type.BaseType[m];
            type.chunk[m].count = vars[n].Type.Count[m];
            if ( type.chunk[m].type != TYPE_UNDEFINED ) type.depth++;
        }

        if ( vars[n].Type.Members != NO_MEMBERS )
            type.varspace = &varspaces[vars[n].Type.Members];
        else
            type.varspace = NULL;

        var.code = newid[vars[n].ID];
        var.type = type;
        var.offset = data->current;

        if ( ( v = varspace_search( vs, newid[vars[n].ID] ) ) ) {
            int64_t sz;

            if ( !typedef_is_equal( v->type, type ) ) {
                printf( "ERROR: var %s already exits in varspace but is different\n", identifier_name( newid[vars[n].ID] ) );
                exit ( -1 );
            }

            sz = typedef_size( v->type );
            if ( sz & 0x0007 ) sz = ( sz & ~0x0007 ) + 8;

            if ( offs ) {
                (*offs)[n].offstart = vars[n].Offset;
                (*offs)[n].offend = vars[n].Offset + sz - 1;
                (*offs)[n].offnew = v->offset;
            }
        } else {
            int64_t sz;
            if ( n < count - 1 ) sz = vars[n+1].Offset - vars[n].Offset;
            else                 sz = totalsize - vars[n].Offset;

            segment_alloc(data, sz);
            var.offset = data->current;
            varspace_add( vs, var );

            if ( offs ) {
                (*offs)[n].offstart = vars[n].Offset;
                (*offs)[n].offend = vars[n].Offset + sz - 1;
                (*offs)[n].offnew = var.offset;
            }

            memmove( &((char *)data->bytes)[var.offset], &data_source[vars[n].Offset], sz );

            data->current += sz;
        }
    }

    free( vars );
}

/* ---------------------------------------------------------------------- */

int get_new_off( int off, range * offs, int noffs ) {
    int n;
    for ( n = 0; n < noffs; n++ ) if ( off >= offs[n].offstart && off <= offs[n].offend ) return off - offs[n].offstart + offs[n].offnew;
    return 0;
}

/* ---------------------------------------------------------------------- */

void codeblock_adjust(CODEBLOCK * code) {
    int64_t * ptr = code->data ;
    int n;

    while (ptr < code->data + code->current) {
        if (*ptr == MN_CALL || *ptr == MN_PROC || *ptr == MN_TYPE) {
            ptr[1] = newid[ptr[1]];
        }

        if (MN_TYPEOF(*ptr) == MN_STRING && (*ptr & MN_MASK) == MN_PUSH) {
            ptr[1] = stringid[ptr[1]];
        }

        if ( (*ptr & MN_MASK) == MN_GLOBAL || (*ptr & MN_MASK) == MN_GET_GLOBAL ) {
            ptr[1] = get_new_off( ptr[1], glovaroffs, dcb.data.NGloVars );
        }

        if ( (*ptr & MN_MASK) == MN_LOCAL) {
            ptr[1] = get_new_off( ptr[1], locvaroffs, dcb.data.NLocVars );
        }

        if ( (*ptr & MN_MASK) == MN_SENTENCE ) {
            if ( dcb.data.Version == 0x0700 ) ptr[1] = (ptr[1] & 0xfffff) | ( fileid[ptr[1]>>24] << 20 );
            else                              ptr[1] = (ptr[1] & 0xfffff) | ( fileid[ptr[1]>>20] << 20 );
        }

        if ( (*ptr & MN_MASK) == MN_SYSCALL || (*ptr & MN_MASK)  == MN_SYSPROC ) {
            DCB_SYSPROC_CODE2 * s = sysproc_code_ref ;
            int found = 0;
            for ( n = 0; n < dcb.data.NSysProcsCodes && !found; n++, s++ ) {
                if ( s->Code == ptr[1] ) {
                    SYSPROC * p = sysproc_get( newid[ s->Id ] );
                    for ( ; p && !found; p = p->next ) {
                        if ( p->type == s->Type && p->params == s->Params &&
                             !strcmp( (const char *)s->ParamTypes, p->paramtypes ) ) {
                            ptr[1] = p->code;
                            found = 1;
                        }
                    }
                }
            }
        }
        ptr += MN_PARAMS(*ptr)+1 ;
    }
}

/* ---------------------------------------------------------------------- */

int dcb_load_lib( const char * filename ) {
    unsigned int n ;
    file * fp ;
    PROCDEF * proc;

    /* check for existence of the DCB FILE */
    if ( !file_exists( filename ) ) return 0 ;

    fp = file_open( filename, "rb0" ) ;
    if ( !fp ) {
        fprintf( stderr, "ERROR: Runtime error - Could not open file (%s)\n", filename ) ;
        exit( 1 );
    }

    /* Read Header */

    file_seek( fp, 0, SEEK_SET );
    file_read( fp, &dcb, sizeof( DCB_HEADER_DATA ) ) ;

    ARRANGE_QWORD( &dcb.data.Version );
    ARRANGE_QWORD( &dcb.data.NProcs );
    ARRANGE_QWORD( &dcb.data.NFiles );
    ARRANGE_QWORD( &dcb.data.NID );
    ARRANGE_QWORD( &dcb.data.NStrings );
    ARRANGE_QWORD( &dcb.data.NLocVars );
    ARRANGE_QWORD( &dcb.data.NLocStrings );
    ARRANGE_QWORD( &dcb.data.NGloVars );

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

    if ( memcmp( dcb.data.Header, DCL_MAGIC, sizeof( DCL_MAGIC ) - 1 ) != 0 ) return 0 ;
    if ( dcb.data.Version < 0x0710 ) return -1 ;

    /* Load identifiers */

    if ( dcb.data.NID ) {
        dcb.id = ( DCB_ID * ) calloc( dcb.data.NID, sizeof( DCB_ID ) ) ;

        FREEM( newid );
        newid = calloc( dcb.data.NID, sizeof( int64_t ) );

        file_seek( fp, dcb.data.OID, SEEK_SET ) ;
        for ( n = 0; n < dcb.data.NID; n++ ) {
            file_read( fp, &dcb.id[n], sizeof( DCB_ID ) ) ;
            ARRANGE_QWORD( &dcb.id[n].Code );
            newid[dcb.id[n].Code] = identifier_search_or_add( ( const char * ) dcb.id[n].Name );
        }
        free( dcb.id );
    }

    /* Load strings */

    if ( dcb.data.NStrings ) {
        char * strdata = calloc( 1, dcb.data.SText );
        uint64_t * stroffs = calloc( dcb.data.NStrings, sizeof( uint64_t ) );

        FREEM( stringid );
        stringid = calloc( dcb.data.NStrings, sizeof( int64_t ) );

        file_seek(( file * )fp, dcb.data.OStrings, SEEK_SET ) ;
        file_readUint64A(( file * )fp, stroffs, dcb.data.NStrings ) ;

        file_seek(( file * )fp, dcb.data.OText, SEEK_SET ) ;
        file_read(( file * )fp, strdata, dcb.data.SText ) ;

        for ( n = 0; n < dcb.data.NStrings; n++ ) stringid[n] = string_new( &strdata[stroffs[n]] );

        free( strdata );
        free( stroffs );
    }

    /* Load imports */

    if ( dcb.data.NImports ) {
        dcb.imports = ( uint64_t * )calloc( dcb.data.NImports, sizeof( uint64_t ) ) ;
        file_seek( fp, dcb.data.OImports, SEEK_SET ) ;
        file_readUint64A( fp, dcb.imports, dcb.data.NImports ) ;
        for ( n = 0; n < dcb.data.NImports; n++ ) import_mod( ( char * ) string_get( stringid[dcb.imports[n]] ) );
        FREEM( dcb.imports );
    }

    /* Recupero tabla de fixup de sysprocs */

    sysproc_code_ref = calloc( dcb.data.NSysProcsCodes, sizeof( DCB_SYSPROC_CODE2 ) ) ;
    file_seek( fp, dcb.data.OSysProcsCodes, SEEK_SET ) ;
    for ( n = 0; n < dcb.data.NSysProcsCodes; n++ ) {
        DCB_SYSPROC_CODE sdcb;
        file_read( fp, &sdcb, sizeof( DCB_SYSPROC_CODE ) ) ;

        ARRANGE_QWORD( &sdcb.Id );
        ARRANGE_QWORD( &sdcb.Type );
        ARRANGE_QWORD( &sdcb.Params );
        ARRANGE_QWORD( &sdcb.Code );

        sysproc_code_ref[n].Id = sdcb.Id ;
        sysproc_code_ref[n].Type = sdcb.Type ;
        sysproc_code_ref[n].Params = sdcb.Params ;
        sysproc_code_ref[n].Code = sdcb.Code ;
        sysproc_code_ref[n].ParamTypes = ( uint8_t * ) calloc( sdcb.Params + 1, sizeof( char ) );
        if ( sdcb.Params ) file_read( fp, sysproc_code_ref[n].ParamTypes, sdcb.Params ) ;
    }

    /* Load sources */

    if ( dcb.data.NSourceFiles ) {
        char filename[__MAX_PATH] ;

        fileid = calloc( dcb.data.NSourceFiles, sizeof( int64_t ) );

        dcb.sourcecount = ( uint64_t * ) calloc( dcb.data.NSourceFiles, sizeof( uint64_t ) ) ;
        dcb.sourcelines = ( uint8_t *** ) calloc( dcb.data.NSourceFiles, sizeof( char ** ) ) ;
        dcb.sourcefiles = ( uint8_t ** ) calloc( dcb.data.NSourceFiles, sizeof( char * ) ) ;
        file_seek( fp, dcb.data.OSourceFiles, SEEK_SET ) ;
        for ( n = 0; n < dcb.data.NSourceFiles; n++ ) {
            int m;
            uint64_t size;
            file_readUint64( fp, &size ) ;
            file_read( fp, filename, size ) ;
            fileid[n] = -1;
            for( m = 0; m < n_files; m++ ) if ( !strcmp( filename, files[m] ) ) fileid[n] = m;
            if ( fileid[n] == -1 ) {
                strcpy( files[n_files], filename );
                fileid[n] = n_files++;
            }
        }
    }

    /* Load datas */

    glodata = ( void * ) calloc( dcb.data.SGlobal, 1 ) ;
    locdata = ( void * ) calloc( dcb.data.SLocal, 1 ) ;

    /* Recupera las zonas de datos globales */

    file_seek( fp, dcb.data.OGlobal, SEEK_SET ) ;
    file_read( fp, glodata, dcb.data.SGlobal ) ;        /* **** */

    file_seek( fp, dcb.data.OLocal, SEEK_SET ) ;
    file_read( fp, locdata, dcb.data.SLocal ) ;         /* **** */

    /* Varspaces Load */

    if ( dcb.data.NVarSpaces ) {
        varspaces = ( VARSPACE * ) calloc( dcb.data.NVarSpaces, sizeof( VARSPACE ) ) ;
        dcb.varspace = ( DCB_VARSPACE * ) calloc( dcb.data.NVarSpaces, sizeof( DCB_VARSPACE ) ) ;

        file_seek( fp, dcb.data.OVarSpaces, SEEK_SET ) ;
        for ( n = 0; n < dcb.data.NVarSpaces; n++ ) {
            file_read( fp, &dcb.varspace[n], sizeof( DCB_VARSPACE ) ) ;
            ARRANGE_QWORD( &dcb.varspace[n].NVars );
            ARRANGE_QWORD( &dcb.varspace[n].OVars );
        }

        for ( n = 0; n < dcb.data.NVarSpaces; n++ ) {
            if ( !dcb.varspace[n].NVars ) continue ;
            file_seek( fp, dcb.varspace[n].OVars, SEEK_SET ) ;
            dcb_varspaces_load( fp, &varspaces[n], varspaces, dcb.varspace[n].NVars );
        }
    }

    if ( dcb.data.NGloVars ) {
        file_seek( fp, dcb.data.OGloVars, SEEK_SET ) ;
        dcb_vars_load( fp, &global, globaldata, glodata, dcb.data.NGloVars, &glovaroffs, dcb.data.SGlobal );
    }

    if ( dcb.data.NLocVars ) {
        file_seek( fp, dcb.data.OLocVars, SEEK_SET ) ;
        dcb_vars_load( fp, &local, localdata, locdata, dcb.data.NLocVars, &locvaroffs, dcb.data.SLocal );
    }

    if ( dcb.data.NLocStrings ) {
        int64_t o, newoff, found, m;
        int64_t * locstr = ( int64_t * ) calloc( dcb.data.NLocStrings + 8, sizeof( int64_t ) ) ;

        file_seek( fp, dcb.data.OLocStrings, SEEK_SET ) ;
        file_readUint64A( fp, (uint64_t *)locstr, dcb.data.NLocStrings ) ;
        for ( m = 0; m < dcb.data.NLocStrings; m++ ) {
            newoff = get_new_off( locstr[m], locvaroffs, dcb.data.NLocStrings );
            for ( found = 0, o = 0; o < local.stringvar_count; o++ ) if ( newoff == local.stringvars[o] ) { found = 1; break; }
            if ( !found ) varspace_varstring( &local, newoff );
        }
        FREEM( locstr );
    }

    /* Load process */

    dcb.proc   = ( DCB_PROC * ) calloc(( 1 + dcb.data.NProcs ), sizeof( DCB_PROC ) ) ;

    file_seek( fp, dcb.data.OProcsTab, SEEK_SET ) ;
    for ( n = 0; n < dcb.data.NProcs; n++ ) {
        file_read( fp, &dcb.proc[n], sizeof( DCB_PROC_DATA ) ) ;

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

    for ( n = 0; n < dcb.data.NProcs; n++ ) {
        char * pridata = NULL, *pubdata = NULL;
        range * prioffs = NULL, * puboffs = NULL;

        if ( !dcb.proc[n].data.SCode ) {
            continue;
        } else if ( procdef_search( newid[dcb.proc[n].data.ID] ) ) {
            if ( debug ) {
                token.type = IDENTIFIER;
                token.code = newid[dcb.proc[n].data.ID];
                compile_warning(0, MSG_PROC_ALREADY_DEFINED);
            }
            continue;
        }

        proc = procdef_new(procdef_getid(), newid[dcb.proc[n].data.ID]);

        codeblock_alloc(&proc->code, dcb.proc[n].data.SCode);

        file_seek( fp, dcb.proc[n].data.OCode, SEEK_SET ) ;
        file_readUint64A( fp, (uint64_t *)proc->code.data, dcb.proc[n].data.SCode / sizeof(uint64_t) ) ;
        proc->code.current = dcb.proc[n].data.SCode / sizeof(uint64_t);

        if ( dcb.proc[n].data.OExitCode )   proc->exitcode = dcb.proc[n].data.OExitCode ;
        else                                proc->exitcode = 0 ;

        if ( dcb.proc[n].data.OErrorCode )  proc->errorcode = dcb.proc[n].data.OErrorCode ;
        else                                proc->errorcode = 0 ;

        proc->defined = 1;
        proc->declared = 1;
        proc->imported = 1;
        proc->flags = dcb.proc[n].data.Flags ;

        proc->params = dcb.proc[n].data.NParams ;

        if ( dcb.proc[n].data.SPrivate ) {
            pridata = calloc(dcb.proc[n].data.SPrivate, 1);
            file_seek( fp, dcb.proc[n].data.OPrivate, SEEK_SET ) ;
            file_read( fp, pridata, dcb.proc[n].data.SPrivate ) ;       /* *** */
        }

        if ( dcb.proc[n].data.SPublic ) {
            pubdata = calloc(dcb.proc[n].data.SPublic, 1);
            file_seek( fp, dcb.proc[n].data.OPublic, SEEK_SET ) ;
            file_read( fp, pubdata, dcb.proc[n].data.SPublic ) ;       /* *** */
        }

        if ( dcb.proc[n].data.NPriVars ) {
            prioffs = calloc( dcb.proc[n].data.NPriVars, sizeof( range ) );
            file_seek( fp, dcb.proc[n].data.OPriVars, SEEK_SET ) ;
            dcb_vars_load( fp, proc->privars, proc->pridata, pridata, dcb.proc[n].data.NPriVars, &prioffs, dcb.proc[n].data.SPrivate );
        }

        if ( dcb.proc[n].data.NPubVars ) {
            puboffs = calloc( dcb.proc[n].data.NPubVars, sizeof( range ) );
            file_seek( fp, dcb.proc[n].data.OPubVars, SEEK_SET ) ;
            dcb_vars_load( fp, proc->pubvars, proc->pubdata, pubdata, dcb.proc[n].data.NPubVars, &puboffs, dcb.proc[n].data.SPublic );
        }

        if ( dcb.proc[n].data.NPriStrings ) {
            int64_t * sv = ( int64_t * )calloc( dcb.proc[n].data.NPriStrings, sizeof( int64_t ) ) ;
            int64_t m;
            file_seek( fp, dcb.proc[n].data.OPriStrings, SEEK_SET ) ;
            file_readUint64A( fp, (uint64_t *)sv, dcb.proc[n].data.NPriStrings ) ;
            for ( m = 0; m < dcb.proc[n].data.NPriStrings; m++ ) varspace_varstring( proc->privars, get_new_off( sv[m], prioffs, dcb.proc[n].data.NPriVars ));
            FREEM( sv );
        }

        if ( dcb.proc[n].data.NPubStrings ) {
            int64_t * sv = ( int64_t * )calloc( dcb.proc[n].data.NPubStrings, sizeof( int64_t ) ) ;
            int64_t m;
            file_seek( fp, dcb.proc[n].data.OPubStrings, SEEK_SET ) ;
            file_readUint64A( fp, (uint64_t *)sv, dcb.proc[n].data.NPubStrings ) ;
            for ( m = 0; m < dcb.proc[n].data.NPubStrings; m++ ) varspace_varstring( proc->pubvars, get_new_off( sv[m], puboffs, dcb.proc[n].data.NPubVars ));
            FREEM( sv );
        }

        codeblock_adjust( &proc->code );

        FREEM( pridata )
        FREEM( pubdata )
        FREEM( prioffs )
        FREEM( puboffs )
    }

    FREEM( sysproc_code_ref );
    FREEM( fileid );
    FREEM( newid );
    FREEM( stringid );
    FREEM( glodata );
    FREEM( locdata );
    FREEM( glovaroffs );
    FREEM( locvaroffs );

    /* Recupera los ficheros incluídos */
/* N/A
    if ( dcb.data.NFiles )
    {
        DCB_FILE dcbfile;
        char fname[__MAX_PATH];

        xfile_init( dcb.data.NFiles );
        file_seek( fp, dcb.data.OFilesTab, SEEK_SET ) ;
        for ( n = 0 ; n < dcb.data.NFiles; n++ )
        {
            file_read( fp, &dcbfile, sizeof( DCB_FILE ) ) ;

            ARRANGE_QWORD( &dcbfile.SName );
            ARRANGE_QWORD( &dcbfile.SFile );
            ARRANGE_QWORD( &dcbfile.OFile );

            file_read( fp, &fname, dcbfile.SName ) ;
            file_add_xfile( fp, NULL, dcbfile.OFile, fname, dcbfile.SFile ) ;
        }
    }
*/
    file_close( fp );

    return 1 ;
}

/* ---------------------------------------------------------------------- */
