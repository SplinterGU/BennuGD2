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

/* --------------------------------------------------------------------------- */
/* Based on PathFinding.js (https://github.com/qiao/PathFinding.js)            */
/* --------------------------------------------------------------------------- */

#include <stdlib.h>
#include <math.h>

#include "bgdrtm.h"
#include "bgddl.h"

#include "libbggfx.h"

/* --------------------------------------------------------------------------- */
/* Path find                                                                   */
/* --------------------------------------------------------------------------- */



/* --------------------------------------------------------------------------- */
/* Heuristic                                                                   */
/* --------------------------------------------------------------------------- */

/**
 * Manhattan distance.
 * @param {number} dx - Difference in x.
 * @param {number} dy - Difference in y.
 * @return {number} dx + dy
 */
static double manhattan(int dx, int dy) {
    return dx + dy;
}

/**
 * Euclidean distance.
 * @param {number} dx - Difference in x.
 * @param {number} dy - Difference in y.
 * @return {number} sqrt(dx * dx + dy * dy)
 */
static double euclidean(int dx, int dy) {
    return sqrt(dx * dx + dy * dy);
}

/**
 * Octile distance.
 * @param {number} dx - Difference in x.
 * @param {number} dy - Difference in y.
 * @return {number} sqrt(dx * dx + dy * dy) for grids
 */
static double octile(int dx, int dy) {
    double F = sqrt( 2.0f ) - 1.0f;
    return (dx < dy) ? F * dx + dy : F * dy + dx;
}

/**
 * Chebyshev distance.
 * @param {number} dx - Difference in x.
 * @param {number} dy - Difference in y.
 * @return {number} max(int dx, int dy)
 */
static double chebyshev(int dx, int dy) {
    return MAX( dx, dy);
}

/* --------------------------------------------------------------------------- */
/* Grid                                                                        */
/* --------------------------------------------------------------------------- */

GRID * path_new( GRAPH * gr ) {
    if ( !gr ) return NULL;
    SDL_Surface * surface;

#ifdef USE_SDL2
    if ( !gr->surface || gr->surface->format->BitsPerPixel == 1 ) return NULL;
#endif
#ifdef USE_SDL2_GPU
    if ( !gr->tex ) return NULL;
#endif

    GRID * grid = ( GRID * ) calloc( 1, sizeof( GRID ) );
    if ( !grid ) return NULL;

#ifdef USE_SDL2
    surface = gr->surface;
#endif
#ifdef USE_SDL2_GPU
    surface = GPU_CopySurfaceFromImage( gr->tex );
    if ( !surface ) {
        free( grid );
        return NULL;
    }
#endif

    int w = grid->w = surface->w;
    int h = grid->h = surface->h;

    grid->matrix = calloc( grid->w * grid->h, sizeof( NODE ) );
    if ( !grid->matrix ) {
        free( grid );
#ifdef USE_SDL2_GPU
        SDL_FreeSurface( surface );
#endif
        return NULL;
    }

    int x, y;

    switch ( surface->format->BitsPerPixel ) {
        case 8: {
            uint8_t * mem, * pmem = ( (uint8_t *) surface->pixels );
            int pos = 0;
            for ( y = 0; y < h; y++, pmem += surface->pitch )
                for ( mem = pmem, x = 0; x < w; x++ ) {
                    grid->matrix[pos].walkable = *mem++ ? 0 : 1;
                    grid->matrix[pos].x = x;
                    grid->matrix[pos].y = y;
                    pos++;
                }
            break;
        }

        case 16: {
            uint8_t * pmem = ( (uint8_t *) surface->pixels );
            uint16_t * mem;
            int pos = 0;
            for ( y = 0; y < h; y++, pmem += surface->pitch )
                for ( mem = ( uint16_t * ) pmem, x = 0; x < w; x++ ) {
                    grid->matrix[pos].walkable = ( *mem++ & ~( ( uint16_t )surface->format->Amask ) ) ? 0 : 1;
                    grid->matrix[pos].x = x;
                    grid->matrix[pos].y = y;
                    pos++;
                }
            break;
        }

        case 24: {
            uint8_t * mem, * pmem = ( (uint8_t *) surface->pixels );
            int pos = 0;
            for ( y = 0; y < h; y++, pmem += surface->pitch )
                for ( mem = pmem, x = 0; x < w; x++ ) {
                    grid->matrix[pos].walkable = ( ( *mem++ << surface->format->Rmask ) | ( *mem++ << surface->format->Gmask ) | ( *mem++ << surface->format->Bmask ) ) ? 0 : 1;
                    grid->matrix[pos].x = x;
                    grid->matrix[pos].y = y;
                    pos++;
                }
            break;
        }

        case 32: {
            uint8_t * pmem = ( (uint8_t *) surface->pixels );
            uint32_t * mem;
            int pos = 0;
            for ( y = 0; y < h; y++, pmem += surface->pitch )
                for ( mem = ( uint32_t * ) pmem, x = 0; x < w; x++ ) {
                    grid->matrix[pos].walkable = ( *mem++ & ~( ( uint32_t ) surface->format->Amask ) ) ? 0 : 1;
                    grid->matrix[pos].x = x;
                    grid->matrix[pos].y = y;
                    pos++;
                }
            break;
        }
    }
#ifdef USE_SDL2_GPU
    SDL_FreeSurface( surface );
#endif
    return ( void * ) grid;
}

