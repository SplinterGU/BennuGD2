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

#include "bgdc.h"

/* ----------------------------------------------------------------------- */
/* This module contains the definitions of constants, predefined global    */
/* and local variables, as well as the code that loads and defines each of */
/* the system functions (it is called before starting the compilation).    */
/* ----------------------------------------------------------------------- */

static struct {
    char *  name;
    int     type;
    int64_t code;
} constants_def[] = {
    { "STATUS_DEAD"             , TYPE_QWORD,   STATUS_DEAD         },
    { "STATUS_KILLED"           , TYPE_QWORD,   STATUS_KILLED       },
    { "STATUS_RUNNING"          , TYPE_QWORD,   STATUS_RUNNING      },
    { "STATUS_SLEEPING"         , TYPE_QWORD,   STATUS_SLEEPING     },
    { "STATUS_FROZEN"           , TYPE_QWORD,   STATUS_FROZEN       },
    { "STATUS_PAUSED"           , TYPE_QWORD,   STATUS_PAUSED_MASK  },
    { "STATUS_WAITING"          , TYPE_QWORD,   STATUS_WAITING_MASK },

    { "NULL"                    , TYPE_QWORD,   0LL                 },

    { "FALSE"                   , TYPE_QWORD,   0LL                 },
    { "TRUE"                    , TYPE_QWORD,   !0LL                },

    { "OS_WIN32"                , TYPE_QWORD,   OS_WIN32            },
    { "OS_LINUX"                , TYPE_QWORD,   OS_LINUX            },
    { "OS_BEOS"                 , TYPE_QWORD,   OS_BEOS             },
    { "OS_MACOS"                , TYPE_QWORD,   OS_MACOS            },
    { "OS_GP32"                 , TYPE_QWORD,   OS_GP32             },
    { "OS_DC"                   , TYPE_QWORD,   OS_DC               },
    { "OS_BSD"                  , TYPE_QWORD,   OS_BSD              },
    { "OS_GP2X"                 , TYPE_QWORD,   OS_GP2X             },
    { "OS_GP2X_WIZ"             , TYPE_QWORD,   OS_GP2X_WIZ         },
    { "OS_CAANOO"               , TYPE_QWORD,   OS_CAANOO           },
    { "OS_DINGUX_A320"          , TYPE_QWORD,   OS_DINGUX_A320      },
    { "OS_WII"                  , TYPE_QWORD,   OS_WII              },
    { "OS_ANDROID"              , TYPE_QWORD,   OS_ANDROID          },
    { "OS_IOS"                  , TYPE_QWORD,   OS_IOS              },

    { "MIN_INT"                 , TYPE_INT,     INT64_MIN           },
    { "MAX_INT"                 , TYPE_INT,     INT64_MAX           },

    { "MIN_INT64"               , TYPE_INT,     INT64_MIN           },
    { "MAX_INT64"               , TYPE_INT,     INT64_MAX           },
    { "MIN_LONG"                , TYPE_INT,     INT64_MIN           },
    { "MAX_LONG"                , TYPE_INT,     INT64_MAX           },
    { "MAX_QWORD"               , TYPE_QWORD,   UINT64_MAX          },

    { "MIN_INT32"               , TYPE_INT32,   INT32_MIN           },
    { "MAX_INT32"               , TYPE_INT32,   INT32_MAX           },
    { "MAX_DWORD"               , TYPE_DWORD,   UINT32_MAX          },

    { "MIN_SHORT"               , TYPE_SHORT,   INT16_MIN           },
    { "MAX_SHORT"               , TYPE_SHORT,   INT16_MAX           },
    { "MAX_WORD"                , TYPE_WORD ,   UINT16_MAX          },

    { "MIN_SBYTE"               , TYPE_SBYTE,   INT8_MIN            },
    { "MAX_SBYTE"               , TYPE_SBYTE,   INT8_MAX            },
    { "MAX_BYTE"                , TYPE_BYTE ,   UINT8_MAX           },

    { "MAX_CHAR"                , TYPE_BYTE ,   UINT8_MAX           },

    { "BENNUGD_VERSION"         , TYPE_BYTE ,   0xff                },

    { NULL                      , 0         ,   0LL                 }
};

static char * globals_def =
    "int argc;\n"
    "string argv[32];\n"
    "int os_id = -1;\n";

static char * locals_def =
    "int id;\n"
    "STRUCT reserved\n"
        "int process_type;\n"
        "int frame_percent;\n"
        "int status = STATUS_RUNNING;\n"
        "int saved_status = STATUS_RUNNING;\n"
        "int saved_priority;\n"
    "END\n"

    "int father;\n"
    "int son;\n"
    "int smallbro;\n"
    "int bigbro;\n"
    "int priority;\n";


static struct {
    char * name;
    char * value;
} defines[] = {
    { "BYTE_MAX"    , "((BYTE)  255)"                   },
    { "WORD_MAX"    , "((WORD)  65535)"                 },
    { "DWORD_MAX"   , "((DWORD) 4294967295)"            },
    { "QWORD_MAX"   , "((QWORD) 18446744073709551615)"  },

    { "CHAR_MIN"    , "-128"                            },
    { "CHAR_MAX"    , "127"                             },
    { "SHORT_MIN"   , "-32768"                          },
    { "SHORT_MAX"   , "32767"                           },
    { "INT32_MIN"   , "-2147483648"                     },
    { "INT32_MAX"   , "2147483647"                      },
    { "INT_MIN"     , "-9223372036854775808"            },
    { "INT_MAX"     , "9223372036854775807"             },

    { "UINT8_MAX"    , "BYTE_MAX"                       },
    { "UINT16_MAX"   , "WORD_MAX"                       },
    { "UINT32_MAX"   , "DWORD_MAX"                      },
    { "UINT_MAX"     , "QWORD_MAX"                      },

    { NULL          , NULL                              }
};

void core_init() {
    int i = 0;

    while ( constants_def[i].name ) {
        int64_t code = identifier_search_or_add( constants_def[i].name );
        constants_add( code, typedef_new( constants_def[i].type ), constants_def[i].code );
        i++;
    }

    i = 0;
    while ( defines[i].name ) {
        add_simple_define( defines[i].name, defines[i].value );
        i++;
    }

    token_init( globals_def, -1 );
    compile_varspace( &global, globaldata, 1, 1, 0, NULL, DEFAULT_ALIGNMENT, 0, 0, 0, 0 );

    token_init( locals_def, -1 );
    compile_varspace( &local, localdata, 1, 1, 0, NULL, DEFAULT_ALIGNMENT, 0, 0, 0, 0 );
}
