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
            
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "g_media_theora.h"

#include "theora/theoradec.h"
#include "vorbis/codec.h"

#include "theoraplay.h"

#include "g_media.h"

/* --------------------------------------------------------------------------- */

// Definition of the audio queue structure
typedef struct AudioQueue {
    const THEORAPLAY_AudioPacket *audio; /**< Pointer to the audio packet. */
    int offset; /**< Offset into the audio packet. */
    struct AudioQueue *next; /**< Pointer to the next item in the queue. */
} AudioQueue;

/* --------------------------------------------------------------------------- */

/**
 * @brief Audio callback function for SDL.
 *
 * This function is called by SDL to provide audio data to the audio device.
 * It processes audio packets from the queue and writes audio samples to the stream.
 *
 * @param userdata Pointer to the THR_ID structure containing playback state.
 * @param stream Pointer to the audio stream buffer.
 * @param len Length of the audio stream buffer in bytes.
 */
static void SDLCALL audio_callback(void *userdata, Uint8 *stream, int len) {
    THR_ID *ctx = (THR_ID *)userdata;
    Uint32 baseticks = ctx->baseticks;
    Sint16 *dst = (Sint16 *)stream;

    if (!ctx->paused) {
        while (ctx->audio_queue && (len > 0)) {
            AudioQueue *item = (AudioQueue *)(ctx->audio_queue);
            AudioQueue *next = item->next;
            const int channels = item->audio->channels;
            const float *src = item->audio->samples + (item->offset * channels);
            int cpy = (item->audio->frames - item->offset) * channels;
            int i;

            if (cpy > (len / sizeof(Sint16)))
                cpy = len / sizeof(Sint16);

            if (ctx->muted || ctx->volume <= 0) {
                memset(dst, '\0', cpy * sizeof(Sint16));
                dst += cpy;
            } else {
                for (i = 0; i < cpy; i++) {
                    float val = *(src++);

                    val = val * ((float)ctx->volume / 128.0);

                    if (val < -1.0f)
                        *(dst++) = -32768;
                    else if (val > 1.0f)
                        *(dst++) = 32767;
                    else
                        *(dst++) = (Sint16)(val * 32767.0f);
                }
            }

            item->offset += (cpy / channels);
            len -= cpy * sizeof(Sint16);

            if (item->offset >= item->audio->frames) {
                THEORAPLAY_freeAudio(item->audio);
                free(item);
                ctx->audio_queue = next;
            }
        }

        if (!ctx->audio_queue)
            ctx->audio_queue_tail = NULL;
    }

    if (len > 0)
        memset(dst, '\0', len);
}

/* --------------------------------------------------------------------------- */

/**
 * @brief Adds audio to the queue.
 *
 * This function creates a new AudioQueue item and adds it to the end of the audio queue.
 *
 * @param ctx Pointer to the THR_ID structure containing playback state.
 * @param audio Pointer to the audio packet to be added to the queue.
 */
static void queue_audio(THR_ID *ctx, const THEORAPLAY_AudioPacket *audio) {
    if (!audio) return;

    AudioQueue *item = (AudioQueue *)malloc(sizeof(AudioQueue));
    if (!item) {
        THEORAPLAY_freeAudio(audio);
        return;
    }

    item->audio = audio;
    item->offset = 0;
    item->next = NULL;

    SDL_LockAudioDevice(ctx->audio_devid);
    if (ctx->audio_queue_tail)
        ((AudioQueue *)ctx->audio_queue_tail)->next = item;
    else
        ctx->audio_queue = item;
    ctx->audio_queue_tail = item;
    SDL_UnlockAudioDevice(ctx->audio_devid);
}

/* --------------------------------------------------------------------------- */

/**
 * @brief Adds more audio to the queue.
 *
 * This function retrieves additional audio packets from the decoder and adds them to the queue.
 * It ensures that the audio queue does not get too far ahead of the current playback time.
 *
 * @param ctx Pointer to the THR_ID structure containing playback state.
 * @param now Current playback time in milliseconds.
 */
