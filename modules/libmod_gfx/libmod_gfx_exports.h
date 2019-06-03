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
 *, 0
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
    { "G_WIDE"              , TYPE_QWORD      , G_WIDE                },    /* Obsolete */
    { "G_WIDTH"             , TYPE_QWORD      , G_WIDTH               },
    { "G_HEIGHT"            , TYPE_QWORD      , G_HEIGHT              },
    { "G_CENTER_X"          , TYPE_QWORD      , G_CENTER_X            },
    { "G_X_CENTER"          , TYPE_QWORD      , G_X_CENTER            },
    { "G_CENTER_Y"          , TYPE_QWORD      , G_CENTER_Y            },
    { "G_Y_CENTER"          , TYPE_QWORD      , G_Y_CENTER            },
    { "G_PITCH"             , TYPE_QWORD      , G_PITCH               },
    { "G_DEPTH"             , TYPE_QWORD      , G_DEPTH               },

    { "B_CLEAR"             , TYPE_QWORD      , B_CLEAR               },

    { "CHARSET_ISO8859"     , TYPE_QWORD      , CHARSET_ISO8859       },
    { "CHARSET_CP850"       , TYPE_QWORD      , CHARSET_CP850         },

    { "NFB_VARIABLEWIDTH"   , TYPE_QWORD      , 0                     },
    { "NFB_FIXEDWIDTH"      , TYPE_QWORD      , NFB_FIXEDWIDTH        },

    /* Text */
    { "ALL_TEXT"            , TYPE_QWORD      , 0                     },

    /* Texts align */
    { "ALIGN_TOP_LEFT"      , TYPE_QWORD      , ALIGN_TOP_LEFT        },
    { "ALIGN_TOP"           , TYPE_QWORD      , ALIGN_TOP             },
    { "ALIGN_TOP_RIGHT"     , TYPE_QWORD      , ALIGN_TOP_RIGHT       },
    { "ALIGN_CENTER_LEFT"   , TYPE_QWORD      , ALIGN_CENTER_LEFT     },
    { "ALIGN_CENTER"        , TYPE_QWORD      , ALIGN_CENTER          },
    { "ALIGN_CENTER_RIGHT"  , TYPE_QWORD      , ALIGN_CENTER_RIGHT    },
    { "ALIGN_BOTTOM_LEFT"   , TYPE_QWORD      , ALIGN_BOTTOM_LEFT     },
    { "ALIGN_BOTTOM"        , TYPE_QWORD      , ALIGN_BOTTOM          },
    { "ALIGN_BOTTOM_RIGHT"  , TYPE_QWORD      , ALIGN_BOTTOM_RIGHT    },

    /* Pathfind */
    { "PF_NODIAG"           , TYPE_QWORD      , PF_NODIAG             }, /* Prohibit the pathfinding from using diagonal paths. */
    { "PF_REVERSE"          , TYPE_QWORD      , PF_REVERSE            }, /* Return the path found in reverse order.             */

    { NULL                  , 0               , 0                     }
} ;

#endif

/* --------------------------------------------------------------------------- */

char * __bgdexport( libmod_gfx, locals_def ) =
    /* f_gproc */
    "STRUCT __collision_reserved\n"
        "INT type_scan;\n"
        "INT id_scan;\n"
        "INT context;\n"
    "END\n"
    ;


char * __bgdexport( libmod_gfx, types_def ) =
    /* m_draw */
    "TYPE GFX_POINT\n"
        "INT32 x;\n"
        "INT32 y;\n"
    "END\n"

    "TYPE GFX_RECT\n"
        "INT32 x;\n"
        "INT32 y;\n"
        "INT32 w;\n"
        "INT32 h;\n"
    "END\n"
    ;

/* ---------------------------------------------------------------------------" */

