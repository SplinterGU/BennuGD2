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
#ifdef TARGET_BEOS
#include <posix/assert.h>
#else
#include <assert.h>
#endif
#include <string.h>
#include <stdlib.h>

#include "bgdc.h"
#include "xctype.h"
#include "levelstack.h"

/* ---------------------------------------------------------------------- */
/* This module contains the functions that compile data definitions:      */
/* variable declarations and their initialization.                        */
/* ---------------------------------------------------------------------- */

int compile_array_data( VARSPACE * n, segment * data, int size, int subsize, BASETYPE *t, int is_inline ) {
    int block, count = 0, remaining = size;
    int64_t base_offset, total_length, base;
    expresion_result res;

    for (;;) {
        if ( !remaining && size ) {
            token_back();
            break;
        }

        token_next();

        if ( token.type == IDENTIFIER && ( token.code == identifier_rightcb || token.code == identifier_semicolon ) ) {
            token_back();
            break;
        }
        else
        if ( token.type == STRING && *t == TYPE_CHAR ) {
            const char * str = string_get( token.code );
            int subcount = 0;

            if ( subsize == 0 ) subsize = strlen( str ) + 1;

            if (( int )strlen( str ) > subsize ) compile_error( MSG_TOO_MANY_INIT );
            while ( *str ) {
                if (is_inline) {
                    codeblock_add(code, MN_PRIVATE | MN_BYTE | MN_UNSIGNED, data->current);
                    codeblock_add(code, MN_PUSH, *str);
                    codeblock_add(code, MN_LETNP | MN_BYTE | MN_UNSIGNED, 0);
                }
                segment_add_as( data, *str++, TYPE_CHAR );
                subcount++;
            }
            while ( subcount++ < subsize ) {
                if (is_inline) {
                    codeblock_add(code, MN_PRIVATE | MN_BYTE | MN_UNSIGNED, data->current);
                    codeblock_add(code, MN_PUSH, 0);
                    codeblock_add(code, MN_LETNP | MN_BYTE | MN_UNSIGNED, 0);
                }
                segment_add_as( data, 0, TYPE_CHAR );
            }
            count += subsize;
            remaining -= subsize;
        }
        else
        if ( token.type == IDENTIFIER && token.code == identifier_leftcb ) {
            block = compile_array_data( n, data, remaining, remaining, t, is_inline );
            remaining -= block;
            count += block;
            token_next();
            if ( token.type != IDENTIFIER || token.code != identifier_rightcb ) compile_error( MSG_EXPECTED, "}" );
        }
        else
        {
            token_back();

            CODEBLOCK_POS pos1 = codeblock_pos(code), pos2;

            if ( is_inline ) {
                /* Reserve space to set 'MN_PRIVATE | mntype(*t, 0)' when the type of *t is resolved. */
                codeblock_add(code, MN_NOP, 0);
                codeblock_add(code, MN_NOP, 0);
                pos2 = codeblock_pos(code); /* Save this point for optimize constants */
            }

            res = compile_expresion( !is_inline, 0, !is_inline, TYPE_UNDEFINED );

            if ( *t == TYPE_UNDEFINED ) {
                *t = typedef_base( res.type );
                if ( *t == TYPE_UNDEFINED ) {
                    compile_error( MSG_INCOMP_TYPE );
                }
            }

            TYPEDEF type = typedef_new( *t );
            int64_t mn = mntype( type, 0 );

            token_next();
            /* DUP */
            if ( token.type == IDENTIFIER && token.code == identifier_dup ) {
                if ( !res.constant ) {
                    token_back();
                    compile_error( MSG_CONSTANT_EXP );
                }

                if ( typedef_is_string( res.type ) ) {
                    compile_error( MSG_NUMBER_REQUIRED );
                }

                base = res.value;
                if ( *t == TYPE_FLOAT ) {
                    float f = ( float ) res.fvalue;
                    base = *( int32_t * ) &f;
                }
                else
                if ( *t == TYPE_DOUBLE ) base = *( int64_t * ) &res.fvalue;

                /* Ignore code from "pos1" */
                codeblock_setpos(code, pos1);

                token_next();
                if ( token.type != IDENTIFIER || token.code != identifier_leftp ) compile_error( MSG_EXPECTED, "(" );
                base_offset = data->current;
                block = compile_array_data( n, data, remaining, remaining, t, is_inline );
                total_length = data->current - base_offset;
                if ( size && block * base > remaining ) {
                    break; /* MSG_TOO_MANY_INIT */
                }
                count += block * base;
                if ( size ) remaining -= block * base;

                if ( is_inline ) {
                    int64_t target_offset = data->current, tsize = typedef_size( type ), nitems = total_length / tsize;
                    codeblock_add( code, MN_PRIVATE, target_offset );
                    codeblock_add( code, MN_PRIVATE, base_offset );
                    codeblock_add( code, MN_PUSH, nitems );
                    codeblock_add( code, MN_COPY_ARRAY_REPEAT | mn, base - 1 );
                    if ( *t == TYPE_STRING ) {
                        for ( int64_t j = 0; j < nitems * ( base - 1 ); j++, target_offset += tsize ) varspace_varstring( n, target_offset );
                    }
                }

                while ( base-- > 1 ) {
                    segment_copy( data, base_offset, total_length );
                    base_offset += total_length;
                }
                token_next();
                if ( token.type != IDENTIFIER || token.code != identifier_rightp ) compile_error( MSG_EXPECTED, ")" );
            }
            /* Non DUP */
            else
            {
                if ( typedef_is_array( res.type ) ) {
                    if ( typedef_size( res.type ) != size * typedef_size( type ) ) compile_error( MSG_TYPES_NOT_THE_SAME );

                    int rescount = typedef_tcount( res.type );

                    CODEBLOCK_POS curr_pos = codeblock_pos(code);
                    codeblock_setpos(code, pos1);
                    codeblock_add(code, MN_PRIVATE | mn, data->current);
                    codeblock_setpos(code, curr_pos);

                    /* Optimized fast memcopy version */
                    codeblock_add( code, MN_PUSH, rescount );
                    codeblock_add( code, MN_COPY_ARRAY | mntype( res.type, 1 ), 0 );
                    codeblock_add( code, MN_POP, 0 );

                    token_back();

                    if ( *t == TYPE_STRING ) {
                        int64_t tsize = typedef_size( type );
                        for ( int64_t j = 0; j < rescount; j++, data->current += tsize ) {
                            varspace_varstring( n, data->current );
                            segment_add_as(data, 0, TYPE_STRING);
                        }
                    } else {
                        segment_fill(data, typedef_size( res.type ));
                    }

                    count+=rescount;
                    if ( size ) remaining-=rescount;
                } else {
                    if ( *t != typedef_base( res.type ) ) {
                        res = convert_result_type( res, *t );
                    }

                    base = res.value;
                    if ( *t == TYPE_FLOAT ) {
                        float f = ( float ) res.fvalue;
                        base = *( int32_t * ) &f;
                    }
                    else
                    if ( *t == TYPE_DOUBLE ) base = *( int64_t * ) &res.fvalue;

                    if ( is_inline ) {
                        if ( res.constant ) {
                            codeblock_setpos( code, pos2 );
                            codeblock_add( code, MN_PUSH | ( *t == TYPE_STRING ? MN_STRING : 0 ), base );
                        }

                        CODEBLOCK_POS curr_pos = codeblock_pos(code);
                        codeblock_setpos(code, pos1);
                        codeblock_add(code, MN_PRIVATE | mn, data->current);
                        codeblock_setpos(code, curr_pos);
                        codeblock_add(code, MN_LETNP | mn, 0);
                    }
                    token_back();
                    if ( *t == TYPE_STRING ) varspace_varstring( n, data->current );
                    segment_add_as( data, base, *t );
                    count++;
                    if ( size ) remaining--;
                }
            }
        }
        token_next();
        if ( token.type == IDENTIFIER && token.code == identifier_comma ) {
            if ( !size && *t == TYPE_CHAR ) compile_error( MSG_TOO_MANY_INIT );
            continue;
        }
        token_back();
        break;
    }

    return count;
}

