/*
 *  Copyright (C) 2006-2019 SplinterGU (Fenix/BennuGD)
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
#if USE_CRYPT
    /* Crypt */
    { "CRYPT_NONE"              , TYPE_QWORD    , CRYPT_NONE            },
    { "CRYPT_DES"               , TYPE_QWORD    , CRYPT_DES             },
    { "CRYPT_3DES"              , TYPE_QWORD    , CRYPT_3DES            },
#endif

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

    { "ALL_PROCESS"             , TYPE_INT      , ALL_PROCESS           },

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
    "   STRING path;\n"
    "   STRING name;\n"
    "   INT directory;\n"
    "   INT hidden;\n"
    "   INT readonly;\n"
    "   INT size;\n"
    "   STRING created;\n"
    "   STRING modified;\n"
    "   STRING accessed;\n"
    "   STRING statechg;\n"
    "END\n"

    "STRING regex_reg[15];\n"

    "INT timer[9];\n"
    ;

/* ----------------------------------------------------------------- */

char * __bgdexport( libmod_misc, locals_def ) =
    "STRUCT __proccess_reserved\n"
    "   INT type_scan;\n"
    "   INT id_scan;\n"
    "   INT context;\n"
    "   QWORD signal_action;\n"
    "END\n"
    ;

/* ----------------------------------------------------------------- */
/* Functions declaration                                             */

