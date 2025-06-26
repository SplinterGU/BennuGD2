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

/* --------------------------------------------------------------------------- */

#include <SDL.h>
#ifdef USE_SDL2_GPU
    #include <SDL_gpu.h>
#endif
#include <stdlib.h>
#include <string.h>

#include "bgdrtm.h"
#include "bgddl.h"
#include "dlvaracc.h"

#include "libbggfx.h"

/* --------------------------------------------------------------------------- */

/**
 * @brief Updates media playback and texture.
 *
 * This function checks if the media playback needs to be paused or resumed based
 * on the system's hold state. It updates the texture if required.
 *
 * @param what Pointer to the MEDIA structure.
 * @param bbox Pointer to a REGION structure for bounding box information.
 * @param z Pointer to an int64_t variable for depth information.
 * @param drawme Pointer to an int64_t variable for draw flag.
 * @return 0 on success.
 */
static int __media_info(void *what, REGION *bbox, int64_t *z, int64_t *drawme) {
    MEDIA *mh = (MEDIA *)what;

    int hold = debugger_show_console || system_paused;
    if (hold != mh->in_hold_state) {
        if (hold) {
            // Need pause
            mh->is_paused = media_get_status(mh) == MEDIA_STATUS_PAUSED;
            if (!mh->is_paused) media_pause(mh);
        } else {
            // Need resume
            if (!mh->is_paused) media_resume(mh);
        }
        mh->in_hold_state = hold;
    }

    int ret = !hold && thr_update(mh->m);
    if (ret || mh->texture_must_update) {
#ifdef USE_SDL2_GPU
        GPU_UpdateImage(mh->graph->tex, NULL, mh->surface, NULL);
#endif
        mh->texture_must_update = 0;
    }
    *drawme = 0;
    return 0;
}

/* --------------------------------------------------------------------------- */

/**
 * @brief Initializes media subsystem.
 *
 * This function initializes the SDL audio subsystem if it is not already initialized.
 *
 * @return None.
 */
void media_init() {

    if (!SDL_WasInit(SDL_INIT_AUDIO)) SDL_InitSubSystem(SDL_INIT_AUDIO);
}

/* --------------------------------------------------------------------------- */

/**
 * @brief Exits media subsystem.
 *
 * This function cleans up and shuts down the SDL audio subsystem if it was initialized.
 *
 * @return None.
 */
