/*
 *  Copyright (C) 2006-2019 SplinterGU (Fenix/BennuGD)
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

#include "bgdrtm.h"
#include "bgddl.h"

#include "libbggfx.h"
#include "libmod_gfx.h"

#include "xstrings.h"

#include "dlvaracc.h"

/* --------------------------------------------------------------------------- */
/* MEDIA                                                                       */
/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_load( INSTANCE * my, int64_t * params ) {
    int64_t ret, graphid = 0;
    const char * media = string_get( params[0] );
    ret = ( int64_t ) ( intptr_t ) media_load( media, &graphid, 0, 0, 0 );
    string_discard( params[0] );
    if ( params[1] ) *(( int64_t * )( intptr_t )( params[1] ) ) = graphid;

    return ret;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_load2( INSTANCE * my, int64_t * params ) {
    int64_t ret, graphid;
    const char * media = string_get( params[0] );
    ret = ( int64_t ) ( intptr_t ) media_load( media, &graphid, 0, 0, params[2] );
    string_discard( params[0] );
    if ( params[1] ) *(( int64_t * )( intptr_t )( params[1] ) ) = graphid;
    return ret;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_load3( INSTANCE * my, int64_t * params ) {
    int64_t ret, graphid;
    const char * media = string_get( params[0] );
    ret = ( int64_t ) ( intptr_t ) media_load( media, &graphid, params[2], params[3], 0 );
    string_discard( params[0] );
    if ( params[1] ) *(( int64_t * )( intptr_t )( params[1] ) ) = graphid;

    return ret;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_load4( INSTANCE * my, int64_t * params ) {
    int64_t ret, graphid;
    const char * media = string_get( params[0] );
    ret = ( int64_t ) ( intptr_t ) media_load( media, &graphid, params[2], params[3], params[4] );
    string_discard( params[0] );
    if ( params[1] ) *(( int64_t * )( intptr_t )( params[1] ) ) = graphid;
    return ret;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_unload( INSTANCE * my, int64_t * params ) {
    media_unload( ( MEDIA * ) ( params[0] ) );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_play( INSTANCE * my, int64_t * params ) {
    return media_play( ( MEDIA * ) ( params[0] ) );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_pause_action( INSTANCE * my, int64_t * params ) {
    media_pause_action( ( MEDIA * ) ( params[0] ), params[1] );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_pause( INSTANCE * my, int64_t * params ) {
    media_pause( ( MEDIA * ) ( params[0] ) );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_resume( INSTANCE * my, int64_t * params ) {
    media_resume( ( MEDIA * ) ( params[0] ) );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_stop( INSTANCE * my, int64_t * params ) {
    media_stop( ( MEDIA * ) ( params[0] ) );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_get_time( INSTANCE * my, int64_t * params ) {
    return media_get_time( ( MEDIA * ) ( params[0] ) );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_set_time( INSTANCE * my, int64_t * params ) {
    media_set_time( ( MEDIA * ) ( params[0] ), params[1] );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_get_duration( INSTANCE * my, int64_t * params ) {
    return media_get_duration( ( MEDIA * ) ( params[0] ) );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_get_status( INSTANCE * my, int64_t * params ) {
    return media_get_status( ( MEDIA * ) ( params[0] ) );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_get_rate( INSTANCE * my, int64_t * params ) {
    float res =  media_get_rate( ( MEDIA * ) ( params[0] ) );
    return *(( int64_t * )&res );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_set_rate( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) media_set_rate( ( MEDIA * ) ( params[0] ), *( float * ) &params[1] );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_next_frame( INSTANCE * my, int64_t * params ) {
    media_next_frame( ( MEDIA * ) ( params[0] ) );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_get_mute( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) media_get_mute(( MEDIA * ) ( params[0] ) );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_set_mute( INSTANCE * my, int64_t * params ) {
    media_set_mute( ( MEDIA * ) ( params[0] ), params[1] );
    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_get_volume( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) media_get_volume( ( MEDIA * ) ( params[0] ) );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_media_set_volume( INSTANCE * my, int64_t * params ) {
    return ( int64_t ) media_set_volume( ( MEDIA * ) ( params[0] ), params[1] );
}

/* --------------------------------------------------------------------------- */
