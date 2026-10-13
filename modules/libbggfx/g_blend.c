#include "g_blend.h"

#ifdef USE_SDL2

SDL_BlendMode blend_get_sdl2_blend_mode(
    BLENDMODE blend_mode,
    CUSTOM_BLENDMODE * custom_blendmode
) {
    SDL_BlendMode b;

    if ( blend_mode == BLEND_CUSTOM ) {
        if ( custom_blendmode ) {
            b = SDL_ComposeCustomBlendMode(
                __Get_SDL_BlendFactor( custom_blendmode->src_rgb ),
                __Get_SDL_BlendFactor( custom_blendmode->dst_rgb ),
                __Get_SDL_BlendOperation( custom_blendmode->eq_rgb ),
                __Get_SDL_BlendFactor( custom_blendmode->src_alpha ),
                __Get_SDL_BlendFactor( custom_blendmode->dst_alpha ),
                __Get_SDL_BlendOperation( custom_blendmode->eq_alpha )
            );
        } else {
            b = SDL_BLENDMODE_BLEND;
        }

        return b;
    }

    switch ( blend_mode ) {
        case BLEND_NORMAL:
            b = SDL_ComposeCustomBlendMode(
                SDL_BLENDFACTOR_SRC_ALPHA,
                SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
                SDL_BLENDOPERATION_ADD,
                SDL_BLENDFACTOR_ONE,
                SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
                SDL_BLENDOPERATION_ADD
            );
            break;

        case BLEND_PREMULTIPLIED_ALPHA:
            b = SDL_ComposeCustomBlendMode(
                SDL_BLENDFACTOR_ONE,
                SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
                SDL_BLENDOPERATION_ADD,
                SDL_BLENDFACTOR_ONE,
                SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
                SDL_BLENDOPERATION_ADD
            );
            break;

        case BLEND_MULTIPLY:
            b = SDL_ComposeCustomBlendMode(
                SDL_BLENDFACTOR_DST_COLOR,
                SDL_BLENDFACTOR_ZERO,
                SDL_BLENDOPERATION_ADD,
                SDL_BLENDFACTOR_ZERO,
                SDL_BLENDFACTOR_ONE,
                SDL_BLENDOPERATION_ADD
            );
            break;

        case BLEND_ADD:
            b = SDL_ComposeCustomBlendMode(
                SDL_BLENDFACTOR_SRC_ALPHA,
                SDL_BLENDFACTOR_ONE,
                SDL_BLENDOPERATION_ADD,
                SDL_BLENDFACTOR_ONE,
                SDL_BLENDFACTOR_ONE,
                SDL_BLENDOPERATION_ADD
            );
            break;

        case BLEND_SUBTRACT:
            b = SDL_ComposeCustomBlendMode(
                SDL_BLENDFACTOR_ONE,
                SDL_BLENDFACTOR_ONE,
                SDL_BLENDOPERATION_SUBTRACT,
                SDL_BLENDFACTOR_ONE,
                SDL_BLENDFACTOR_ONE,
                SDL_BLENDOPERATION_SUBTRACT
            );
            break;

        case BLEND_MOD_ALPHA:
            b = SDL_ComposeCustomBlendMode(
                SDL_BLENDFACTOR_ZERO,
                SDL_BLENDFACTOR_ONE,
                SDL_BLENDOPERATION_ADD,
                SDL_BLENDFACTOR_ZERO,
                SDL_BLENDFACTOR_SRC_ALPHA,
                SDL_BLENDOPERATION_ADD
            );
            break;

        case BLEND_SET_ALPHA:
            b = SDL_ComposeCustomBlendMode(
                SDL_BLENDFACTOR_ZERO,
                SDL_BLENDFACTOR_ONE,
                SDL_BLENDOPERATION_ADD,
                SDL_BLENDFACTOR_ONE,
                SDL_BLENDFACTOR_ZERO,
                SDL_BLENDOPERATION_ADD
            );
            break;

        case BLEND_SET:
            b = SDL_ComposeCustomBlendMode(
                SDL_BLENDFACTOR_ONE,
                SDL_BLENDFACTOR_ZERO,
                SDL_BLENDOPERATION_ADD,
                SDL_BLENDFACTOR_ONE,
                SDL_BLENDFACTOR_ZERO,
                SDL_BLENDOPERATION_ADD
            );
            break;

        case BLEND_NORMAL_KEEP_ALPHA:
            b = SDL_ComposeCustomBlendMode(
                SDL_BLENDFACTOR_SRC_ALPHA,
                SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
                SDL_BLENDOPERATION_ADD,
                SDL_BLENDFACTOR_ZERO,
                SDL_BLENDFACTOR_ONE,
                SDL_BLENDOPERATION_ADD
            );
            break;

        case BLEND_NORMAL_ADD_ALPHA:
            b = SDL_ComposeCustomBlendMode(
                SDL_BLENDFACTOR_SRC_ALPHA,
                SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
                SDL_BLENDOPERATION_ADD,
                SDL_BLENDFACTOR_ONE,
                SDL_BLENDFACTOR_ONE,
                SDL_BLENDOPERATION_ADD
            );
            break;

        case BLEND_NORMAL_FACTOR_ALPHA:
            b = SDL_ComposeCustomBlendMode(
                SDL_BLENDFACTOR_SRC_ALPHA,
                SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
                SDL_BLENDOPERATION_ADD,
                SDL_BLENDFACTOR_ONE_MINUS_DST_ALPHA,
                SDL_BLENDFACTOR_ONE,
                SDL_BLENDOPERATION_ADD
            );
            break;

        case BLEND_ALPHA_MASK:
            b = SDL_ComposeCustomBlendMode(
                SDL_BLENDFACTOR_ZERO,
                SDL_BLENDFACTOR_ONE,
                SDL_BLENDOPERATION_ADD,
                SDL_BLENDFACTOR_ZERO,
                SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
                SDL_BLENDOPERATION_ADD
            );
            break;

        default:
            b = SDL_BLENDMODE_BLEND;
            break;
    }

    return b;
}

