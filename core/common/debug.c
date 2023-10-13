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
#include <inttypes.h>

#include "pslang.h"
#include "dcb.h"
#include "xstrings.h"

#ifndef __BGDRTM__
#include "bgdc.h"
#include "identifiers.h"
#else
#include "sysprocs_p.h"
#include "i_procdef.h"
#endif

/* ---------------------------------------------------------------------- */
/* Este modulo contiene funciones que muestran el equivalente             */
/* en texto de una instruccion o mnemonico                                */
/* ---------------------------------------------------------------------- */

struct {
    char *  name;
    int64_t code;
    int     params;
} mnemonics[] = {
    { "END"                         , MN_END                    , 0 },
    { "RETURN"                      , MN_RETURN                 , 0 },
    { "FRAME"                       , MN_FRAME                  , 0 },

    { "DUP"                         , MN_DUP                    , 0 },
    { "PUSH"                        , MN_PUSH                   , 1 },
    { "POP"                         , MN_POP                    , 0 },

    { "SWITCH"                      , MN_SWITCH                 , 0 },
    { "CASE"                        , MN_CASE                   , 0 },
    { "CASE_R"                      , MN_CASE_R                 , 0 },

    { "CALL"                        , MN_CALL                   , 1 },
    { "SYSCALL"                     , MN_SYSCALL                , 1 },
    { "PROCESS"                     , MN_PROC                   , 1 },
    { "SYSPROC"                     , MN_SYSPROC                , 1 },
    { "CLONE"                       , MN_CLONE                  , 1 },

    { "TYPE"                        , MN_TYPE                   , 1 },
    { "DEBUG"                       , MN_DEBUG                  , 0 },

    { "INDEX"                       , MN_INDEX                  , 1 },
    { "ARRAY"                       , MN_ARRAY                  , 1 },
    { "PRIVATE"                     , MN_PRIVATE                , 1 },
    { "LOCAL"                       , MN_LOCAL                  , 1 },
    { "GLOBAL"                      , MN_GLOBAL                 , 1 },
    { "REMOTE"                      , MN_REMOTE                 , 1 },
    { "PTR"                         , MN_PTR                    , 0 },

    { "GET_PRIVATE"                 , MN_GET_PRIV               , 1 },
    { "GET_LOCAL"                   , MN_GET_LOCAL              , 1 },
    { "GET_GLOBAL"                  , MN_GET_GLOBAL             , 1 },
    { "GET_REMOTE"                  , MN_GET_REMOTE             , 1 },

    { "JUMP"                        , MN_JUMP                   , 1 },
    { "JFLASE"                      , MN_JFALSE                 , 1 },
    { "JTRUE"                       , MN_JTRUE                  , 1 },
    { "JTFALSE"                     , MN_JTFALSE                , 1 },
    { "JTTRUE"                      , MN_JTTRUE                 , 1 },
    { "JNOCASE"                     , MN_JNOCASE                , 1 },

    { "NEG"                         , MN_NEG                    , 0 },
    { "NOT"                         , MN_NOT                    , 0 },

    { "MUL"                         , MN_MUL                    , 0 },
    { "DIV"                         , MN_DIV                    , 0 },
    { "ADD"                         , MN_ADD                    , 0 },
    { "SUB"                         , MN_SUB                    , 0 },
    { "MOD"                         , MN_MOD                    , 0 },
    { "ROR"                         , MN_ROR                    , 0 },
    { "ROL"                         , MN_ROL                    , 0 },
    { "AND"                         , MN_AND                    , 0 },
    { "OR"                          , MN_OR                     , 0 },
    { "XOR"                         , MN_XOR                    , 0 },

    { "EQ"                          , MN_EQ                     , 0 },
    { "NE"                          , MN_NE                     , 0 },
    { "GT"                          , MN_GT                     , 0 },
    { "LT"                          , MN_LT                     , 0 },
    { "GTE"                         , MN_GTE                    , 0 },
    { "LTE"                         , MN_LTE                    , 0 },

    { "POSTINC"                     , MN_POSTINC                , 1 },
    { "POSTDEC"                     , MN_POSTDEC                , 1 },
    { "INC"                         , MN_INC                    , 1 },
    { "DEC"                         , MN_DEC                    , 1 },

    { "LET"                         , MN_LET                    , 0 },
    { "VARADD"                      , MN_VARADD                 , 0 },
    { "VARSUB"                      , MN_VARSUB                 , 0 },
    { "VARMUL"                      , MN_VARMUL                 , 0 },
    { "VARDIV"                      , MN_VARDIV                 , 0 },
    { "VARMOD"                      , MN_VARMOD                 , 0 },
    { "VARXOR"                      , MN_VARXOR                 , 0 },
    { "VARAND"                      , MN_VARAND                 , 0 },
    { "VAROR"                       , MN_VAROR                  , 0 },
    { "VARROR"                      , MN_VARROR                 , 0 },
    { "VARROL"                      , MN_VARROL                 , 0 },

    { "SUBSTR"                      , MN_SUBSTR                 , 0 },
    { "STRI2CHR"                    , MN_STRI2CHR               , 0 },
    { "INT2STR"                     , MN_INT2STR                , 1 },
    { "DOUBLE2STR"                  , MN_DOUBLE2STR             , 1 },
    { "FLOAT2STR"                   , MN_FLOAT2STR              , 1 },
    { "CHR2STR"                     , MN_CHR2STR                , 1 },

    { "LETNP"                       , MN_LETNP                  , 0 },

    { "BNOT"                        , MN_BNOT                   , 0 },
    { "BAND"                        , MN_BAND                   , 0 },
    { "BOR"                         , MN_BOR                    , 0 },
    { "BXOR"                        , MN_BXOR                   , 0 },

    { "INT2FLOAT"                   , MN_INT2FLOAT              , 1 },
    { "FLOAT2INT"                   , MN_FLOAT2INT              , 1 },

    { "FLOAT2DOUBLE"                , MN_FLOAT2DOUBLE           , 1 },
    { "DOUBLE2FLOAT"                , MN_DOUBLE2FLOAT           , 1 },

    { "INT2DOUBLE"                  , MN_INT2DOUBLE             , 1 },
    { "DOUBLE2INT"                  , MN_DOUBLE2INT             , 1 },

    { "STR2CHARNUL"                 , MN_STR2CHARNUL            , 0 },

    { "A2STR"                       , MN_A2STR                  , 1 },
    { "STR2A"                       , MN_STR2A                  , 1 },
    { "STRACAT"                     , MN_STRACAT                , 1 },
    { "INT2DWORD"                   , MN_INT2DWORD              , 1 },
    { "INT2WORD"                    , MN_INT2WORD               , 1 },
    { "INT2BYTE"                    , MN_INT2BYTE               , 1 },

    { "MN_NCALL"                    , MN_NCALL                  , 1 },

    { "MN_EXITHNDLR"                , MN_EXITHNDLR              , 1 },
    { "MN_ERRHNDLR"                 , MN_ERRHNDLR               , 1 },
    { "MN_RESUME"                   , MN_RESUME                 , 1 },
    { "MN_RESUMEGOTO"               , MN_RESUMEGOTO             , 1 },

    { "REPEAT"                      , MN_REPEAT                 , 1 },
    { "BREAK"                       , MN_BREAK                  , 1 },
    { "BRFALSE"                     , MN_BRFALSE                , 1 },
    { "RETRUE"                      , MN_RETRUE                 , 1 },
    { "REFALSE"                     , MN_REFALSE                , 1 },

    { "POINTER2STR"                 , MN_POINTER2STR            , 1 },
    { "STR2INT"                     , MN_STR2INT                , 1 },
    { "STR2DOUBLE"                  , MN_STR2DOUBLE             , 1 },
    { "STR2FLOAT"                   , MN_STR2FLOAT              , 1 },
    { "STR2CHR"                     , MN_STR2CHR                , 1 },

    { "PUBLIC"                      , MN_PUBLIC                 , 1 },
    { "GET_PUBLIC"                  , MN_GET_PUBLIC             , 1 },

    { "REMOTE_PUBLIC"               , MN_REMOTE_PUBLIC          , 1 },
    { "GET_REMOTE_PUBLIC"           , MN_GET_REMOTE_PUBLIC      , 1 },

    { "NOP"                         , MN_NOP                    , 0 },

    { ""                            , MN_SENTENCE               , 1 },

    { 0                             , -1                        , 0 }
};

