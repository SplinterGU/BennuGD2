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

#ifndef __TYPEDEF_ST_H
#define __TYPEDEF_ST_H

/* Data types */

typedef enum {
    TYPE_UNDEFINED = 0,

    TYPE_INT        , // 1   64 bits - NUMERIC - INTEGER
    TYPE_QWORD      , // 2   64 bits - NUMERIC - INTEGER - UNSIGNED
    TYPE_INT32      , // 3   32 bits - NUMERIC - INTEGER
    TYPE_DWORD      , // 4   32 bits - NUMERIC - INTEGER - UNSIGNED
    TYPE_SHORT	    , // 5   16 bits - NUMERIC - INTEGER
    TYPE_WORD	    , // 6   16 bits - NUMERIC - INTEGER - UNSIGNED
    TYPE_SBYTE	    , // 7    8 bits - NUMERIC - INTEGER
    TYPE_BYTE	    , // 8    8 bits - NUMERIC - INTEGER - UNSIGNED
    TYPE_CHAR       , // 9    8 bits - NUMERIC
    TYPE_FLOAT	    , // 10  32 bits - NUMERIC
    TYPE_DOUBLE     , // 11  64 bits - NUMERIC
    TYPE_STRING     , // 12  variant
    TYPE_ARRAY	    , // 13  variant
    TYPE_STRUCT	    , // 14  variant
    TYPE_POINTER    , // 15  32/64 bits
} BASETYPE ;

typedef struct _typechunk {
    BASETYPE   type ;
    int        count ;	/* for type == TYPE_ARRAY */
} TYPECHUNK ;

#define MAX_TYPECHUNKS 8

typedef struct _typedef {
    TYPECHUNK          chunk[MAX_TYPECHUNKS] ;
    int                depth ;
    struct _varspace * varspace ;
} TYPEDEF ;

#define typedef_is_numeric(t)   ((t).chunk[0].type < TYPE_STRING)
#define typedef_is_integer(t)   ((t).chunk[0].type < TYPE_CHAR)
#define typedef_is_float(t)     ((t).chunk[0].type == TYPE_FLOAT)
#define typedef_is_double(t)    ((t).chunk[0].type == TYPE_DOUBLE)
#define typedef_is_string(t)    ((t).chunk[0].type == TYPE_STRING)
#define typedef_is_struct(t)    ((t).chunk[0].type == TYPE_STRUCT)
#define typedef_is_array(t)     ((t).chunk[0].type == TYPE_ARRAY)
#define typedef_is_pointer(t)   ((t).chunk[0].type == TYPE_POINTER)
#define typedef_count(t)        ((t).chunk[0].count)
#define typedef_base(t)         ((t).chunk[0].type)
#define typedef_members(t)      ((t).varspace)
#define typedef_is_unsigned(t)	((t).chunk[0].type <= TYPE_CHAR && (t).chunk[0].type > TYPE_UNDEFINED && !((t).chunk[0].type & 1))

#endif
