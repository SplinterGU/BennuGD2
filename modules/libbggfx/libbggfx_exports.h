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

#ifndef __LIBBGFGX_EXPORTS
#define __LIBBGFGX_EXPORTS

/* --------------------------------------------------------------------------- */

#include "bgddl.h"

/* --------------------------------------------------------------------------- */

#if defined(__BGDC__) || !defined(__STATIC__)

/* --------------------------------------------------------------------------- */

#include "libbggfx.h"

/* --------------------------------------------------------------------------- */

DLCONSTANT  __bgdexport( libbggfx, constants_def )[] = {
    { "M320X200"            , TYPE_QWORD    , 3200200               },
    { "M320X240"            , TYPE_QWORD    , 3200240               },
    { "M320X400"            , TYPE_QWORD    , 3200400               },
    { "M360X240"            , TYPE_QWORD    , 3600240               },
    { "M376X282"            , TYPE_QWORD    , 3760282               },
    { "M400X300"            , TYPE_QWORD    , 4000300               },
    { "M512X384"            , TYPE_QWORD    , 5120384               },
    { "M640X400"            , TYPE_QWORD    , 6400400               },
    { "M640X480"            , TYPE_QWORD    , 6400480               },
    { "M800X600"            , TYPE_QWORD    , 8000600               },
    { "M1024X768"           , TYPE_QWORD    , 10240768              },
    { "M1280X1024"          , TYPE_QWORD    , 12801024              },

    { "MODE_WINDOW"         , TYPE_QWORD    , MODE_WINDOW           },
    { "MODE_FULLSCREEN"     , TYPE_QWORD    , MODE_FULLSCREEN       },

    { "MODE_WAITVSYNC"      , TYPE_QWORD    , MODE_WAITVSYNC        },
    { "WAITVSYNC"           , TYPE_QWORD    , MODE_WAITVSYNC        },

    { "MODE_MODAL"          , TYPE_QWORD    , MODE_MODAL            },  /* GRAB INPU */
    { "MODE_FRAMELESS"      , TYPE_QWORD    , MODE_FRAMELESS        },  /* FRAMELESS window */

    { "SRA_STRETCH"         , TYPE_QWORD    , SRA_STRETCH           },
    { "SRA_PRESERVE"        , TYPE_QWORD    , SRA_PRESERVE          },

    /* --- */
    { "C_SCREEN"            , TYPE_QWORD    , C_SCREEN              },
    { "SCREEN"              , TYPE_QWORD    , -1                    },

    /* Flags para gr_blit */
    { "B_HMIRROR"           , TYPE_QWORD    , B_HMIRROR             },
    { "B_VMIRROR"           , TYPE_QWORD    , B_VMIRROR             },
    { "B_TRANSLUCENT"       , TYPE_QWORD    , B_TRANSLUCENT         },
    { "B_ABLEND"            , TYPE_QWORD    , B_ABLEND              },
    { "B_SBLEND"            , TYPE_QWORD    , B_SBLEND              },
    { "B_MBLEND"            , TYPE_QWORD    , B_MBLEND              },
    { "B_NOCOLORKEY"        , TYPE_QWORD    , B_NOCOLORKEY          },

    { "CPOINT_UNDEFINED"    , TYPE_QWORD    , CPOINT_UNDEFINED      },

    /*
    { "BITMAP_STATIC"       , TYPE_QWORD    , BITMAP_STATIC         },
    { "BITMAP_STREAMING"    , TYPE_QWORD    , BITMAP_STREAMING      },
    { "BITMAP_TARGET"       , TYPE_QWORD    , BITMAP_TARGET         },
    */

    /* Scroll */
    { "C_SCROLL"            , TYPE_QWORD    , C_SCROLL              },

    { "C_0"                 , TYPE_QWORD    , 0x0001                },
    { "C_1"                 , TYPE_QWORD    , 0x0002                },
    { "C_2"                 , TYPE_QWORD    , 0x0004                },
    { "C_3"                 , TYPE_QWORD    , 0x0008                },
    { "C_4"                 , TYPE_QWORD    , 0x0010                },
    { "C_5"                 , TYPE_QWORD    , 0x0020                },
    { "C_6"                 , TYPE_QWORD    , 0x0040                },
    { "C_7"                 , TYPE_QWORD    , 0x0080                },
    { "C_8"                 , TYPE_QWORD    , 0x0100                },
    { "C_9"                 , TYPE_QWORD    , 0x0200                },

    { NULL                  , 0             , 0                     }
} ;

