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

#include <stdio.h>
#include <stdlib.h>

#include "bgddl.h"
#include "dlvaracc.h"

#include "libbggfx.h"
#include "resolution.h"

/* --------------------------------------------------------------------------- */

/* Indicadores de bloqueo */
#define GRAPH_HWRAP 1
#define GRAPH_VWRAP 2
#define BACK_HWRAP 4
#define BACK_VWRAP 8

/* --------------------------------------------------------------------------- */

int64_t scrolls_objects[ 10 ] = { 0 };

scrolldata scrolls[ 10 ];

/* --------------------------------------------------------------------------- */

static void draw_scroll( void * what, REGION * clip );
static int info_scroll( void * what, REGION * clip, int64_t * z, int64_t * drawme );

/* --------------------------------------------------------------------------- */

void scroll_region( int64_t n, REGION * r ) {
    if ( n < 0 || n > 9 ) return;

    /* Corrected from x,y... to posx,posy... so out_region works fine */
    r->x  -= scrolls[n].posx0;
    r->y  -= scrolls[n].posy0;
    r->x2 -= scrolls[n].posx0;
    r->y2 -= scrolls[n].posy0;
}

/* --------------------------------------------------------------------------- */

void scroll_start( int64_t n, int64_t fileid, int64_t graphid, int64_t filebackid, int64_t backid, int64_t region, int64_t flags, int64_t destfile, int64_t destid ) {
    SCROLL_EXTRA_DATA * data;

    if ( n >= 0 && n <= 9 ) {
        if ( region < 0 || region > 31 ) region = 0;

        scrolls[n].active       = 1;
        scrolls[n].fileid       = fileid;
        scrolls[n].graphid      = graphid;
        scrolls[n].filebackid   = filebackid;
        scrolls[n].backid       = backid;
        scrolls[n].region       = &regions[region];
        scrolls[n].flags        = flags;
        scrolls[n].destfile     = destfile;
        scrolls[n].destid       = destid;

        data = &(( SCROLL_EXTRA_DATA * ) &GLOQWORD( libbggfx, SCROLLS ) )[n];

        data->reserved[0] = ( int64_t ) &scrolls[n]; /* First reserved qword point to internal scrolldata struct */

        if ( scrolls_objects[n] ) gr_destroy_object( scrolls_objects[n] );
        scrolls_objects[n] = ( int64_t ) gr_new_object( 0, info_scroll, draw_scroll, ( void * ) n );
    }
}

/* --------------------------------------------------------------------------- */

void scroll_stop( int64_t n ) {
    if ( n >= 0 && n <= 9 ) {
        if ( scrolls_objects[n] ) {
            gr_destroy_object( scrolls_objects[n] );
            scrolls_objects[n] = 0;
            scrolls[n].active = 0;
        }
    }
}

/* --------------------------------------------------------------------------- */

