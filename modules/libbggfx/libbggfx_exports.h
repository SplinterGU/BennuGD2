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

    { "MODE_WINDOW"         , TYPE_QWORD    , MODE_WINDOW           },
    { "MODE_FULLSCREEN"     , TYPE_QWORD    , MODE_FULLSCREEN       },

    { "MODE_WAITVSYNC"      , TYPE_QWORD    , MODE_WAITVSYNC        },
    { "WAITVSYNC"           , TYPE_QWORD    , MODE_WAITVSYNC        },

    { "MODE_MODAL"          , TYPE_QWORD    , MODE_MODAL            },  /* GRAB INPU */
    { "MODE_FRAMELESS"      , TYPE_QWORD    , MODE_FRAMELESS        },  /* FRAMELESS window */

    { "SRA_PRESERVE"        , TYPE_QWORD    , SRA_PRESERVE          },
    { "SRA_OVERSCAN"        , TYPE_QWORD    , SRA_OVERSCAN          },
    { "SRA_FIT"             , TYPE_QWORD    , SRA_FIT               },

    /* --- */
    { "C_SCREEN"            , TYPE_QWORD    , C_SCREEN              },

    /* Flags para gr_blit */
    { "B_HMIRROR"           , TYPE_QWORD    , B_HMIRROR             },
    { "B_VMIRROR"           , TYPE_QWORD    , B_VMIRROR             },
    { "B_TRANSLUCENT"       , TYPE_QWORD    , B_TRANSLUCENT         },
    { "B_ABLEND"            , TYPE_QWORD    , B_ABLEND              },
    { "B_SBLEND"            , TYPE_QWORD    , B_SBLEND              },
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
    "STRUCT screen\n"
    "    INT fullscreen=0;\n"
    "    INT scale_resolution=-1;\n"
    "    INT scale_resolution_aspectratio=0;\n"
    "END\n"

    /* Frame */
    "STRUCT frame_info\n"
    "   INT fps;\n"
    "   INT speed_gauge=0;\n"
    "   DOUBLE frame_time=0;\n"
    "   INT frames_count=0;\n"
    "END\n"

    /* Fade */
    "STRUCT fade_info\n"
    "   INT fading;\n"
    "END\n"

    /* wm */
    "STRUCT wm_info\n"
    "   INT exit_status=0;\n"                /* SDL_QUIT status */
    "   INT window_status=1;\n"              /* MINIMIZED:0 VISIBLE:1 */
    "   INT focus_status=1;\n"               /* FOCUS status */
    "   INT mouse_status=1;\n"               /* MOUSE status (INSIDE WINDOW:1) */
    "END\n"

    /* text */
    "STRUCT text\n"
    "   INT z=-512;\n"
    "   INT flags;\n"
    "   BYTE alpha=255;\n"
    "   BYTE color_r=255;\n"
    "   BYTE color_g=255;\n"
    "   BYTE color_b=255;\n"
    "END\n"

    /* backgound */
    "STRUCT background\n"
    "   INT file=0;\n"
    "   INT graph=0;\n"
    "   BYTE color_r=255;\n"
    "   BYTE color_g=255;\n"
    "   BYTE color_b=255;\n"
    "   INT flags=0;\n"
    "   INT angle=0;\n"
    "   INT size=100;\n"
    "   INT size_x=100;\n"
    "   INT size_y=100;\n"
    "END\n"

    /* scroll */
    "STRUCT scroll[9]\n"
    "   INT x0;\n"
    "   INT y0;\n"
    "   INT x1;\n"
    "   INT y1;\n"
    "   INT z=512;\n"
    "   INT camera;\n"
    "   INT ratio=200;\n"
    "   INT speed;\n"
    "   INT region1=-1;\n"
    "   INT region2=-1;\n"
    "   INT flags1;\n"
    "   INT flags2;\n"
    "   INT follow=-1;\n"
    "   BYTE alpha=255;\n"
    "   BYTE color_r=255;\n"
    "   BYTE color_g=255;\n"
    "   BYTE color_b=255;\n"
    "   BYTE alpha2=255;\n"
    "   BYTE color_r2=255;\n"
    "   BYTE color_g2=255;\n"
    "   BYTE color_b2=255;\n"
    "   INT reserved[7];\n"
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
    "   INT object_id=0;\n"
    "   INT graph_ptr=0;\n"
    "   INT xgraph_flags;\n"
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
