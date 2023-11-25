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

#ifndef __LIBMOD_SOUND_H
#define __LIBMOD_SOUND_H

/* --------------------------------------------------------------------------- */

#include <SDL.h>

#include "SDL_mixer.h"

enum {
    SOUND_MODE_MONO           = 1,
    SOUND_MODE_STEREO         = 2,
    SOUND_MODE_STEREO_2_1     = 3,
    SOUND_MODE_SURROUND_4_1   = 5,
    SOUND_MODE_SURROUND_5_1   = 6,
    SOUND_MODE_SURROUND_7_1   = 8
};

#define DEFAULT_SOUND_FREQ      44100
#define DEFAULT_SOUND_MODE      MODE_STEREO
#define DEFAULT_SOUND_CHANNELS  8               // Channels (not mono, stereo, etc.), these are sounds that can play simultaneously.

/* --------------------------------------------------------------------------- */

#endif
