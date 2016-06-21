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

#include <stdlib.h>
#include <stdio.h>

#include "bgdrtm.h"

#include "bgddl.h"
#include "instance.h"

#include "xstrings.h"

#include "libmod_misc.h"

#include "dlvaracc.h"

/* ----------------------------------------------------------------- */
/* Interaccion entre procesos */

/* --------------------------------------------------------------------------- */
/** EXIT ()
 *  Leaves the program at next frame
 */

/* ----------------------------------------------------------------- */

int64_t libmod_misc_proc_exit_0( INSTANCE * my, int64_t * params ) {
    exit_value = 0;
    must_exit = 1;

    return 1;
}

/* ----------------------------------------------------------------- */

int64_t libmod_misc_proc_exit_1( INSTANCE * my, int64_t * params ) {
    printf( "%s", string_get( params[0] ) );
    printf( "\n" );
    fflush( stdout );
    string_discard( params[0] );

    exit_value = 0;
    must_exit = 1;

    return 1;
}

/* --------------------------------------------------------------------------- */

int64_t libmod_misc_proc_exit( INSTANCE * my, int64_t * params ) {
    INSTANCE * i = first_instance;
    while ( i ) {
        LOCQWORD( libmod_misc, i, STATUS ) = STATUS_KILLED;
        i = i->next;
    }

    printf( "%s\n", string_get( params[0] ) );
    fflush( stdout );
    string_discard( params[0] );

    exit_value = params[1];
    must_exit = 1;

    return 1;
}

/* ----------------------------------------------------------------- */

int64_t libmod_misc_proc_running( INSTANCE * my, int64_t * params ) {
    INSTANCE * i, * ctx;

    if ( params[0] == 0 ) return 0;

    if ( params[0] >= FIRST_INSTANCE_ID ) {
        i = instance_get( params[0] );
        if ( i && ( LOCQWORD( libmod_misc, i, STATUS ) & ~STATUS_WAITING_MASK ) >= STATUS_RUNNING ) return 1;
        return 0;
    }

    ctx = NULL;
    while ( ( i = instance_get_by_type( params[0], &ctx ) ) ) {
        if ( ( LOCQWORD( libmod_misc, i, STATUS ) & ~STATUS_WAITING_MASK ) >= STATUS_RUNNING ) return 1;
    }

    return 0;
}

/* ----------------------------------------------------------------- */