/*
static BASETYPE get_basetype( VARSPACE * v ) {
    TYPEDEF t;
    BASETYPE type = TYPE_UNDEFINED, newtype;
    int n;

    for ( n = 0 ; n < v->count ; n++ ) {
        t = v->vars[n].type;
        while ( typedef_is_array( t ) ) {
            t = typedef_reduce( t );
        }

        if ( typedef_is_struct( t ) ) {
            newtype = get_basetype( typedef_members( t ) ) ;
        } else {
            newtype = typedef_base( t );
        }

        if ( type != TYPE_UNDEFINED && type != newtype ) {
            return TYPE_UNDEFINED;
        }

        type = newtype;
    }
    return type;
}
*/

/*
 *  FUNCTION : compile_struct_data
 *
 *  Compile the values of an struct initialization
 *  Does not suppor parenthized sections or the DUP operator
 *
 *  PARAMS:
 *      n       Current variable space
 *      data    Current data segment
 *      size    Size (total number of variables) of the array
 *              including all dimensions (1 for single struct)
 *      sub     1 if the struct we'are initializing is part of
 *              another one
 *             (the function will then ignore a trailing comma)
 *
 *  RETURN VALUE:
 *      Number of structs initialized
 */

int compile_struct_data( VARSPACE * n, segment * data, int size, int sub, int is_inline ) {
    int elements = 0, position = 0;
    expresion_result res;

    if ( !n ) return 0;

    for (;;) {
        token_next();

        /* Allow parenthized struct initialization */
        if ( token.type == IDENTIFIER && token.code == identifier_leftcb ) {

            if (( elements % n->count ) != 0 ) compile_error( MSG_NOT_ENOUGH_INIT );

            /* Note - don't ignore a trailing comma! */
            elements = compile_struct_data( n, data, size, 0, is_inline );

            if ( elements >= n->count ) size -= ( elements / n->count );

            token_next();

            if ( token.type != IDENTIFIER || token.code != identifier_rightcb ) compile_error( MSG_EXPECTED, "}" );

            token_next();
            if (( elements % n->count ) == 0 && size > 0 && token.type == IDENTIFIER && token.code == identifier_comma )
                continue;

            token_back();
            return elements;
        }

        /* Allow empty initialization */
        if ( token.type == IDENTIFIER && token.code == identifier_semicolon ) return 0;

        token_back();

        for (;;) {
            TYPEDEF next_type = n->vars[position].type;

            /* Next variable is a pointer */
            if ( typedef_is_pointer( next_type ) ) {

                int64_t mn = mntype( next_type, 0 );

                if ( is_inline ) codeblock_add(code, MN_PRIVATE | mn, data->current);

                CODEBLOCK_POS pos = codeblock_pos(code);

                res = compile_expresion( !is_inline, 0, !is_inline, TYPE_UNDEFINED );

                if ( !is_inline && !res.constant ) compile_error( MSG_INCORRECT_PTR_INIT );

                if ( TYPE_POINTER != typedef_base( res.type ) ) {
                    res = convert_result_type( res, TYPE_POINTER );
                }

                if ( !is_inline || res.constant ) codeblock_setpos(code, pos);

                if ( is_inline ) {
                    if ( res.constant ) codeblock_add( code, MN_PUSH, res.value );
                    codeblock_add( code, MN_LETNP | mn, 0);
                }

                segment_add_as( data, res.value, TYPE_POINTER );

            } else
            /* Next variable is an array */
            if ( typedef_is_array( next_type ) ) {
                int elements = typedef_tcount( next_type );
                BASETYPE base;

                /* Get the array base type */
                while ( typedef_is_array( next_type ) ) next_type = typedef_reduce( next_type );

                base = typedef_base( next_type );

                /* Special case: array of structs */
                if ( base == TYPE_STRUCT ) {
                    compile_struct_data( next_type.varspace, data, elements, 1, is_inline );
                } else {
                    token_next();

                    /* Special case: intializing char[] strings */
                    if ( token.type == STRING && next_type.chunk[1].type == TYPE_CHAR ) {
                        const char * str = string_get( token.code );
                        int subcount = 0;

                        if (( int )strlen( str ) > typedef_count( next_type ) - 1 ) compile_error( MSG_TOO_MANY_INIT );

                        while ( *str ) {
                            if (is_inline) {
                                codeblock_add(code, MN_PRIVATE | MN_BYTE | MN_UNSIGNED, data->current);
                                codeblock_add(code, MN_PUSH, *str);
                                codeblock_add(code, MN_LETNP | MN_BYTE | MN_UNSIGNED, 0);
                            }
                            segment_add_as( data, *str++, TYPE_CHAR );
                            subcount++;
                        }

                        while ( subcount++ < typedef_count( next_type ) ) {
                            if (is_inline) {
                                codeblock_add(code, MN_PRIVATE | MN_BYTE | MN_UNSIGNED, data->current);
                                codeblock_add(code, MN_PUSH, 0);
                                codeblock_add(code, MN_LETNP | MN_BYTE | MN_UNSIGNED, 0);
                            }
                            segment_add_as( data, 0, TYPE_CHAR );
                        }
                    }
                    else
                    /* Initializing normal arrays */
                    {
                        int has_curlybraces = 1;

                        if ( token.type != IDENTIFIER || token.code != identifier_leftcb ) {
                            has_curlybraces = 0;
                            token_back();
                        }

                        compile_array_data( n, data, elements, elements, &base, is_inline );

                        if ( has_curlybraces ) {
                            token_next();
                            if ( token.type != IDENTIFIER || token.code != identifier_rightcb ) compile_error( MSG_EXPECTED, "}" );
                        }
                    }
                }
            }
            else
            /* Next variable is another struct */
            if ( typedef_is_struct( next_type ) ) {
                compile_struct_data( next_type.varspace, data, 1, 1, is_inline );
            }
            else
            /* Next variable is a single type */
            {
                BASETYPE t = typedef_base( next_type );

                int64_t mn = mntype( next_type, 0 );

                CODEBLOCK_POS pos;
                if ( is_inline ) {
                    codeblock_add(code, MN_PRIVATE | mn, data->current);
                    pos = codeblock_pos(code);
                }

                res = compile_expresion( !is_inline, 0, 0, TYPE_UNDEFINED );

                if ( typedef_is_array( res.type ) ) {
                    if ( !typedef_is_equal( res.type, next_type ) || typedef_size( res.type ) != size * n->size ) compile_error( MSG_TYPES_NOT_THE_SAME );

                    int typesize = res.type.varspace->count * sizeof( DCB_TYPEDEF );
                    segment_ensure_capacity(globaldata, typesize);
                    codeblock_add( code, MN_GLOBAL, globaldata->current );
                    for ( int nvar = 0 ; nvar < res.type.varspace->count ; nvar++ ) {
                        DCB_TYPEDEF dcbtype;
                        dcb_settype( &dcbtype, &res.type.varspace->vars[nvar].type );
                        memcpy(( uint8_t* )globaldata->bytes + globaldata->current, &dcbtype, sizeof( DCB_TYPEDEF ) );
                        globaldata->current += sizeof( DCB_TYPEDEF );
                    }

                    /* Optimized fast memcopy version */
                    codeblock_add( code, MN_PUSH, n->count );
                    codeblock_add( code, MN_PUSH, size ? size : 1 );
                    codeblock_add( code, MN_COPY_STRUCT, 0 );
                    codeblock_add( code, MN_POP, 0 );

                    position+=n->size-1;
                    elements+=size-1;

                } else {
                    if ( t != typedef_base( res.type ) ) {
                        res = convert_result_type( res, t );
                    }

                    if ( is_inline ) {
                        if ( res.constant ) {
                            codeblock_setpos(code, pos);

                            int64_t base = res.value;
                            if ( t == TYPE_FLOAT ) {
                                float f = ( float ) res.fvalue;
                                base = *( int32_t * ) &f;
                            }
                            else
                            if ( t == TYPE_DOUBLE ) base = *( int64_t * ) &res.fvalue;

                            codeblock_add(code, MN_PUSH | ( t == TYPE_STRING ? MN_STRING : 0 ), base);
                        }
                        codeblock_add(code, MN_LETNP | mn, 0);
                    }
                    else
                    /* not inline */
                    if ( !res.constant ) compile_error( MSG_CONSTANT_EXP );

                    if ( t == TYPE_FLOAT ) {
                        float f = ( float ) res.fvalue;
                        segment_add_as( data, *( int32_t * ) &f, t );
                    }
                    else
                    if ( t == TYPE_DOUBLE ) {
                        segment_add_as( data, *( int64_t * ) &res.fvalue, t );
                    }
                    else
                    {
                        if ( t == TYPE_STRING ) varspace_varstring( n, data->current );
                        segment_add_as( data, res.value, t );
                    }
                }
            }

            position++;
            elements++;

            if ( position == n->count && size < 2 && sub ) break;

            /* A comma should be here */
            token_next();
            if ( token.type == IDENTIFIER && token.code == identifier_semicolon ) {
                token_back();
                break;
            }

            if ( token.type == IDENTIFIER && token.code == identifier_rightcb ) {
                token_back();
                break;
            }

            if ( token.type != IDENTIFIER || token.code != identifier_comma ) compile_error( MSG_EXPECTED, "," );

            /* Wrap around for the next struct */

            if ( position == n->count ) {
                if ( size == 1 && !sub ) compile_error( MSG_TOO_MANY_INIT );
                size--;
                position = 0;
            }
        }
        break;
    }

    return elements;
}

