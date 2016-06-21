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

#ifndef __LIBMOD_MISC_EXPORTS
#define __LIBMOD_MISC_EXPORTS

/* ----------------------------------------------------------------- */

#include "bgddl.h"
#include "libmod_misc.h"

/* ----------------------------------------------------------------- */

#if defined(__BGDC__) || !defined(__STATIC__)

/* ----------------------------------------------------------------- */

DLCONSTANT  __bgdexport( libmod_misc, constants_def)[] =
{
    /* Crypt */
    { "CRYPT_NONE"              , TYPE_QWORD    , CRYPT_NONE            },
    { "CRYPT_DES"               , TYPE_QWORD    , CRYPT_DES             },
    { "CRYPT_3DES"              , TYPE_QWORD    , CRYPT_3DES            },

    /* Files */
    { "O_READ"                  , TYPE_QWORD    , 0                     },
    { "O_READWRITE"             , TYPE_QWORD    , 1                     },
    { "O_RDWR"                  , TYPE_QWORD    , 1                     },
    { "O_WRITE"                 , TYPE_QWORD    , 2                     },
    { "O_ZREAD"                 , TYPE_QWORD    , 3                     },
    { "O_ZWRITE"                , TYPE_QWORD    , 4                     },

    { "SEEK_SET"                , TYPE_QWORD    , 0                     },
    { "SEEK_CUR"                , TYPE_QWORD    , 1                     },
    { "SEEK_END"                , TYPE_QWORD    , 2                     },

    /* Math */
    { "PI"                      , TYPE_QWORD    , 180000                },

    /* Signals and processes */
    { "S_KILL"                  , TYPE_QWORD    , S_KILL                },
    { "S_WAKEUP"                , TYPE_QWORD    , S_WAKEUP              },
    { "S_SLEEP"                 , TYPE_QWORD    , S_SLEEP               },
    { "S_FREEZE"                , TYPE_QWORD    , S_FREEZE              },

    { "S_FORCE"                 , TYPE_QWORD    , S_FORCE               },
    { "S_TREE"                  , TYPE_QWORD    , S_TREE                },

    { "S_KILL_TREE"             , TYPE_QWORD    , S_KILL_TREE           },
    { "S_WAKEUP_TREE"           , TYPE_QWORD    , S_WAKEUP_TREE         },
    { "S_SLEEP_TREE"            , TYPE_QWORD    , S_SLEEP_TREE          },
    { "S_FREEZE_TREE"           , TYPE_QWORD    , S_FREEZE_TREE         },

    { "S_KILL_FORCE"            , TYPE_QWORD    , S_KILL_FORCE          },
    { "S_WAKEUP_FORCE"          , TYPE_QWORD    , S_WAKEUP_FORCE        },
    { "S_SLEEP_FORCE"           , TYPE_QWORD    , S_SLEEP_FORCE         },
    { "S_FREEZE_FORCE"          , TYPE_QWORD    , S_FREEZE_FORCE        },
    { "S_KILL_TREE_FORCE"       , TYPE_QWORD    , S_KILL_TREE_FORCE     },
    { "S_WAKEUP_TREE_FORCE"     , TYPE_QWORD    , S_WAKEUP_TREE_FORCE   },
    { "S_SLEEP_TREE_FORCE"      , TYPE_QWORD    , S_SLEEP_TREE_FORCE    },
    { "S_FREEZE_TREE_FORCE"     , TYPE_QWORD    , S_FREEZE_TREE_FORCE   },

    { "S_DFL"                   , TYPE_QWORD    , S_DFL                 },
    { "S_IGN"                   , TYPE_QWORD    , S_IGN                 },

    { "ALL_PROCESS"             , TYPE_QWORD    , ALL_PROCESS           },

    /* Sys */
    { "_P_WAIT"                 , TYPE_QWORD    , _P_WAIT               },
    { "_P_NOWAIT"               , TYPE_QWORD    , _P_NOWAIT             },

    { NULL                      , 0             , 0                     }
} ;

#endif

/* ----------------------------------------------------------------- */
/* Definicion de variables globales (usada en tiempo de compilacion) */

