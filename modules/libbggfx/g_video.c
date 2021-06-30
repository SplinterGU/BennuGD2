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

#ifdef USE_SDL2_GPU
#include <GL/gl.h>
#endif

/* --------------------------------------------------------------------------- */

//GRAPH * icon = NULL ;

char * apptitle = NULL ;

int64_t scr_width = 320 ;
int64_t scr_height = 240 ;

int64_t scr_initialized = 0 ;

int64_t fullscreen = 0 ;
int64_t grab_input = 0 ;
int64_t frameless = 0 ;
int64_t waitvsync = 0 ;

int64_t scale_resolution = -1 ;
int64_t scale_resolution_aspectratio = 0;

int renderer_width = 0;
int renderer_height = 0;

//The window we'll be rendering to
SDL_Window * gWindow = NULL;

#ifdef USE_SDL2
SDL_Renderer * gRenderer = NULL;
#else
GPU_Target * gRenderer = NULL;
#endif

SDL_RendererInfo gRendererInfo = { 0 };
SDL_PixelFormat * gPixelFormat = NULL;
SDL_Surface * gIcon = NULL;
int64_t gMaxTextureSize = 0;

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

static void show_renderer_info( SDL_RendererInfo * ri ) {
    Uint32 Rmask, Gmask, Bmask, Amask;
    int    bpp;

    printf( "Name                       : %s\n", ri->name );
    printf( "Software Render            : %s\n", ri->flags & SDL_RENDERER_SOFTWARE      ? "Yes" : "No" );
    printf( "Hardware Acceleration      : %s\n", ri->flags & SDL_RENDERER_ACCELERATED   ? "Yes" : "No" );
    printf( "Vsync Present              : %s\n", ri->flags & SDL_RENDERER_PRESENTVSYNC  ? "Yes" : "No" );
    printf( "Rendering to Texture       : %s\n", ri->flags & SDL_RENDERER_TARGETTEXTURE ? "Yes" : "No" );
    printf( "Max Texture Size           : %d x %d\n", ri->max_texture_width, ri->max_texture_height );
    printf( "Supported Texture Formats  :\n" );

    for ( int i = 0; i < ri->num_texture_formats; ++i ) {
        SDL_PixelFormatEnumToMasks( ri->texture_formats[i],
                                    &bpp,
                                    &Rmask,
                                    &Gmask,
                                    &Bmask,
                                    &Amask);
        printf( "%s Bpp: %d RGBA Mask: %08"PRIX32" %08"PRIX32" %08"PRIX32" %08"PRIX32"\n",
                SDL_GetPixelFormatName( ri->texture_formats[i] ),
                bpp,
                Rmask,
                Gmask,
                Bmask,
                Amask
                );

    }
    printf("\n");

}

/* --------------------------------------------------------------------------- */

SDL_PixelFormat * get_system_pixel_format( void ) {
#ifdef USE_SDL2
    #define PIXFMT  SDL_PIXELFORMAT_ARGB8888
#endif
#ifdef USE_SDL2_GPU
    #define PIXFMT  SDL_PIXELFORMAT_RGBA8888
#endif
    if ( !gPixelFormat ) gPixelFormat = SDL_AllocFormat( PIXFMT );
    return gPixelFormat;
}

/* --------------------------------------------------------------------------- */