void scroll_update( int64_t n ) {
    int64_t x0, y0, x1, y1, cx, cy, w, h, speed;

    REGION bbox;
    GRAPH * gr, * graph, * back;

    SCROLL_EXTRA_DATA * data;

    if ( n < 0 || n > 9 ) return;

    if ( !scrolls[n].active || !scrolls[n].region || !scrolls[n].graphid ) return;

    graph = scrolls[n].graphid ? bitmap_get( scrolls[n].fileid, scrolls[n].graphid ) : NULL;
    back  = scrolls[n].backid  ? bitmap_get( scrolls[n].filebackid, scrolls[n].backid )  : NULL;

    if (                        !graph ) return; // El fondo de scroll no existe
    if (  scrolls[n].backid  && !back  ) return; // Grafico no existe

    data = &(( SCROLL_EXTRA_DATA * ) &GLOQWORD( libbggfx, SCROLLS ) )[n];

    w = scrolls[n].region->x2 - scrolls[n].region->x + 1;
    h = scrolls[n].region->y2 - scrolls[n].region->y + 1;

    scrolls[n].z       = data->z;
    scrolls[n].ratio   = data->ratio;
    scrolls[n].camera  = instance_get( data->camera );
    scrolls[n].speed   = data->speed;

    if ( data->follows < 0 || data->follows > 9 )   scrolls[n].follows = 0;
    else                                            scrolls[n].follows = &scrolls[data->follows];

    if ( data->region1 < 0 || data->region1 > 31 )  scrolls[n].region1 = 0;
    else                                            scrolls[n].region1 = &regions[data->region1];

    if ( data->region2 < 0 || data->region2 > 31 )  scrolls[n].region2 = 0;
    else                                            scrolls[n].region2 = &regions[data->region2];

    /* Actualiza las variables globales (perseguir la camara, etc) */

    if ( scrolls[n].follows ) {
        if ( scrolls[n].ratio ) {
            data->x0 = scrolls[n].follows->x0 * 100 / scrolls[n].ratio;
            data->y0 = scrolls[n].follows->y0 * 100 / scrolls[n].ratio;
        }
        else {
            data->x0 = scrolls[n].follows->x0;
            data->y0 = scrolls[n].follows->y0;
        }
    }

    if ( scrolls[n].camera ) {
        /* Mira a ver si entra dentro de la region 1 o 2 */

        speed = scrolls[n].speed;
        if ( scrolls[n].speed == 0 ) speed = 9999999;

        /* Update speed */

        if (( gr = instance_graph( scrolls[n].camera ) ) ) {
            instance_get_bbox( scrolls[n].camera, gr, &bbox );

            x0 = bbox.x - data->x0;
            y0 = bbox.y - data->y0;
            x1 = bbox.x2 - data->x0;
            y1 = bbox.y2 - data->y0;

            if ( scrolls[n].region1 &&
                    (
                        x0 < scrolls[n].region1->x2 && y0 < scrolls[n].region1->y2 &&
                        x1 > scrolls[n].region1->x  && y1 > scrolls[n].region1->y
                    )
               ) {
                speed = 0;
            }
            else
                if ( scrolls[n].region2 ) {
                    if ( x0 > scrolls[n].region2->x2 ) speed = ( x0 - scrolls[n].region2->x2 );
                    if ( y0 > scrolls[n].region2->y2 ) speed = ( y0 - scrolls[n].region2->y2 );
                    if ( x1 < scrolls[n].region2->x  ) speed = ( scrolls[n].region2->x - x1  );
                    if ( y1 < scrolls[n].region2->y  ) speed = ( scrolls[n].region2->y - y1  );
                }
        }

        /* Forzar a que esté en el centro de la ventana */

        cx = LOCQWORD( libbggfx, scrolls[n].camera, COORDX );
        cy = LOCQWORD( libbggfx, scrolls[n].camera, COORDY );

        RESOLXY( libbggfx, scrolls[n].camera, cx, cy );

        cx -= w / 2;
        cy -= h / 2;

        if ( data->x0 < cx ) data->x0 = MIN( data->x0 + speed, cx );
        if ( data->y0 < cy ) data->y0 = MIN( data->y0 + speed, cy );
        if ( data->x0 > cx ) data->x0 = MAX( data->x0 - speed, cx );
        if ( data->y0 > cy ) data->y0 = MAX( data->y0 - speed, cy );
    }

    /* Scrolls no c�clicos y posici�n del background */

    if ( graph ) {
        if ( !( scrolls[n].flags & GRAPH_HWRAP ) ) data->x0 = MAX( 0, MIN( data->x0, ( int64_t )graph->width  - w ) );
        if ( !( scrolls[n].flags & GRAPH_VWRAP ) ) data->y0 = MAX( 0, MIN( data->y0, ( int64_t )graph->height - h ) );
    }

    if ( scrolls[n].ratio ) {
        data->x1 = data->x0 * 100 / scrolls[n].ratio;
        data->y1 = data->y0 * 100 / scrolls[n].ratio;
    }

    if ( back ) {
        if ( !( scrolls[n].flags & BACK_HWRAP ) ) data->x1 = MAX( 0, MIN( data->x1, ( int64_t )back->width  - w ) );
        if ( !( scrolls[n].flags & BACK_VWRAP ) ) data->y1 = MAX( 0, MIN( data->y1, ( int64_t )back->height - h ) );
    }

    /* Actualiza la posici�n del scroll seg�n las variables globales */

    scrolls[n].posx0 = data->x0;
    scrolls[n].posy0 = data->y0;
    scrolls[n].x0 = data->x0 % ( int64_t ) graph->width;
    scrolls[n].y0 = data->y0 % ( int64_t ) graph->height;

    if ( scrolls[n].x0 < 0 ) scrolls[n].x0 += graph->width;
    if ( scrolls[n].y0 < 0 ) scrolls[n].y0 += graph->height;

    if ( back ) {
        scrolls[n].x1 = data->x1 % ( int64_t ) back->width;
        scrolls[n].y1 = data->y1 % ( int64_t ) back->height;
        if ( scrolls[n].x1 < 0 ) scrolls[n].x1 += back->width;
        if ( scrolls[n].y1 < 0 ) scrolls[n].y1 += back->height;
    }
}

/* --------------------------------------------------------------------------- */

static int compare_instances( const void * ptr1, const void * ptr2 ) {
    const INSTANCE * i1 = *( const INSTANCE ** )ptr1;
    const INSTANCE * i2 = *( const INSTANCE ** )ptr2;

    int64_t ret = LOCQWORD( libbggfx, i2, COORDZ ) - LOCQWORD( libbggfx, i1, COORDZ );

    return !ret ? LOCQWORD( libbggfx, i1, PROCESS_ID ) - LOCQWORD( libbggfx, i2, PROCESS_ID ) : ret;
}

/* --------------------------------------------------------------------------- */

