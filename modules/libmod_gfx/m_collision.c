/*
 *  Copyright (C) 2006-2016 SplinterGU (Fenix/BennuGD)
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

#include <SDL.h>

#include <stdlib.h>

#include "bgdrtm.h"
#include "bgddl.h"

#include "resolution.h"

#include "m_resolxy.h"

#include "libbggfx.h"
#include "libmod_gfx.h"

#include "fmath.h"

#include "dlvaracc.h"

/* --------------------------------------------------------------------------- */

enum {
    COLLISION_NORMAL = 0,
    COLLISION_BOX,
    COLLISION_CIRCLE
};

/* --------------------------------------------------------------------------- */

static int64_t inline get_distance( int64_t x1, int64_t y1, int64_t r1, int64_t x2, int64_t y2, int64_t r2 ) {
    RESOLXY_RES( x1, y1, r1 );
    RESOLXY_RES( x2, y2, r2 );

    double dx = ( x2 - x1 ) * ( x2 - x1 );
    double dy = ( y2 - y1 ) * ( y2 - y1 );

    if ( r1 > 0 ) return ( int64_t )sqrt( dx + dy ) * r1;
    else if ( r1 < 0 ) return ( int64_t )sqrt( dx + dy ) / -r1;

    return ( int64_t )sqrt( dx + dy );
}

/* --------------------------------------------------------------------------- */
/* Rutinas de utilidad local */
#if 0
void draw_at( GRAPH * dest, int x, int y, REGION * r, INSTANCE * i ) {
    GRAPH * map;
    int scalex, scaley;

    scalex = LOCINT64( libmod_gfx, i, GRAPHSIZEX );
    scaley = LOCINT64( libmod_gfx, i, GRAPHSIZEY );
    if ( scalex == 100 && scaley == 100 ) scalex = scaley = LOCINT64( libmod_gfx, i, GRAPHSIZE );

    map = instance_graph( i );
    if ( !map ) return;

    // PATCH - XGRAPH DOES NOT ROTATE DESTINATION GRAPHIC
    if ( LOCINT64( libmod_gfx, i, ANGLE ) || scaley != 100 || scalex != 100 ) {
        if ( LOCQWORD( libmod_gfx, i, XGRAPH ) && scalex == 100 && scaley == 100 ) {
            gr_blit( dest, r, x, y, LOCQWORD( libmod_gfx, i, FLAGS ) & ( B_HMIRROR | B_VMIRROR ), map );
        }
        else
        {
            if ( LOCQWORD( libmod_gfx, i, XGRAPH ) ) {
                gr_rotated_blit( dest, r, x, y, LOCQWORD( libmod_gfx, i, FLAGS ) & ( B_HMIRROR | B_VMIRROR ), 0, scalex, scaley, map );
            }
            else
            {
                gr_rotated_blit( dest, r, x, y, LOCQWORD( libmod_gfx, i, FLAGS ) & ( B_HMIRROR | B_VMIRROR ), LOCINT64( libmod_gfx, i, ANGLE ), scalex, scaley, map );
            }
        }
    }
    else
    {
        gr_blit( dest, r, x, y, LOCQWORD( libmod_gfx, i, FLAGS ) & ( B_HMIRROR | B_VMIRROR ), map );
    }
}
#endif
/* --------------------------------------------------------------------------- */

static int get_bbox( REGION * bbox, INSTANCE * proc ) {
    GRAPH * b;
    int64_t x, y;
    int64_t scalex, scaley;
    SDL_Rect *map_clip = NULL, _map_clip;

    b = instance_graph( proc );
    if ( !b ) return 0;

    scalex = LOCINT64( libmod_gfx, proc, GRAPHSIZEX );
    scaley = LOCINT64( libmod_gfx, proc, GRAPHSIZEY );
    if ( scalex == 100 && scaley == 100 ) scalex = scaley = LOCINT64( libmod_gfx, proc, GRAPHSIZE );

    x = LOCINT64( libmod_gfx, proc, COORDX );
    y = LOCINT64( libmod_gfx, proc, COORDY );

    RESOLXY( libmod_gfx, proc, x, y );

    _map_clip.w = LOCINT64( libmod_gfx, proc, CLIPW );
    _map_clip.h = LOCINT64( libmod_gfx, proc, CLIPH );

    if ( _map_clip.w && _map_clip.h ) {
        _map_clip.x = LOCINT64( libmod_gfx, proc, CLIPX );
        _map_clip.y = LOCINT64( libmod_gfx, proc, CLIPY );
        map_clip = &_map_clip;
    }

    gr_get_bbox( bbox, NULL, x, y, LOCQWORD( libmod_gfx, proc, FLAGS ) & ( B_HMIRROR | B_VMIRROR ), LOCINT64( libmod_gfx, proc, ANGLE ), scalex, scaley, b, map_clip );

    return 1;
}