int64_t libmod_misc_proc_signal( INSTANCE * my, int64_t * params ) {
    INSTANCE * i, * ctx;
    int64_t fake_params[2];

    if ( params[0] == ALL_PROCESS ) {
        /* Signal all process but my */
        int64_t myid = LOCQWORD( libmod_misc, my, PROCESS_ID );
        fake_params[1] = ( params[1] >= S_TREE ) ? params[1] - S_TREE : params[1];
        i = first_instance;
        while ( i ) {
            if ( LOCQWORD( libmod_misc, i, PROCESS_ID ) != myid && ( LOCQWORD( libmod_misc, i, STATUS ) & ~STATUS_WAITING_MASK ) > STATUS_KILLED ) {
                fake_params[0] = LOCQWORD( libmod_misc, i, PROCESS_ID );
                libmod_misc_proc_signal( my, fake_params );
            }
            i = i->next;
        }
        return 0;
    }
    else if ( params[0] < FIRST_INSTANCE_ID ) {
        /* Signal by type */
        fake_params[1] = params[1];

        ctx = NULL;
        while ( ( i = instance_get_by_type( params[0], &ctx ) ) ) {
            fake_params[0] = LOCQWORD( libmod_misc, i, PROCESS_ID );
            libmod_misc_proc_signal( my, fake_params );
        }
        return 0;
    }

    i = instance_get( params[0] );
    if ( i ) {
        if (( LOCQWORD( libmod_misc, i, STATUS ) & ~STATUS_WAITING_MASK ) > STATUS_KILLED ) {
            switch ( params[1] ) {
                case S_KILL:
                case S_KILL_FORCE:
                    if ( params[1] == S_KILL_FORCE || !( LOCQWORD( libmod_misc, i, SIGNAL_ACTION ) & SMASK_KILL ) )
                        LOCQWORD( libmod_misc, i, STATUS ) = STATUS_KILLED;
                    break;

                case S_WAKEUP:
                case S_WAKEUP_FORCE:
                    if ( params[1] == S_WAKEUP_FORCE || !( LOCQWORD( libmod_misc, i, SIGNAL_ACTION ) & SMASK_WAKEUP ) )
                        LOCQWORD( libmod_misc, i, STATUS ) = ( LOCQWORD( libmod_misc, i, STATUS ) & STATUS_WAITING_MASK ) | STATUS_RUNNING;
                    break;

                case S_SLEEP:
                case S_SLEEP_FORCE:
                    if ( params[1] == S_SLEEP_FORCE || !( LOCQWORD( libmod_misc, i, SIGNAL_ACTION ) & SMASK_SLEEP ) )
                        LOCQWORD( libmod_misc, i, STATUS ) = ( LOCQWORD( libmod_misc, i, STATUS ) & STATUS_WAITING_MASK ) | STATUS_SLEEPING;
                    break;

                case S_FREEZE:
                case S_FREEZE_FORCE:
                    if ( params[1] == S_FREEZE_FORCE || !( LOCQWORD( libmod_misc, i, SIGNAL_ACTION ) & SMASK_FREEZE ) )
                        LOCQWORD( libmod_misc, i, STATUS ) = ( LOCQWORD( libmod_misc, i, STATUS ) & STATUS_WAITING_MASK ) | STATUS_FROZEN;
                    break;

                case S_KILL_TREE:
                case S_KILL_TREE_FORCE:
                    if ( params[1] == S_KILL_TREE_FORCE || !( LOCQWORD( libmod_misc, i, SIGNAL_ACTION ) & SMASK_KILL_TREE ) )
                        LOCQWORD( libmod_misc, i, STATUS ) = ( LOCQWORD( libmod_misc, i, STATUS ) & STATUS_WAITING_MASK ) | STATUS_KILLED;
                    break;

                case S_WAKEUP_TREE:
                case S_WAKEUP_TREE_FORCE:
                    if ( params[1] == S_WAKEUP_TREE_FORCE || !( LOCQWORD( libmod_misc, i, SIGNAL_ACTION ) & SMASK_WAKEUP_TREE ) )
                        LOCQWORD( libmod_misc, i, STATUS ) = ( LOCQWORD( libmod_misc, i, STATUS ) & STATUS_WAITING_MASK ) | STATUS_RUNNING;
                    break;

                case S_SLEEP_TREE:
                case S_SLEEP_TREE_FORCE:
                    if ( params[1] == S_SLEEP_TREE_FORCE || !( LOCQWORD( libmod_misc, i, SIGNAL_ACTION ) & SMASK_SLEEP_TREE ) )
                        LOCQWORD( libmod_misc, i, STATUS ) = ( LOCQWORD( libmod_misc, i, STATUS ) & STATUS_WAITING_MASK ) | STATUS_SLEEPING;
                    break;

                case S_FREEZE_TREE:
                case S_FREEZE_TREE_FORCE:
                    if ( params[1] == S_FREEZE_TREE_FORCE || !( LOCQWORD( libmod_misc, i, SIGNAL_ACTION ) & SMASK_FREEZE_TREE ) )
                        LOCQWORD( libmod_misc, i, STATUS ) = ( LOCQWORD( libmod_misc, i, STATUS ) & STATUS_WAITING_MASK ) | STATUS_FROZEN;
                    break;

                default:
                    return 1;
            }
        }

        if ( params[1] >= S_TREE ) {
            fake_params[1] = params[1];
            i = instance_getson( i );
            while ( i ) {
                fake_params[0] = LOCQWORD( libmod_misc, i, PROCESS_ID );
                libmod_misc_proc_signal( my, fake_params );
                i = instance_getbigbro( i ) ;
            }
        }
    }
    return 1;
}

