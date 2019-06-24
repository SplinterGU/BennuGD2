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

#include <SDL.h>

#include "bgddl.h"
#include "dlvaracc.h"

#include "libbggfx.h"
#include "libbginput.h"

/* --------------------------------------------------------------------------- */

static GRAPH * mouse_map = NULL;

/* --------------------------------------------------------------------------- */

/*
 *  FUNCTION : mouse_events
 *
 *  Process all pending SDL events, updating all global variables
 *  and handling debug key presses
 *  and cannot be changed
 *
 *  PARAMS :
 *      None
 *
 *  RETURN VALUE :
 *      None
 */

void do_mouse_events() {
    SDL_Event e;

    /* El cambio de mouse.x/y afecta directamente al rat�n */

//    SDL_WarpMouseInWindow( gWindow, GLOINT64( libbginput, MOUSEX ), GLOINT64( libbginput, MOUSEY ) );

    /* Procesa los eventos de mouse pendientes */

    GLOQWORD( libbginput, MOUSEWHEELUP )   = 0;
    GLOQWORD( libbginput, MOUSEWHEELDOWN ) = 0;
    GLOQWORD( libbginput, MOUSEWHEELRIGHT) = 0;
    GLOQWORD( libbginput, MOUSEWHEELLEFT)  = 0;

    while ( SDL_PeepEvents( &e, 1, SDL_GETEVENT, SDL_MOUSEMOTION, SDL_MOUSEWHEEL ) > 0 ) {
        switch ( e.type ) {
            case SDL_MOUSEMOTION:
                GLOINT64( libbginput, MOUSEX ) = e.motion.x;
                GLOINT64( libbginput, MOUSEY ) = e.motion.y;
                break;

            case SDL_MOUSEBUTTONDOWN:
                if ( e.button.button == SDL_BUTTON_LEFT )   GLOQWORD( libbginput, MOUSELEFT )     = e.button.clicks;
                if ( e.button.button == SDL_BUTTON_MIDDLE ) GLOQWORD( libbginput, MOUSEMIDDLE )   = e.button.clicks;
                if ( e.button.button == SDL_BUTTON_RIGHT )  GLOQWORD( libbginput, MOUSERIGHT )    = e.button.clicks;
                if ( e.button.button == SDL_BUTTON_X1 )     GLOQWORD( libbginput, MOUSEX1 )       = e.button.clicks;
                if ( e.button.button == SDL_BUTTON_X2 )     GLOQWORD( libbginput, MOUSEX2 )       = e.button.clicks;
                break;

            case SDL_MOUSEBUTTONUP:
                if ( e.button.button == SDL_BUTTON_LEFT )   GLOQWORD( libbginput, MOUSELEFT )     = 0;
                if ( e.button.button == SDL_BUTTON_MIDDLE ) GLOQWORD( libbginput, MOUSEMIDDLE )   = 0;
                if ( e.button.button == SDL_BUTTON_RIGHT )  GLOQWORD( libbginput, MOUSERIGHT )    = 0;
                if ( e.button.button == SDL_BUTTON_X1 )     GLOQWORD( libbginput, MOUSEX1 )       = 0;
                if ( e.button.button == SDL_BUTTON_X2 )     GLOQWORD( libbginput, MOUSEX2 )       = 0;
                break;

            case SDL_MOUSEWHEEL:
                if ( e.wheel.x > 0 ) GLOQWORD( libbginput, MOUSEWHEELRIGHT) += e.wheel.x;
                else                 GLOQWORD( libbginput, MOUSEWHEELLEFT ) -= e.wheel.x;

                if ( e.wheel.y > 0 ) GLOQWORD( libbginput, MOUSEWHEELUP   ) += e.wheel.y;
                else                 GLOQWORD( libbginput, MOUSEWHEELDOWN ) -= e.wheel.y;
                break;

        }
    }
}

/* --------------------------------------------------------------------------- */

/*
 *  FUNCTION : mouse_info
 *
 *  Returns information about the mouse
 *
 *  PARAMS :
 *      ptr             Void pointer, used for compatibility with DLL-type objects
 *      clip            Region to fill with bounding box
 *
 *  RETURN VALUE :
 *      1 if the mouse has changed since last call
 */

static int mouse_info( void * what, REGION * clip, int64_t * z, int64_t * drawme ) {
    int64_t mousefile, mousegraph;

    * z = GLOINT64( libbginput, MOUSEZ );

    mousefile   = GLOQWORD( libbginput, MOUSEFILE );
    mousegraph  = GLOQWORD( libbginput, MOUSEGRAPH );
    mouse_map = ( mousegraph > 0 ) ? bitmap_get( mousefile, mousegraph ) : NULL;

    if ( !mouse_map ) {
        * drawme = 0;
        return 0;
    }

    * drawme = 1;

    return 1;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : mouse_draw
 *
 *  Draws the mouse graphic at screen
 *
 *  PARAMS :
 *      ptr             Void pointer, used for compatibility with DLL-type objects
 *      clip            Clipping region
 *
 *  RETURN VALUE :
 *      None
 */

static void mouse_draw( void * what, REGION * clip ) {
    int64_t r;
    REGION region;

    if ( !mouse_map ) return;

    r = GLOINT64( libbginput, MOUSEREGION );
    if ( r < 0 || r > 31 ) r = 0;

    region = regions[r];
    if ( clip ) region_union( &region, clip );

    SDL_Rect *mouse_map_clip = NULL, _mouse_map_clip;

    _mouse_map_clip.w = GLOINT64( libbginput, MOUSECLIPW );
    _mouse_map_clip.h = GLOINT64( libbginput, MOUSECLIPH );

    if ( _mouse_map_clip.w && _mouse_map_clip.h ) {
        _mouse_map_clip.x = GLOINT64( libbginput, MOUSECLIPX );
        _mouse_map_clip.y = GLOINT64( libbginput, MOUSECLIPY );
        mouse_map_clip = &_mouse_map_clip;
    }

    double sizex = GLODOUBLE( libbginput, MOUSESIZEX ), sizey = GLODOUBLE( libbginput, MOUSESIZEY );

    if ( sizex == 100.0 && sizey == 100.0 ) sizex = sizey = GLODOUBLE( libbginput, MOUSESIZE );

    gr_blit(    0,
                &region,
                GLOINT64( libbginput, MOUSEX ),
                GLOINT64( libbginput, MOUSEY ),
                GLOQWORD( libbginput, MOUSEFLAGS ),
                GLOINT64( libbginput, MOUSEANGLE ),
                sizex,
                sizey,
                mouse_map,
                mouse_map_clip,
                GLOBYTE( libbginput, MOUSEALPHA ),
                GLOBYTE( libbginput, MOUSECOLOR_R ),
                GLOBYTE( libbginput, MOUSECOLOR_G ),
                GLOBYTE( libbginput, MOUSECOLOR_B )
           );
}

/* --------------------------------------------------------------------------- */

void mouse_init() {
    gr_new_object( GLOINT64( libbginput, MOUSEZ ), mouse_info, mouse_draw, 0 );
}

/* --------------------------------------------------------------------------- */
