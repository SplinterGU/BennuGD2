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

/* --------------------------------------------------------------------------- */

#include <stdlib.h>

/* --------------------------------------------------------------------------- */

#include "bgddl.h"
#include "dlvaracc.h"

#include "libbggfx.h"

/* --------------------------------------------------------------------------- */
/* Son las variables que se desea acceder.                           */
/* El interprete completa esta estructura, si la variable existe.    */
/* (usada en tiempo de ejecucion)                                    */

DLVARFIXUP __bgdexport( libbggfx, globals_fixup )[] = {
    /* Nombre de variable global, puntero al dato, tama�o del elemento, cantidad de elementos */
    { "screen.fullscreen"                               , NULL, -1, -1 },
    { "screen.scale_resolution"                         , NULL, -1, -1 },
    { "screen.scale_resolution_aspectratio"             , NULL, -1, -1 },

    { "frame_info.fps"                                  , NULL, -1, -1 },
    { "frame_info.speed_gauge"                          , NULL, -1, -1 },
    { "frame_info.frame_time"                           , NULL, -1, -1 },
    { "frame_info.frames_count"                         , NULL, -1, -1 },

    { "fade_info.fading"                                , NULL, -1, -1 },

    { "wm_info.exit_status"                             , NULL, -1, -1 },
    { "wm_info.window_status"                           , NULL, -1, -1 },
    { "wm_info.focus_status"                            , NULL, -1, -1 },
    { "wm_info.mouse_status"                            , NULL, -1, -1 },

    { "text.z"                                          , NULL, -1, -1 },
    { "text.flags"                                      , NULL, -1, -1 },
    { "text.alpha"                                      , NULL, -1, -1 },
    { "text.color_r"                                    , NULL, -1, -1 },
    { "text.color_g"                                    , NULL, -1, -1 },
    { "text.color_b"                                    , NULL, -1, -1 },
    { "text.blendmode"                                  , NULL, -1, -1 },
    { "text.custom_blendmode"                           , NULL, -1, -1 },
    { "text.custom_blendmode.src_rgb"                   , NULL, -1, -1 },
    { "text.custom_blendmode.dst_rgb"                   , NULL, -1, -1 },
    { "text.custom_blendmode.src_alpha"                 , NULL, -1, -1 },
    { "text.custom_blendmode.dst_alpha"                 , NULL, -1, -1 },
    { "text.custom_blendmode.equation_rgb"              , NULL, -1, -1 },
    { "text.custom_blendmode.equation_alpha"            , NULL, -1, -1 },
    { "text.shader"                                     , NULL, -1, -1 },

    /* backgound */
    { "background.file"                                 , NULL, -1, -1 },
    { "background.graph"                                , NULL, -1, -1 },
    { "background.color_r"                              , NULL, -1, -1 },
    { "background.color_g"                              , NULL, -1, -1 },
    { "background.color_b"                              , NULL, -1, -1 },
    { "background.flags"                                , NULL, -1, -1 },
    { "background.angle"                                , NULL, -1, -1 },
    { "background.size"                                 , NULL, -1, -1 },
    { "background.size_x"                               , NULL, -1, -1 },
    { "background.size_y"                               , NULL, -1, -1 },
    { "background.blendmode"                            , NULL, -1, -1 },
    { "background.custom_blendmode"                     , NULL, -1, -1 },
    { "background.custom_blendmode.src_rgb"             , NULL, -1, -1 },
    { "background.custom_blendmode.dst_rgb"             , NULL, -1, -1 },
    { "background.custom_blendmode.src_alpha"           , NULL, -1, -1 },
    { "background.custom_blendmode.dst_alpha"           , NULL, -1, -1 },
    { "background.custom_blendmode.equation_rgb"        , NULL, -1, -1 },
    { "background.custom_blendmode.equation_alpha"      , NULL, -1, -1 },
    { "background.shader"                               , NULL, -1, -1 },

    { "scroll"                                          , NULL, -1, -1 },

    { "shader"                                          , NULL, -1, -1 },

    { NULL                                              , NULL, -1, -1 }
};

