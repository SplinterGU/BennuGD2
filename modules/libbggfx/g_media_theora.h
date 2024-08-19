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

/**
 * @brief Structure representing the playback context.
 *
 * This structure holds all the necessary information for managing playback,
 * including the decoder, audio, video, and playback state.
 */
typedef struct THR_ID {
    void *decoder;               /**< Decoder handle for media playback. */
    const void *video;           /**< Current video frame. */
    const void *audio;           /**< Current audio data. */
    SDL_Surface *shadow;         /**< SDL_Surface used for video rendering. */
    int has_audio;               /**< Flag indicating if audio is present. */
    int has_video;               /**< Flag indicating if video is present. */
    Uint32 framems;             /**< Frame duration in milliseconds. */
    int opened_audio;           /**< Flag indicating if the audio device is opened. */
    Uint32 baseticks;           /**< Base time for playback. */
    SDL_AudioDeviceID audio_devid; /**< ID of the audio device. */
    void *audio_queue;          /**< Queue for audio data. */
    void *audio_queue_tail;     /**< Tail of the audio queue. */
    int muted;                  /**< Flag indicating if audio is muted. */
    int volume;                 /**< Current volume level (0-128). */
    int paused;                 /**< Flag indicating if playback is paused. */
    Uint32 paused_ticks;        /**< Time when playback was paused. */
    unsigned int playms;        /**< Playback start time in milliseconds. */
} THR_ID;

/**
 * @brief Opens a media file for playback.
 *
 * This function initializes a playback context and begins decoding the media file.
 *
 * @param fname Path to the media file.
 * @param timeout Timeout value in milliseconds for opening the file.
 * @return Pointer to the initialized THR_ID structure, or NULL if the operation fails.
 */
extern THR_ID* thr_open(const char *fname, const uint32_t timeout);

/**
 * @brief Updates the playback context.
 *
 * This function processes the next frame of media, handles audio playback, and updates
 * the playback state.
 *
 * @param ctx Pointer to the THR_ID structure containing playback state.
 * @return 1 if a frame was processed, 0 otherwise.
 */
extern int thr_update(THR_ID *ctx);

/**
 * @brief Closes the playback context.
 *
 * This function releases all resources associated with the playback context,
 * including audio and video data, and stops decoding.
 *
 * @param ctx Pointer to the THR_ID structure containing playback state.
 * @return None.
 */
extern void thr_close(THR_ID *ctx);

/**
 * @brief Retrieves the size of the video.
 *
 * This function returns the width and height of the current video frame.
 *
 * @param ctx Pointer to the THR_ID structure containing playback state.
 * @param w Pointer to an integer to receive the width of the video.
 * @param h Pointer to an integer to receive the height of the video.
 * @return 0 on success, or -1 if an error occurs.
 */
extern int thr_get_video_size(THR_ID *ctx, int *w, int *h);

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
extern void thr_set_shadow_surface(THR_ID *ctx, SDL_Surface *shadow);

/**
 * @brief Retrieves the current mute status.
 *
 * This function returns the current mute status of the audio.
 *
 * @param ctx Pointer to the THR_ID structure containing playback state.
 * @return 1 if muted, 0 if not muted, or -1 if the context is NULL.
 */
extern int thr_get_mute(THR_ID *ctx);

/**
 * @brief Sets the mute status.
 *
 * This function sets the mute status of the audio.
 *
 * @param ctx Pointer to the THR_ID structure containing playback state.
 * @param status Mute status (non-zero for mute, zero for unmute).
 * @return None.
 */
extern void thr_set_mute(THR_ID *ctx, int status);

/**
 * @brief Retrieves the current volume level.
 *
 * This function returns the current volume level of the audio.
 *
 * @param ctx Pointer to the THR_ID structure containing playback state.
 * @return Volume level (0-128), or -1 if the context is NULL.
 */
extern int thr_get_volume(THR_ID *ctx);

/**
 * @brief Sets the volume level.
 *
 * This function sets the volume level of the audio.
 *
 * @param ctx Pointer to the THR_ID structure containing playback state.
 * @param volume New volume level (0-128).
 * @return Previous volume level, or -1 if the context is NULL or volume is out of range.
 */
extern int thr_set_volume(THR_ID *ctx, int volume);

/**
 * @brief Pauses or unpauses playback.
 *
 * This function sets the playback state to paused or playing.
 *
 * @param ctx Pointer to the THR_ID structure containing playback state.
 * @param action Action to perform (0 to unpause, 1 to pause).
 * @return None.
 */
extern void thr_pause(THR_ID *ctx, int action);

/**
 * @brief Retrieves the current playback state.
 *
 * This function returns the current state of playback.
 *
 * @param ctx Pointer to the THR_ID structure containing playback state.
 * @return Playback state (MEDIA_STATUS_ERROR, MEDIA_STATUS_ENDED, MEDIA_STATUS_PAUSED, or MEDIA_STATUS_PLAYING).
 */
extern int thr_get_state(THR_ID *ctx);

/**
 * @brief Retrieves the current playback time.
 *
 * This function returns the current playback time in milliseconds.
 *
 * @param ctx Pointer to the THR_ID structure containing playback state.
 * @return Playback time in milliseconds, or 0 if the context is NULL.
 */
extern int thr_get_time(THR_ID *ctx);

#endif
