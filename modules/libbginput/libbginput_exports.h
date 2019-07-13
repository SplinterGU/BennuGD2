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

#ifndef __LIBBGINPUT_EXPORTS
#define __LIBBGINPUT_EXPORTS

/* ----------------------------------------------------------------- */

#if defined(__BGDC__) || !defined(__STATIC__)

/* ----------------------------------------------------------------- */

#include "bgddl.h"
#include "libbginput.h"

/* ----------------------------------------------------------------- */

DLCONSTANT  __bgdexport( libbginput, constants_def )[] = {
    { "_ESC"                    , TYPE_QWORD    , 1                 },
    { "_1"                      , TYPE_QWORD    , 2                 },
    { "_2"                      , TYPE_QWORD    , 3                 },
    { "_3"                      , TYPE_QWORD    , 4                 },
    { "_4"                      , TYPE_QWORD    , 5                 },
    { "_5"                      , TYPE_QWORD    , 6                 },
    { "_6"                      , TYPE_QWORD    , 7                 },
    { "_7"                      , TYPE_QWORD    , 8                 },
    { "_8"                      , TYPE_QWORD    , 9                 },
    { "_9"                      , TYPE_QWORD    , 10                },
    { "_0"                      , TYPE_QWORD    , 11                },
    { "_MINUS"                  , TYPE_QWORD    , 12                },
    { "_PLUS"                   , TYPE_QWORD    , 13                },
    { "_BACKSPACE"              , TYPE_QWORD    , 14                },
    { "_TAB"                    , TYPE_QWORD    , 15                },
    { "_Q"                      , TYPE_QWORD    , 16                },
    { "_W"                      , TYPE_QWORD    , 17                },
    { "_E"                      , TYPE_QWORD    , 18                },
    { "_R"                      , TYPE_QWORD    , 19                },
    { "_T"                      , TYPE_QWORD    , 20                },
    { "_Y"                      , TYPE_QWORD    , 21                },
    { "_U"                      , TYPE_QWORD    , 22                },
    { "_I"                      , TYPE_QWORD    , 23                },
    { "_O"                      , TYPE_QWORD    , 24                },
    { "_P"                      , TYPE_QWORD    , 25                },
    { "_L_BRACKET"              , TYPE_QWORD    , 26                },
    { "_R_BRACKET"              , TYPE_QWORD    , 27                },
    { "_ENTER"                  , TYPE_QWORD    , 28                },
    { "_C_ENTER"                , TYPE_QWORD    , 28                },
    { "_CONTROL"                , TYPE_QWORD    , 29                },
    { "_A"                      , TYPE_QWORD    , 30                },
    { "_S"                      , TYPE_QWORD    , 31                },
    { "_D"                      , TYPE_QWORD    , 32                },
    { "_F"                      , TYPE_QWORD    , 33                },
    { "_G"                      , TYPE_QWORD    , 34                },
    { "_H"                      , TYPE_QWORD    , 35                },
    { "_J"                      , TYPE_QWORD    , 36                },
    { "_K"                      , TYPE_QWORD    , 37                },
    { "_L"                      , TYPE_QWORD    , 38                },
    { "_SEMICOLON"              , TYPE_QWORD    , 39                },
    { "_APOSTROPHE"             , TYPE_QWORD    , 40                },
    { "_WAVE"                   , TYPE_QWORD    , 41                },
    { "_L_SHIFT"                , TYPE_QWORD    , 42                },
    { "_BACKSLASH"              , TYPE_QWORD    , 43                },
    { "_Z"                      , TYPE_QWORD    , 44                },
    { "_X"                      , TYPE_QWORD    , 45                },
    { "_C"                      , TYPE_QWORD    , 46                },
    { "_V"                      , TYPE_QWORD    , 47                },
    { "_B"                      , TYPE_QWORD    , 48                },
    { "_N"                      , TYPE_QWORD    , 49                },
    { "_M"                      , TYPE_QWORD    , 50                },
    { "_COMMA"                  , TYPE_QWORD    , 51                },
    { "_POINT"                  , TYPE_QWORD    , 52                },
    { "_SLASH"                  , TYPE_QWORD    , 53                },
    { "_C_BACKSLASH"            , TYPE_QWORD    , 53                },
    { "_R_SHIFT"                , TYPE_QWORD    , 54                },
    { "_C_ASTERISK"             , TYPE_QWORD    , 55                },
    { "_PRN_SCR"                , TYPE_QWORD    , 55                },
    { "_ALT"                    , TYPE_QWORD    , 56                },
    { "_SPACE"                  , TYPE_QWORD    , 57                },
    { "_CAPS_LOCK"              , TYPE_QWORD    , 58                },
    { "_F1"                     , TYPE_QWORD    , 59                },
    { "_F2"                     , TYPE_QWORD    , 60                },
    { "_F3"                     , TYPE_QWORD    , 61                },
    { "_F4"                     , TYPE_QWORD    , 62                },
    { "_F5"                     , TYPE_QWORD    , 63                },
    { "_F6"                     , TYPE_QWORD    , 64                },
    { "_F7"                     , TYPE_QWORD    , 65                },
    { "_F8"                     , TYPE_QWORD    , 66                },
    { "_F9"                     , TYPE_QWORD    , 67                },
    { "_F10"                    , TYPE_QWORD    , 68                },
    { "_NUM_LOCK"               , TYPE_QWORD    , 69                },
    { "_SCROLL_LOCK"            , TYPE_QWORD    , 70                },
    { "_HOME"                   , TYPE_QWORD    , 71                },
    { "_C_HOME"                 , TYPE_QWORD    , 71                },
    { "_UP"                     , TYPE_QWORD    , 72                },
    { "_C_UP"                   , TYPE_QWORD    , 72                },
    { "_PGUP"                   , TYPE_QWORD    , 73                },
    { "_C_PGUP"                 , TYPE_QWORD    , 73                },
    { "_C_MINUS"                , TYPE_QWORD    , 74                },
    { "_LEFT"                   , TYPE_QWORD    , 75                },
    { "_C_LEFT"                 , TYPE_QWORD    , 75                },
    { "_C_CENTER"               , TYPE_QWORD    , 76                },
    { "_RIGHT"                  , TYPE_QWORD    , 77                },
    { "_C_RIGHT"                , TYPE_QWORD    , 77                },
    { "_C_PLUS"                 , TYPE_QWORD    , 78                },
    { "_END"                    , TYPE_QWORD    , 79                },
    { "_C_END"                  , TYPE_QWORD    , 79                },
    { "_DOWN"                   , TYPE_QWORD    , 80                },
    { "_C_DOWN"                 , TYPE_QWORD    , 80                },
    { "_PGDN"                   , TYPE_QWORD    , 81                },
    { "_C_PGDN"                 , TYPE_QWORD    , 81                },
    { "_INS"                    , TYPE_QWORD    , 82                },
    { "_C_INS"                  , TYPE_QWORD    , 82                },
    { "_DEL"                    , TYPE_QWORD    , 83                },
    { "_C_DEL"                  , TYPE_QWORD    , 83                },
    { "_F11"                    , TYPE_QWORD    , 87                },
    { "_F12"                    , TYPE_QWORD    , 88                },
    { "_LESS"                   , TYPE_QWORD    , 89                },
    { "_EQUALS"                 , TYPE_QWORD    , 90                },
    { "_GREATER"                , TYPE_QWORD    , 91                },
    { "_ASTERISK"               , TYPE_QWORD    , 92                },
    { "_R_ALT"                  , TYPE_QWORD    , 93                },
    { "_R_CONTROL"              , TYPE_QWORD    , 94                },
    { "_L_ALT"                  , TYPE_QWORD    , 95                },
    { "_L_CONTROL"              , TYPE_QWORD    , 96                },
    { "_MENU"                   , TYPE_QWORD    , 97                },
    { "_L_WINDOWS"              , TYPE_QWORD    , 98                },
    { "_R_WINDOWS"              , TYPE_QWORD    , 99                },

    { "STAT_RSHIFT"             , TYPE_QWORD    , STAT_RSHIFT       },
    { "STAT_LSHIFT"             , TYPE_QWORD    , STAT_LSHIFT       },
    { "STAT_CTRL"               , TYPE_QWORD    , STAT_CTRL         },
    { "STAT_ALT"                , TYPE_QWORD    , STAT_ALT          },
    { "STAT_RCTRL"              , TYPE_QWORD    , STAT_RCTRL        },
    { "STAT_LCTRL"              , TYPE_QWORD    , STAT_LCTRL        },
    { "STAT_RALT"               , TYPE_QWORD    , STAT_RALT         },
    { "STAT_LALT"               , TYPE_QWORD    , STAT_LALT         },
    { "STAT_NUM"                , TYPE_QWORD    , STAT_NUM          },
    { "STAT_CAPS"               , TYPE_QWORD    , STAT_CAPS         },
    { "STAT_SHIFT"              , TYPE_QWORD    , STAT_SHIFT        },

    /* Joys */
    { "JOY_HAT_CENTERED"        , TYPE_QWORD    , SDL_HAT_CENTERED  },
    { "JOY_HAT_UP"              , TYPE_QWORD    , SDL_HAT_UP        },
    { "JOY_HAT_RIGHT"           , TYPE_QWORD    , SDL_HAT_RIGHT     },
    { "JOY_HAT_DOWN"            , TYPE_QWORD    , SDL_HAT_DOWN      },
    { "JOY_HAT_LEFT"            , TYPE_QWORD    , SDL_HAT_LEFT      },
    { "JOY_HAT_RIGHTUP"         , TYPE_QWORD    , SDL_HAT_RIGHTUP   },
    { "JOY_HAT_RIGHTDOWN"       , TYPE_QWORD    , SDL_HAT_RIGHTDOWN },
    { "JOY_HAT_LEFTUP"          , TYPE_QWORD    , SDL_HAT_LEFTUP    },
    { "JOY_HAT_LEFTDOWN"        , TYPE_QWORD    , SDL_HAT_LEFTDOWN  },

    { NULL                      , 0             , 0                 }
} ;

