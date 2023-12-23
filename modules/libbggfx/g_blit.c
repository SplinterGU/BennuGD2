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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fmath.h>

/* --------------------------------------------------------------------------- */

#include "bgddl.h"
#include "libbggfx.h"
#include "dlvaracc.h"

// #define __DISABLE_PALETTES__

/* --------------------------------------------------------------------------- */

#ifdef USE_SDL2_GPU
    GPU_FilterEnum gr_filter_mode = GPU_FILTER_NEAREST;
#endif

/* --------------------------------------------------------------------------- */

#ifdef USE_SDL2

static inline int gr_update_texture( GRAPH * gr ) {
    SDL_Surface * surface;

#ifndef __DISABLE_PALETTES__
    if ( gr->surface->format->format == gPixelFormat->format ) {
        surface = gr->surface;
    } else {
        surface = SDL_ConvertSurfaceFormat( gr->surface, gPixelFormat->format, 0 );
        if ( !surface ) return -1;
    }
#else
    surface = gr->tex;
#endif

    if ( SDL_MUSTLOCK( surface ) ) {
        SDL_LockSurface( surface );
        SDL_UpdateTexture( gr->tex, NULL, surface->pixels, surface->pitch );
        SDL_UnlockSurface( surface );
    } else {
        SDL_UpdateTexture( gr->tex, NULL, surface->pixels, surface->pitch );
    }

#ifndef __DISABLE_PALETTES__
    if ( surface != gr->surface ) SDL_FreeSurface( surface );
#endif

    gr->texture_must_update = 0;

    return 0;
}

/* --------------------------------------------------------------------------- */

SDL_BlendFactor __Get_SDL_BlendFactor( int64_t factor ) {
    switch( factor ) {
        case    0:  // GL_ZERO
            return SDL_BLENDFACTOR_ZERO;
            break;

        case    1:  // GL_ONE
            return SDL_BLENDFACTOR_ONE;
            break;

        case    0x0300: // GL_SRC_COLOR
            return SDL_BLENDFACTOR_SRC_COLOR;
            break;

        case    0x0301: // GL_ONE_MINUS_SRC_COLOR
            return SDL_BLENDFACTOR_ONE_MINUS_SRC_COLOR;
            break;

        case    0x0302: // GL_SRC_ALPHA
            return SDL_BLENDFACTOR_SRC_ALPHA;
            break;

        case    0x0303: // GL_ONE_MINUS_SRC_ALPHA
            return SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
            break;

        case    0x0304: // GL_DST_ALPHA
            return SDL_BLENDFACTOR_DST_ALPHA;
            break;

        case    0x0305: // GL_ONE_MINUS_DST_ALPHA
            return SDL_BLENDFACTOR_ONE_MINUS_DST_ALPHA;
            break;

        case    0x0306: // GL_DST_COLOR
            return SDL_BLENDFACTOR_DST_COLOR;
            break;

        case    0x0307: // GL_ONE_MINUS_DST_COLOR
            return SDL_BLENDFACTOR_ONE_MINUS_DST_COLOR;
            break;

        case    0x0308: // GL_SRC_ALPHA_SATURATE
        case    0x8001: // GL_CONSTANT_COLOR
        case    0x8002: // GL_ONE_MINUS_CONSTANT_COLOR
        case    0x8003: // GL_CONSTANT_ALPHA
        case    0x8004: // GL_ONE_MINUS_CONSTANT_ALPHA
            break;
    }

    return SDL_BLENDMODE_NONE;

}

/* --------------------------------------------------------------------------- */

SDL_BlendOperation __Get_SDL_BlendOperation( int64_t operation ) {
    switch( operation ) {
        case 0x8006: // GL_FUNC_ADD
            return SDL_BLENDOPERATION_ADD;
            break;

        case 0x8007: // GL_MIN
            return SDL_BLENDOPERATION_MINIMUM;
            break;

        case 0x8008: // GL_MAX
            return SDL_BLENDOPERATION_MAXIMUM;
            break;

        case 0x800A: // GL_FUNC_SUBTRACT
            return SDL_BLENDOPERATION_SUBTRACT;
            break;

        case 0x800B: // GL_FUNC_REVERSE_SUBTRACT
            return SDL_BLENDOPERATION_REV_SUBTRACT;
            break;
    }

    return SDL_BLENDOPERATION_ADD;
}

