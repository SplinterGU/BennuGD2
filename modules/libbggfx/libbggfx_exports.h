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

    { "MODE_WINDOW"                     , TYPE_QWORD    , MODE_WINDOW                   },
    { "MODE_FULLSCREEN"                 , TYPE_QWORD    , MODE_FULLSCREEN               },

    { "MODE_WAITVSYNC"                  , TYPE_QWORD    , MODE_WAITVSYNC                },
    { "WAITVSYNC"                       , TYPE_QWORD    , MODE_WAITVSYNC                },

    { "MODE_MODAL"                      , TYPE_QWORD    , MODE_MODAL                    },  /* GRAB INPU */
    { "MODE_FRAMELESS"                  , TYPE_QWORD    , MODE_FRAMELESS                },  /* FRAMELESS window */

    { "SRA_PRESERVE"                    , TYPE_QWORD    , SRA_PRESERVE                  },
    { "SRA_OVERSCAN"                    , TYPE_QWORD    , SRA_OVERSCAN                  },
    { "SRA_FIT"                         , TYPE_QWORD    , SRA_FIT                       },

    /* --- */
    { "C_SCREEN"                        , TYPE_QWORD    , C_SCREEN                      },

    /* Flags para gr_blit */
    { "B_HMIRROR"                       , TYPE_QWORD    , B_HMIRROR                     },
    { "B_VMIRROR"                       , TYPE_QWORD    , B_VMIRROR                     },
    { "B_TRANSLUCENT"                   , TYPE_QWORD    , B_TRANSLUCENT                 },
    { "B_ABLEND"                        , TYPE_QWORD    , B_ABLEND                      },
    { "B_SBLEND"                        , TYPE_QWORD    , B_SBLEND                      },
    { "B_NOCOLORKEY"                    , TYPE_QWORD    , B_NOCOLORKEY                  },

    { "CPOINT_UNDEFINED"                , TYPE_QWORD    , CPOINT_UNDEFINED              },
    { "POINT_UNDEFINED"                 , TYPE_INT      , POINT_UNDEFINED               },

    { "BLEND_CUSTOM"                    , TYPE_INT      , BLEND_CUSTOM                  },
    { "BLEND_DISABLED"                  , TYPE_INT      , BLEND_DISABLED                },
    { "BLEND_NONE"                      , TYPE_INT      , BLEND_NONE                    },
    { "BLEND_NORMAL"                    , TYPE_INT      , BLEND_NORMAL                  },
    { "BLEND_PREMULTIPLIED_ALPHA"       , TYPE_INT      , BLEND_PREMULTIPLIED_ALPHA     },
    { "BLEND_MULTIPLY"                  , TYPE_INT      , BLEND_MULTIPLY                },
    { "BLEND_ADD"                       , TYPE_INT      , BLEND_ADD                     },
    { "BLEND_SUBTRACT"                  , TYPE_INT      , BLEND_SUBTRACT                },
    { "BLEND_MOD_ALPHA"                 , TYPE_INT      , BLEND_MOD_ALPHA               },
    { "BLEND_SET_ALPHA"                 , TYPE_INT      , BLEND_SET_ALPHA               },
    { "BLEND_SET"                       , TYPE_INT      , BLEND_SET                     },
    { "BLEND_NORMAL_KEEP_ALPHA"         , TYPE_INT      , BLEND_NORMAL_KEEP_ALPHA       },
    { "BLEND_NORMAL_ADD_ALPHA"          , TYPE_INT      , BLEND_NORMAL_ADD_ALPHA        },
    { "BLEND_NORMAL_FACTOR_ALPHA"       , TYPE_INT      , BLEND_NORMAL_FACTOR_ALPHA     },
    { "BLEND_ALPHA_MASK"                , TYPE_INT      , BLEND_ALPHA_MASK              },

    { "GL_ZERO"                         , TYPE_QWORD    , 0                             },
    { "GL_ONE"                          , TYPE_QWORD    , 1                             },
    { "GL_SRC_COLOR"                    , TYPE_QWORD    , 0x0300                        },
    { "GL_ONE_MINUS_SRC_COLOR"          , TYPE_QWORD    , 0x0301                        },
    { "GL_SRC_ALPHA"                    , TYPE_QWORD    , 0x0302                        },
    { "GL_ONE_MINUS_SRC_ALPHA"          , TYPE_QWORD    , 0x0303                        },
    { "GL_DST_ALPHA"                    , TYPE_QWORD    , 0x0304                        },
    { "GL_ONE_MINUS_DST_ALPHA"          , TYPE_QWORD    , 0x0305                        },
    { "GL_DST_COLOR"                    , TYPE_QWORD    , 0x0306                        },
    { "GL_ONE_MINUS_DST_COLOR"          , TYPE_QWORD    , 0x0307                        },
    { "GL_SRC_ALPHA_SATURATE"           , TYPE_QWORD    , 0x0308                        },
    { "GL_CONSTANT_COLOR"               , TYPE_QWORD    , 0x8001                        },
    { "GL_ONE_MINUS_CONSTANT_COLOR"     , TYPE_QWORD    , 0x8002                        },
    { "GL_CONSTANT_ALPHA"               , TYPE_QWORD    , 0x8003                        },
    { "GL_ONE_MINUS_CONSTANT_ALPHA"     , TYPE_QWORD    , 0x8004                        },

    { "GL_FUNC_ADD"                     , TYPE_QWORD    , 0x8006                        },
    { "GL_MIN"                          , TYPE_QWORD    , 0x8007                        },
    { "GL_MAX"                          , TYPE_QWORD    , 0x8008                        },
    { "GL_FUNC_SUBTRACT"                , TYPE_QWORD    , 0x800A                        },
    { "GL_FUNC_REVERSE_SUBTRACT"        , TYPE_QWORD    , 0x800B                        },

    /* Scroll */
    { "C_SCROLL"                        , TYPE_QWORD    , C_SCROLL                      },

    { "C_0"                             , TYPE_QWORD    , 1LL << 0                      },
    { "C_1"                             , TYPE_QWORD    , 1LL << 1                      },
    { "C_2"                             , TYPE_QWORD    , 1LL << 2                      },
    { "C_3"                             , TYPE_QWORD    , 1LL << 3                      },
    { "C_4"                             , TYPE_QWORD    , 1LL << 4                      },
    { "C_5"                             , TYPE_QWORD    , 1LL << 5                      },
    { "C_6"                             , TYPE_QWORD    , 1LL << 6                      },
    { "C_7"                             , TYPE_QWORD    , 1LL << 7                      },
    { "C_8"                             , TYPE_QWORD    , 1LL << 8                      },
    { "C_9"                             , TYPE_QWORD    , 1LL << 9                      },
    { "C_10"                            , TYPE_QWORD    , 1LL << 10                     },
    { "C_11"                            , TYPE_QWORD    , 1LL << 11                     },
    { "C_12"                            , TYPE_QWORD    , 1LL << 12                     },
    { "C_13"                            , TYPE_QWORD    , 1LL << 13                     },
    { "C_14"                            , TYPE_QWORD    , 1LL << 14                     },
    { "C_15"                            , TYPE_QWORD    , 1LL << 15                     },
    { "C_16"                            , TYPE_QWORD    , 1LL << 16                     },
    { "C_17"                            , TYPE_QWORD    , 1LL << 17                     },
    { "C_18"                            , TYPE_QWORD    , 1LL << 18                     },
    { "C_19"                            , TYPE_QWORD    , 1LL << 19                     },
    { "C_20"                            , TYPE_QWORD    , 1LL << 20                     },
    { "C_21"                            , TYPE_QWORD    , 1LL << 21                     },
    { "C_22"                            , TYPE_QWORD    , 1LL << 22                     },
    { "C_23"                            , TYPE_QWORD    , 1LL << 23                     },
    { "C_24"                            , TYPE_QWORD    , 1LL << 24                     },
    { "C_25"                            , TYPE_QWORD    , 1LL << 25                     },
    { "C_26"                            , TYPE_QWORD    , 1LL << 26                     },
    { "C_27"                            , TYPE_QWORD    , 1LL << 27                     },
    { "C_28"                            , TYPE_QWORD    , 1LL << 28                     },
    { "C_29"                            , TYPE_QWORD    , 1LL << 29                     },
    { "C_30"                            , TYPE_QWORD    , 1LL << 30                     },
    { "C_31"                            , TYPE_QWORD    , 1LL << 31                     },
    { "C_32"                            , TYPE_QWORD    , 1LL << 32                     },
    { "C_33"                            , TYPE_QWORD    , 1LL << 33                     },
    { "C_34"                            , TYPE_QWORD    , 1LL << 34                     },
    { "C_35"                            , TYPE_QWORD    , 1LL << 35                     },
    { "C_36"                            , TYPE_QWORD    , 1LL << 36                     },
    { "C_37"                            , TYPE_QWORD    , 1LL << 37                     },
    { "C_38"                            , TYPE_QWORD    , 1LL << 38                     },
    { "C_39"                            , TYPE_QWORD    , 1LL << 39                     },
    { "C_40"                            , TYPE_QWORD    , 1LL << 40                     },
    { "C_41"                            , TYPE_QWORD    , 1LL << 41                     },
    { "C_42"                            , TYPE_QWORD    , 1LL << 42                     },
    { "C_43"                            , TYPE_QWORD    , 1LL << 43                     },
    { "C_44"                            , TYPE_QWORD    , 1LL << 44                     },
    { "C_45"                            , TYPE_QWORD    , 1LL << 45                     },
    { "C_46"                            , TYPE_QWORD    , 1LL << 46                     },
    { "C_47"                            , TYPE_QWORD    , 1LL << 47                     },
    { "C_48"                            , TYPE_QWORD    , 1LL << 48                     },
    { "C_49"                            , TYPE_QWORD    , 1LL << 49                     },
    { "C_50"                            , TYPE_QWORD    , 1LL << 50                     },
    { "C_51"                            , TYPE_QWORD    , 1LL << 51                     },
    { "C_52"                            , TYPE_QWORD    , 1LL << 52                     },
    { "C_53"                            , TYPE_QWORD    , 1LL << 53                     },
    { "C_54"                            , TYPE_QWORD    , 1LL << 54                     },
    { "C_55"                            , TYPE_QWORD    , 1LL << 55                     },
    { "C_56"                            , TYPE_QWORD    , 1LL << 56                     },
    { "C_57"                            , TYPE_QWORD    , 1LL << 57                     },
    { "C_58"                            , TYPE_QWORD    , 1LL << 58                     },
    { "C_59"                            , TYPE_QWORD    , 1LL << 59                     },
    { "C_60"                            , TYPE_QWORD    , 1LL << 60                     },
    { "C_61"                            , TYPE_QWORD    , 1LL << 61                     },
    { "C_62"                            , TYPE_QWORD    , 1LL << 62                     },
    { "C_63"                            , TYPE_QWORD    , 1ULL << 63                    },

    { NULL                              , 0             , 0                             }
} ;

