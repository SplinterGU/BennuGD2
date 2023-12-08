/*
 *  Copyright (C) SplinterGU (Fenix/BennuGD) (Since 2006)
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

#ifndef __SEGMENT_H
#define __SEGMENT_H

#include <stdint.h>

#include "typedef.h"

/* A segment is a linear area of data that can grow dynamically */

typedef struct _segment {
    void * bytes;     // Pointer to the memory buffer.
    int64_t current;  // Current offset within the segment.
    int64_t reserved; // Total size reserved for the segment's data.
    int64_t id;       // Unique identifier for the segment.
} segment;

extern segment * segment_new();
extern segment * segment_duplicate(segment * s);
extern segment * segment_get(int64_t id);

/* They return the offset of the new data */
extern int64_t segment_add_as(segment * n, int64_t value, BASETYPE t);
extern int64_t segment_add_qword(segment * n, int64_t value);
extern int64_t segment_add_dword(segment * n, int32_t value);
extern int64_t segment_add_word(segment * n, int16_t value);
extern int64_t segment_add_byte(segment * n, int8_t  value);
extern int64_t segment_add_from(segment * n, segment * s);

extern void segment_dump(segment *);
extern void segment_destroy(segment *);
extern void segment_copy(segment *, int64_t base_offset, int64_t total_length);
extern void segment_fill(segment *s, int64_t total_length);
extern void segment_alloc(segment * n, int64_t count);
extern void segment_ensure_capacity(segment * n, int64_t count);

extern segment  * globaldata;
extern segment  * localdata;

/* Named segments (for user-defined data types) */

extern segment * segment_by_name (int64_t code);
extern void      segment_name(segment * s, int64_t code);

#endif
