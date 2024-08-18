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

// Definición de la estructura de la cola de audio
typedef struct AudioQueue {
    const THEORAPLAY_AudioPacket *audio;
    int offset;
    struct AudioQueue *next;
} AudioQueue;

/* --------------------------------------------------------------------------- */

// Función de callback de audio para SDL
static void SDLCALL audio_callback(void *userdata, Uint8 *stream, int len) {
    THR_ID *ctx = (THR_ID *)userdata;
    Uint32 baseticks = ctx->baseticks;
    Sint16 *dst = (Sint16 *)stream;

    if ( !ctx->paused ) {
        while (ctx->audio_queue && (len > 0)) {
            AudioQueue *item = (AudioQueue *)(ctx->audio_queue);
            AudioQueue *next = item->next;
            const int channels = item->audio->channels;
            const float *src = item->audio->samples + (item->offset * channels);
            int cpy = (item->audio->frames - item->offset) * channels;
            int i;

            if (cpy > (len / sizeof(Sint16)))
                cpy = len / sizeof(Sint16);

            if ( ctx->muted || ctx->volume <= 0 ) {
                memset( dst, '\0', cpy * sizeof(Sint16));
                dst += cpy;
            } else {
                for (i = 0; i < cpy; i++) {
                    float val = *(src++);

                    val = val * ( ( float ) ctx->volume / 128.0 );

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

// Función para añadir audio a la cola
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

// Función para añadir más audio a la cola
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

// Función para abrir el contexto
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
    //  file. In a video game, you could choose not to block on this, and
    //  instead do something else until the file is ready.
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
            if (THEORAPLAY_availableVideo(ctx->decoder) >= MAX_FRAMES || ( timeout > 0 && SDL_GetTicks() >= startticks + timeout ) ) break;
            SDL_Delay(0);
        }
    }

    ctx->framems = (((THEORAPLAY_VideoFrame *)ctx->video)->fps == 0.0) ? 0 : ((Uint32)(1000.0 / ((THEORAPLAY_VideoFrame *)ctx->video)->fps));

    ctx->paused_ticks = ctx->baseticks = SDL_GetTicks();

    return ctx;
}

/* --------------------------------------------------------------------------- */

// Función para actualizar el contexto
int thr_update(THR_ID *ctx) {
    int ret = 0;

    if ( !ctx ) return -1;

    if ( ctx->paused ) return 1;

    if ( THEORAPLAY_isDecoding( ctx->decoder ) ) {
        const Uint32 now = SDL_GetTicks() - ctx->baseticks;

        if (!ctx->video) ctx->video = THEORAPLAY_getVideo(ctx->decoder);

        if (ctx->has_audio && !ctx->opened_audio ) {
            if (ctx->audio){
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

        if ( ctx->video && ((THEORAPLAY_VideoFrame *)ctx->video)->playms <= now ) {
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

        if ( ctx->opened_audio && !ctx->audio ) queue_more_audio(ctx, now);

    }

    return ret;
}

/* --------------------------------------------------------------------------- */

// Función para cerrar el contexto
void thr_close(THR_ID *ctx) {
    if (!ctx) return;
    if (ctx->opened_audio) {
        SDL_CloseAudioDevice(ctx->audio_devid);
        AudioQueue * aq = (AudioQueue *) ctx->audio_queue;
        ctx->audio_queue = NULL;
        while ( aq ) {
            AudioQueue * next = aq->next;
            free( aq );
            aq = next;
        }
    }

    if (ctx->video) THEORAPLAY_freeVideo(ctx->video);
    if (ctx->audio) THEORAPLAY_freeAudio(ctx->audio);
    if (ctx->decoder) THEORAPLAY_stopDecode(ctx->decoder);
    free(ctx);
}

/* --------------------------------------------------------------------------- */

// Función para obtener el tamaño del video
int thr_get_video_size(THR_ID *ctx, int *w, int *h) {
    if (!ctx || !ctx->video) return -1;

    if ( w ) *w = ((THEORAPLAY_VideoFrame *)ctx->video)->width;
    if ( h ) *h = ((THEORAPLAY_VideoFrame *)ctx->video)->height;

    return 0;
}

/* --------------------------------------------------------------------------- */

void thr_set_shadow_surface(THR_ID *ctx, SDL_Surface *shadow) {
    if (ctx) ctx->shadow = shadow;
}

/* --------------------------------------------------------------------------- */

int thr_get_mute( THR_ID *ctx ) {
    if ( !ctx ) return -1;
    return ctx->muted;
}

/* --------------------------------------------------------------------------- */

void thr_set_mute( THR_ID *ctx, int status ) {
    if ( ctx ) ctx->muted = ( status != 0 );
}

/* --------------------------------------------------------------------------- */

int thr_get_volume( THR_ID *ctx ) {
    if ( !ctx ) return -1;
    return ctx->volume;
}

/* --------------------------------------------------------------------------- */

int thr_set_volume( THR_ID *ctx, int volume ) {
    if ( !ctx || volume < 0 || volume > 128 ) return -1;
    int vol = ctx->volume;
    ctx->volume = volume; 
    return vol;
}

/* --------------------------------------------------------------------------- */

void thr_pause( THR_ID *ctx, int action ) {
    if ( !ctx || ( action != 0 && action != 1 ) ) return;
    if ( action && !ctx->paused ) ctx->paused_ticks = SDL_GetTicks();
    if ( !action && ctx->paused ) {
        ctx->baseticks += SDL_GetTicks() - ctx->paused_ticks;
    }
    ctx->paused = action; 
}

/* --------------------------------------------------------------------------- */

int thr_get_state( THR_ID *ctx ) {
    if ( !ctx ) return MEDIA_STATUS_ERROR;
    if ( !THEORAPLAY_isDecoding( ctx->decoder ) ) return MEDIA_STATUS_ENDED;
    if ( ctx->paused ) return MEDIA_STATUS_PAUSED;
    return MEDIA_STATUS_PLAYING;
}

/* --------------------------------------------------------------------------- */

int thr_get_time( THR_ID * ctx ) {
    if ( !ctx ) return 0;
    return ctx->playms;
}

/* --------------------------------------------------------------------------- */
