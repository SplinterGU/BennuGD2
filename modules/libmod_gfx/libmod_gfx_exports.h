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

#ifndef __LIBMOD_GFX_EXPORTS
#define __LIBMOD_GFX_EXPORTS

/* ----------------------------------------------------------------- */

#include "bgddl.h"

/* --------------------------------------------------------------------------- */

#if defined(__BGDC__) || !defined(__STATIC__)

/* ----------------------------------------------------------------- */

#include "libmod_gfx.h"

/* ----------------------------------------------------------------- */

DLCONSTANT __bgdexport( libmod_gfx, constants_def )[] = {
    { "G_WIDE"              , TYPE_INT          , G_WIDE                                },    /* Obsolete */
    { "G_WIDTH"             , TYPE_INT          , G_WIDTH                               },
    { "G_HEIGHT"            , TYPE_INT          , G_HEIGHT                              },
    { "G_CENTER_X"          , TYPE_INT          , G_CENTER_X                            },
    { "G_X_CENTER"          , TYPE_INT          , G_X_CENTER                            },
    { "G_CENTER_Y"          , TYPE_INT          , G_CENTER_Y                            },
    { "G_Y_CENTER"          , TYPE_INT          , G_Y_CENTER                            },
    { "G_PITCH"             , TYPE_INT          , G_PITCH                               },
    { "G_DEPTH"             , TYPE_INT          , G_DEPTH                               },

    { "B_CLEAR"             , TYPE_INT          , B_CLEAR                               },

    { "Q_NEAREST"           , TYPE_INT          , Q_NEAREST                             },
    { "Q_LINEAR"            , TYPE_INT          , Q_LINEAR                              },
    { "Q_BEST"              , TYPE_INT          , Q_BEST                                },
#ifdef USE_SDL2_GPU
    { "Q_MIPMAP"            , TYPE_INT          , Q_MIPMAP                              },
#endif
    { "CHARSET_ISO8859"     , TYPE_INT          , CHARSET_ISO8859                       },
    { "CHARSET_CP850"       , TYPE_INT          , CHARSET_CP850                         },

    { "NFB_VARIABLEWIDTH"   , TYPE_INT          , 0                                     },
    { "NFB_FIXEDWIDTH"      , TYPE_INT          , NFB_FIXEDWIDTH                        },

    /* Text */
    { "ALL_TEXT"            , TYPE_INT          , 0                                     },

    /* Texts align */
    { "ALIGN_TOP_LEFT"      , TYPE_INT          , ALIGN_TOP_LEFT                        },
    { "ALIGN_TOP"           , TYPE_INT          , ALIGN_TOP                             },
    { "ALIGN_TOP_RIGHT"     , TYPE_INT          , ALIGN_TOP_RIGHT                       },
    { "ALIGN_CENTER_LEFT"   , TYPE_INT          , ALIGN_CENTER_LEFT                     },
    { "ALIGN_CENTER"        , TYPE_INT          , ALIGN_CENTER                          },
    { "ALIGN_CENTER_RIGHT"  , TYPE_INT          , ALIGN_CENTER_RIGHT                    },
    { "ALIGN_BOTTOM_LEFT"   , TYPE_INT          , ALIGN_BOTTOM_LEFT                     },
    { "ALIGN_BOTTOM"        , TYPE_INT          , ALIGN_BOTTOM                          },
    { "ALIGN_BOTTOM_RIGHT"  , TYPE_INT          , ALIGN_BOTTOM_RIGHT                    },

    /* Pathfind */
    { "PF_DIAG"             , TYPE_INT          , PF_DIAG                               }, /* Allow the pathfinding from using diagonal paths. */

    { "PF_MANHATTAN"        , TYPE_INT          , PF_HEURISTIC_MANHATTAN                },
    { "PF_EUCLIDEAN"        , TYPE_INT          , PF_HEURISTIC_EUCLIDEAN                },
    { "PF_OCTILE"           , TYPE_INT          , PF_HEURISTIC_OCTILE                   },
    { "PF_CHEBYSHEV"        , TYPE_INT          , PF_HEURISTIC_CHEBYSHEV                },

    { "TEXT_TEXT"           , TYPE_INT          , TEXT_TEXT                             },
    { "TEXT_STRING"         , TYPE_INT          , TEXT_STRING                           },
    { "TEXT_INT"            , TYPE_INT          , TEXT_INT                              },
    { "TEXT_QWORD"          , TYPE_INT          , TEXT_QWORD                            },
    { "TEXT_INT32"          , TYPE_INT          , TEXT_INT32                            },
    { "TEXT_DWORD"          , TYPE_INT          , TEXT_DWORD                            },
    { "TEXT_SHORT"          , TYPE_INT          , TEXT_SHORT                            },
    { "TEXT_WORD"           , TYPE_INT          , TEXT_WORD                             },
    { "TEXT_SBYTE"          , TYPE_INT          , TEXT_SBYTE                            },
    { "TEXT_BYTE"           , TYPE_INT          , TEXT_BYTE                             },
    { "TEXT_CHAR"           , TYPE_INT          , TEXT_CHAR                             },
    { "TEXT_DOUBLE"         , TYPE_INT          , TEXT_DOUBLE                           },
    { "TEXT_FLOAT"          , TYPE_INT          , TEXT_FLOAT                            },
    { "TEXT_POINTER"        , TYPE_INT          , TEXT_POINTER                          },
    { "TEXT_CHARARRAY"      , TYPE_INT          , TEXT_CHARARRAY                        },

    { "SHAPE_BOX"           , TYPE_INT          , BITMAP_CB_SHAPE_BOX                   },
    { "SHAPE_CIRCLE"        , TYPE_INT          , BITMAP_CB_SHAPE_CIRCLE                },
    { "GRAPH_SIZE"          , TYPE_INT          , BITMAP_CB_CIRCLE_GRAPH_SIZE           },
    { "GRAPH_WIDTH"         , TYPE_INT          , BITMAP_CB_CIRCLE_GRAPH_WIDTH          },
    { "GRAPH_HEIGHT"        , TYPE_INT          , BITMAP_CB_CIRCLE_GRAPH_HEIGHT         },
    { "GRAPH_MIN_SIZE"      , TYPE_INT          , BITMAP_CB_CIRCLE_GRAPH_MIN_SIZE       },
    { "GRAPH_MAX_SIZE"      , TYPE_INT          , BITMAP_CB_CIRCLE_GRAPH_MAX_SIZE       },
    { "GRAPH_AVERAGE_SIZE"  , TYPE_INT          , BITMAP_CB_CIRCLE_GRAPH_AVERAGE_SIZE   },

    { "POINT_UNDEFINED"     , TYPE_INT          , POINT_UNDEFINED                       },

    { NULL                  , 0                 , 0                                     }
} ;