char * __bgdexport( libmod_misc, globals_def ) =
    "STRUCT fileinfo\n"
        "STRING path;\n"
        "STRING name;\n"
        "directory;\n"
        "hidden;\n"
        "readonly;\n"
        "size;\n"
        "STRING created;\n"
        "STRING modified;\n"
        "STRING accessed;\n"
        "STRING statechg;\n"
    "END\n"

    "STRING regex_reg[15];\n"

    "timer[9];\n"
    ;

/* ----------------------------------------------------------------- */

char * __bgdexport( libmod_misc, locals_def ) =
    "STRUCT __proccess_reserved\n"
        "int64 type_scan;\n"
        "int64 id_scan;\n"
        "int64 context;\n"
        "qword signal_action;\n"
    "END\n"
    ;

/* ----------------------------------------------------------------- */
/* Functions declaration                                             */

DLSYSFUNCS  __bgdexport( libmod_misc, functions_exports)[] =
{
    /* Crypt */
    FUNC( "CRYPT_NEW"       , "QP"      , TYPE_POINTER      , libmod_misc_crypt_new              ),
    FUNC( "CRYPT_DEL"       , "P"       , TYPE_QWORD        , libmod_misc_crypt_del              ),
    FUNC( "CRYPT_ENCRYPT"   , "PPPQ"    , TYPE_QWORD        , libmod_misc_crypt_encrypt          ),
    FUNC( "CRYPT_DECRYPT"   , "PPPQ"    , TYPE_QWORD        , libmod_misc_crypt_decrypt          ),
    FUNC( "CRYPT_ENCRYPT"   , "QPPPQ"   , TYPE_QWORD        , libmod_misc_crypt_encrypt2         ),
    FUNC( "CRYPT_DECRYPT"   , "QPPPQ"   , TYPE_QWORD        , libmod_misc_crypt_decrypt2         ),

    /* Directories */
    FUNC( "CD"              , ""        , TYPE_STRING       , libmod_misc_dir_cd                 ),
    FUNC( "CD"              , "S"       , TYPE_QWORD        , libmod_misc_dir_chdir              ),
    FUNC( "CHDIR"           , "S"       , TYPE_QWORD        , libmod_misc_dir_chdir              ),
    FUNC( "MKDIR"           , "S"       , TYPE_QWORD        , libmod_misc_dir_mkdir              ),
    FUNC( "RMDIR"           , "S"       , TYPE_QWORD        , libmod_misc_dir_rmdir              ),
    FUNC( "GLOB"            , "S"       , TYPE_STRING       , libmod_misc_dir_glob               ),
    FUNC( "RM"              , "S"       , TYPE_QWORD        , libmod_misc_dir_rm                 ),

    FUNC( "DIROPEN"         , "S"       , TYPE_QWORD        , libmod_misc_dir_open               ),
    FUNC( "DIRCLOSE"        , "Q"       , TYPE_QWORD        , libmod_misc_dir_close              ),
    FUNC( "DIRREAD"         , "Q"       , TYPE_STRING       , libmod_misc_dir_read               ),

    /* Files */
    FUNC( "SAVE"            , "SV++"    , TYPE_QWORD        , libmod_misc_file_save              ),
    FUNC( "LOAD"            , "SV++"    , TYPE_QWORD        , libmod_misc_file_load              ),

    FUNC( "FILE"            , "S"       , TYPE_STRING       , libmod_misc_file_file              ),

    FUNC( "FOPEN"           , "SQ"      , TYPE_QWORD        , libmod_misc_file_fopen             ),
    FUNC( "FCLOSE"          , "Q"       , TYPE_QWORD        , libmod_misc_file_fclose            ),
    FUNC( "FREAD"           , "QV++"    , TYPE_QWORD        , libmod_misc_file_fread             ),
    FUNC( "FREAD"           , "PQQ"     , TYPE_QWORD        , libmod_misc_file_freadC            ),
    FUNC( "FWRITE"          , "QV++"    , TYPE_QWORD        , libmod_misc_file_fwrite            ),
    FUNC( "FWRITE"          , "PQQ"     , TYPE_QWORD        , libmod_misc_file_fwriteC           ),
    FUNC( "FSEEK"           , "QQQ"     , TYPE_QWORD        , libmod_misc_file_fseek             ),
    FUNC( "FREWIND"         , "Q"       , TYPE_UNDEFINED    , libmod_misc_file_frewind           ),
    FUNC( "FTELL"           , "Q"       , TYPE_QWORD        , libmod_misc_file_ftell             ),
    FUNC( "FFLUSH"          , "Q"       , TYPE_QWORD        , libmod_misc_file_fflush            ),
    FUNC( "FPUTS"           , "QS"      , TYPE_QWORD        , libmod_misc_file_fputs             ),
    FUNC( "FGETS"           , "Q"       , TYPE_STRING       , libmod_misc_file_fgets             ),
    FUNC( "FEOF"            , "Q"       , TYPE_QWORD        , libmod_misc_file_feof              ),

    FUNC( "FLENGTH"         , "Q"       , TYPE_QWORD        , libmod_misc_file_filelength        ),
    FUNC( "FEXISTS"         , "S"       , TYPE_QWORD        , libmod_misc_file_exists            ) ,
    FUNC( "FREMOVE"         , "S"       , TYPE_QWORD        , libmod_misc_file_remove            ) ,
    FUNC( "FMOVE"           , "SS"      , TYPE_QWORD        , libmod_misc_file_move              ) ,

    /* Math */
    FUNC( "ABS"             , "D"       , TYPE_DOUBLE       , libmod_misc_math_abs               ),
    FUNC( "POW"             , "DD"      , TYPE_DOUBLE       , libmod_misc_math_pow               ),
    FUNC( "SQRT"            , "D"       , TYPE_DOUBLE       , libmod_misc_math_sqrt              ),

    FUNC( "COS"             , "D"       , TYPE_DOUBLE       , libmod_misc_math_cos               ),
    FUNC( "SIN"             , "D"       , TYPE_DOUBLE       , libmod_misc_math_sin               ),
    FUNC( "TAN"             , "D"       , TYPE_DOUBLE       , libmod_misc_math_tan               ),
    FUNC( "ACOS"            , "D"       , TYPE_DOUBLE       , libmod_misc_math_acos              ),
    FUNC( "ASIN"            , "D"       , TYPE_DOUBLE       , libmod_misc_math_asin              ),
    FUNC( "ATAN"            , "D"       , TYPE_DOUBLE       , libmod_misc_math_atan              ),
    FUNC( "ATAN2"           , "DD"      , TYPE_DOUBLE       , libmod_misc_math_atan2             ),

    FUNC( "ISINF"           , "D"       , TYPE_QWORD        , libmod_misc_math_isinf             ),
    FUNC( "ISNAN"           , "D"       , TYPE_QWORD        , libmod_misc_math_isnan             ),
    FUNC( "FINITE"          , "D"       , TYPE_QWORD        , libmod_misc_math_finite            ),

    FUNC( "FGET_ANGLE"      , "QQQQ"    , TYPE_QWORD        , libmod_misc_math_fget_angle        ),
    FUNC( "FGET_DIST"       , "QQQQ"    , TYPE_QWORD        , libmod_misc_math_fget_dist         ),
    FUNC( "NEAR_ANGLE"      , "QQQ"     , TYPE_QWORD        , libmod_misc_math_near_angle        ),
    FUNC( "GET_DISTX"       , "QQ"      , TYPE_QWORD        , libmod_misc_math_get_distx         ),
    FUNC( "GET_DISTY"       , "QQ"      , TYPE_QWORD        , libmod_misc_math_get_disty         ),

    /* Mem */
    FUNC( "MEM_CALLOC"      , "QQ"      , TYPE_POINTER      , libmod_misc_mem_calloc             ),
    FUNC( "MEM_ALLOC"       , "Q"       , TYPE_POINTER      , libmod_misc_mem_alloc              ),
    FUNC( "MEM_FREE"        , "P"       , TYPE_QWORD        , libmod_misc_mem_free               ),
    FUNC( "MEM_REALLOC"     , "PQ"      , TYPE_POINTER      , libmod_misc_mem_realloc            ),
    FUNC( "MEM_CMP"         , "PPQ"     , TYPE_QWORD        , libmod_misc_mem_memcmp             ),
    FUNC( "MEM_SET"         , "PBQ"     , TYPE_QWORD        , libmod_misc_mem_memset             ),
    FUNC( "MEM_SETW"        , "PWQ"     , TYPE_QWORD        , libmod_misc_mem_memsetw            ),
    FUNC( "MEM_SETI"        , "PQQ"     , TYPE_QWORD        , libmod_misc_mem_memseti            ),
    FUNC( "MEM_COPY"        , "PPQ"     , TYPE_QWORD        , libmod_misc_mem_memcopy            ),
    FUNC( "MEM_MOVE"        , "PPQ"     , TYPE_QWORD        , libmod_misc_mem_memmove            ),

    FUNC( "MEM_AVAILABLE"   , ""        , TYPE_QWORD        , libmod_misc_mem_memory_free        ),
    FUNC( "MEM_TOTAL"       , ""        , TYPE_QWORD        , libmod_misc_mem_memory_total       ),

    FUNC( "CALLOC"          , "QQ"      , TYPE_POINTER      , libmod_misc_mem_calloc             ),
    FUNC( "ALLOC"           , "Q"       , TYPE_POINTER      , libmod_misc_mem_alloc              ),
    FUNC( "FREE"            , "P"       , TYPE_QWORD        , libmod_misc_mem_free               ),
    FUNC( "REALLOC"         , "PQ"      , TYPE_POINTER      , libmod_misc_mem_realloc            ),
    FUNC( "MEMCMP"          , "PPQ"     , TYPE_QWORD        , libmod_misc_mem_memcmp             ),
    FUNC( "MEMSET"          , "PBQ"     , TYPE_QWORD        , libmod_misc_mem_memset             ),
    FUNC( "MEMSETQ"         , "PQQ"     , TYPE_QWORD        , libmod_misc_mem_memsetq            ),
    FUNC( "MEMSETW"         , "PWQ"     , TYPE_QWORD        , libmod_misc_mem_memsetw            ),
    FUNC( "MEMSETI"         , "PIQ"     , TYPE_QWORD        , libmod_misc_mem_memseti            ),
    FUNC( "MEMCOPY"         , "PPQ"     , TYPE_QWORD        , libmod_misc_mem_memcopy            ),
    FUNC( "MEMMOVE"         , "PPQ"     , TYPE_QWORD        , libmod_misc_mem_memmove            ),

    FUNC( "MEMORY_FREE"     , ""        , TYPE_QWORD        , libmod_misc_mem_memory_free        ),
    FUNC( "MEMORY_TOTAL"    , ""        , TYPE_QWORD        , libmod_misc_mem_memory_total       ),

    /* Signals & Processes */
    FUNC( "GET_ID"          , "Q"       , TYPE_QWORD        , libmod_misc_proc_get_id            ),
    FUNC( "GET_STATUS"      , "Q"       , TYPE_QWORD        , libmod_misc_proc_get_status        ),
    FUNC( "SIGNAL"          , "QQ"      , TYPE_QWORD        , libmod_misc_proc_signal            ),
    FUNC( "SIGNAL_ACTION"   , "QQ"      , TYPE_QWORD        , libmod_misc_proc_signal_action     ),
    FUNC( "SIGNAL_ACTION"   , "QQQ"     , TYPE_QWORD        , libmod_misc_proc_signal_action3    ),
    FUNC( "LET_ME_ALONE"    , ""        , TYPE_QWORD        , libmod_misc_proc_let_me_alone      ),
    FUNC( "EXIT"            , "SQ"      , TYPE_QWORD        , libmod_misc_proc_exit              ),
    FUNC( "EXIT"            , "S"       , TYPE_QWORD        , libmod_misc_proc_exit_1            ),
    FUNC( "EXIT"            , ""        , TYPE_QWORD        , libmod_misc_proc_exit_0            ),
    FUNC( "EXISTS"          , "Q"       , TYPE_QWORD        , libmod_misc_proc_running           ),

    FUNC( "RAND_SEED"       , "Q"       , TYPE_QWORD        , libmod_misc_rand_seed              ),
    FUNC( "RAND"            , "QQ"      , TYPE_QWORD        , libmod_misc_rand_std               ),

    /* Regex */
    FUNC( "REGEX"           , "SS"      , TYPE_QWORD        , libmod_misc_regex_regex            ),
    FUNC( "REGEX_REPLACE"   , "SSS"     , TYPE_STRING       , libmod_misc_regex_regex_replace    ),
    FUNC( "SPLIT"           , "SSPQ"    , TYPE_QWORD        , libmod_misc_regex_split            ),
    FUNC( "JOIN"            , "SPQ"     , TYPE_STRING       , libmod_misc_regex_join             ),

    FUNC( "SAY"             , "S"       , TYPE_UNDEFINED    , libmod_misc_say_say                ),
    FUNC( "SAY_FAST"        , "S"       , TYPE_UNDEFINED    , libmod_misc_say_say_fast           ),

    /* Sort */
    FUNC( "QSORT"           , "PQQQBB"  , TYPE_QWORD        , libmod_misc_sort_quicksort         ),

    FUNC( "KSORT"           , "V++V++"  , TYPE_QWORD        , libmod_misc_sort_ksort             ),
    FUNC( "KSORT"           , "V++V++Q" , TYPE_QWORD        , libmod_misc_sort_ksort_n           ),

    FUNC( "SORT"            , "V++Q"    , TYPE_QWORD        , libmod_misc_sort_sort_n            ),
    FUNC( "SORT"            , "V++"     , TYPE_QWORD        , libmod_misc_sort_sort              ),

    /* Strings */
    FUNC( "STRLEN"          , "S"       , TYPE_QWORD        , libmod_misc_string_strlen          ),
    FUNC( "LEN"             , "S"       , TYPE_QWORD        , libmod_misc_string_strlen          ),
    FUNC( "UCASE"           , "S"       , TYPE_STRING       , libmod_misc_string_strupper        ),
    FUNC( "LCASE"           , "S"       , TYPE_STRING       , libmod_misc_string_strlower        ),
    FUNC( "STRCASECMP"      , "SS"      , TYPE_QWORD        , libmod_misc_string_strcasecmp      ),
    FUNC( "SUBSTR"          , "SQQ"     , TYPE_STRING       , libmod_misc_string_substr          ),
    FUNC( "SUBSTR"          , "SQ"      , TYPE_STRING       , libmod_misc_string_substr2         ),
    FUNC( "FIND"            , "SS"      , TYPE_QWORD        , libmod_misc_string_strfind         ),
    FUNC( "FIND"            , "SSQ"     , TYPE_QWORD        , libmod_misc_string_strfindSSI      ),
    FUNC( "LPAD"            , "SQ"      , TYPE_STRING       , libmod_misc_string_lpad            ),
    FUNC( "RPAD"            , "SQ"      , TYPE_STRING       , libmod_misc_string_rpad            ),
    FUNC( "ITOA"            , "Q"       , TYPE_STRING       , libmod_misc_string_itos            ),
    FUNC( "FTOA"            , "D"       , TYPE_STRING       , libmod_misc_string_ftos            ),
    FUNC( "ATOI"            , "S"       , TYPE_QWORD        , libmod_misc_string_stoi            ),
    FUNC( "ATOF"            , "S"       , TYPE_DOUBLE       , libmod_misc_string_stof            ),
    FUNC( "ASC"             , "S"       , TYPE_BYTE         , libmod_misc_string_asc             ),
    FUNC( "CHR"             , "Q"       , TYPE_STRING       , libmod_misc_string_chr             ),
    FUNC( "TRIM"            , "S"       , TYPE_STRING       , libmod_misc_string_trim            ),
    FUNC( "STRREV"          , "S"       , TYPE_STRING       , libmod_misc_string_strrev          ),
    FUNC( "FORMAT"          , "Q"       , TYPE_STRING       , libmod_misc_string_formatI         ),
    FUNC( "FORMAT"          , "D"       , TYPE_STRING       , libmod_misc_string_formatF         ),
    FUNC( "FORMAT"          , "DQ"      , TYPE_STRING       , libmod_misc_string_formatFI        ),

    FUNC( "GETENV"          , "S"       , TYPE_STRING       , libmod_misc_sys_getenv             ),
    FUNC( "EXEC"            , "QSQP"    , TYPE_QWORD        , libmod_misc_sys_exec               ),

    /* Date/Time & Timers */
    FUNC( "GET_TIMER"       , ""        , TYPE_QWORD        , libmod_misc_get_timer              ),
    FUNC( "TIME"            , ""        , TYPE_QWORD        , libmod_misc_time                   ),
    FUNC( "FTIME"           , "SQ"      , TYPE_STRING       , libmod_misc_ftime                  ),

    FUNC( 0                 , 0         , 0                , 0                                   )
};

/* ----------------------------------------------------------------- */

#endif