/* --------------------------------------------------------------------------- */

void path_destroy( GRID * grid ) {
    if ( grid ) {
        free( grid->matrix );
        free( grid );
    }
}

/* --------------------------------------------------------------------------- */

static void resetGrid( GRID * g ) {
    int i, l = g->h * g->w;
    for ( i = 0; i < l; i++) {
        g->matrix[i].f = 0;
        g->matrix[i].g = 0;
        g->matrix[i].h = 0;
        g->matrix[i].opened = 0;
        g->matrix[i].parent = NULL;
        g->matrix[i].next = NULL;
        g->matrix[i].prev = NULL;
    }
}

/* --------------------------------------------------------------------------- */
/*                                                                             */
/* --------------------------------------------------------------------------- */

/* Uso: pf_open = add(pf_open, this) ; */
/* La lista permanecerá ordenada */
static NODE * node_add( NODE * list, NODE * this ) {
    NODE * curr = list;
    if ( !curr ) {
        this->next = NULL;
        this->prev = NULL;
        return this;
    }

    if ( curr->f > this->f ) {
        this->next = curr;
        this->prev = NULL;
        return this;
    }

    for ( ; curr->next; ) {
        if ( curr->next->f > this->f ) {
            this->next = curr->next;
            this->prev = curr;
            curr->next->prev = this;
            curr->next = this;
            return list;
        }
        curr = curr->next;
    }

    curr->next = this;
    this->prev = curr;
    this->next = NULL;
    return list;
}

/* --------------------------------------------------------------------------- */

/* Uso: pf_open = remove(pf_open, this) ; */
static NODE * node_remove( NODE * list, NODE * this ) {
    if ( this->next ) this->next->prev = this->prev;
    if ( this->prev ) this->prev->next = this->next;
    if ( list == this ) return this->next;
    return list;
}

/* --------------------------------------------------------------------------- */

static int64_t * return_path_results( NODE * nodeA, NODE * nodeB ) {
    int count = 0;
    NODE * idx = nodeA, * idxA = nodeA;
    idx->next = nodeB;  // reuse next for avoid invert results
    do {
        if ( idx->parent ) idx->parent->next = idx;
        idxA = idx;
        idx = idx->parent;
        count++;
    } while( idx );
    idx = nodeB;
    idx->next = idx->parent;
    do {
        idx = idx->next = idx->parent;
        count++;
    } while( idx );

    int64_t * results = ( int64_t * ) calloc( ( count + 1 ) * 2, sizeof( int64_t ) ), * p;
    if ( !results ) return NULL;
    p = results;

    while( idxA ) {
        *p++ = idxA->x;
        *p++ = idxA->y;
        idxA = idxA->next;
    }
    *p++ = -1;
    *p++ = -1;

    return results;
}

/* --------------------------------------------------------------------------- */
/*                                                                             */
/* --------------------------------------------------------------------------- */

#define BY_START    1
#define BY_END      2

#define checkNeighbor(grid,node,sx,sy,dx,dy,iam,opener_target,list)  \
            if ( (sx) >= 0 && (sx) < grid->w && (sy) >= 0 && (sy) < grid->h && ( neighbor = &grid->matrix[(sx)+(sy)*grid->w] )->walkable && !neighbor->closed ) {  \
                if ( neighbor->opened == opener_target ) {  \
                    /*  Found, make results and return */ \
                    if ( iam == BY_START ) return return_path_results( node, neighbor ); \
                    return return_path_results( neighbor, node ); \
                }  \
                int nx = neighbor->x, ny = neighbor->y;  \
                /* get the distance between current node and the neighbor \
                   and calculate the next g score */ \
                ng = node->g + ((nx - node->x == 0 || ny - node->y == 0) ? 1 : sqrt( 2.0f )); \
                /* check if the neighbor has not been inspected yet, or \
                   can be reached with smaller cost from the current node */ \
                if (!neighbor->opened || ng < neighbor->g) { \
                    neighbor->g = ng; \
                    if ( !neighbor->h ) neighbor->h = weight * heuristic(abs(nx - dx), abs(ny - dy)); \
                    neighbor->f = neighbor->g + neighbor->h; \
                    neighbor->parent = node; \
                    if (!neighbor->opened) { \
                        neighbor->opened = iam; \
                        list = node_add(list,neighbor); \
                    } else { \
                        /* the neighbor can be reached with smaller cost. \
                           Since its f value has been updated, we have to \
                           update its position in the open list */ \
                        list = node_remove(list,neighbor); \
                        list = node_add(list,neighbor); \
                    } \
                } \
            }

