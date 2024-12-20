/*
 *  Copyright (C) SplinterGU (Fenix/BennuGD) (Since 2006)
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

#ifndef __OBJECT_H
#define __OBJECT_H

/* --------------------------------------------------------------------------- */

typedef int ( OBJ_INFO )( void * what, REGION * clip, int64_t * key, int64_t * ready );
typedef void ( OBJ_DRAW )( void * what, REGION * clip );

typedef struct _object {
    int64_t z;
    OBJ_INFO * info;
    OBJ_DRAW * draw;
    void * what;
    int64_t ready;         /* Ready to draw */
    int64_t seq;

    struct _object * prev;
    struct _object * next;
} OBJECT;

typedef struct _container {
    int64_t key;
    OBJECT * first_in_key;

    struct _container * prev;
    struct _container * next;
} CONTAINER;

/* --------------------------------------------------------------------------- */

//extern CONTAINER * sorted_object_list;

/* --------------------------------------------------------------------------- */

extern CONTAINER * search_container( int64_t key );
extern CONTAINER * get_container( int64_t key );
extern void destroy_container( CONTAINER * ctr );
extern int64_t gr_new_object( int64_t z, OBJ_INFO * info, OBJ_DRAW * draw, void * what );
extern void gr_destroy_object( int64_t id );
extern void gr_update_objects( void );
extern void gr_draw_objects( void );

/* --------------------------------------------------------------------------- */

#endif