/* ---------------------------------------------------------------------- */

#ifdef __BGDRTM__
extern int debug;
#endif

static int mnemonics_inited = 0;

struct {
    char   name[21];
    int    params;
} mnemonics_sorted[256];

/* ---------------------------------------------------------------------- */

void mnemonic_dump( int64_t i, int64_t param ) {
    int n = 0;

    if ( !mnemonics_inited ) {
        while ( mnemonics[n].name ) {
            sprintf( mnemonics_sorted[mnemonics[n].code & MN_MASK].name, "%-20s", mnemonics[n].name );
            mnemonics_sorted[mnemonics[n].code & MN_MASK].params = mnemonics[n].params;
            n++;
        }
        mnemonics_inited = 1;
    }

    n = i & MN_MASK;

#ifdef __BGDRTM__
    if ( debug > 1 ) {
#endif
        if ( MN_PARAMS( i ) ) printf( "%016" PRIX64 " %016" PRIX64 " "  , i, param );
        else                  printf( "%016" PRIX64 "                  ", i );
#ifdef __BGDRTM__
    }
#endif

    if ( mnemonics_sorted[ n ].name ) {
#ifdef __BGDRTM__
        if ( debug > 1 ) {
#endif
            switch ( MN_TYPEOF( i ) ) {
//                case MN_UNSIGNED:
//                    printf( "UNSIGNED " );
//                    break;

/*
                case MN_QWORD:
                    printf( "INT64    " );
                    break;
*/
                case MN_QWORD | MN_UNSIGNED:
                    printf( "QWORD    " );
                    break;

                case MN_DWORD:
                    printf( "INT32    " );
                    break;

                case MN_DWORD | MN_UNSIGNED:
                    printf( "DWORD    " );
                    break;

                case MN_WORD:
                    printf( "SHORT    " );
                    break;

                case MN_WORD | MN_UNSIGNED:
                    printf( "WORD     " );
                    break;

                case MN_BYTE:
                    printf( "CHAR     " );
                    break;

                case MN_BYTE | MN_UNSIGNED:
                    printf( "BYTE     " );
                    break;

                case MN_FLOAT:
                    printf( "FLOAT    " );
                    break;

                case MN_DOUBLE:
                    printf( "DOUBLE   " );
                    break;

                case MN_STRING:
                    printf( "STRING   " );
                    break;

                default:
                    printf( "         " );
                    break;
            }

            printf( "%s", mnemonics_sorted[n].name );

            if ( i == MN_SYSCALL || i == MN_SYSPROC ) {
                printf( "%s (%" PRId64 ")\n", sysproc_name( param ), param );
            } else if ( i == MN_CALL || i == MN_PROC || i == MN_TYPE ) {
#ifndef __BGDRTM__
                if ( libmode )  printf( "%s (%" PRId64 ")\n", identifier_name( (procdef_search(param))->identifier ), param );
                else            printf( "%s (%" PRId64 ")\n", identifier_name( (procdef_get(param))->identifier ), param );
#else
                printf( "%s (%" PRId64 ")\n", procdef_get(param)->name, param );
#endif
            } else if ( i != MN_SENTENCE ) {
                switch ( MN_PARAMS( i ) ) {
                    case    1:
                            printf( "%" PRId64, param );
                            break;

                }
                printf( "\n" );
            }

#ifdef __BGDRTM__
        }
#endif

        if ( i == MN_SENTENCE ) {
#ifdef __BGDRTM__
            if ( dcb.sourcecount[param >> 24] ) {
                if ( dcb.data.Version == 0x0700 ) printf( "%s:%-10" PRId64 " %s\n", dcb.sourcefiles[param >> 24], param & 0xFFFFFF, dcb.sourcelines[param >> 24] [( param & 0xFFFFFF )-1] );
                else                              printf( "%s:%-10" PRId64 " %s\n", dcb.sourcefiles[param >> 20], param & 0xFFFFF , dcb.sourcelines[param >> 20] [( param & 0xFFFFF  )-1] );
            }
#else
            printf( "%s:%-10" PRId64 "\n", files[param >> 20], param & (( 1 << 20 ) - 1 ) );
#endif
        }
    }
#ifdef __BGDRTM__
    else if ( debug > 1 )
#endif
        printf( "\n" );

}
