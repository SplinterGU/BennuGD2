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
#else
    { "Q_MIPMAP"            , TYPE_INT          , -1                                    },
#endif
    { "CHARSET_ISO8859"     , TYPE_INT          , CHARSET_ISO8859                       },
    { "CHARSET_CP850"       , TYPE_INT          , CHARSET_CP850                         },

    { "NFB_VARIABLEWIDTH"   , TYPE_INT          , NFB_VARIABLEWIDTH                     },
    { "NFB_FIXEDWIDTH"      , TYPE_INT          , NFB_FIXEDWIDTH                        },
    { "NFB_FIXEDWIDTHCENTER", TYPE_INT          , NFB_FIXEDWIDTHCENTER                  },

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

    /* MEDIA */

    { "MEDIA_STATUS_ERROR"      , TYPE_INT      , MEDIA_STATUS_ERROR                    },
    { "MEDIA_STATUS_PLAYING"    , TYPE_INT      , MEDIA_STATUS_PLAYING                  },
    { "MEDIA_STATUS_PAUSED"     , TYPE_INT      , MEDIA_STATUS_PAUSED                   },
    { "MEDIA_STATUS_STOPPED"    , TYPE_INT      , MEDIA_STATUS_STOPPED                  },
    { "MEDIA_STATUS_ENDED"      , TYPE_INT      , MEDIA_STATUS_ENDED                    },
 
    { "SHADER_IMAGE"            , TYPE_INT      , SHADER_IMAGE                          },
#if 0
    { "ATTRIBUTE_INT"           , TYPE_INT      , ATTRIBUTE_INT                         },
    { "ATTRIBUTE_INT_ARRAY"     , TYPE_INT      , ATTRIBUTE_INT_ARRAY                   },
    { "ATTRIBUTE_UINT"          , TYPE_INT      , ATTRIBUTE_UINT                        },
    { "ATTRIBUTE_UINT_ARRAY"    , TYPE_INT      , ATTRIBUTE_UINT_ARRAY                  },
    { "ATTRIBUTE_FLOAT"         , TYPE_INT      , ATTRIBUTE_FLOAT                       },
    { "ATTRIBUTE_FLOAT_ARRAY"   , TYPE_INT      , ATTRIBUTE_FLOAT_ARRAY                 },
#endif
    { "UNIFORM_INT"             , TYPE_INT      , UNIFORM_INT                           },
    { "UNIFORM_INT_ARRAY"       , TYPE_INT      , UNIFORM_INT_ARRAY                     },
    { "UNIFORM_INT2_ARRAY"      , TYPE_INT      , UNIFORM_INT2_ARRAY                    },
    { "UNIFORM_INT3_ARRAY"      , TYPE_INT      , UNIFORM_INT3_ARRAY                    },
    { "UNIFORM_INT4_ARRAY"      , TYPE_INT      , UNIFORM_INT4_ARRAY                    },
    { "UNIFORM_UINT"            , TYPE_INT      , UNIFORM_UINT                          },
    { "UNIFORM_UINT_ARRAY"      , TYPE_INT      , UNIFORM_UINT_ARRAY                    },
    { "UNIFORM_UINT2_ARRAY"     , TYPE_INT      , UNIFORM_UINT2_ARRAY                   },
    { "UNIFORM_UINT3_ARRAY"     , TYPE_INT      , UNIFORM_UINT3_ARRAY                   },
    { "UNIFORM_UINT4_ARRAY"     , TYPE_INT      , UNIFORM_UINT4_ARRAY                   },
    { "UNIFORM_FLOAT"           , TYPE_INT      , UNIFORM_FLOAT                         },
    { "UNIFORM_FLOAT_ARRAY"     , TYPE_INT      , UNIFORM_FLOAT_ARRAY                   },
    { "UNIFORM_FLOAT2_ARRAY"    , TYPE_INT      , UNIFORM_FLOAT2_ARRAY                  },
    { "UNIFORM_FLOAT3_ARRAY"    , TYPE_INT      , UNIFORM_FLOAT3_ARRAY                  },
    { "UNIFORM_FLOAT4_ARRAY"    , TYPE_INT      , UNIFORM_FLOAT4_ARRAY                  },
    { "UNIFORM_MATRIX"          , TYPE_INT      , UNIFORM_MATRIX                        },

    { NULL                  , 0                 , 0                                     }
} ;

