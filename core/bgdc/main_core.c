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

    { "FALSE"                   , TYPE_BYTE,    0                   },
    { "TRUE"                    , TYPE_BYTE,    !0                  },

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
    { "OS_SWITCH"               , TYPE_QWORD,   OS_SWITCH           },
    { "OS_PS3"                  , TYPE_QWORD,   OS_PS3              },

    { "MIN_INT64"               , TYPE_INT,     INT64_MIN           },
    { "MAX_INT64"               , TYPE_INT,     INT64_MAX           },
    { "MIN_INT32"               , TYPE_INT32,   INT32_MIN           },
    { "MAX_INT32"               , TYPE_INT32,   INT32_MAX           },
    { "MIN_INT16"               , TYPE_SHORT,   INT16_MIN           },
    { "MAX_INT16"               , TYPE_SHORT,   INT16_MAX           },
    { "MIN_INT8"                , TYPE_SBYTE,   INT8_MIN            },
    { "MAX_INT8"                , TYPE_SBYTE,   INT8_MAX            },

    { "MIN_UINT64"              , TYPE_QWORD,   0                   },
    { "MAX_UINT64"              , TYPE_QWORD,   UINT64_MAX          },
    { "MIN_UINT32"              , TYPE_DWORD,   0                   },
    { "MAX_UINT32"              , TYPE_DWORD,   UINT32_MAX          },
    { "MIN_UINT16"              , TYPE_WORD ,   0                   },
    { "MAX_UINT16"              , TYPE_WORD ,   UINT16_MAX          },
    { "MIN_UINT8"               , TYPE_BYTE ,   0                   },
    { "MAX_UINT8"               , TYPE_BYTE ,   UINT8_MAX           },

    { "MIN_BYTE"                , TYPE_BYTE ,   0                   },
    { "MAX_BYTE"                , TYPE_BYTE ,   UINT8_MAX           },
    { "MIN_WORD"                , TYPE_WORD ,   0                   },
    { "MAX_WORD"                , TYPE_WORD ,   UINT16_MAX          },
    { "MIN_DWORD"               , TYPE_DWORD,   0                   },
    { "MAX_DWORD"               , TYPE_DWORD,   UINT32_MAX          },
    { "MIN_QWORD"               , TYPE_QWORD,   0                   },
    { "MAX_QWORD"               , TYPE_QWORD,   UINT64_MAX          },

    { "MIN_SBYTE"               , TYPE_SBYTE,   INT8_MIN            },
    { "MAX_SBYTE"               , TYPE_SBYTE,   INT8_MAX            },
    { "MIN_SHORT"               , TYPE_SHORT,   INT16_MIN           },
    { "MAX_SHORT"               , TYPE_SHORT,   INT16_MAX           },
    { "MIN_INT"                 , TYPE_INT,     INT64_MIN           },
    { "MAX_INT"                 , TYPE_INT,     INT64_MAX           },
    { "MIN_LONG"                , TYPE_INT,     INT64_MIN           },
    { "MAX_LONG"                , TYPE_INT,     INT64_MAX           },

    { "MIN_CHAR"                , TYPE_BYTE ,   0                   },
    { "MAX_CHAR"                , TYPE_BYTE ,   UINT8_MAX           },

    { "MIN_ULONG"               , TYPE_QWORD,   0                   },
    { "MAX_ULONG"               , TYPE_QWORD,   UINT64_MAX          },

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
    { "INT64_MIN"    , "MIN_INT64"       },
    { "INT64_MAX"    , "MAX_INT64"       },
    { "INT32_MIN"    , "MIN_INT32"       },
    { "INT32_MAX"    , "MAX_INT32"       },
    { "INT16_MIN"    , "MIN_INT16"       },
    { "INT16_MAX"    , "MAX_INT16"       },
    { "INT8_MIN"     , "MIN_INT8"        },
    { "INT8_MAX"     , "MAX_INT8"        },

    { "UINT64_MIN"   , "MIN_UINT64"      },
    { "UINT64_MAX"   , "MAX_UINT64"      },
    { "UINT32_MIN"   , "MIN_UINT32"      },
    { "UINT32_MAX"   , "MAX_UINT32"      },
    { "UINT16_MIN"   , "MIN_UINT16"      },
    { "UINT16_MAX"   , "MAX_UINT16"      },
    { "UINT8_MIN"    , "MIN_UINT8"       },
    { "UINT8_MAX"    , "MAX_UINT8"       },

    { "BYTE_MIN"     , "MIN_BYTE"        },
    { "BYTE_MAX"     , "MAX_BYTE"        },
    { "WORD_MIN"     , "MIN_WORD"        },
    { "WORD_MAX"     , "MAX_WORD"        },
    { "DWORD_MIN"    , "MIN_DWORD"       },
    { "DWORD_MAX"    , "MAX_DWORD"       },
    { "QWORD_MIN"    , "MIN_QWORD"       },
    { "QWORD_MAX"    , "MAX_QWORD"       },

    { "SBYTE_MIN"    , "MIN_SBYTE"       },
    { "SBYTE_MAX"    , "MAX_SBYTE"       },
    { "SHORT_MIN"    , "MIN_SHORT"       },
    { "SHORT_MAX"    , "MAX_SHORT"       },
    { "INT_MIN"      , "MIN_INT"         },
    { "INT_MAX"      , "MAX_INT"         },
    { "LONG_MIN"     , "MIN_LONG"        },
    { "LONG_MAX"     , "MAX_LONG"        },

    { "CHAR_MIN"     , "MIN_CHAR"        },
    { "CHAR_MAX"     , "MAX_CHAR"        },

    { "ULONG_MIN"    , "MIN_ULONG"       },
    { "ULONG_MAX"    , "MAX_ULONG"       },

    { "MIN_BOOL"     , "FALSE"           },
    { "MAX_BOOL"     , "TRUE"            },

    { "BOOL_MIN"     , "MIN_BOOL"        },
    { "BOOL_MAX"     , "MAX_BOOL"        },

    { NULL           , NULL              }
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
    compile_varspace( &global, globaldata, 1, 1, NULL, DEFAULT_ALIGNMENT, 0, 0, 0, 0 );

    token_init( locals_def, -1 );
    compile_varspace( &local, localdata, 1, 1, NULL, DEFAULT_ALIGNMENT, 0, 0, 0, 0 );
}