static void set_type( TYPEDEF * t, BASETYPE type ) {
/*
    int n;

    for ( n = 0 ; n < t->depth ; n++ )
        if ( t->chunk[n].type == TYPE_UNDEFINED )
        {
            t->chunk[n].type = type;
            break;
        }
    return;
*/
    t->chunk[t->depth-1].type = type;
}

/* Reverses the indices [10][5] -> [5][10] */
TYPEDEF reverse_indices(TYPEDEF type) {
    int i;

    for ( i = 0 ; i < type.depth ; i++ ) if ( type.chunk[i].type != TYPE_ARRAY ) break;
    i--;

    TYPECHUNK chunk;
    for ( int j = 0 ; j < ( i + 1 ) / 2 ; j++ ) {
        chunk               = type.chunk[ j ];
        type.chunk[ j ]     = type.chunk[ i - j ];
        type.chunk[ i - j ] = chunk;
    }

    return type;
}

/* --------------------------- */

static char strid[16384];

const char * identifier_string_for_limited_scope_variable( int64_t code, int l ) {
    if ( l >= 0 ) {
        const char * idname = identifier_name( code );
        char * lstr = getLevelCounters( &lvlstk, l );
        sprintf( strid, "%s%s", idname, lstr );
        return strid;
    }
    return NULL;
}