#endif /* USE_SDL2 */

#ifdef USE_SDL2_GPU

GPU_BlendPresetEnum blend_get_gpu_blend_mode(
    BLENDMODE blend_mode,
    CUSTOM_BLENDMODE * custom_blendmode,
    int * custom
) {
    *custom = 0;

    if ( blend_mode == BLEND_CUSTOM ) {
        *custom = 1;
        return GPU_BLEND_NORMAL;
    }

    if ( blend_mode == BLEND_ALPHA_MASK ) {
        *custom = 1;
        return GPU_BLEND_NORMAL;
    }

    switch ( blend_mode ) {
        case BLEND_NORMAL:
            return GPU_BLEND_NORMAL;

        case BLEND_PREMULTIPLIED_ALPHA:
            return GPU_BLEND_PREMULTIPLIED_ALPHA;

        case BLEND_MULTIPLY:
            return GPU_BLEND_MULTIPLY;

        case BLEND_ADD:
            return GPU_BLEND_ADD;

        case BLEND_SUBTRACT:
            return GPU_BLEND_SUBTRACT;

        case BLEND_MOD_ALPHA:
            return GPU_BLEND_MOD_ALPHA;

        case BLEND_SET_ALPHA:
            return GPU_BLEND_SET_ALPHA;

        case BLEND_SET:
            return GPU_BLEND_SET;

        case BLEND_NORMAL_KEEP_ALPHA:
            return GPU_BLEND_NORMAL_KEEP_ALPHA;

        case BLEND_NORMAL_ADD_ALPHA:
            return GPU_BLEND_NORMAL_ADD_ALPHA;

        case BLEND_NORMAL_FACTOR_ALPHA:
            return GPU_BLEND_NORMAL_FACTOR_ALPHA;

        default:
            return GPU_BLEND_NORMAL;
    }
}

#endif /* USE_SDL2_GPU */