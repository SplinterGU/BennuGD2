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

#ifndef __ARRANGE_ST_H
    #define __ARRANGE_ST_H

    #include <stdint.h>

    #define __LIL_ENDIAN 1234
    #define __BIG_ENDIAN 4321

    #if defined(__hppa__) || \
        defined(__m68k__) || \
        defined(mc68000) || \
        defined(_M_M68K) || \
        (defined(__MIPS__) && defined(__MISPEB__)) || \
        defined(__ppc__) || \
        defined(__POWERPC__) || \
        defined(_M_PPC) || \
        defined(__sparc__) || \
        defined(PS3_PPU)
        #define __BYTEORDER  __BIG_ENDIAN
    #else
        #define __BYTEORDER  __LIL_ENDIAN
    #endif

    /* ---------------------------------------------------------------------- */
    /* Portability tricks                                                     */
    /* ---------------------------------------------------------------------- */

    #if __BYTEORDER == __LIL_ENDIAN
        #define ARRANGE_QWORD(x)
        #define ARRANGE_DWORD(x)
        #define ARRANGE_WORD(x)

        #define ARRANGE_QWORDS(x,c)
        #define ARRANGE_DWORDS(x,c)
        #define ARRANGE_WORDS(x,c)
    #else
        static __inline__ void DO_Swap16(uint16_t * D) {
            *D = (*D<<8) | (*D>>8);
        }

        static __inline__ void DO_Swap32(uint32_t * D) {
            *D = (*D<<24) | ((*D<<8)&0x00FF0000LL) | ((*D>>8)&0x0000FF00LL) | (*D>>24);
        }

        static __inline__ void DO_Swap64(uint64_t * D) {
            uint32_t hi, lo;
            uint64_t x = *D;
            lo = (x & 0xFFFFFFFFLL);
            x >>= 32;
            hi = (x & 0xFFFFFFFFLL);
            DO_Swap32(&lo);
            DO_Swap32(&hi);
            *D = ( ( ( uint64_t ) lo ) << 32 ) | hi;
        }

        #define ARRANGE_QWORD(x)    DO_Swap64((uint64_t *)(x))
        #define ARRANGE_DWORD(x)    DO_Swap32((uint32_t *)(x))
        #define ARRANGE_WORD(x)     DO_Swap16((uint16_t *)(x))

        #define ARRANGE_QWORDS(x,c) {               \
            int __n;                                \
            uint64_t * __p = (uint64_t *)(x);       \
            for (__n = 0; __n < (int)(c); __n++)    \
                ARRANGE_QWORD(&__p[__n]);           \
            }
        #define ARRANGE_DWORDS(x,c) {               \
            int __n;                                \
            uint32_t * __p = (uint32_t *)(x);       \
            for (__n = 0; __n < (int)(c); __n++)    \
                ARRANGE_DWORD(&__p[__n]);           \
            }
        #define ARRANGE_WORDS(x,c) {                \
            int __n;                                \
            uint16_t * __p = (uint16_t *)(x);       \
            for (__n = 0; __n < (int)(c); __n++)    \
                ARRANGE_WORD(&__p[__n]);            \
            }
    #endif
#endif