#endif

/* --------------------------------------------------------------------------- */

int gr_create_image_for_graph( GRAPH * gr ) {
    if ( !gr ) return 1;
#ifdef USE_SDL2
    if ( !gr->surface ) {
        gr->surface = SDL_CreateRGBSurface( 0, gr->width, gr->height, gPixelFormat->BitsPerPixel, gPixelFormat->Rmask, gPixelFormat->Gmask, gPixelFormat->Bmask, gPixelFormat->Amask );
        if ( !gr->surface ) return 1;
    }
#endif
#ifdef USE_SDL2_GPU
    if ( !gr->tex ) gr->tex = GPU_CreateImage( gr->width, gr->height, GPU_FORMAT_RGBA );
    if ( !gr->tex ) {
        printf ("error in image creation\n" );
        return 1;
    }
    if ( !gr->tex->target ) GPU_LoadTarget( gr->tex );
#endif
    return 0;
}

/* --------------------------------------------------------------------------- */

void gr_set_blend( 
#ifdef USE_SDL2
        SDL_Texture * tex
#endif
#ifdef USE_SDL2_GPU
        GPU_Image * tex
#endif
    , BLENDMODE blend_mode, CUSTOM_BLENDMODE * custom_blendmode ) {
#ifdef USE_SDL2
    if ( blend_mode == BLEND_DISABLED || blend_mode == BLEND_NONE ) {
        SDL_SetTextureBlendMode( tex, SDL_BLENDMODE_NONE );
    } else {
        SDL_BlendMode b;

        if ( blend_mode == BLEND_CUSTOM ) { // custom
            if ( custom_blendmode ) {
                CUSTOM_BLENDMODE cbm_sdl_format;

                b = SDL_ComposeCustomBlendMode(
                        __Get_SDL_BlendFactor( custom_blendmode->src_rgb ),
                        __Get_SDL_BlendFactor( custom_blendmode->dst_rgb ),
                        __Get_SDL_BlendOperation( custom_blendmode->eq_rgb ),
                        __Get_SDL_BlendFactor( custom_blendmode->src_alpha ),
                        __Get_SDL_BlendFactor( custom_blendmode->dst_alpha ),
                        __Get_SDL_BlendOperation( custom_blendmode->eq_alpha )
                    );
            }
        } else {
            switch( blend_mode ) {
                case BLEND_NORMAL:
                    b = SDL_ComposeCustomBlendMode( SDL_BLENDFACTOR_SRC_ALPHA, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_SRC_ALPHA, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDOPERATION_ADD );
                    break;

                case BLEND_PREMULTIPLIED_ALPHA:
                    b = SDL_ComposeCustomBlendMode( SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDOPERATION_ADD );
                    break;

                case BLEND_MULTIPLY:
                    b = SDL_ComposeCustomBlendMode( SDL_BLENDFACTOR_DST_COLOR, SDL_BLENDFACTOR_ZERO, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_SRC_ALPHA, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDOPERATION_ADD );
                    break;

                case BLEND_ADD:
                    b = SDL_ComposeCustomBlendMode( SDL_BLENDFACTOR_SRC_ALPHA, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_SRC_ALPHA, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD );
                    break;

                case BLEND_SUBTRACT:
                    // FIXME: Use src alpha for source components?
                    b = SDL_ComposeCustomBlendMode( SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_SUBTRACT, SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_SUBTRACT );
// maybe we must try this -> b = SDL_ComposeCustomBlendMode( SDL_BLENDFACTOR_SRC_ALPHA, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_SUBTRACT, SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_SUBTRACT );
                    break;

                case BLEND_MOD_ALPHA:
                    // Don't disturb the colors, but multiply the dest alpha by the src alpha
                    b = SDL_ComposeCustomBlendMode( SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_SRC_ALPHA, SDL_BLENDOPERATION_ADD );
                    break;

                case BLEND_SET_ALPHA:
                    // Don't disturb the colors, but set the alpha to the src alpha
                    b = SDL_ComposeCustomBlendMode( SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ZERO, SDL_BLENDOPERATION_ADD );
                    break;

                case BLEND_SET:
                    b = SDL_ComposeCustomBlendMode( SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ZERO, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ZERO, SDL_BLENDOPERATION_ADD );
                    break;

                case BLEND_NORMAL_KEEP_ALPHA:
                    b = SDL_ComposeCustomBlendMode( SDL_BLENDFACTOR_SRC_ALPHA, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD );
                    break;

                case BLEND_NORMAL_ADD_ALPHA:
                    b = SDL_ComposeCustomBlendMode( SDL_BLENDFACTOR_SRC_ALPHA, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD );
                    break;

                case BLEND_NORMAL_FACTOR_ALPHA:
                    b = SDL_ComposeCustomBlendMode( SDL_BLENDFACTOR_SRC_ALPHA, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ONE_MINUS_DST_ALPHA, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD );
                    break;

                case BLEND_ALPHA_MASK:
                    b = SDL_ComposeCustomBlendMode( SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDOPERATION_ADD );
                    break;
            }
        }
        SDL_SetTextureBlendMode( tex, b );
    }
#endif
#ifdef USE_SDL2_GPU
    if ( blend_mode == BLEND_DISABLED || blend_mode == BLEND_NONE ) {
        GPU_SetBlending( tex, GPU_FALSE );
    } else {
        GPU_SetBlending( tex, GPU_TRUE );
        if ( blend_mode == BLEND_CUSTOM ) { // custom
            if ( custom_blendmode ) {
                GPU_SetBlendFunction( tex, custom_blendmode->src_rgb, custom_blendmode->dst_rgb, custom_blendmode->src_alpha, custom_blendmode->dst_alpha );
                GPU_SetBlendEquation( tex, custom_blendmode->eq_rgb, custom_blendmode->eq_alpha );
            }
        } else {
            GPU_BlendPresetEnum b;

            switch( blend_mode ) {
                case BLEND_NORMAL:
                    b = GPU_BLEND_NORMAL;
                    break;

                case BLEND_PREMULTIPLIED_ALPHA:
                    b = GPU_BLEND_PREMULTIPLIED_ALPHA;
                    break;

                case BLEND_MULTIPLY:
                    b = GPU_BLEND_MULTIPLY;
                    break;

                case BLEND_ADD:
                    b = GPU_BLEND_ADD;
                    break;

                case BLEND_SUBTRACT:
/*
maybe we must do a custom and try this
                GPU_SetBlendFunction( tex, 0x0302, 1 , 0, 1 ); // GL_SRC_ALPHA, GL_ONE, GL_ZERO, GL_ONE
                GPU_SetBlendEquation( tex, 0x800A, 0x800A ); // GL_FUNC_SUBTRACT, GL_FUNC_SUBTRACT
*/
                    b = GPU_BLEND_SUBTRACT;
                    break;

                case BLEND_MOD_ALPHA:
                    b = GPU_BLEND_MOD_ALPHA;
                    break;

                case BLEND_SET_ALPHA:
                    b = GPU_BLEND_SET_ALPHA;
                    break;

                case BLEND_SET:
                    b = GPU_BLEND_SET;
                    break;

                case BLEND_NORMAL_KEEP_ALPHA:
                    b = GPU_BLEND_NORMAL_KEEP_ALPHA;
                    break;

                case BLEND_NORMAL_ADD_ALPHA:
                    b = GPU_BLEND_NORMAL_ADD_ALPHA;
                    break;

                case BLEND_NORMAL_FACTOR_ALPHA:
                    b = GPU_BLEND_NORMAL_FACTOR_ALPHA;
                    break;

                case BLEND_ALPHA_MASK:
                    GPU_SetBlendFunction( tex, 0, 1, 0, 0x0303 ); // GL_ZERO, GL_ONE, GL_ZERO, GL_ONE_MINUS_SRC_ALPHA
                    GPU_SetBlendEquation( tex, 0x8006, 0x8006 ); // GL_FUNC_ADD, GL_FUNC_ADD
                    break;

            }

            if ( blend_mode != BLEND_ALPHA_MASK ) GPU_SetBlendMode( tex, b );
        }
    }
#endif
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : gr_prepare_renderer
 *
 *  Setup common parameters for render a object
 *
 *  PARAMS :
 *      dest                Destination bitmap or NULL for screen
 *      clip                Clipping region or NULL for the whole screen
 *      flags               Flags
 *      blend_mode          In/Ouput BLENDMODE
 *
 *  RETURN VALUE :
 *      None
 *
 */

int gr_prepare_renderer( GRAPH * dest, REGION * clip, int64_t flags, BLENDMODE * blend_mode ) {

    if ( dest ) {
        if ( gr_create_image_for_graph( dest ) ) return 1;
#ifdef USE_SDL2
        if ( !dest->tex ) {
            dest->tex = SDL_CreateTexture( gRenderer, gPixelFormat->format, SDL_TEXTUREACCESS_TARGET, dest->width, dest->height );
            if ( !dest->tex ) {
                printf ("error in RW texture creation [%s]\n", SDL_GetError() );
                return 1;
            }

            gr_update_texture(dest);

            dest->type = BITMAP_TEXTURE_TARGET;
        }
        else if ( dest->type != BITMAP_TEXTURE_TARGET ) return 1;
#endif
    }

    if ( *blend_mode == BLEND_DISABLED || *blend_mode == BLEND_NONE ) {
             if ( flags & B_NOCOLORKEY )    *blend_mode = BLEND_DISABLED;       //Disable
        else if ( flags & B_ABLEND     )    *blend_mode = BLEND_ADD;            //Additive
        else if ( flags & B_SBLEND     )    *blend_mode = BLEND_SUBTRACT;       //Substract
        else                                *blend_mode = BLEND_NORMAL;         //Enable blending on texture
    }

#ifdef USE_SDL2
    SDL_Rect rect;

    if ( clip ) {
        rect.x = clip->x;
        rect.y = clip->y;
        rect.w = clip->x2 - clip->x + 1;
        rect.h = clip->y2 - clip->y + 1;
    } else {
        rect.x = 0;
        rect.y = 0;
        if ( dest ) {
            rect.w = dest->width;
            rect.h = dest->height;
        } else {
            rect.w = scr_width;
            rect.h = scr_height;
        }
    }

    if ( dest ) SDL_SetRenderTarget( gRenderer, dest->tex );
//    if ( dest ) { SDL_SetRenderTarget( gRenderer, dest->tex ); SDL_SetTextureBlendMode( dest->tex, SDL_BLENDMODE_NONE ); }

    SDL_RenderSetClipRect( gRenderer, &rect );
#endif
#ifdef USE_SDL2_GPU
    static REGION _lastClip = { 0, 0, 0, 0 }, * lastClip = NULL;
    static GRAPH * lastDest = NULL;

    int doClip = 0, doUnClip = 0;

    if ( clip && ( dest != lastDest || !lastClip || lastClip->x != clip->x || lastClip->y != clip->y || lastClip->x2 != clip->x2 || lastClip->y2 != clip->y2 ) ) doClip = 1;
    if ( dest != lastDest && !clip ) doUnClip = 1;
    if ( dest == lastDest && !clip && clip != lastClip ) doUnClip = 1;

    GPU_Target * dst = dest ? dest->tex->target : gRenderer;

    if ( doUnClip ) {
        GPU_UnsetClip( dst );
        lastClip = NULL;
        lastDest = dest;
    }

    if ( doClip ) {
        GPU_SetClip( dst, clip->x, clip->y, clip->x2 - clip->x + 1, clip->y2 - clip->y + 1 );
        _lastClip = *clip;
        lastClip = &_lastClip;
        lastDest = dest;
    }
#endif

    return 0;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : gr_blit
 *
 *  Draw a rotated and/or scaled bitmap
 *
 *  PARAMS :
 *      dest            Destination bitmap or NULL for screen
 *      clip            Clipping region or NULL for the whole screen
 *      scrx, scry      Pixel coordinates of the center on screen
 *      angle           Angle of rotation in miliangles
 *      scalex, scaley  Scaling ratio in percentaje (100 for original size)
 *      gr              Pointer to the graphic object to draw
 *      gr_clip         Source clip
 *
 *  RETURN VALUE :
 *      None
 *
 */

void gr_blit(   GRAPH * dest,
                REGION * clip,
                double scrx,
                double scry,
                int64_t flags,
                int64_t angle,
                double scalex,
                double scaley,
                double centerx,
                double centery,
                GRAPH * gr,
                BGD_Rect * gr_clip,
                uint8_t alpha,
                uint8_t color_r,
                uint8_t color_g,
                uint8_t color_b,
                BLENDMODE blend_mode,
                CUSTOM_BLENDMODE * custom_blendmode
            ) {
    if ( !gr ) return;

#ifdef USE_SDL2
    if ( !gr->surface ) return;
#endif

    if ( scalex <= 0.0 || scaley <= 0.0 ) return;

    /* Create segments if needed */

    if ( !gr->tex && !gr->segments ) {
        if ( gr->width > gMaxTextureSize || gr->height > gMaxTextureSize ) {
            int64_t nsegx = ( gr->width - 1 ) / gMaxTextureSize + 1,
                    nsegy = ( gr->height - 1 ) / gMaxTextureSize + 1;
            int64_t x, y, offx, offy, seg, w, h;
            int64_t total = nsegx * nsegy;

#define TEX_SEGMENTS 1
#ifdef USE_SDL2
    #undef TEX_SEGMENTS
    #define TEX_SEGMENTS 4
#endif
            gr->segments = calloc( total * TEX_SEGMENTS, sizeof( *( gr->segments ) ) );

            if ( !gr->segments ) return;
            gr->nsegments = total;

            SDL_Surface *auxSurface = NULL;
            int64_t oldw = -1, oldh = -1;

            SDL_Rect srcrect = {0};

            for ( y = 0; y < nsegy; y++ ) {
                offy = y * gMaxTextureSize;
                h = ( y + 1 == nsegy ) ? 1 + gr->height % gMaxTextureSize : gMaxTextureSize;
                for ( x = 0; x < nsegx; x++ ) {
                    seg = y * nsegx + x;
                    offx = x * gMaxTextureSize;
                    w = ( x + 1 == nsegx ) ? 1 + gr->width % gMaxTextureSize : gMaxTextureSize;
                    gr->segments[seg].offx = offx;
                    gr->segments[seg].offy = offy;
                    gr->segments[seg].width = w;
                    gr->segments[seg].height = h;

                    srcrect.x = offx;
                    srcrect.y = offy;
                    srcrect.w = w;
                    srcrect.h = h;
#ifdef USE_SDL2
                    gr->segments[seg].tex = SDL_CreateTexture( gRenderer, gPixelFormat->format, SDL_TEXTUREACCESS_STATIC, w, h );
                    if ( !gr->segments[seg].tex ) {
                        printf ("error creando multi textura RO [%s]\n", SDL_GetError() );
                        return;
                    }
#endif
                    if ( oldw != w || oldh != h ) {
                        if ( auxSurface ) SDL_FreeSurface( auxSurface );
                        auxSurface = SDL_CreateRGBSurface( 0, w, h, gPixelFormat->BitsPerPixel, gPixelFormat->Rmask, gPixelFormat->Gmask, gPixelFormat->Bmask, gPixelFormat->Amask );
                        if ( !auxSurface ) {
                            printf ("error creando temp surface [%s]\n", SDL_GetError() );
                            return;
                        }
                        oldw = w; oldh = h;
                    }
                    SDL_BlitSurface( gr->surface, &srcrect, auxSurface, NULL );
#ifdef USE_SDL2
                    SDL_UpdateTexture( gr->segments[seg].tex, NULL, auxSurface->pixels, auxSurface->pitch );
#endif
#ifdef USE_SDL2_GPU
                    gr->segments[seg].tex = GPU_CopyImageFromSurface( auxSurface );
                    if ( !gr->segments[seg].tex ) {
                        SDL_FreeSurface( auxSurface );
                        printf ("error creando multi textura RO [%s]\n", SDL_GetError() );
                        return;
                    }
#endif
                }
            }
            if ( auxSurface ) SDL_FreeSurface( auxSurface );

#ifdef USE_SDL2
            /* calc mirror texture */
            int64_t offy1 = 0, offy2 = 0, offy3 = 0;
            int64_t seg0, seg1, seg2, seg3;
            for ( y = 0; y < nsegy; y++ ) {
                int64_t offx1 = 0, offx2 = 0, offx3 = 0;
                for ( x = 0; x < nsegx; x++ ) {
                    seg0 = y * nsegx + x;
                    seg1 = y * nsegx + nsegx - 1 - x;
                    seg2 = ( nsegy - 1 - y ) * nsegx + x;
                    seg3 = ( nsegy - 1 - y ) * nsegx + nsegx - 1 - x;

                    gr->segments[seg0 + gr->nsegments * 1] = gr->segments[seg1];
                    gr->segments[seg0 + gr->nsegments * 2] = gr->segments[seg2];
                    gr->segments[seg0 + gr->nsegments * 3] = gr->segments[seg3];

                    gr->segments[seg0 + gr->nsegments * 1].offx = offx1;
                    gr->segments[seg0 + gr->nsegments * 2].offx = offx2;
                    gr->segments[seg0 + gr->nsegments * 3].offx = offx3;

                    gr->segments[seg0 + gr->nsegments * 1].offy = offy1;
                    gr->segments[seg0 + gr->nsegments * 2].offy = offy2;
                    gr->segments[seg0 + gr->nsegments * 3].offy = offy3;

                    offx1 += gr->segments[seg1].width;
                    offx2 += gr->segments[seg2].width;
                    offx3 += gr->segments[seg3].width;
                }
                offy1 += gr->segments[seg1].height;
                offy2 += gr->segments[seg2].height;
                offy3 += gr->segments[seg3].height;
            }
#endif
        }
#ifdef USE_SDL2
        else {
            gr->tex = SDL_CreateTexture( gRenderer, gPixelFormat->format, SDL_TEXTUREACCESS_STATIC, gr->width, gr->height );
            if ( !gr->tex ) {
                printf ("error creando textura RO [%s]\n", SDL_GetError() );
                return;
            }

            gr_update_texture(gr);
        }
        gr->type = BITMAP_TEXTURE_STATIC;
#endif
    }

#ifdef USE_SDL2
    if ( gr->tex && gr->texture_must_update ) gr_update_texture(gr);
#endif

#ifdef USE_SDL2_GPU
    if ( !gr->tex && !gr->segments ) return;
#endif

    /* blit */

    if ( gr_prepare_renderer( dest, clip, flags, &blend_mode ) ) return;

#ifdef USE_SDL2
    SDL_Point center;
#endif
    int64_t w = 0, h = 0;

    if ( gr_clip ) {
        w = gr_clip->w;
        h = gr_clip->h;
    } else {
        w = gr->width;
        h = gr->height;
    }

    SDL_Rect dstrect;
    double scalex_adjusted, scaley_adjusted;
    int i;

    if ( flags & B_TRANSLUCENT ) alpha >>= 1;

    scalex_adjusted = scalex / 100.0;
    scaley_adjusted = scaley / 100.0;

#ifdef USE_SDL2
    SDL_RendererFlip flip = ( ( flags & B_HMIRROR ) ? SDL_FLIP_HORIZONTAL : 0 ) | ( ( flags & B_VMIRROR ) ? SDL_FLIP_VERTICAL : 0 );
#endif

    if ( centerx == POINT_UNDEFINED || centery == POINT_UNDEFINED ) {
        if ( gr->ncpoints && gr->cpoints[0].x != CPOINT_UNDEFINED ) {
            centerx = gr->cpoints[0].x;
            centery = gr->cpoints[0].y;
        } else {
            centerx = w / 2.0;
            centery = h / 2.0;
        }
    }

    if ( flags & B_HMIRROR ) {
        angle = -angle;
#ifdef USE_SDL2
        centerx = gr->width - 1 - centerx;
#endif
#ifdef USE_SDL2_GPU
        scalex_adjusted = -scalex_adjusted;
#endif
    }
    if ( flags & B_VMIRROR ) {
        angle = -angle;
#ifdef USE_SDL2
        centery = gr->height - 1 - centery;
#endif
#ifdef USE_SDL2_GPU
        scaley_adjusted = -scaley_adjusted;
#endif
    }

#ifdef USE_SDL2
    centerx *= scalex_adjusted;
    centery *= scaley_adjusted;
#endif

    if ( gr->segments ) {
#ifdef USE_SDL2
        SDL_Texture * tex;
#endif
#ifdef USE_SDL2_GPU
        GPU_Image * tex;
#endif
        int mirror_offset = 0;

#ifdef USE_SDL2
        if ( flags & B_HMIRROR ) mirror_offset += gr->nsegments;
        if ( flags & B_VMIRROR ) mirror_offset += gr->nsegments * 2;
#endif

        for ( i = mirror_offset; i < mirror_offset + gr->nsegments; i++ ) {
            double  offx = gr->segments[ i ].offx,
                    offy = gr->segments[ i ].offy;

            tex = gr->segments[ i ].tex;
#ifdef USE_SDL2
            offx *= scalex_adjusted,
            offy *= scaley_adjusted;

            center.x = centerx - offx;
            center.y = centery - offy;

            dstrect.x = scrx - centerx + offx;
            dstrect.y = scry - centery + offy;
            dstrect.w = 1 + scalex_adjusted * gr->segments[ i ].width; // Calculate dest region according texture
            dstrect.h = 1 + scaley_adjusted * gr->segments[ i ].height; // Calculate dest region according texture

            gr_set_blend( tex, blend_mode, custom_blendmode );
            SDL_SetTextureAlphaMod( tex, alpha );
            SDL_SetTextureColorMod( tex, color_r, color_g, color_b );

            //Render
            SDL_RenderCopyEx( gRenderer, tex, gr_clip, &dstrect, angle / -1000.0, &center, flip );
#endif
#ifdef USE_SDL2_GPU
            gr_set_blend( tex, blend_mode, custom_blendmode );
            GPU_SetRGBA( tex, color_r, color_g, color_b, alpha );

            if ( tex->filter_mode != gr_filter_mode ) GPU_SetImageFilter( tex, gr_filter_mode );

            GPU_Target * dst = dest ? dest->tex->target : gRenderer;

            GPU_BlitTransformX( tex, gr_clip, dst, ( float ) scrx, ( float ) scry, ( float ) centerx - offx, ( float ) centery - offy, ( float ) angle / -1000.0, scalex_adjusted, scaley_adjusted );
#endif
        }
    } else {
#ifdef USE_SDL2
        center.x = centerx;
        center.y = centery;

        dstrect.x = scrx - center.x;
        dstrect.y = scry - center.y;
        dstrect.w = scalex_adjusted * w;
        dstrect.h = scaley_adjusted * h;

        gr_set_blend( gr->tex, blend_mode, custom_blendmode );
        SDL_SetTextureAlphaMod( gr->tex, alpha );
        SDL_SetTextureColorMod( gr->tex, color_r, color_g, color_b );

        //Render
        SDL_RenderCopyEx( gRenderer, gr->tex, gr_clip, &dstrect, angle / -1000.0, &center, flip );
#endif
#ifdef USE_SDL2_GPU
        gr_set_blend( gr->tex, blend_mode, custom_blendmode );
        GPU_SetRGBA( gr->tex, color_r, color_g, color_b, alpha );

        if ( gr->tex->filter_mode != gr_filter_mode ) GPU_SetImageFilter( gr->tex, gr_filter_mode );

        GPU_Target * dst = dest ? dest->tex->target : gRenderer;

        GPU_BlitTransformX( gr->tex, gr_clip, dst, ( float ) scrx, ( float ) scry, ( float ) centerx, ( float ) centery, ( float ) angle / -1000.0, scalex_adjusted, scaley_adjusted );
#endif
    }

#ifdef USE_SDL2
   if ( dest ) SDL_SetRenderTarget( gRenderer, NULL );
#endif

}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : gr_calculate_corners
 *
 *  Calculate the screen coordinates of the corners of a graphic
 *  when rotated and scaled in an specific angle
 *
 *  PARAMS :
 *      graph           Pointer to the graphic object
 *      x, y            Pixel coordinates of the center on screen
 *      angle           Angle of rotation in miliangles
 *      scalex, scaley  Scaling ratio in percentaje (100 for original size)
 *      corners         Pointer to the output array of 4 points
 *
 *  RETURN VALUE :
 *      The screen coordinates of the points will be written
 *      into the "corners" array
 *
 */

static void gr_calculate_corners( GRAPH * dest,
                                  int64_t screen_x,
                                  int64_t screen_y,
                                  int64_t flags,
                                  int64_t angle,
                                  double scalex,
                                  double scaley,
                                  double centerx,
                                  double centery,
                                  BGD_Point * corners,
                                  BGD_Rect * map_clip
                                  ) {
    double sx = 1, sy = -1;
    int64_t width = dest->width, height = dest->height;

    if ( scalex < 0.0 ) scalex = 0.0;
    if ( scaley < 0.0 ) scaley = 0.0;

    if ( map_clip ) {
        width = map_clip->w;
        height = map_clip->h;
    }

    /* Calculate the graphic center */

    if ( centerx == POINT_UNDEFINED || centery == POINT_UNDEFINED ) {
        if ( dest->ncpoints && dest->cpoints[0].x != CPOINT_UNDEFINED ) {
            centerx = dest->cpoints[0].x;
            centery = dest->cpoints[0].y;
        } else {
            centerx = width  / 2.0;
            centery = height / 2.0;
        }
    }

    if ( flags & B_HMIRROR ) sx = -1;
    if ( flags & B_VMIRROR ) sy = 1;

    /* Rotate the coordinates */

    double cos_angle = cos_deg( angle );
    double sin_angle = sin_deg( angle );

    /* Top-left, top-right, bottom-left, bottom-right */

    int64_t x0 = screen_x * 100,
            y0 = screen_y * 100;

    double lef_x, top_y, rig_x, bot_y;

    lef_x = - ( scalex * centerx );
    rig_x =   ( scalex * ( width - 1 - centerx ) );

    top_y = - ( scaley * centery );
    bot_y =   ( scaley * ( height - 1 - centery ) );

    corners[0].x = ( lef_x * cos_angle + top_y * sin_angle ) * sx + x0;
    corners[0].y = ( lef_x * sin_angle - top_y * cos_angle ) * sy + y0;
    corners[1].x = ( rig_x * cos_angle + top_y * sin_angle ) * sx + x0;
    corners[1].y = ( rig_x * sin_angle - top_y * cos_angle ) * sy + y0;
    corners[2].x = ( lef_x * cos_angle + bot_y * sin_angle ) * sx + x0;
    corners[2].y = ( lef_x * sin_angle - bot_y * cos_angle ) * sy + y0;
    corners[3].x = ( rig_x * cos_angle + bot_y * sin_angle ) * sx + x0;
    corners[3].y = ( rig_x * sin_angle - bot_y * cos_angle ) * sy + y0;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : gr_get_bbox
 *
 *  Calculate the bounding box of a graphic, when drawing it with
 *  the gr_rotated_blit function
 *
 *  PARAMS :
 *      dest            Destination bitmap or NULL for screen
 *      clip            Clipping region or NULL for the whole screen
 *      scrx, scry      Pixel coordinates of the center on screen
 *      angle           Angle of rotation in miliangles
 *      scalex, scaley  Scaling ratio in percentaje (100 for original size)
 *      gr              Pointer to the graphic object to draw
 *
 *  RETURN VALUE :
 *      None
 *
 */

void gr_get_bbox( REGION * dest,
                  REGION * clip,
                  double x,
                  double y,
                  int64_t flags,
                  int64_t angle,
                  double scalex,
                  double scaley,
                  double centerx,
                  double centery,
                  GRAPH * gr,
                  BGD_Rect * map_clip
    ) {
    SDL_Point corners[4];
    SDL_Point min, max;
    int i;

    /* Calculate the coordinates of each corner in the graphic */

    gr_calculate_corners( gr, x, y, flags, angle, scalex, scaley, centerx, centery, corners, map_clip );

    /* Calculate the bounding box */

    min.x = max.x = corners[0].x;
    min.y = max.y = corners[0].y;

    for ( i = 1; i < 4; i++ ) {
        if ( min.x > corners[i].x ) min.x = corners[i].x;
        if ( max.x < corners[i].x ) max.x = corners[i].x;
        if ( min.y > corners[i].y ) min.y = corners[i].y;
        if ( max.y < corners[i].y ) max.y = corners[i].y;
    }

    dest->x  = min.x / 100;
    dest->y  = min.y / 100;
    dest->x2 = max.x / 100;
    dest->y2 = max.y / 100;
}

/* --------------------------------------------------------------------------- */