/* --------------------------------------------------------------------------- */

char * __bgdexport( libbggfx, types_def ) =
    /* m_draw */
    "TYPE G_POINT\n"
    "   INT x;\n"
    "   INT y;\n"
    "END\n"

    "TYPE G_POINTW\n"
    "   INT32 x;\n"
    "   INT32 y;\n"
    "END\n"

    "TYPE G_POINTF\n"
    "   FLOAT x;\n"
    "   FLOAT y;\n"
    "END\n"

    "TYPE G_POINTD\n"
    "   DOUBLE x;\n"
    "   DOUBLE y;\n"
    "END\n"

    "TYPE G_RECT\n"
    "   INT x;\n"
    "   INT y;\n"
    "   INT w;\n"
    "   INT h;\n"
    "END\n"

    "TYPE G_RECTW\n"
    "   INT32 x;\n"
    "   INT32 y;\n"
    "   INT32 w;\n"
    "   INT32 h;\n"
    "END\n"

    "TYPE G_RECTF\n"
    "   FLOAT x;\n"
    "   FLOAT y;\n"
    "   FLOAT w;\n"
    "   FLOAT h;\n"
    "END\n"

    "TYPE G_RECTD\n"
    "   DOUBLE x;\n"
    "   DOUBLE y;\n"
    "   DOUBLE w;\n"
    "   DOUBLE h;\n"
    "END\n"

    "TYPE G_CUSTOM_BLENDMODE\n"
    "   INT src_rgb;\n"
    "   INT dst_rgb;\n"
    "   INT src_alpha;\n"
    "   INT dst_alpha;\n"
    "   INT equation_rgb;\n"
    "   INT equation_alpha;\n"
    "END\n"
    ;

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
    "   INT blendmode=" TOSTRING(BLEND_DISABLED) ";\n"
    "   G_CUSTOM_BLENDMODE custom_blendmode;\n"
    "   UINT * shader=NULL;\n"
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
    "   DOUBLE size=100.0;\n"
    "   DOUBLE size_x=100.0;\n"
    "   DOUBLE size_y=100.0;\n"
    "   INT blendmode=" TOSTRING(BLEND_DISABLED) ";\n"
    "   G_CUSTOM_BLENDMODE custom_blendmode;\n"
    "   UINT * shader=NULL;\n"
    "END\n"

    /* scroll */
    "STRUCT scroll[63]\n"
    "   DOUBLE x0;\n"
    "   DOUBLE y0;\n"
    "   DOUBLE x1;\n"
    "   DOUBLE y1;\n"
    "   INT z=512;\n"
    "   INT camera;\n"
    "   DOUBLE ratio=200.0;\n"
    "   DOUBLE speed;\n"
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
    "   INT blendmode1=" TOSTRING(BLEND_DISABLED) ";\n"
    "   G_CUSTOM_BLENDMODE custom_blendmode1;\n"
    "   INT blendmode2=" TOSTRING(BLEND_DISABLED) ";\n"
    "   G_CUSTOM_BLENDMODE custom_blendmode2;\n"
    "   UINT * shader1=NULL;\n"
    "   UINT * shader2=NULL;\n"
    "END\n"
    ;