/* --------------------------------------------------------------------------- */
/* Bidirectional AStart finder                                                 */
/* --------------------------------------------------------------------------- */

int64_t * path_find_biastar( GRID * grid, int startX, int startY, int endX, int endY, int options, int weight, double ( * heuristic )( int dx, int dy ) ) {
    if ( !grid ||
         startX < 0 || startX >= grid->w ||
         startY < 0 || startY >= grid->h ||
         endX   < 0 || endX   >= grid->w ||
         endY   < 0 || endY   >= grid->h )
        return NULL;

    NODE * startOpenList = NULL, * endOpenList = NULL,
         * startNode = &grid->matrix[startX + startY * grid->w ],
         * endNode = &grid->matrix[endX + endY * grid->w ],
         * node,
         * neighbor;

    double ng;

    if ( weight <= 1 ) weight = 1;

    resetGrid( grid );

    // set the `g` and `f` value of the start node to be 0
    // and push it into the start open list
    startNode->g = 0;
    startNode->f = 0;
    startOpenList = node_add(startOpenList,startNode);
    startNode->opened = BY_START;

    // set the `g` and `f` value of the end node to be 0
    // and push it into the open open list
    endNode->g = 0;
    endNode->f = 0;
    endOpenList = node_add(endOpenList,endNode);
    endNode->opened = BY_END;

    // while both the open lists are not empty
    while ( startOpenList && endOpenList ) {

        // pop the position of start node which has the minimum `f` value.
        node = startOpenList;
        node->closed = 1;
        startOpenList = node_remove( startOpenList, node );

        checkNeighbor(grid,node,node->x,node->y-1,endX,endY,BY_START,BY_END,startOpenList);
        checkNeighbor(grid,node,node->x+1,node->y,endX,endY,BY_START,BY_END,startOpenList);
        checkNeighbor(grid,node,node->x,node->y+1,endX,endY,BY_START,BY_END,startOpenList);
        checkNeighbor(grid,node,node->x-1,node->y,endX,endY,BY_START,BY_END,startOpenList);
        if ( options & PF_DIAG ) {
            checkNeighbor(grid,node,node->x-1,node->y-1,startX,startY,BY_START,BY_END,startOpenList);
            checkNeighbor(grid,node,node->x+1,node->y-1,startX,startY,BY_START,BY_END,startOpenList);
            checkNeighbor(grid,node,node->x+1,node->y+1,startX,startY,BY_START,BY_END,startOpenList);
            checkNeighbor(grid,node,node->x-1,node->y+1,startX,startY,BY_START,BY_END,startOpenList);
        }

        // pop the position of end node which has the minimum `f` value.
        node = endOpenList;
        node->closed = 1;
        endOpenList = node_remove( endOpenList, node );

        checkNeighbor(grid,node,node->x,node->y-1,startX,startY,BY_END,BY_START,endOpenList);
        checkNeighbor(grid,node,node->x+1,node->y,startX,startY,BY_END,BY_START,endOpenList);
        checkNeighbor(grid,node,node->x,node->y+1,startX,startY,BY_END,BY_START,endOpenList);
        checkNeighbor(grid,node,node->x-1,node->y,startX,startY,BY_END,BY_START,endOpenList);
        if ( options & PF_DIAG ) {
            checkNeighbor(grid,node,node->x-1,node->y-1,startX,startY,BY_END,BY_START,endOpenList);
            checkNeighbor(grid,node,node->x+1,node->y-1,startX,startY,BY_END,BY_START,endOpenList);
            checkNeighbor(grid,node,node->x+1,node->y+1,startX,startY,BY_END,BY_START,endOpenList);
            checkNeighbor(grid,node,node->x-1,node->y+1,startX,startY,BY_END,BY_START,endOpenList);
        }
    } // end while not open list empty

    // fail to find the path
    return NULL;
}

/* --------------------------------------------------------------------------- */

int64_t * path_find( GRID * grid, int startX, int startY, int endX, int endY, int options, int weight, int heuristic ) {

    double ( * heuristic_fn )( int dx, int dy );

    switch ( heuristic ) {
        default:
        case PF_HEURISTIC_MANHATTAN:
            if ( options & PF_DIAG )    heuristic_fn = octile; // If diagonals not manhattan allowed
            else                        heuristic_fn = manhattan;
            break;

        case PF_HEURISTIC_EUCLIDEAN:
            heuristic_fn = euclidean;
            break;

        case PF_HEURISTIC_OCTILE:
            heuristic_fn = octile;
            break;

        case PF_HEURISTIC_CHEBYSHEV:
            heuristic_fn = chebyshev;
            break;
    }

    return path_find_biastar( grid, startX, startY, endX, endY, options, weight, heuristic_fn );
}

/* --------------------------------------------------------------------------- */
