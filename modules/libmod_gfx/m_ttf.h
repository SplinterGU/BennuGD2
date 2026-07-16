/*
 * FreeType font support for BennuGD2.
 *
 * This is an altered/additional source file for Bennu Game Development.
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * Permission is granted to use, alter, and redistribute it for any purpose,
 * subject to the BennuGD2 source distribution's license terms.
 */

#ifndef __M_TTF_H
#define __M_TTF_H

#include "bgddl.h"

extern int64_t libmod_gfx_ttf_load( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_ttf_set_size( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_ttf_get_family( INSTANCE * my, int64_t * params );
extern int64_t libmod_gfx_ttf_get_style( INSTANCE * my, int64_t * params );

#endif