/* ----------------------------------------------------------------- */
/* Definicion de variables globales (usada en tiempo de compilacion) */

char * __bgdexport( libbginput, globals_def ) =
    "STRUCT keyboard\n"
    "   INT shift_status;\n"
    "   INT ascii;\n"
    "   INT scan_code;\n"
    "END\n"

    "STRUCT mouse\n"
    "   INT x=99999, y=99999;\n"
    "   INT z=-512;\n"
    "   INT file;\n"
    "   INT graph;\n"
    "   INT angle;\n"
    "   INT size=100;\n"
    "   INT size_x=100;\n"
    "   INT size_y=100;\n"
    "   INT flags;\n"
    "   INT region;\n"
    "   INT left, middle, right, x1, x2;\n"
    "   INT wheelup, wheeldown;\n"
    "   INT wheelright, wheelleft;\n"

    "   INT clip_x=0;\n"
    "   INT clip_y=0;\n"
    "   INT clip_w=0;\n"
    "   INT clip_h=0;\n"

    "   BYTE alpha=255;\n"
    "   BYTE color_r=255;\n"
    "   BYTE color_g=255;\n"
    "   BYTE color_b=255;\n"
    "END\n"
    ;

#endif

/* ----------------------------------------------------------------- */

char * __bgdexport( libbginput, modules_dependency )[] = {
    "libsdlhandler",
    "libbggfx",
    NULL
};

/* ----------------------------------------------------------------- */

#endif
