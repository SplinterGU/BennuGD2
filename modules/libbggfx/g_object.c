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

#include <stdlib.h>
#include <string.h>

#include "libbggfx.h"

/* --------------------------------------------------------------------------- */

static int64_t sequencer = 0;

CONTAINER * sorted_object_list = NULL;

/* --------------------------------------------------------------------------- */

CONTAINER * search_container( int64_t key ) {
    CONTAINER * ctr = NULL;

    if ( !sorted_object_list ) return NULL;

    for ( ctr = sorted_object_list; ctr && ctr->key > key; ctr = ctr->next );

    if ( ctr && ctr->key == key ) return ctr;

    return NULL;
}

/* --------------------------------------------------------------------------- */

CONTAINER * get_container( int64_t key ) {
    CONTAINER * ctr = NULL, * prev_ctr = NULL, * new_ctr = NULL;

    if ( sorted_object_list ) {
        for ( ctr = sorted_object_list; ctr && ctr->key > key; ctr = ctr->next ) {
            prev_ctr = ctr;
        }
        if ( ctr && ctr->key == key ) return ctr;
    }

    new_ctr = ( CONTAINER * ) malloc( sizeof( CONTAINER ) );
    if ( !new_ctr ) return NULL;

    new_ctr->key = key;
    new_ctr->first_in_key = NULL;

    if ( ctr == sorted_object_list ) {
        if ( !sorted_object_list ) {
            /* Only happen "!sorted_object_list" when "ctr == sorted_object_list" (NULL) */
            sorted_object_list = new_ctr;
            new_ctr->next = NULL;
            new_ctr->prev = NULL;

            return new_ctr;
        }

        sorted_object_list = new_ctr;
    }

    if ( ctr ) {
        new_ctr->next = ctr;
        new_ctr->prev = ctr->prev;
        if ( ctr->prev ) ctr->prev->next = new_ctr;
        ctr->prev = new_ctr;
    } else {
        // prev_ctr implicit exist
        new_ctr->next = NULL;
        new_ctr->prev = prev_ctr;
        prev_ctr->next = new_ctr;
    }

    return new_ctr;
}

/* --------------------------------------------------------------------------- */

void destroy_container( CONTAINER * ctr ) {
    if ( ctr->next ) ctr->next->prev = ctr->prev;
    if ( ctr->prev ) ctr->prev->next = ctr->next;
    if ( ctr == sorted_object_list ) sorted_object_list = ctr->next ;

    free( ctr );
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : gr_new_object
 *
 *  Register a visible object with a Z coordinate to be drawn
 *  by an user-defined function.
 *
 *  If your DLL does some screen operations, you should register
 *  a draw-type hook or an object. Any access to the screen
 *  surface outside those are invalid.
 *
 *  PARAMS :
 *      z               Z value of the object to be drawn
 *      info            Pointer to the object information function
 *                      (fills bounding box, returns 1 if changed since last frame)
 *      draw            Pointer to the object drawing function
 *      what            User-defined parameter that will be passed to "draw"
 *
 *  RETURN VALUE :
 *      An integer ID that uniquely identifies the object,
 *    or 0 if not enough memory
 */

int64_t gr_new_object( int64_t z, OBJ_INFO * info, OBJ_DRAW * draw, void * what ) {
    CONTAINER * ctr ;
    OBJECT * object = ( OBJECT * ) malloc( sizeof( OBJECT ) );

    if ( !object ) return 0;

    ctr = get_container( z );
    if ( !ctr ) {
        free( object );
        return 0;
    }

    object->z = z;
    object->info = info;
    object->draw = draw;
    object->what = what;
    object->ready = 0;
    object->seq = 0;
    object->prev = NULL;

    if ( ctr->first_in_key ) {
        ctr->first_in_key->prev = object;
        object->next = ctr->first_in_key;
    } else {
        object->next = NULL;
    }

    ctr->first_in_key = object;

    return ( int64_t ) ( intptr_t ) object;
}

/* --------------------------------------------------------------------------- */
/*
 *  FUNCTION : gr_destroy_object
 *
 *  Unregister and remove a given object created by gr_new_object from memory
 *
 *  PARAMS :
 *      id              ID returned by gr_new_object
 *
 *  RETURN VALUE :
 *      None
 */

void gr_destroy_object( int64_t id ) {
    CONTAINER * ctr ;
    OBJECT * object = ( OBJECT * ) ( intptr_t ) id ;

    if ( !object ) return ;

    ctr = search_container( object->z );
    if ( !ctr ) return;

    if ( object->next ) object->next->prev = object->prev;
    if ( object->prev ) object->prev->next = object->next;
    if ( object == ctr->first_in_key ) ctr->first_in_key = object->next;
    if ( !ctr->first_in_key ) destroy_container( ctr );

    free( object );
}

/* --------------------------------------------------------------------------- */

void gr_update_objects( void ) {
    CONTAINER * ctr = NULL, * next_ctr, * fix_ctr;
    OBJECT * object, * next_object ;
    int64_t key;

    if ( !sorted_object_list ) return ;

    sequencer++;

    next_ctr = sorted_object_list;
    while (( ctr = next_ctr ) ) {
        /* Get Next Container */
        next_ctr = ctr->next ;

        key = ctr->key;

        next_object = ctr->first_in_key;
        while ( ctr && ( object = next_object ) ) {
            next_object = object->next ;

            if ( object->seq != sequencer ) {
                /* Mark object as already proceed */
                object->seq = sequencer;

                /* Update key & get_info */
                ( *object->info )( object->what, NULL, &object->z, &object->ready );

                /* Move to correct container */
                if ( object->z != key ) {
                    /* Remove from list */
                    if ( object->next ) object->next->prev = object->prev;
                    if ( object->prev ) object->prev->next = object->next;
                    if ( object == ctr->first_in_key ) ctr->first_in_key = object->next;

                    if ( !ctr->first_in_key ) {
                        destroy_container( ctr );
                        ctr = NULL;
                    }

                    /* Get new or exist container */
                    fix_ctr = get_container( object->z );
                    if ( !fix_ctr ) continue; /* Error */

                    /* Put new object first */
                    if ( fix_ctr->first_in_key ) fix_ctr->first_in_key->prev = object;

                    object->prev = NULL;
                    object->next = fix_ctr->first_in_key;

                    fix_ctr->first_in_key = object;
                }
            }
        }
    }
}

/* --------------------------------------------------------------------------- */

void gr_draw_objects( void ) {
    CONTAINER * ctr = NULL;
    OBJECT * object;

    ctr = sorted_object_list;
    while ( ctr ) {
        object = ctr->first_in_key;
        while ( object ) {
            if ( object->ready ) ( *object->draw )( object->what, NULL ) ;
            object = object->next ;
        }
        ctr = ctr->next ;
    }
}

/* --------------------------------------------------------------------------- */