/* --------------------------------------------------------------------------- */
/* Colisiones */

static int check_collision_with_mouse( INSTANCE * proc1, int colltype ) {
    REGION bbox1, bbox2;
    int64_t x, y, mx, my;
//    static GRAPH * bmp = NULL;

    switch ( colltype ) {
        case    COLLISION_BOX:
        case    COLLISION_CIRCLE:
                if ( !get_bbox( &bbox2, proc1 ) ) return 0;

        case    COLLISION_NORMAL:
                break;

        default:
                return 0;
    }

    mx = GLOINT64( libmod_gfx, MOUSEX );
    my = GLOINT64( libmod_gfx, MOUSEY );

    /* Checks the process's bounding box to optimize checking
       (only for screen-type objects) */

    if ( LOCQWORD( libmod_gfx, proc1, CTYPE ) == C_SCREEN ) {
        switch ( colltype ) {
            case    COLLISION_NORMAL:
                    if ( !get_bbox( &bbox2, proc1 ) ) return 0;
                    if ( bbox2.x > mx || bbox2.x2 < mx || bbox2.y > my || bbox2.y2 < my ) return 0;
                    break;

            case    COLLISION_BOX:
                    if ( bbox2.x <= mx && bbox2.x2 >= mx && bbox2.y <= my && bbox2.y2 >= my ) return 1;
                    return 0;
                    break;

            case    COLLISION_CIRCLE:
                {
                    int64_t cx1, cy1, dx1, dy1;

                    cx1 = bbox2.x + ( dx1 = ( bbox2.x2 - bbox2.x + 1 ) ) / 2;
                    cy1 = bbox2.y + ( dy1 = ( bbox2.y2 - bbox2.y + 1 ) ) / 2;

                    if ( get_distance( cx1, cy1, 0, mx, my, 0 ) < ( dx1 + dy1 ) / 4 ) return 1;
                    return 0;
                    break;
                }
        }
    }

    /* Creates a temporary bitmap (only once) */
#if 0
    if ( colltype == COLLISION_NORMAL ) {
        /* maybe must force this to 32 bits */
        if ( bmp && bmp->format->depth != sys_pixel_format->depth ) { bitmap_destroy( bmp ); bmp = NULL; }
        if ( !bmp ) bmp = bitmap_new( 0, 2, 1, sys_pixel_format->depth );
        if ( !bmp ) return 0;

        memset( bmp->data, 0, bmp->pitch * bmp->height );
    }
#endif
    /* Retrieves process information */

    bbox1.x = 0; bbox1.x2 = 1;
    bbox1.y = 0; bbox1.y2 = 0;

    x = LOCINT64( libmod_gfx, proc1, COORDX );
    y = LOCINT64( libmod_gfx, proc1, COORDY );

    RESOLXY( libmod_gfx, proc1, x, y );

    /* Scroll-type process: check for each region */

    if ( LOCQWORD( libmod_gfx, proc1, CTYPE ) == C_SCROLL ) {
        SCROLL_EXTRA_DATA * data;
        scrolldata  * scroll;
        int i;

        if ( GLOEXISTS( libmod_gfx, SCROLLS ) ) {
            int64_t cnumber = LOCQWORD( libmod_gfx, proc1, CNUMBER );
            if ( !cnumber ) cnumber = 0xffffffff;

            for ( i = 0; i < 10; i++ ) {
                data = &(( SCROLL_EXTRA_DATA * ) & GLOQWORD( libmod_gfx, SCROLLS ) )[i];
                scroll = ( scrolldata * ) data->reserved[0];

                if ( scroll && scroll->active && ( cnumber & ( 1 << i ) ) ) {
                    REGION * r = scroll->region;

                    switch ( colltype ) {
#if 0
                        case    COLLISION_NORMAL:
                                if ( r->x > mx || r->x2 < mx || r->y > my || r->y2 < my ) continue;

                                draw_at( bmp, x + r->x - mx - scroll->posx0, y + r->y - my - scroll->posy0, &bbox1, proc1 );
                                switch ( sys_pixel_format->depth ) {
                                    case    8:
                                        if ( *( uint8_t * )bmp->data ) return 1;
                                        break;

                                    case    16:
                                        if ( *( uint16_t * )bmp->data ) return 1;
                                        break;

                                    case    32:
                                        if ( *( uint32_t * )bmp->data ) return 1;
                                        break;
                                }
                                break;
#endif
                        case    COLLISION_BOX:
                                if ( bbox2.x <= scroll->posx0 + r->x + mx && bbox2.x2 >= scroll->posx0 + r->x + mx &&
                                     bbox2.y <= scroll->posy0 + r->y + my && bbox2.y2 >= scroll->posy0 + r->y + my ) return 1;
                                break;

                        case    COLLISION_CIRCLE:
                            {
                                int64_t cx1, cy1, dx1, dy1;

                                cx1 = bbox2.x + ( dx1 = ( bbox2.x2 - bbox2.x + 1 ) ) / 2;
                                cy1 = bbox2.y + ( dy1 = ( bbox2.y2 - bbox2.y + 1 ) ) / 2;

                                if ( get_distance( cx1, cy1, 0, r->x + mx + scroll->posx0, r->y + my + scroll->posy0, 0 ) < ( dx1 + dy1 ) / 4 ) return 1;
                                break;
                            }
                    }
                }
            }
        }

        return 0;
    }

    switch ( colltype ) {
#if 0
        case    COLLISION_NORMAL:
                /* Collision check (blits into temporary space and checks the resulting pixel) */
                draw_at( bmp, x - mx, y - my, &bbox1, proc1 );

                switch ( sys_pixel_format->depth ) {
                    case    8:
                        if ( *( uint8_t * )bmp->data ) return 1;
                        break;

                    case    16:
                        if ( *( uint16_t * )bmp->data ) return 1;
                        break;

                    case    32:
                        if ( *( uint32_t * )bmp->data ) return 1;
                        break;
                }
                break;
#endif
        case    COLLISION_BOX:
                if ( bbox2.x <= mx && bbox2.x2 >= mx &&
                     bbox2.y <= my && bbox2.y2 >= my ) return 1;
                break;

        case    COLLISION_CIRCLE:
            {
                int64_t cx1, cy1, dx1, dy1;

                cx1 = bbox2.x + ( dx1 = ( bbox2.x2 - bbox2.x + 1 ) ) / 2;
                cy1 = bbox2.y + ( dy1 = ( bbox2.y2 - bbox2.y + 1 ) ) / 2;

                if ( get_distance( cx1, cy1, 0, mx, my, 0 ) < ( dx1 + dy1 ) / 4 ) return 1;
                break;
            }
    }

    return 0;
}