/* ----------------------------------------------------------------- */

int64_t libmod_misc_proc_signal_action( INSTANCE * my, int64_t * params ) {
    int64_t action = params[1];

    if ( my ) {
        switch ( params[0] ) {
            case S_KILL:
                switch ( action ) {
                    case S_IGN:
                        LOCQWORD( libmod_misc, my, SIGNAL_ACTION ) |=  SMASK_KILL;
                        break;

                    case S_DFL:
                        LOCQWORD( libmod_misc, my, SIGNAL_ACTION ) &= ~SMASK_KILL;
                        break;
                }
                break;

            case S_WAKEUP:
                switch ( action ) {
                    case S_IGN:
                        LOCQWORD( libmod_misc, my, SIGNAL_ACTION ) |=  SMASK_WAKEUP;
                        break;

                    case S_DFL:
                        LOCQWORD( libmod_misc, my, SIGNAL_ACTION ) &= ~SMASK_WAKEUP;
                        break;
                }
                break;

            case S_SLEEP:
                switch ( action ) {
                    case S_IGN:
                        LOCQWORD( libmod_misc, my, SIGNAL_ACTION ) |=  SMASK_SLEEP;
                        break;

                    case S_DFL:
                        LOCQWORD( libmod_misc, my, SIGNAL_ACTION ) &= ~SMASK_SLEEP;
                        break;
                }
                break;

            case S_FREEZE:
                switch ( action ) {
                    case S_IGN:
                        LOCQWORD( libmod_misc, my, SIGNAL_ACTION ) |=  SMASK_FREEZE;
                        break;

                    case S_DFL:
                        LOCQWORD( libmod_misc, my, SIGNAL_ACTION ) &= ~SMASK_FREEZE;
                        break;
                }
                break;

            case S_KILL_TREE:
                switch ( action ) {
                    case S_IGN:
                        LOCQWORD( libmod_misc, my, SIGNAL_ACTION ) |=  SMASK_KILL_TREE;
                        break;

                    case S_DFL:
                        LOCQWORD( libmod_misc, my, SIGNAL_ACTION ) &= ~SMASK_KILL_TREE;
                        break;
                }
                break;

            case S_WAKEUP_TREE:
                switch ( action ) {
                    case S_IGN:
                        LOCQWORD( libmod_misc, my, SIGNAL_ACTION ) |=  SMASK_WAKEUP_TREE;
                        break;

                    case S_DFL:
                        LOCQWORD( libmod_misc, my, SIGNAL_ACTION ) &= ~SMASK_WAKEUP_TREE;
                        break;
                }
                break;

            case S_SLEEP_TREE:
                switch ( action ) {
                    case S_IGN:
                        LOCQWORD( libmod_misc, my, SIGNAL_ACTION ) |=  SMASK_SLEEP_TREE;
                        break;

                    case S_DFL:
                        LOCQWORD( libmod_misc, my, SIGNAL_ACTION ) &= ~SMASK_SLEEP_TREE;
                        break;
                }
                break;

            case S_FREEZE_TREE:
                switch ( action ) {
                    case S_IGN:
                        LOCQWORD( libmod_misc, my, SIGNAL_ACTION ) |=  SMASK_FREEZE_TREE;
                        break;

                    case S_DFL:
                        LOCQWORD( libmod_misc, my, SIGNAL_ACTION ) &= ~SMASK_FREEZE_TREE;
                        break;
                }
                break;
        }
    }
    return 1;
}

/* ----------------------------------------------------------------- */

