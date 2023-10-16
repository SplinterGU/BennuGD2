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
 * FILE        : dcb.h
 * DESCRIPTION : Data compiled block information
 *
 */

#ifndef __DCB_H
#define __DCB_H

#include <stdint.h>

#include "typedef.h"

#define DCB_DEBUG 1

#ifdef _MSC_VER
#pragma pack(push, 1)
#endif

/* GNU C option for pack struct */

#ifdef __GNUC__
#define __PACKED __attribute__ ((packed))
#pragma pack(1)
#else
#define __PACKED
#endif

/* Estructura del fichero .dcb */

/* Please update the version's high-number between versions */
#define DCB_VERSION 0x0800

#define DCL_MAGIC       "dcl\x0d\x0a\x1f\x00\x00"
#define DCB_MAGIC       "dcb\x0d\x0a\x1f\x00\x00"

#define DCB_STUB_MAGIC  "DCB Stub\x1a\x0d\x0a"

typedef struct {
    uint8_t     Name[60];
    uint64_t    Code;
} __PACKED DCB_ID;

#define DCB_FILE_COMPRESSED 1

typedef struct {
    union {
        uint8_t     * Name;
        uint64_t    SName;
    };
    uint8_t     Flags;
    uint64_t    SFile;
    uint64_t    OFile;
} __PACKED DCB_FILE;

typedef struct {
    uint64_t    NFile;
    uint64_t    NLine;
    uint64_t    NCol;
    uint64_t    OCode;
} __PACKED DCB_SENTENCE;

#define NO_MEMBERS       0xFFFFFFFF

typedef struct {
    uint8_t     BaseType [MAX_TYPECHUNKS];
    uint64_t    Count    [MAX_TYPECHUNKS];
    uint64_t    Members;
} __PACKED DCB_TYPEDEF;

typedef struct {
    DCB_TYPEDEF Type;      /* 40 bytes */
    uint64_t    ID;
    uint64_t    Offset;
    uint64_t    Varspace;
} __PACKED DCB_VAR;

typedef struct {
    uint64_t    NVars;
    uint64_t    OVars;
} __PACKED DCB_VARSPACE;

typedef struct {
    uint64_t    Id; /* Identifier of Name, Segment Key 1 */
    BASETYPE    Type; /* Segment Key 2 */
    uint64_t    Params; /* Segment Key 3 */
    uint64_t    Code;
} __PACKED DCB_SYSPROC_CODE;

/* Internal Use, see also DCB_SYSPROC_CODE */
typedef struct {
    uint64_t    Id;
    BASETYPE    Type;
    uint64_t    Params;
    uint64_t    Code;
    uint8_t     * ParamTypes;
} __PACKED DCB_SYSPROC_CODE2;

/* Header for each process */
typedef struct {
    uint64_t    ID;

    uint64_t    Flags;

    uint64_t    NParams;
    uint64_t    NPriVars;
    uint64_t    NPriStrings;
    uint64_t    NPubVars;
    uint64_t    NPubStrings;
    uint64_t    NSentences;

    uint64_t    SPrivate;
    uint64_t    SPublic;
    uint64_t    SCode;

    uint64_t    OPrivate;
    uint64_t    OPriVars;
    uint64_t    OPriStrings;

    uint64_t    OPublic;
    uint64_t    OPubVars;
    uint64_t    OPubStrings;

    uint64_t    OCode;
    uint64_t    OSentences;

    uint64_t    OExitCode;
    uint64_t    OErrorCode;
} __PACKED DCB_PROC_DATA;

/* Header of Each Process */
typedef struct {
    DCB_PROC_DATA   data; /* Structure loaded from the file */

    DCB_SENTENCE    * sentence;

    DCB_VAR         * privar;
    DCB_VAR         * pubvar;
} __PACKED DCB_PROC;

/*
typedef struct {
    uint64_t    ProcId;
    uint64_t    Identifier;
    uint64_t    Code;
} __PACKED DCB_LABEL;
*/

/* General Header of the File */

typedef struct {
    uint8_t     Header[8]; /* "DCB" */
    uint64_t    Version;   /* 0x0100 is version 1.0  */

    uint64_t    NProcs;
    uint64_t    NFiles;
    uint64_t    NID;
    uint64_t    NStrings;
    uint64_t    NLocStrings;
    uint64_t    NLocVars;
    uint64_t    NGloVars;
    uint64_t    NVarSpaces;

    uint64_t    SGlobal;
    uint64_t    SLocal;
    uint64_t    SText;

    uint64_t    NImports;
    uint64_t    NSourceFiles;
    uint64_t    NSysProcsCodes; /* For the fixup of SYSPROCS */
    uint64_t    __reserved1[2];

    uint64_t    OProcsTab;
    uint64_t    OID;
    uint64_t    OStrings;
    uint64_t    OText;
    uint64_t    OGlobal;
    uint64_t    OGloVars;
    uint64_t    OLocal;
    uint64_t    OLocVars;
    uint64_t    OLocStrings;
    uint64_t    OVarSpaces;
    uint64_t    OFilesTab;
    uint64_t    OImports;
    uint64_t    OSourceFiles;
    uint64_t    OSysProcsCodes; /* For the fixup of SYSPROCS */
    uint64_t    __reserved2[2];
} __PACKED DCB_HEADER_DATA;

/* General Header of the File */
typedef struct {
    DCB_HEADER_DATA data;   /* Structure loaded from the file */

    DCB_ID          * id;
    DCB_VAR         * glovar;
    DCB_VAR         * locvar;
    DCB_PROC        * proc;
    DCB_FILE        * file;
    DCB_VARSPACE    * varspace;
    DCB_VAR         ** varspace_vars;
    uint64_t        * imports;
    uint8_t         *** sourcelines;
    uint64_t        * sourcecount;
    uint8_t         ** sourcefiles;
} __PACKED DCB_HEADER;

typedef struct {
    uint8_t magic[12];
    int64_t dcb_offset;
} __PACKED dcb_signature;

#ifdef __GNUC__
#pragma pack()
#endif

#ifdef _MSC_VER
#pragma pack(pop)
#endif

extern DCB_HEADER dcb;

extern void sysprocs_fixup( void );
extern int64_t getid( char * name );

#endif