/* --------------------------------------------------------------------------- */

static int check_collision_circle( INSTANCE * proc1, REGION * bbox1, INSTANCE * proc2 ) {
    REGION bbox2;
    GRAPH * bmp;
    int64_t cx1, cy1, cx2, cy2, dx1, dy1, dx2, dy2;

    bmp = instance_graph( proc2 ); if ( !bmp ) return 0;
    instance_get_bbox( proc2, bmp, &bbox2 );

    cx1 = bbox1->x + ( dx1 = ( bbox1->x2 - bbox1->x + 1 ) ) / 2;
    cy1 = bbox1->y + ( dy1 = ( bbox1->y2 - bbox1->y + 1 ) ) / 2;

    cx2 = bbox2.x + ( dx2 = ( bbox2.x2 - bbox2.x + 1 ) ) / 2;
    cy2 = bbox2.y + ( dy2 = ( bbox2.y2 - bbox2.y + 1 ) ) / 2;

    if ( get_distance( cx1, cy1, 0, cx2, cy2, 0 ) < ( ( dx1 + dy1 ) / 2 + ( dx2 + dy2 ) / 2 ) / 2 ) return 1;

    return 0;
}

/* --------------------------------------------------------------------------- */

static int check_collision_box( INSTANCE * proc1, REGION * bbox1, INSTANCE * proc2 ) {
    REGION bbox2;
    GRAPH * bmp;

    bmp = instance_graph( proc2 ); if ( !bmp ) return 0;
    instance_get_bbox( proc2, bmp, &bbox2 );

    region_union( &bbox2, bbox1 );
    if ( region_is_empty( &bbox2 ) ) return 0;

    return 1;
}