#endif

/* --------------------------------------------------------------------------- */

char * __bgdexport( libmod_gfx, types_def ) =
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
    ;

char * __bgdexport( libmod_gfx, locals_def ) =
    /* f_gproc */
    "STRUCT __collision_reserved\n"
    "   INT type_scan;\n"
    "   INT id_scan;\n"
    "   INT context;\n"
    "END\n"

    "INT cshape = SHAPE_BOX;\n"
    "G_RECT cbox = POINT_UNDEFINED, POINT_UNDEFINED, GRAPH_SIZE, GRAPH_SIZE;\n"
    ;

/* --------------------------------------------------------------------------- */

DLSYSFUNCS  __bgdexport( libmod_gfx, functions_exports )[] = {

    /* f_grproc */
    FUNC( "ADVANCE"             , "I"               , TYPE_INT        , libmod_gfx_advance              ),
    FUNC( "XADVANCE"            , "II"              , TYPE_INT        , libmod_gfx_xadvance             ),

    FUNC( "GET_ANGLE"           , "I"               , TYPE_INT        , libmod_gfx_get_angle            ),
    FUNC( "GET_ANGLE"           , "II"              , TYPE_INT        , libmod_gfx_get_angle2           ),
    FUNC( "GET_DIST"            , "I"               , TYPE_INT        , libmod_gfx_get_dist             ),
    FUNC( "GET_DIST"            , "II"              , TYPE_INT        , libmod_gfx_get_dist2            ),

    FUNC( "GET_REAL_POINT"      , "IPP"             , TYPE_INT        , libmod_gfx_get_real_point       ),

    FUNC( "COLLISION"           , "I"               , TYPE_INT        , libmod_gfx_collision            ),
//    FUNC( "COLLISION_BOX"       , "I"               , TYPE_INT        , libmod_gfx_collision_box        ),
//    FUNC( "COLLISION_CIRCLE"    , "I"               , TYPE_INT        , libmod_gfx_collision_circle     ),

    /* scroll */
    FUNC( "SCROLL_START"        , "IIIIIIIII"       , TYPE_INT        , libmod_gfx_scroll_start2        ),
    FUNC( "SCROLL_START"        , "IIIIIII"         , TYPE_INT        , libmod_gfx_scroll_start         ),
    FUNC( "SCROLL_STOP"         , "I"               , TYPE_INT        , libmod_gfx_scroll_stop          ),
    FUNC( "SCROLL_MOVE"         , "I"               , TYPE_INT        , libmod_gfx_scroll_move          ),

    /* Regiones */
    FUNC( "REGION_DEFINE"       , "IIIII"           , TYPE_INT        , libmod_gfx_define_region        ),
    FUNC( "REGION_OUT"          , "II"              , TYPE_INT        , libmod_gfx_out_region           ),

    /* Video */
    FUNC( "SCREEN_GET"          , ""                , TYPE_INT        , libmod_gfx_get_screen           ),

    FUNC( "RGB"                 , "BBB"             , TYPE_INT        , libmod_gfx_rgb                  ),
    FUNC( "RGB"                 , "IIBBB"           , TYPE_INT        , libmod_gfx_rgb_map              ),
    FUNC( "RGBA"                , "BBBB"            , TYPE_INT        , libmod_gfx_rgba                 ),
    FUNC( "RGBA"                , "IIBBBB"          , TYPE_INT        , libmod_gfx_rgba_map             ),
    FUNC( "RGB_GET"             , "IPPP"            , TYPE_INT        , libmod_gfx_get_rgb              ),
    FUNC( "RGB_GET"             , "IIIPPP"          , TYPE_INT        , libmod_gfx_get_rgb_map          ),
    FUNC( "RGBA_GET"            , "IPPPP"           , TYPE_INT        , libmod_gfx_get_rgba             ),
    FUNC( "RGBA_GET"            , "IIIPPPP"         , TYPE_INT        , libmod_gfx_get_rgba_map         ),

    FUNC( "FADE"                , "IIIII"           , TYPE_INT        , libmod_gfx_fade                 ),
    FUNC( "FADE"                , "IIIIII"          , TYPE_INT        , libmod_gfx_fade_region          ),
    FUNC( "FADE_ON"             , "I"               , TYPE_INT        , libmod_gfx_fade_on              ),
    FUNC( "FADE_ON"             , "II"              , TYPE_INT        , libmod_gfx_fade_on_region       ),
    FUNC( "FADE_OFF"            , "I"               , TYPE_INT        , libmod_gfx_fade_off             ),
    FUNC( "FADE_OFF"            , "II"              , TYPE_INT        , libmod_gfx_fade_off_region      ),

    /* Video */
    FUNC( "SET_MODE"            , "II"              , TYPE_INT        , libmod_gfx_set_mode             ),
    FUNC( "SET_MODE"            , "III"             , TYPE_INT        , libmod_gfx_set_mode_extended    ),
    FUNC( "SET_FPS"             , "II"              , TYPE_INT        , libmod_gfx_set_fps              ),

//    FUNC( "GET_MODES"           , "II"              , TYPE_POINTER    , libmod_gfx_list_modes           ),
//    FUNC( "MODE_IS_OK"          , "IIII"            , TYPE_INT        , libmod_gfx_mode_is_ok           ),

    FUNC( "WINDOW_SET_TITLE"    , "S"               , TYPE_INT        , libmod_gfx_set_title            ),
    FUNC( "WINDOW_SET_ICON"     , "II"              , TYPE_INT        , libmod_gfx_set_icon             ),
    FUNC( "WINDOW_MOVE"         , "II"              , TYPE_INT        , libmod_gfx_move_window          ),
    FUNC( "WINDOW_SET_POS"      , "II"              , TYPE_INT        , libmod_gfx_move_window          ),
    FUNC( "WINDOW_GET_POS"      , "PP"              , TYPE_INT        , libmod_gfx_get_window_pos       ),
    FUNC( "WINDOW_GET_SIZE"     , "PPPPPP"          , TYPE_INT        , libmod_gfx_get_window_size      ),
    FUNC( "DESKTOP_GET_SIZE"    , "PP"              , TYPE_INT        , libmod_gfx_get_desktop_size     ),

    FUNC( "WINDOW_HIDE"         , ""                , TYPE_INT        , libmod_gfx_hide_window          ),
    FUNC( "WINDOW_SHOW"         , ""                , TYPE_INT        , libmod_gfx_show_window          ),
    FUNC( "WINDOW_MINIMIZE"     , ""                , TYPE_INT        , libmod_gfx_minimize             ),
    FUNC( "WINDOW_MAXIMIZE"     , ""                , TYPE_INT        , libmod_gfx_maximize             ),
    FUNC( "WINDOW_RAISE"        , ""                , TYPE_INT        , libmod_gfx_raise_window         ),
    FUNC( "WINDOW_RESTORE"      , ""                , TYPE_INT        , libmod_gfx_restore_window       ),

    /* Texts */
    FUNC( "WRITE"               , "IIIIS"           , TYPE_INT        , libmod_gfx_text_write           ),
    FUNC( "WRITE"               , "IIIIIS"          , TYPE_INT        , libmod_gfx_text_write2          ),
    FUNC( "WRITE_MOVE"          , "III"             , TYPE_INT        , libmod_gfx_text_move            ),
    FUNC( "WRITE_MOVE"          , "IIII"            , TYPE_INT        , libmod_gfx_text_move2           ),
    FUNC( "WRITE_DELETE"        , "I"               , TYPE_INT        , libmod_gfx_text_delete          ),
    FUNC( "WRITE_IN_MAP"        , "ISI"             , TYPE_INT        , libmod_gfx_text_write_in_map    ),

    FUNC( "TEXT_WIDTH"          , "IS"              , TYPE_INT        , libmod_gfx_text_width           ),
    FUNC( "TEXT_HEIGHT"         , "IS"              , TYPE_INT        , libmod_gfx_text_height          ),

    /* Extended Write */
    FUNC( "WRITE_VAR"           , "IIIIV++"         , TYPE_INT        , libmod_gfx_text_write_var       ),
    FUNC( "WRITE_VAR"           , "IIIIIV++"        , TYPE_INT        , libmod_gfx_text_write_var2      ),
    FUNC( "WRITE_VALUE"         , "IIIIIP"          , TYPE_INT        , libmod_gfx_text_write_value     ),
    FUNC( "WRITE_VALUE"         , "IIIIIIP"         , TYPE_INT        , libmod_gfx_text_write_value2    ),

    FUNC( "WRITE_GET_RGBA"      , "IPPPP"           , TYPE_INT        , libmod_gfx_text_get_rgba        ),
    FUNC( "WRITE_SET_RGBA"      , "IBBBB"           , TYPE_INT        , libmod_gfx_text_set_rgba        ),

    /* Maps */
    FUNC( "MAP_BLOCK_COPY"      , "IIIIIIIIIII"     , TYPE_INT        , libmod_gfx_map_block_copy       ),
    FUNC( "MAP_BLOCK_COPY"      , "IIIIIIIIIIIBBBB" , TYPE_INT        , libmod_gfx_map_block_copy2      ),
    FUNC( "MAP_PUT"             , "IIIIII"          , TYPE_INT        , libmod_gfx_map_put              ),
    FUNC( "MAP_PUT"             , "IIIIIIIIIIBBBB"  , TYPE_INT        , libmod_gfx_map_put2             ),
    FUNC( "MAP_NEW"             , "II"              , TYPE_INT        , libmod_gfx_new_map              ),
    FUNC( "MAP_NEW"             , "III"             , TYPE_INT        , libmod_gfx_new_map_extend       ),
    FUNC( "MAP_CLEAR"           , "II"              , TYPE_INT        , libmod_gfx_map_clear            ),
    FUNC( "MAP_CLEAR"           , "III"             , TYPE_INT        , libmod_gfx_map_clear2           ),
    FUNC( "MAP_CLEAR"           , "IIIIII"          , TYPE_INT        , libmod_gfx_map_clear3           ),
    FUNC( "MAP_CLEAR"           , "IIIIIII"         , TYPE_INT        , libmod_gfx_map_clear4           ),
    FUNC( "MAP_CLONE"           , "II"              , TYPE_INT        , libmod_gfx_map_clone            ),
    FUNC( "MAP_NAME"            , "II"              , TYPE_STRING     , libmod_gfx_map_name             ),
    FUNC( "MAP_SET_NAME"        , "IIS"             , TYPE_INT        , libmod_gfx_map_set_name         ),
    FUNC( "MAP_EXISTS"          , "II"              , TYPE_INT        , libmod_gfx_map_exists           ),
    FUNC( "MAP_DEL"             , "II"              , TYPE_INT        , libmod_gfx_unload_map           ),
    FUNC( "MAP_UNLOAD"          , "II"              , TYPE_INT        , libmod_gfx_unload_map           ),
    FUNC( "MAP_LOAD"            , "S"               , TYPE_INT        , libmod_gfx_load_map             ),
    FUNC( "MAP_LOAD"            , "SP"              , TYPE_INT        , libmod_gfx_bgload_map           ),
    FUNC( "MAP_SAVE"            , "IIS"             , TYPE_INT        , libmod_gfx_map_save             ),
//    FUNC( "MAP_BUFFER"          , "II"              , TYPE_POINTER  , libmod_gfx_map_buffer           ),
    FUNC( "MAP_GET_PIXEL"       , "IIII"            , TYPE_INT        , libmod_gfx_map_get_pixel        ),
    FUNC( "MAP_PUT_PIXEL"       , "IIIII"           , TYPE_INT        , libmod_gfx_map_put_pixel        ),

    /* FPG */
    FUNC( "FPG_ADD"             , "IIII"            , TYPE_INT        , libmod_gfx_fpg_add              ),
    FUNC( "FPG_NEW"             , ""                , TYPE_INT        , libmod_gfx_fpg_new              ),
    FUNC( "FPG_EXISTS"          , "I"               , TYPE_INT        , libmod_gfx_fpg_exists           ),
    FUNC( "FPG_LOAD"            , "S"               , TYPE_INT        , libmod_gfx_load_fpg             ),
    FUNC( "FPG_LOAD"            , "SP"              , TYPE_INT        , libmod_gfx_bgload_fpg           ),
//    FUNC( "FPG_SAVE"            , "IS"              , TYPE_INT        , libmod_gfx_save_fpg             ),
    FUNC( "FPG_DEL"             , "I"               , TYPE_INT        , libmod_gfx_unload_fpg           ),
    FUNC( "FPG_UNLOAD"          , "I"               , TYPE_INT        , libmod_gfx_unload_fpg           ),

    /* Graphic information */
    FUNC( "MAP_INFO_SET"        , "IIII"            , TYPE_INT        , libmod_gfx_graphic_set          ),
    FUNC( "MAP_INFO"            , "IIII"            , TYPE_INT        , libmod_gfx_graphic_set          ),
    FUNC( "MAP_INFO"            , "III"             , TYPE_INT        , libmod_gfx_graphic_info         ),

    FUNC( "GRAPHIC_SET"         , "IIII"            , TYPE_INT        , libmod_gfx_graphic_set          ),
    FUNC( "GRAPHIC_INFO"        , "III"             , TYPE_INT        , libmod_gfx_graphic_info         ),

    /* Control points */
    FUNC( "POINT_GET"           , "IIIPP"           , TYPE_INT        , libmod_gfx_get_point            ),
    FUNC( "POINT_SET"           , "IIIII"           , TYPE_INT        , libmod_gfx_set_point            ),
    FUNC( "CENTER_SET"          , "IIII"            , TYPE_INT        , libmod_gfx_set_center           ),

    /* Fonts */
    FUNC( "FNT_NEW"             , ""                , TYPE_INT        , libmod_gfx_fnt_new              ),
    FUNC( "FNT_NEW"             , "I"               , TYPE_INT        , libmod_gfx_fnt_new_charset      ),
    FUNC( "FNT_NEW"             , "IIIIIIII"        , TYPE_INT        , libmod_gfx_fnt_new_from_bitmap  ),
    FUNC( "FNT_NEW"             , "IIIIIIIIII"      , TYPE_INT        , libmod_gfx_fnt_new_from_bitmap2 ),
    FUNC( "FNT_LOAD"            , "S"               , TYPE_INT        , libmod_gfx_load_fnt             ),
    FUNC( "FNT_LOAD"            , "SP"              , TYPE_INT        , libmod_gfx_bgload_fnt           ),
    FUNC( "FNT_UNLOAD"          , "I"               , TYPE_INT        , libmod_gfx_unload_fnt           ),
//    FUNC( "FNT_SAVE"            , "IS"              , TYPE_INT        , libmod_gfx_save_fnt             ),
//    FUNC( "BDF_LOAD"            , "S"               , TYPE_INT        , libmod_gfx_load_bdf             ),
//    FUNC( "BDF_LOAD"            , "SP"              , TYPE_INT        , libmod_gfx_bgload_bdf           ),
    FUNC( "GLYPH_GET"           , "II"              , TYPE_INT        , libmod_gfx_get_glyph            ),
    FUNC( "GLYPH_SET"           , "IIII"            , TYPE_INT        , libmod_gfx_set_glyph            ),

    /* Draw */

    FUNC( "DRAWING_COLOR"       , "I"               , TYPE_INT         , libmod_gfx_draw_drawing_color          ),
    FUNC( "DRAWING_COLOR"       , "II"              , TYPE_INT         , libmod_gfx_draw_drawing_color2         ),
    FUNC( "DRAWING_RGBA"        , "BBBB"            , TYPE_INT         , libmod_gfx_draw_drawing_rgba           ),
    FUNC( "DRAWING_RGBA"        , "IBBBB"           , TYPE_INT         , libmod_gfx_draw_drawing_rgba2          ),
    FUNC( "DRAWING_BLENDMODE"   , "I"               , TYPE_INT         , libmod_gfx_draw_drawing_blend_mode     ),
    FUNC( "DRAWING_BLENDMODE"   , "II"              , TYPE_INT         , libmod_gfx_draw_drawing_blend_mode2    ),

    FUNC( "DRAWING_Z"           , "I"               , TYPE_INT         , libmod_gfx_draw_drawing_z              ),
    FUNC( "DRAWING_Z"           , "II"              , TYPE_INT         , libmod_gfx_draw_drawing_z2             ),
    FUNC( "DRAWING_MAP"         , "II"              , TYPE_INT         , libmod_gfx_draw_drawing_map            ),

    FUNC( "DRAW_DELETE"         , "I"               , TYPE_INT         , libmod_gfx_draw_delete_drawing         ),

    FUNC( "DRAW_MOVE"           , "III"             , TYPE_INT         , libmod_gfx_draw_move_drawing           ),
    FUNC( "DRAW_COORDS"         , "IPPP"            , TYPE_INT         , libmod_gfx_draw_getcoords              ),

    FUNC( "DRAW_POINT"          , "II"              , TYPE_INT         , libmod_gfx_draw_point                  ),
    FUNC( "DRAW_POINT"          , "III"             , TYPE_INT         , DRWFN_COLOR(point)                     ), // libmod_gfx_draw_point_color

    FUNC( "DRAW_LINE"           , "IIII"            , TYPE_INT         , libmod_gfx_draw_line                   ),
    FUNC( "DRAW_LINE"           , "IIIII"           , TYPE_INT         , DRWFN_COLOR(line)                      ), // libmod_gfx_draw_line_color

    FUNC( "DRAW_RECTANGLE"      , "IIII"            , TYPE_INT         , libmod_gfx_draw_rectangle              ),
    FUNC( "DRAW_RECTANGLE"      , "IIIII"           , TYPE_INT         , DRWFN_COLOR(rectangle)                 ), // libmod_gfx_draw_rectangle_color

    FUNC( "DRAW_RECTANGLE_FILLED", "IIII"           , TYPE_INT         , libmod_gfx_draw_rectangle_filled       ),
    FUNC( "DRAW_RECTANGLE_FILLED", "IIIII"          , TYPE_INT         , DRWFN_COLOR(rectangle_filled)          ), // libmod_gfx_draw_rectangle_filled_color

#if ENABLE_MULTIDRAW
    FUNC( "DRAW_POINTS"         , "IP"              , TYPE_INT         , libmod_gfx_draw_points                 ),
    FUNC( "DRAW_POINTS"         , "IPI"             , TYPE_INT         , DRWFN_COLOR(points)                    ), // libmod_gfx_draw_points_color

    FUNC( "DRAW_LINES"          , "IP"              , TYPE_INT         , libmod_gfx_draw_lines                  ),
    FUNC( "DRAW_LINES"          , "IPI"             , TYPE_INT         , DRWFN_COLOR(lines)                     ), // libmod_gfx_draw_lines_color

    FUNC( "DRAW_RECTANGLES"     , "IP"              , TYPE_INT         , libmod_gfx_draw_rectangles             ),
    FUNC( "DRAW_RECTANGLES"     , "IPI"             , TYPE_INT         , DRWFN_COLOR(rectangles)                ), // libmod_gfx_draw_rectangles_color

    FUNC( "DRAW_RECTANGLES_FILLED", "IP"            , TYPE_INT         , libmod_gfx_draw_rectangles_filled      ),
    FUNC( "DRAW_RECTANGLES_FILLED", "IPI"           , TYPE_INT         , DRWFN_COLOR(rectangles_filled)         ), // libmod_gfx_draw_rectangles_filled_color
#endif

    FUNC( "DRAW_CIRCLE"         , "III"             , TYPE_INT         , libmod_gfx_draw_circle                 ),
    FUNC( "DRAW_CIRCLE"         , "IIII"            , TYPE_INT         , DRWFN_COLOR(circle)                    ), // libmod_gfx_draw_circle_color
    FUNC( "DRAW_CIRCLE_FILLED"  , "III"             , TYPE_INT         , libmod_gfx_draw_circle_filled          ),
    FUNC( "DRAW_CIRCLE_FILLED"  , "IIII"            , TYPE_INT         , DRWFN_COLOR(circle_filled)             ), // libmod_gfx_draw_circle_filled_color

    FUNC( "DRAW_CURVE"          , "IIIIIIIII"       , TYPE_INT         , libmod_gfx_draw_bezier                 ),
    FUNC( "DRAW_CURVE"          , "IIIIIIIIII"      , TYPE_INT         , DRWFN_COLOR(bezier)                    ), // libmod_gfx_draw_bezier_color

#ifdef USE_SDL2_GPU
    FUNC( "DRAW_ARC"            , "IIIII"           , TYPE_INT         , libmod_gfx_draw_arc                    ),
    FUNC( "DRAW_ARC"            , "IIIIII"          , TYPE_INT         , DRWFN_COLOR(arc)                       ), // libmod_gfx_draw_arc_color
    FUNC( "DRAW_ARC_FILLED"     , "IIIII"           , TYPE_INT         , libmod_gfx_draw_arc_filled             ),
    FUNC( "DRAW_ARC_FILLED"     , "IIIIII"          , TYPE_INT         , DRWFN_COLOR(arc_filled)                ), // libmod_gfx_draw_arc_filled_color

    FUNC( "DRAW_ELLIPSE"        , "IIIII"           , TYPE_INT         , libmod_gfx_draw_ellipse                ),
    FUNC( "DRAW_ELLIPSE"        , "IIIIII"          , TYPE_INT         , DRWFN_COLOR(ellipse)                   ), // libmod_gfx_draw_ellipse_color
    FUNC( "DRAW_ELLIPSE_FILLED" , "IIIII"           , TYPE_INT         , libmod_gfx_draw_ellipse_filled         ),
    FUNC( "DRAW_ELLIPSE_FILLED" , "IIIIII"          , TYPE_INT         , DRWFN_COLOR(ellipse_filled)            ), // libmod_gfx_draw_ellipse_filled_color

    FUNC( "DRAW_SECTOR"         , "IIIIII"          , TYPE_INT         , libmod_gfx_draw_sector                 ),
    FUNC( "DRAW_SECTOR"         , "IIIIIII"         , TYPE_INT         , DRWFN_COLOR(sector)                    ), // libmod_gfx_draw_sector_color
    FUNC( "DRAW_SECTOR_FILLED"  , "IIIIII"          , TYPE_INT         , libmod_gfx_draw_sector_filled          ),
    FUNC( "DRAW_SECTOR_FILLED"  , "IIIIIII"         , TYPE_INT         , DRWFN_COLOR(sector_filled)             ), // libmod_gfx_draw_sector_filled_color

    FUNC( "DRAW_TRIANGLE"       , "IIIIII"          , TYPE_INT         , libmod_gfx_draw_triangle               ),
    FUNC( "DRAW_TRIANGLE"       , "IIIIIII"         , TYPE_INT         , DRWFN_COLOR(triangle)                  ), // libmod_gfx_draw_triangle_color
    FUNC( "DRAW_TRIANGLE_FILLED", "IIIIII"          , TYPE_INT         , libmod_gfx_draw_triangle_filled        ),
    FUNC( "DRAW_TRIANGLE_FILLED", "IIIIIII"         , TYPE_INT         , DRWFN_COLOR(triangle_filled)           ), // libmod_gfx_draw_triangle_filled_color

    FUNC( "DRAW_RECTANGLE_ROUND"       , "IIIII"    , TYPE_INT         , libmod_gfx_draw_rectangle_round        ),
    FUNC( "DRAW_RECTANGLE_ROUND"       , "IIIIII"   , TYPE_INT         , DRWFN_COLOR(rectangle_round)           ), // libmod_gfx_draw_rectangle_round_color
    FUNC( "DRAW_RECTANGLE_ROUND_FILLED", "IIIII"    , TYPE_INT         , libmod_gfx_draw_rectangle_round_filled ),
    FUNC( "DRAW_RECTANGLE_ROUND_FILLED", "IIIIII"   , TYPE_INT         , DRWFN_COLOR(rectangle_round_filled)    ), // libmod_gfx_draw_rectangle_round_filled_color

    FUNC( "DRAW_POLYGON"        , "IP"              , TYPE_INT         , libmod_gfx_draw_polygon                ),
    FUNC( "DRAW_POLYGON"        , "IPI"             , TYPE_INT         , DRWFN_COLOR(polygon)                   ), // libmod_gfx_draw_polygon_color
    FUNC( "DRAW_POLYGON_FILLED" , "IP"              , TYPE_INT         , libmod_gfx_draw_polygon_filled         ),
    FUNC( "DRAW_POLYGON_FILLED" , "IPI"             , TYPE_INT         , DRWFN_COLOR(polygon_filled)            ), // libmod_gfx_draw_polygon_filled_color

    FUNC( "DRAW_POLYLINE"       , "IPI"             , TYPE_INT         , libmod_gfx_draw_polyline               ),
    FUNC( "DRAW_POLYLINE"       , "IPII"            , TYPE_INT         , DRWFN_COLOR(polyline)                  ), // libmod_gfx_draw_ellipse_color

    FUNC( "DRAW_SET_THICKNESS"  , "F"               , TYPE_FLOAT       , libmod_gfx_draw_set_thickness          ),
    FUNC( "DRAW_SET_THICKNESS"  , "IF"              , TYPE_FLOAT       , libmod_gfx_draw_set_thickness2         ),
    FUNC( "DRAW_GET_THICKNESS"  , ""                , TYPE_FLOAT       , libmod_gfx_draw_get_thickness          ),
    FUNC( "DRAW_GET_THICKNESS"  , "I"               , TYPE_FLOAT       , libmod_gfx_draw_get_thickness2         ),

#endif

    /* pathfind */
    FUNC( "PATH_NEW"            , "II"              , TYPE_POINTER    , libmod_gfx_path_new                     ),
    FUNC( "PATH_DESTROY"        , "P"               , TYPE_INT        , libmod_gfx_path_destroy                 ),
    FUNC( "PATH_FIND"           , "PIIIII"          , TYPE_POINTER    , libmod_gfx_path_find                    ),
    FUNC( "PATH_FIND"           , "PIIIIIII"        , TYPE_POINTER    , libmod_gfx_path_find2                   ),
    FUNC( "PATH_FREE_RESULTS"   , "P"               , TYPE_INT        , libmod_gfx_path_free_results            ),

    FUNC( "TEXTURE_SET_QUALITY" , "I"               , TYPE_INT        , libmod_gfx_set_texture_quality          ),

    FUNC( "MAP_SET_PALETTE"     , "IIIIP"           , TYPE_INT        , libmod_gfx_set_palette                  ),
    FUNC( "MAP_GET_PALETTE"     , "IIIIP"           , TYPE_INT        , libmod_gfx_get_palette                  ),


    FUNC( NULL                  , NULL              , 0             , NULL                                      )
};

/* --------------------------------------------------------------------------- */

char * __bgdexport( libmod_gfx, modules_dependency )[] = {
    "libbggfx",
    "libbginput",
    NULL
};

/* --------------------------------------------------------------------------- */

#endif
