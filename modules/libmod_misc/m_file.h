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

#ifndef __M_FILE_H
#define __M_FILE_H

extern int64_t libmod_misc_file_save( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_file_load( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_file_fopen( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_file_fclose( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_file_fread( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_file_fwrite( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_file_freadC( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_file_fwriteC( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_file_fseek( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_file_frewind( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_file_ftell( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_file_fflush( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_file_filelength( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_file_fputs( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_file_fgets( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_file_file( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_file_feof( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_file_exists( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_file_remove( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_file_move( INSTANCE * my, int64_t * params );

#endif
