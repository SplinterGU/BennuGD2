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

#ifndef __SCROLL_H
#define __SCROLL_H

/* --------------------------------------------------------------------------- */

#define C_SCROLL        1

#define MAX_SCROLLS     10

/* --------------------------------------------------------------------------- */

#ifndef __BGDC__

#ifdef _MSC_VER
#pragma pack(push, 1)
#endif

#ifdef __GNUC__
#define __PACKED __attribute__ ((packed))
#pragma pack(1)
#else
#define __PACKED
#define inline __inline
#endif

typedef struct _scrolldata {
    int64_t fileid;
    int64_t graphid;
    int64_t filebackid;
    int64_t backid;
    REGION * region;
    int64_t flags;

    int64_t x0, y0;
    int64_t posx0, posy0;
    int64_t x1, y1;
    int64_t z;

    INSTANCE * camera;

    int64_t ratio;
    int64_t speed;
    REGION * region1;
    REGION * region2;

    int64_t destfile;
    int64_t destid;

    int64_t active;

    struct _scrolldata * follows;
} __PACKED scrolldata;

typedef struct _scroll_Extra_data {
    int64_t x0;
    int64_t y0;
    int64_t x1;
    int64_t y1;
    int64_t z;
    int64_t camera;
    int64_t ratio;
    int64_t speed;
    int64_t region1;
    int64_t region2;
    int64_t flags1;
    int64_t flags2;
    int64_t follows;
    uint8_t alpha;
    uint8_t color_r;
    uint8_t color_g;
    uint8_t color_b;
    uint8_t alpha2;
    uint8_t color_r2;
    uint8_t color_g2;
    uint8_t color_b2;
    int64_t reserved[8];        /* First reserved dword point to internal scrolldata struct */
} __PACKED SCROLL_EXTRA_DATA;

#ifdef __GNUC__
#pragma pack()
#endif

#ifdef _MSC_VER
#pragma pack(pop)
#endif

/* --------------------------------------------------------------------------- */

extern scrolldata  scrolls[ MAX_SCROLLS ];

/* --------------------------------------------------------------------------- */

extern void scroll_start( int64_t n, int64_t fileid, int64_t graphid, int64_t filebackid, int64_t backid, int64_t region, int64_t flags, int64_t destfile, int64_t destid );
extern void scroll_stop( int64_t n );
extern void scroll_update( int64_t n );
extern void scroll_draw( int64_t n, REGION * clipping );
extern void scroll_region( int64_t n, REGION * r );

/* --------------------------------------------------------------------------- */

#endif
#endif