/* --------------------------------------------------------------------------- */
/* Definicion de variables globales (usada en tiempo de compilacion) */

char * __bgdexport( libbggfx, globals_def ) =
    /* Video */
    "INT graph_mode=0;\n"
    "INT full_screen=0;\n"
    "INT scale_resolution=-1;\n"
    "INT scale_resolution_aspectratio=0;\n"

    /* Frame */
    "INT fps;\n"
    "INT speed_gauge=0;\n"
    "DOUBLE frame_time=0;\n"

    /* Fade */
    "INT fading;\n"

    /* wm */
    "INT exit_status=0;\n"                /* SDL_QUIT status */
    "INT window_status=1;\n"              /* MINIMIZED:0 VISIBLE:1 */
    "INT focus_status=1;\n"               /* FOCUS status */
    "INT mouse_status=1;\n"               /* MOUSE status (INSIDE WINDOW:1) */

    /* text */
    "INT text_z=-256;\n"
    "INT text_flags;\n"
    "BYTE text_alpha=255;\n"
    "BYTE text_color_r=255;\n"
    "BYTE text_color_g=255;\n"
    "BYTE text_color_b=255;\n"

    /* backgound */
    "INT background_file=0;\n"
    "INT background_graph=0;\n"
    "BYTE background_color_r=255;\n"
    "BYTE background_color_g=255;\n"
    "BYTE background_color_b=255;\n"
    "INT background_flags=0;\n"
    "INT background_angle=0;\n"
    "INT background_size=100;\n"
    "INT background_size_x=100;\n"
    "INT background_size_y=100;\n"

    /* scroll */
    "STRUCT scroll[9]\n"
        "INT x0;\n"
        "INT y0;\n"
        "INT x1;\n"
        "INT y1;\n"
        "INT z = 512;\n"
        "INT camera;\n"
        "INT ratio = 200;\n"
        "INT speed;\n"
        "INT region1 = -1;\n"
        "INT region2 = -1;\n"
        "INT flags1;\n"
        "INT flags2;\n"
        "INT follow = -1;\n"
        "BYTE alpha = 255;\n"
        "BYTE color_r = 255;\n"
        "BYTE color_g = 255;\n"
        "BYTE color_b = 255;\n"
        "BYTE alpha2 = 255;\n"
        "BYTE color_r2 = 255;\n"
        "BYTE color_g2 = 255;\n"
        "BYTE color_b2 = 255;\n"
        "INT reserved[7];\n"
    "END\n"
    ;

/* --------------------------------------------------------------------------- */

char * __bgdexport( libbggfx, locals_def ) =

    /* Render/Scroll */

    "INT ctype;\n"
    "INT cnumber;\n"

    /* Render */

    "INT x;\n"
    "INT y;\n"
    "INT z;\n"
    "INT file;\n"
    "INT graph;\n"
    "INT size=100;\n"
    "INT angle;\n"
    "INT flags;\n"
    "INT region;\n"
    "INT resolution;\n"
    "INT size_x=100;\n"
    "INT size_y=100;\n"
    "INT POINTER xgraph;\n"

    "INT render_file=0;\n"
    "INT render_graph=0;\n"

    "INT clip_x=0;\n"
    "INT clip_y=0;\n"
    "INT clip_w=0;\n"
    "INT clip_h=0;\n"

    "BYTE alpha=255;\n"
    "BYTE color_r=255;\n"
    "BYTE color_g=255;\n"
    "BYTE color_b=255;\n"

    "STRUCT _render_reserved_\n"
        "INT object_id=0;\n"
        "INT graph_ptr=0;\n"
        "INT xgraph_flags;\n"
    "END\n"
    ;

#endif

/* --------------------------------------------------------------------------- */

char * __bgdexport( libbggfx, modules_dependency )[] = {
    "libsdlhandler",
    NULL
};

/* --------------------------------------------------------------------------- */

#endif
