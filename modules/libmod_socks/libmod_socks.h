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

#ifndef __LIBMOD_SOCKS_H
#define __LIBMOD_SOCKS_H

/* --------------------------------------------------------------------------- */

#include <SDL.h>

#include "bgddl.h"

#include "tcp.h"

/* --------------------------------------------------------------------------- */

extern int64_t libmod_socks_tcp_listen( INSTANCE *my, int64_t *params );
extern int64_t libmod_socks_tcp_accept( INSTANCE *my, int64_t *params );
extern int64_t libmod_socks_tcp_connect( INSTANCE *my, int64_t *params );
extern int64_t libmod_socks_tcp_send( INSTANCE *my, int64_t *params );
extern int64_t libmod_socks_tcp_recv( INSTANCE *my, int64_t *params );
extern int64_t libmod_socks_tcp_wait( INSTANCE *my, int64_t *params );
extern int64_t libmod_socks_tcp_close( INSTANCE *my, int64_t *params );
extern int64_t libmod_socks_tcp_getavailablebytes( INSTANCE *my, int64_t *params );
extern int64_t libmod_socks_tcp_getremoteaddr( INSTANCE *my, int64_t *params );

/* --------------------------------------------------------------------------- */

// extern void __bgdexport( libmod_socks, process_exec_hook )( INSTANCE * r );
extern void __bgdexport( libmod_socks, module_initialize )();
extern void __bgdexport( libmod_socks, module_finalize )();

/* --------------------------------------------------------------------------- */
                                                             
#endif