/* --------------------------------------------------------------------------- */

char * __bgdexport( libmod_gfx, locals_def ) =
    /* f_gproc */
    "STRUCT __collision_reserved\n"
    "   INT mode = -1;\n"
    "   UINT type_scan;\n"
    "   UINT id_scan;\n"
    "   UINT * context = NULL;\n"
    "   UINT id_scroll;\n"
    "   UINT idx_cboxA;\n"
    "   UINT idx_cboxB;\n"
    "END\n"

    "INT cshape = SHAPE_BOX;\n"
    "G_RECT cbox = POINT_UNDEFINED, POINT_UNDEFINED, GRAPH_SIZE, GRAPH_SIZE;\n"
    "STRUCT collision_info\n"
    "   INT collider_cbox;\n"
    "   INT collided_id;\n"
    "   INT collided_cbox;\n"
    "   INT penetration_x;\n"
    "   INT penetration_y;\n"
    "END\n"
    ;

#endif

/* --------------------------------------------------------------------------- */

DLSYSFUNCS  __bgdexport( libmod_gfx, functions_exports )[] = {

    /* f_grproc */
    FUNC( "ADVANCE"             , "D"               , TYPE_INT        , libmod_gfx_advance              ),
    FUNC( "XADVANCE"            , "ID"              , TYPE_INT        , libmod_gfx_xadvance             ),

    FUNC( "GET_ANGLE"           , "I"               , TYPE_INT        , libmod_gfx_get_angle            ),
    FUNC( "GET_ANGLE"           , "II"              , TYPE_INT        , libmod_gfx_get_angle2           ),
    FUNC( "GET_DIST"            , "I"               , TYPE_DOUBLE     , libmod_gfx_get_dist             ),
    FUNC( "GET_DIST"            , "II"              , TYPE_DOUBLE     , libmod_gfx_get_dist2            ),

    FUNC( "GET_REAL_POINT"      , "IDDPP"           , TYPE_INT        , libmod_gfx_get_real_point3      ),
    FUNC( "GET_REAL_POINT"      , "IIPP"            , TYPE_INT        , libmod_gfx_get_real_point2      ),
    FUNC( "GET_REAL_POINT"      , "IPP"             , TYPE_INT        , libmod_gfx_get_real_point       ),

    FUNC( "COLLISION"           , "II"              , TYPE_INT        , libmod_gfx_collision2           ),
    FUNC( "COLLISION"           , "I"               , TYPE_INT        , libmod_gfx_collision            ),

    /* scroll */
    FUNC( "SCROLL_START"        , "IIIIIIIII"       , TYPE_INT        , libmod_gfx_scroll_start2        ),
    FUNC( "SCROLL_START"        , "IIIIIII"         , TYPE_INT        , libmod_gfx_scroll_start         ),
    FUNC( "SCROLL_START"        , "IIIIII"          , TYPE_INT        , libmod_gfx_scroll_start3        ),
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
    FUNC( "WRITE_IN_MAP"        , "IIIIISI"         , TYPE_INT        , libmod_gfx_text_write_in_map2   ),

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
    FUNC( "MAP_BLOCK_COPY"      , "IIIIIIIIIIIBBBBI", TYPE_INT        , libmod_gfx_map_block_copy3      ),
    FUNC( "MAP_BLOCK_COPY"      , "IIIIIIIIIIIBBBBIIIIIII", TYPE_INT  , libmod_gfx_map_block_copy4      ),
    FUNC( "MAP_BLOCK_COPY"      , "IIIIIIIIII"      , TYPE_INT        , libmod_gfx_map_block_copy5      ),
    FUNC( "MAP_PUT"             , "IIIIII"          , TYPE_INT        , libmod_gfx_map_put              ),
    FUNC( "MAP_PUT"             , "IIIIIIIIIIBBBB"  , TYPE_INT        , libmod_gfx_map_put2             ),
    FUNC( "MAP_PUT"             , "IIIIIIIIIIBBBBI" , TYPE_INT        , libmod_gfx_map_put3             ),
    FUNC( "MAP_PUT"             , "IIIIIIIIIIBBBBIIIIIII", TYPE_INT   , libmod_gfx_map_put4             ),
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
    FUNC( "POINT_GET_TOTAL"     , "II"              , TYPE_INT        , libmod_gfx_get_point_total       ),

    /* Control boxes */
    FUNC( "BOX_SET"             , "IIIIIIII"        , TYPE_INT        , libmod_gfx_set_box              ),
    FUNC( "BOX_SET"             , "IIIIII"          , TYPE_INT        , libmod_gfx_set_box2             ), // use my instance_graph
    FUNC( "BOX_GET"             , "IIIPPPPP"        , TYPE_INT        , libmod_gfx_get_box              ),
    FUNC( "BOX_GET"             , "IPPPPP"          , TYPE_INT        , libmod_gfx_get_box2             ), // use my instance_graph
    FUNC( "BOX_GET_BY_POS"      , "IIIPPPPPP"       , TYPE_INT        , libmod_gfx_get_box_by_pos       ),
    FUNC( "BOX_GET_BY_POS"      , "IPPPPPP"         , TYPE_INT        , libmod_gfx_get_box_by_pos2      ), // use my instance_graph
    FUNC( "BOX_REMOVE"          , "III"             , TYPE_INT        , libmod_gfx_remove_box           ),
    FUNC( "BOX_REMOVE"          , "I"               , TYPE_INT        , libmod_gfx_remove_box2          ), // use my instance_graph
    FUNC( "BOX_GET_REAL_VERTEX" , "IIIIPP"          , TYPE_INT        , libmod_gfx_get_real_box_vertex2 ), // with instance
    FUNC( "BOX_GET_REAL_VERTEX" , "IIIPP"           , TYPE_INT        , libmod_gfx_get_real_box_vertex  ), // use my instance, but need graph
    FUNC( "BOX_GET_REAL_VERTEX" , "IPP"             , TYPE_INT        , libmod_gfx_get_real_box_vertex3 ), // use my instance_graph

    /* Fonts */
    FUNC( "FNT_NEW"             , ""                , TYPE_INT        , libmod_gfx_fnt_new              ),
    FUNC( "FNT_NEW"             , "I"               , TYPE_INT        , libmod_gfx_fnt_new_charset      ),
    FUNC( "FNT_NEW"             , "IIIIIIII"        , TYPE_INT        , libmod_gfx_fnt_new_from_bitmap  ),
    FUNC( "FNT_NEW"             , "IIIIIIIIS"       , TYPE_INT        , libmod_gfx_fnt_new_from_bitmap2 ),
    FUNC( "FNT_NEW"             , "IIIIIIIIIIII"    , TYPE_INT        , libmod_gfx_fnt_new_from_bitmap3 ),
    FUNC( "FNT_NEW"             , "IIIIIIIIIIIIS"   , TYPE_INT        , libmod_gfx_fnt_new_from_bitmap4 ),
    FUNC( "FNT_NEW"             , "IIIIIIIIIIIII"   , TYPE_INT        , libmod_gfx_fnt_new_from_bitmap5 ),
    FUNC( "FNT_NEW"             , "IIIIIIIISIIIII"  , TYPE_INT        , libmod_gfx_fnt_new_from_bitmap6 ),
    FUNC( "FNT_NEW"             , "IIIIIIIIIIIIIIIII", TYPE_INT       , libmod_gfx_fnt_new_from_bitmap7 ),
    FUNC( "FNT_NEW"             , "IIIIIIIIIIIISIIIII", TYPE_INT      , libmod_gfx_fnt_new_from_bitmap8 ),
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
    FUNC( "DRAWING_BLENDMODE"   , "IIIIII"          , TYPE_INT         , libmod_gfx_draw_drawing_blend_mode3    ),
    FUNC( "DRAWING_BLENDMODE"   , "IIIIIII"         , TYPE_INT         , libmod_gfx_draw_drawing_blend_mode4    ),

    FUNC( "DRAWING_Z"           , ""                , TYPE_INT         , libmod_gfx_draw_drawing_z_get          ),
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

    /* shaders */

    FUNC( "SHADER_GET_LANGUAGE"         , ""               , TYPE_INT        , libmod_gfx_shader_get_language          ),
    FUNC( "SHADER_GET_MIN_VERSION"      , ""               , TYPE_INT        , libmod_gfx_shader_get_min_version       ),
    FUNC( "SHADER_GET_MAX_VERSION"      , ""               , TYPE_INT        , libmod_gfx_shader_get_max_version       ),

    FUNC( "SHADER_CREATE"               , "SS"             , TYPE_POINTER    , libmod_gfx_shader_create                ),
    FUNC( "SHADER_FREE"                 , "P"              , TYPE_INT        , libmod_gfx_shader_free                  ),

#if 0
    FUNC( "SHADER_GETATTRIBUTELOCATION" , "PS"             , TYPE_INT        , libmod_gfx_shader_getattributelocation  ),
#endif
    FUNC( "SHADER_GET_PARAM_LOCATION"   , "PS"             , TYPE_INT        , libmod_gfx_shader_getuniformlocation    ),

    FUNC( "SHADER_CREATE_PARAMS"        , "I"              , TYPE_POINTER    , libmod_gfx_shader_create_parameters     ),

    FUNC( "SHADER_FREE_PARAMS"          , "P"              , TYPE_INT        , libmod_gfx_shader_free_parameters       ),

    FUNC( "SHADER_SET_PARAM"            , "PIII"           , TYPE_INT        , libmod_gfx_shader_setparam              ),
    FUNC( "SHADER_SET_PARAM"            , "PIIF"           , TYPE_INT        , libmod_gfx_shader_setparam_float        ),
    FUNC( "SHADER_SET_PARAM"            , "PIIIII"         , TYPE_INT        , libmod_gfx_shader_setparam_image        ),
    FUNC( "SHADER_SET_PARAM"            , "PIIIP"          , TYPE_INT        , libmod_gfx_shader_setparam_vector       ),
    FUNC( "SHADER_SET_PARAM"            , "PIIPIIII"       , TYPE_INT        , libmod_gfx_shader_setparam_matrix       ),

    FUNC( "SHADER_GET_PARAM"            , "PIPP"           , TYPE_INT        , libmod_gfx_shader_getparam              ),
    FUNC( "SHADER_GET_PARAM"            , "PIPPP"          , TYPE_INT        , libmod_gfx_shader_getparam_vector       ),
    FUNC( "SHADER_GET_PARAM"            , "PIPPPPPP"       , TYPE_INT        , libmod_gfx_shader_getparam_matrix       ),

    /* MEDIA */

    FUNC( "MEDIA_LOAD"                  , "SP"          , TYPE_POINTER      , libmod_gfx_media_load                 ),
    FUNC( "MEDIA_LOAD"                  , "SPI"         , TYPE_POINTER      , libmod_gfx_media_load2                ),
    FUNC( "MEDIA_UNLOAD"                , "P"           , TYPE_INT          , libmod_gfx_media_unload               ),
    FUNC( "MEDIA_PLAY"                  , "P"           , TYPE_INT          , libmod_gfx_media_play                 ),
    FUNC( "MEDIA_PAUSE"                 , "P"           , TYPE_INT          , libmod_gfx_media_pause                ),
    FUNC( "MEDIA_PAUSE"                 , "PI"          , TYPE_INT          , libmod_gfx_media_pause_action         ),
    FUNC( "MEDIA_RESUME"                , "P"           , TYPE_INT          , libmod_gfx_media_resume               ),
    FUNC( "MEDIA_STOP"                  , "P"           , TYPE_INT          , libmod_gfx_media_stop                 ),
    FUNC( "MEDIA_GET_TIME"              , "P"           , TYPE_INT          , libmod_gfx_media_get_time             ),
    FUNC( "MEDIA_GET_DURATION"          , "P"           , TYPE_INT          , libmod_gfx_media_get_duration         ),
    FUNC( "MEDIA_GET_STATUS"            , "P"           , TYPE_INT          , libmod_gfx_media_get_status           ),
    FUNC( "MEDIA_GET_MUTE"              , "P"           , TYPE_INT          , libmod_gfx_media_get_mute             ),
    FUNC( "MEDIA_SET_MUTE"              , "PI"          , TYPE_INT          , libmod_gfx_media_set_mute             ),
    FUNC( "MEDIA_GET_VOLUME"            , "P"           , TYPE_INT          , libmod_gfx_media_get_volume           ),
    FUNC( "MEDIA_SET_VOLUME"            , "PI"          , TYPE_INT          , libmod_gfx_media_set_volume           ),

    FUNC( NULL                          , NULL          , 0                 , NULL                                  )
};

/* --------------------------------------------------------------------------- */

char * __bgdexport( libmod_gfx, modules_dependency )[] = {
    "libbggfx",
    "libbginput",
    NULL
};

/* --------------------------------------------------------------------------- */

#endif
