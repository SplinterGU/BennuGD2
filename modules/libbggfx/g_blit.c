/*
 *  Copyright (C) 2006-2019 SplinterGU (Fenix/BennuGD)
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

#include <SDL.h>

#include "bgddl.h"
#include "libbggfx.h"
#include "dlvaracc.h"

// #define __DISABLE_PALETTES__
int no_draw = 0;
/* --------------------------------------------------------------------------- */

static int inline gr_update_texture( GRAPH * gr ) {
    SDL_Surface * surface;

#ifndef __DISABLE_PALETTES__
    if ( gr->surface->format->format == SDL_PIXELFORMAT_ARGB8888 /*|| surface->format->format == SDL_PIXELFORMAT_RGB565*/ ) {
        surface = gr->surface;
    } else {
        surface = SDL_ConvertSurfaceFormat(gr->surface, SDL_PIXELFORMAT_ARGB8888, 0);
        if ( !surface ) return -1;
    }
#else
    surface = gr->texture;
#endif

    if ( SDL_MUSTLOCK( surface ) ) {
        SDL_LockSurface( surface );
        SDL_UpdateTexture( gr->texture, NULL, surface->pixels, surface->pitch);
        SDL_UnlockSurface( surface );
    } else {
        SDL_UpdateTexture( gr->texture, NULL, surface->pixels, surface->pitch);
    }

#ifndef __DISABLE_PALETTES__
    if ( surface != gr->surface ) SDL_FreeSurface( surface );
#endif

    gr->texture_must_update = 0;

    return 0;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : gr_prepare_renderer
 *
 *  Setup common parameters for render a object
 *
 *  PARAMS :
 *      dest            Destination bitmap or NULL for screen
 *      clip            Clipping region or NULL for the whole screen
 *      flags           Flags
 *      blend_mode      Ouput SDL_BlendMode
 *
 *  RETURN VALUE :
 *      None
 *
 */

int gr_prepare_renderer( GRAPH * dest, REGION * clip, int64_t flags, SDL_BlendMode * blend_mode ) {

    if ( dest ) {
        if ( !dest->surface ) {
            dest->surface = SDL_CreateRGBSurface( 0, dest->width, dest->height, gPixelFormat->BitsPerPixel, gPixelFormat->Rmask, gPixelFormat->Gmask, gPixelFormat->Bmask, gPixelFormat->Amask );
            if ( !dest->surface ) return 1;
//            SDL_SetColorKey( dest->surface, SDL_TRUE, 0 );
        }

        if ( !dest->texture ) {
            dest->texture = SDL_CreateTexture( gRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, dest->width, dest->height );
            if ( !dest->texture ) {
                printf ("error creando textura RW [%s]\n", SDL_GetError() );
                return 1;
            }

            gr_update_texture(dest);

            dest->type = BITMAP_TEXTURE_TARGET;
        }
        else if ( dest->type != BITMAP_TEXTURE_TARGET ) return 1;
    }

         if ( flags & B_NOCOLORKEY )    * blend_mode = SDL_BLENDMODE_NONE;  //Disable blending on texture
    else if ( flags & B_ABLEND     )    * blend_mode = SDL_BLENDMODE_ADD;   //Additive blending on texture
    else if ( flags & B_MBLEND     )    * blend_mode = SDL_BLENDMODE_MOD;   //Modulate blending on texture
    else if ( flags & B_SBLEND     ) {
        * blend_mode = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_SRC_ALPHA, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_SUBTRACT, SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_SUBTRACT);
    }
    else                                * blend_mode = SDL_BLENDMODE_BLEND; //Enable blending on texture

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

    if ( dest ) SDL_SetRenderTarget( gRenderer, dest->texture );
//    if ( dest ) { SDL_SetRenderTarget( gRenderer, dest->texture ); SDL_SetTextureBlendMode( dest->texture, SDL_BLENDMODE_NONE ); }

    SDL_RenderSetClipRect( gRenderer, &rect );

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

void gr_blit(
                GRAPH * dest,
                REGION * clip,
                double scrx,
                double scry,
                int64_t flags,
                int64_t angle,
                double scalex,
                double scaley,
                GRAPH * gr,
                SDL_Rect * gr_clip,
                uint8_t alpha,
                uint8_t color_r,
                uint8_t color_g,
                uint8_t color_b
            ) {
    if ( !gr || !gr->surface ) return;

    if ( scalex <= 0.0 || scaley <= 0.0 ) return;

    if ( !gr->texture && !gr->segments ) {

        if ( gr->width > gRendererInfo.max_texture_width || gr->height > gRendererInfo.max_texture_height ) {
            int64_t nsegx = ( gr->width - 1 ) / gRendererInfo.max_texture_width + 1,
                    nsegy = ( gr->height - 1 ) / gRendererInfo.max_texture_height + 1;
            int64_t x, y, offx, offy, seg, w, h;
            int64_t total = nsegx * nsegy;

// free old gr->segments
//            free ( gr->segments );

            gr->segments = calloc( total * 4, sizeof( *( gr->segments ) ) );
            if ( !gr->segments ) return;
            gr->nsegments = total;

            SDL_Surface *auxSurface = NULL;
            int64_t oldw = 0, oldh = 0;
            SDL_Rect srcrect = {0};

            for ( y = 0; y < nsegy; y++ ) {
                offy = y * gRendererInfo.max_texture_height;
                if ( y + 1 == nsegy )   h = gr->height % gRendererInfo.max_texture_height;
                else                    h = gRendererInfo.max_texture_height;
                for ( x = 0; x < nsegx; x++ ) {
                    offx = x * gRendererInfo.max_texture_width;
                    seg = y * nsegx + x;
                    gr->segments[seg].offx = offx;
                    gr->segments[seg].offy = offy;
                    if ( x + 1 == nsegx )   w = gr->width % gRendererInfo.max_texture_width;
                    else                    w = gRendererInfo.max_texture_width;
                    gr->segments[seg].w = w;
                    gr->segments[seg].h = h;
                    gr->segments[seg].texture = SDL_CreateTexture( gRenderer, SDL_PIXELFORMAT_ARGB8888 /*gr->surface->format->format*/, SDL_TEXTUREACCESS_STATIC, w, h );
                    if ( !gr->segments[seg].texture ) {
                        printf ("error creando multi textura RO [%s]\n", SDL_GetError() );
                        return;
                    }

                    if ( oldw != w || oldh != h ) {
                        if ( auxSurface ) SDL_FreeSurface( auxSurface );
                        oldw = w; oldh = h;
                        auxSurface = SDL_CreateRGBSurface( 0, w, h, gPixelFormat->BitsPerPixel, gPixelFormat->Rmask, gPixelFormat->Gmask, gPixelFormat->Bmask, gPixelFormat->Amask );
                        if ( !auxSurface ) {
                            printf ("error creando temp surface [%s]\n", SDL_GetError() );
                            return;
                        }
//                        SDL_SetColorKey( auxSurface, SDL_TRUE, 0 );
                    }

                    srcrect.x = offx;
                    srcrect.y = offy;
                    srcrect.w = w;
                    srcrect.h = h;

                    SDL_BlitSurface( gr->surface, &srcrect, auxSurface, NULL );
                    SDL_UpdateTexture( gr->segments[seg].texture, NULL, auxSurface->pixels, auxSurface->pitch);
                }
                if ( auxSurface ) SDL_FreeSurface( auxSurface );
            }

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

                    offx1 += gr->segments[seg1].w;
                    offx2 += gr->segments[seg2].w;
                    offx3 += gr->segments[seg3].w;
                }
                offy1 += gr->segments[seg1].h;
                offy2 += gr->segments[seg2].h;
                offy3 += gr->segments[seg3].h;
            }

        } else {
            gr->texture = SDL_CreateTexture( gRenderer, SDL_PIXELFORMAT_ARGB8888 /*gr->surface->format->format*/, SDL_TEXTUREACCESS_STATIC, gr->width, gr->height );
            if ( !gr->texture ) {
                printf ("error creando textura RO [%s]\n", SDL_GetError() );
                return;
            }

            gr_update_texture(gr);
        }
        gr->type = BITMAP_TEXTURE_STATIC;
    }

    if ( gr->texture && gr->texture_must_update ) gr_update_texture(gr);

    SDL_BlendMode blend_mode;

    if ( gr_prepare_renderer( dest, clip, flags, &blend_mode ) ) return;

    SDL_Point center;
    int64_t w = 0, h = 0;

    if ( gr_clip ) {
        w = gr_clip->w;
        h = gr_clip->h;
    } else {
        w = gr->width;
        h = gr->height;
    }

    SDL_RendererFlip flip = ( ( flags & B_HMIRROR ) ? SDL_FLIP_HORIZONTAL : 0 ) | ( ( flags & B_VMIRROR ) ? SDL_FLIP_VERTICAL : 0 );

    SDL_Rect dstrect;
    double centerx, centery,
           scalex_adjusted, scaley_adjusted;
    int i;

    if ( flags & B_TRANSLUCENT ) alpha >>= 1;

    if ( flags & B_HMIRROR ) angle = -angle;
    if ( flags & B_VMIRROR ) angle = -angle;

    scalex_adjusted = scalex / 100.0;
    scaley_adjusted = scaley / 100.0;

    if ( gr->ncpoints && gr->cpoints[0].x != CPOINT_UNDEFINED ) {
        int64_t cx = gr->cpoints[0].x, cy = gr->cpoints[0].y;

        if ( flags & B_HMIRROR ) cx = gr->width - cx - 1;
        if ( flags & B_VMIRROR ) cy = gr->height - cy - 1;

        centerx = scalex_adjusted * cx;
        centery = scaley_adjusted * cy;
    } else {
        centerx = scalex_adjusted * w / 2.0;
        centery = scaley_adjusted * h / 2.0;
    }

    if ( gr->segments ) {
        SDL_Texture * tex;
        int mirror_offset = 0;

        if ( flags & B_HMIRROR ) mirror_offset += gr->nsegments;
        if ( flags & B_VMIRROR ) mirror_offset += gr->nsegments * 2;

        for ( i = mirror_offset; i < mirror_offset + gr->nsegments; i++ ) {
            double  offx = gr->segments[ i ].offx * scalex_adjusted,
                    offy = gr->segments[ i ].offy * scaley_adjusted;

            tex = gr->segments[ i ].texture;

            center.x = centerx - offx;
            center.y = centery - offy;

            dstrect.x = scrx - centerx + offx;
            dstrect.y = scry - centery + offy;
            dstrect.w = scalex_adjusted * gr->segments[ i ].w; // Calculate dest region according texture
            dstrect.h = scaley_adjusted * gr->segments[ i ].h; // Calculate dest region according texture

            SDL_SetTextureBlendMode( tex, blend_mode );
            SDL_SetTextureAlphaMod( tex, alpha );
            SDL_SetTextureColorMod( tex, color_r, color_g, color_b );

            //Render
            SDL_RenderCopyEx( gRenderer, tex, gr_clip, &dstrect, angle / -1000.0, &center, flip );
        }
    } else {
        center.x = centerx;
        center.y = centery;

        dstrect.x = scrx - center.x;
        dstrect.y = scry - center.y;
        dstrect.w = scalex_adjusted * w;
        dstrect.h = scaley_adjusted * h;

        SDL_SetTextureBlendMode( gr->texture, blend_mode );
        SDL_SetTextureAlphaMod( gr->texture, alpha );
        SDL_SetTextureColorMod( gr->texture, color_r, color_g, color_b );

        //Render
        SDL_RenderCopyEx( gRenderer, gr->texture, gr_clip, &dstrect, angle / -1000.0, &center, flip );
    }

    if ( dest ) SDL_SetRenderTarget( gRenderer, NULL );

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

