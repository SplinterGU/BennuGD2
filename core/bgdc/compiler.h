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

#ifndef __COMPILER_H
#define __COMPILER_H

#include <stdint.h>

#include "typedef.h"
#include "procdef.h"
#include "varspace.h"
#include "levelstack.h"

/* Compiler */

typedef struct _expresion_result {
    int asignation;
    int call;
    int lvalue;
    int constant;
    TYPEDEF type;

    int count; /* Used for generic arrays */

    int64_t value;
    double  fvalue;
} expresion_result;

extern void compile_error(const char * error, ...);
extern void compile_warning(int, const char * error, ...);

/* Compilation of upper sections */
extern void compile_init();
extern void compile_program();
extern void compile_sentence(PROCDEF * p);
extern void compile_block(PROCDEF *);

extern void import_mod( char * libname );

/* Compilation of special sections (data definition, etc) */
extern int  compile_array_data(VARSPACE * n, segment * data, int size, int subsize, BASETYPE *t, int is_inline);
extern int  compile_varspace(VARSPACE * n, segment * data, int additive, int copies, VARSPACE ** c, int alignment, int duplicateignore, int block_without_begin, int level, int is_inline );
extern void compile_constants();

#define DEFAULT_ALIGNMENT 8

/* Expression compiler */
extern expresion_result compile_value();
extern expresion_result compile_factor();
extern expresion_result compile_operand();
extern expresion_result compile_operation();
extern expresion_result compile_rotation();
extern expresion_result compile_clausule();
extern expresion_result compile_comparison();
extern expresion_result compile_subexpresion();
extern expresion_result compile_expresion(int need_constant, int need_lvalue, int discart_code, BASETYPE t);
extern expresion_result convert_result_type(expresion_result res, BASETYPE t);

extern CODEBLOCK * code;
extern int64_t mntype( TYPEDEF type, int accept_structs );

extern void compile_process();

/* Codes for identifiers and reserved words */
extern int64_t
    identifier_program,     identifier_debug,       identifier_const,
    identifier_begin,       identifier_end,         identifier_struct,
    identifier_global,      identifier_local,       identifier_public,      identifier_private,
    identifier_const,       identifier_dup,         identifier_while,
    identifier_repeat,      identifier_until,       identifier_switch,
    identifier_case,        identifier_default,     identifier_loop,
    identifier_for,         identifier_from,        identifier_step,
    identifier_to,          identifier_if,          identifier_else,
    identifier_break,       identifier_continue,    identifier_return,
    identifier_frame,       identifier_clone,       identifier_yield,
    identifier_onexit,      identifier_onerror,
    identifier_jmp,         identifier_call,
    identifier_on,          identifier_exit,        identifier_error,       identifier_resume,      identifier_stm_next,
    identifier_sizeof,      identifier_bandoffset,
    identifier_offset,      identifier_pointer,     identifier_type,
    identifier_and,         identifier_or ,         identifier_xor,         identifier_not,
    identifier_band,        identifier_bor,         identifier_bxor,        identifier_bnot,
    identifier_plus,        identifier_minus,       identifier_plusplus,    identifier_minusminus,
    identifier_equal,       identifier_multiply,    identifier_mod,
    identifier_divide,      identifier_semicolon,   identifier_colon,
    identifier_comma,       identifier_ror,         identifier_rol,
    identifier_rightp,      identifier_leftp,       identifier_rightb,      identifier_leftb,       identifier_rightcb,     identifier_leftcb,
    identifier_point,       identifier_twopoints,
    identifier_eq,          identifier_ne,          identifier_gte,
    identifier_lte,         identifier_lt,          identifier_gt,
    identifier_plusequal,   identifier_minusequal,  identifier_multequal,
    identifier_divequal,    identifier_modequal,    identifier_orequal,
    identifier_xorequal,    identifier_andequal,    identifier_rorequal,
    identifier_rolequal,    identifier_mouse,       identifier_uint32,
    identifier_uint16,      identifier_uint8,       identifier_string,
    identifier_float,       identifier_include,     identifier_embed_file, 	identifier_type,
    identifier_import,      identifier_elseif,      identifier_question,
    identifier_function,    identifier_int32,       identifier_int16,       identifier_int8,
    identifier_char,        identifier_dollar,
    identifier_unsigned,    identifier_signed,
    identifier_int64,       identifier_uint64,      identifier_double
    ;

extern int64_t reserved_words ;  /* Number of reserved words */

extern int identifier_is_type(int64_t id);
extern int identifier_is_basic_type(int64_t id);
extern VARIABLE * varspace_search_in_all_scopes( VARSPACE *n, int64_t code );

extern LevelStack lvlstk;

#endif
