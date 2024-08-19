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

#ifndef __MEDIA_H
#define __MEDIA_H

#include <SDL.h>
#ifdef USE_SDL2_GPU
    #include <SDL_gpu.h>
#endif
#include <stdlib.h>
#include <string.h>

#include "g_media_theora.h"
#include "libbggfx.h"

/* --------------------------------------------------------------------------- */

/**
 * @brief Enumeration of media playback status codes.
 *
 * This enumeration defines the possible states of media playback.
 */
enum {
    MEDIA_STATUS_ERROR = -1,    /**< Error state. */
    MEDIA_STATUS_STOPPED = 0,  /**< Media is stopped. */
    MEDIA_STATUS_PLAYING,      /**< Media is currently playing. */
    MEDIA_STATUS_PAUSED,       /**< Media is paused. */
    MEDIA_STATUS_ENDED         /**< Media playback has ended. */
};

/**
 * @brief Structure representing media data and state.
 *
 * This structure holds information about the media being played, including
 * playback state, video dimensions, and related resources.
 */
typedef struct media {
    THR_ID *m;                 /**< Pointer to the media decoder and state. */

    GRAPH * graph;             /**< Pointer to the associated graphics data. */

#if defined(USE_SDL2_GPU) || defined(USE_SDL2)
    SDL_Surface * surface;     /**< Surface for rendering the video. */
    int texture_must_update;   /**< Flag indicating if the texture needs updating. */
#endif

    int64_t objectid;          /**< Identifier for the media object. */

    int in_hold_state;         /**< Flag indicating if the media is in hold state. */
    int is_paused;             /**< Flag indicating if the media is paused. */
    int is_muted;              /**< Flag indicating if the media is muted. */
    int volume;                /**< Volume level of the media. */

    char * media;              /**< File name or identifier for the media. */
    int timeout;               /**< Timeout value for media operations. */

} MEDIA;

/* --------------------------------------------------------------------------- */
/* General                                                                     */
/* --------------------------------------------------------------------------- */

/**
 * @brief Initializes the media subsystem.
 *
 * This function initializes the necessary components for media playback, such
 * as the SDL audio subsystem.
 */
extern void media_init();

/**
 * @brief Cleans up and shuts down the media subsystem.
 *
 * This function cleans up resources used by the media subsystem and shuts down
 * any subsystems that were initialized.
 */
extern void media_exit();

/**
 * @brief Loads media and prepares it for playback.
 *
 * This function initializes a new MEDIA structure and prepares it for playback
 * by opening the media file and setting up necessary resources.
 *
 * @param media Pointer to a string containing the media file name or identifier.
 * @param graph_id Pointer to a variable that will receive the graph identifier.
 * @param timeout Timeout value for media operations.
 * @return Pointer to a MEDIA structure if successful, or NULL if an error occurs.
 */
extern MEDIA * media_load(const char *media, int64_t *graph_id, int timeout);

/**
 * @brief Unloads and frees resources associated with the media.
 *
 * This function releases any resources associated with the MEDIA structure and
 * deallocates it.
 *
 * @param mh Pointer to the MEDIA structure to be unloaded.
 */
extern void media_unload(MEDIA *mh);

/**
 * @brief Starts media playback.
 *
 * This function starts playing the media. If the media was previously stopped
 * or ended, it will be reopened.
 *
 * @param mh Pointer to the MEDIA structure.
 * @return 0 if successful, or -1 if an error occurs.
 */
extern int media_play(MEDIA *mh);

/**
 * @brief Pauses or resumes media playback.
 *
 * This function sets the media playback to paused or resumed based on the provided
 * action flag.
 *
 * @param mh Pointer to the MEDIA structure.
 * @param do_pause Flag indicating whether to pause (non-zero) or resume (zero) playback.
 */
extern void media_pause_action(MEDIA *mh, int do_pause);

/**
 * @brief Pauses media playback.
 *
 * This function pauses the media playback.
 *
 * @param mh Pointer to the MEDIA structure.
 */
extern void media_pause(MEDIA *mh);

/**
 * @brief Resumes media playback.
 *
 * This function resumes the media playback if it was paused.
 *
 * @param mh Pointer to the MEDIA structure.
 */
extern void media_resume(MEDIA *mh);

/**
 * @brief Stops media playback.
 *
 * This function stops the media playback and releases the media resources.
 *
 * @param mh Pointer to the MEDIA structure.
 */
extern void media_stop(MEDIA *mh);

/**
 * @brief Retrieves the current playback time of the media.
 *
 * This function returns the current playback time in milliseconds.
 *
 * @param mh Pointer to the MEDIA structure.
 * @return Current playback time in milliseconds. Returns -1 if the MEDIA structure is NULL.
 */
extern int64_t media_get_time(MEDIA *mh);

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
extern int64_t media_get_duration(MEDIA *mh);

/**
 * @brief Retrieves the current playback status of the media.
 *
 * This function returns the current playback status of the media.
 *
 * @param mh Pointer to the MEDIA structure.
 * @return Playback status code, one of MEDIA_STATUS_ERROR, MEDIA_STATUS_STOPPED,
 *         MEDIA_STATUS_PLAYING, MEDIA_STATUS_PAUSED, or MEDIA_STATUS_ENDED. Returns
 *         MEDIA_STATUS_ERROR if the MEDIA structure is NULL.
 */
extern int media_get_status(MEDIA *mh);

/**
 * @brief Retrieves the mute status of the media.
 *
 * This function returns the current mute status of the media.
 *
 * @param mh Pointer to the MEDIA structure.
 * @return Mute status, 1 if muted, 0 if not muted. Returns -1 if the MEDIA structure is NULL.
 */
extern int media_get_mute(MEDIA *mh);

/**
 * @brief Sets the mute status of the media.
 *
 * This function sets the mute status of the media.
 *
 * @param mh Pointer to the MEDIA structure.
 * @param status Mute status, 1 to mute, 0 to unmute.
 */
extern void media_set_mute(MEDIA *mh, int status);

/**
 * @brief Retrieves the volume level of the media.
 *
 * This function returns the current volume level of the media.
 *
 * @param mh Pointer to the MEDIA structure.
 * @return Volume level of the media. Returns -1 if the MEDIA structure is NULL.
 */
extern int media_get_volume(MEDIA *mh);

/**
 * @brief Sets the volume level of the media.
 *
 * This function sets the volume level of the media.
 *
 * @param mh Pointer to the MEDIA structure.
 * @param volume Volume level to set.
 * @return Volume level set. Returns -1 if the MEDIA structure is NULL.
 */
extern int media_set_volume(MEDIA *mh, int volume);

#endif
