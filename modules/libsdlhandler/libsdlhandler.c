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

#include "bgddl.h"

#include <SDL.h>

#define MAX_EVENT_ID    0x1200
#define EVENT_MASK_BITS (MAX_EVENT_ID)
#define EVENT_MASK_BYTES ((EVENT_MASK_BITS + 7) >> 3)

static uint8_t sdl_event_mask[EVENT_MASK_BYTES] = {0};

/**
 * Enable a single SDL event type.
 */
void enableSDLEvent(Uint32 type) {
    if (type > 0 && type <= MAX_EVENT_ID) {
        sdl_event_mask[(type - 1) >> 3] |= (1 << ((type - 1) & 7));
    }
}

/**
 * Enable a range of SDL event types (inclusive).
 */
void enableSDLEventRange(Uint32 first, Uint32 last) {
    if (first == 0) first = 1;
    if (last > MAX_EVENT_ID) last = MAX_EVENT_ID;

    for (Uint32 type = first; type <= last; ++type) {
        sdl_event_mask[(type - 1) >> 3] |= (1 << ((type - 1) & 7));
    }
}

/**
 * SDL event filter function to be passed to SDL_SetEventFilter().
 */
static int libsdlhandler_event_filter(void *userdata, SDL_Event *event) {
    Uint32 type = event->type;
    if (type > 0 && type <= MAX_EVENT_ID) {
        return (sdl_event_mask[(type - 1) / 8] & (1 << ((type - 1) % 8))) != 0;
    }
    return SDL_FALSE;
}

/* ----------------------------------------------------------------- */
/* Public functions                                                  */

static void dump_new_events() {
    /* Remove all pendings events */
//    SDL_FlushEvents( SDL_FIRSTEVENT, SDL_LASTEVENT );

    /* Get new events */
    SDL_PumpEvents();
}

/* ----------------------------------------------------------------- */
/* Funciones de inicializacion del modulo/plugin                     */

void __bgdexport( libsdlhandler, module_initialize )() {
    if ( !SDL_WasInit( SDL_INIT_EVENTS ) ) SDL_InitSubSystem( SDL_INIT_EVENTS );
    SDL_SetEventFilter( libsdlhandler_event_filter, NULL );
}

/* ----------------------------------------------------------------- */

void __bgdexport( libsdlhandler, module_finalize )() {
    if ( SDL_WasInit( SDL_INIT_EVENTS ) ) SDL_QuitSubSystem( SDL_INIT_EVENTS );
}

/* ----------------------------------------------------------------- */

/* Bigest priority first execute
   Lowest priority last execute */

HOOK __bgdexport( libsdlhandler, handler_hooks )[] =
{
    { 5000, dump_new_events                   },
    {    0, NULL                              }
};

/* ----------------------------------------------------------------- */