static void queue_more_audio(THR_ID *ctx, const Uint32 now) {
    const THEORAPLAY_AudioPacket *audio;
    while ((audio = THEORAPLAY_getAudio(ctx->decoder)) != NULL) {
        const unsigned int playms = audio->playms;
        queue_audio(ctx, audio);
        if (playms >= now + 2000)  // don't let this get too far ahead.
            break;
    }
}

/* --------------------------------------------------------------------------- */

/**
 * @brief Opens the playback context.
 *
 * This function initializes a new playback context, sets up the decoder, and waits
 * for the decoder to be ready. It also retrieves the initial video and audio frames.
 *
 * @param fname Path to the media file to be opened.
 * @param timeout Maximum time to wait for the decoder to initialize in milliseconds.
 * @return Pointer to the THR_ID structure containing the playback context, or NULL if failed.
 */
THR_ID* thr_open(const char *fname, const uint32_t timeout) {
    THR_ID *ctx = (THR_ID *)malloc(sizeof(THR_ID));
    if (!ctx) {
        fprintf(stderr, "Failed to allocate context!\n");
        return NULL;
    }

    ctx->has_audio = 0;
    ctx->has_video = 0;
    ctx->opened_audio = 0;
    ctx->audio_queue = NULL;
    ctx->audio_queue_tail = NULL;
    ctx->shadow = NULL;
    ctx->muted = 0;
    ctx->volume = 128;
    ctx->paused = 1;
    ctx->playms = 0;

    const int MAX_FRAMES = 120;

    ctx->decoder = THEORAPLAY_startDecodeFile(fname, MAX_FRAMES, THEORAPLAY_VIDFMT_BGRA, NULL, 1);
    if (!ctx->decoder) {
        fprintf(stderr, "Failed to start decoding '%s'!\n", fname);
        free(ctx);
        return NULL;
    }

    Uint32 startticks = SDL_GetTicks();

    // Wait until the decoder has parsed out some basic truths from the
    // file. In a video game, you could choose not to block on this, and
    // instead do something else until the file is ready.
    while (!THEORAPLAY_isInitialized(ctx->decoder)) {
        SDL_Delay(0);
        if (timeout > 0 && SDL_GetTicks() >= startticks + timeout) {
            THEORAPLAY_stopDecode(ctx->decoder);
            free(ctx);
            return NULL;   
        }
    } // while

    ctx->has_audio = THEORAPLAY_hasAudioStream(ctx->decoder);
    ctx->has_video = THEORAPLAY_hasVideoStream(ctx->decoder);

    if (!ctx->has_video) {
        THEORAPLAY_stopDecode(ctx->decoder);
        free(ctx);
        return NULL;
    }

    while ((ctx->video = THEORAPLAY_getVideo(ctx->decoder)) == NULL) SDL_Delay(0);

    if (ctx->has_audio) {
        startticks = SDL_GetTicks();
        while ((ctx->audio = THEORAPLAY_getAudio(ctx->decoder)) == NULL) {
            if (THEORAPLAY_availableVideo(ctx->decoder) >= MAX_FRAMES || (timeout > 0 && SDL_GetTicks() >= startticks + timeout)) break;
            SDL_Delay(0);
        }
    }

    ctx->framems = (((THEORAPLAY_VideoFrame *)ctx->video)->fps == 0.0) ? 0 : ((Uint32)(1000.0 / ((THEORAPLAY_VideoFrame *)ctx->video)->fps));

    ctx->paused_ticks = ctx->baseticks = SDL_GetTicks();

    return ctx;
}

/* --------------------------------------------------------------------------- */

/**
 * @brief Updates the playback context.
 *
 * This function processes any new audio and video frames, updates the playback
 * state, and synchronizes audio and video. It also queues additional audio if needed.
 *
 * @param ctx Pointer to the THR_ID structure containing playback state.
 * @return 1 if a video frame was updated, 0 if no update, or -1 on error.
 */
