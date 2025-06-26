/*
 *  Copyright (C) SplinterGU (Fenix/BennuGD) (Since 2006)
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

#ifdef __ANDROID__
#include "jni.h"
#include <android/log.h>
#define LOG_TAG "bgdi-native"
#endif

#include "bgddl.h"
#include "dlvaracc.h"

#include "libbggfx.h"
#include "libbginput.h"
#include "libsdlhandler.h"

#ifdef _MSC_VER
#pragma pack(push, 1)
#endif

#ifdef __GNUC__
#define __PACKED __attribute__ ((packed))
#pragma pack(1)
#else
#define __PACKED
#define inline __inline
#endif

typedef struct {
    int8_t active;
    int64_t touchId;
    int64_t fingerId;
    float x;                /**< Normalized in the range 0...1 */
    float y;                /**< Normalized in the range 0...1 */
    float pressure;         /**< Normalized in the range 0...1 */
} TouchPoint;

#ifdef __GNUC__
#pragma pack()
#endif

#ifdef _MSC_VER
#pragma pack(pop)
#endif

/* --------------------------------------------------------------------------- */

static TouchPoint *touches = NULL;

/* --------------------------------------------------------------------------- */

/*
 *  FUNCTION : touch_events
 *
 *  Process all pending SDL events, updating all global variables
 *  and handling debug key presses
 *  and cannot be changed
 *
 *  PARAMS :
 *      None
 *
 *  RETURN VALUE :
 *      None
 */

void process_touch_events() {
#ifdef __ANDROID__
    SDL_Event e;

    /* NOTE: TOUCH ONLY IN ANDROID AT THIS MOMENT !!!*/
    /* Touch events */
    while ( SDL_PeepEvents( &e, 1, SDL_GETEVENT, SDL_FINGERDOWN, SDL_FINGERCANCEL ) > 0 ) {
        switch ( e.type ) {
            case SDL_FINGERDOWN: {
                if ( e.tfinger.touchId == SDL_MOUSE_TOUCHID) break;
                int64_t id = (int64_t) e.tfinger.fingerId;
                int free_index = -1;
                for (int i = 0; i < MAX_TOUCHES; i++) {
                    if (!touches[i].active && free_index == -1) free_index = i;
                    if (touches[i].fingerId == id) {
                        free_index = i;
                        break;
                    }
                }
                if (free_index != -1) {
                    touches[free_index].active = 1;
                    touches[free_index].touchId = e.tfinger.touchId;
                    touches[free_index].fingerId = id;
                    touches[free_index].x = ( e.tfinger.x * renderer_width - renderer_offset_x ) / renderer_scale_factor_width;
                    touches[free_index].y = ( e.tfinger.y * renderer_height - renderer_offset_y ) / renderer_scale_factor_height;
                    touches[free_index].pressure = e.tfinger.pressure;
                }
                break;
            }

            case SDL_FINGERMOTION: {
                if ( e.tfinger.touchId == SDL_MOUSE_TOUCHID) break;
                int64_t id = (int64_t) e.tfinger.fingerId;
                for (int i = 0; i < MAX_TOUCHES; i++) {
                    if (touches[i].active && touches[i].fingerId == id) {
                        touches[i].x = ( e.tfinger.x * renderer_width - renderer_offset_x ) / renderer_scale_factor_width;
                        touches[i].y = ( e.tfinger.y * renderer_height - renderer_offset_y ) / renderer_scale_factor_height;
                        touches[i].pressure = e.tfinger.pressure;
                        break;
                    }
                }
                break;
            }

            case SDL_FINGERUP: {
                if ( e.tfinger.touchId == SDL_MOUSE_TOUCHID) break;
                int64_t id = (int64_t) e.tfinger.fingerId;
                for (int i = 0; i < MAX_TOUCHES; i++) {
                    if (touches[i].active && touches[i].fingerId == id) {
                        touches[i].active = 0;
                        touches[i].touchId = -1;
                        touches[i].fingerId = -1;
                        touches[i].pressure = 0;
                        break;
                    }
                }
                break;
            }

            case SDL_FINGERCANCEL: {
                if ( e.tfinger.touchId == SDL_MOUSE_TOUCHID) break;
                SDL_TouchID id = e.tfinger.touchId;
                for (int i = 0; i < MAX_TOUCHES; i++) {
                    if (touches[i].touchId == id) {
                        touches[i].active = 0;
                        touches[i].touchId = -1;
                        touches[i].fingerId = -1;
                        touches[i].pressure = 0;
                    }
                }
                break;
            }
        }
    }
#endif
}

/* --------------------------------------------------------------------------- */

void touch_init() {
#ifdef __ANDROID__
    if ( SDL_GetNumTouchDevices() > 0 ) {
        touches = (TouchPoint *)GLOADDR( libbginput, TOUCHES );
        if (touches) enableSDLEventRange(SDL_FINGERDOWN, SDL_FINGERCANCEL);
        //SDL_SetHint(SDL_HINT_MOUSE_TOUCH_EVENTS, "0");
        //SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "0");
    }
#endif
}

/* --------------------------------------------------------------------------- */