int64_t create_id_variable_code_for_current_scope( int64_t code, int variable_level /* for structs */ ) {
    const char * name = NULL;
    if ( !variable_level ) name = identifier_string_for_limited_scope_variable( code, getStackLevel(&lvlstk) );
    if ( !name ) name = identifier_name( code );
    return identifier_search_or_add( name );
}

VARIABLE * varspace_search_in_all_scopes( VARSPACE *n, int64_t code ) {
    VARIABLE * var;
    int l = getStackLevel(&lvlstk);
    for ( ; l >= 0; l-- ) {
        const char * name = identifier_string_for_limited_scope_variable( code, l );
        if ( name ) {
            int64_t id = identifier_search( name );
            if ( id && ( var = varspace_search( n, id ) ) ) return var;
        }
    }
    return NULL;
}

VARIABLE * varspace_search_in_current_scope( VARSPACE *n, int64_t code ) {
    const char * name = identifier_string_for_limited_scope_variable( code, getStackLevel(&lvlstk) );
    if ( !name ) name = identifier_name( code );
    return varspace_search( n, identifier_search( name ) );
}

VARIABLE * validate_variable( VARSPACE *n, TYPEDEF type, int duplicateignore ) {
    VARIABLE * var = varspace_search_in_current_scope( n, token.code );
    if ( var ) {
        if ( !duplicateignore ) compile_error( MSG_VARIABLE_REDECLARE );

        if ( !typedef_is_equal( type, var->type )) compile_error( MSG_VARIABLE_REDECLARE_DIFF );

        compile_warning( 0, MSG_VARIABLE_REDECLARE );
    }

    return var;
}