/* --------------------------------------------------------------------------- */
#if 0
static int check_collision( INSTANCE * proc1, REGION * bbox3, INSTANCE * proc2 ) {
    REGION bbox1, bbox2;
    int x, y, w, h;
    GRAPH * bmp1 = NULL, * bmp2 = NULL;

    bbox1 = *bbox3;

    bmp2 = instance_graph( proc2 ); if ( !bmp2 ) return 0;
    instance_get_bbox( proc2, bmp2, &bbox2 );

    region_union( &bbox1, &bbox2 );
    if ( region_is_empty( &bbox1 ) ) return 0;

    // Solo si las regiones de ambos bbox se superponen

    w = bbox1.x2 - bbox1.x + 1;
    h = bbox1.y2 - bbox1.y + 1;
    bbox2.x = bbox2.y = 0;
    bbox2.x2 = w - 1;
    bbox2.y2 = h - 1;

    bmp1 = bitmap_new( 0, w, h, sys_pixel_format->depth );
    if ( !bmp1 ) return 0;

    bmp2 = bitmap_new( 0, w, h, sys_pixel_format->depth );
    if ( !bmp2 ) {
        bitmap_destroy( bmp1 );
        return 0;
    }

    memset( bmp1->data, 0, bmp1->pitch * h );
    memset( bmp2->data, 0, bmp2->pitch * h );

    x = LOCINT64( libmod_gfx, proc1, COORDX );
    y = LOCINT64( libmod_gfx, proc1, COORDY );
    RESOLXY( libmod_gfx, proc1, x, y );

    x -= bbox1.x;
    y -= bbox1.y;
    draw_at( bmp1, x, y, &bbox2, proc1 );

    x = LOCINT64( libmod_gfx, proc2, COORDX );
    y = LOCINT64( libmod_gfx, proc2, COORDY );
    RESOLXY( libmod_gfx, proc2, x, y );

    x -= bbox1.x;
    y -= bbox1.y;
    draw_at( bmp2, x, y, &bbox2, proc2 );

    if ( sys_pixel_format->depth == 32 ) {
        uint32_t * ptr1 = ( uint32_t * ) bmp1->data;
        uint32_t * ptr2 = ( uint32_t * ) bmp2->data;

        uint8_t * _ptr1 = ( uint8_t * ) ptr1;
        uint8_t * _ptr2 = ( uint8_t * ) ptr2;

        for ( y = 0; y < h; y++ ) {
            for ( x = 0; x < w; x++, ptr1++, ptr2++ ) {
                if ( *ptr1 && *ptr2 ) {
                    bitmap_destroy( bmp1 );
                    bitmap_destroy( bmp2 );
                    return 1;
                }
            }
            ptr1 = ( uint32_t * )( _ptr1 += bmp1->pitch );
            ptr2 = ( uint32_t * )( _ptr2 += bmp2->pitch );
        }
    }
    else
    {
        if ( sys_pixel_format->depth == 16 ) {
            uint16_t * ptr1 = ( uint16_t * ) bmp1->data;
            uint16_t * ptr2 = ( uint16_t * ) bmp2->data;

            uint8_t * _ptr1 = ( uint8_t * ) ptr1;
            uint8_t * _ptr2 = ( uint8_t * ) ptr2;

            for ( y = 0; y < h; y++ ) {
                for ( x = 0; x < w; x++, ptr1++, ptr2++ ) {
                    if ( *ptr1 && *ptr2 ) {
                        bitmap_destroy( bmp1 );
                        bitmap_destroy( bmp2 );
                        return 1;
                    }
                }
                ptr1 = ( uint16_t * )( _ptr1 += bmp1->pitch );
                ptr2 = ( uint16_t * )( _ptr2 += bmp2->pitch );
            }
        }
        else
        {
            uint8_t * ptr1 = ( uint8_t * )bmp1->data;
            uint8_t * ptr2 = ( uint8_t * )bmp2->data;

            uint8_t * _ptr1 = ptr1;
            uint8_t * _ptr2 = ptr2;

            for ( y = 0; y < h; y++ ) {
                for ( x = 0; x < w; x++, ptr1++, ptr2++ ) {
                    if ( *ptr1 && *ptr2 ) {
                        bitmap_destroy( bmp1 );
                        bitmap_destroy( bmp2 );
                        return 1;
                    }
                }
                ptr1 = _ptr1 += bmp1->pitch;
                ptr2 = _ptr2 += bmp2->pitch;
            }
        }
    }

    bitmap_destroy( bmp1 );
    bitmap_destroy( bmp2 );
    return 0;
}
#endif
/* --------------------------------------------------------------------------- */

