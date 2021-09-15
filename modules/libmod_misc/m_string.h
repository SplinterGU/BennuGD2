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

#ifndef __M_STRING_H
#define __M_STRING_H

extern int64_t libmod_misc_string_strlen( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_string_strupper( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_string_strlower( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_string_strcasecmp( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_string_substr( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_string_substr2( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_string_strfind( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_string_strfindSSI( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_string_lpad( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_string_rpad( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_string_itos( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_string_ftos( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_string_stoi( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_string_stof( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_string_asc( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_string_chr( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_string_trim( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_string_strrev( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_string_formatI( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_string_formatF( INSTANCE * my, int64_t * params );
extern int64_t libmod_misc_string_formatFI( INSTANCE * my, int64_t * params );

extern int64_t modstring_string_new_array( INSTANCE * my, int64_t * params );
extern int64_t modstring_string_resize_array( INSTANCE * my, int64_t * params );
extern int64_t modstring_string_delete_array( INSTANCE * my, int64_t * params );
extern int64_t modstring_string_dump( INSTANCE * my, int64_t * params );

extern int64_t modstring_string_isalpha( INSTANCE * my, int64_t * params );
extern int64_t modstring_string_isspace( INSTANCE * my, int64_t * params );
extern int64_t modstring_string_isnum( INSTANCE * my, int64_t * params );
extern int64_t modstring_string_isxnum( INSTANCE * my, int64_t * params );
extern int64_t modstring_string_isalnum( INSTANCE * my, int64_t * params );
extern int64_t modstring_string_iswordchar( INSTANCE * my, int64_t * params );
extern int64_t modstring_string_iswordfirst( INSTANCE * my, int64_t * params );
extern int64_t modstring_string_toupper( INSTANCE * my, int64_t * params );
extern int64_t modstring_string_tolower( INSTANCE * my, int64_t * params );

#endif
