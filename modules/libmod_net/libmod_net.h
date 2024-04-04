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

#ifndef __LIBMOD_NET_H
#define __LIBMOD_NET_H

/* --------------------------------------------------------------------------- */

#include <SDL.h>

#include "bgddl.h"

#include "net.h"

/* --------------------------------------------------------------------------- */

extern int64_t libmod_net_open( INSTANCE *my, int64_t *params );
extern int64_t libmod_net_wait( INSTANCE *my, int64_t *params );
extern int64_t libmod_net_accept( INSTANCE *my, int64_t *params );
extern int64_t libmod_net_send( INSTANCE *my, int64_t *params );
extern int64_t libmod_net_recv( INSTANCE *my, int64_t *params );
extern int64_t libmod_net_close( INSTANCE *my, int64_t *params );
extern int64_t libmod_net_getavailablebytes( INSTANCE *my, int64_t *params );
extern int64_t libmod_net_getReceiveBufferSize( INSTANCE *my, int64_t *params );
extern int64_t libmod_net_getremoteaddr( INSTANCE *my, int64_t *params );
extern int64_t libmod_net_is_new_connection( INSTANCE *my, int64_t *params );
extern int64_t libmod_net_is_message_incoming( INSTANCE *my, int64_t *params );

/* --------------------------------------------------------------------------- */

// extern void __bgdexport( libmod_net, process_exec_hook )( INSTANCE * r );
extern void __bgdexport( libmod_net, module_initialize )();
extern void __bgdexport( libmod_net, module_finalize )();

/* --------------------------------------------------------------------------- */
                                                             
#endif