/* --------------------------------------------------------------------------- */

char * __bgdexport( libbggfx, locals_def ) =

    /* Render/Scroll */
    "INT ctype;\n"
    "INT cnumber;\n"

    /* Render */
    "DOUBLE x;\n"
    "DOUBLE y;\n"
    "INT z;\n"
    "INT file;\n"
    "INT graph;\n"
    "DOUBLE size=100.0;\n"
    "INT angle;\n"
    "INT flags;\n"
    "INT region;\n"
    "INT resolution;\n"
    "DOUBLE size_x=100.0;\n"
    "DOUBLE size_y=100.0;\n"
    "INT POINTER xgraph;\n"

    "INT render_file=0;\n"
    "INT render_graph=0;\n"

    "G_RECT clip = 0, 0, 0, 0;\n"
    "G_POINTD center = POINT_UNDEFINED, POINT_UNDEFINED;\n"

    "BYTE alpha=255;\n"
    "BYTE color_r=255;\n"
    "BYTE color_g=255;\n"
    "BYTE color_b=255;\n"

    "STRUCT _render_reserved_\n"
    "   INT object_id=0;\n"
//    "   INT graph_ptr=0;\n"
    "   INT xgraph_flags;\n"
    "END\n"

    "INT blendmode=" TOSTRING(BLEND_DISABLED) ";\n"
    "G_CUSTOM_BLENDMODE custom_blendmode;\n"

    "UINT * shader = NULL;\n"
    ;
#endif

/* --------------------------------------------------------------------------- */

char * __bgdexport( libbggfx, modules_dependency )[] = {
    "libsdlhandler",
    NULL
};

/* --------------------------------------------------------------------------- */

#endif