int thr_update(THR_ID *ctx) {
    int ret = 0;

    if (!ctx) return -1;

    if (ctx->paused) return 1;

    if (THEORAPLAY_isDecoding(ctx->decoder)) {
        const Uint32 now = SDL_GetTicks() - ctx->baseticks;

        if (!ctx->video) ctx->video = THEORAPLAY_getVideo(ctx->decoder);

        if (ctx->has_audio && !ctx->opened_audio) {
            if (ctx->audio) {
                SDL_AudioSpec spec;
                memset(&spec, '\0', sizeof(SDL_AudioSpec));
                spec.freq = ((THEORAPLAY_AudioPacket *)ctx->audio)->freq;
                spec.format = AUDIO_S16SYS;
                spec.channels = ((THEORAPLAY_AudioPacket *)ctx->audio)->channels;
                spec.samples = 2048;
                spec.callback = audio_callback;
                spec.userdata = ctx;

                ctx->audio_devid = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0);
                if (ctx->audio_devid) {
                    ctx->opened_audio = 1;
                    queue_audio(ctx, ctx->audio);
                    ctx->audio = NULL;
                    queue_more_audio(ctx, 0);
                    SDL_PauseAudioDevice(ctx->audio_devid, 0);
                } else {
                    fprintf(stderr, "Failed to open audio device: %s\n", SDL_GetError());
//                    THEORAPLAY_stopDecode(ctx->decoder);
//                    free(ctx);
                }
            }
        }

        if (ctx->video && ((THEORAPLAY_VideoFrame *)ctx->video)->playms <= now) {
            if (ctx->framems && ((now - ((THEORAPLAY_VideoFrame *)ctx->video)->playms) >= ctx->framems)) {
                THEORAPLAY_VideoFrame *last = (THEORAPLAY_VideoFrame *)ctx->video;
                while ((ctx->video = THEORAPLAY_getVideo(ctx->decoder)) != NULL) {
                    THEORAPLAY_freeVideo(last);
                    last = (THEORAPLAY_VideoFrame *)ctx->video;
                    if ((now - ((THEORAPLAY_VideoFrame *)ctx->video)->playms) < ctx->framems) break;
                }
                if (!ctx->video) ctx->video = last;
            }

            if (ctx->video) {
                memcpy(ctx->shadow->pixels, ((THEORAPLAY_VideoFrame *)ctx->video)->pixels, ctx->shadow->h * ctx->shadow->pitch);
                ctx->playms = ((THEORAPLAY_VideoFrame *)ctx->video)->playms;
            }

            THEORAPLAY_freeVideo(ctx->video);
            ctx->video = NULL;

            ret = 1;
        }

        if (ctx->opened_audio && !ctx->audio) queue_more_audio(ctx, now);

    }

    return ret;
}

/* --------------------------------------------------------------------------- */

/**
 * @brief Closes the playback context.
 *
 * This function releases all resources associated with the playback context,
 * including audio and video data, and stops decoding.
 *
 * @param ctx Pointer to the THR_ID structure containing playback state.
 * @return None.
 */
void thr_close(THR_ID *ctx) {
    if (!ctx) return;
    if (ctx->opened_audio) {
        SDL_CloseAudioDevice(ctx->audio_devid);
        AudioQueue *aq = (AudioQueue *)ctx->audio_queue;
        ctx->audio_queue = NULL;
        while (aq) {
            AudioQueue *next = aq->next;
            free(aq);
            aq = next;
        }
    }

    if (ctx->video) THEORAPLAY_freeVideo(ctx->video);
    if (ctx->audio) THEORAPLAY_freeAudio(ctx->audio);
    if (ctx->decoder) THEORAPLAY_stopDecode(ctx->decoder);
    free(ctx);
}

/* --------------------------------------------------------------------------- */

/**
 * @brief Gets the size of the video.
 *
 * This function retrieves the width and height of the current video frame.
 *
 * @param ctx Pointer to the THR_ID structure containing playback state.
 * @param w Pointer to an integer to receive the width of the video.
 * @param h Pointer to an integer to receive the height of the video.
 * @return 0 on success, or -1 if the context or video is NULL.
 */
int thr_get_video_size(THR_ID *ctx, int *w, int *h) {
    if (!ctx || !ctx->video) return -1;

    if (w) *w = ((THEORAPLAY_VideoFrame *)ctx->video)->width;
    if (h) *h = ((THEORAPLAY_VideoFrame *)ctx->video)->height;

    return 0;
}

/* --------------------------------------------------------------------------- */

