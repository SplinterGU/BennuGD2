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

/* ------------------------------------------ */
/* Offsets de los datos globales predefinidos */
/* ------------------------------------------ */

#define ARGC                    0
#define ARGV_TABLE              ARGC                    + 8     /* 32+1 cadenas */
#define OS_ID                   ARGV_TABLE              + 33*8

/* ----------------------------------------- */
/* Offsets de los datos locales predefinidos */
/* ----------------------------------------- */

#define PROCESS_ID              0
#define PROCESS_TYPE            PROCESS_ID          + 8
#define FRAME_PERCENT           PROCESS_TYPE        + 8
#define STATUS                  FRAME_PERCENT       + 8
#define SAVED_STATUS            STATUS              + 8
#define SAVED_PRIORITY          SAVED_STATUS        + 8
#define FATHER                  SAVED_PRIORITY      + 8
#define SON                     FATHER              + 8
#define SMALLBRO                SON                 + 8
#define BIGBRO                  SMALLBRO            + 8
#define PRIORITY                BIGBRO              + 8
