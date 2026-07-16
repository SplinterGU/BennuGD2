/*
 * FreeType font support for BennuGD2.
 *
 * This is an altered/additional source file for Bennu Game Development.
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * Permission is granted to use, alter, and redistribute it for any purpose,
 * subject to the BennuGD2 source distribution's license terms.
 */

#include "bgdrtm.h"
#include "bgddl.h"
#include "xstrings.h"

#include "libbggfx.h"
#include "libmod_gfx.h"

int64_t libmod_gfx_ttf_load( INSTANCE * my, int64_t * params ) {
    const char * filename = string_get( params[0] );
    int64_t result = gr_font_ttf_load( filename );

    string_discard( params[0] );
    return result;
}

int64_t libmod_gfx_ttf_set_size( INSTANCE * my, int64_t * params ) {
    return gr_font_ttf_set_size( params[0], params[1] );
}

static int64_t make_bennu_string( const char * value ) {
    int64_t string_id = string_new( value ? value : "" );
    string_use( string_id );
    return string_id;
}

int64_t libmod_gfx_ttf_get_family( INSTANCE * my, int64_t * params ) {
    return make_bennu_string( gr_font_ttf_get_family( params[0] ) );
}

int64_t libmod_gfx_ttf_get_style( INSTANCE * my, int64_t * params ) {
    return make_bennu_string( gr_font_ttf_get_style( params[0] ) );
}