static void gr_calculate_corners( GRAPH * dest, int64_t screen_x, int64_t screen_y, int64_t flags, int64_t angle, double scalex, double scaley, SDL_Point * corners, SDL_Rect * map_clip ) {
    int64_t center_x, center_y, sx = 1, sy = -1;
    int64_t width = dest->width, height = dest->height;

    if ( scalex < 0.0 ) scalex = 0.0;
    if ( scaley < 0.0 ) scaley = 0.0;

    if ( map_clip ) {
        width = map_clip->w;
        height = map_clip->h;
    }

    /* Calculate the graphic center */

    if ( dest->ncpoints && dest->cpoints[0].x != CPOINT_UNDEFINED ) {
        center_x = dest->cpoints[0].x;
        center_y = dest->cpoints[0].y;
    } else {
        center_x = width  / 2.0;
        center_y = height / 2.0;
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

    lef_x = - ( scalex * center_x );
    rig_x =   ( scalex * ( width - center_x ) );

    top_y = - ( scaley * center_y );
    bot_y =   ( scaley * ( height - center_y ) );

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

void gr_get_bbox( REGION * dest, REGION * clip, double x, double y, int64_t flags, int64_t angle, double scalex, double scaley, GRAPH * gr, SDL_Rect * map_clip ) {
    SDL_Point corners[4];
    SDL_Point min, max;
    int i;

    /* Calculate the coordinates of each corner in the graphic */

    gr_calculate_corners( gr, x, y, flags, angle, scalex, scaley, corners, map_clip );

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
