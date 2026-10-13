#ifndef __BENNUGD_BLEND_H__
#define __BENNUGD_BLEND_H__

#include "bgddl.h"
#include "libbggfx.h"

#ifdef USE_SDL2
#include <SDL.h>

SDL_BlendMode blend_get_sdl2_blend_mode(
    BLENDMODE blend_mode,
    CUSTOM_BLENDMODE * custom_blendmode
);
#endif

#ifdef USE_SDL2_GPU
#include <SDL_gpu.h>

GPU_BlendPresetEnum blend_get_gpu_blend_mode(
    BLENDMODE blend_mode,
    CUSTOM_BLENDMODE * custom_blendmode,
    int * custom
);
#endif

#endif /* __BENNUGD_BLEND_H__ */