int gr_set_icon( GRAPH * map ) {
    if ( gWindow ) {
#ifdef USE_SDL2
        gIcon = map->surface;
#endif
#ifdef USE_SDL2_GPU
        if ( gIcon ) SDL_FreeSurface( gIcon );
        gIcon = GPU_CopySurfaceFromImage( map->tex );
#endif
        SDL_SetWindowIcon( gWindow, gIcon );
    }

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

    SDL_GetCurrentDisplayMode( 0, &current );

    if ( !width ) width = current.w;
    if ( !height ) height = current.h;

    renderer_width = width;
    renderer_height = height;

    fullscreen = ( flags & MODE_FULLSCREEN ) ? 1 : 0 ;
    grab_input = ( flags & MODE_MODAL ) ? 1 : 0 ;
    frameless = ( flags & MODE_FRAMELESS ) ? 1 : 0 ;
    waitvsync = ( flags & MODE_WAITVSYNC ) ? 1 : 0 ;
    fullscreen |= GLOQWORD( libbggfx, fullscreen );

    scale_resolution = GLOQWORD( libbggfx, SCALE_RESOLUTION );
    scale_resolution_aspectratio = GLOQWORD( libbggfx, SCALE_RESOLUTION_ASPECTRATIO );

    /* Overwrite all params */

    if ( ( e = getenv( "SCALE_RESOLUTION"             ) ) ) scale_resolution = atol( e );
    if ( ( e = getenv( "SCALE_RESOLUTION_ASPECTRATIO" ) ) ) scale_resolution_aspectratio = atol( e );

    if ( !scale_resolution ) scale_resolution = ( int ) current.w * 10000L + ( int ) current.h ;

    if ( scale_resolution != -1 ) {
        renderer_width  = ( int ) scale_resolution / 10000L ;
        renderer_height = ( int ) scale_resolution % 10000L ;
    }

#ifdef USE_SDL2
    SDL_SetHint( SDL_HINT_RENDER_VSYNC, waitvsync ? "1" : "0" );

    if ( !gWindow ) {
        //Create window
        int sdl_flags = SDL_WINDOW_SHOWN;
        if ( frameless ) sdl_flags |= SDL_WINDOW_BORDERLESS;
        if ( fullscreen ) sdl_flags |= SDL_WINDOW_FULLSCREEN;
        if ( grab_input ) sdl_flags |= SDL_WINDOW_INPUT_GRABBED;
        gWindow = SDL_CreateWindow( apptitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, renderer_width, renderer_height, sdl_flags | SDL_WINDOW_OPENGL );
        if( gWindow == NULL ) return -1;
    } else {
        SDL_SetWindowFullscreen( gWindow, fullscreen  ? SDL_WINDOW_FULLSCREEN : 0 );
        SDL_SetWindowBordered( gWindow, frameless ? SDL_FALSE : SDL_TRUE );
        SDL_SetWindowGrab( gWindow, grab_input ? SDL_TRUE : SDL_FALSE );
        SDL_SetWindowSize( gWindow, renderer_width, renderer_height );
        SDL_SetWindowPosition( gWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED );
    }

    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

    if ( !gRenderer ) {
        //Create renderer for window
        gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
        if( gRenderer == NULL ) {
            printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
            return -1;
        }
        SDL_GetRendererInfo( gRenderer, &gRendererInfo );
        gMaxTextureSize = gRendererInfo.max_texture_width;
        show_renderer_info( &gRendererInfo );
//        printf( "max texture size: %d x %d\n", gRendererInfo.max_texture_width, gRendererInfo.max_texture_height );
    }
#endif
#ifdef USE_SDL2_GPU
    if ( !gRenderer ) {
        // Create Renderer
        int sdl_flags = SDL_WINDOW_SHOWN;
        if ( frameless ) sdl_flags |= SDL_WINDOW_BORDERLESS;
        if ( fullscreen ) sdl_flags |= SDL_WINDOW_FULLSCREEN;
        if ( grab_input ) sdl_flags |= SDL_WINDOW_INPUT_GRABBED;
/*
        GPU_InitFlagEnum GPU_flags = GPU_GetPreInitFlags() & ~( GPU_INIT_ENABLE_VSYNC | GPU_INIT_DISABLE_VSYNC );
        if ( !waitvsync ) GPU_flags |= GPU_INIT_DISABLE_VSYNC;
        GPU_SetPreInitFlags( GPU_flags );
*/
        gRenderer = GPU_Init( renderer_width, renderer_height, sdl_flags | SDL_WINDOW_OPENGL );
        if( gRenderer == NULL ) return -1;
//        gWindow = SDL_GetWindowFromID( gRenderer->context->windowID );
        gWindow = SDL_GetWindowFromID(GPU_GetInitWindow());

        GLint params = 0;
        glGetIntegerv( GL_MAX_TEXTURE_SIZE, &params );
        gMaxTextureSize = ( int64_t ) params;
    } else {
        if ( !fullscreen && GPU_GetFullscreen() ) GPU_SetWindowResolution( 1, 1 ); // Force Update when go to window mode (dirty fix)
        GPU_SetFullscreen( fullscreen ? GPU_TRUE : GPU_FALSE, GPU_FALSE );
        GPU_SetWindowResolution( renderer_width, renderer_height );
        GPU_Clear( gRenderer );

        GPU_SetViewport( gRenderer, GPU_MakeRect(0, 0, renderer_width, renderer_height) );
        GPU_SetVirtualResolution( gRenderer, renderer_width, renderer_height );
    }

    if ( waitvsync ) {
        if ( SDL_GL_SetSwapInterval( -1 ) == -1 ) {
            SDL_GL_SetSwapInterval( 1 );
        }
    } else {
        SDL_GL_SetSwapInterval( 0 );
    }

    if ( fullscreen ) {
        SDL_GetCurrentDisplayMode( 0, &current );
        if ( current.w != renderer_width || current.h != renderer_height ) {
            renderer_width = current.w;
            renderer_height = current.h;
            if ( !scale_resolution || scale_resolution == -1 ) {
                scale_resolution = renderer_width * 1000 + renderer_height;
                scale_resolution_aspectratio = SRA_PRESERVE;
            }
        }
    }

#endif

#ifdef USE_SDL2
    //Initialize renderer color
    SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
#endif

    SDL_ShowCursor( 0 ) ;

    scr_initialized = 1 ;

    if ( scale_resolution && scale_resolution != -1 && ( renderer_width != width || renderer_height != height ) ) {
        switch ( scale_resolution_aspectratio ) {
            case SRA_PRESERVE:
#ifdef USE_SDL2
                SDL_SetHint( SDL_HINT_RENDER_LOGICAL_SIZE_MODE, "letterbox");
                SDL_RenderSetLogicalSize( gRenderer, width, height );
#endif
#ifdef USE_SDL2_GPU
            {
                double relw = ( double ) renderer_width / ( double ) width,
                       relh = ( double ) renderer_height / ( double ) height,
                       vp, vp_offset;

                if ( relw > relh ) {
                    vp = width * renderer_height / ( double ) height;
                    vp_offset = ( renderer_width - vp ) / 2.0;
                    GPU_SetViewport( gRenderer, GPU_MakeRect( vp_offset, 0.0, vp, renderer_height ) );
                } else if ( relw < relh ) {
                    vp = height * renderer_width / ( double ) width;
                    vp_offset = ( renderer_height - vp ) / 2.0;
                    GPU_SetViewport( gRenderer, GPU_MakeRect( 0.0, vp_offset, renderer_width, vp ) );
                }
            }

                GPU_SetVirtualResolution( gRenderer, width, height );
#endif
                break;

            case SRA_OVERSCAN:
#ifdef USE_SDL2
                SDL_SetHint( SDL_HINT_RENDER_LOGICAL_SIZE_MODE, "overscan");
                SDL_RenderSetLogicalSize( gRenderer, width, height );
#endif
#ifdef USE_SDL2_GPU
            {
                double relw = ( double ) renderer_width / ( double ) width,
                       relh = ( double ) renderer_height / ( double ) height,
                       vp, vp_offset;

                if ( relw < relh ) {
                    vp = width * renderer_height / ( double ) height;
                    vp_offset = ( renderer_width - vp ) / 2.0;
                    GPU_SetViewport( gRenderer, GPU_MakeRect( vp_offset, 0.0, vp, renderer_height ) );
                } else if ( relw > relh ) {
                    vp = height * renderer_width / ( double ) width;
                    vp_offset = ( renderer_height - vp ) / 2.0;
                    GPU_SetViewport( gRenderer, GPU_MakeRect( 0.0, vp_offset, renderer_width, vp ) );
                }
                GPU_SetVirtualResolution( gRenderer, width, height );
            }
#endif
                break;

            case SRA_FIT:
#ifdef USE_SDL2
                // Issues with rotated textures
                SDL_SetHint( SDL_HINT_RENDER_LOGICAL_SIZE_MODE, "overscan");
                SDL_RenderSetLogicalSize( gRenderer, width, height );
                SDL_RenderSetScale( gRenderer, (float) renderer_width / (float) width, (float) renderer_height / (float) height );
                SDL_RenderSetViewport(gRenderer, NULL); // Fix SDL_RenderSetScale issue
#endif
#ifdef USE_SDL2_GPU
                GPU_SetViewport( gRenderer, GPU_MakeRect(0, 0, renderer_width, renderer_height) );
                GPU_SetVirtualResolution( gRenderer, width, height );
#endif
                break;
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

    SDL_DisableScreenSaver();

    ( void ) get_system_pixel_format();

    apptitle = appname;

/*
    if ( ( e = getenv( "VIDEO_WIDTH"  ) ) ) scr_width = atoi(e);
    if ( ( e = getenv( "VIDEO_HEIGHT" ) ) ) scr_height = atoi(e);
    if ( ( e = getenv( "VIDEO_FULLSCREEN" ) ) ) flags = atoi(e) ? MODE_FULLSCREEN : 0;

    gr_set_mode( scr_width, scr_height, flags );
*/
}

/* --------------------------------------------------------------------------- */

void gr_video_exit() {
    if ( gPixelFormat ) SDL_FreeFormat( gPixelFormat );
#ifdef USE_SDL2
    if ( gRenderer ) SDL_DestroyRenderer( gRenderer );
    if ( gWindow ) SDL_DestroyWindow( gWindow );
#endif
#ifdef USE_SDL2_GPU
    if ( gRenderer ) GPU_Quit();
#endif

    if ( SDL_WasInit( SDL_INIT_VIDEO ) ) SDL_QuitSubSystem( SDL_INIT_VIDEO );
}

/* --------------------------------------------------------------------------- */