/**
 * @brief Sets the shadow surface for video rendering.
 *
 * This function sets the SDL_Surface that will be used for shadow rendering
 * of the video frames.
 *
 * @param ctx Pointer to the THR_ID structure containing playback state.
 * @param shadow Pointer to the SDL_Surface to be used as the shadow surface.
 * @return None.
 */
void thr_set_shadow_surface(THR_ID *ctx, SDL_Surface *shadow) {
    if (ctx) ctx->shadow = shadow;
}

/* --------------------------------------------------------------------------- */

/**
 * @brief Gets the current mute status.
 *
 * This function retrieves the current mute status of the audio.
 *
 * @param ctx Pointer to the THR_ID structure containing playback state.
 * @return 1 if muted, 0 if not muted, or -1 if the context is NULL.
 */
int thr_get_mute(THR_ID *ctx) {
    if (!ctx) return -1;
    return ctx->muted;
}

/* --------------------------------------------------------------------------- */

/**
 * @brief Sets the mute status.
 *
 * This function sets the mute status of the audio.
 *
 * @param ctx Pointer to the THR_ID structure containing playback state.
 * @param status Mute status (non-zero for mute, zero for unmute).
 * @return None.
 */
void thr_set_mute(THR_ID *ctx, int status) {
    if (ctx) ctx->muted = (status != 0);
}

/* --------------------------------------------------------------------------- */

/**
 * @brief Gets the current volume level.
 *
 * This function retrieves the current volume level of the audio.
 *
 * @param ctx Pointer to the THR_ID structure containing playback state.
 * @return Volume level (0-128), or -1 if the context is NULL.
 */
int thr_get_volume(THR_ID *ctx) {
    if (!ctx) return -1;
    return ctx->volume;
}

/* --------------------------------------------------------------------------- */

/**
 * @brief Sets the volume level.
 *
 * This function sets the volume level of the audio.
 *
 * @param ctx Pointer to the THR_ID structure containing playback state.
 * @param volume New volume level (0-128).
 * @return Previous volume level, or -1 if the context is NULL or volume is out of range.
 */
int thr_set_volume(THR_ID *ctx, int volume) {
    if (!ctx || volume < 0 || volume > 128) return -1;
    int vol = ctx->volume;
    ctx->volume = volume; 
    return vol;
}

/* --------------------------------------------------------------------------- */

/**
 * @brief Pauses or unpauses playback.
 *
 * This function sets the playback state to paused or playing.
 *
 * @param ctx Pointer to the THR_ID structure containing playback state.
 * @param action Action to perform (0 to unpause, 1 to pause).
 * @return None.
 */
void thr_pause(THR_ID *ctx, int action) {
    if (!ctx || (action != 0 && action != 1)) return;
    if (action && !ctx->paused) ctx->paused_ticks = SDL_GetTicks();
    if (!action && ctx->paused) {
        ctx->baseticks += SDL_GetTicks() - ctx->paused_ticks;
    }
    ctx->paused = action; 
}

/* --------------------------------------------------------------------------- */

/**
 * @brief Gets the current playback state.
 *
 * This function retrieves the current state of playback.
 *
 * @param ctx Pointer to the THR_ID structure containing playback state.
 * @return Playback state (MEDIA_STATUS_ERROR, MEDIA_STATUS_ENDED, MEDIA_STATUS_PAUSED, or MEDIA_STATUS_PLAYING).
 */
int thr_get_state(THR_ID *ctx) {
    if (!ctx) return MEDIA_STATUS_ERROR;
    if (!THEORAPLAY_isDecoding(ctx->decoder)) return MEDIA_STATUS_ENDED;
    if (ctx->paused) return MEDIA_STATUS_PAUSED;
    return MEDIA_STATUS_PLAYING;
}

/* --------------------------------------------------------------------------- */

/**
 * @brief Gets the current playback time.
 *
 * This function retrieves the current playback time in milliseconds.
 *
 * @param ctx Pointer to the THR_ID structure containing playback state.
 * @return Playback time in milliseconds, or 0 if the context is NULL.
 */
int thr_get_time(THR_ID *ctx) {
    if (!ctx) return 0;
    return ctx->playms;
}

/* --------------------------------------------------------------------------- */