static int64_t __collision( INSTANCE * my, int64_t id, int64_t colltype ) {
    INSTANCE * ptr, ** ctx;
    int64_t status, p;
    int ( *colfunc )( INSTANCE *, REGION *, INSTANCE * );
    REGION bbox1;
    GRAPH * bmp1;

    if ( id == -1 ) return ( check_collision_with_mouse( my, colltype ) ) ? 1 : 0;

    switch ( colltype ) {
#if 0
        case    COLLISION_NORMAL:
                colfunc = check_collision;
                break;
#endif
        case    COLLISION_BOX:
                colfunc = check_collision_box;
                break;

        case    COLLISION_CIRCLE:
                colfunc = check_collision_circle;
                break;

        default:
                return 0;
    }

    bmp1 = instance_graph( my ); if ( !bmp1 ) return 0;
    instance_get_bbox( my, bmp1, &bbox1 );

    int64_t ctype = LOCQWORD( libmod_gfx, my, CTYPE );

    /* Checks only for a single instance */
    if ( id >= FIRST_INSTANCE_ID ) return ( ( ( ptr = instance_get( id ) ) && ctype == LOCQWORD( libmod_gfx, ptr, CTYPE ) ) ? colfunc( my, &bbox1, ptr ) : 0 );

    /* we must use full list of instances or get types from it */
    ptr = first_instance;

    if ( !id ) {
        LOCQWORD( libmod_gfx, my, COLLISION_RESERVED_TYPE_SCAN ) = 0;
        if ( ( p = LOCQWORD( libmod_gfx, my, COLLISION_RESERVED_ID_SCAN ) ) ) {
            ptr = instance_get( p );
            if ( ptr ) ptr = ptr->next;
        }

        while ( ptr ) {
            if ( ptr != my &&
                 ctype == LOCQWORD( libmod_gfx, ptr, CTYPE ) &&
                  (
                    ( status = ( LOCQWORD( libmod_gfx, ptr, STATUS ) & ~STATUS_WAITING_MASK ) ) == STATUS_RUNNING ||
                    status == STATUS_FROZEN
                  ) && colfunc( my, &bbox1, ptr )
                ) {
                LOCQWORD( libmod_gfx, my, COLLISION_RESERVED_ID_SCAN ) = LOCQWORD( libmod_gfx, ptr, PROCESS_ID );
                return LOCQWORD( libmod_gfx, ptr, PROCESS_ID );
            }
            ptr = ptr->next;
        }
        return 0;
    }

    LOCQWORD( libmod_gfx, my, COLLISION_RESERVED_ID_SCAN ) = 0;
    /* Check if already in scan by type and we reach limit */
    ctx = ( INSTANCE ** ) LOCADDR( libmod_gfx, my, COLLISION_RESERVED_CONTEXT );
    if ( LOCQWORD( libmod_gfx, my, COLLISION_RESERVED_TYPE_SCAN ) != id ) { /* Check if type change from last call */
        *ctx = NULL;
        LOCQWORD( libmod_gfx, my, COLLISION_RESERVED_TYPE_SCAN ) = id;
    }

    while ( ( ptr = instance_get_by_type( id, ctx ) ) ) {
        if ( ptr != my &&
             ctype == LOCQWORD( libmod_gfx, ptr, CTYPE ) &&
             (
                ( status = ( LOCQWORD( libmod_gfx, ptr, STATUS ) & ~STATUS_WAITING_MASK ) ) == STATUS_RUNNING ||
                  status == STATUS_FROZEN
             ) &&
             colfunc( my, &bbox1, ptr )
           ) {
            return LOCQWORD( libmod_gfx, ptr, PROCESS_ID );
        }
    }

    return 0;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_collision( INSTANCE * my, int64_t * params ) {
    return __collision( my, params[ 0 ], COLLISION_NORMAL );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_collision_box( INSTANCE * my, int64_t * params ) {
    return __collision( my, params[ 0 ], COLLISION_BOX );
}

/* --------------------------------------------------------------------------- */

int64_t libmod_gfx_collision_circle( INSTANCE * my, int64_t * params ) {
    return __collision( my, params[ 0 ], COLLISION_CIRCLE );
}
