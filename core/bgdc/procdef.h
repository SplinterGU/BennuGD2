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

#ifndef __PROCDEF_H
#define __PROCDEF_H

/* Processes (a "PROCDEF" is, in fact, just its definition) */

#include "segment.h"
#include "varspace.h"
#include "codeblock.h"
#include "typedef.h"

#define MAX_PARAMS          256

#define PROC_USES_FRAME     0x01
#define PROC_USES_LOCALS    0x02
#define PROC_FUNCTION       0x04
#define PROC_USES_PUBLICS   0x08

typedef struct _sentence {
    int64_t file;
    int64_t line;
    int64_t col;
    int64_t offset;
} SENTENCE;

typedef struct _procdef {
    VARSPACE    * privars;
    segment     * pridata;

    VARSPACE    * pubvars;
    segment     * pubdata;

    int64_t     typeid;
    int64_t     identifier;
    int64_t     params;
    int64_t     defined;
    int64_t     declared;
    int64_t     flags;

    int64_t     imported; /* this proc is a libproc */

    int64_t     paramname[MAX_PARAMS];
    BASETYPE    paramtype[MAX_PARAMS];
    BASETYPE    type;

    CODEBLOCK   code;

    int64_t     exitcode;
    int64_t     errorcode;

    SENTENCE    * sentences;
    int64_t     sentence_count;
} PROCDEF;

extern int64_t procdef_count;
extern int64_t procdef_maxid;

extern int64_t procdef_getid();
extern PROCDEF * procdef_new( int64_t typeid, int64_t identifier );
extern PROCDEF * procdef_get( int64_t typeid );
extern PROCDEF * procdef_search( int64_t identifier );
extern PROCDEF * procdef_search_by_codeblock( CODEBLOCK * p );
extern void procdef_destroy( PROCDEF * );

extern void procdef_dump( PROCDEF * proc );

/* The "main" process, the first one to be defined and executed */
extern PROCDEF * mainproc;

extern void program_dumpprocesses();

#endif