void scroll_draw( int64_t n, REGION * clipping ) {
    int64_t nproc, x, y, cx, cy;

    static INSTANCE ** proclist = 0;
    static int64_t proclist_reserved = 0;
    int64_t proclist_count;
    REGION r;
    int64_t status;

    GRAPH * graph, * back, * dest = NULL;

    SCROLL_EXTRA_DATA * data;
    INSTANCE * i;

    if ( n < 0 || n > 9 ) return;

    if ( !scrolls[n].active || !scrolls[n].region || !scrolls[n].graphid ) return;

    graph = scrolls[n].graphid ? bitmap_get( scrolls[n].fileid, scrolls[n].graphid ) : NULL;
    back  = scrolls[n].backid  ? bitmap_get( scrolls[n].filebackid, scrolls[n].backid )  : NULL;

    if (                        !graph ) return; // El fondo de scroll no existe
    if (  scrolls[n].backid  && !back  ) return; // Grafico no existe

    data = &(( SCROLL_EXTRA_DATA * ) &GLOQWORD( libbggfx, SCROLLS ) )[n];

    dest = scrolls[n].destid ? bitmap_get( scrolls[n].destfile, scrolls[n].destid ) : NULL;
    if ( dest ) {
        /* Clear screen */
        SDL_SetRenderTarget( gRenderer, dest->texture );

        SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
        SDL_RenderClear( gRenderer );

        SDL_SetRenderTarget( gRenderer, NULL );
    }

    /* Dibuja el fondo */

    r = *scrolls[n].region;
    if ( !dest && clipping ) region_union( &r, clipping );

    if ( back ) {
        if ( back->ncpoints > 0 && back->cpoints[0].x >= 0 ) {
            cx = back->cpoints[0].x;
            cy = back->cpoints[0].y;
        }
        else {
            cx = back->width / 2;
            cy = back->height / 2;
        }

        y = scrolls[n].region->y - scrolls[n].y1;

        while ( y < scrolls[n].region->y2 ) {
            x = scrolls[n].region->x - scrolls[n].x1;
            while ( x < scrolls[n].region->x2 ) {
                gr_blit( dest, &r, x + cx, y + cy, data->flags2, 0, 100, 100, back, NULL, data->alpha2, data->color_r2, data->color_g2, data->color_b2 );
                x += back->width;
            }
            y += back->height;
        }
    }

    /* Dibuja el primer plano */

    if ( graph->ncpoints > 0 && graph->cpoints[0].x >= 0 ) {
        cx = graph->cpoints[0].x;
        cy = graph->cpoints[0].y;
    }
    else {
        cx = graph->width / 2;
        cy = graph->height / 2;
    }

    y = scrolls[n].region->y - scrolls[n].y0;
    while ( y < scrolls[n].region->y2 ) {
        x = scrolls[n].region->x - scrolls[n].x0;
        while ( x < scrolls[n].region->x2 ) {
            gr_blit( dest, &r, x + cx, y + cy, data->flags1, 0, 100, 100, graph, NULL, data->alpha, data->color_r, data->color_g, data->color_b );
            x += graph->width;
        }
        y += graph->height;
    }

    /* Crea una lista ordenada de instancias a dibujar */

    i = first_instance;
    proclist_count = 0;
    while ( i ) {
        if ( LOCQWORD( libbggfx, i, CTYPE ) == C_SCROLL &&
            (( status = ( LOCQWORD( libbggfx, i, STATUS ) & ~STATUS_WAITING_MASK ) ) == STATUS_RUNNING || status == STATUS_FROZEN )
           ) {
            if ( LOCQWORD( libbggfx, i, CNUMBER ) && !( LOCQWORD( libbggfx, i, CNUMBER ) & ( 1 << n ) ) ) {
                i = i->next;
                continue;
            }

            if ( proclist_count == proclist_reserved ) {
                proclist_reserved += 16;
                proclist = ( INSTANCE ** ) realloc( proclist, sizeof( INSTANCE * ) * proclist_reserved );
            }

            proclist[proclist_count++] = i;
        }
        i = i->next;
    }

    if ( proclist_count ) {
        /* Ordena la listilla */
        qsort( proclist, proclist_count, sizeof( INSTANCE * ), compare_instances );

        /* Visualiza los procesos */
        for ( nproc = 0; nproc < proclist_count; nproc++ ) {
            x = LOCQWORD( libbggfx, proclist[nproc], COORDX );
            y = LOCQWORD( libbggfx, proclist[nproc], COORDY );

            RESOLXY( libbggfx, proclist[nproc], x, y );
            draw_instance_at( proclist[nproc], &r, x - scrolls[n].posx0 + scrolls[n].region->x, y - scrolls[n].posy0 + scrolls[n].region->y, dest );
        }
    }
}

/* --------------------------------------------------------------------------- */

static void draw_scroll( void * what, REGION * clip ) {
    scroll_draw( ( int64_t ) what, clip );
}

/* --------------------------------------------------------------------------- */

static int info_scroll( void * what, REGION * clip, int64_t * z, int64_t * drawme ) {
    int64_t n = ( int64_t ) what;
    * z = scrolls[n].z;
    * drawme = 1;
    if ( clip ) * clip = * scrolls[n].region;
    scroll_update( n );
    return 1;
}

/* --------------------------------------------------------------------------- */