DLSYSFUNCS  __bgdexport( libmod_misc, functions_exports)[] =
{
#if USE_CRYPT
    /* Crypt */
    FUNC( "CRYPT_NEW"       , "IP"      , TYPE_POINTER      , libmod_misc_crypt_new              ),
    FUNC( "CRYPT_DEL"       , "P"       , TYPE_INT          , libmod_misc_crypt_del              ),
    FUNC( "CRYPT_ENCRYPT"   , "PPPI"    , TYPE_INT          , libmod_misc_crypt_encrypt          ),
    FUNC( "CRYPT_DECRYPT"   , "PPPI"    , TYPE_INT          , libmod_misc_crypt_decrypt          ),
    FUNC( "CRYPT_ENCRYPT"   , "IPPPI"   , TYPE_INT          , libmod_misc_crypt_encrypt2         ),
    FUNC( "CRYPT_DECRYPT"   , "IPPPI"   , TYPE_INT          , libmod_misc_crypt_decrypt2         ),
#endif

    /* Directories */
    FUNC( "CD"              , ""        , TYPE_STRING       , libmod_misc_dir_cd                 ),
    FUNC( "CD"              , "S"       , TYPE_INT          , libmod_misc_dir_chdir              ),
    FUNC( "CHDIR"           , "S"       , TYPE_INT          , libmod_misc_dir_chdir              ),
    FUNC( "MKDIR"           , "S"       , TYPE_INT          , libmod_misc_dir_mkdir              ),
    FUNC( "RMDIR"           , "S"       , TYPE_INT          , libmod_misc_dir_rmdir              ),
    FUNC( "GLOB"            , "S"       , TYPE_STRING       , libmod_misc_dir_glob               ),
    FUNC( "RM"              , "S"       , TYPE_INT          , libmod_misc_dir_rm                 ),

    FUNC( "DIROPEN"         , "S"       , TYPE_INT          , libmod_misc_dir_open               ),
    FUNC( "DIRCLOSE"        , "I"       , TYPE_INT          , libmod_misc_dir_close              ),
    FUNC( "DIRREAD"         , "I"       , TYPE_STRING       , libmod_misc_dir_read               ),

    /* Files */
    FUNC( "SAVE"            , "SV++"    , TYPE_INT          , libmod_misc_file_save              ),
    FUNC( "LOAD"            , "SV++"    , TYPE_INT          , libmod_misc_file_load              ),

    FUNC( "FILE"            , "S"       , TYPE_STRING       , libmod_misc_file_file              ),

    FUNC( "FOPEN"           , "SI"      , TYPE_INT          , libmod_misc_file_fopen             ),
    FUNC( "FCLOSE"          , "I"       , TYPE_INT          , libmod_misc_file_fclose            ),
    FUNC( "FREAD"           , "IV++"    , TYPE_INT          , libmod_misc_file_fread             ),
    FUNC( "FREAD"           , "PII"     , TYPE_INT          , libmod_misc_file_freadC            ),
    FUNC( "FWRITE"          , "IV++"    , TYPE_INT          , libmod_misc_file_fwrite            ),
    FUNC( "FWRITE"          , "PII"     , TYPE_INT          , libmod_misc_file_fwriteC           ),
    FUNC( "FSEEK"           , "III"     , TYPE_INT          , libmod_misc_file_fseek             ),
    FUNC( "FREWIND"         , "I"       , TYPE_UNDEFINED    , libmod_misc_file_frewind           ),
    FUNC( "FTELL"           , "I"       , TYPE_INT          , libmod_misc_file_ftell             ),
    FUNC( "FFLUSH"          , "I"       , TYPE_INT          , libmod_misc_file_fflush            ),
    FUNC( "FPUTS"           , "IS"      , TYPE_INT          , libmod_misc_file_fputs             ),
    FUNC( "FGETS"           , "I"       , TYPE_STRING       , libmod_misc_file_fgets             ),
    FUNC( "FEOF"            , "I"       , TYPE_INT          , libmod_misc_file_feof              ),

    FUNC( "FLENGTH"         , "I"       , TYPE_INT          , libmod_misc_file_filelength        ),
    FUNC( "FEXISTS"         , "S"       , TYPE_INT          , libmod_misc_file_exists            ),
    FUNC( "FREMOVE"         , "S"       , TYPE_INT          , libmod_misc_file_remove            ),
    FUNC( "FMOVE"           , "SS"      , TYPE_INT          , libmod_misc_file_move              ),

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

    FUNC( "ISINF"           , "D"       , TYPE_INT          , libmod_misc_math_isinf             ),
    FUNC( "ISNAN"           , "D"       , TYPE_INT          , libmod_misc_math_isnan             ),
    FUNC( "FINITE"          , "D"       , TYPE_INT          , libmod_misc_math_finite            ),

    FUNC( "FGET_ANGLE"      , "IIII"    , TYPE_INT          , libmod_misc_math_fget_angle        ),
    FUNC( "FGET_DIST"       , "IIII"    , TYPE_INT          , libmod_misc_math_fget_dist         ),
    FUNC( "NEAR_ANGLE"      , "III"     , TYPE_INT          , libmod_misc_math_near_angle        ),
    FUNC( "GET_DISTX"       , "II"      , TYPE_INT          , libmod_misc_math_get_distx         ),
    FUNC( "GET_DISTY"       , "II"      , TYPE_INT          , libmod_misc_math_get_disty         ),

    FUNC( "CLAMP"           , "III"     , TYPE_INT          , libmod_misc_math_clamp             ),
    FUNC( "CLAMPF"          , "DDD"     , TYPE_DOUBLE       , libmod_misc_math_clampf            ),

    /* Mem */
    FUNC( "MEM_CALLOC"      , "II"      , TYPE_POINTER      , libmod_misc_mem_calloc             ),
    FUNC( "MEM_ALLOC"       , "I"       , TYPE_POINTER      , libmod_misc_mem_alloc              ),
    FUNC( "MEM_FREE"        , "P"       , TYPE_INT          , libmod_misc_mem_free               ),
    FUNC( "MEM_REALLOC"     , "PI"      , TYPE_POINTER      , libmod_misc_mem_realloc            ),
    FUNC( "MEM_CMP"         , "PPI"     , TYPE_INT          , libmod_misc_mem_memcmp             ),
    FUNC( "MEM_SET"         , "PII"     , TYPE_INT          , libmod_misc_mem_memset             ),
    FUNC( "MEM_SETW"        , "PII"     , TYPE_INT          , libmod_misc_mem_memset16           ),
    FUNC( "MEM_SETD"        , "PII"     , TYPE_INT          , libmod_misc_mem_memset32           ),
    FUNC( "MEM_SETI"        , "PII"     , TYPE_INT          , libmod_misc_mem_memset64           ),
    FUNC( "MEM_COPY"        , "PPI"     , TYPE_INT          , libmod_misc_mem_memcopy            ),
    FUNC( "MEM_MOVE"        , "PPI"     , TYPE_INT          , libmod_misc_mem_memmove            ),

    FUNC( "MEM_AVAILABLE"   , ""        , TYPE_INT          , libmod_misc_mem_memory_free        ),
    FUNC( "MEM_TOTAL"       , ""        , TYPE_INT          , libmod_misc_mem_memory_total       ),

    FUNC( "CALLOC"          , "II"      , TYPE_POINTER      , libmod_misc_mem_calloc             ),
    FUNC( "ALLOC"           , "I"       , TYPE_POINTER      , libmod_misc_mem_alloc              ),
    FUNC( "FREE"            , "P"       , TYPE_INT          , libmod_misc_mem_free               ),
    FUNC( "REALLOC"         , "PI"      , TYPE_POINTER      , libmod_misc_mem_realloc            ),
    FUNC( "MEMCMP"          , "PPI"     , TYPE_INT          , libmod_misc_mem_memcmp             ),
    FUNC( "MEMSET"          , "PII"     , TYPE_INT          , libmod_misc_mem_memset             ),
    FUNC( "MEMSETW"         , "PII"     , TYPE_INT          , libmod_misc_mem_memset16           ),
    FUNC( "MEMSETD"         , "PII"     , TYPE_INT          , libmod_misc_mem_memset32           ),
    FUNC( "MEMSETI"         , "PII"     , TYPE_INT          , libmod_misc_mem_memset64           ),
    FUNC( "MEMCOPY"         , "PPI"     , TYPE_INT          , libmod_misc_mem_memcopy            ),
    FUNC( "MEMMOVE"         , "PPI"     , TYPE_INT          , libmod_misc_mem_memmove            ),

    FUNC( "MEMORY_FREE"     , ""        , TYPE_INT          , libmod_misc_mem_memory_free        ),
    FUNC( "MEMORY_TOTAL"    , ""        , TYPE_INT          , libmod_misc_mem_memory_total       ),

    /* Signals & Processes */
    FUNC( "GET_ID"          , "I"       , TYPE_INT          , libmod_misc_proc_get_id            ),
    FUNC( "GET_STATUS"      , "I"       , TYPE_QWORD        , libmod_misc_proc_get_status        ),
    FUNC( "SIGNAL"          , "II"      , TYPE_INT          , libmod_misc_proc_signal            ),
    FUNC( "SIGNAL_ACTION"   , "II"      , TYPE_INT          , libmod_misc_proc_signal_action     ),
    FUNC( "SIGNAL_ACTION"   , "III"     , TYPE_INT          , libmod_misc_proc_signal_action3    ),
    FUNC( "LET_ME_ALONE"    , ""        , TYPE_INT          , libmod_misc_proc_let_me_alone      ),
    FUNC( "EXIT"            , "SI"      , TYPE_INT          , libmod_misc_proc_exit              ),
    FUNC( "EXIT"            , "S"       , TYPE_INT          , libmod_misc_proc_exit_1            ),
    FUNC( "EXIT"            , ""        , TYPE_INT          , libmod_misc_proc_exit_0            ),
    FUNC( "EXISTS"          , "I"       , TYPE_INT          , libmod_misc_proc_running           ),
    FUNC( "PAUSE"           , ""        , TYPE_INT          , libmod_misc_proc_pause0            ),
    FUNC( "RESUME"          , ""        , TYPE_INT          , libmod_misc_proc_resume0           ),
/*    FUNC( "PAUSE"           , "I"       , TYPE_INT          , libmod_misc_proc_pause1            ),
    FUNC( "RESUME"          , "I"       , TYPE_INT          , libmod_misc_proc_resume1           ), */

    FUNC( "RAND_SEED"       , "I"       , TYPE_INT          , libmod_misc_rand_seed              ),
    FUNC( "RAND"            , "II"      , TYPE_INT          , libmod_misc_rand_std               ),

    /* Regex */
    FUNC( "REGEX"           , "SS"      , TYPE_INT          , libmod_misc_regex_regex            ),
    FUNC( "REGEX_REPLACE"   , "SSS"     , TYPE_STRING       , libmod_misc_regex_regex_replace    ),
    FUNC( "SPLIT"           , "SSPI"    , TYPE_INT          , libmod_misc_regex_split            ),
    FUNC( "JOIN"            , "SPI"     , TYPE_STRING       , libmod_misc_regex_join             ),

    FUNC( "SAY"             , "S"       , TYPE_UNDEFINED    , libmod_misc_say_say                ),
    FUNC( "SAY_FAST"        , "S"       , TYPE_UNDEFINED    , libmod_misc_say_say_fast           ),

    /* Sort */
    FUNC( "ISORT"           , "PIIIBB"  , TYPE_INT          , libmod_misc_sort_quicksort         ),

    FUNC( "KSORT"           , "V++V++"  , TYPE_INT          , libmod_misc_sort_ksort             ),
    FUNC( "KSORT"           , "V++V++I" , TYPE_INT          , libmod_misc_sort_ksort_n           ),

    FUNC( "SORT"            , "V++I"    , TYPE_INT          , libmod_misc_sort_sort_n            ),
    FUNC( "SORT"            , "V++"     , TYPE_INT          , libmod_misc_sort_sort              ),

    /* Strings */
    FUNC( "STRLEN"          , "S"       , TYPE_INT          , libmod_misc_string_strlen          ),
    FUNC( "LEN"             , "S"       , TYPE_INT          , libmod_misc_string_strlen          ),
    FUNC( "UCASE"           , "S"       , TYPE_STRING       , libmod_misc_string_strupper        ),
    FUNC( "LCASE"           , "S"       , TYPE_STRING       , libmod_misc_string_strlower        ),
    FUNC( "STRCASECMP"      , "SS"      , TYPE_INT          , libmod_misc_string_strcasecmp      ),
    FUNC( "SUBSTR"          , "SII"     , TYPE_STRING       , libmod_misc_string_substr          ),
    FUNC( "SUBSTR"          , "SI"      , TYPE_STRING       , libmod_misc_string_substr2         ),
    FUNC( "FIND"            , "SS"      , TYPE_INT          , libmod_misc_string_strfind         ),
    FUNC( "FIND"            , "SSI"     , TYPE_INT          , libmod_misc_string_strfindSSI      ),
    FUNC( "LPAD"            , "SI"      , TYPE_STRING       , libmod_misc_string_lpad            ),
    FUNC( "RPAD"            , "SI"      , TYPE_STRING       , libmod_misc_string_rpad            ),
    FUNC( "ITOA"            , "I"       , TYPE_STRING       , libmod_misc_string_itos            ),
    FUNC( "FTOA"            , "D"       , TYPE_STRING       , libmod_misc_string_ftos            ),
    FUNC( "ATOI"            , "S"       , TYPE_INT          , libmod_misc_string_stoi            ),
    FUNC( "ATOF"            , "S"       , TYPE_DOUBLE       , libmod_misc_string_stof            ),
    FUNC( "ASC"             , "S"       , TYPE_BYTE         , libmod_misc_string_asc             ),
    FUNC( "CHR"             , "I"       , TYPE_STRING       , libmod_misc_string_chr             ),
    FUNC( "TRIM"            , "S"       , TYPE_STRING       , libmod_misc_string_trim            ),
    FUNC( "STRREV"          , "S"       , TYPE_STRING       , libmod_misc_string_strrev          ),
    FUNC( "FORMAT"          , "I"       , TYPE_STRING       , libmod_misc_string_formatI         ),
    FUNC( "FORMAT"          , "D"       , TYPE_STRING       , libmod_misc_string_formatF         ),
    FUNC( "FORMAT"          , "DI"      , TYPE_STRING       , libmod_misc_string_formatFI        ),

    FUNC( "GETENV"          , "S"       , TYPE_STRING       , libmod_misc_sys_getenv             ),
    FUNC( "EXEC"            , "ISIP"    , TYPE_INT          , libmod_misc_sys_exec               ),

    /* Date/Time & Timers */
    FUNC( "GET_TIMER"       , ""        , TYPE_INT          , libmod_misc_get_timer              ),
    FUNC( "TIME"            , ""        , TYPE_INT          , libmod_misc_time                   ),
    FUNC( "FTIME"           , "SI"      , TYPE_STRING       , libmod_misc_ftime                  ),

    FUNC( 0                 , 0         , 0                , 0                                   )
};

/* ----------------------------------------------------------------- */

#endif