DLVARFIXUP __bgdexport( libbggfx, locals_fixup )[] = {
    { "ctype"                                           , NULL, -1, -1 },
    { "cnumber"                                         , NULL, -1, -1 },
    { "x"                                               , NULL, -1, -1 },
    { "y"                                               , NULL, -1, -1 },
    { "z"                                               , NULL, -1, -1 },
    { "file"                                            , NULL, -1, -1 },
    { "graph"                                           , NULL, -1, -1 },
    { "size"                                            , NULL, -1, -1 },
    { "angle"                                           , NULL, -1, -1 },
    { "flags"                                           , NULL, -1, -1 },
    { "region"                                          , NULL, -1, -1 },
    { "resolution"                                      , NULL, -1, -1 },
    { "size_x"                                          , NULL, -1, -1 },
    { "size_y"                                          , NULL, -1, -1 },
    { "xgraph"                                          , NULL, -1, -1 },
    { "_render_reserved_.object_id"                     , NULL, -1, -1 },
//    { "_render_reserved_.graph_ptr"                     , NULL, -1, -1 },
    { "_render_reserved_.xgraph_flags"                  , NULL, -1, -1 },
    { "reserved.status"                                 , NULL, -1, -1 },
    { "id"                                              , NULL, -1, -1 },
    { "render_file"                                     , NULL, -1, -1 },
    { "render_graph"                                    , NULL, -1, -1 },
    { "clip.x"                                          , NULL, -1, -1 },
    { "clip.y"                                          , NULL, -1, -1 },
    { "clip.w"                                          , NULL, -1, -1 },
    { "clip.h"                                          , NULL, -1, -1 },
    { "center.x"                                        , NULL, -1, -1 },
    { "center.y"                                        , NULL, -1, -1 },
    { "alpha"                                           , NULL, -1, -1 },
    { "color_r"                                         , NULL, -1, -1 },
    { "color_g"                                         , NULL, -1, -1 },
    { "color_b"                                         , NULL, -1, -1 },
    { "blendmode"                                       , NULL, -1, -1 },
    { "custom_blendmode"                                , NULL, -1, -1 },
    { "custom_blendmode.src_rgb"                        , NULL, -1, -1 },
    { "custom_blendmode.dst_rgb"                        , NULL, -1, -1 },
    { "custom_blendmode.src_alpha"                      , NULL, -1, -1 },
    { "custom_blendmode.dst_alpha"                      , NULL, -1, -1 },
    { "custom_blendmode.equation_rgb"                   , NULL, -1, -1 },
    { "custom_blendmode.equation_alpha"                 , NULL, -1, -1 },
    { "shader"                                          , NULL, -1, -1 },

    { NULL                                              , NULL, -1, -1 }
};

/* --------------------------------------------------------------------------- */

/* Bigest priority first execute
   Lowest priority last execute */

HOOK __bgdexport( libbggfx, handler_hooks )[] = {
    { 9500, gr_wait_frame },
    { 9000, gr_draw_frame },
    { 4700, wm_events     },
    {    0, NULL          }
} ;

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : instance_create_hook
 *
 *  PARAMS :
 *      r           Pointer to the instance
 *
 *  RETURN VALUE :
 *      None
 */

void __bgdexport( libbggfx, instance_create_hook )( INSTANCE * r ) {
    /* COORZ is 0 when a new instance is created */
    LOCQWORD( libbggfx, r, _OBJECTID ) = gr_new_object( /* LOCINT32( libbggfx, r, COORDZ ) */ 0, draw_instance_info, draw_instance, r );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : instance_destroy_hook
 *
 *  PARAMS :
 *      r           Pointer to the instance
 *
 *  RETURN VALUE :
 *      None
 */

void __bgdexport( libbggfx, instance_destroy_hook )( INSTANCE * r ) {
    if ( LOCQWORD( libbggfx, r, _OBJECTID ) ) gr_destroy_object( LOCQWORD( libbggfx, r, _OBJECTID ) );
}

/* --------------------------------------------------------------------------- */

void __bgdexport( libbggfx, module_initialize )() {
    grlib_init() ;
    gr_video_init();
    gr_font_init();
    frame_init();
}

/* --------------------------------------------------------------------------- */

void __bgdexport( libbggfx, module_finalize )() {
    frame_exit();
    gr_video_exit();
}

/* --------------------------------------------------------------------------- */
/* exports                                                                     */
/* --------------------------------------------------------------------------- */

#include "libbggfx_exports.h"

/* --------------------------------------------------------------------------- */