void media_exit() {
    if (SDL_WasInit(SDL_INIT_AUDIO)) SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

/* --------------------------------------------------------------------------- */

/**
 * @brief Loads media file and initializes media structure.
 *
 * This function creates a MEDIA structure, opens the media file, and initializes
 * related resources.
 *
 * @param media Path to the media file.
 * @param graph_id Pointer to an int64_t variable to receive the graph ID.
 * @param timeout Timeout value in milliseconds.
 * @return Pointer to the initialized MEDIA structure, or NULL if an error occurs.
 */
MEDIA *media_load(const char *media, int64_t *graph_id, int timeout) {
    int vw, vh;

    if (!media) return NULL;

    MEDIA *mh = calloc(1, sizeof(MEDIA));
    if (!mh) return NULL;

    mh->media = strdup(media);
    if (!mh->media) {
        free(mh);
        return NULL;
    }

    mh->m = thr_open(mh->media, mh->timeout);
    if (!mh->m) {
        free(mh->media);
        free(mh);
        return NULL;
    }

    mh->volume = thr_get_volume(mh->m);
    mh->is_muted = thr_get_mute(mh->m);

    if (thr_get_video_size(mh->m, &vw, &vh) != -1) {
        mh->surface = SDL_CreateRGBSurface(0, vw, vh, 32, 0, 0, 0, 0 /* Force alpha to opaque */);
        if (!mh->surface) {
            thr_close(mh->m);
            free(mh->media);
            free(mh);
            return NULL;
        }
        SDL_SetColorKey(mh->surface, SDL_FALSE, 0);

        mh->graph = bitmap_new(0, vw, vh, mh->surface);
        if (!mh->graph) {
            thr_close(mh->m);
            SDL_FreeSurface(mh->surface);
            free(mh->media);
            free(mh);
            return NULL;
        }

        *graph_id = (int64_t)(mh->graph->code = bitmap_next_code());

        grlib_add_map(0, mh->graph);

        mh->objectid = gr_new_object(INT64_MIN + 10, __media_info, NULL, (void *)mh);

        thr_set_shadow_surface(mh->m, mh->surface);
    }

    return mh;
}

/* --------------------------------------------------------------------------- */

/**
 * @brief Unloads media and releases resources.
 *
 * This function frees all resources associated with the MEDIA structure and
 * closes the media file.
 *
 * @param mh Pointer to the MEDIA structure to unload.
 * @return None.
 */
void media_unload(MEDIA *mh) {
    if (mh) {
        if (mh->objectid) gr_destroy_object(mh->objectid);

        thr_close(mh->m);
        if (mh->surface) SDL_FreeSurface(mh->surface);
        if (mh->graph) grlib_unload_map(0, mh->graph->code);

        free(mh->media);
        free(mh);
    }
}

/* --------------------------------------------------------------------------- */

/**
 * @brief Starts or restarts media playback.
 *
 * This function opens the media file if it is not already opened and starts playback.
 *
 * @param mh Pointer to the MEDIA structure.
 * @return 0 on success, or -1 if an error occurs.
 */
int media_play(MEDIA *mh) {
    if (!mh) return -1;
    if (thr_get_state(mh->m) == MEDIA_STATUS_ENDED) {
        thr_close(mh->m);
        mh->m = NULL;
    }
    if (!mh->m) {
        mh->m = thr_open(mh->media, mh->timeout);
        if (!mh->m) return -1;
        thr_set_shadow_surface(mh->m, mh->surface);
        thr_set_volume(mh->m, mh->volume);
        thr_set_mute(mh->m, mh->is_muted);
    }
    thr_pause(mh->m, 0);
    return 0;
}

/* --------------------------------------------------------------------------- */

/**
 * @brief Pauses or resumes media playback.
 *
 * This function pauses or resumes media playback based on the provided flag.
 *
 * @param mh Pointer to the MEDIA structure.
 * @param do_pause Flag indicating whether to pause (non-zero) or resume (zero).
 * @return None.
 */
void media_pause_action(MEDIA *mh, int do_pause) {
    if (!mh) return;
    thr_pause(mh->m, do_pause);
}

/* --------------------------------------------------------------------------- */

/**
 * @brief Pauses media playback.
 *
 * This function pauses the media playback.
 *
 * @param mh Pointer to the MEDIA structure.
 * @return None.
 */
void media_pause(MEDIA *mh) {
    if (!mh) return;
    thr_pause(mh->m, 1);
}

/* --------------------------------------------------------------------------- */

/**
 * @brief Resumes media playback.
 *
 * This function resumes the media playback.
 *
 * @param mh Pointer to the MEDIA structure.
 * @return None.
 */
void media_resume(MEDIA *mh) {
    if (!mh) return;
    thr_pause(mh->m, 0);
}

/* --------------------------------------------------------------------------- */

/**
 * @brief Stops media playback.
 *
 * This function stops media playback and closes the media file.
 *
 * @param mh Pointer to the MEDIA structure.
 * @return None.
 */
void media_stop(MEDIA *mh) {
    if (!mh) return;
    if (mh->m) thr_close(mh->m);
    mh->m = NULL;
}

/* --------------------------------------------------------------------------- */

/**
 * @brief Retrieves the current playback time.
 *
 * This function returns the current playback time in milliseconds.
 *
 * @param mh Pointer to the MEDIA structure.
 * @return Playback time in milliseconds, or -1 if the MEDIA structure is NULL.
 */
int64_t media_get_time(MEDIA *mh) {
    if (!mh) return -1;
    return (int64_t)thr_get_time(mh->m);
}

/* --------------------------------------------------------------------------- */

/**
 * @brief Retrieves the current playback status of the media.
 *
 * This function returns the status of the media playback, such as whether it is
 * playing, paused, stopped, or ended.
 *
 * @param mh Pointer to the MEDIA structure.
 * @return Playback status, which could be MEDIA_STATUS_PLAYING, MEDIA_STATUS_PAUSED,
 *         MEDIA_STATUS_STOPPED, MEDIA_STATUS_ENDED, or MEDIA_STATUS_ERROR. Returns
 *         MEDIA_STATUS_ERROR if the MEDIA structure is NULL.
 */
int media_get_status(MEDIA *mh) {
    if (!mh) return MEDIA_STATUS_ERROR;
    if (!mh->m) return MEDIA_STATUS_STOPPED;
    return thr_get_state(mh->m);
}

/* --------------------------------------------------------------------------- */

/**
 * @brief Retrieves the current mute status of the media.
 *
 * This function returns whether the media playback is currently muted or not.
 *
 * @param mh Pointer to the MEDIA structure.
 * @return Non-zero if the media is muted, 0 if it is not muted. Returns -1 if the
 *         MEDIA structure is NULL.
 */
int media_get_mute(MEDIA *mh) {
    if (!mh) return -1;
    return thr_get_mute(mh->m);
}

/* --------------------------------------------------------------------------- */

/**
 * @brief Mutes or unmutes media playback.
 *
 * This function sets the mute state for media playback.
 *
 * @param mh Pointer to the MEDIA structure.
 * @param mute Flag indicating whether to mute (non-zero) or unmute (zero).
 * @return None.
 */
void media_set_mute(MEDIA *mh, int mute) {
    if (!mh) return;
    mh->is_muted = mute;
    thr_set_mute(mh->m, mute);
}

/* --------------------------------------------------------------------------- */

/**
 * @brief Retrieves the current volume level.
 *
 * This function retrieves the current volume level for media playback.
 *
 * @param mh Pointer to the MEDIA structure.
 * @return Current volume level, or -1 if the MEDIA structure is NULL.
 */
int media_get_volume(MEDIA *mh) {
    if (!mh) return -1;
    return thr_get_volume(mh->m);
}

/* --------------------------------------------------------------------------- */

/**
 * @brief Sets the volume for media playback.
 *
 * This function sets the volume for media playback.
 *
 * @param mh Pointer to the MEDIA structure.
 * @param volume Volume level to set.
 * @return None.
 */
int media_set_volume(MEDIA *mh, int volume) {
    if (!mh) return -1;
    mh->volume = volume;
    return thr_set_volume(mh->m, volume);
}

/* --------------------------------------------------------------------------- */

/**
 * @brief Checks if media is currently muted.
 *
 * This function returns whether the media playback is muted.
 *
 * @param mh Pointer to the MEDIA structure.
 * @return Non-zero if muted, 0 otherwise. Returns -1 if the MEDIA structure is NULL.
 */
int media_is_muted(MEDIA *mh) {
    if (!mh) return -1;
    return mh->is_muted;
}

/* --------------------------------------------------------------------------- */

/**
 * @brief Retrieves the duration of the media.
 *
 * This function returns the total duration of the media in milliseconds. If the
 * duration cannot be determined, it returns -1.
 *
 * @param mh Pointer to the MEDIA structure.
 * @return Duration of the media in milliseconds. Returns -1 if the duration cannot
 *         be determined or if the MEDIA structure is NULL.
 */
int64_t media_get_duration(MEDIA *mh) {
    if (!mh) return -1;
    return (int64_t)-1;  // Placeholder value; actual implementation may vary.
}

/* --------------------------------------------------------------------------- */
