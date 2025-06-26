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

#include "bgddl.h"
#include "dlvaracc.h"

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
    int8_t type;
    int64_t instance_id;
    char name[64];
    float values[MAX_SENSORS_VALUES];   /**< Depending on type: acc, gyro, etc. Max MAX_SENSORS_VALUES floats */
} SensorData;

#ifdef __GNUC__
#pragma pack()
#endif

#ifdef _MSC_VER
#pragma pack(pop)
#endif

/* --------------------------------------------------------------------------- */

static SDL_Sensor* sdl_sensors[MAX_SENSORS] = { 0 };
static SensorData *sensors = NULL;

/* --------------------------------------------------------------------------- */

/*
 *  FUNCTION : process_sensors_events
 *
 *  Process SDL sensor events and update global sensor array
 *
 *  PARAMS :
 *      None
 *
 *  RETURN VALUE :
 *      None
 */

void process_sensors_events() {
    SDL_Event e;

    while ( SDL_PeepEvents( &e, 1, SDL_GETEVENT, SDL_SENSORUPDATE, SDL_SENSORUPDATE ) > 0 ) {
        if ( e.type == SDL_SENSORUPDATE ) {
            SDL_SensorID id = e.sensor.which;
            for (int i = 0; i < MAX_SENSORS; i++) {
                if (sensors[i].active && sensors[i].instance_id == id) {
                    for (int j = 0; j < MAX_SENSORS_VALUES; j++) {
                        sensors[i].values[j] = e.sensor.data[j];
                    }
                    break;
                }
            }
        }
    }
}

/* --------------------------------------------------------------------------- */

/*
 *  FUNCTION : sensors_init
 *
 *  Initialize sensors and enable SDL sensor events
 *
 *  PARAMS :
 *      None
 *
 *  RETURN VALUE :
 *      None
 */

void sensors_init() {
    sensors = (SensorData *)GLOADDR(libbginput, SENSORS);
    if (!sensors) return;

    if ( !SDL_WasInit( SDL_INIT_SENSOR ) ) SDL_InitSubSystem( SDL_INIT_SENSOR );

    enableSDLEventRange(SDL_SENSORUPDATE, SDL_SENSORUPDATE);

    SDL_SetHint(SDL_HINT_ACCELEROMETER_AS_JOYSTICK, "0");
    SDL_SensorUpdate();

    int count = SDL_NumSensors();
    SDL_Log("SDL_NumSensors %d", count);
    for (int i = 0; i < count && i < MAX_SENSORS; i++) {
        SDL_Sensor* s = SDL_SensorOpen(i);
        if (s) {
            sdl_sensors[i] = s;
            SensorData* sd = &sensors[i];
            sd->active = 1;
            sd->type = SDL_SensorGetType(s);
            sd->instance_id = SDL_SensorGetInstanceID(s);
            const char * name = SDL_SensorGetName(s);
            strncpy(sd->name, name, sizeof(sd->name) - 1);
            sd->name[sizeof(sd->name) - 1] = '\0';
            SDL_SensorGetData(s, sd->values, MAX_SENSORS_VALUES);
        }
    }
}

/* --------------------------------------------------------------------------- */

/*
 *  FUNCTION : sensors_exit
 *
 *  Close all opened sensors
 *
 *  PARAMS :
 *      None
 *
 *  RETURN VALUE :
 *      None
 */

void sensors_exit() {
    for (int i = 0; i < MAX_SENSORS; i++) {
        if (sdl_sensors[i]) {
            SDL_SensorClose(sdl_sensors[i]);
            sdl_sensors[i] = NULL;
        }
        if (sensors) {
            sensors[i].active = 0;
        }
    }
}

/* --------------------------------------------------------------------------- */