DLSYSFUNCS  __bgdexport( libmod_gfx, functions_exports )[] = {

    /* f_grproc */
    FUNC( "ADVANCE"             , "I"               , TYPE_QWORD      , libmod_gfx_advance              ),
    FUNC( "XADVANCE"            , "II"              , TYPE_QWORD      , libmod_gfx_xadvance             ),

    FUNC( "GET_ANGLE"           , "I"               , TYPE_QWORD      , libmod_gfx_get_angle            ),
    FUNC( "GET_DIST"            , "I"               , TYPE_QWORD      , libmod_gfx_get_dist             ),

    FUNC( "GET_REAL_POINT"      , "IPP"             , TYPE_QWORD      , libmod_gfx_get_real_point       ),

    FUNC( "COLLISION"           , "I"               , TYPE_QWORD      , libmod_gfx_collision            ),
    FUNC( "COLLISION_BOX"       , "I"               , TYPE_QWORD      , libmod_gfx_collision_box        ),
    FUNC( "COLLISION_CIRCLE"    , "I"               , TYPE_QWORD      , libmod_gfx_collision_circle     ),

    /* scroll */
    FUNC( "SCROLL_START"        , "IIIIIIIII"       , TYPE_QWORD      , libmod_gfx_scroll_start2        ),
    FUNC( "SCROLL_START"        , "IIIIIII"         , TYPE_QWORD      , libmod_gfx_scroll_start         ),
    FUNC( "SCROLL_STOP"         , "I"               , TYPE_QWORD      , libmod_gfx_scroll_stop          ),
    FUNC( "SCROLL_MOVE"         , "I"               , TYPE_QWORD      , libmod_gfx_scroll_move          ),

    /* Regiones */
    FUNC( "REGION_DEFINE"       , "IIIII"           , TYPE_QWORD      , libmod_gfx_define_region        ),
    FUNC( "REGION_OUT"          , "II"              , TYPE_QWORD      , libmod_gfx_out_region           ),

    /* Video */
    FUNC( "SCREEN_GET"          , ""                , TYPE_QWORD      , libmod_gfx_get_screen           ),

    FUNC( "RGB"                 , "BBB"             , TYPE_QWORD      , libmod_gfx_rgb                  ),
    FUNC( "RGBA"                , "BBBB"            , TYPE_QWORD      , libmod_gfx_rgba                 ),

    FUNC( "RGB_GET"             , "IPPP"            , TYPE_QWORD      , libmod_gfx_get_rgb              ),
    FUNC( "RGBA_GET"            , "IPPPP"           , TYPE_QWORD      , libmod_gfx_get_rgba             ),

    FUNC( "FADE"                , "II"              , TYPE_QWORD      , libmod_gfx_fade                 ),
    FUNC( "FADE_ON"             , "I"               , TYPE_QWORD      , libmod_gfx_fade_on              ),
    FUNC( "FADE_OFF"            , "I"               , TYPE_QWORD      , libmod_gfx_fade_off             ),

    /* Video */
    FUNC( "SET_MODE"            , "I"               , TYPE_QWORD      , libmod_gfx_set_mode             ),
    FUNC( "SET_MODE"            , "II"              , TYPE_QWORD      , libmod_gfx_set_mode_2           ),
    FUNC( "SET_MODE"            , "III"             , TYPE_QWORD      , libmod_gfx_set_mode_3           ),
    FUNC( "SET_MODE"            , "IIII"            , TYPE_QWORD      , libmod_gfx_set_mode_4           ),
    FUNC( "SET_FPS"             , "II"              , TYPE_QWORD      , libmod_gfx_set_fps              ),

//    FUNC( "GET_MODES"           , "II"              , TYPE_POINTER  , libmod_gfx_list_modes           ),
//    FUNC( "MODE_IS_OK"          , "IIII"            , TYPE_QWORD      , libmod_gfx_mode_is_ok           ),

    FUNC( "WINDOW_SET_TITLE"    , "S"               , TYPE_QWORD      , libmod_gfx_set_title            ),
    FUNC( "WINDOW_SET_ICON"     , "II"              , TYPE_QWORD      , libmod_gfx_set_icon             ),
    FUNC( "WINDOW_MOVE"         , "II"              , TYPE_QWORD      , libmod_gfx_move_window          ),
    FUNC( "WINDOW_SET_POS"      , "II"              , TYPE_QWORD      , libmod_gfx_move_window          ),
    FUNC( "WINDOW_GET_POS"      , "PP"              , TYPE_QWORD      , libmod_gfx_get_window_pos       ),
    FUNC( "WINDOW_GET_SIZE"     , "PPPPPP"          , TYPE_QWORD      , libmod_gfx_get_window_size      ),
    FUNC( "DESKTOP_GET_SIZE"    , "PP"              , TYPE_QWORD      , libmod_gfx_get_desktop_size     ),

    FUNC( "WINDOW_HIDE"         , ""                , TYPE_QWORD      , libmod_gfx_hide_window          ),
    FUNC( "WINDOW_SHOW"         , ""                , TYPE_QWORD      , libmod_gfx_show_window          ),
    FUNC( "WINDOW_MINIMIZE"     , ""                , TYPE_QWORD      , libmod_gfx_minimize             ),
    FUNC( "WINDOW_MAXIMIZE"     , ""                , TYPE_QWORD      , libmod_gfx_maximize             ),
    FUNC( "WINDOW_RAISE"        , ""                , TYPE_QWORD      , libmod_gfx_raise_window         ),
    FUNC( "WINDOW_RESTORE"      , ""                , TYPE_QWORD      , libmod_gfx_restore_window       ),

    /* Texts */
    FUNC( "WRITE"               , "IIIIS"           , TYPE_QWORD      , libmod_gfx_text_write           ),
    FUNC( "WRITE"               , "IIIIIS"          , TYPE_QWORD      , libmod_gfx_text_write2          ),
    FUNC( "WRITE_INT"           , "IIIIP"           , TYPE_QWORD      , libmod_gfx_text_write_int       ),
    FUNC( "WRITE_INT"           , "IIIIIP"          , TYPE_QWORD      , libmod_gfx_text_write_int2      ),
    FUNC( "WRITE_MOVE"          , "III"             , TYPE_QWORD      , libmod_gfx_text_move            ),
    FUNC( "WRITE_MOVE"          , "IIII"            , TYPE_QWORD      , libmod_gfx_text_move2           ),
    FUNC( "WRITE_DELETE"        , "I"               , TYPE_QWORD      , libmod_gfx_text_delete          ),
    FUNC( "WRITE_IN_MAP"        , "ISI"             , TYPE_QWORD      , libmod_gfx_text_write_in_map    ),

    FUNC( "TEXT_WIDTH"          , "IS"              , TYPE_QWORD      , libmod_gfx_text_width           ),
    FUNC( "TEXT_HEIGHT"         , "IS"              , TYPE_QWORD      , libmod_gfx_text_height          ),

    /* Extended Write */
    FUNC( "WRITE_VAR"           , "IIIIV++"         , TYPE_QWORD      , libmod_gfx_text_write_var       ),
    FUNC( "WRITE_VAR"           , "IIIIIV++"        , TYPE_QWORD      , libmod_gfx_text_write_var2      ),
    FUNC( "WRITE_FLOAT"         , "IIIIP"           , TYPE_QWORD      , libmod_gfx_text_write_float     ),
    FUNC( "WRITE_FLOAT"         , "IIIIIP"          , TYPE_QWORD      , libmod_gfx_text_write_float2    ),
    FUNC( "WRITE_STRING"        , "IIIIP"           , TYPE_QWORD      , libmod_gfx_text_write_string    ),
    FUNC( "WRITE_STRING"        , "IIIIIP"          , TYPE_QWORD      , libmod_gfx_text_write_string2   ),

    FUNC( "WRITE_GET_COLOR"     , "IPPP"            , TYPE_QWORD      , libmod_gfx_text_get_color       ),
    FUNC( "WRITE_SET_COLOR"     , "IBBB"            , TYPE_QWORD      , libmod_gfx_text_set_color       ),
    FUNC( "WRITE_GET_ALPHA"     , "IP"              , TYPE_QWORD      , libmod_gfx_text_get_alpha       ),
    FUNC( "WRITE_SET_ALPHA"     , "IB"              , TYPE_QWORD      , libmod_gfx_text_set_alpha       ),

    /* Maps */
    FUNC( "MAP_BLOCK_COPY"      , "IIIIIIIIII"      , TYPE_QWORD      , libmod_gfx_map_block_copy       ),
    FUNC( "MAP_PUT"             , "IIIIII"          , TYPE_QWORD      , libmod_gfx_map_put              ),
    FUNC( "MAP_XPUT"            , "IIIIIIIIIIBBBB"  , TYPE_QWORD      , libmod_gfx_map_xput             ),
    FUNC( "MAP_NEW"             , "II"              , TYPE_QWORD      , libmod_gfx_new_map              ),
    FUNC( "MAP_NEW"             , "III"             , TYPE_QWORD      , libmod_gfx_new_map_extend       ),
    FUNC( "MAP_CLEAR"           , "II"              , TYPE_QWORD      , libmod_gfx_map_clear            ),
    FUNC( "MAP_CLEAR"           , "III"             , TYPE_QWORD      , libmod_gfx_map_clear2           ),
    FUNC( "MAP_CLEAR"           , "IIIIII"          , TYPE_QWORD      , libmod_gfx_map_clear3           ),
    FUNC( "MAP_CLEAR"           , "IIIIIII"         , TYPE_QWORD      , libmod_gfx_map_clear4           ),
    FUNC( "MAP_CLONE"           , "II"              , TYPE_QWORD      , libmod_gfx_map_clone            ),
    FUNC( "MAP_NAME"            , "II"              , TYPE_STRING   , libmod_gfx_map_name             ),
    FUNC( "MAP_SET_NAME"        , "IIS"             , TYPE_QWORD      , libmod_gfx_map_set_name         ),
    FUNC( "MAP_EXISTS"          , "II"              , TYPE_QWORD      , libmod_gfx_map_exists           ),
    FUNC( "MAP_DEL"             , "II"              , TYPE_QWORD      , libmod_gfx_unload_map           ),
    FUNC( "MAP_UNLOAD"          , "II"              , TYPE_QWORD      , libmod_gfx_unload_map           ),
    FUNC( "MAP_LOAD"            , "S"               , TYPE_QWORD      , libmod_gfx_load_map             ),
    FUNC( "MAP_LOAD"            , "SP"              , TYPE_QWORD      , libmod_gfx_bgload_map           ),
//    FUNC( "MAP_SAVE"            , "IIS"             , TYPE_QWORD      , libmod_gfx_save_map             ),
//    FUNC( "MAP_BUFFER"          , "II"              , TYPE_POINTER  , libmod_gfx_map_buffer           ),
    FUNC( "MAP_GET_PIXEL"       , "IIII"            , TYPE_QWORD      , libmod_gfx_map_get_pixel        ),
    FUNC( "MAP_PUT_PIXEL"       , "IIIII"           , TYPE_QWORD      , libmod_gfx_map_put_pixel        ),

    /* FPG */
    FUNC( "FPG_ADD"             , "IIII"            , TYPE_QWORD      , libmod_gfx_fpg_add              ),
    FUNC( "FPG_NEW"             , ""                , TYPE_QWORD      , libmod_gfx_fpg_new              ),
    FUNC( "FPG_EXISTS"          , "I"               , TYPE_QWORD      , libmod_gfx_fpg_exists           ),
    FUNC( "FPG_LOAD"            , "S"               , TYPE_QWORD      , libmod_gfx_load_fpg             ),
    FUNC( "FPG_LOAD"            , "SP"              , TYPE_QWORD      , libmod_gfx_bgload_fpg           ),
//    FUNC( "FPG_SAVE"            , "IS"              , TYPE_QWORD      , libmod_gfx_save_fpg             ),
    FUNC( "FPG_DEL"             , "I"               , TYPE_QWORD      , libmod_gfx_unload_fpg           ),
    FUNC( "FPG_UNLOAD"          , "I"               , TYPE_QWORD      , libmod_gfx_unload_fpg           ),

    /* Graphic information */
    FUNC( "MAP_INFO_SET"        , "IIII"            , TYPE_QWORD      , libmod_gfx_graphic_set          ),
    FUNC( "MAP_INFO_GET"        , "III"             , TYPE_QWORD      , libmod_gfx_graphic_info         ),
    FUNC( "MAP_INFO"            , "III"             , TYPE_QWORD      , libmod_gfx_graphic_info         ),

    FUNC( "GRAPHIC_SET"         , "IIII"            , TYPE_QWORD      , libmod_gfx_graphic_set          ),
    FUNC( "GRAPHIC_INFO"        , "III"             , TYPE_QWORD      , libmod_gfx_graphic_info         ),

    /* Control points */
    FUNC( "POINT_GET"           , "IIIPP"           , TYPE_QWORD      , libmod_gfx_get_point            ),
    FUNC( "POINT_SET"           , "IIIII"           , TYPE_QWORD      , libmod_gfx_set_point            ),
    FUNC( "CENTER_SET"          , "IIII"            , TYPE_QWORD      , libmod_gfx_set_center           ),

    /* Fonts */
    FUNC( "FNT_NEW"             , ""                , TYPE_QWORD      , libmod_gfx_fnt_new              ),
    FUNC( "FNT_NEW"             , "I"               , TYPE_QWORD      , libmod_gfx_fnt_new_charset      ),
    FUNC( "FNT_NEW"             , "IIIIIIII"        , TYPE_QWORD      , libmod_gfx_fnt_new_from_bitmap  ),
    FUNC( "FNT_LOAD"            , "S"               , TYPE_QWORD      , libmod_gfx_load_fnt             ),
    FUNC( "FNT_LOAD"            , "SP"              , TYPE_QWORD      , libmod_gfx_bgload_fnt           ),
    FUNC( "FNT_UNLOAD"          , "I"               , TYPE_QWORD      , libmod_gfx_unload_fnt           ),
//    FUNC( "FNT_SAVE"            , "IS"              , TYPE_QWORD      , libmod_gfx_save_fnt             ),
//    FUNC( "BDF_LOAD"            , "S"               , TYPE_QWORD      , libmod_gfx_load_bdf             ),
//    FUNC( "BDF_LOAD"            , "SP"              , TYPE_QWORD      , libmod_gfx_bgload_bdf           ),
    FUNC( "GLYPH_GET"           , "II"              , TYPE_QWORD      , libmod_gfx_get_glyph            ),
    FUNC( "GLYPH_SET"           , "IIII"            , TYPE_QWORD      , libmod_gfx_set_glyph            ),

    /* Draw */

    FUNC( "DRAWING_COLOR"       , "I"               , TYPE_QWORD       , libmod_gfx_draw_drawing_color          ),
    FUNC( "DRAWING_COLOR"       , "II"              , TYPE_QWORD       , libmod_gfx_draw_drawing_color_id       ),
    FUNC( "DRAWING_RGBA"        , "BBBB"            , TYPE_QWORD       , libmod_gfx_draw_drawing_rgba           ),
    FUNC( "DRAWING_RGBA"        , "IBBBB"           , TYPE_QWORD       , libmod_gfx_draw_drawing_rgba_id        ),
    FUNC( "DRAWING_BLENDMODE"   , "I"               , TYPE_QWORD       , libmod_gfx_draw_drawing_blend_mode     ),
    FUNC( "DRAWING_BLENDMODE"   , "II"              , TYPE_QWORD       , libmod_gfx_draw_drawing_blend_mode_id  ),
    FUNC( "DRAWING_Z"           , "I"               , TYPE_QWORD       , libmod_gfx_draw_drawing_z              ),
    FUNC( "DRAWING_Z"           , "II"              , TYPE_QWORD       , libmod_gfx_draw_drawing_z_id           ),
    FUNC( "DRAWING_MAP"         , "II"              , TYPE_QWORD       , libmod_gfx_draw_drawing_map            ),
    FUNC( "DELETE_DRAW"         , "I"               , TYPE_QWORD       , libmod_gfx_draw_delete_drawing         ),
    FUNC( "MOVE_DRAW"           , "III"             , TYPE_QWORD       , libmod_gfx_draw_move_drawing           ),
    FUNC( "DRAW_DELETE"         , "I"               , TYPE_QWORD       , libmod_gfx_draw_delete_drawing         ),
    FUNC( "DRAW_MOVE"           , "III"             , TYPE_QWORD       , libmod_gfx_draw_move_drawing           ),
    FUNC( "DRAW_POINT"          , "II"              , TYPE_QWORD       , libmod_gfx_draw_point                  ),
    FUNC( "DRAW_POINTS"         , "IP"              , TYPE_QWORD       , libmod_gfx_draw_points                 ),
    FUNC( "DRAW_LINE"           , "IIII"            , TYPE_QWORD       , libmod_gfx_draw_line                   ),
    FUNC( "DRAW_LINES"          , "IP"              , TYPE_QWORD       , libmod_gfx_draw_lines                  ),
    FUNC( "DRAW_BOX"            , "IIII"            , TYPE_QWORD       , libmod_gfx_draw_box                    ),
    FUNC( "DRAW_BOXES"          , "IP"              , TYPE_QWORD       , libmod_gfx_draw_boxes                  ),
    FUNC( "DRAW_RECT"           , "IIII"            , TYPE_QWORD       , libmod_gfx_draw_rect                   ),
    FUNC( "DRAW_RECTS"          , "IP"              , TYPE_QWORD       , libmod_gfx_draw_rects                  ),
    FUNC( "DRAW_CIRCLE"         , "III"             , TYPE_QWORD       , libmod_gfx_draw_circle                 ),
    FUNC( "DRAW_FCIRCLE"        , "III"             , TYPE_QWORD       , libmod_gfx_draw_fcircle                ),
    FUNC( "DRAW_CURVE"          , "IIIIIIIII"       , TYPE_QWORD       , libmod_gfx_draw_bezier                 ),

#if 0
    /* pathfind */
    FUNC( "PATH_FIND"           , "IIIIIII"         , TYPE_QWORD      , libmod_gfx_path_find            ),
    FUNC( "PATH_GETXY"          , "PP"              , TYPE_QWORD      , libmod_gfx_path_getxy           ),
    FUNC( "PATH_WALL"           , "I"               , TYPE_QWORD      , libmod_gfx_path_wall            ),
#endif
    FUNC( NULL                  , NULL              , 0             , NULL                              )
};

/* --------------------------------------------------------------------------- */

char * __bgdexport( libmod_gfx, modules_dependency )[] = {
    "libbggfx",
    "libbginput",
    NULL
};

/* --------------------------------------------------------------------------- */

#endif
