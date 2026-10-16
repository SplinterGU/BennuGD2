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
    #include <string.h>

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

        static __inline__ void DO_Swap16(void *data) {
            uint16_t value;

            memcpy(&value, data, sizeof(value));

            value = (uint16_t)(
                (value << 8) |
                (value >> 8)
            );

            memcpy(data, &value, sizeof(value));
        }

        static __inline__ void DO_Swap32(void *data) {
            uint32_t value;

            memcpy(&value, data, sizeof(value));

            value =
                (value << 24) |
                ((value << 8) & UINT32_C(0x00FF0000)) |
                ((value >> 8) & UINT32_C(0x0000FF00)) |
                (value >> 24);

            memcpy(data, &value, sizeof(value));
        }

        static __inline__ void DO_Swap64(void *data) {
            uint64_t value;

            memcpy(&value, data, sizeof(value));

            value =
                (value << 56) |
                ((value << 40) & UINT64_C(0x00FF000000000000)) |
                ((value << 24) & UINT64_C(0x0000FF0000000000)) |
                ((value << 8)  & UINT64_C(0x000000FF00000000)) |
                ((value >> 8)  & UINT64_C(0x00000000FF000000)) |
                ((value >> 24) & UINT64_C(0x0000000000FF0000)) |
                ((value >> 40) & UINT64_C(0x000000000000FF00)) |
                (value >> 56);

            memcpy(data, &value, sizeof(value));
        }

        #define ARRANGE_QWORD(x) DO_Swap64((void *)(x))
        #define ARRANGE_DWORD(x) DO_Swap32((void *)(x))
        #define ARRANGE_WORD(x)  DO_Swap16((void *)(x))

        #define ARRANGE_QWORDS(x, c)                                      \
            do {                                                         \
                unsigned char *arrange_ptr_ = (unsigned char *)(void *)(x);\
                int arrange_count_ = (int)(c);                            \
                for (int arrange_i_ = 0;                                  \
                     arrange_i_ < arrange_count_;                         \
                     ++arrange_i_, arrange_ptr_ += sizeof(uint64_t)) {     \
                    DO_Swap64(arrange_ptr_);                              \
                }                                                        \
            } while (0)

        #define ARRANGE_DWORDS(x, c)                                      \
            do {                                                         \
                unsigned char *arrange_ptr_ = (unsigned char *)(void *)(x);\
                int arrange_count_ = (int)(c);                            \
                for (int arrange_i_ = 0;                                  \
                     arrange_i_ < arrange_count_;                         \
                     ++arrange_i_, arrange_ptr_ += sizeof(uint32_t)) {     \
                    DO_Swap32(arrange_ptr_);                              \
                }                                                        \
            } while (0)

        #define ARRANGE_WORDS(x, c)                                       \
            do {                                                         \
                unsigned char *arrange_ptr_ = (unsigned char *)(void *)(x);\
                int arrange_count_ = (int)(c);                            \
                for (int arrange_i_ = 0;                                  \
                     arrange_i_ < arrange_count_;                         \
                     ++arrange_i_, arrange_ptr_ += sizeof(uint16_t)) {     \
                    DO_Swap16(arrange_ptr_);                              \
                }                                                        \
            } while (0)

    #endif

#endif

/* Access a sub-64-bit typed value within an int64_t stack slot.
   On big-endian, adjusts the pointer to point to the LOW sizeof(ctype) bytes.
   On little-endian, this is a no-op. */
#if __BYTEORDER == __BIG_ENDIAN
    #define _STACK_FPTR(ctype, ptr) ((ctype *)((uint8_t *)(ptr) + (sizeof(int64_t) - sizeof(ctype))))
#else
    #define _STACK_FPTR(ctype, ptr) ((ctype *)(ptr))
#endif
