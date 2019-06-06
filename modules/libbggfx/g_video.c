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
#include <string.h>

#include "bgdrtm.h"

#include "bgddl.h"
#include "dlvaracc.h"

#include "libbggfx.h"

#ifdef _WIN32
#include <initguid.h>
#include "ddraw.h"
#endif

/* --------------------------------------------------------------------------- */

//GRAPH * icon = NULL ;

char * apptitle = NULL ;

int64_t scr_width = 0 ;
int64_t scr_height = 0 ;

int64_t scr_initialized = 0 ;

int64_t full_screen = 0 ;
int64_t grab_input = 0 ;
int64_t frameless = 0 ;
int64_t waitvsync = 0 ;

int64_t scale_resolution = -1 ;
int64_t scale_resolution_aspectratio = 0;

//The window we'll be rendering to
SDL_Window * gWindow = NULL;
SDL_Renderer * gRenderer = NULL;
SDL_RendererInfo gRendererInfo = { 0 };

/*
SDL_RendererInfo

Data Fields

const char* name; // the name of the renderer
Uint32 flags; // a mask of supported renderer flags; see Remarks for details
Uint32 num_texture_formats; // the number of available texture formats
Uint32[16] texture_formats; // the available texture formats; see Remarks for details
int max_texture_width; // the maximum texture width
int max_texture_height; // the maximum texture height

flags may be 0 or a mask of any of the following SDL_RendererFlags values OR'd together:

SDL_RENDERER_SOFTWARE             the renderer is a software fallback
SDL_RENDERER_ACCELERATED          the renderer uses hardware acceleration
SDL_RENDERER_PRESENTVSYNC         present is synchronized with the refresh rate
SDL_RENDERER_TARGETTEXTURE        the renderer supports rendering to texture

Note that providing no flags gives priority to available SDL_RENDERER_ACCELERATED renderers.

texture_formats is an array of SDL_PixelFormatEnum values representing the available texture formats for the renderer
*/

/* --------------------------------------------------------------------------- */

int gr_set_icon( GRAPH * map ) {
//    SDL_SetWindowIcon(SDL_Window* window, SDL_Surface* icon);
    return 1;
}

/* --------------------------------------------------------------------------- */

void gr_set_caption( char * title ) {
    SDL_SetWindowTitle( gWindow, title );
}

/* --------------------------------------------------------------------------- */

