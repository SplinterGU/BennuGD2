/*
 *  Copyright (C) SplinterGU (Fenix/BennuGD) (Since 2006)
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

#ifndef __MEDIA_THEORA_H
#define __MEDIA_THEORA_H   

    #include "SDL.h"

    // Definición de la estructura de contexto
    typedef struct THR_ID {
        void *decoder;
        const void *video;
        const void *audio;
        SDL_Surface *shadow;
        int has_audio;
        int has_video;
        Uint32 framems;
        int opened_audio;
        Uint32 baseticks;
        SDL_AudioDeviceID audio_devid;
        void *audio_queue;
        void *audio_queue_tail;
        int muted;
        int volume;
        int paused;
        // internal
        Uint32 paused_ticks;
        unsigned int playms;  /* playback start time in milliseconds. */
    } THR_ID;

    extern THR_ID* thr_open(const char *fname, const uint32_t timeout);
    extern int thr_update(THR_ID *ctx);
    extern void thr_close(THR_ID *ctx);
    extern int thr_get_video_size(THR_ID *ctx, int *w, int *h);
    extern void thr_set_shadow_surface(THR_ID *ctx, SDL_Surface *shadow);
    extern int thr_get_mute( THR_ID *ctx );
    extern void thr_set_mute( THR_ID *ctx, int status );
    extern int thr_get_volume( THR_ID *ctx );
    extern int thr_set_volume( THR_ID *ctx, int volume );
    extern void thr_pause( THR_ID *ctx, int action );
    extern int thr_get_state( THR_ID *ctx );
    extern int thr_get_time( THR_ID * ctx );

#endif
