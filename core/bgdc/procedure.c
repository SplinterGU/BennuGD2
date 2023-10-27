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
#include <string.h>
#include <stdlib.h>
#ifdef TARGET_BEOS
#include <posix/assert.h>
#else
#include <assert.h>
#endif

#include "bgdc.h"

/* ---------------------------------------------------------------------- */
/* Process and Code Block Manager. This module contains utility functions */
/* for creating processes and code blocks, as well as others used during  */
/* and after compilation.                                                 */
/* ---------------------------------------------------------------------- */

PROCDEF * mainproc = 0;
int64_t procdef_count = 0;

int64_t procdef_maxid = -1;
PROCDEF ** procs = 0;
int64_t procs_allocated = 0;

int64_t procdef_getid() {
    for ( int64_t i = 0; i <= procdef_maxid; i++ ) if ( !procs[i] ) return i;
    return ++procdef_maxid;
}

PROCDEF * procdef_new( int64_t typeid, int64_t id ) {
    PROCDEF * proc = ( PROCDEF * ) calloc( 1, sizeof( PROCDEF ) );
    int n;

    if (!proc) {
        fprintf( stdout, "procdef_new: out of memory\n" );
        exit (1);
    }

    proc->pridata = segment_new();
    proc->privars = varspace_new();

    proc->pubdata = segment_new();
    proc->pubvars = varspace_new();

    proc->params            = -1;
    proc->minparams         = -1;
    proc->defined           = 0;
    proc->declared          = 0;
    proc->type              = TYPE_QWORD;
    proc->flags             = 0;
    proc->imported          = 0;

    proc->sentence_count    = 0;
    proc->sentences         = 0;

    if ( typeid >= procs_allocated ) {
        procs_allocated = typeid + 15;
        procs = ( PROCDEF ** ) realloc( procs, sizeof( PROCDEF * ) * procs_allocated );
        if (!procs) {
            fprintf( stdout, "procdef_new: out of memory\n" );
            exit (1);
        }
    }
    proc->typeid            = typeid;
    proc->identifier        = id;
    procs[typeid]           = proc;

    for ( n = 0; n < MAX_PARAMS; n++ ) proc->paramtype[n] = TYPE_UNDEFINED;

    proc->exitcode          = 0;
    proc->errorcode         = 0;

    codeblock_init( &proc->code );
    procdef_count++;
    return proc;
}

PROCDEF * procdef_search( int64_t id ) {
    int n;
    for ( n = 0; n <= procdef_maxid; n++ ) if ( procs[n]->identifier == id ) return procs[n];
    return 0;
}

PROCDEF * procdef_search_by_codeblock( CODEBLOCK * p ) {
    int n;
    for ( n = 0; n <= procdef_maxid; n++ ) if ( &procs[n]->code == p ) return procs[n];
    return 0;
}

PROCDEF * procdef_get (int64_t typeid) {
    return procs_allocated > typeid ? procs[typeid] : 0;
}

void procdef_destroy (PROCDEF * proc) {
    varspace_destroy( proc->privars );
    segment_destroy( proc->pridata );

    varspace_destroy( proc->pubvars );
    segment_destroy( proc->pubdata );

    procs[proc->typeid] = 0;
    free( proc->code.data );
    free( proc->code.loops );
    free( proc->code.labels );
    free( proc );

    procdef_count--;
}

/* ---------------------------------------------------------------------- */
/* Performs post-compilation actions on the code:                         */
/* - Converts label code jumps to offsets                                 */
/* - Converts process identifiers to CALL or TYPE to typeid               */
/* ---------------------------------------------------------------------- */

void program_postprocess() {
    int n;
    for ( n = 0; n <= procdef_maxid; n++ ) codeblock_postprocess( &procs[n]->code );
}

void program_dumpprocesses() {
    int n;
    for ( n = 0; n <= procdef_maxid; n++ ) procdef_dump( procs[n] );
}

void procdef_dump( PROCDEF * proc ) {
    printf( "\n\n---------- Process %" PRId64 " (%s)\n\n", proc->typeid, identifier_name( proc->identifier ) );

    if ( proc->privars->count ) {
        printf( "---- Private variables\n" );
        varspace_dump( proc->privars, 0 );
        printf( "\n" );
    }

    if ( proc->pubvars->count ) {
        printf( "---- Public variables\n" );
        varspace_dump( proc->pubvars, 0 );
        printf( "\n" );
    }

    /* segment_dump  (proc->pridata); */
    codeblock_dump( &proc->code );
}