int gr_set_mode( int width, int height, int flags ) {
    char * e;
    SDL_DisplayMode current;

    SDL_DisableScreenSaver();

    SDL_GetCurrentDisplayMode( 0, &current );

    if ( !width ) width = current.w;
    if ( !height ) height = current.h;

    int renderer_width = width;
    int renderer_height = height;

    full_screen = ( flags & MODE_FULLSCREEN ) ? 1 : 0 ;
    grab_input = ( flags & MODE_MODAL ) ? 1 : 0 ;
    frameless = ( flags & MODE_FRAMELESS ) ? 1 : 0 ;
    waitvsync = ( flags & MODE_WAITVSYNC ) ? 1 : 0 ;
    full_screen |= GLOQWORD( libbggfx, FULL_SCREEN );

    scale_resolution = GLOQWORD( libbggfx, SCALE_RESOLUTION );
    if ( GLOEXISTS( libbggfx, SCALE_RESOLUTION_ASPECTRATIO ) ) scale_resolution_aspectratio = GLOQWORD( libbggfx, SCALE_RESOLUTION_ASPECTRATIO );

    /* Overwrite all params */

    if ( ( e = getenv( "SCALE_RESOLUTION"             ) ) ) scale_resolution = atol( e );
    if ( ( e = getenv( "SCALE_RESOLUTION_ASPECTRATIO" ) ) ) scale_resolution_aspectratio = atol( e );

    if ( scale_resolution != -1 ) {
        renderer_width  = scale_resolution / 10000 ;
        renderer_height = scale_resolution % 10000 ;
    }

    /* Initialize video */
/*
    if ( gRenderer ) {
        SDL_DestroyRenderer( gRenderer );
        gRenderer = NULL;
    }

    if ( gWindow ) {
        SDL_DestroyWindow( gWindow );
        gWindow = NULL;
    }
*/
    /* Set texture filtering to linear */
    if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ) { // linear
        printf( "Warning: Linear texture filtering not enabled!" );
    }

    /*
    SDL_WINDOW_FULLSCREEN               fullscreen window
    SDL_WINDOW_FULLSCREEN_DESKTOP       fullscreen window at the current desktop resolution
    SDL_WINDOW_OPENGL                   window usable with OpenGL context
    SDL_WINDOW_SHOWN                    window is visible
    SDL_WINDOW_HIDDEN                   window is not visible
    SDL_WINDOW_BORDERLESS               no window decoration
    SDL_WINDOW_RESIZABLE                window can be resized
    SDL_WINDOW_MINIMIZED                window is minimized
    SDL_WINDOW_MAXIMIZED                window is maximized
    SDL_WINDOW_INPUT_GRABBED            window has grabbed input focus
    SDL_WINDOW_INPUT_FOCUS              window has input focus
    SDL_WINDOW_MOUSE_FOCUS              window has mouse focus
    SDL_WINDOW_FOREIGN                  window not created by SDL
    SDL_WINDOW_ALLOW_HIGHDPI            window should be created in high-DPI mode if supported (>= SDL 2.0.1)
    */

    if ( !gWindow ) {
        //Create window
        int sdl_flags = SDL_WINDOW_SHOWN;
        if ( frameless )  sdl_flags |= SDL_WINDOW_BORDERLESS;
        if ( full_screen )  sdl_flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
        if ( grab_input ) sdl_flags |= SDL_WINDOW_INPUT_GRABBED;

        gWindow = SDL_CreateWindow( apptitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, renderer_width, renderer_height, sdl_flags | SDL_WINDOW_OPENGL );
        if( gWindow == NULL ) return -1;
        SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    } else {
        SDL_SetWindowSize( gWindow, renderer_width, renderer_height );
        if ( frameless ) SDL_SetWindowBordered( gWindow, SDL_TRUE );
        if ( full_screen ) SDL_SetWindowFullscreen( gWindow, SDL_WINDOW_FULLSCREEN_DESKTOP );
        if ( grab_input ) SDL_SetWindowGrab( gWindow, SDL_TRUE );
    }

    if ( !gRenderer ) {
        //Create renderer for window
        gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | ( waitvsync ? SDL_RENDERER_PRESENTVSYNC : 0 ));
        if( gRenderer == NULL ) {
            printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
            return -1;
        }
        SDL_GetRendererInfo( gRenderer, &gRendererInfo );
        printf( "max texture size: %d x %d\n", gRendererInfo.max_texture_width, gRendererInfo.max_texture_height );
    }

    if ( waitvsync ) {
        if ( SDL_GL_SetSwapInterval( -1 ) == -1 ) {
            SDL_GL_SetSwapInterval( 1 );
        }
    } else {
        SDL_GL_SetSwapInterval( 0 );
    }

    //Initialize renderer color
    SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );

    scr_initialized = 1 ;

    SDL_ShowCursor( 0 ) ;

//    SDL_GetRendererOutputSize( gRenderer, &renderer_width, &renderer_height );

    if ( renderer_width != width || renderer_height != height ) {
        SDL_RenderSetLogicalSize( gRenderer, width, height );

        if ( scale_resolution_aspectratio != SRA_PRESERVE ) {
            SDL_RenderSetScale( gRenderer, (double) renderer_width / (double) width, (double) renderer_height  / (double) height );
        }
    }

    scr_width = width;
    scr_height = height;

    regions[0].x  = 0 ;
    regions[0].y  = 0 ;
    regions[0].x2 = scr_width - 1 ;
    regions[0].y2 = scr_height - 1 ;

    return 0;
}

/* --------------------------------------------------------------------------- */

void gr_video_init() {
    char * e;
    int flags = 0;

    if ( !SDL_WasInit( SDL_INIT_VIDEO ) ) SDL_InitSubSystem( SDL_INIT_VIDEO );

    apptitle = appname;

    if ( ( e = getenv( "VIDEO_WIDTH"  ) ) ) scr_width = atoi(e);
    if ( ( e = getenv( "VIDEO_HEIGHT" ) ) ) scr_height = atoi(e);
    if ( ( e = getenv( "VIDEO_FULLSCREEN" ) ) ) flags = atoi(e) ? MODE_FULLSCREEN : 0;

    gr_set_mode( scr_width, scr_height, flags );
}

/* --------------------------------------------------------------------------- */

void gr_video_exit() {
    if ( SDL_WasInit( SDL_INIT_VIDEO ) ) SDL_QuitSubSystem( SDL_INIT_VIDEO );
}

/* --------------------------------------------------------------------------- */
