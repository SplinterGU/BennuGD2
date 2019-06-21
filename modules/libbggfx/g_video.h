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

#ifndef __G_VIDEO_H
#define __G_VIDEO_H

/* --------------------------------------------------------------------------- */
/* Graph Mode */
#define MODE_WINDOW         0x0000
#define MODE_FULLSCREEN     0x0200
#define MODE_MODAL          0x1000
#define MODE_FRAMELESS      0x2000
#define MODE_WAITVSYNC      0x4000

/* Scale resolution orientation */
#define SRO_NORMAL          0
#define SRO_LEFT            1
#define SRO_DOWN            2
#define SRO_RIGHT           3

/* Scale resolution aspectratio */
#define SRA_PRESERVE        0
#define SRA_OVERSCAN        1
#define SRA_FIT             2

/* --------------------------------------------------------------------------- */

#ifndef __BGDC__

extern char * apptitle;

extern int64_t scr_width;
extern int64_t scr_height;

extern int64_t scr_initialized;

extern int64_t fullscreen;
extern int64_t grab_input;
extern int64_t frameless;

extern int64_t waitvsync;

extern int64_t scale_resolution;

extern int64_t scale_resolution_aspectratio;
extern int64_t scale_resolution_orientation;

extern int renderer_width;
extern int renderer_height;

extern SDL_Window * gWindow;
extern SDL_Renderer * gRenderer;
extern SDL_RendererInfo gRendererInfo;
extern SDL_PixelFormat * gPixelFormat;

/* --------------------------------------------------------------------------- */

extern void gr_video_init();
extern void gr_video_exit();
extern int gr_init( int width, int height );
extern void gr_set_caption( char * title );
extern int gr_set_icon( GRAPH * map );
extern int gr_set_mode( int width, int height, int depth );

/* --------------------------------------------------------------------------- */

#endif
#endif