int64_t libmod_misc_proc_signal_action3( INSTANCE * my, int64_t * params ) {
    INSTANCE * i, * ctx;

    if ( params[0] == ALL_PROCESS ) {
        i = first_instance;
        while ( i ) {
            libmod_misc_proc_signal_action( i, &params[1] );
            i = i->next;
        }
        return 0;
    }
    else if ( params[0] < FIRST_INSTANCE_ID ) {
        ctx = NULL;
        while ( ( i = instance_get_by_type( params[0], &ctx ) ) ) {
            libmod_misc_proc_signal_action( i, &params[1] );
        }
        return 0;
    }

    i = instance_get( params[0] );
    if ( i ) libmod_misc_proc_signal_action( i, &params[1] );

    return 1;
}

/* ----------------------------------------------------------------- */

int64_t libmod_misc_proc_let_me_alone( INSTANCE * my, int64_t * params ) {
    INSTANCE * i = first_instance;

    while ( i ) {
        if ( i != my && ( LOCQWORD( libmod_misc, i, STATUS ) & ~STATUS_WAITING_MASK ) != STATUS_DEAD )
            LOCQWORD( libmod_misc, i, STATUS ) = ( LOCQWORD( libmod_misc, i, STATUS ) & STATUS_WAITING_MASK ) | STATUS_KILLED;
        i = i->next;
    }
    if ( LOCQWORD( libmod_misc, my, STATUS ) > STATUS_KILLED ) LOCQWORD( libmod_misc, my, STATUS ) = STATUS_RUNNING;
    return 1;
}

/* ----------------------------------------------------------------- */

int64_t libmod_misc_proc_get_id( INSTANCE * my, int64_t * params ) {
    INSTANCE * ptr = first_instance, ** ctx;

    if ( !params[0] ) {
        LOCQWORD( libmod_misc, my, TYPE_SCAN ) = 0;
        if ( LOCQWORD( libmod_misc, my, ID_SCAN ) ) {
            ptr = instance_get( LOCQWORD( libmod_misc, my, ID_SCAN ) );
            if ( ptr ) ptr = ptr->next;
        }

        while ( ptr ) {
            if (( LOCQWORD( libmod_misc, ptr, STATUS ) & ~STATUS_WAITING_MASK ) >= STATUS_RUNNING ) {
                LOCQWORD( libmod_misc, my, ID_SCAN ) = LOCQWORD( libmod_misc, ptr, PROCESS_ID );
                return LOCQWORD( libmod_misc, ptr, PROCESS_ID );
            }
            ptr = ptr->next;
        }
        return 0;
    }

    LOCQWORD( libmod_misc, my, ID_SCAN ) = 0;
    /* Check if already in scan by type and we reach limit */
    ctx = ( INSTANCE ** ) LOCADDR( libmod_misc, my, CONTEXT );
/*
    if ( !*ctx && LOCQWORD( libmod_misc, my, TYPE_SCAN ) ) {
        LOCQWORD( libmod_misc, my, TYPE_SCAN ) = 0;
        return 0;
    }
*/
    /* Check if type change from last call */
    if ( LOCQWORD( libmod_misc, my, TYPE_SCAN ) != params[0] ) {
        *ctx = NULL;
        LOCQWORD( libmod_misc, my, TYPE_SCAN ) = params[0];
    }

    while ( ( ptr = instance_get_by_type( params[0], ctx ) ) ) {
        if ( /*ptr != my &&*/ ( LOCQWORD( libmod_misc, ptr, STATUS ) & ~STATUS_WAITING_MASK ) >= STATUS_RUNNING ) {
            return LOCQWORD( libmod_misc, ptr, PROCESS_ID );
        }
    }

    return 0;
}

/* ----------------------------------------------------------------- */

int64_t libmod_misc_proc_get_status( INSTANCE * my, int64_t * params ) {
    INSTANCE * i;
    if ( !params[0] || !( i = instance_get( params[0] ) ) ) return 0;
    return LOCQWORD( libmod_misc, i, STATUS );
}

/* ----------------------------------------------------------------- */
