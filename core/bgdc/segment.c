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

#define MAX_SEGMENTS 1024

static int64_t max_id = 0;
static int64_t free_id[MAX_SEGMENTS];
static int64_t free_count = 0;

static segment ** segments = 0;
static int64_t segments_reserved = 0;

segment * globaldata, * localdata;

/**
 * Registers a segment in the global array for later use.
 *
 * @param s The segment to register.
 */

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

/**
 * Creates a new segment.
 *
 * @return A pointer to the newly created segment.
 */

segment * segment_new() {
	/* Creates the segment */

	segment * s = (segment *) calloc(1, sizeof(segment));
	if (!s) {
        compile_error("segment_new: out of memory\n");
        return NULL; /* Avoid scan-build warning */
    }

	if (free_count) s->id = free_id[--free_count];
	else            s->id = max_id++;

    s->current = 0; // Initialize the displacement within the segment
    s->reserved = 256; // Set the initial capacity

	s->bytes = calloc(s->reserved, sizeof(char)); /* Size in bytes */
	if (!s->bytes) compile_error("segment_new: out of memory\n");

	segment_register (s);
	return s;
}

/**
 * Creates a duplicate segment based on an existing segment.
 *
 * @param b The segment to duplicate.
 *
 * @return A pointer to the newly created duplicate segment.
 */

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

	s->bytes = calloc(s->reserved, sizeof(char)) ; /* Size in bytes */
	if (!s->bytes) {
        free( s );  /* Avoid scan-build warning */
        compile_error("segment_new: out of memory\n");
        return NULL; /* Avoid scan-build warning */
    }
	memcpy(s->bytes, b->bytes, s->current);

	segment_register(s);
	return s;
}

/**
 * Destroys a segment and releases its resources.
 *
 * @param s The segment to destroy.
 */

void segment_destroy(segment * s) {
	segments[s->id] = 0;
	if (free_count < MAX_SEGMENTS) free_id[free_count++] = s->id;

	free (s->bytes);
	free (s);
}

/**
 * Allocates memory for a segment to increase its capacity.
 *
 * @param n The segment to allocate memory for.
 * @param count The additional capacity to allocate in bytes.
 */

void segment_alloc(segment * n, int64_t count) {
	n->reserved += count;
	n->bytes = realloc(n->bytes, n->reserved);
	if (!n->bytes) compile_error("segment_alloc: out of memory\n");
}

/**
 * Ensures a segment has sufficient capacity to accommodate additional data.
 *
 * This function checks if the current capacity of the segment is sufficient
 * to hold an additional specified number of bytes. If the capacity is
 * insufficient, it allocates more memory to meet the required capacity.
 *
 * @param n The segment for which to ensure capacity.
 * @param count The additional capacity required in bytes.
 *
 * @note If the current capacity is already sufficient or if the reallocation
 *       fails due to a lack of memory, an error message is displayed.
 */

void segment_ensure_capacity(segment * n, int64_t count) {
	if ( n->current + count > n->reserved ) segment_alloc( n, ( 255 + count - ( n->reserved - n->current ) ) & ~255LL );
}

/**
 * Adds a value to a segment with the specified base type.
 *
 * @param n The segment to add the value to.
 * @param value The value to add.
 * @param t The base type of the value.
 *
 * @return The updated displacement within the segment.
 */

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

/**
 * Adds data from one segment to another.
 *
 * @param n The target segment to add data to.
 * @param s The source segment to copy data from.
 *
 * @return The updated displacement within the target segment.
 */

int64_t segment_add_from(segment * n, segment * s) {
	segment_ensure_capacity(n, s->current);
	memcpy((uint8_t *)n->bytes + n->current, s->bytes, s->current);
	return n->current += s->current;
}

/**
 * Adds a signed byte to a segment.
 * @param n The segment to add the byte to.
 * @param value The byte value to add.
 * @return The updated offset from the start of the added item within the segment.
 */

