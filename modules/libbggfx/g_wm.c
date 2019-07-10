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

#include "bgdrtm.h"

#include "bgddl.h"
#include "dlvaracc.h"

#include "libbggfx.h"

/* --------------------------------------------------------------------------- */
/* Gesti�n de eventos de ventana                                               */
/* --------------------------------------------------------------------------- */

/*
 *  FUNCTION : wm_events
 *
 *  Process all pending wm SDL events, updating all global variables
 *
 *  PARAMS :
 *      None
 *
 *  RETURN VALUE :
 *      None
 */

void wm_events() {
    SDL_Event e ;

    /* Procesa los eventos de ventana pendientes */

    GLOQWORD( libbggfx, EXIT_STATUS ) = 0 ;

    while ( SDL_PeepEvents( &e, 1, SDL_GETEVENT, SDL_QUIT, SDL_QUIT ) ) {
        /* UPDATE  exit status... initilized each frame */
        GLOQWORD( libbggfx, EXIT_STATUS ) = 1 ;
//        bgdrtm_exit( -1 );
    }

    while ( SDL_PeepEvents( &e, 1, SDL_GETEVENT, SDL_WINDOWEVENT, SDL_WINDOWEVENT ) ) {
        switch ( e.window.event ) {
            //Get new dimensions and repaint on window size change
/*
            case    SDL_WINDOWEVENT_SIZE_CHANGED:
                    mWidth = e.window.data1;
                    mHeight = e.window.data2;
                    SDL_RenderPresent( gRenderer );
                    break;
*/
            //Repaint on exposure
            case    SDL_WINDOWEVENT_EXPOSED:
#ifdef USE_NATIVE_SDL2
                    SDL_RenderPresent( gRenderer );
#else
                    GPU_Flip( gRenderer );
#endif
                    break;

            //Mouse entered window
            case    SDL_WINDOWEVENT_ENTER:
                    GLOQWORD( libbggfx, MOUSE_STATUS ) = 1 ;
                    break;

            //Mouse left window
            case    SDL_WINDOWEVENT_LEAVE:
                    GLOQWORD( libbggfx, MOUSE_STATUS ) = 0 ;
                    break;

            //Window has keyboard focus
            case    SDL_WINDOWEVENT_FOCUS_GAINED:
                    GLOQWORD( libbggfx, FOCUS_STATUS ) = 1 ;
                    break;

            //Window lost keyboard focus
            case    SDL_WINDOWEVENT_FOCUS_LOST:
                    GLOQWORD( libbggfx, FOCUS_STATUS ) = 0 ;
                    break;

            //Window minimized
            case SDL_WINDOWEVENT_MINIMIZED:
                    GLOQWORD( libbggfx, WINDOW_STATUS ) = 0 ;
                    break;

            //Window maxized
            case SDL_WINDOWEVENT_MAXIMIZED:
                    GLOQWORD( libbggfx, WINDOW_STATUS ) = 2 ;
                    break;

            //Window restored
            case SDL_WINDOWEVENT_RESTORED:
                    GLOQWORD( libbggfx, WINDOW_STATUS ) = 1 ;
                    break;
        }
    }
}

/* --------------------------------------------------------------------------- */