/*
 *  FUNCTION : compile_varspace
 *
 *  Compile a variable space (a LOCAL/PUBLIC, PRIVATE or GLOBAL section, or part of a STRUCT data)
 *
 *  PARAMS :
 *      n               Pointer to the VARSPACE object (already initialized)
 *      data            Pointer to the data segment. All variables are added at end.
 *      additive        1 if the compiling is additive (the struct members have non-local offset; GLOBAL case)
 *                      or not (STRUCT case)
 *      copies          Number of data copies (if the STRUCT is part of an array)
 *      collision       Check this varspace array for name collisions (NULL = end of array),
 *                      if nonzero (2006/11/19 19:34 GMT-03:00, Splinter - splintergu@gmail.com)
 *      alignment       Byte size for member alignment. Empty data will be added
 *                      to the varspace if its size is not multiple of this value.
 *
 *  In any case, nested varspaces (structs) will have an alignment of 0.
 *
 *  RETURN VALUE :
 *      None
 *
 */

int compile_varspace( VARSPACE * n, segment * data, int additive, int copies, VARSPACE ** collision, int alignment, int duplicateignore, int block_without_begin, int level, int is_inline ) {
    int i, j,
        total_count, last_count = 0,
        base_offset = data->current, /* base_offset is the current data pos of the varspace */
        total_length,
        sign_prefix = 0;
    expresion_result res;
    BASETYPE basetype = TYPE_UNDEFINED;
    TYPEDEF type;
    segment * segm = NULL;
    PROCDEF * proc = NULL, *p;

    /* Backup vars */
    BASETYPE basetype_last = TYPE_UNDEFINED;
    TYPEDEF type_last;
    segment * segm_last = NULL;

    /* Initialize some stuffs */
    type = typedef_new( TYPE_UNDEFINED );
    type_last = typedef_new( TYPE_UNDEFINED );

    for (;;) {
        int variable_already_exists = 0;

        if ( n->reserved == n->count ) varspace_alloc( n, 16 );

        if ( alignment && ( n->size % alignment ) > 0 ) {
            int extra = alignment - ( n->size % alignment );
/*
            if ( n->reserved <= n->count + extra ) {
                varspace_alloc( n, extra + 16 );
            }
*/
            segment_ensure_capacity(data, extra);
            n->size += extra;
            data->current += extra;
        }

        token_next();
        tok_pos tokp = token_pos();

        /* It skips commas and semicolons */
        if ( token.type == NOTOKEN ) break;

        if ( token.type != IDENTIFIER ) compile_error( MSG_INCOMP_TYPE );

        if ( token.code == identifier_comma ) {
            basetype = basetype_last;
            type = type_last;
            segm = segm_last;
            continue;
        }
        else
        if ( token.code == identifier_semicolon ) {
            if ( block_without_begin ) break;

            basetype = TYPE_UNDEFINED;
            set_type( &type, TYPE_UNDEFINED );

            basetype_last = basetype;
            type_last = type;
            segm = NULL;
            proc = NULL;
            continue;
        }
        else
        if ( !block_without_begin && token.code == identifier_end ) {
            break;
        }
        else
        if ( !( identifier_is_basic_type( token.code ) || token.code == identifier_struct || procdef_search( token.code ) )
             && ( token.code < reserved_words || sysproc_by_name( token.code ) != NULL )
             && token.code != identifier_pointer
             && token.code != identifier_multiply
           ) {
            token_back();
            break;
        }

        /* "Sign" */
        if ( token.code == identifier_unsigned ) {
            sign_prefix = 1;
            token_next();
        } else if ( token.code == identifier_signed ) {
            sign_prefix = -1;
            token_next();
        }

        if ( identifier_is_basic_type(token.code) ) {
            basetype = typedef_basic_type_basetype_by_name( token.code, sign_prefix );
            if ( basetype == TYPE_UNDEFINED ) compile_error( MSG_INVALID_TYPE );
            token_next();
            sign_prefix = 0;
        }
        else {
            if ( ( p = procdef_search( token.code ) ) ) { /* Process-type variables */
                proc = p;
                basetype = TYPE_INT;
                token_next();
            }
            else
            if ( token.type == IDENTIFIER && token.code >= reserved_words && !segment_by_name( token.code ) ) {
                int64_t code = token.code;
                token_next();
                if ( token.type == IDENTIFIER && token.code >= reserved_words ) {
                    proc = procdef_new( procdef_getid(), code );
                    basetype = TYPE_INT;
                }
                else
                {
                    token_back();
                }
            }
        }

        if ( basetype != TYPE_STRUCT ) {
            /* User-defined data type */
            if ( ( segm = segment_by_name( token.code ) ) ) {
                basetype = TYPE_STRUCT;
                type = *typedef_by_name( token.code );
                token_next();
            }
            else
            {
                type = typedef_new( basetype );
            }
        }

        if ( block_without_begin && basetype == TYPE_UNDEFINED && varspace_search_in_current_scope( n, token.code ) ) { /* end */
            token_set_pos( tokp );
            break;
        }

        tok_pos tokp1 = token_pos();
        token_next();
        if ( token.code == identifier_leftcb ) { /* process without type */
            token_set_pos( tokp );
            compile_process();
            break;
        }
        token_set_pos( tokp1 );

        /* Variable type required */
        if ( basetype == TYPE_UNDEFINED && token.code != identifier_struct ) compile_error( MSG_DATA_TYPE_REQUIRED ); /* Data Type Required */

        if ( token.type == IDENTIFIER && token.code == identifier_struct ) {
            type.chunk[0].type = TYPE_STRUCT;
            type.chunk[0].count = 1;
            type.depth = 1;
            token_next();
            segm = NULL;
        }

        basetype_last = basetype;
        type_last = type;
        segm_last = segm;

        /* Derived data types */
        while ( token.type == IDENTIFIER && ( token.code == identifier_pointer || token.code == identifier_multiply ) ) {
            type = typedef_enlarge( type );
            type.chunk[0].type = TYPE_POINTER;
            basetype = TYPE_POINTER;
            segm = NULL;
            token_next();
        }

        /* Data name */
        if ( token.type != IDENTIFIER ) compile_error( MSG_IDENTIFIER_EXP );

        if ( !level ) {
            if ( procdef_search( token.code ) ) compile_error(MSG_VARIABLE_ERROR);

            if ( token.code < reserved_words ) {
                if ( proc ) compile_error( MSG_VARIABLE_ERROR );
                token_back();
                break;
            }

            if ( constants_search( token.code ) ) compile_error( MSG_CONSTANT_REDECLARED_AS_VARIABLE );
        }

        if ( collision )
            for ( i = 0; collision[i]; i++ )
                if ( varspace_search( collision[i], token.code ) ) compile_error( MSG_VARIABLE_REDECLARE_IN_DIFF_CONTEXT );

        tok_pos var_pos = token_pos();

        VARIABLE *var = &n->vars[n->count];

        var->code = token.code;
        var->offset = data->current;

        /* Non-additive STRUCT; use zero-based member offsets */

        if ( !additive ) var->offset -= base_offset;

        token_next();

        /* Compiles a non-predefined structure */
        if ( !segm && typedef_is_struct( type ) ) {
            VARSPACE * members;

            type.chunk[0].count = 1;
            int count = 1;
            while ( token.type == IDENTIFIER && token.code == identifier_leftb ) {
                res = compile_expresion( 1, 0, 1, TYPE_INT );
                if ( !typedef_is_integer( res.type ) ) compile_error( MSG_INTEGER_REQUIRED );
                count *= res.value + 1;
                type = typedef_enlarge( type );
                type.chunk[0].type  = TYPE_ARRAY;
                type.chunk[0].count = res.value + 1;
                token_next();
                if ( token.type != IDENTIFIER || token.code != identifier_rightb ) compile_error( MSG_EXPECTED, "]" );
                token_next();
            }
            token_back();

            /* Reverses the indices [10][5] -> [5][10] */
            type = reverse_indices( type );

            members = ( VARSPACE * )calloc( 1, sizeof( VARSPACE ) );
            if ( !members ) compile_error( MSG_OUT_OF_MEMORY );
            varspace_init( members );

            ( void ) compile_varspace( members, data, is_inline /* test */, count, NULL, 0, duplicateignore, 0, level + 1, is_inline );

            type.varspace = members;

            /* check if variable exists and compare types */
            tok_pos actual_pos = token_pos();
            token_set_pos( var_pos );
            VARIABLE * var_aux = validate_variable( n, type, duplicateignore );
            if ( !var_aux ) var->code = create_id_variable_code_for_current_scope( token.code, level );
            token_set_pos( actual_pos );

            token_next();
            if ( token.type == IDENTIFIER && token.code == identifier_equal ) {
                if ( var_aux ) {
                    variable_already_exists = 1;
                    var->offset = var_aux->offset;
                }

                int64_t current_offset = data->current;
                data->current = var->offset;
                compile_struct_data( members, data, count, 0, is_inline );
                data->current = current_offset;
            } else {
                token_back();
            }

            if ( var_aux ) { /* variable already exists */
                varspace_destroy( members );
                continue;
            }

            for ( i = 0 ; i < members->stringvar_count ; i++ ) varspace_varstring( n, members->stringvars[i] );

            n->size += typedef_size( type );
            var->type = type;
            n->count++;

            continue ;  /* No ; */

        } else
        /* Compile an array */
        if ( token.type == IDENTIFIER && token.code == identifier_leftb ) {
            total_count = 1;

            while ( token.type == IDENTIFIER && token.code == identifier_leftb ) {
                if ( type.depth == MAX_TYPECHUNKS ) compile_error( MSG_TOO_MANY_ARRAY_LEVELS );

                type = typedef_enlarge( type );
                type.chunk[0].type = TYPE_ARRAY;

                token_next();
                if ( token.type == IDENTIFIER && token.code == identifier_rightb ) {
                    /* Undefined size dimension only is allowed in arrays of 1 dimension */
                    type.chunk[0].count = 0;
                    if ( total_count != 1 ) compile_error( MSG_UNDEFINED_MULTIPLE_ARRAY );
                    total_count = 0;
                    last_count = 0;
                }
                else
                {
                    /* Undefined size dimension only is allowed in arrays of 1 dimension */
                    if ( !total_count ) compile_error( MSG_UNDEFINED_MULTIPLE_ARRAY);
                    token_back();
                    res = compile_expresion( 1, 0, 1, TYPE_QWORD );
                    total_count *= res.value + 1;           /* + 1 last index of this dimension */
                    last_count = res.value + 1;             /* + 1 last index of this dimension */
                    type.chunk[0].count = res.value + 1;    /* + 1 last index of this dimension */
                    token_next();
                    if ( token.type != IDENTIFIER || token.code != identifier_rightb ) compile_error( MSG_EXPECTED, "]" );
                }
                token_next();
            }

            /* Reverses the indices [10][5] -> [5][10] */
            type = reverse_indices( type );

            /* check if variable exists and compare types */
            tok_pos actual_pos = token_pos();
            token_set_pos( var_pos );

            VARIABLE * var_aux = validate_variable( n, type, duplicateignore );
            if ( !var_aux ) var->code = create_id_variable_code_for_current_scope( token.code, level );
            token_set_pos( actual_pos );

            if ( token.type == IDENTIFIER && token.code == identifier_equal ) {
                if ( var_aux ) {
                    variable_already_exists = 1;
                    var->offset = var_aux->offset;
                }

                /* Structs arrays */
                if ( segm ) {
                    for ( i = 0 ; i < total_count ; i++ ) segment_add_from( data, segm );
                    int64_t current_offset = data->current;
                    data->current = var->offset;
                    compile_struct_data( type.varspace, data, typedef_count( type ), 0, is_inline );

                    if ( variable_already_exists ) { /* variable already exists */
                        data->current = current_offset;
                        continue;
                    }

                    if ( !type.chunk[0].count ) type.chunk[0].count = ( data->current - current_offset ) / typedef_size( type_last );
                    else                        data->current = current_offset; /* Only if it had already been allocated */
                }
                else
                /* Basic arrays */
                {
                    int64_t current_offset = data->current;
                    if ( variable_already_exists ) data->current = var->offset;
  
                    int count = compile_array_data( n, data, total_count, last_count, &basetype, is_inline );
                    assert( basetype != TYPE_UNDEFINED );
                    set_type( &type, basetype );


                    if ( variable_already_exists ) { /* variable already exists */
                        data->current = current_offset;
                        continue;
                    }

                    if ( total_count == 0 ) type.chunk[0].count = count;
                    else
                    for ( ; count < total_count; count++ ) {
                        if ( basetype == TYPE_STRING ) varspace_varstring( n, data->current );
                        segment_add_as( data, 0, basetype );
                    }
                }
            }
            else
            /* Arrays without value */
            {
                /* If the variable already exists then ignore */
                if ( var_aux ) continue;

                /* Struct Array */
                if ( segm ) {
                    /* Error if the variable is an array and does not have a specified dimension or initialization values */
                    if ( total_count == 0 ) compile_error( MSG_EXPECTED, "=" );

                    int string_offset = 0;

                    for ( i = 0; i < total_count; i++ ) {
                        segment_add_from( data, segm );
                        for ( j = 0; j < type.varspace->stringvar_count; j++ )
                            varspace_varstring( n, type.varspace->stringvars[j] + string_offset );
                        string_offset += type.varspace->size;
                    }
                    token_back();
                }
                else
                /* Simple Array */
                {
                    /* Error if the variable is an array and does not have a specified dimension or initialization values */
                    if ( type.chunk[0].count == 0 ) compile_error( MSG_EXPECTED, "=" );

                    if ( basetype == TYPE_UNDEFINED ) {
                        basetype = TYPE_INT;
                        set_type( &type, basetype );
                    }

                    for ( i = 0; i < total_count; i++ ) {
                        if ( basetype == TYPE_STRING ) varspace_varstring( n, data->current );
                        segment_add_as( data, 0, basetype );
                    }
                    token_back();
                }
            }
        }
        else
        /* Compile non-struct and non-array */
        {
            /* check if variable exists and compare types */
            tok_pos actual_pos = token_pos();
            token_set_pos( var_pos );
            VARIABLE * var_aux = validate_variable( n, type, duplicateignore );
            if ( !var_aux ) var->code = create_id_variable_code_for_current_scope( token.code, level );
            token_set_pos( actual_pos );

            /* Compiles an assignment of default values (variable initialization in declaration) */
            if ( token.type == IDENTIFIER && token.code == identifier_equal ) {
                int64_t current_offset = data->current;

                if ( var_aux ) {
                    variable_already_exists = 1;
                    var->offset = var_aux->offset;
                    data->current = var->offset;
                }

                if ( segm ) {
                    segment_add_from( data, segm );
                    int64_t current_offset = data->current; /* 2nd current_offset */
                    data->current = var->offset;
                    if ( !additive ) data->current += base_offset;
                    compile_struct_data( type.varspace, data, 1, 0, is_inline );
                    data->current = current_offset;
                }
                else
                {
                    if ( is_inline ) {
                        codeblock_add( code, MN_PRIVATE | mntype( type, 0 ), var->offset );
                    }

                    res = compile_expresion( !is_inline, 0, !is_inline, basetype );

                    if ( basetype == TYPE_UNDEFINED ) {
                        basetype = typedef_base( res.type );
                        if ( basetype == TYPE_UNDEFINED ) compile_error( MSG_INCOMP_TYPE );
                        set_type( &type, basetype );
                    }

                    if ( basetype == TYPE_DOUBLE ) {
                        segment_add_as( data, *( int64_t * ) &res.fvalue, basetype );
                    } else if ( basetype == TYPE_FLOAT ) {
                        float f = ( float ) res.fvalue;
                        segment_add_as( data, *( int32_t * ) &f, basetype );
                    } else {
                        if ( basetype == TYPE_STRING ) varspace_varstring( n, data->current );
                        segment_add_as( data, res.value, basetype );
                    }

                    if ( is_inline ) {
                        codeblock_add( code, MN_LETNP | mntype( res.type, 0 ), 0 );
                    }
                }

                if ( variable_already_exists ) {
                    data->current = current_offset;
                    continue;
                }
            }
            else
            {
                /* If the variable already exists then ignore */
                if ( var_aux ) continue;

                /* Compiles an nil value assignment  (variable initialization, without value) */
                /* Assigns default values (0) */
                if ( !segm ) {
                    if ( basetype == TYPE_UNDEFINED ) {
                        basetype = TYPE_INT;
                        set_type( &type, basetype );
                    }

                    if ( basetype == TYPE_STRING ) varspace_varstring( n, data->current );

                    segment_add_as( data, 0, basetype );
                    token_back();
                }
                else
                {
                    if ( typedef_is_struct( type ) )
                        for ( i = 0 ; i < type.varspace->stringvar_count ; i++ )
                            varspace_varstring( n, type.varspace->stringvars[i] + n->size );
                    segment_add_from( data, segm );
                    token_back();
                }
            }
        }

        if ( !variable_already_exists ) {
            n->size += typedef_size( type );
            var->type = type;
            n->count++;
        }

        /* Process-type variables, assign varspace to the type */
        if ( proc ) var->type.varspace = proc->pubvars;

        token_next();

        if ( token.type == IDENTIFIER && token.code == identifier_comma ) {
            token_back();
            continue;
        }

        if ( token.type == IDENTIFIER && token.code == identifier_semicolon ) {
            token_back();
            continue;
        }

        compile_error( MSG_EXPECTED, ";" );
        token_back();
        break;
    }

    n->last_offset = data->current;
    total_length = data->current - base_offset;

    /* n->size *= copies ; */
    while ( copies-- > 1 ) {
        for ( i = 0 ; i < n->stringvar_count ; i++ ) {
            if ( n->stringvars[i] >= base_offset && n->stringvars[i] < base_offset + total_length ) {
                varspace_varstring( n, n->stringvars[i] - base_offset + data->current );
            }
        }
        segment_copy( data, base_offset, total_length );
        base_offset += total_length;
    }

    return total_length;
}