int64_t segment_add_byte(segment * n, int8_t value) {
	segment_ensure_capacity(n, 1);
	*((int8_t *)n->bytes + n->current) = value;
	return n->current++;
}

/**
 * Adds a word (2 bytes) to a segment.
 *
 * @param n The segment to add the word to.
 * @param value The word value to add.
 *
 * @return The updated offset from the start of the added item within the segment.
 */

int64_t segment_add_word(segment * n, int16_t value) {
	segment_ensure_capacity(n, 2);
	*(int16_t *)((uint8_t *)n->bytes + n->current) = value;
	n->current += 2;
	return n->current - 2;
}

/**
 * Adds a double word (4 bytes) to a segment.
 *
 * @param n The segment to add the double word to.
 * @param value The double word value to add.
 *
 * @return The updated offset from the start of the added item within the segment.
 */

int64_t segment_add_dword(segment * n, int32_t value) {
	segment_ensure_capacity(n, 4);
	*(int32_t *)((uint8_t *)n->bytes + n->current) = value;
	n->current += 4;
	return n->current - 4;
}

/**
 * Adds a quad word (8 bytes) to a segment.
 *
 * @param n The segment to add the quad word to.
 * @param value The quad word value to add.
 *
 * @return The updated offset from the start of the added item within the segment.
 */

int64_t segment_add_qword(segment * n, int64_t value) {
	segment_ensure_capacity(n, 8);
	*(int64_t *)((uint8_t *)n->bytes + n->current) = value;
	n->current += 8;
	return n->current - 8;
}

/**
 * Gets a segment by its identifier.
 *
 * @param id The identifier of the segment to retrieve.
 *
 * @return A pointer to the segment with the specified identifier, or NULL if not found.
 */

segment * segment_get(int64_t id) {
	return segments[id];
}

/**
 * Dumps the content of a segment in hexadecimal format.
 *
 * @param s The segment to dump.
 */

void segment_dump(segment * s) {
	int i;
	for (i = 0 ; i < s->current ; i++) printf ("%02X  ", *((uint8_t*)s->bytes + i));
	printf ("\n");
}

/**
 * Copies data from one part of a segment to another.
 *
 * @param s The target segment.
 * @param base_offset The starting offset of the source data within the segment.
 * @param total_length The total length of data to be copied.
 */

void segment_copy(segment *s, int64_t base_offset, int64_t total_length) {
	segment_ensure_capacity(s, total_length);
	memcpy((uint8_t *)s->bytes + s->current, (uint8_t *)s->bytes + base_offset, total_length);
	s->current += total_length;
}

/**
 * Fill data with 0.
 *
 * @param s The target segment.
 * @param total_length The total length of data to be filled.
 */

void segment_fill(segment *s, int64_t total_length) {
	segment_ensure_capacity(s, total_length);
	memset((uint8_t *)s->bytes + s->current, '\0', total_length);
	s->current += total_length;
}

/**
 * Creates a new VARIABLE structure and initializes it.
 *
 * @return A pointer to the newly created VARIABLE structure.
 */

VARIABLE * variable_new() {
	VARIABLE * v = (VARIABLE *) calloc (1, sizeof(VARIABLE));
	if (!v) compile_error ("variable_new: out of memory\n");
	return v;
}

/* Named Segments */

static segment **   named_segs = NULL;
static int64_t *    named_codes = NULL;
static int64_t      named_count = 0;
static int64_t      named_reserved = 0;

/**
 * Retrieves a segment based on its associated code.
 *
 * @param code The code associated with the segment.
 *
 * @return A pointer to the segment with the specified code, or NULL if not found.
 */

segment * segment_by_name(int64_t code) {
	int n;
	for (n = 0 ; n < named_count ; n++) if (named_codes[n] == code) return named_segs[n];
	return NULL;
}

/**
 * Associates a segment with a code for easy retrieval.
 *
 * @param s The segment to be associated with a code.
 * @param code The code to associate with the segment.
 */

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
