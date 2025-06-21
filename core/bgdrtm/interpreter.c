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
#include <math.h>

#include <inttypes.h>

#include "bgdrtm.h"
#include "dcb.h"

#include "sysprocs_p.h"
#include "pslang.h"
#include "instance.h"
#include "offsets.h"
#include "xstrings.h"

/* ---------------------------------------------------------------------- */
/* Interpreter's main module                                              */
/* ---------------------------------------------------------------------- */

int64_t exit_value = 0;
int64_t must_exit = 0;

int64_t frame_completed = 0;

int64_t debugger_show_console = 0;          // debuggin
int64_t debugger_trace = 0;                 // 1 single sentence
int64_t debugger_step = 0;                  // execute 1 sentence or 1 procedure or 1 function as an unit
// int64_t debugger_step_out = 0;           // execute until return or exit procedure or function
// int64_t debugger_goto_next_instance = 0; // break on next instance
// int64_t debugger_next_frame = 0;         // run until next frame is complete

int64_t trace_sentence = -1;
INSTANCE * trace_instance = NULL;

/* ---------------------------------------------------------------------- */

/* Errors checks */

#define FATAL_ERROR_CHECK(cond, ...) \
    if ( cond ) { \
        fprintf( stderr, __VA_ARGS__ ); \
        exit( 0 ); \
    }

#define FATAL_ERROR_DIV_BY_ZERO_CHECK(cond)     FATAL_ERROR_CHECK( cond, "ERROR: Runtime error in %s(%" PRId64 ") - Division by zero\n", r->proc->name, LOCQWORD( r, PROCESS_ID ) )

#define GET_INSTANCE() \
    INSTANCE* i = instance_get( r->stack_ptr[-1] ); \
    FATAL_ERROR_CHECK( !i, "ERROR: Runtime error in %s(%" PRId64 ") - Process %" PRId64 " not active\n", r->proc->name, LOCQWORD( r, PROCESS_ID ), r->stack_ptr[-1] )

/* ---------------------------------------------------------------------- */

int64_t instance_go_all() {

    must_exit = 0;

    while ( first_instance ) {
        frame_completed = 0;

        // Reset iterator by priority
        instance_reset_iterator_by_priority(); // Don't must be neccessary
        INSTANCE* i = instance_next_by_priority();

        int64_t i_count = 0;
        while ( i ) {
            if ( LOCINT64( i, FRAME_PERCENT ) < 100 ) {
                int64_t status = LOCQWORD( i, STATUS );
                if ( status == STATUS_RUNNING ) {
                    /* Run instance */
                    /* Hook */
                    if ( process_exec_hook_count )
                        for ( int n = 0; n < process_exec_hook_count; n++ )
                            process_exec_hook_list[n]( i );
                    /* Hook */
                } else if ( status & ~( STATUS_KILLED | STATUS_DEAD ) ) { /* STATUS_SLEEPING OR STATUS_FROZEN OR STATUS_WAITING_MASK OR STATUS_PAUSED_MASK */
                    i = instance_next_by_priority();
                    continue;
                }
                /* If instance is KILLED or DEAD, run instance without exec_hook executed. */

                instance_go( i );

                i_count++;

                if ( must_exit ) goto instance_go_all_exit;

            }

            i = instance_next_by_priority();
        }

        /* If frame is complete, then update internal vars and execute main hooks. */

        if ( !i_count ) {
            frame_completed = 1;
            /* Honors the signal-changed status of the process and
             * saves so it is used in this loop the next frame
             */
            i = first_instance;
            while ( i ) {
                int64_t status = LOCQWORD( i, SAVED_STATUS ) = LOCQWORD( i, STATUS );
                // if status == STATUS_KILLED or STATUS_DEAD then the process still lives
                if ( status == STATUS_DEAD || status == STATUS_KILLED || status == STATUS_RUNNING ) LOCINT64( i, FRAME_PERCENT ) -= 100;

                if ( i->last_priority != LOCINT64( i, PRIORITY ) ) {
                    instance_dirty( i );
                    LOCINT64( i, SAVED_PRIORITY ) = LOCINT64( i, PRIORITY );
                }

                i = i->next;
            }

            if ( !first_instance ) break;

            /* Hook */
            if ( handler_hook_count )
                for ( int n = 0; n < handler_hook_count; n++ )
                    handler_hook_list[n].hook();
            /* Hook */

            continue;
        }
    }

instance_go_all_exit:

    return exit_value;

}

/* ---------------------------------------------------------------------- */

static int stack_dump( INSTANCE * r ) {
    register int64_t * stack_ptr = &r->stack[1];
    register int i = 0;

    while ( stack_ptr < r->stack_ptr ) {
        if ( i == 2 ) {
            i = 0;
            printf( "\n" );
        }
        printf( "%016"PRIX64" ", *stack_ptr++ );
        i++;
    }

    return i;
}

/* ---------------------------------------------------------------------- */

