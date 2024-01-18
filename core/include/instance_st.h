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

#ifndef __INSTANCE_ST_H
#define __INSTANCE_ST_H

#define FIRST_INSTANCE_ID   0x00010000
#define LAST_INSTANCE_ID    0x0001ffff

#define STACK_RETURN_VALUE  0x8000
#define STACK_SIZE_MASK     0x7FFF
#define STACK_SIZE          4096

/* ---------------------------------------------------------------------- */
/* Instances. An instance is created from a process, but in reality,      */
/* it is independent of the original process.                             */
/* ---------------------------------------------------------------------- */

typedef struct _instance {
    void * locdata;
    void * pridata;
    void * pubdata;

    int64_t * code;
    int64_t * codeptr;

    int64_t exitcode;
    int64_t errorcode;

    int64_t call_level;

    struct _procdef  * proc;

    int64_t private_size;
    int64_t public_size;

    int64_t first_run;

    /* General list - unsorted */

    struct _instance * next;
    struct _instance * prev;

    /* Linked list by priority */

    struct _instance * next_by_priority;
    struct _instance * prev_by_priority;
    int64_t last_priority;

    /* Linked list by process_type */

    struct _instance * next_by_type;
    struct _instance * prev_by_type;

    /* Linked list by INSTANCE * */

    struct _instance * next_by_instance;
    struct _instance * prev_by_instance;

    /* Function support */

    struct _instance * called_by;

    /* The first integer at the stack is the stack size,
       with optional NO_RETURN_VALUE mask. The stack contents follows */

    int64_t * stack;
    int64_t * stack_ptr;

    /* For debugging */

    int64_t switchval;
    int64_t switchval_string;
    int64_t cased;

    int64_t breakpoint;

} INSTANCE;

/* Macros for accessing local or private instance data. */

#define LOCQWORD(a,b)   ( *(uint64_t *) ((uint8_t *)(a->locdata)+b) )
#define LOCDWORD(a,b)   ( *(uint32_t *) ((uint8_t *)(a->locdata)+b) )
#define LOCWORD(a,b)    ( *(uint16_t *) ((uint8_t *)(a->locdata)+b) )
#define LOCBYTE(a,b)    ( *(uint8_t  *) ((uint8_t *)(a->locdata)+b) )

#define LOCINT64(a,b)   ( *(int64_t  *) ((uint8_t *)(a->locdata)+b) )
#define LOCINT32(a,b)   ( *(int32_t  *) ((uint8_t *)(a->locdata)+b) )
#define LOCINT16(a,b)   ( *(int16_t  *) ((uint8_t *)(a->locdata)+b) )
#define LOCINT8(a,b)    ( *(int8_t   *) ((uint8_t *)(a->locdata)+b) )

#define LOCUINT64(a,b)  LOCQWORD(a,b)
#define LOCUINT32(a,b)  LOCDWORD(a,b)
#define LOCUINT16(a,b)  LOCWORD(a,b)
#define LOCUINT8(a,b)   LOCBYTE(a,b)

#define LOCFLOAT(a,b)   ( *(float   *) ((uint8_t *)(a->locdata)+b) )
#define LOCDOUBLE(a,b)  ( *(double  *) ((uint8_t *)(a->locdata)+b) )


#define PRIQWORD(a,b)   ( *(uint64_t *) ((uint8_t *)(a->pridata)+b) )
#define PRIDWORD(a,b)   ( *(uint32_t *) ((uint8_t *)(a->pridata)+b) )
#define PRIWORD(a,b)    ( *(uint16_t *) ((uint8_t *)(a->pridata)+b) )
#define PRIBYTE(a,b)    ( *(uint8_t  *) ((uint8_t *)(a->pridata)+b) )

#define PRIINT64(a,b)   ( *(int64_t  *) ((uint8_t *)(a->pridata)+b) )
#define PRIINT32(a,b)   ( *(int32_t  *) ((uint8_t *)(a->pridata)+b) )
#define PRIINT16(a,b)   ( *(int16_t  *) ((uint8_t *)(a->pridata)+b) )
#define PRIINT8(a,b)    ( *(int8_t   *) ((uint8_t *)(a->pridata)+b) )

#define PRIUINT64(a,b)  PRIQWORD(a,b)
#define PRIUINT32(a,b)  PRIDWORD(a,b)
#define PRIUINT16(a,b)  PRIWORD(a,b)
#define PRIUINT8(a,b)   PRIBYTE(a,b)

#define PRIFLOAT(a,b)   ( *(float    *) ((uint8_t *)(a->pridata)+b) )
#define PRIDOUBLE(a,b)  ( *(double   *) ((uint8_t *)(a->pridata)+b) )


#define PUBQWORD(a,b)   ( *(uint64_t *) ((uint8_t *)(a->pubdata)+b) )
#define PUBDWORD(a,b)   ( *(uint32_t *) ((uint8_t *)(a->pubdata)+b) )
#define PUBWORD(a,b)    ( *(uint16_t *) ((uint8_t *)(a->pubdata)+b) )
#define PUBBYTE(a,b)    ( *(uint8_t  *) ((uint8_t *)(a->pubdata)+b) )

#define PUBINT64(a,b)   ( *(int64_t  *) ((uint8_t *)(a->pubdata)+b) )
#define PUBINT32(a,b)   ( *(int32_t  *) ((uint8_t *)(a->pubdata)+b) )
#define PUBINT16(a,b)   ( *(int16_t  *) ((uint8_t *)(a->pubdata)+b) )
#define PUBINT8(a,b)    ( *(int8_t   *) ((uint8_t *)(a->pubdata)+b) )

#define PUBUINT64(a,b)  PUBQWORD(a,b)
#define PUBUINT32(a,b)  PUBDWORD(a,b)
#define PUBUINT16(a,b)  PUBWORD(a,b)
#define PUBUINT8(a,b)   PUBBYTE(a,b)

#define PUBFLOAT(a,b)   ( *(float    *) ((uint8_t *)(a->pubdata)+b) )
#define PUBDOUBLE(a,b)  ( *(double   *) ((uint8_t *)(a->pubdata)+b) )


#define GLOQWORD(b)     ( *(uint64_t *) ((uint8_t *)(globaldata)+b) )
#define GLODWORD(b)     ( *(uint32_t *) ((uint8_t *)(globaldata)+b) )
#define GLOWORD(b)      ( *(uint16_t *) ((uint8_t *)(globaldata)+b) )
#define GLOBYTE(b)      ( *(uint8_t  *) ((uint8_t *)(globaldata)+b) )

#define GLOINT64(b)     ( *(int64_t  *) ((uint8_t *)(globaldata)+b) )
#define GLOINT32(b)     ( *(int32_t  *) ((uint8_t *)(globaldata)+b) )
#define GLOINT16(b)     ( *(int16_t  *) ((uint8_t *)(globaldata)+b) )
#define GLOINT8(b)      ( *(int8_t   *) ((uint8_t *)(globaldata)+b) )

#define GLOUINT64(b)    GLOQWORD(b)
#define GLOUINT32(b)    GLODWORD(b)
#define GLOUINT16(b)    GLOWORD(b)
#define GLOUINT8(b)     GLOBYTE(b)

#define GLOFLOAT(b)     ( *(float    *) ((uint8_t *)(globaldata)+b) )
#define GLODOUBLE(b)    ( *(double   *) ((uint8_t *)(globaldata)+b) )

#endif
