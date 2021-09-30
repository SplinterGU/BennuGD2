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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bgdc.h"

static int64_t max_id = 0;
static int64_t free_id[1024];
static int64_t free_count = 0;

static segment ** segments = 0;
static int64_t segments_reserved = 0;

segment * globaldata, * localdata;

static void segment_register(segment * s) {
	/* Saves the segment in the global array */

	if (!segments) {
		segments = (segment **) calloc(16, sizeof(segment *));
		segments_reserved = 16;
	}
	if (segments_reserved <= s->id) {
		segments_reserved = s->id + 16;
		segments = (segment **) calloc(segments_reserved, sizeof(segment *));
	}
	if (!segments) compile_error("segment_new: out of memory\n");
	segments[s->id] = s;
}

segment * segment_new() {
	/* Creates the segment */

	segment * s = (segment *) calloc(1, sizeof(segment));
	if (!s) {
        compile_error("segment_new: out of memory\n");
        return NULL; /* Avoid scan-build warning */
    }

	if (free_count) s->id = free_id[--free_count];
	else            s->id = max_id++;

	s->current = 0;
	s->reserved = 256;

	s->bytes = calloc(s->reserved, sizeof(char)); /* Tama�o en bytes */
	if (!s->bytes) compile_error("segment_new: out of memory\n");

	segment_register (s);
	return s;
}

segment * segment_duplicate(segment * b) {
	segment * s = (segment *) calloc(1, sizeof(segment));
	if (!s) {
        compile_error("segment_new: out of memory\n");
        return NULL; /* Avoid scan-build warning */
    }

	if (free_count) s->id = free_id[--free_count];
	else            s->id = max_id++;

	s->current = b->current;
	s->reserved = b->reserved;

	s->bytes = calloc(s->reserved, sizeof(char)) ; /* Tama�o en bytes */
	if (!s->bytes) {
        free( s );  /* Avoid scan-build warning */
        compile_error("segment_new: out of memory\n");
        return NULL; /* Avoid scan-build warning */
    }
	memcpy(s->bytes, b->bytes, s->current);

	segment_register(s);
	return s;
}

void segment_destroy(segment * s) {
	segments[s->id] = 0;
	if (free_count < 1024) free_id[free_count++] = s->id;

	free (s->bytes);
	free (s);
}

void segment_alloc(segment * n, int64_t count) {
	n->reserved += count;
	n->bytes = realloc(n->bytes, n->reserved);
	if (!n->bytes) compile_error("segment_alloc: out of memory\n");
}

int64_t segment_add_as(segment * n, int64_t value, BASETYPE t) {
	switch (t) {
        case TYPE_QWORD:
		case TYPE_INT:
		case TYPE_DOUBLE:
		case TYPE_STRING:
		case TYPE_POINTER:
			return segment_add_qword(n, (int64_t)value);

		case TYPE_DWORD:
		case TYPE_INT32:
		case TYPE_FLOAT:
			return segment_add_dword(n, (int32_t)value);

		case TYPE_WORD:
		case TYPE_SHORT:
			return segment_add_word(n, (int16_t)value);

		case TYPE_BYTE:
		case TYPE_SBYTE:
		case TYPE_CHAR:
			return segment_add_byte(n, (int8_t)value);

		default:
			compile_error(MSG_INCOMP_TYPE);
			return 0;
	}
}

int64_t segment_add_from(segment * n, segment * s) {
	if (n->current+s->current >= n->reserved) segment_alloc(n, s->current);
	memcpy((uint8_t *)n->bytes + n->current, s->bytes, s->current);
	return n->current += s->current;
}

int64_t segment_add_byte(segment * n, int8_t value) {
	if (n->current+1 >= n->reserved) segment_alloc(n, 256);
	*((int8_t *)n->bytes + n->current) = value;
	return n->current++;
}

int64_t segment_add_word(segment * n, int16_t value) {
	if (n->current+2 >= n->reserved) segment_alloc(n, 256);
	*(int16_t *)((uint8_t *)n->bytes + n->current) = value;
	n->current += 2;
	return n->current - 2;
}

int64_t segment_add_dword(segment * n, int32_t value) {
	if (n->current+4 >= n->reserved) segment_alloc(n, 256);
	*(int32_t *)((uint8_t *)n->bytes + n->current) = value;
	n->current += 4;
	return n->current - 4;
}

int64_t segment_add_qword(segment * n, int64_t value) {
	if (n->current+8 >= n->reserved) segment_alloc(n, 256);
	*(int64_t *)((uint8_t *)n->bytes + n->current) = value;
	n->current += 8;
	return n->current - 8;
}

segment * segment_get(int64_t id) {
	return segments[id];
}

void segment_dump(segment * s) {
	int i;
	for (i = 0 ; i < s->current ; i++) printf ("%02X  ", *((uint8_t*)s->bytes + i));
	printf ("\n");
}

void segment_copy(segment *s, int64_t base_offset, int64_t total_length) {
	if (s->reserved < s->current + total_length) segment_alloc (s, total_length);
	memcpy((uint8_t *)s->bytes + s->current, (uint8_t *)s->bytes + base_offset, total_length);
	s->current += total_length;
}

VARIABLE * variable_new() {
	VARIABLE * v = (VARIABLE *) calloc (1, sizeof(VARIABLE));
	if (!v) compile_error ("variable_new: out of memory\n");
	return v;
}

/* Segmentos nombrados */

static segment **   named_segs = NULL;
static int64_t *    named_codes = NULL;
static int64_t      named_count = 0;
static int64_t      named_reserved = 0;

segment * segment_by_name(int64_t code) {
	int n;
	for (n = 0 ; n < named_count ; n++) if (named_codes[n] == code) return named_segs[n];
	return 0;
}

void segment_name(segment * s, int64_t code) {
    if (named_count >= named_reserved) {
        named_reserved += 16;
        named_segs = realloc(named_segs, named_reserved * sizeof (segment *));
	    if ( !named_segs ) compile_error( MSG_OUT_OF_MEMORY );
        named_codes = realloc(named_codes, named_reserved * sizeof (int64_t));
    	if ( !named_codes ) compile_error( MSG_OUT_OF_MEMORY );
        if (!named_segs || !named_codes) compile_error("segment_name: out of memory\n");
    }
	named_segs[named_count] = s;
	named_codes[named_count] = code;
	named_count++;
}