int64_t instance_go( INSTANCE * r ) {
    if ( !r ) return 0;

    register int64_t * pc = r->codeptr;

    uint64_t return_value = LOCQWORD( r, PROCESS_ID );

    uint64_t *status_ptr = &LOCQWORD( r, STATUS );

    /* Pointer to the current process's code (it may be a called one) */

    int64_t debugger_step_pending = 0; // local to instance

    /* ------------------------------------------------------------------------------- */
    /* Restore if exit by debug                                                        */

    if ( debug > 0 ) {
        printf( "\n>>> Instance:%s ProcID:%" PRId64 " StackUsed:%" PRId64 "/%" PRId64 "\n", r->proc->name,
                                                                 LOCQWORD( r, PROCESS_ID ),
                                                                 ( int64_t )
                                                                 ( ( void * ) r->stack_ptr - ( void * ) r->stack ) / sizeof( r->stack[0] ),
                                                                 ( r->stack[0] & ~STACK_RETURN_VALUE )
              );
    }

    /* Start process or return from frame */
    /* Hook */
    if ( instance_pre_execute_hook_count )
        for ( int n = 0; n < instance_pre_execute_hook_count; n++ )
            instance_pre_execute_hook_list[n]( r );
    /* Hook */

    // breakpoint on entry
    if ( r->proc->breakpoint || r->breakpoint ) {
        debugger_show_console = 1; // *** call debugger when info is available
        debugger_trace = 1;
    }

main_loop_instance_go:
    trace_sentence = -1;

    while ( !must_exit ) {
        /* If I was killed or I'm waiting status, then exit */
        uint64_t status = *status_ptr;
        if ( status & ( STATUS_KILLED | STATUS_WAITING_MASK | STATUS_PAUSED_MASK ) ) {
            r->codeptr = pc;
            return_value = LOCQWORD( r, PROCESS_ID );
            goto break_all;
        }

        if ( trace_sentence != -1 ) {
             while( debugger_show_console ) {
                /* Hook */
                if ( handler_hook_count )
                    for ( int n = 0; n < handler_hook_count; n++ )
                        handler_hook_list[n].hook();
                /* Hook */
            }
        }

        /* debug output */
        if ( debug > 0 ) {
            if ( debug > 2 ) {
                int c = 34 - stack_dump( r ) * 17;
                if ( debug > 1 ) printf( "%*.*s[%4" PRIu64 "] ", c, c, "", ( uint64_t ) ( pc - r->code ) );
            }
            else if ( debug > 1 ) printf( "[%4" PRIu64 "] ", ( uint64_t ) ( pc - r->code ) );
            mnemonic_dump( *pc, pc[1] );
            fflush(stdout);
        }

        switch ( *pc ) {

            /* No operation */
            case MN_NOP:
                ++pc;
                break;

            /* Stack manipulation */

            case MN_DUP:
                *r->stack_ptr = r->stack_ptr[-1];
                ++r->stack_ptr;
                ++pc;
                break;

            case MN_PUSH:
                *r->stack_ptr++ = pc[1];
                pc += 2;
                break;

            case MN_PUSH | MN_STRING:
            {
                uint64_t n = pc[1];
                *r->stack_ptr++ = n;
                string_use( n );
                pc += 2;
                break;
            }

            case MN_POP:
                --r->stack_ptr;
                ++pc;
                break;

            case MN_POP | MN_STRING:
                --r->stack_ptr;
                string_discard( *r->stack_ptr );
                ++pc;
                break;

            // array[var] (maybe data type is nonsense)
            case MN_INDEX | MN_QWORD:
            case MN_INDEX | MN_UNSIGNED | MN_QWORD:
            case MN_INDEX | MN_DWORD:
            case MN_INDEX | MN_UNSIGNED | MN_DWORD:
            case MN_INDEX | MN_WORD:
            case MN_INDEX | MN_UNSIGNED | MN_WORD:
            case MN_INDEX | MN_BYTE:
            case MN_INDEX | MN_UNSIGNED | MN_BYTE:
            case MN_INDEX | MN_STRING:
            case MN_INDEX | MN_FLOAT:
            case MN_INDEX | MN_DOUBLE:
                r->stack_ptr[-1] += pc[1];
                pc += 2;
                break;

            case MN_ARRAY:
                --r->stack_ptr;
                r->stack_ptr[-1] += pc[1] * r->stack_ptr[0];
                pc += 2;
                break;

            /* Process calls */

            case MN_CLONE:
            {
                INSTANCE* i = instance_duplicate( r );
                i->codeptr = pc + 2;
                pc = r->code + pc[1];
                break;
            }

#define OP_PROC_CALL(case_value, stack_error, stack_run, stack_child_alive) \
case case_value: \
{ \
    PROCDEF * proc = procdef_get( pc[1] ); \
    FATAL_ERROR_CHECK( !proc, "ERROR: Runtime error in %s(%" PRId64 ") - Unknown process\n", r->proc->name, LOCQWORD( r, PROCESS_ID ) ) \
    \
    /* Process uses FRAME or locals, must create an instance */ \
    INSTANCE* i = instance_new( proc, r ); \
    if ( !i ) { \
        /* Can't create instante, return -1 */ \
        r->stack_ptr -= proc->params; \
        stack_error \
        pc += 2; \
        break; \
    } \
    \
    for ( int n = 0; n < proc->params; n++ ) \
        PRIQWORD( i, sizeof( uint64_t ) * n ) = r->stack_ptr[-proc->params+n]; \
    \
    r->stack_ptr -= proc->params; \
    \
    /* I go to waiting status (by default) */ \
    *status_ptr |= STATUS_WAITING_MASK; \
    i->called_by   = r; \
    \
    if ( debugger_step ) { \
        debugger_step_pending = 1; \
        debugger_step = 0; \
    } \
    \
    /* Run the process/function */ \
    stack_run \
    \
    if ( debugger_step_pending ) { \
        debugger_step_pending = 0; \
        debugger_step = 1; \
    } \
    \
    int64_t child_is_alive = instance_exists( i ); \
    pc += 2; \
    \
    /* If the process is a function in a frame, save the stack and leave */ \
    /* If the process/function still running, then it is in a FRAME. */ \
    /* If the process/function is running code, then it his status is RUNNING */ \
    if ( child_is_alive && *status_ptr & ( STATUS_WAITING_MASK | STATUS_FROZEN | STATUS_SLEEPING ) ) { \
        /* I go to sleep and return from this process/function */ \
        i->called_by   = r; \
        \
        /* Save the instruction pointer */ \
        /* This instance don't run other code until the child return */ \
        r->codeptr = pc; \
        \
        /* If it don't was a CALL, then I set a flag in "len" for no return value */ \
        stack_child_alive \
        return 0; \
    } \
    \
    /* Wake up! */ \
    *status_ptr &= ~STATUS_WAITING_MASK; \
    if ( child_is_alive ) i->called_by = NULL; \
    \
    break; \
}

            // MN_CALL
            OP_PROC_CALL(
                MN_CALL,
                /* stack_error */
                r->stack[0] |= STACK_RETURN_VALUE; \
                *r->stack_ptr = -1; \
                ++r->stack_ptr;,

                /* stack_run */
                r->stack[0] |= STACK_RETURN_VALUE; \
                ++r->stack_ptr; \
                *r->stack_ptr = instance_go( i );,

                /* stack_child_alive */
                r->stack[0] |= STACK_RETURN_VALUE;
            )

            // MN_PROC
            OP_PROC_CALL(
                MN_PROC,
                /* stack_error */
                r->stack[0] &= ~STACK_RETURN_VALUE;,

                /* stack_run */
                r->stack[0] &= ~STACK_RETURN_VALUE; \
                instance_go( i );,

                /* stack_child_alive */
                r->stack[0] &= ~STACK_RETURN_VALUE;
            )

            case MN_SYSCALL:
            {
                SYSPROC * p = sysproc_get( pc[1] );
                FATAL_ERROR_CHECK( !p, "ERROR: Runtime error in %s(%" PRId64 ") - Unknown system function\n", r->proc->name, LOCQWORD( r, PROCESS_ID ) )
                r->stack_ptr -= p->params;
                *r->stack_ptr = ( *p->func )( r, r->stack_ptr );
                ++r->stack_ptr;
                pc += 2;
                break;
            }

            case MN_SYSPROC:
            {
                SYSPROC * p = sysproc_get( pc[1] );
                FATAL_ERROR_CHECK( !p, "ERROR: Runtime error in %s(%" PRId64 ") - Unknown system process\n", r->proc->name, LOCQWORD( r, PROCESS_ID ) )
                r->stack_ptr -= p->params;
                ( *p->func )( r, r->stack_ptr );
                pc += 2;
                break;
            }

#define CASE_ALL_TYPES(case_value) \
case case_value | MN_QWORD: \
case case_value | MN_UNSIGNED | MN_QWORD: \
case case_value | MN_DWORD: \
case case_value | MN_UNSIGNED | MN_DWORD: \
case case_value | MN_WORD: \
case case_value | MN_UNSIGNED | MN_WORD: \
case case_value | MN_BYTE: \
case case_value | MN_UNSIGNED | MN_BYTE: \
case case_value | MN_STRING: \
case case_value | MN_DOUBLE: \
case case_value | MN_FLOAT:

            /* Access to variables address */

#define OP_VAR_PTR(case_value, get_macro) \
CASE_ALL_TYPES(case_value) { \
    *r->stack_ptr++ = ( uint64_t )( intptr_t )&get_macro( r, pc[1] ); \
    pc += 2; \
    break; \
}

            OP_VAR_PTR(MN_PRIVATE, PRIQWORD)
            OP_VAR_PTR(MN_PUBLIC, PUBQWORD)
            OP_VAR_PTR(MN_LOCAL, LOCQWORD)

            CASE_ALL_TYPES(MN_GLOBAL)
                *r->stack_ptr++ = ( uint64_t )( intptr_t )&GLOQWORD( pc[1] );
                pc += 2;
                break;

#define OP_VAR_REMOTE_PTR(case_value, get_macro) \
CASE_ALL_TYPES(case_value) { \
    GET_INSTANCE() \
    r->stack_ptr[-1] = ( uint64_t )( intptr_t )&get_macro( i, pc[1] ); \
    pc += 2; \
    break; \
}

            OP_VAR_REMOTE_PTR(MN_REMOTE, LOCQWORD)
            OP_VAR_REMOTE_PTR(MN_REMOTE_PUBLIC, PUBQWORD)

            /***********************/
            /* Access to variables */
            /***********************/

#define OP_GET(case_value, get_macro) \
case case_value: \
{ \
    *r->stack_ptr++ = get_macro(r, pc[1]); \
    pc += 2; \
    break; \
}

#define OP_GETS(case_value, get_macro) \
case case_value: \
{ \
    uint64_t string_id = get_macro( r, pc[1] ); \
    *r->stack_ptr++ = string_id; \
    string_use( string_id ); \
    pc += 2; \
    break; \
}

            /*******************************/
            /* Access to private variables */
            /*******************************/

            case MN_GET_PRIV | MN_UNSIGNED | MN_QWORD:
            case MN_GET_PRIV | MN_DOUBLE:
            OP_GET(MN_GET_PRIV | MN_QWORD, PRIQWORD)
            OP_GET(MN_GET_PRIV | MN_DWORD, PRIINT32)
            case MN_GET_PRIV | MN_FLOAT:
            OP_GET(MN_GET_PRIV | MN_UNSIGNED | MN_DWORD, PRIDWORD)
            OP_GET(MN_GET_PRIV | MN_WORD, PRIINT16)
            OP_GET(MN_GET_PRIV | MN_UNSIGNED | MN_WORD, PRIWORD)
            OP_GET(MN_GET_PRIV | MN_BYTE, PRIINT8)
            OP_GET(MN_GET_PRIV | MN_UNSIGNED | MN_BYTE, PRIBYTE)
            OP_GETS(MN_GET_PRIV | MN_STRING, PRIQWORD)

            /******************************/
            /* Access to public variables */
            /******************************/

            case MN_GET_PUBLIC | MN_UNSIGNED | MN_QWORD:
            case MN_GET_PUBLIC | MN_DOUBLE:
            OP_GET(MN_GET_PUBLIC | MN_QWORD, PUBQWORD)
            OP_GET(MN_GET_PUBLIC | MN_DWORD, PUBINT32)
            case MN_GET_PUBLIC | MN_FLOAT:
            OP_GET(MN_GET_PUBLIC | MN_UNSIGNED | MN_DWORD, PUBDWORD)
            OP_GET(MN_GET_PUBLIC | MN_WORD, PUBINT16)
            OP_GET(MN_GET_PUBLIC | MN_UNSIGNED | MN_WORD, PUBWORD)
            OP_GET(MN_GET_PUBLIC | MN_BYTE, PUBINT8)
            OP_GET(MN_GET_PUBLIC | MN_UNSIGNED | MN_BYTE, PUBBYTE)
            OP_GETS(MN_GET_PUBLIC | MN_STRING, PUBQWORD)

            /*****************************/
            /* Access to local variables */
            /*****************************/

            case MN_GET_LOCAL | MN_UNSIGNED | MN_QWORD:
            case MN_GET_LOCAL | MN_DOUBLE:
            OP_GET(MN_GET_LOCAL | MN_QWORD, LOCQWORD)
            OP_GET(MN_GET_LOCAL | MN_DWORD, LOCINT32)
            case MN_GET_LOCAL | MN_FLOAT:
            OP_GET(MN_GET_LOCAL | MN_UNSIGNED | MN_DWORD, LOCDWORD)
            OP_GET(MN_GET_LOCAL | MN_WORD, LOCINT16)
            OP_GET(MN_GET_LOCAL | MN_UNSIGNED | MN_WORD, LOCWORD)
            OP_GET(MN_GET_LOCAL | MN_BYTE, LOCINT8)
            OP_GET(MN_GET_LOCAL | MN_UNSIGNED | MN_BYTE, LOCBYTE)
            OP_GETS(MN_GET_LOCAL | MN_STRING, LOCQWORD)

            /******************************/
            /* Access to global variables */
            /******************************/

#define OP_GET_GLOBAL(case_value, get_macro) \
case case_value: \
{ \
    *r->stack_ptr++ = get_macro(pc[1]); \
    pc += 2; \
    break; \
}

            case MN_GET_GLOBAL | MN_UNSIGNED | MN_QWORD:
            case MN_GET_GLOBAL | MN_DOUBLE:
            OP_GET_GLOBAL(MN_GET_GLOBAL | MN_QWORD, GLOQWORD)
            OP_GET_GLOBAL(MN_GET_GLOBAL | MN_DWORD, GLOINT32)
            case MN_GET_GLOBAL | MN_FLOAT:
            OP_GET_GLOBAL(MN_GET_GLOBAL | MN_UNSIGNED | MN_DWORD, GLODWORD)
            OP_GET_GLOBAL(MN_GET_GLOBAL | MN_WORD, GLOINT16)
            OP_GET_GLOBAL(MN_GET_GLOBAL | MN_UNSIGNED | MN_WORD, GLOWORD)
            OP_GET_GLOBAL(MN_GET_GLOBAL | MN_BYTE, GLOINT8)
            OP_GET_GLOBAL(MN_GET_GLOBAL | MN_UNSIGNED | MN_BYTE, GLOBYTE)

            case MN_GET_GLOBAL | MN_STRING:
            {
                uint64_t string_id = GLOQWORD( pc[1] );
                *r->stack_ptr++ = string_id;
                string_use( string_id );
                pc += 2;
                break;
            }

            /******************************/
            /* Access to remote variables */
            /******************************/

#define OP_GET_REMOTE(case_value, get_macro) \
case case_value: \
{ \
    GET_INSTANCE() \
    r->stack_ptr[-1] = get_macro(i, pc[1]); \
    pc += 2; \
    break; \
}

#define OP_GET_REMOTES(case_value, get_macro) \
case case_value: \
{ \
    GET_INSTANCE() \
    uint64_t string_id = get_macro( i, pc[1] ); \
    r->stack_ptr[-1] = string_id; \
    string_use( string_id ); \
    pc += 2; \
    break; \
}

            case MN_GET_REMOTE | MN_UNSIGNED | MN_QWORD:
            case MN_GET_REMOTE | MN_DOUBLE:
            OP_GET_REMOTE(MN_GET_REMOTE | MN_QWORD, LOCQWORD)
            OP_GET_REMOTE(MN_GET_REMOTE | MN_DWORD, LOCINT32)
            case MN_GET_REMOTE | MN_FLOAT:
            OP_GET_REMOTE(MN_GET_REMOTE | MN_UNSIGNED | MN_DWORD, LOCDWORD)
            OP_GET_REMOTE(MN_GET_REMOTE | MN_WORD, LOCINT16)
            OP_GET_REMOTE(MN_GET_REMOTE | MN_UNSIGNED | MN_WORD, LOCWORD)
            OP_GET_REMOTE(MN_GET_REMOTE | MN_BYTE, LOCINT8)
            OP_GET_REMOTE(MN_GET_REMOTE | MN_UNSIGNED | MN_BYTE, LOCBYTE)
            OP_GET_REMOTES(MN_GET_REMOTE | MN_STRING, LOCQWORD)

            /*************************************/
            /* Access to remote public variables */
            /*************************************/

            case MN_GET_REMOTE_PUBLIC | MN_UNSIGNED | MN_QWORD:
            case MN_GET_REMOTE_PUBLIC | MN_DOUBLE:
            OP_GET_REMOTE(MN_GET_REMOTE_PUBLIC | MN_QWORD, PUBQWORD)
            OP_GET_REMOTE(MN_GET_REMOTE_PUBLIC | MN_DWORD, PUBINT32)
            case MN_GET_REMOTE_PUBLIC | MN_FLOAT:
            OP_GET_REMOTE(MN_GET_REMOTE_PUBLIC | MN_UNSIGNED | MN_DWORD, PUBDWORD)
            OP_GET_REMOTE(MN_GET_REMOTE_PUBLIC | MN_WORD, PUBINT16)
            OP_GET_REMOTE(MN_GET_REMOTE_PUBLIC | MN_UNSIGNED | MN_WORD, PUBWORD)
            OP_GET_REMOTE(MN_GET_REMOTE_PUBLIC | MN_BYTE, PUBINT8)
            OP_GET_REMOTE(MN_GET_REMOTE_PUBLIC | MN_UNSIGNED | MN_BYTE, PUBBYTE)
            OP_GET_REMOTES(MN_GET_REMOTE_PUBLIC | MN_STRING, PUBQWORD)

            /*******************************/
            /* Access to pointer variables */
            /*******************************/

#define OP_PTR(case_value, ctype) \
case case_value: \
{ \
    r->stack_ptr[-1] = *( ctype * )( intptr_t )r->stack_ptr[-1]; \
    ++pc; \
    break; \
}

            case MN_PTR | MN_UNSIGNED | MN_QWORD:
            case MN_PTR | MN_DOUBLE:
            OP_PTR(MN_PTR | MN_QWORD, int64_t)
            OP_PTR(MN_PTR | MN_DWORD, int32_t)
            case MN_PTR | MN_FLOAT:
            OP_PTR(MN_PTR | MN_UNSIGNED | MN_DWORD, uint32_t)
            OP_PTR(MN_PTR | MN_WORD, int16_t)
            OP_PTR(MN_PTR | MN_UNSIGNED | MN_WORD, uint16_t)
            OP_PTR(MN_PTR | MN_BYTE, int8_t)
            OP_PTR(MN_PTR | MN_UNSIGNED | MN_BYTE, uint8_t)

            case MN_PTR | MN_STRING:
            {
                uint64_t string_id = *( int64_t * )( intptr_t )r->stack_ptr[-1];
                r->stack_ptr[-1] = string_id;
                string_use( string_id );
                ++pc;
                break;
            }

            /*********/
            /* Maths */
            /*********/

#define OP_EXPRF(case_value, ctype, oper) \
case case_value: \
{ \
    *( ctype * )&r->stack_ptr[-1] = ( ctype ) oper *(( ctype * ) &r->stack_ptr[-1] ); \
    ++pc; \
    break; \
}

#define OP_EXPRF_WITH_ARG(case_value, ctype, oper) \
case case_value: \
{ \
    *( ctype * )&r->stack_ptr[-2] oper##= *(( ctype * ) &r->stack_ptr[-1] ); \
    --r->stack_ptr; \
    ++pc; \
    break; \
}

#define OP_EXPRF_WITH_ARG_MOD(case_value, ctype) \
case case_value: \
{ \
    *( ctype * )&r->stack_ptr[-2] = fmod( *( ctype * )&r->stack_ptr[-2], *(( ctype * ) &r->stack_ptr[-1] ) ); \
    --r->stack_ptr; \
    ++pc; \
    break; \
}

#define OP_EXPR(case_value, oper) \
case case_value: \
{ \
    r->stack_ptr[-1] = oper r->stack_ptr[-1]; \
    ++pc; \
    break; \
}

#define OP_EXPR_WITH_ARG_BODY(ctype, oper) \
    r->stack_ptr[-2] oper##= ( ctype ) r->stack_ptr[-1]; \
    --r->stack_ptr; \
    ++pc; \
    break;

#define OP_EXPR_WITH_ARG(case_value, ctype, oper) \
case case_value: \
{ \
    OP_EXPR_WITH_ARG_BODY(ctype, oper) \
}

#define OP_EXPR_WITH_ARG_DIVMOD(case_value, ctype, oper) \
case case_value: \
{ \
    FATAL_ERROR_DIV_BY_ZERO_CHECK( !r->stack_ptr[-1] ) \
    OP_EXPR_WITH_ARG_BODY(ctype, oper) \
}

#define OP_EXPR_WITH_ARG_DIV(case_value, ctype) OP_EXPR_WITH_ARG_DIVMOD(case_value, ctype, /)
#define OP_EXPR_WITH_ARG_MOD(case_value, ctype) OP_EXPR_WITH_ARG_DIVMOD(case_value, ctype, %)

            case MN_NEG | MN_UNSIGNED | MN_QWORD:
            OP_EXPR(MN_NEG | MN_QWORD, -)
            OP_EXPRF(MN_NEG | MN_DOUBLE, double, -)
            OP_EXPRF(MN_NEG | MN_FLOAT, float, -)

            case MN_NOT | MN_UNSIGNED | MN_QWORD:
            OP_EXPR(MN_NOT | MN_QWORD, !)
            OP_EXPRF(MN_NOT | MN_DOUBLE, double, !)
            OP_EXPRF(MN_NOT | MN_FLOAT, float, !)

            OP_EXPR_WITH_ARG(MN_ADD | MN_QWORD, int64_t, +)
            OP_EXPR_WITH_ARG(MN_ADD | MN_UNSIGNED | MN_QWORD, uint64_t, +)
            OP_EXPRF_WITH_ARG(MN_ADD | MN_DOUBLE, double, +)
            OP_EXPRF_WITH_ARG(MN_ADD | MN_FLOAT, float, +)

            case MN_ADD | MN_STRING:
            {
                uint64_t string_id1 = r->stack_ptr[-2], string_id2 = r->stack_ptr[-1];
                int64_t n = string_add( string_id1, string_id2 );
                string_use( n );
                string_discard( string_id1 );
                string_discard( string_id2 );
                --r->stack_ptr;
                r->stack_ptr[-1] = n;
                ++pc;
                break;
            }

            OP_EXPR_WITH_ARG(MN_SUB | MN_QWORD, int64_t, -)
            OP_EXPR_WITH_ARG(MN_SUB | MN_UNSIGNED | MN_QWORD, uint64_t, -)
            OP_EXPRF_WITH_ARG(MN_SUB | MN_DOUBLE, double, -)
            OP_EXPRF_WITH_ARG(MN_SUB | MN_FLOAT, float, -)

            case MN_MUL | MN_DWORD:
            case MN_MUL | MN_WORD:
            case MN_MUL | MN_BYTE:
            OP_EXPR_WITH_ARG(MN_MUL | MN_QWORD, int64_t, *)
            case MN_MUL| MN_UNSIGNED | MN_DWORD:
            case MN_MUL| MN_UNSIGNED | MN_WORD:
            case MN_MUL| MN_UNSIGNED | MN_BYTE:
            OP_EXPR_WITH_ARG(MN_MUL | MN_UNSIGNED | MN_QWORD, uint64_t, *)
            OP_EXPRF_WITH_ARG(MN_MUL | MN_DOUBLE, double, *)
            OP_EXPRF_WITH_ARG(MN_MUL | MN_FLOAT, float, *)

            case MN_DIV | MN_DWORD:
            case MN_DIV | MN_WORD:
            case MN_DIV | MN_BYTE:
            OP_EXPR_WITH_ARG_DIV(MN_DIV | MN_QWORD, int64_t)
            case MN_DIV | MN_UNSIGNED | MN_DWORD:
            case MN_DIV | MN_UNSIGNED | MN_WORD:
            case MN_DIV | MN_UNSIGNED | MN_BYTE:
            OP_EXPR_WITH_ARG_DIV(MN_DIV | MN_UNSIGNED | MN_QWORD, uint64_t)
            OP_EXPRF_WITH_ARG(MN_DIV | MN_DOUBLE, double, /)
            OP_EXPRF_WITH_ARG(MN_DIV | MN_FLOAT, float, /)

            OP_EXPR_WITH_ARG_MOD(MN_MOD | MN_QWORD, int64_t)
            OP_EXPR_WITH_ARG_MOD(MN_MOD | MN_UNSIGNED | MN_QWORD, uint64_t)
            OP_EXPR_WITH_ARG_MOD(MN_MOD | MN_DWORD, int32_t)
            OP_EXPR_WITH_ARG_MOD(MN_MOD | MN_UNSIGNED | MN_DWORD, uint32_t)
            OP_EXPR_WITH_ARG_MOD(MN_MOD | MN_WORD, int16_t)
            OP_EXPR_WITH_ARG_MOD(MN_MOD | MN_UNSIGNED | MN_WORD, uint16_t)
            OP_EXPR_WITH_ARG_MOD(MN_MOD | MN_BYTE, int8_t)
            OP_EXPR_WITH_ARG_MOD(MN_MOD | MN_UNSIGNED | MN_BYTE, uint8_t)
            OP_EXPRF_WITH_ARG_MOD(MN_MOD | MN_DOUBLE, double)
            OP_EXPRF_WITH_ARG_MOD(MN_MOD | MN_FLOAT, float)

            /* Bitwise operations */

            OP_EXPR_WITH_ARG(MN_ROR | MN_QWORD, int64_t, >>)
            OP_EXPR_WITH_ARG(MN_ROR | MN_UNSIGNED | MN_QWORD, uint64_t, >>)
            OP_EXPR_WITH_ARG(MN_ROR | MN_DWORD, int32_t, >>)
            OP_EXPR_WITH_ARG(MN_ROR | MN_UNSIGNED | MN_DWORD, uint32_t, >>)
            OP_EXPR_WITH_ARG(MN_ROR | MN_WORD, int16_t, >>)
            OP_EXPR_WITH_ARG(MN_ROR | MN_UNSIGNED | MN_WORD, uint16_t, >>)
            OP_EXPR_WITH_ARG(MN_ROR | MN_BYTE, int8_t, >>)
            OP_EXPR_WITH_ARG(MN_ROR | MN_UNSIGNED | MN_BYTE, uint8_t, >>)

            OP_EXPR_WITH_ARG(MN_ROL | MN_QWORD, int64_t, <<)
            OP_EXPR_WITH_ARG(MN_ROL | MN_UNSIGNED | MN_QWORD, uint64_t, <<)
            OP_EXPR_WITH_ARG(MN_ROL | MN_DWORD, int32_t, <<)
            OP_EXPR_WITH_ARG(MN_ROL | MN_UNSIGNED | MN_DWORD, uint32_t, <<)
            OP_EXPR_WITH_ARG(MN_ROL | MN_WORD, int16_t, <<)
            OP_EXPR_WITH_ARG(MN_ROL | MN_UNSIGNED | MN_WORD, uint16_t, <<)
            OP_EXPR_WITH_ARG(MN_ROL | MN_BYTE, int8_t, <<)
            OP_EXPR_WITH_ARG(MN_ROL | MN_UNSIGNED | MN_BYTE, uint8_t, <<)

            OP_EXPR_WITH_ARG(MN_BAND | MN_QWORD, int64_t, &)
            OP_EXPR_WITH_ARG(MN_BAND | MN_UNSIGNED | MN_QWORD, uint64_t, &)
            OP_EXPR_WITH_ARG(MN_BAND | MN_DWORD, int32_t, &)
            OP_EXPR_WITH_ARG(MN_BAND | MN_UNSIGNED | MN_DWORD, uint32_t, &)
            OP_EXPR_WITH_ARG(MN_BAND | MN_WORD, int16_t, &)
            OP_EXPR_WITH_ARG(MN_BAND | MN_UNSIGNED | MN_WORD, uint16_t, &)
            OP_EXPR_WITH_ARG(MN_BAND | MN_BYTE, int8_t, &)
            OP_EXPR_WITH_ARG(MN_BAND | MN_UNSIGNED | MN_BYTE, uint8_t, &)

            OP_EXPR_WITH_ARG(MN_BOR | MN_QWORD, int64_t, |)
            OP_EXPR_WITH_ARG(MN_BOR | MN_UNSIGNED | MN_QWORD, uint64_t, |)
            OP_EXPR_WITH_ARG(MN_BOR | MN_DWORD, int32_t, |)
            OP_EXPR_WITH_ARG(MN_BOR | MN_UNSIGNED | MN_DWORD, uint32_t, |)
            OP_EXPR_WITH_ARG(MN_BOR | MN_WORD, int16_t, |)
            OP_EXPR_WITH_ARG(MN_BOR | MN_UNSIGNED | MN_WORD, uint16_t, |)
            OP_EXPR_WITH_ARG(MN_BOR | MN_BYTE, int8_t, |)
            OP_EXPR_WITH_ARG(MN_BOR | MN_UNSIGNED | MN_BYTE, uint8_t, |)

            OP_EXPR_WITH_ARG(MN_BXOR | MN_QWORD, int64_t, ^)
            OP_EXPR_WITH_ARG(MN_BXOR | MN_UNSIGNED | MN_QWORD, uint64_t, ^)
            OP_EXPR_WITH_ARG(MN_BXOR | MN_DWORD, int32_t, ^)
            OP_EXPR_WITH_ARG(MN_BXOR | MN_UNSIGNED | MN_DWORD, uint32_t, ^)
            OP_EXPR_WITH_ARG(MN_BXOR | MN_WORD, int16_t, ^)
            OP_EXPR_WITH_ARG(MN_BXOR | MN_UNSIGNED | MN_WORD, uint16_t, ^)
            OP_EXPR_WITH_ARG(MN_BXOR | MN_BYTE, int8_t, ^)
            OP_EXPR_WITH_ARG(MN_BXOR | MN_UNSIGNED | MN_BYTE, uint8_t, ^)

            OP_EXPR(MN_BNOT | MN_QWORD, ( int64_t ) ~)
            OP_EXPR(MN_BNOT | MN_UNSIGNED | MN_QWORD, ( uint64_t ) ~)
            OP_EXPR(MN_BNOT | MN_DWORD, ( int32_t ) ~)
            OP_EXPR(MN_BNOT | MN_UNSIGNED | MN_DWORD, ( uint32_t ) ~)
            OP_EXPR(MN_BNOT | MN_WORD, ( int16_t ) ~)
            OP_EXPR(MN_BNOT | MN_UNSIGNED | MN_WORD, ( uint16_t ) ~)
            OP_EXPR(MN_BNOT | MN_BYTE, ( int8_t ) ~)
            OP_EXPR(MN_BNOT | MN_UNSIGNED | MN_BYTE, ( uint8_t ) ~)

            /* Logical operations */

#define OP_EXPR_WITH_ARG_NON_ASSIGN_OPERATOR(case_value, ctype, oper) \
case case_value: \
{ \
    r->stack_ptr[-2] = ( ctype ) r->stack_ptr[-2] oper ( ctype ) r->stack_ptr[-1]; \
    --r->stack_ptr; \
    ++pc; \
    break; \
}

            OP_EXPR_WITH_ARG_NON_ASSIGN_OPERATOR(MN_AND, int64_t, &&)
            OP_EXPR_WITH_ARG_NON_ASSIGN_OPERATOR(MN_OR, int64_t, ||)

            case MN_XOR:
            {
                r->stack_ptr[-2] = ( r->stack_ptr[-2] != 0 ) ^ ( r->stack_ptr[-1] != 0 );
                --r->stack_ptr;
                ++pc;
                break;
            }

            /* Comparisons */

#define OP_CMP(case_value, ctype, oper) OP_EXPR_WITH_ARG_NON_ASSIGN_OPERATOR(case_value, ctype, oper)

            OP_CMP(MN_EQ | MN_QWORD, int64_t, ==)
            OP_CMP(MN_NE | MN_QWORD, int64_t, !=)
            OP_CMP(MN_GTE | MN_QWORD, int64_t, >=)
            OP_CMP(MN_GTE | MN_UNSIGNED | MN_QWORD, uint64_t, >=)
            OP_CMP(MN_LTE | MN_QWORD, int64_t, <=)
            OP_CMP(MN_LTE | MN_UNSIGNED | MN_QWORD, uint64_t, <=)
            OP_CMP(MN_GT | MN_QWORD, int64_t, >)
            OP_CMP(MN_GT | MN_UNSIGNED | MN_QWORD, uint64_t, >)
            OP_CMP(MN_LT | MN_QWORD, int64_t, <)
            OP_CMP(MN_LT | MN_UNSIGNED | MN_QWORD, uint64_t, <)

            OP_CMP(MN_EQ | MN_DWORD, int32_t, ==)
            OP_CMP(MN_NE | MN_DWORD, int32_t, !=)
            OP_CMP(MN_GTE | MN_DWORD, int32_t, >=)
            OP_CMP(MN_GTE | MN_UNSIGNED | MN_DWORD, uint32_t, >=)
            OP_CMP(MN_LTE | MN_DWORD, int32_t, <=)
            OP_CMP(MN_LTE | MN_UNSIGNED | MN_DWORD, uint32_t, <=)
            OP_CMP(MN_GT | MN_DWORD, int32_t, >)
            OP_CMP(MN_GT | MN_UNSIGNED | MN_DWORD, uint32_t, >)
            OP_CMP(MN_LT | MN_DWORD, int32_t, <)
            OP_CMP(MN_LT | MN_UNSIGNED | MN_DWORD, uint32_t, <)

            OP_CMP(MN_EQ | MN_WORD, int16_t, ==)
            OP_CMP(MN_NE | MN_WORD, int16_t, !=)
            OP_CMP(MN_GTE | MN_WORD, int16_t, >=)
            OP_CMP(MN_GTE | MN_UNSIGNED | MN_WORD, uint16_t, >=)
            OP_CMP(MN_LTE | MN_WORD, int16_t, <=)
            OP_CMP(MN_LTE | MN_UNSIGNED | MN_WORD, uint16_t, <=)
            OP_CMP(MN_GT | MN_WORD, int16_t, >)
            OP_CMP(MN_GT | MN_UNSIGNED | MN_WORD, uint16_t, >)
            OP_CMP(MN_LT | MN_WORD, int16_t, <)
            OP_CMP(MN_LT | MN_UNSIGNED | MN_WORD, uint16_t, <)

            OP_CMP(MN_EQ | MN_BYTE, int8_t, ==)
            OP_CMP(MN_NE | MN_BYTE, int8_t, !=)
            OP_CMP(MN_GTE | MN_BYTE, int8_t, >=)
            OP_CMP(MN_GTE | MN_UNSIGNED | MN_BYTE, uint8_t, >=)
            OP_CMP(MN_LTE | MN_BYTE, int8_t, <=)
            OP_CMP(MN_LTE | MN_UNSIGNED | MN_BYTE, uint8_t, <=)
            OP_CMP(MN_GT | MN_BYTE, int8_t, >)
            OP_CMP(MN_GT | MN_UNSIGNED | MN_BYTE, uint8_t, >)
            OP_CMP(MN_LT | MN_BYTE, int8_t, <)
            OP_CMP(MN_LT | MN_UNSIGNED | MN_BYTE, uint8_t, <)

#define OP_CMPF(case_value, ctype, oper) \
case case_value: \
{ \
    r->stack_ptr[-2] = *(( ctype * ) &r->stack_ptr[-2] ) oper *(( ctype * ) &r->stack_ptr[-1] ); \
    --r->stack_ptr; \
    ++pc; \
    break; \
}

            /* Floating point comparisons (double) */

            OP_CMPF(MN_EQ | MN_DOUBLE, double, ==)
            OP_CMPF(MN_NE | MN_DOUBLE, double, !=)
            OP_CMPF(MN_GTE | MN_DOUBLE, double, >=)
            OP_CMPF(MN_LTE | MN_DOUBLE, double, <=)
            OP_CMPF(MN_GT | MN_DOUBLE, double, >)
            OP_CMPF(MN_LT | MN_DOUBLE, double, <)

            /* Floating point comparisons (float) */

            OP_CMPF(MN_EQ | MN_FLOAT, float, ==)
            OP_CMPF(MN_NE | MN_FLOAT, float, !=)
            OP_CMPF(MN_GTE | MN_FLOAT, float, >=)
            OP_CMPF(MN_LTE | MN_FLOAT, float, <=)
            OP_CMPF(MN_GT | MN_FLOAT, float, >)
            OP_CMPF(MN_LT | MN_FLOAT, float, <)

            /* String comparisons */

#define OP_CMPS(case_value, oper) \
case case_value: \
{ \
    uint64_t string_id1 = r->stack_ptr[-2], string_id2 = r->stack_ptr[-1]; \
    int64_t n = string_comp( string_id1, string_id2 ) oper 0; \
    string_discard( string_id1 ); \
    string_discard( string_id2 ); \
    r->stack_ptr[-2] = n; \
    --r->stack_ptr; \
    ++pc; \
    break; \
}

            OP_CMPS(MN_EQ | MN_STRING, ==)
            OP_CMPS(MN_NE | MN_STRING, !=)
            OP_CMPS(MN_GTE | MN_STRING, >=)
            OP_CMPS(MN_LTE | MN_STRING, <=)
            OP_CMPS(MN_GT | MN_STRING, >)
            OP_CMPS(MN_LT | MN_STRING, <)

            /* Convert */

#define OP_CVTF2I(case_value, ctype_from, ctype_to) \
case case_value: \
{ \
    r->stack_ptr[-pc[1] - 1] = ( ctype_to ) * ( ctype_from * ) &( r->stack_ptr[-pc[1] - 1] ); \
    pc += 2; \
    break; \
}

            OP_CVTF2I(MN_DOUBLE2INT | MN_QWORD, double, int64_t)
            OP_CVTF2I(MN_DOUBLE2INT | MN_UNSIGNED | MN_QWORD, double, uint64_t)
            OP_CVTF2I(MN_DOUBLE2INT | MN_DWORD, double, int32_t)
            OP_CVTF2I(MN_DOUBLE2INT | MN_UNSIGNED | MN_DWORD, double, uint32_t)
            OP_CVTF2I(MN_DOUBLE2INT | MN_WORD, double, int16_t)
            OP_CVTF2I(MN_DOUBLE2INT | MN_UNSIGNED | MN_WORD, double, uint16_t)
            OP_CVTF2I(MN_DOUBLE2INT | MN_BYTE, double, int8_t)
            OP_CVTF2I(MN_DOUBLE2INT | MN_UNSIGNED | MN_BYTE, double, uint8_t)

            OP_CVTF2I(MN_FLOAT2INT | MN_QWORD, float, int64_t)
            OP_CVTF2I(MN_FLOAT2INT | MN_UNSIGNED | MN_QWORD, float, uint64_t)
            OP_CVTF2I(MN_FLOAT2INT | MN_DWORD, float, int32_t)
            OP_CVTF2I(MN_FLOAT2INT | MN_UNSIGNED | MN_DWORD, float, uint32_t)
            OP_CVTF2I(MN_FLOAT2INT | MN_WORD, float, int16_t)
            OP_CVTF2I(MN_FLOAT2INT | MN_UNSIGNED | MN_WORD, float, uint16_t)
            OP_CVTF2I(MN_FLOAT2INT | MN_BYTE, float, int8_t)
            OP_CVTF2I(MN_FLOAT2INT | MN_UNSIGNED | MN_BYTE, float, uint8_t)

#define OP_CVTI2F(case_value, ctype_from, ctype_to) \
case case_value: \
{ \
    *( ctype_to* ) &(r->stack_ptr[-pc[1] - 1]) = ( ctype_to ) ( ctype_from ) r->stack_ptr[-pc[1] - 1]; \
    pc += 2; \
    break; \
}

            OP_CVTI2F(MN_INT2DOUBLE | MN_QWORD, int64_t, double)
            OP_CVTI2F(MN_INT2DOUBLE | MN_UNSIGNED | MN_QWORD, uint64_t, double)
            OP_CVTI2F(MN_INT2DOUBLE | MN_DWORD, int32_t, double)
            OP_CVTI2F(MN_INT2DOUBLE | MN_UNSIGNED | MN_DWORD, uint32_t, double)
            OP_CVTI2F(MN_INT2DOUBLE | MN_WORD, int16_t, double)
            OP_CVTI2F(MN_INT2DOUBLE | MN_UNSIGNED | MN_WORD, uint16_t, double)
            OP_CVTI2F(MN_INT2DOUBLE | MN_BYTE, int8_t, double)
            OP_CVTI2F(MN_INT2DOUBLE | MN_UNSIGNED | MN_BYTE, uint8_t, double)

            OP_CVTI2F(MN_INT2FLOAT | MN_QWORD, int64_t, float)
            OP_CVTI2F(MN_INT2FLOAT | MN_UNSIGNED | MN_QWORD, uint64_t, float)
            OP_CVTI2F(MN_INT2FLOAT | MN_DWORD, int32_t, float)
            OP_CVTI2F(MN_INT2FLOAT | MN_UNSIGNED | MN_DWORD, uint32_t, float)
            OP_CVTI2F(MN_INT2FLOAT | MN_WORD, int16_t, float)
            OP_CVTI2F(MN_INT2FLOAT | MN_UNSIGNED | MN_WORD, uint16_t, float)
            OP_CVTI2F(MN_INT2FLOAT | MN_BYTE, int8_t, float)
            OP_CVTI2F(MN_INT2FLOAT | MN_UNSIGNED | MN_BYTE, uint8_t, float)

#define OP_CVTI2I(case_value, ctype_from) \
case case_value: \
{ \
    r->stack_ptr[-pc[1] - 1] = ( ctype_from ) r->stack_ptr[-pc[1] - 1]; \
    pc += 2; \
    break; \
}

            OP_CVTI2I(MN_INT2DWORD, int32_t)
            OP_CVTI2I(MN_INT2DWORD | MN_UNSIGNED, uint32_t)
            OP_CVTI2I(MN_INT2WORD, int16_t)
            OP_CVTI2I(MN_INT2WORD | MN_UNSIGNED, uint16_t)
            OP_CVTI2I(MN_INT2BYTE, int8_t)
            OP_CVTI2I(MN_INT2BYTE | MN_UNSIGNED, uint8_t)

#define OP_CVTF2F(case_value, ctype_from, ctype_to) \
case case_value: \
{ \
    *( ctype_to * )&( r->stack_ptr[-pc[1] - 1] ) = ( ctype_to ) * ( ctype_from * ) &( r->stack_ptr[-pc[1] - 1] ); \
    pc += 2; \
    break; \
}

            OP_CVTF2F(MN_DOUBLE2FLOAT, double, float)
            OP_CVTF2F(MN_FLOAT2DOUBLE, float, double)

#define OP_INT2STR(case_value, ntos_func, ctype) \
case case_value: \
{ \
    uint64_t* string_id1_ptr = &r->stack_ptr[-pc[1] - 1]; \
    uint64_t value = ntos_func( ( ctype ) *string_id1_ptr ); \
    *string_id1_ptr = value; \
    string_use( value ); \
    pc += 2; \
    break; \
}

            OP_INT2STR(MN_INT2STR | MN_QWORD, string_itoa, int64_t)
            OP_INT2STR(MN_INT2STR | MN_UNSIGNED | MN_QWORD, string_uitoa, uint64_t)
            OP_INT2STR(MN_INT2STR | MN_DWORD, string_itoa, int32_t)
            OP_INT2STR(MN_INT2STR | MN_UNSIGNED | MN_DWORD, string_uitoa, uint32_t)
            OP_INT2STR(MN_INT2STR | MN_WORD, string_itoa, int16_t)
            OP_INT2STR(MN_INT2STR | MN_UNSIGNED | MN_WORD, string_uitoa, uint16_t)
            OP_INT2STR(MN_INT2STR | MN_BYTE, string_itoa, int8_t)
            OP_INT2STR(MN_INT2STR | MN_UNSIGNED | MN_BYTE, string_uitoa, uint8_t)

#define OP_CVTF2S(case_value, ctype_from) \
case case_value: \
{ \
    uint64_t* string_id1_ptr = &r->stack_ptr[-pc[1] - 1]; \
    uint64_t value = string_ftoa( *( ctype_from * ) string_id1_ptr ); \
    *string_id1_ptr = value; \
    string_use( value ); \
    pc += 2; \
    break; \
}

            OP_CVTF2S(MN_FLOAT2STR, float)
            OP_CVTF2S(MN_DOUBLE2STR, double)

            case MN_CHR2STR:
            {
                char buffer[2];
                buffer[0] = ( uint8_t )r->stack_ptr[-pc[1] - 1];
                buffer[1] = 0;
                r->stack_ptr[-pc[1] - 1] = string_new( buffer );
                string_use( r->stack_ptr[-pc[1] - 1] );
                pc += 2;
                break;
            }

            case MN_STRI2CHR:
            {
                uint64_t* string_id_ptr = &r->stack_ptr[-2];
                uint64_t string_old = *string_id_ptr;
                *string_id_ptr = string_char( string_old, r->stack_ptr[-1] );
                string_discard( string_old );
                --r->stack_ptr;
                ++pc;
                break;
            }

            case MN_STR2CHR:
            {
                int64_t string_id = r->stack_ptr[-pc[1] - 1];
                r->stack_ptr[-1] = *string_get( string_id );
                string_discard( string_id );
                pc += 2;
                break;
            }

            case MN_STR2POINTER:
            {
                uint64_t* string_id_ptr = &r->stack_ptr[-pc[1] - 1];
                int64_t string_id = *string_id_ptr;
                *string_id_ptr = string_atop( ( char * )string_get( string_id ) );
                string_discard( string_id );
                pc += 2;
                break;
            }

            case MN_POINTER2STR:
            {
                uint64_t* string_id_ptr = &r->stack_ptr[-pc[1] - 1];
                uint64_t string_id = string_ptoa( ( void * )( intptr_t )( *string_id_ptr ) );
                *string_id_ptr = string_id;
                string_use( string_id );
                pc += 2;
                break;
            }


#define OP_CVTS2F(case_value, ctype_to) \
case case_value: \
{ \
    uint64_t* string_id_ptr = &r->stack_ptr[-pc[1] - 1]; \
    uint64_t string_id = *string_id_ptr; \
    char* str = ( char * )string_get( string_id ); \
    *( ctype_to * ) string_id_ptr = str ? ( ctype_to )atof( str ) : 0.0f; \
    string_discard( string_id ); \
    pc += 2; \
    break; \
}

            OP_CVTS2F(MN_STR2DOUBLE, double)
            OP_CVTS2F(MN_STR2FLOAT, float)

            case MN_STR2INT:
            {
                uint64_t* string_id_ptr = &r->stack_ptr[-pc[1] - 1];
                uint64_t string_id = *string_id_ptr;
                char* str = ( char * )string_get( string_id );
                *string_id_ptr = str ? atoll( str ) : 0;
                string_discard( string_id );
                pc += 2;
                break;
            }

            case MN_A2STR:
            {
                uint64_t* param = &r->stack_ptr[-pc[1] - 1];
                int64_t string_id = string_new( *( char ** )param );
                string_use( string_id );
                *param = string_id;
                pc += 2;
                break;
            }

            case MN_STR2A:
            {
                uint64_t* param1 = &r->stack_ptr[-2];
                uint64_t string_id = r->stack_ptr[-1];
                uint64_t qty = pc[1];
                strncpy( *( char ** )param1, string_get( string_id ), qty );
                (( char * )( intptr_t )param1 )[ qty ] = 0;
                *param1 = string_id;
                --r->stack_ptr;
                pc += 2;
                break;
            }

            case MN_STR2CHARNUL:
            {
                uint64_t string_id = r->stack_ptr[-1];
                strcpy( *( char ** )( intptr_t ) r->stack_ptr[-2], string_get( string_id ) );
                string_discard( string_id );
                --r->stack_ptr;
                pc += 2;
                break;
            }

            /* Direct variables operations */

#define OP_LETNP(case_value, ctype, getvalue) \
case case_value: \
{ \
    ( *( ctype* )( intptr_t )( r->stack_ptr[-2] ) ) = getvalue r->stack_ptr[-1]; \
    r->stack_ptr -= 2; \
    ++pc; \
    break; \
}

            OP_LETNP(MN_LETNP | MN_QWORD, int64_t,)
            OP_LETNP(MN_LETNP | MN_UNSIGNED | MN_QWORD, uint64_t,)
            OP_LETNP(MN_LETNP | MN_DWORD, int32_t,)
            OP_LETNP(MN_LETNP | MN_UNSIGNED | MN_DWORD, uint32_t,)
            OP_LETNP(MN_LETNP | MN_WORD, int16_t,)
            OP_LETNP(MN_LETNP | MN_UNSIGNED | MN_WORD, uint16_t,)
            OP_LETNP(MN_LETNP | MN_BYTE, int8_t,)
            OP_LETNP(MN_LETNP | MN_UNSIGNED | MN_BYTE, uint8_t,)
            OP_LETNP(MN_LETNP | MN_DOUBLE, double, *( double * ) &)
            OP_LETNP(MN_LETNP | MN_FLOAT, float, *( float * ) &)

            case MN_LETNP | MN_STRING:
            {
                uint64_t* string_dest = ( int64_t * )( intptr_t )( r->stack_ptr[-2] );
                string_discard( *string_dest );
                *string_dest = r->stack_ptr[-1];
                r->stack_ptr -= 2;
                ++pc;
                break;
            }

#define OP_LET(case_value, ctype, getvalue) \
case case_value: \
{ \
    ( *( ctype* )( intptr_t )( r->stack_ptr[-2] ) ) = getvalue r->stack_ptr[-1]; \
    --r->stack_ptr; \
    ++pc; \
    break; \
}

            OP_LET(MN_LET | MN_QWORD, int64_t,)
            OP_LET(MN_LET | MN_UNSIGNED | MN_QWORD, uint64_t,)
            OP_LET(MN_LET | MN_DWORD, int32_t,)
            OP_LET(MN_LET | MN_UNSIGNED | MN_DWORD, uint32_t,)
            OP_LET(MN_LET | MN_WORD, int16_t,)
            OP_LET(MN_LET | MN_UNSIGNED | MN_WORD, uint16_t,)
            OP_LET(MN_LET | MN_BYTE, int8_t,)
            OP_LET(MN_LET | MN_UNSIGNED | MN_BYTE, uint8_t,)
            OP_LET(MN_LET | MN_DOUBLE, double, *( double * ) &)
            OP_LET(MN_LET | MN_FLOAT, float, *( float * ) &)

            case MN_LET | MN_STRING:
            {
                uint64_t* string_dest = ( int64_t * )( intptr_t )( r->stack_ptr[-2] );
                string_discard( *string_dest );
                *string_dest = r->stack_ptr[-1];
                --r->stack_ptr;
                ++pc;
                break;
            }


#define OP_VAR(case_value, ctype, oper, getvalue) \
case case_value: \
{ \
    *( ctype * )( intptr_t )( r->stack_ptr[-2] ) oper##= getvalue r->stack_ptr[-1]; \
    --r->stack_ptr; \
    ++pc; \
    break; \
}

            OP_VAR(MN_VARADD | MN_QWORD, int64_t, +,)
            OP_VAR(MN_VARADD | MN_UNSIGNED | MN_QWORD, uint64_t, +,)
            OP_VAR(MN_VARADD | MN_DWORD, int32_t, +,)
            OP_VAR(MN_VARADD | MN_UNSIGNED | MN_DWORD, uint32_t, +,)
            OP_VAR(MN_VARADD | MN_WORD, int16_t, +,)
            OP_VAR(MN_VARADD | MN_UNSIGNED | MN_WORD, uint16_t, +,)
            OP_VAR(MN_VARADD | MN_BYTE, int8_t, +,)
            OP_VAR(MN_VARADD | MN_UNSIGNED | MN_BYTE, uint8_t, +,)
            OP_VAR(MN_VARADD | MN_DOUBLE, double, +, *( double * ) &)
            OP_VAR(MN_VARADD | MN_FLOAT, float, +, *( float * ) &)

            case MN_VARADD | MN_STRING:
            {
                uint64_t* string_id1_ptr = ( int64_t * )( intptr_t )( r->stack_ptr[-2] );
                uint64_t string_id1 = *string_id1_ptr, string_id2 = r->stack_ptr[-1];
                *string_id1_ptr = string_add( string_id1, string_id2 );
                string_use( *string_id1_ptr );
                string_discard( string_id1 );
                string_discard( string_id2 );
                --r->stack_ptr;
                ++pc;
                break;
            }

            OP_VAR(MN_VARSUB | MN_QWORD, int64_t, -,)
            OP_VAR(MN_VARSUB | MN_UNSIGNED | MN_QWORD, uint64_t, -,)
            OP_VAR(MN_VARSUB | MN_DWORD, int32_t, -,)
            OP_VAR(MN_VARSUB | MN_UNSIGNED | MN_DWORD, uint32_t, -,)
            OP_VAR(MN_VARSUB | MN_WORD, int16_t, -,)
            OP_VAR(MN_VARSUB | MN_UNSIGNED | MN_WORD, uint16_t, -,)
            OP_VAR(MN_VARSUB | MN_BYTE, int8_t, -,)
            OP_VAR(MN_VARSUB | MN_UNSIGNED | MN_BYTE, uint8_t, -,)
            OP_VAR(MN_VARSUB | MN_DOUBLE, double, -, *( double * ) &)
            OP_VAR(MN_VARSUB | MN_FLOAT, float, -, *( float * ) &)

            OP_VAR(MN_VARMUL | MN_QWORD, int64_t, *,)
            OP_VAR(MN_VARMUL | MN_UNSIGNED | MN_QWORD, uint64_t, *,)
            OP_VAR(MN_VARMUL | MN_DWORD, int32_t, *,)
            OP_VAR(MN_VARMUL | MN_UNSIGNED | MN_DWORD, uint32_t, *,)
            OP_VAR(MN_VARMUL | MN_WORD, int16_t, *,)
            OP_VAR(MN_VARMUL | MN_UNSIGNED | MN_WORD, uint16_t, *,)
            OP_VAR(MN_VARMUL | MN_BYTE, int8_t, *,)
            OP_VAR(MN_VARMUL | MN_UNSIGNED | MN_BYTE, uint8_t, *,)
            OP_VAR(MN_VARMUL | MN_DOUBLE, double, *, *( double * ) &)
            OP_VAR(MN_VARMUL | MN_FLOAT, float, *, *( float * ) &)

            OP_VAR(MN_VAROR | MN_QWORD, int64_t, |,)
            OP_VAR(MN_VAROR | MN_UNSIGNED | MN_QWORD, uint64_t, |,)
            OP_VAR(MN_VAROR | MN_DWORD, int32_t, |,)
            OP_VAR(MN_VAROR | MN_UNSIGNED | MN_DWORD, uint32_t, |,)
            OP_VAR(MN_VAROR | MN_WORD, int16_t, |,)
            OP_VAR(MN_VAROR | MN_UNSIGNED | MN_WORD, uint16_t, |,)
            OP_VAR(MN_VAROR | MN_BYTE, int8_t, |,)
            OP_VAR(MN_VAROR | MN_UNSIGNED | MN_BYTE, uint8_t, |,)

            OP_VAR(MN_VARXOR | MN_QWORD, int64_t, ^,)
            OP_VAR(MN_VARXOR | MN_UNSIGNED | MN_QWORD, uint64_t, ^,)
            OP_VAR(MN_VARXOR | MN_DWORD, int32_t, ^,)
            OP_VAR(MN_VARXOR | MN_UNSIGNED | MN_DWORD, uint32_t, ^,)
            OP_VAR(MN_VARXOR | MN_WORD, int16_t, ^,)
            OP_VAR(MN_VARXOR | MN_UNSIGNED | MN_WORD, uint16_t, ^,)
            OP_VAR(MN_VARXOR | MN_BYTE, int8_t, ^,)
            OP_VAR(MN_VARXOR | MN_UNSIGNED | MN_BYTE, uint8_t, ^,)

            OP_VAR(MN_VARAND | MN_QWORD, int64_t, &,)
            OP_VAR(MN_VARAND | MN_UNSIGNED | MN_QWORD, uint64_t, &,)
            OP_VAR(MN_VARAND | MN_DWORD, int32_t, &,)
            OP_VAR(MN_VARAND | MN_UNSIGNED | MN_DWORD, uint32_t, &,)
            OP_VAR(MN_VARAND | MN_WORD, int16_t, &,)
            OP_VAR(MN_VARAND | MN_UNSIGNED | MN_WORD, uint16_t, &,)
            OP_VAR(MN_VARAND | MN_BYTE, int8_t, &,)
            OP_VAR(MN_VARAND | MN_UNSIGNED | MN_BYTE, uint8_t, &,)

            OP_VAR(MN_VARROR | MN_QWORD, int64_t, >>,)
            OP_VAR(MN_VARROR | MN_UNSIGNED | MN_QWORD, uint64_t, >>,)
            OP_VAR(MN_VARROR | MN_DWORD, int32_t, >>,)
            OP_VAR(MN_VARROR | MN_UNSIGNED | MN_DWORD, uint32_t, >>,)
            OP_VAR(MN_VARROR | MN_WORD, int16_t, >>,)
            OP_VAR(MN_VARROR | MN_UNSIGNED | MN_WORD, uint16_t, >>,)
            OP_VAR(MN_VARROR | MN_BYTE, int8_t, >>,)
            OP_VAR(MN_VARROR | MN_UNSIGNED | MN_BYTE, uint8_t, >>,)

            OP_VAR(MN_VARROL | MN_QWORD, int64_t, <<,)
            OP_VAR(MN_VARROL | MN_UNSIGNED | MN_QWORD, uint64_t, <<,)
            OP_VAR(MN_VARROL | MN_DWORD, int32_t, <<,)
            OP_VAR(MN_VARROL | MN_UNSIGNED | MN_DWORD, uint32_t, <<,)
            OP_VAR(MN_VARROL | MN_WORD, int16_t, <<,)
            OP_VAR(MN_VARROL | MN_UNSIGNED | MN_WORD, uint16_t, <<,)
            OP_VAR(MN_VARROL | MN_BYTE, int8_t, <<,)
            OP_VAR(MN_VARROL | MN_UNSIGNED | MN_BYTE, uint8_t, <<,)

            /* VAR DIV/MOD */

#define op_VARDIVMOD(case_value, ctype, oper) \
case case_value: \
{ \
    ctype divider = r->stack_ptr[-1]; \
    FATAL_ERROR_DIV_BY_ZERO_CHECK( !divider ) \
    *( ctype * )( intptr_t )( r->stack_ptr[-2] ) oper##= divider; \
    --r->stack_ptr; \
    ++pc; \
    break; \
}

#define op_VARDIV(case_value, ctype) op_VARDIVMOD(case_value, ctype, /)
#define op_VARMOD(case_value, ctype) op_VARDIVMOD(case_value, ctype, %)

#define op_VARDIVF(case_value, ctype) \
case case_value: \
{ \
    *( ctype * )( intptr_t )( r->stack_ptr[-2] ) /= *( ctype * ) &r->stack_ptr[-1]; \
    --r->stack_ptr; \
    ++pc; \
    break; \
}

#define op_VARMODF(case_value, ctype) \
case case_value: \
{ \
    *( ctype * )( intptr_t )( r->stack_ptr[-2] ) = fmod( *( ctype * ) r->stack_ptr[-2], *( ctype * ) &r->stack_ptr[-1] ); \
    --r->stack_ptr; \
    ++pc; \
    break; \
}

            /* VAR DIV */

            op_VARDIV(MN_VARDIV | MN_QWORD, int64_t)
            op_VARDIV(MN_VARDIV | MN_UNSIGNED | MN_QWORD, uint64_t)
            op_VARDIV(MN_VARDIV | MN_DWORD, int32_t)
            op_VARDIV(MN_VARDIV | MN_UNSIGNED | MN_DWORD, uint32_t)
            op_VARDIV(MN_VARDIV | MN_WORD, int16_t)
            op_VARDIV(MN_VARDIV | MN_UNSIGNED | MN_WORD, uint16_t)
            op_VARDIV(MN_VARDIV | MN_BYTE, int8_t)
            op_VARDIV(MN_VARDIV | MN_UNSIGNED | MN_BYTE, uint8_t)
            op_VARDIVF(MN_VARDIV | MN_DOUBLE, double)
            op_VARDIVF(MN_VARDIV | MN_FLOAT, float)

            /* VAR MOD */

            op_VARMOD(MN_VARMOD | MN_QWORD, int64_t)
            op_VARMOD(MN_VARMOD | MN_UNSIGNED | MN_QWORD, uint64_t)
            op_VARMOD(MN_VARMOD | MN_DWORD, int32_t)
            op_VARMOD(MN_VARMOD | MN_UNSIGNED | MN_DWORD, uint32_t)
            op_VARMOD(MN_VARMOD | MN_WORD, int16_t)
            op_VARMOD(MN_VARMOD | MN_UNSIGNED | MN_WORD, uint16_t)
            op_VARMOD(MN_VARMOD | MN_BYTE, int8_t)
            op_VARMOD(MN_VARMOD | MN_UNSIGNED | MN_BYTE, uint8_t)
            op_VARMODF(MN_VARMOD | MN_DOUBLE, double)
            op_VARMODF(MN_VARMOD | MN_FLOAT, float)

            /* String operations */

            case MN_STRACAT:
            {
                int64_t n = r->stack_ptr[-1];
                strncat( *( char ** )( &r->stack_ptr[-2] ), string_get( n ), (pc[1]-1) - strlen( *( char ** )( &r->stack_ptr[-2] ) ) );
                (( char * )( intptr_t )( r->stack_ptr[-2] ) )[pc[1]-1] = 0;
                r->stack_ptr[-2] = r->stack_ptr[-1];
                --r->stack_ptr;
                pc += 2;
                break;
            }

            /* Direct operations with variables QWORD type */

#define OP_DECINC(case_value, ctype, oper) \
case case_value: \
{ \
    ( *( ctype * )( intptr_t )( r->stack_ptr[-1] ) ) oper##= pc[1]; \
    pc += 2; \
    break; \
}

            OP_DECINC(MN_INC | MN_QWORD, int64_t, +)
            OP_DECINC(MN_INC | MN_UNSIGNED | MN_QWORD, uint64_t, +)
            OP_DECINC(MN_INC | MN_DWORD, int32_t, +)
            OP_DECINC(MN_INC | MN_UNSIGNED | MN_DWORD, uint32_t, +)
            OP_DECINC(MN_INC | MN_WORD, int16_t, +)
            OP_DECINC(MN_INC | MN_UNSIGNED | MN_WORD, uint16_t, +)
            OP_DECINC(MN_INC | MN_BYTE, int8_t, +)
            OP_DECINC(MN_INC | MN_UNSIGNED | MN_BYTE, uint8_t, +)
            OP_DECINC(MN_INC | MN_DOUBLE, double, +)
            OP_DECINC(MN_INC | MN_FLOAT, float, +)

            OP_DECINC(MN_DEC | MN_QWORD, int64_t, -)
            OP_DECINC(MN_DEC | MN_UNSIGNED | MN_QWORD, uint64_t, -)
            OP_DECINC(MN_DEC | MN_DWORD, int32_t, -)
            OP_DECINC(MN_DEC | MN_UNSIGNED | MN_DWORD, uint32_t, -)
            OP_DECINC(MN_DEC | MN_WORD, int16_t, -)
            OP_DECINC(MN_DEC | MN_UNSIGNED | MN_WORD, uint16_t, -)
            OP_DECINC(MN_DEC | MN_BYTE, int8_t, -)
            OP_DECINC(MN_DEC | MN_UNSIGNED | MN_BYTE, uint8_t, -)
            OP_DECINC(MN_DEC | MN_DOUBLE, double, -)
            OP_DECINC(MN_DEC | MN_FLOAT, float, -)

#define OP_POSTDECINC(case_value, ctype, oper, getvalue) \
case case_value: \
{ \
    ctype* var_ptr = ( ctype * )( intptr_t ) r->stack_ptr[-1]; \
    ctype current_value = *var_ptr; \
    *var_ptr = current_value oper pc[1]; \
    r->stack_ptr[-1] = getvalue current_value; \
    pc += 2; \
    break; \
}

            case MN_POSTDEC | MN_UNSIGNED | MN_QWORD:
            OP_POSTDECINC(MN_POSTDEC | MN_QWORD, int64_t, -,)
            OP_POSTDECINC(MN_POSTDEC | MN_DWORD, int32_t, -,)
            OP_POSTDECINC(MN_POSTDEC | MN_UNSIGNED | MN_DWORD, uint32_t, -,)
            OP_POSTDECINC(MN_POSTDEC | MN_WORD, int16_t, -,)
            OP_POSTDECINC(MN_POSTDEC | MN_UNSIGNED | MN_WORD, uint16_t, -,)
            OP_POSTDECINC(MN_POSTDEC | MN_BYTE, int8_t, -,)
            OP_POSTDECINC(MN_POSTDEC | MN_UNSIGNED | MN_BYTE, uint8_t, -,)
            OP_POSTDECINC(MN_POSTDEC | MN_DOUBLE, double, -, *( uint64_t * ) &)
            OP_POSTDECINC(MN_POSTDEC | MN_FLOAT, float, -, *( uint32_t * ) &)

            case MN_POSTINC | MN_UNSIGNED | MN_QWORD:
            OP_POSTDECINC(MN_POSTINC | MN_QWORD, int64_t, +,)
            OP_POSTDECINC(MN_POSTINC | MN_DWORD, int32_t, +,)
            OP_POSTDECINC(MN_POSTINC | MN_UNSIGNED | MN_DWORD, uint32_t, +,)
            OP_POSTDECINC(MN_POSTINC | MN_WORD, int16_t, +,)
            OP_POSTDECINC(MN_POSTINC | MN_UNSIGNED | MN_WORD, uint16_t, +,)
            OP_POSTDECINC(MN_POSTINC | MN_BYTE, int8_t, +,)
            OP_POSTDECINC(MN_POSTINC | MN_UNSIGNED | MN_BYTE, uint8_t, +,)
            OP_POSTDECINC(MN_POSTINC | MN_DOUBLE, double, +, *( uint64_t * ) &)
            OP_POSTDECINC(MN_POSTINC | MN_FLOAT, float, +, *( uint32_t * ) &)

            /* Jumps */

            case MN_JUMP:
                pc = r->code + pc[1];
                continue;

            case MN_JTRUE:
                --r->stack_ptr;
                if ( *r->stack_ptr ) {
                    pc = r->code + pc[1];
                    continue;
                }
                pc += 2;
                break;

            case MN_JFALSE:
                --r->stack_ptr;
                if ( !*r->stack_ptr ) {
                    pc = r->code + pc[1];
                    continue;
                }
                pc += 2;
                break;

            case MN_JTTRUE:
                if ( r->stack_ptr[-1] ) {
                    pc = r->code + pc[1];
                    continue;
                }
                pc += 2;
                break;

            case MN_JTFALSE:
                if ( !r->stack_ptr[-1] ) {
                    pc = r->code + pc[1];
                    continue;
                }
                pc += 2;
                break;

            case MN_NCALL:
                *r->stack_ptr++ = pc - r->code + 2 ; /* Push next address */
                pc = r->code + pc[1] ; /* Call function */
                ++r->call_level;
                break;

            /* Switch */

            case MN_SWITCH:
                r->switchval = *--r->stack_ptr;
                r->cased = 0;
                ++pc;
                break;

            case MN_SWITCH | MN_STRING:
                if ( r->switchval_string != 0 ) string_discard( r->switchval_string );
                r->switchval_string = *--r->stack_ptr;
                r->cased = 0;
                ++pc;
                break;

            case MN_CASE:
                if ( r->switchval == *--r->stack_ptr ) r->cased = 2;
                ++pc;
                break;

            case MN_CASE | MN_STRING:
            {
                --r->stack_ptr;
                uint64_t string_id = *r->stack_ptr;
                if ( string_comp( r->switchval_string, string_id ) == 0 ) r->cased = 2;
                string_discard( string_id );
                //string_discard( r->stack_ptr[-1] );
                ++pc;
                break;
            }

            case MN_CASE_R:
            {
                r->stack_ptr -= 2;
                if ( r->switchval >= r->stack_ptr[0] && r->switchval <= r->stack_ptr[1] ) r->cased = 1;
                ++pc;
                break;
            }

            case MN_CASE_R | MN_STRING:
            {
                r->stack_ptr -= 2;
                if ( string_comp( r->switchval_string, r->stack_ptr[0] ) >= 0 &&
                     string_comp( r->switchval_string, r->stack_ptr[1] ) <= 0 )
                    r->cased = 1;
                string_discard( r->stack_ptr[0] );
                string_discard( r->stack_ptr[1] );
                ++pc;
                break;
            }

            case MN_JNOCASE:
                if ( r->cased < 1 ) {
                    pc = r->code + pc[1];
                    continue;
                }
                pc += 2;
                break;

            /* Process control */

            case MN_TYPE:
            {
                PROCDEF * def = procdef_get( pc[1] );
                FATAL_ERROR_CHECK( !def, "ERROR: Runtime error in %s(%" PRId64 ") - Invalid type\n", r->proc->name, LOCQWORD( r, PROCESS_ID ) );
                *r->stack_ptr++ = def->type;
                pc += 2;
                break;
            }

            case MN_FRAME:
                LOCINT64( r, FRAME_PERCENT ) += r->stack_ptr[-1];
                --r->stack_ptr;
                r->codeptr = pc + 1;
                return_value = LOCQWORD( r, PROCESS_ID );

                if ( !( r->proc->flags & PROC_FUNCTION ) &&
                        r->called_by && instance_exists( r->called_by ) && ( LOCQWORD( r->called_by, STATUS ) & STATUS_WAITING_MASK ) )
                {
                    /* We're returning and the parent is waiting: wake it up */
                    if ( r->called_by->stack && ( r->called_by->stack[0] & STACK_RETURN_VALUE ) )
                        r->called_by->stack_ptr[-1] = return_value;

                    LOCQWORD( r->called_by, STATUS ) &= ~STATUS_WAITING_MASK;
                    r->called_by = NULL;
                }
                goto break_all;

            case MN_END:
                if ( r->call_level > 0 ) {
                    pc = r->code + *--r->stack_ptr;
                    r->call_level--;
                    continue;
                }

                if ( r->switchval_string != 0 ) string_discard( r->switchval_string );

                if ( *status_ptr != STATUS_DEAD ) *status_ptr = STATUS_KILLED;
                goto break_all;

            case MN_RETURN:
                if ( r->call_level > 0 ) {
                    pc = r->code + *--r->stack_ptr;
                    r->call_level--;
                    continue;
                }

                if ( r->switchval_string != 0 ) string_discard( r->switchval_string );

                if ( *status_ptr != STATUS_DEAD ) *status_ptr = STATUS_KILLED;
                --r->stack_ptr;
                return_value = *r->stack_ptr;
                goto break_all;

            /* Handlers */

            case MN_EXITHNDLR:
                r->exitcode = pc[1];
                pc += 2;
                break;

            case MN_ERRHNDLR:
                r->errorcode = pc[1];
                pc += 2;
                break;

            /* Others */

            case MN_DEBUG:
                if ( dcb.data.NSourceFiles ) {
                    if ( debug > 0 ) printf( "\n::: DEBUG from %s(%" PRId64 ")\n", r->proc->name, LOCQWORD( r, PROCESS_ID ) );
                    trace_sentence = -1;
                    debugger_show_console = 1;
                }
                ++pc;
                break;

            case MN_SENTENCE:
                trace_sentence     = pc[1];
                trace_instance     = r;
                pc += 2;
                if ( debugger_trace || debugger_step ) {
                    debugger_trace = 0;
                    debugger_step = 0;
                    debugger_show_console = 1;
                }
                break;


            case MN_COPY_ARRAY | MN_QWORD:
            case MN_COPY_ARRAY | MN_UNSIGNED | MN_QWORD:
            case MN_COPY_ARRAY | MN_DOUBLE:
                memmove( ( int8_t * )( ( intptr_t )r->stack_ptr[-3] ), ( int8_t * )( ( intptr_t )r->stack_ptr[-2] ), r->stack_ptr[-1] * sizeof( int64_t ));
                r->stack_ptr -= 2;
                ++pc;
                break;

            case MN_COPY_ARRAY | MN_DWORD:
            case MN_COPY_ARRAY | MN_UNSIGNED | MN_DWORD:
            case MN_COPY_ARRAY | MN_FLOAT:
                memmove( ( int8_t * )( ( intptr_t )r->stack_ptr[-3] ), ( int8_t * )( ( intptr_t )r->stack_ptr[-2] ), r->stack_ptr[-1] * sizeof( int32_t ) );
                r->stack_ptr -= 2;
                ++pc;
                break;

            case MN_COPY_ARRAY | MN_WORD:
            case MN_COPY_ARRAY | MN_UNSIGNED | MN_WORD:
                memmove( ( int8_t * )( ( intptr_t )r->stack_ptr[-3] ), ( int8_t * )( ( intptr_t )r->stack_ptr[-2] ), r->stack_ptr[-1] * sizeof( int16_t ) );
                r->stack_ptr -= 2;
                ++pc;
                break;

            case MN_COPY_ARRAY | MN_BYTE:
            case MN_COPY_ARRAY | MN_UNSIGNED | MN_BYTE:
                memmove( ( int8_t * )( ( intptr_t )r->stack_ptr[-3] ), ( int8_t * )( ( intptr_t )r->stack_ptr[-2] ), r->stack_ptr[-1] );
                r->stack_ptr -= 2;
                ++pc;
                break;

            case MN_COPY_ARRAY | MN_STRING:
            {
                // Size in elements
                int64_t * dst = ( int64_t * )( ( intptr_t )r->stack_ptr[-3] );
                int64_t * src = ( int64_t * )( ( intptr_t )r->stack_ptr[-2] );
                int64_t sz = r->stack_ptr[-1];
                while( sz-- ) {
                    string_discard( *dst );
                    *dst = *src;
                    string_use( *dst );
                    dst++; src++;
                }
                r->stack_ptr -= 2;
                ++pc;
                break;
            }

            case MN_COPY_ARRAY_REPEAT | MN_QWORD:
            case MN_COPY_ARRAY_REPEAT | MN_UNSIGNED | MN_QWORD:
            case MN_COPY_ARRAY_REPEAT | MN_DOUBLE:
            {
                int8_t * dst = ( int8_t * )( ( intptr_t )r->stack_ptr[-3] );
                int8_t * src = ( int8_t * )( ( intptr_t )r->stack_ptr[-2] );
                int64_t sz = r->stack_ptr[-1] * sizeof( int64_t );
                int64_t n = pc[1];
                while( n-- ) {
                    memmove( dst, src, sz );
                    dst += sz;
                }
                r->stack_ptr -= 2;
                pc += 2;
                break;
            }

            case MN_COPY_ARRAY_REPEAT | MN_DWORD:
            case MN_COPY_ARRAY_REPEAT | MN_UNSIGNED | MN_DWORD:
            case MN_COPY_ARRAY_REPEAT | MN_FLOAT:
            {
                int8_t * dst = ( int8_t * )( ( intptr_t )r->stack_ptr[-3] );
                int8_t * src = ( int8_t * )( ( intptr_t )r->stack_ptr[-2] );
                int64_t sz = r->stack_ptr[-1] * sizeof( int32_t );
                int64_t n = pc[1];
                while( n-- ) {
                    memmove( dst, src, sz );
                    dst += sz;
                }
                r->stack_ptr -= 2;
                pc += 2;
                break;
            }

            case MN_COPY_ARRAY_REPEAT | MN_WORD:
            case MN_COPY_ARRAY_REPEAT | MN_UNSIGNED | MN_WORD:
            {
                int8_t * dst = ( int8_t * )( ( intptr_t )r->stack_ptr[-3] );
                int8_t * src = ( int8_t * )( ( intptr_t )r->stack_ptr[-2] );
                int64_t sz = r->stack_ptr[-1] * sizeof( int16_t );
                int64_t n = pc[1];
                while( n-- ) {
                    memmove( dst, src, sz );
                    dst += sz;
                }
                r->stack_ptr -= 2;
                pc += 2;
                break;
            }

            case MN_COPY_ARRAY_REPEAT | MN_BYTE:
            case MN_COPY_ARRAY_REPEAT | MN_UNSIGNED | MN_BYTE:
            {
                int8_t * dst = ( int8_t * )( ( intptr_t )r->stack_ptr[-3] );
                int8_t * src = ( int8_t * )( ( intptr_t )r->stack_ptr[-2] );
                int64_t sz = r->stack_ptr[-1];
                int64_t n = pc[1];
                while( n-- ) {
                    memmove( dst, src, sz );
                    dst += sz;
                }
                r->stack_ptr -= 2;
                pc += 2;
                break;
            }

            case MN_COPY_ARRAY_REPEAT | MN_STRING:
            {
                // Size in elements
                int64_t * dst = ( int64_t * )( ( intptr_t )r->stack_ptr[-3] );
                int64_t * src = ( int64_t * )( ( intptr_t )r->stack_ptr[-2] );
                int64_t n = pc[1];
                while( n-- ) {
                    int64_t sz = r->stack_ptr[-1];
                    while( sz-- ) {
                        string_discard( *dst );
                        *dst = *src;
                        string_use( *dst );
                        dst++; src++;
                    }
                }
                r->stack_ptr -= 2;
                pc += 2;
                break;
            }

            case MN_COPY_STRUCT:
                copytypes(( void * )( intptr_t )r->stack_ptr[-5], ( void * )( intptr_t )r->stack_ptr[-4], ( DCB_TYPEDEF * )( intptr_t )r->stack_ptr[-3], r->stack_ptr[-2], r->stack_ptr[-1] );
                r->stack_ptr -= 4;
                ++pc;
                break;

            default:
                FATAL_ERROR_CHECK( 1, "ERROR: Runtime error in %s(%" PRId64 ") - Mnemonic 0x%02"PRIX64" not implemented\n", r->proc->name, LOCQWORD( r, PROCESS_ID ), *pc )
                break;
        }

        FATAL_ERROR_CHECK(r->stack_ptr < r->stack, "ERROR: Runtime error in %s(%" PRId64 ") - Critical Stack Problem StackBase=%p StackPTR=%p\n", r->proc->name, LOCQWORD( r, PROCESS_ID ), (void *)r->stack, (void *)r->stack_ptr );

#ifdef EXIT_ON_EMPTY_STACK
        if ( r->stack_ptr == r->stack ) {
            r->codeptr = pc;
            if ( *status_ptr != STATUS_RUNNING && *status_ptr != STATUS_DEAD ) break;
        }
#endif

    }

    /* *** GENERAL EXIT *** */
break_all:

    if ( !*pc || *pc == MN_RETURN || *pc == MN_END || *status_ptr == STATUS_KILLED ) {
        /* Check for waiting parent */
        if ( r->called_by && instance_exists( r->called_by ) && ( LOCQWORD( r->called_by, STATUS ) & STATUS_WAITING_MASK ) ) {
            /* We're returning and the parent is waiting: wake it up */
            if ( r->called_by->stack && ( r->called_by->stack[0] & STACK_RETURN_VALUE ) )
                r->called_by->stack_ptr[-1] = return_value;

            LOCQWORD( r->called_by, STATUS ) &= ~STATUS_WAITING_MASK;
        }
        r->called_by = NULL;

        /* The process should be destroyed immediately, it is a function-type one */
        /* Run ONEXIT */
        if ( ( ( *status_ptr & ~( STATUS_WAITING_MASK | STATUS_PAUSED_MASK ) ) != STATUS_DEAD ) && r->exitcode ) {
            *status_ptr = ( STATUS_DEAD | ( *status_ptr & STATUS_PAUSED_MASK ) );
            r->codeptr = r->code + r->exitcode;
            pc = r->codeptr;
            goto main_loop_instance_go;
        } else {
            instance_destroy( r );
            r = NULL;
        }
    }

    /* Hook */
    if ( r && instance_pos_execute_hook_count ) {
        for ( int n = 0; n < instance_pos_execute_hook_count; n++ )
            instance_pos_execute_hook_list[n]( r );
    }
    /* Hook */
    if ( r && *status_ptr != STATUS_KILLED && r->first_run ) r->first_run = 0;

    return return_value;
}

/* ---------------------------------------------------------------------- */
