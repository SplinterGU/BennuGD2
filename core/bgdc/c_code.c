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
#include <stdlib.h>
#include <string.h>

#include "bgdc.h"
#include "levelstack.h"

extern int autodeclare;

int reduce_arrays = 1;

/* ---------------------------------------------------------------------- */
/* Returns the code that needs to be appended to a mnemonic to produce    */
/* a variant of it suitable for the specific data type                    */
/* ---------------------------------------------------------------------- */

int64_t mntype( TYPEDEF type, int accept_structs ) {
    BASETYPE t;

    while ( typedef_is_array( type ) ) type = typedef_reduce( type );

    t = typedef_base( type );
    switch ( t ) {
        case TYPE_QWORD     : return MN_QWORD | MN_UNSIGNED;
        case TYPE_INT       : return MN_QWORD;
        case TYPE_DWORD     : return MN_DWORD | MN_UNSIGNED;
        case TYPE_INT32     : return MN_DWORD;
        case TYPE_WORD      : return MN_WORD | MN_UNSIGNED;
        case TYPE_SHORT     : return MN_WORD;
        case TYPE_BYTE      : return MN_BYTE | MN_UNSIGNED;
        case TYPE_SBYTE     : return MN_BYTE;
        case TYPE_CHAR      : return MN_BYTE | MN_UNSIGNED;
        case TYPE_FLOAT     : return MN_FLOAT;
        case TYPE_DOUBLE    : return MN_DOUBLE;
        case TYPE_STRING    : return MN_STRING;
        case TYPE_POINTER   : return MN_QWORD;
        case TYPE_UNDEFINED :
        case TYPE_ARRAY     :
        case TYPE_STRUCT    :
            break;
    }

    if ( t == TYPE_STRUCT && accept_structs ) return 0;

    compile_error( MSG_INCOMP_TYPE );
    return 0;
}

/* ---------------------------------------------------------------------- */

void basetype_describe( char * buffer, BASETYPE t ) {
    switch ( t ) {
        case TYPE_INT:
            sprintf( buffer, "INT" );
            return;

        case TYPE_QWORD:
            sprintf( buffer, "QWORD" );
            return;

        case TYPE_INT32:
            sprintf( buffer, "INT32" );
            return;

        case TYPE_DWORD:
            sprintf( buffer, "DWORD" );
            return;

        case TYPE_SHORT:
            sprintf( buffer, "SHORT" );
            return;

        case TYPE_WORD:
            sprintf( buffer, "WORD" );
            return;

        case TYPE_BYTE:
            sprintf( buffer, "BYTE" );
            return;

        case TYPE_CHAR:
            sprintf( buffer, "CHAR" );
            return;

        case TYPE_SBYTE:
            sprintf( buffer, "SIGNED BYTE" );
            return;

        case TYPE_STRING:
            sprintf( buffer, "STRING" );
            return;

        case TYPE_DOUBLE:
            sprintf( buffer, "DOUBLE" );
            return;

        case TYPE_FLOAT:
            sprintf( buffer, "FLOAT" );
            return;

        case TYPE_STRUCT:
            sprintf( buffer, "STRUCT" );

        case TYPE_ARRAY:
            sprintf( buffer, "ARRAY" );
            return;

        case TYPE_POINTER:
            sprintf( buffer, "POINTER" );
            return;

        case TYPE_UNDEFINED:
        default:
            sprintf( buffer, "<UNDEFINED>" );
            return;
    }
}

/* ---------------------------------------------------------------------- */
/* Expression and statement compiler. This module contains all the        */
/* compilation functions that generate effective code.                    */
/* ---------------------------------------------------------------------- */

PROCDEF * proc = NULL;
CODEBLOCK * code = NULL;

/* Checks that the parameters of a binary expression are numeric data */
/* and returns the operation type (MN_FLOAT or MN_QWORD)               */

static int64_t check_integer_type( expresion_result *exp ) {
    if ( typedef_is_pointer( exp->type ) || typedef_base( exp->type ) == TYPE_CHAR ) {
/*        codeblock_add(code, MN_POINTER2BOL, 0) ; */
        exp->type = typedef_new( TYPE_QWORD );
    }

    if ( typedef_is_integer( exp->type ) ) {
        BASETYPE t = typedef_base( exp->type );
        switch (t) {
            case TYPE_INT32     :
            case TYPE_DWORD     :
            case TYPE_FLOAT     :
                return MN_DWORD;

            case TYPE_SHORT     :
            case TYPE_WORD      :
                return MN_WORD;

            case TYPE_SBYTE	    :
            case TYPE_BYTE      :
            case TYPE_CHAR      : // <<< ????
                return MN_BYTE;

            case TYPE_INT       :
            case TYPE_QWORD     :

            case TYPE_UNDEFINED :
            case TYPE_DOUBLE    :
            case TYPE_STRING    :
            case TYPE_ARRAY     :
            case TYPE_STRUCT    :
            case TYPE_POINTER   :
                break;
        }
        return MN_QWORD;
    }

    compile_error( MSG_INTEGER_REQUIRED );
    return 0;
}

static int64_t check_integer_types( expresion_result *left, expresion_result *right ) {
    if ( typedef_is_pointer( left->type ) || typedef_base( left->type ) == TYPE_CHAR ) {
/*        codeblock_add(code, MN_POINTER2BOL, 1) ; */
        left->type = typedef_new( TYPE_QWORD );
    }

    if ( typedef_is_pointer( right->type ) || typedef_base( right->type ) == TYPE_CHAR ) {
/*        codeblock_add(code, MN_POINTER2BOL, 0) ; */
        right->type = typedef_new( TYPE_QWORD );
    }

    if ( typedef_is_integer( left->type ) && typedef_is_integer( right->type ) ) {
        if ( typedef_base( left->type ) == typedef_base( right->type ) ) {
            BASETYPE t = typedef_base( left->type );
            switch (t) {
                case TYPE_INT32     :
                case TYPE_DWORD     :
                case TYPE_FLOAT     :
                    return MN_DWORD;

                case TYPE_SHORT	    :
                case TYPE_WORD	    :
                    return MN_WORD;

                case TYPE_SBYTE	    :
                case TYPE_BYTE	    :
                case TYPE_CHAR      : // <<< ????
                    return MN_BYTE;

                case TYPE_INT       :
                case TYPE_QWORD     :
                    return MN_QWORD;

                case TYPE_UNDEFINED :
                case TYPE_DOUBLE    :
                case TYPE_STRING    :
                case TYPE_ARRAY     :
                case TYPE_STRUCT    :
                case TYPE_POINTER   :
                    break;
            }
        }
        if ( typedef_base( left->type ) > typedef_base( right->type ) ) return mntype( left->type, 0 );
        return mntype( right->type, 0 );
//        return MN_QWORD;
    }

    compile_error( MSG_INTEGER_REQUIRED );
    return 0;
}

static int64_t check_numeric_types( expresion_result *left, expresion_result *right ) {
    if ( typedef_is_double( left->type ) ) {
        if ( typedef_is_double( right->type ) ) return MN_DOUBLE;

        if ( typedef_is_float( right->type ) ) {
            codeblock_add( code, MN_FLOAT2DOUBLE, 0 );
            left->type = typedef_new( TYPE_DOUBLE );
            right->fvalue = ( double )right->value;
            return MN_DOUBLE;
        }

        if ( typedef_is_integer( right->type ) ) {
            codeblock_add( code, MN_INT2DOUBLE | mntype( right->type, 0 ), 0 );
            if ( typedef_is_unsigned( right->type ) ) {
                right->fvalue = (uint64_t)right->value;
            } else {
                right->fvalue = (double)right->value;
            }
            left->type = typedef_new( TYPE_DOUBLE );
            return MN_DOUBLE;
        }
    }

    if ( typedef_is_float( left->type ) ) {
        if ( typedef_is_float( right->type ) ) return MN_FLOAT;

        if ( typedef_is_double( right->type ) ) {
            codeblock_add( code, MN_FLOAT2DOUBLE, 1 );
            left->type = typedef_new( TYPE_DOUBLE );
            return MN_DOUBLE;
        }

        if ( typedef_is_integer( right->type ) ) {
            codeblock_add( code, MN_INT2FLOAT | mntype( right->type, 0 ), 0 );
            if ( typedef_is_unsigned( right->type ) ) {
                right->fvalue = (uint64_t)right->value;
            } else {
                right->fvalue = (float)right->value;
            }
            left->type = typedef_new( TYPE_FLOAT );
            return MN_FLOAT;
        }
    }

    if ( typedef_is_integer( left->type ) || typedef_is_pointer( left->type ) ) {
        if ( typedef_is_double( right->type ) ) {
            codeblock_add( code, MN_INT2DOUBLE | mntype( left->type, 0 ), 1 );
            if ( typedef_is_unsigned( left->type ) ) {
                left->fvalue = (uint64_t)left->value;
            } else {
                left->fvalue = (double)left->value;
            }
            left->type = typedef_new( TYPE_DOUBLE );
            return MN_DOUBLE;
        }

        if ( typedef_is_float( right->type ) ) {
            codeblock_add( code, MN_INT2FLOAT | mntype( left->type, 0 ), 1 );
            if ( typedef_is_unsigned( left->type ) ) {
                left->fvalue = (uint64_t)left->value;
            } else {
                left->fvalue = (float)left->value;
            }
            left->type = typedef_new( TYPE_FLOAT );
            return MN_FLOAT;
        }

        if ( typedef_is_pointer( right->type ) ) {
            left->type = typedef_new( TYPE_POINTER );
            return MN_QWORD;
        }

        if ( typedef_is_integer( right->type ) ) {
            if ( typedef_base( left->type ) <= typedef_base( right->type ) ) return mntype( left->type, 0 );
            left->type = right->type;
            return mntype( right->type, 0 );
        }
    }

    if ( typedef_base( left->type ) == TYPE_CHAR ) {
        if ( typedef_base( right->type ) == TYPE_STRING ) {
            codeblock_add( code, MN_STR2CHR, 0 );
            if ( right->constant == 1 ) right->value = ( unsigned char ) * ( string_get( right->value ) );
            right->type = typedef_new( TYPE_CHAR );
            return MN_BYTE;
        }
        if ( typedef_base( right->type ) == TYPE_CHAR || typedef_is_integer( right->type ) ) return MN_BYTE;
    }

    if ( typedef_base( right->type ) == TYPE_CHAR ) {
        if ( typedef_base( left->type ) == TYPE_STRING ) {
            codeblock_add( code, MN_STR2CHR, 1 );
            if ( left->constant == 1 ) left->value = ( unsigned char ) * ( string_get( left->value ) );
            left->type = typedef_new( TYPE_CHAR );
            return MN_BYTE;
        }
        if ( typedef_base( left->type ) == TYPE_CHAR || typedef_is_integer( left->type ) ) return MN_BYTE;
    }

    compile_error( MSG_INCOMP_TYPES );
    return 0;
}


/* Checks that the parameters of a binary expression are strings
 * or numeric data and returns MN_STRING or the numeric data type */

static int64_t check_numeric_or_string_types( expresion_result * left, expresion_result * right ) {
    if ( typedef_is_array( left->type ) && left->type.chunk[1].type == TYPE_CHAR && typedef_is_string( right->type ) ) {
        left->type = typedef_new( TYPE_STRING );
        left->lvalue = 0;
        codeblock_add( code, MN_A2STR, 1 );
        return MN_STRING;
    }
    if ( typedef_is_array( right->type ) && right->type.chunk[1].type == TYPE_CHAR && typedef_is_string( left->type ) ) {
        right->type = typedef_new( TYPE_STRING );
        right->lvalue = 0;
        codeblock_add( code, MN_A2STR, 0 );
        return MN_STRING;
    }
    if ( typedef_is_array( right->type ) && right->type.chunk[1].type == TYPE_CHAR &&
         typedef_is_array( left->type )  &&  left->type.chunk[1].type == TYPE_CHAR )
    {
        left->type = typedef_new( TYPE_STRING );
        right->type = typedef_new( TYPE_STRING );
        left->lvalue = 0;
        right->lvalue = 0;
        codeblock_add( code, MN_A2STR, 0 );
        codeblock_add( code, MN_A2STR, 1 );
        return MN_STRING;
    }

    if ( typedef_base( left->type ) == TYPE_CHAR && typedef_base( right->type ) == TYPE_STRING ) {
        codeblock_add( code, MN_CHR2STR, 1 );
        left->lvalue = 0;
        left->type = typedef_new( TYPE_STRING );
        return MN_STRING;
    }

    if ( typedef_base( right->type ) == TYPE_CHAR && typedef_base( left->type ) == TYPE_STRING ) {
        codeblock_add( code, MN_CHR2STR, 0 );
        right->lvalue = 0;
        right->type = typedef_new( TYPE_STRING );
        return MN_STRING;
    }

    if ( typedef_is_string( left->type ) && typedef_is_string( right->type ) ) return MN_STRING;
    if ( typedef_is_string( left->type ) || typedef_is_string( right->type ) ) compile_error( MSG_INCOMP_TYPES );

    return check_numeric_types( left, right );
}

/* Compiles the access to a global, local, private, or public VARIABLE */

expresion_result compile_sublvalue( VARSPACE * from, int base_offset, VARSPACE * remote ) {
    VARIABLE * var = NULL;
    VARSPACE * here = from;
    VARSPACE * privars = ( proc ? proc->privars : NULL );
    VARSPACE * pubvars = ( proc ? proc->pubvars : NULL );
    expresion_result res = { 0 }, ind;

    if ( here ) token_next();

    if ( token.type != IDENTIFIER ) compile_error( MSG_IDENTIFIER_EXP );

    if ( !here && !remote ) {
        /* Locals */
        if ( proc ) {
            here = privars;
//            var = varspace_search( here, token.code );
            var = varspace_search_in_all_scopes( here, token.code );
            if ( !var ) {
                here = pubvars;
                var = varspace_search( here, token.code );
            }
        }

        if ( !var ) {
            here = &local;
            var = varspace_search( here, token.code );
        }

        if ( !var ) {
            here = &global;
            var = varspace_search( here, token.code );
        }
    } else {
        if ( remote ) {
            here = remote;
            if ( here == privars ) var = varspace_search_in_all_scopes( here, token.code );
            else
            var = varspace_search( here, token.code );
        }

        if ( !var ) {
            here = from;
            if ( here == privars ) var = varspace_search_in_all_scopes( here, token.code );
            else
            var = varspace_search( here, token.code );
        }
    }

    if ( !var ) {
        compile_error( MSG_UNKNOWN_IDENTIFIER );
        return res; /* Avoid scan-build warning */
    }

    if ( var->offset - base_offset != 0 ||
         here == &global ||
         here == &local  ||
         ( privars && ( here == privars ) ) ||
         ( pubvars && ( here == pubvars ) ) ||
         ( remote  && ( here == remote ) )
       )
    {
        codeblock_add( code, (
                    remote && here == remote            ? MN_REMOTE_PUBLIC  :
                    here == &global                     ? MN_GLOBAL         :
                    here == &local && here == from      ? MN_REMOTE         :
                    here == &local                      ? MN_LOCAL          :
                    privars && here == privars          ? MN_PRIVATE        :
                    pubvars && here == pubvars          ? MN_PUBLIC         :
                    MN_INDEX
                ) | mntype( var->type, 1 ), var->offset - base_offset );

        /* Also the remote locals? */
        if (( pubvars && ( here == pubvars ) ) || ( remote  && ( here == remote ) ) ) {
            proc->flags |= PROC_USES_PUBLICS;
        }

        if ( here == &local ) {
            proc->flags |= PROC_USES_LOCALS;
        }
    }

    token_next();

    res.type       = var->type;
    res.lvalue     = 1;
    res.asignation = 0;
    res.constant   = 0;
    res.call       = 0;
    res.value      = 0;
    res.fvalue     = 0;
    res.count      = 1;

    /* Indexed via [...] */

    while ( token.type == IDENTIFIER && token.code == identifier_leftb ) { /* "[" */
        /* From structures or arrays */
        if ( typedef_is_struct( res.type ) && typedef_count( res.type ) == 1 ) compile_error( MSG_NOT_AN_ARRAY );

        /* Strings and pointers are indexed at another level */
        if ( typedef_is_pointer( res.type ) || typedef_is_string( res.type ) ) break;

        if ( !typedef_is_struct( res.type ) && !typedef_is_array( res.type ) ) compile_error( MSG_NOT_AN_ARRAY );

        ind = compile_expresion( 0, 0, 0, TYPE_QWORD );
        if ( ind.lvalue ) codeblock_add( code, MN_PTR | mntype( ind.type, 0 ), 0 );

        if ( ind.constant && ( ind.value < 0 || ind.value >= typedef_count( res.type ) ) ) compile_error( MSG_BOUND );

        token_next();
        if ( token.type != IDENTIFIER || token.code != identifier_rightb ) compile_error( MSG_EXPECTED, "]" ) ; /* "]" */

        if ( typedef_is_array( res.type ) ) {
            res.type = typedef_reduce( res.type );
            codeblock_add( code, MN_ARRAY, typedef_size( res.type ) );
        } else {
            /* Struct */
            codeblock_add( code, MN_ARRAY, typedef_size( res.type ) / typedef_count( res.type ) );
        }
        token_next();
    }

    /* An access to an array is an access to its first element */
/*
    res.count = 1;
    if ( reduce_arrays == 1 && typedef_is_array( res.type ) )
    {
        if ( res.type.chunk[1].type != TYPE_CHAR )
        {
            while ( typedef_is_array( res.type ) )
            {
                res.count *= ( typedef_count( res.type ) ? typedef_count( res.type ) : 1 );
                res.type = typedef_reduce( res.type );
            }
        }
    }
*/

    token_back();

    return res;
}

/* Compiles the size of a local, global, private, or public VARIABLE or structure */

#define MAX_EXPR_LEVEL 1024

uint64_t compile_sizeof( VARSPACE * here, int64_t * content_size, int64_t * content_type, int64_t * parent_count ) {
    VARIABLE * var  = NULL;
    expresion_result ind;
    TYPEDEF type, * usertype;
    int64_t * _content_type = NULL, tcode;
    int64_t base = 0, _content_size = 0, _parent_count = 0, /*check_datatype = 0, */ index_pointer = 0;

    if ( !content_size ) content_size = &_content_size;
    if ( !content_type ) content_type = _content_type = ( int64_t * ) calloc( MAX_EXPR_LEVEL, sizeof( int64_t ) );
    if ( !parent_count ) parent_count = &_parent_count;

    /* Basic datatypes */

    token_next();
    /* ignore "UNSIGNED" or "SIGNED" */
    if ( token.type != IDENTIFIER && ( token.code == identifier_unsigned || token.code == identifier_signed ) ) {
//        check_datatype = 1;
        token_next();
    }

    if ( token.type != IDENTIFIER ) compile_error( MSG_INCOMP_TYPE );

    /* Base datatypes */

         if ( token.code == identifier_uint64 )     base = 8;
    else if ( token.code == identifier_int64 )      base = 8;
    else if ( token.code == identifier_double )     base = 8;
    else if ( token.code == identifier_string )     base = 8;
    else if ( token.code == identifier_uint32 )     base = 4;
    else if ( token.code == identifier_int32 )      base = 4;
    else if ( token.code == identifier_float )      base = 4;
    else if ( token.code == identifier_int16 )      base = 2;
    else if ( token.code == identifier_uint16 )     base = 2;
    else if ( token.code == identifier_char )       base = 1;
    else if ( token.code == identifier_uint8 )      base = 1;
    else {
        usertype = typedef_by_name( token.code );
        if ( usertype ) base = typedef_size( *usertype );
    }

    if ( base ) {
        token_next();
        if ( token.type == IDENTIFIER && token.code != identifier_point ) {
            token_back();
            for (;;) {
                token_next();
                if ( token.type == IDENTIFIER && ( token.code == identifier_pointer || token.code == identifier_multiply ) ) { /* "POINTER" */
                    base = 8;
                    continue;
                }
                token_back();
                break;
            }
            if ( _content_type ) free( _content_type );
            return base;
        }
        token_back();
    }

// commented by no sizeof(array)   if ( !base /*check_datatype*/ ) compile_error( MSG_INVALID_TYPE );

    /* Expressions datatypes */

    /* Count "*" or "[" */

    while ( token.type == IDENTIFIER && ( token.code == identifier_multiply || token.code == identifier_leftb || token.code == identifier_leftp ) ) {
        if ( *content_size == MAX_EXPR_LEVEL ) compile_error( MSG_TOO_COMPLEX );
        if ( token.code == identifier_leftp )( *parent_count )++;
        content_type[( *content_size )++] = token.code;
        token_next();
    }

    if ( !here ) {
        /* Locals */
        if ( proc ) {
            here = proc->privars;
            var = varspace_search( here, token.code );
            if ( !var ) {
                here = proc->pubvars;
                var = varspace_search( here, token.code );
            }
        }
        if ( !var ) {
            here = &local;
            var = varspace_search( here, token.code );
        }
        if ( !var ) {
            here = &global;
            var = varspace_search( here, token.code );
        }
    } else {
        var = varspace_search( here, token.code );
    }

    if ( !var ) {
        compile_error( MSG_UNKNOWN_IDENTIFIER );
        if ( _content_type ) free( _content_type ); /* Avoid scan-build warning */
        return 0; /* Avoid scan-build warning */
    }

    type = var->type;

    token_next();

    /* Array indexing */

    while ( token.type == IDENTIFIER && token.code == identifier_leftb ) { /* "[" */
        CODEBLOCK_POS p = codeblock_pos( code );

        /* Strings and pointers are indexed at another level */
        if ( typedef_is_pointer( type ) || typedef_is_string( type ) ) break;

        /* From structures or arrays */
        if ( typedef_is_struct( type ) && typedef_count( type ) == 1 ) compile_error( MSG_NOT_AN_ARRAY );

        if ( !typedef_is_struct( type ) && !typedef_is_array( type ) ) compile_error( MSG_NOT_AN_ARRAY );

        ind = compile_expresion( 0, 0, 0, TYPE_QWORD );
        if ( !ind.constant || ( ind.value < 0 || ind.value >= typedef_count( type ) ) ) compile_error( MSG_BOUND );

        codeblock_setpos( code, p );

        token_next();
        if ( token.type != IDENTIFIER || token.code != identifier_rightb ) compile_error( MSG_EXPECTED, "]" ) ; /* "]" */

        if ( typedef_is_array( type ) ) type = typedef_reduce( type );
        else                            break;

        token_next();
    }

    if ( token.type == IDENTIFIER && token.code == identifier_leftb &&
         ( typedef_is_pointer( type ) || typedef_is_string( type ) ) ) { /* Pointer indexing ptr[0] or strings */
        /* No arrays come through here */
        CODEBLOCK_POS p = codeblock_pos( code );

        ind = compile_subexpresion();
        if ( !typedef_is_integer( ind.type ) ) compile_error( MSG_INTEGER_REQUIRED );

        token_next();
        if ( token.type != IDENTIFIER || token.code != identifier_rightb ) compile_error( MSG_EXPECTED, "]" ) ; /* "]" */

        codeblock_setpos( code, p );

        if ( typedef_is_pointer( type ) ) {
            type = typedef_reduce( type );
            token_next();
            index_pointer = 1;
        } else if ( typedef_is_string( type ) ) {
            if ( _content_type ) free( _content_type );
            return 1; /* String indexing */
        }
    }

    /* Check for pointer by ']' or ')' */
    while ( token.type == IDENTIFIER && ( token.code == identifier_rightb || token.code == identifier_rightp ) ) {
        if ( token.code == identifier_rightp ) { /* ')' */
            if ( !*content_size || !*parent_count ) break;

            while (( *content_size ) && content_type[( *content_size )-1] == identifier_multiply ) { /* only '*', '[]' must be already process */
                if ( !typedef_is_pointer( type ) ) compile_error( MSG_NOT_A_POINTER );
                type = typedef_reduce( type );
                ( *content_size )--;
            }

            if ( content_type[--( *content_size )] != identifier_leftp ) compile_error( MSG_INVALID_EXP );
            ( *parent_count )--;
            token_next();
        } else {
            if ( !*content_size ) compile_error( MSG_EXTRA_CHAR );

            while ( *content_size && ( ( tcode = content_type[( *content_size )-1] ) == identifier_leftb || tcode == identifier_multiply ) ) {
                /* '[' or '*' */
                if ( !typedef_is_pointer( type ) ) compile_error( MSG_NOT_A_POINTER );

                type = typedef_reduce( type );
                ( *content_size )--;
                if ( tcode == identifier_leftb ) {
                    token_next();
                    break;
                }
            }
        }
    }

    if ( index_pointer && ( token.type != IDENTIFIER || token.code != identifier_point ) ) { /* "." */
        token_back();
        if ( _content_type ) free( _content_type );
        return typedef_size( type ); /* Pointer indexing ptr[0] */
    }

    if ( token.type == IDENTIFIER && token.code == identifier_point ) { /* "." */
        int64_t ret;

        if ( typedef_is_pointer( type ) ) type = typedef_reduce( type );
        if ( !typedef_is_struct( type ) && typedef_base( type ) != TYPE_QWORD && typedef_base( type ) != TYPE_INT ) { /* Support for process type for public variables */
           compile_error( MSG_STRUCT_REQUIRED );
        }

        if ( typedef_is_struct( type ) || typedef_is_pointer( type ) || typedef_base( type ) == TYPE_QWORD || typedef_base( type ) == TYPE_INT ) {
            ret = compile_sizeof( typedef_members( type ), content_size, content_type, parent_count );
            if ( _content_type ) free( _content_type );
            return ret;
        }

        ret = compile_sizeof( &local, content_size, content_type, parent_count );
        if ( _content_type ) free( _content_type );
        return ret;
    }

    /*   Process "*ptr|**ptr|***ptr|..."      */
    while (( *content_size )-- ) {
        if ( content_type[*content_size] != identifier_multiply ) compile_error( MSG_INVALID_EXP );
        if ( !typedef_is_pointer( type ) ) compile_error( MSG_NOT_A_POINTER );
        type = typedef_reduce( type );
    }

    if ( _content_type ) free( _content_type );

    token_back();
    return typedef_size( type );

}

/*
 *  FUNCTION : convert_result_type
 *
 *  Given an expresion result in the current context, convert it
 *  if possible to the basic type given(and emit the necessary code)
 *
 *  PARAMS:
 *      res             Result of expression at current context
 *      t               Basic type required
 *
 *  RETURN VALUE:
 *      The converted type result
 */

expresion_result convert_result_type( expresion_result res, BASETYPE t ) {
    /* Type conversions */
    if ( t == TYPE_STRING && typedef_base( res.type ) == TYPE_POINTER ) {
        res.type = typedef_new( t ) ; /* Pointer -> String */
        codeblock_add( code, MN_POINTER2STR, 0 );
    }
    else
    if ( t == TYPE_POINTER && typedef_is_integer( res.type ) ) {
        res.type = typedef_new( t ) ; /* pointer */
    }
    else
    if ( t == TYPE_POINTER && typedef_is_double( res.type ) ) {
        res.type = typedef_new( t ) ; /* pointer */
        res.value = res.fvalue;
        codeblock_add( code, MN_DOUBLE2INT, 0 );
    }
    else
    if ( t == TYPE_POINTER && typedef_is_float( res.type ) ) {
        res.type = typedef_new( t ) ; /* pointer */
        res.value = res.fvalue;
        codeblock_add( code, MN_FLOAT2INT, 0 );
    }
    else
    if ( t == TYPE_POINTER && typedef_is_string( res.type ) ) {
        res.type = typedef_new( t ) ; /* pointer */
        res.value = string_atop( ( unsigned char * ) string_get( res.value ) );
        codeblock_add( code, MN_STR2POINTER, 0 );
    }
    else
    if ( t < TYPE_CHAR && typedef_base( res.type ) == TYPE_POINTER ) {
        res.type = typedef_new( t ) ; /* Pointer -> Int */
    }
    else
    if ( t < TYPE_CHAR && typedef_is_double( res.type ) ) {
        res.type = typedef_new( t );
        res.value = ( int64_t )res.fvalue;
        codeblock_add( code, MN_DOUBLE2INT | mntype( res.type, 0 ), 0 );
    }
    else
    if ( t < TYPE_CHAR && typedef_is_float( res.type ) ) {
        res.type = typedef_new( t );
        res.value = ( int64_t )res.fvalue;
        codeblock_add( code, MN_FLOAT2INT | mntype( res.type, 0 ), 0 );
    }
    else
    if ( t == TYPE_DOUBLE && typedef_is_integer( res.type ) ) {
        codeblock_add( code, MN_INT2DOUBLE | mntype( res.type, 0 ), 0 );
        if ( typedef_is_unsigned( res.type ) ) {
            res.fvalue = ( double )( uint64_t )res.value;
        } else {
            res.fvalue = ( double )res.value;
        }
        res.type = typedef_new( t );
    }
    else
    if ( t == TYPE_DOUBLE && typedef_is_float( res.type ) ) {
        res.type = typedef_new( t );
        res.value = ( double )res.fvalue;
        codeblock_add( code, MN_FLOAT2DOUBLE, 0 );
    }
    else
    if ( t == TYPE_FLOAT && typedef_is_integer( res.type ) ) {
        codeblock_add( code, MN_INT2FLOAT | mntype( res.type, 0 ), 0 );
        if ( typedef_is_unsigned( res.type ) ) {
            res.fvalue = ( float )( uint64_t )res.value;
        } else {
            res.fvalue = ( float )res.value;
        }
        res.type = typedef_new( TYPE_FLOAT );
    }
    else
    if ( t == TYPE_FLOAT && typedef_is_double( res.type ) ) {
        codeblock_add( code, MN_DOUBLE2FLOAT, 0 );
        res.type = typedef_new( t );
    }
    else
    if (( t == TYPE_BYTE || t == TYPE_WORD || t == TYPE_DWORD || t == TYPE_QWORD ) && typedef_is_integer( res.type ) ) {
        res.type = typedef_new( t );
    }
    else
    if ( t < TYPE_FLOAT && typedef_is_integer( res.type ) ) {
        res.type = typedef_new( t );
    }
    else
    if ( t == TYPE_STRING && typedef_is_integer( res.type ) ) {
        codeblock_add( code, MN_INT2STR | mntype( res.type, 0 ), 0 );
        if ( res.constant ) {
            char buffer[128];
            switch ( res.type.chunk[0].type ) {
                case TYPE_INT:
                    sprintf( buffer, "%" PRId64, res.value );
                    break;

                case TYPE_QWORD:
                    sprintf( buffer, "%" PRIu64, res.value );
                    break;

                case TYPE_INT32:
                    sprintf( buffer, "%d", ( int32_t )res.value );
                    break;

                case TYPE_DWORD:
                    sprintf( buffer, "%u", ( uint32_t )res.value );
                    break;

                case TYPE_SHORT:
                    sprintf( buffer, "%d", ( int16_t )res.value );
                    break;

                case TYPE_WORD:
                    sprintf( buffer, "%d", ( uint16_t )res.value );
                    break;

                case TYPE_SBYTE:
                    sprintf( buffer, "%d", ( int8_t )res.value );
                    break;

                case TYPE_BYTE:
                    sprintf( buffer, "%d", ( uint8_t )res.value );
                    break;

                case TYPE_UNDEFINED :
                case TYPE_CHAR      :
                case TYPE_FLOAT     :
                case TYPE_DOUBLE    :
                case TYPE_STRING    :
                case TYPE_ARRAY     :
                case TYPE_STRUCT    :
                case TYPE_POINTER   :
                    break;
            }
            res.value = string_new( buffer );
        }
        res.type = typedef_new( TYPE_STRING );
    }
    else
    if ( t == TYPE_STRING && typedef_is_float( res.type ) ) {
        codeblock_add( code, MN_FLOAT2STR, 0 );
        if ( res.constant ) {
            char buffer[128];
            sprintf( buffer, "%f", res.fvalue );
            res.value = string_new( buffer );
            res.fvalue = res.value;
        }
        res.type = typedef_new( TYPE_STRING );
    }
    else
    if ( t == TYPE_STRING && typedef_is_double( res.type ) ) {
        codeblock_add( code, MN_DOUBLE2STR, 0 );
        if ( res.constant ) {
            char buffer[128];
            sprintf( buffer, "%f", res.fvalue );
            res.value = string_new( buffer );
            res.fvalue = res.value;
        }
        res.type = typedef_new( TYPE_STRING );
    }
    else
    if ( t == TYPE_STRING && typedef_base( res.type ) == TYPE_CHAR ) {
        codeblock_add( code, MN_CHR2STR, 0 );
        if ( res.constant ) {
            char buffer[2];
            buffer[0] = res.value;
            buffer[1] = 0;
            res.value = string_new( buffer );
        }
        res.type = typedef_new( TYPE_STRING );
    }
    else
    if ( t != TYPE_UNDEFINED && typedef_base( res.type ) != t ) {
        switch ( t ) {
            case TYPE_CHAR:
                /* Allow string-to-char conversions */
                if ( typedef_is_string( res.type ) ) {
                    codeblock_add( code, MN_STR2CHR, 0 );
                    if ( res.constant == 1 ) res.value = ( unsigned char ) * ( string_get( res.value ) );
                } else
                    compile_error( MSG_INTEGER_REQUIRED );
                break;

            case TYPE_QWORD:
            case TYPE_INT:
            case TYPE_DWORD:
            case TYPE_INT32:
            case TYPE_WORD:
            case TYPE_SHORT:
            case TYPE_BYTE:
            case TYPE_SBYTE:
                if ( typedef_is_array( res.type ) && res.lvalue && res.type.chunk[1].type == TYPE_CHAR ) {
                    codeblock_add( code, MN_A2STR, 0 );
                    codeblock_add( code, MN_STR2INT, 0 );
                    res.lvalue = 0;
                    res.constant = 0;
                } else if ( typedef_is_string( res.type ) ) {
                    codeblock_add( code, MN_STR2INT, 0 );
                    if ( res.constant == 1 ) res.value = atoi( string_get( res.value ) );
                } else if ( typedef_base( res.type ) != TYPE_CHAR ) //{
//                    res.type = typedef_new( t );
//                } else
                    compile_error( MSG_INTEGER_REQUIRED );
                break;

            case TYPE_DOUBLE:
                if ( typedef_is_string( res.type ) ) {
                    codeblock_add( code, MN_STR2DOUBLE, 0 );
                    if ( res.constant == 1 ) res.fvalue = ( double )atof( string_get( res.value ) );
                } else {
                    compile_error( MSG_NUMBER_REQUIRED );
                }
                break;

            case TYPE_FLOAT:
                if ( typedef_is_string( res.type ) ) {
                    codeblock_add( code, MN_STR2FLOAT, 0 );
                    if ( res.constant == 1 ) res.fvalue = ( float )atof( string_get( res.value ) );
                } else {
                    compile_error( MSG_NUMBER_REQUIRED );
                }
                break;

            case TYPE_STRING:
                if ( typedef_is_array( res.type ) && res.lvalue && res.type.chunk[1].type == TYPE_CHAR ) {
                    codeblock_add( code, MN_A2STR, 0 );
                    res.lvalue = 0;
                } else if ( typedef_is_integer( res.type ) ) {
                    codeblock_add( code, MN_INT2STR | mntype( res.type, 0 ), 0 );
                    if ( res.constant ) {
                        char buffer[32];
                        sprintf( buffer, "%" PRId64, res.value );
                        res.value = string_new( buffer );
                    }
                } else if ( typedef_is_double( res.type ) ) {
                    codeblock_add( code, MN_DOUBLE2STR, 0 );
                    if ( res.constant ) {
                        char buffer[384];
                        sprintf( buffer, "%f", res.fvalue );
                        res.value = string_new( buffer );
                    }
                } else if ( typedef_is_float( res.type ) ) {
                    codeblock_add( code, MN_FLOAT2STR, 0 );
                    if ( res.constant ) {
                        char buffer[384];
                        sprintf( buffer, "%f", res.fvalue );
                        res.value = string_new( buffer );
                    }
                } else
                    compile_error( MSG_STRING_EXP );
                break;

            default:
                compile_error( MSG_INCOMP_TYPE );
        }
        res.type = typedef_new( t );
    }

    return res;
}

/* Static utility function for compile_bestproc */

static void strdelchars( char * str, char * chars ) {
    while ( *str ) {
        if ( strchr( chars, *str ) ) strcpy( str, str + 1 );
        else                         str++;
    }
}

/*
 *  FUNCTION : compile_bestproc
 *
 *  Compile a system call, given a list of system functions
 *  with the same name
 *
 *  PARAMS:
 *      procs           List of system functions
 *
 *  RETURN VALUE:
 *      Identifier code allocated for the function
 */

SYSPROC * compile_bestproc( SYSPROC ** procs ) {
    const char * proc_name = procs[0]->name;
    int n, proc_count = 0, count = 0, min_params = 0;
    char validtypes[32];
    BASETYPE type = TYPE_UNDEFINED;
    CODEBLOCK_POS code_pos_code;
    expresion_result res;
    tok_pos token_save;

    /* --------------------------------------------------- */

    while ( procs[proc_count] ) proc_count++;

    /* Get the minimum number of parameters */

    for ( n = 0; n < proc_count; n++ )
        if ( procs[n]->params > min_params )
            min_params = procs[n]->params;

    /* --------------------------------------------------- */

    /* Count function params */

    code_pos_code = codeblock_pos( code );
    token_save = token_pos();

    int params = 0;

    for (;;) {
        token_next();
        if ( token.type == IDENTIFIER && token.code == identifier_rightp ) { /* ")" */
            token_back();
            break;
        }
        token_back();

        params++;

        compile_expresion( 0, 0, 0, TYPE_UNDEFINED );

        token_next();
        if ( token.type != IDENTIFIER || token.code != identifier_comma ) { /* "," */
            token_back();
            break;
        }
    }

    codeblock_setpos( code, code_pos_code );
    token_set_pos( token_save );

    /* Eliminate any process that has not as many parameters */

    for ( n = 0; n < proc_count; n++ ) {
        char * p = procs[n]->paramtypes;
        int param_diff = 0;

        while( ( *p ) ) if ( *(p++) == '+' ) param_diff++;

        if ( procs[n]->params - param_diff != params ) {
            memmove( &procs[n], &procs[n+1], sizeof( SYSPROC* ) *( proc_count - n ) );
            proc_count--;
            n--;
        }
    }

    count = 0;

    for (;;) {
        token_next();
        if ( token.type == IDENTIFIER && token.code == identifier_rightp ) { /* ")" */
            token_back();
            break;
        }
        token_back();

        count++;

        /* Eliminate any process that has not as many parameters */
/*
        for ( n = 0; n < proc_count; n++ ) {
            if ( procs[n]->params < count ) {
                memmove( &procs[n], &procs[n+1], sizeof( SYSPROC* ) *( proc_count - n ) );
                proc_count--;
                n--;
            }
        }
*/
        if ( proc_count == 0 ) compile_error( MSG_INCORRECT_PARAMC, proc_name, min_params );

        /* Find all the available types */

        validtypes[0] = 0;
        for ( n = 0; n < proc_count; n++ ) {
            if ( !strchr( validtypes, procs[n]->paramtypes[count-1] ) ) {
                validtypes[strlen( validtypes )+1] = 0;
                validtypes[strlen( validtypes )] = procs[n]->paramtypes[count-1];
            }
        }

        if ( strlen( validtypes ) == 1 ) {

            /* Same type for any function variant */

            if ( validtypes[0] == 'V' ) {
                /* Function will receive a varspace struct */
                reduce_arrays = 0;
                res = compile_expresion( 0, 1, 0, TYPE_UNDEFINED );
                reduce_arrays = 1;
                /*
                while ( typedef_is_pointer( res.type ) ) {
                    codeblock_add( code, MN_PTR, 0 );
                    res.type = typedef_reduce( res.type );
                }
                */
                if ( typedef_is_struct( res.type ) ) {
                    int size = res.type.varspace->count * sizeof( DCB_TYPEDEF ), nvar;
                    segment_ensure_capacity(globaldata, size);
                    codeblock_add( code, MN_GLOBAL, globaldata->current );
                    for ( nvar = 0; nvar < res.type.varspace->count; nvar++ ) {
                        DCB_TYPEDEF dcbtype;
                        dcb_settype( &dcbtype, &res.type.varspace->vars[nvar].type );
                        memcpy(( uint8_t* )globaldata->bytes + globaldata->current, &dcbtype, sizeof( DCB_TYPEDEF ) );
                        globaldata->current += sizeof( DCB_TYPEDEF );
                    }
                    codeblock_add( code, MN_PUSH, res.type.varspace->count );
                    count += 2;
                } else {
                    DCB_TYPEDEF dcbtype;
                    dcb_settype( &dcbtype, &res.type );
                    segment_ensure_capacity(globaldata, sizeof( DCB_TYPEDEF ));
                    codeblock_add( code, MN_GLOBAL, globaldata->current );
                    memcpy(( uint8_t* )globaldata->bytes + globaldata->current, &dcbtype, sizeof( DCB_TYPEDEF ) );
                    globaldata->current += sizeof( DCB_TYPEDEF );
                    codeblock_add( code, MN_PUSH, 1 );
                    count += 2;
                }
            } else {
                switch ( validtypes[0] ) {
                    case 'I':
                        type = TYPE_QWORD;
                        break;
                    case 'i':
                        type = TYPE_DWORD;
                        break;
                    case 'W':
                        type = TYPE_WORD;
                        break;
                    case 'B':
                        type = TYPE_BYTE;
                        break;
                    case 'S':
                        type = TYPE_STRING;
                        break;
                    case 'P':
                        type = TYPE_POINTER;
                        break;
                    case 'F':
                        type = TYPE_FLOAT;
                        break;
                    case 'D':
                        type = TYPE_DOUBLE;
                        break;
                    default:
                        compile_error( MSG_INVALID_PARAMT );
                }

                res = compile_expresion( 0, 0, 0, type );
                if ( res.lvalue ) codeblock_add( code, mntype( res.type, 0 ) | MN_PTR, 0 );
            }
        } else {
            /* Different types availables */

            res = compile_expresion( 0, 0, 0, TYPE_UNDEFINED );
            if ( res.lvalue ) codeblock_add( code, mntype( res.type, 0 ) | MN_PTR, 0 );

            /* Eliminate any incompatible data type, but allow some
             * conversions if no exact match is available */

            switch ( typedef_base( res.type ) ) {
                case TYPE_QWORD:
                case TYPE_DWORD:
                case TYPE_WORD:
                case TYPE_BYTE:
                case TYPE_INT:
                case TYPE_INT32:
                case TYPE_SHORT:
                case TYPE_SBYTE:
                    strdelchars( validtypes, "SPDF" );
                    break;
                case TYPE_DOUBLE:
                case TYPE_FLOAT:
                    if ( strchr( validtypes, 'F' ) ||  strchr( validtypes, 'D' ) )  strdelchars( validtypes, "IiWBSP" );
                    else                                                            strdelchars( validtypes, "SP" );
                    break;
                case TYPE_STRING:
                    if ( strchr( validtypes, 'S' ) ) strdelchars( validtypes, "IiWBPDF" );
                    else                             strdelchars( validtypes, "P" );
                    break;
                case TYPE_POINTER:
                    strdelchars( validtypes, "IiWBSDF" );
                    break;
                default:
                    break;
            }

            if ( strlen( validtypes ) != 1 ) compile_error( MSG_INVALID_PARAMT );

            /* Eliminate all functions that are not selected */

            for ( n = 0; n < proc_count; n++ ) {
                if ( procs[n]->paramtypes[count-1] != validtypes[0] ) {
                    memmove( &procs[n], &procs[n+1], sizeof( SYSPROC* ) *( proc_count - n ) );
                    proc_count--;
                    n--;
                }
            }

            if ( strlen( validtypes ) > 1 ) continue;

            /* Convert the result to the appropiate type, if needed */

            switch ( validtypes[0] ) {
                case 'I':
                    type = TYPE_QWORD;
                    break;
                case 'i':
                    type = TYPE_DWORD;
                    break;
                case 'W':
                    type = TYPE_WORD;
                    break;
                case 'B':
                    type = TYPE_BYTE;
                    break;
                case 'S':
                    type = TYPE_STRING;
                    break;
                case 'P':
                    type = TYPE_POINTER;
                    break;
                case 'D':
                    type = TYPE_DOUBLE;
                    break;
                case 'F':
                    type = TYPE_FLOAT;
                    break;
                default:
                    compile_error( MSG_INVALID_PARAMT );
            }

            convert_result_type( res, type );
        }

        token_next();
        if ( token.type != IDENTIFIER || token.code != identifier_comma ) { /* "," */
            token_back();
            break;
        }
    }

    /* Eliminate any process that has too many parameters */

    for ( n = 0 ; n < proc_count ; n++ ) {
        if ( procs[n]->params != count ) {
            memmove( &procs[n], &procs[n+1], sizeof( SYSPROC* ) *( proc_count - n ) );
            proc_count--;
            n--;
        }
    }

    if ( proc_count > 1 ) compile_error( MSG_MULTIPLE_PROCS_FOUND, proc_name );
    if ( proc_count == 0 ) compile_error( MSG_INCORRECT_PARAMC, proc_name, min_params );
    codeblock_add( code, MN_SYSCALL, procs[0]->code );

    return procs[0];
}

/* Compiles a list of parameters */

int compile_paramlist( BASETYPE * types, const char * paramtypes ) {
    expresion_result res;
    int count = 0;

    for (;;) {
        BASETYPE type = types ? *types : TYPE_UNDEFINED;
        if ( paramtypes ) {
            switch ( *paramtypes++ ) {
                case 'I':
                    type = TYPE_QWORD;
                    break;
                case 'i':
                    type = TYPE_DWORD;
                    break;
                case 'W':
                    type = TYPE_WORD;
                    break;
                case 'B':
                    type = TYPE_BYTE;
                    break;
                case 'S':
                    type = TYPE_STRING;
                    break;
                case 'P':
                    type = TYPE_POINTER;
                    break;
                case 'D':
                    type = TYPE_DOUBLE;
                    break;
                case 'F':
                    type = TYPE_FLOAT;
                    break;
                default:
                    compile_error( MSG_INVALID_PARAMT );
            }
        }

        res = compile_expresion( 0, 0, 0, type );

        if ( types ) {
            if ( *types == TYPE_UNDEFINED ) *types = typedef_base( res.type );
            types++;
        }
        if ( res.lvalue ) codeblock_add( code, mntype( res.type, 0 ) | MN_PTR, 0 );
        count++;

        token_next();
        if ( token.type == IDENTIFIER && token.code == identifier_comma ) continue ; /* "," */
        token_back();
        break;
    }
    return count;
}

/*
 *  FUNCTION : compile_cast_cstr
 *
 *  Compiles a cast C string (null terminated string)
 *
 *  PARAMS :
 *      None
 *
 *  RETURN VALUE :
 *      Returns the expression result
 *
 */

expresion_result compile_cast_cstr() {
    expresion_result res = compile_value();

    if ( typedef_is_pointer( res.type ) && res.type.chunk[1].type == TYPE_CHAR ) {
        /* ( string ) <char pointer> */
        if ( res.lvalue ) {
            codeblock_add( code, mntype( res.type, 0 ) | MN_PTR, 0 );
            res.lvalue = 0;
        }
        codeblock_add( code, MN_A2STR, 0 );
        res.type = typedef_new( TYPE_STRING );
    }
    else
        compile_error( MSG_CONVERSION );

    return res;
}

/*
 *  FUNCTION : compile_cast
 *
 *  Compiles a cast operator(c-like type conversion:([type])value)
 *
 *  PARAMS :
 *      None
 *
 *  RETURN VALUE :
 *      Returns the expression result
 *
 */

expresion_result compile_cast() {
    TYPEDEF  type;
    BASETYPE basetype = TYPE_INT;
    int sign_prefix = 0;

    expresion_result res;

    token_next();

    /* Create the data type definition */

    /* "Sign" */

    if ( token.code == identifier_unsigned ) {
        sign_prefix = 1;
        token_next();
    } else if ( token.code == identifier_signed ) {
        sign_prefix = -1;
        token_next();
    }

    if ( identifier_is_basic_type(token.code) ) {
        type = typedef_basic_type_by_name( token.code, sign_prefix );
        if ( typedef_is_undefined( type ) ) compile_error( MSG_INVALID_TYPE );
        token_next();
    }
    else {
        /* If type is not a basic one: check for user-defined types */
        TYPEDEF * typep = typedef_by_name( token.code );
        if ( typep == NULL ) {
            compile_error( MSG_INVALID_TYPE );
            return res; /* Avoid scan-build warning */
        }
        type = *typep;
        token_next();
    }

    /* Check for pointers to defined types */

    while ( token.type == IDENTIFIER && ( token.code == identifier_pointer || token.code == identifier_multiply ) ) { /* "POINTER" or "*" */
        type = typedef_pointer( type );
        token_next();
    }

    /* Check for the right parent */

    if ( token.type != IDENTIFIER || token.code != identifier_rightp ) compile_error( MSG_EXPECTED, ")" ); /* ")" */

    /* I have the cast type, do the cast */

    res = compile_value();

    if ( typedef_is_equal( res.type, type ) ) return res;

    if ( typedef_is_pointer( type ) ) {
        /* Conversion of pointer to pointer of another type */
        if ( typedef_is_pointer( res.type ) || typedef_base( res.type ) == TYPE_INT || typedef_base( res.type ) == TYPE_QWORD ) {
            /* ( pointer ) <pointer|int64|qword> */
            res.type = type;
            return res;
        } else if ( typedef_is_integer( res.type ) ) {
            /* ( pointer ) <any integer value> */
            if ( res.lvalue ) {
                codeblock_add( code, mntype( res.type, 0 ) | MN_PTR, 0 );
                res.lvalue = 0;
            }
            res.type = type;
            return res;
        } else if ( typedef_is_double( res.type ) ) {
            /* ( pointer ) <double> */
            if ( res.lvalue ) {
                codeblock_add( code, mntype( res.type, 0 ) | MN_PTR, 0 );
                res.lvalue = 0;
            }
            codeblock_add( code, MN_DOUBLE2INT | mntype( type, 0 ), 0 );
            res.type = type;
            return res;
        } else if ( typedef_is_float( res.type ) ) {
            /* ( pointer ) <float> */
            if ( res.lvalue ) {
                codeblock_add( code, mntype( res.type, 0 ) | MN_PTR, 0 );
                res.lvalue = 0;
            }
            codeblock_add( code, MN_FLOAT2INT | mntype( type, 0 ), 0 );
            res.type = type;
            return res;
        }
        compile_error( MSG_PTR_CONVERSION );
    } else if ( typedef_is_double( type ) ) {
        /* Conversion of integer to float */
        if ( typedef_is_integer( res.type ) ) {
            /* ( double ) <any integer type> */
            if ( res.lvalue ) {
                codeblock_add( code, mntype( res.type, 0 ) | MN_PTR, 0 );
                res.lvalue = 0;
            }
            codeblock_add( code, MN_INT2DOUBLE | mntype( res.type, 0 ), 0 );
            res.type = type;
        } else if ( typedef_is_float( res.type ) ) {
            /* ( pointer ) <float> */
            if ( res.lvalue ) {
                codeblock_add( code, mntype( res.type, 0 ) | MN_PTR, 0 );
                res.lvalue = 0;
            }
            codeblock_add( code, MN_FLOAT2DOUBLE, 0 );
            res.type = type;
            return res;
        } else {
            compile_error( MSG_CONVERSION );
        }
    } else if ( typedef_is_float( type ) ) {
        /* Conversion of integer to float */
        if ( typedef_is_integer( res.type ) ) {
            /* ( float ) <any integer type> */
            if ( res.lvalue ) {
                codeblock_add( code, mntype( res.type, 0 ) | MN_PTR, 0 );
                res.lvalue = 0;
            }
            codeblock_add( code, MN_INT2FLOAT | mntype( res.type, 0 ), 0 );
            res.type = type;
        } else if ( typedef_is_double( res.type ) ) {
            /* ( pointer ) <double> */
            if ( res.lvalue ) {
                codeblock_add( code, mntype( res.type, 0 ) | MN_PTR, 0 );
                res.lvalue = 0;
            }
            codeblock_add( code, MN_DOUBLE2FLOAT, 0 );
            res.type = type;
            return res;
        } else {
            compile_error( MSG_CONVERSION );
        }
    } else if ( typedef_base( type ) == TYPE_CHAR ) {
        if ( typedef_is_string( res.type ) ) {
            /* ( char ) <string> */
            if ( res.lvalue ) {
                codeblock_add( code, MN_PTR | MN_STRING, 0 );
                res.lvalue = 0;
            }
            /* codeblock_add(code, MN_STR2INT, 0);
            res.type = typedef_new(TYPE_INT); */
            codeblock_add( code, MN_STR2CHR, 0 );
            res.type = typedef_new( TYPE_CHAR );
        } else if ( typedef_is_array( res.type ) && res.type.chunk[1].type == TYPE_CHAR ) {
            /* ( char ) <char[]> */
            codeblock_add( code, MN_A2STR, 0 );
            codeblock_add( code, MN_STR2CHR, 0 );
            res.type = typedef_new( TYPE_CHAR );
            res.count = 0;
            res.lvalue = 0;
        }

        if ( res.constant ) {
            res.fvalue = res.value = ( uint8_t ) res.value;
        }
//        else {
//            compile_error( MSG_CONVERSION );
//        }
    } else if ( typedef_is_integer( type ) ) {
        /* Conversion of float, string or integer to integer */
        if ( typedef_is_double( res.type ) ) {
            /* ( any integer ) <double> */
            if ( res.lvalue ) {
                codeblock_add( code, MN_PTR | MN_DOUBLE, 0 );
                res.lvalue = 0;
            }
            codeblock_add( code, MN_DOUBLE2INT | mntype( type, 0 ), 0 );
            res.type = typedef_new( TYPE_INT );
        } else if ( typedef_is_float( res.type ) ) {
            /* ( any integer ) <float> */
            if ( res.lvalue ) {
                codeblock_add( code, MN_PTR | MN_FLOAT, 0 );
                res.lvalue = 0;
            }
            codeblock_add( code, MN_FLOAT2INT | mntype( type, 0 ), 0 );
            res.type = typedef_new( TYPE_INT );
        } else if ( typedef_is_string( res.type ) ) {
            /* ( any integer ) <string> */
            if ( res.lvalue ) {
                codeblock_add( code, MN_PTR | MN_STRING, 0 );
                res.lvalue = 0;
            }
            codeblock_add( code, MN_STR2INT, 0 );
            res.type = typedef_new( TYPE_INT );
        } else if ( typedef_is_array( res.type ) && res.type.chunk[1].type == TYPE_CHAR ) {
            /* ( any integer ) <char[]> */
            codeblock_add( code, MN_A2STR, 0 );
            codeblock_add( code, MN_STR2INT, 0 );
            res.type = typedef_new( TYPE_INT );
            res.count = 0;
            res.lvalue = 0;
        } else if ( typedef_is_integer( res.type ) || typedef_is_pointer( res.type ) || typedef_base( res.type ) == TYPE_CHAR ) {
            /* ( any integer ) <any integer|pointer|char> */
            if ( res.lvalue ) {
                codeblock_add( code,  mntype( res.type, 0 ) | MN_PTR, 0 );
                res.lvalue = 0;
            }
            if ( res.constant ) {
                switch( typedef_base( type ) ) {
                    case TYPE_QWORD:
                        res.fvalue = res.value = ( uint64_t ) res.value;
                        break;

                    case TYPE_INT:
                        res.fvalue = res.value = ( int64_t ) res.value;
                        break;

                    case TYPE_DWORD:
                        res.fvalue = res.value = ( uint32_t ) res.value;
                        break;

                    case TYPE_INT32:
                        res.fvalue = res.value = ( int32_t ) res.value;
                        break;

                    case TYPE_WORD:
                        res.fvalue = res.value = ( uint16_t ) res.value;
                        break;

                    case TYPE_SHORT:
                        res.fvalue = res.value = ( int16_t ) res.value;
                        break;

                    case TYPE_BYTE:
                    case TYPE_CHAR:
                        res.fvalue = res.value = ( uint8_t ) res.value;
                        break;

                    case TYPE_SBYTE:
                        res.fvalue = res.value = ( int8_t ) res.value;
                        break;

                    // remove compiler warning
                    default:
                        break;
                }
            }
        } else {
            compile_error( MSG_CONVERSION );
        }

        if ( typedef_is_integer( res.type ) ) {
            switch ( type.chunk[0].type ) {
                case    TYPE_BYTE:
                case    TYPE_CHAR:
                        res.fvalue = res.value = ( uint8_t ) res.value;
                        codeblock_add( code, MN_INT2BYTE | MN_UNSIGNED, 0 );
                        res.type = type;
                        break;

                case    TYPE_SBYTE:
                        res.fvalue = res.value = ( int8_t ) res.value;
                        codeblock_add( code, MN_INT2BYTE, 0 );
                        res.type = type;
                        break;

                case    TYPE_WORD:
                        res.fvalue = res.value = ( uint16_t ) res.value;
                        codeblock_add( code, MN_INT2WORD | MN_UNSIGNED, 0 );
                        res.type = type;
                        break;

                case    TYPE_SHORT:
                        res.fvalue = res.value = ( int16_t ) res.value;
                        codeblock_add( code, MN_INT2WORD, 0 );
                        res.type = type;
                        break;

                case    TYPE_DWORD:
                        res.fvalue = res.value = ( uint32_t ) res.value;
                        codeblock_add( code, MN_INT2DWORD | MN_UNSIGNED, 0 );
                        res.type = type;
                        break;

                case    TYPE_INT32:
                        res.fvalue = res.value = ( int32_t ) res.value;
                        codeblock_add( code, MN_INT2DWORD, 0 );
                        res.type = type;
                        break;

                case    TYPE_INT:
                case    TYPE_QWORD:
                case    TYPE_POINTER:
                        res.type = type;
                        break;

                case    TYPE_UNDEFINED:
                case    TYPE_FLOAT:
                case    TYPE_DOUBLE:
                case    TYPE_STRING:
                case    TYPE_ARRAY:
                case    TYPE_STRUCT:
                        break;
            }
        }
    } else if ( typedef_is_string( type ) ) {
        /* Conversion of pointer, float, integer, or fixed-width string to a string */
        /* ( string ) <char[]> */
        if ( typedef_is_array( res.type ) && res.type.chunk[1].type == TYPE_CHAR ) {
            codeblock_add( code, MN_A2STR, 0 );
            res.type = typedef_new( TYPE_STRING );
            res.lvalue = 0;
        } else if ( typedef_is_pointer( res.type ) && res.type.chunk[1].type == TYPE_CHAR ) {
            /* ( string ) <char pointer> */
            if ( res.lvalue ) {
                codeblock_add( code, mntype( res.type, 0 ) | MN_PTR, 0 );
                res.lvalue = 0;
            }
            codeblock_add( code, MN_A2STR, 0 );
            res.type = typedef_new( TYPE_STRING );
        } else if ( res.type.chunk[0].type == TYPE_CHAR ) {
            /* ( string ) <char> */
            if ( res.lvalue ) {
                codeblock_add( code, mntype( res.type, 0 ) | MN_PTR, 0 );
                res.lvalue = 0;
            }
            codeblock_add( code, MN_CHR2STR, 0 );
            res.type = typedef_new( TYPE_STRING );
        } else if ( typedef_is_integer( res.type ) ) {
            /* ( string ) <any int> */
            if ( res.lvalue ) {
                codeblock_add( code, mntype( res.type, 0 ) | MN_PTR, 0 );
                res.lvalue = 0;
            }
            codeblock_add( code, MN_INT2STR | mntype( res.type, 0 ), 0 );
            res.type = typedef_new( TYPE_STRING );
        } else if ( typedef_is_float( res.type ) ) {
            /* ( string ) <float> */
            if ( res.lvalue ) {
                codeblock_add( code, MN_PTR, 0 );
                res.lvalue = 0;
            }
            codeblock_add( code, MN_FLOAT2STR, 0 );
            res.type = typedef_new( TYPE_STRING );
        } else if ( typedef_is_double( res.type ) ) {
            /* ( string ) <float> */
            if ( res.lvalue ) {
                codeblock_add( code, MN_PTR, 0 );
                res.lvalue = 0;
            }
            codeblock_add( code, MN_DOUBLE2STR, 0 );
            res.type = typedef_new( TYPE_STRING );
        }
        else
            compile_error( MSG_CONVERSION );
    } else if ( typedef_is_struct( type ) ) {
        res.type = type;
    } else {
        compile_error( MSG_CONVERSION );
    }

    return res;
}

/* Compiles a value (the smallest element of the language) */

/* Step 15 */
expresion_result compile_value() {
    CONSTANT * c;
    int64_t id;

    expresion_result res;

    token_next();

    /* ( ... ) */

    if ( token.type == IDENTIFIER ) {
        if ( token.code == identifier_dollar ) {
            /* Compile cast null terminated string */
            return compile_cast_cstr();
        }

        if ( token.code == identifier_leftp ) { /* "(" */
            /* Check for cast-type expressions */

            token_next();
            if ( token.type == IDENTIFIER && identifier_is_type( token.code ) ) {
                token_back();
                return compile_cast();
            }
            token_back();

            res = compile_subexpresion();
            token_next();
            if ( token.type != IDENTIFIER || token.code != identifier_rightp ) compile_error( MSG_EXPECTED, ")" ) ; /* ")" */
            return res;
        }

        if ( token.code == identifier_type ) { /* "TYPE" */
            token_next( );
            /* "TYPE mouse" */
            if ( token.type == IDENTIFIER && token.code == identifier_mouse ) { /* "MOUSE" - Hack */
                codeblock_add( code, MN_PUSH, -1 );
                res.value      = -1;
                res.fvalue     = ( double ) res.value;
                res.lvalue     = 0;
                res.constant   = 1;
                res.asignation = 0;
                res.call       = 0;
                res.type       = typedef_new( TYPE_INT );
                return res;
            }

            if ( token.type != IDENTIFIER || token.code < reserved_words ) compile_error( MSG_PROCESS_NAME_EXP );

            codeblock_add( code, MN_TYPE, token.code );
            res.fvalue     = 0.0;
            res.value      = 0;
            res.lvalue     = 0;
            res.constant   = 0;
            res.asignation = 0;
            res.call       = 0;
            res.type       = typedef_new( TYPE_INT );
            return res;
        }

        if ( token.code == identifier_offset || token.code == identifier_bandoffset ) { /* "OFFSET" or "&" */
            res = compile_factor() ; /* To allow &a.b */
            if ( !res.lvalue ) compile_error( MSG_NOT_AN_LVALUE );

            res.lvalue = 0;
            res.type   = typedef_pointer( res.type );
            return res;
        }

        if ( token.code == identifier_leftb ) { /* "[" */  /* POINTER */
            res = compile_subexpresion();
            if ( !typedef_is_pointer( res.type ) ) compile_error( MSG_NOT_A_POINTER );

            if ( res.lvalue ) codeblock_add( code, mntype( res.type, 0 ) | MN_PTR, 0 );

            res.type = typedef_reduce( res.type );
            token_next();

            if ( token.type != IDENTIFIER || token.code != identifier_rightb ) compile_error( MSG_EXPECTED, "]" ) ; /* "]" */

            res.lvalue = 1;
            return res;
        }

        if ( token.code == identifier_multiply ) { /* "*" */
            res = compile_factor() ; /* To accept *ptr++ */
            if ( !typedef_is_pointer( res.type ) ) compile_error( MSG_NOT_A_POINTER );

            if ( res.lvalue ) codeblock_add( code, mntype( res.type, 0 ) | MN_PTR, 0 );

            res.type = typedef_reduce( res.type );
            res.lvalue = 1;
            return res;
        }

        /* SIZEOF */

        if ( token.code == identifier_sizeof ) { /* "SIZEOF" */
            token_next();
            if ( token.type != IDENTIFIER || token.code != identifier_leftp ) compile_error( MSG_EXPECTED, "(" ) ; /* "(" */

            res.value      = compile_sizeof( 0, NULL, NULL, NULL );
            res.fvalue     = ( double ) res.value;
            res.lvalue     = 0;
            res.constant   = 1;
            res.asignation = 0;
            res.call       = 0;
            res.type       = typedef_new( TYPE_QWORD );
            codeblock_add( code, MN_PUSH, res.value );
            token_next();

            if ( token.type != IDENTIFIER || token.code != identifier_rightp ) compile_error( MSG_EXPECTED, ")" ) ; /* ")" */

            return res;
        }

        /* "+2" (Positive) */
        if ( token.type == IDENTIFIER && token.code == identifier_plus ) token_next() ; /* "+" */

        if ( token.type == IDENTIFIER ) {
            if ( token.code == identifier_minus ) { /* "-" */ /* "-2" (Negative) */
                expresion_result part = compile_value();
                if ( part.lvalue ) codeblock_add( code, mntype( part.type, 0 ) | MN_PTR, 0 );
                BASETYPE t = mntype( part.type, 0 );
                codeblock_add( code, ( ( t == MN_DOUBLE || t == MN_FLOAT ) ? t : 0 ) | MN_NEG, 0 );
                res.type = part.type;
                if ( typedef_is_numeric( part.type ) ) {
                    res.constant = part.constant;
                    res.value    = -part.value;
                    if ( typedef_is_unsigned( res.type ) ) {
                        res.fvalue   = ( uint64_t ) -part.value;
                    } else {
                        res.fvalue   = -part.fvalue;
                    }
                    return res;
                }
                compile_error( MSG_NUMBER_REQUIRED );
            }
        }
    }

    switch ( token.type ) {
        case    NUMBER:         /* Numbers */
            codeblock_add( code, MN_PUSH, token.code );
            res.lvalue     = 0;
            res.asignation = 0;
            res.constant   = 1;
            res.call       = 0;
            res.value      = token.code;
            res.fvalue     = ( double ) res.value;
            res.type       = typedef_new( TYPE_INT );
            return res;

        case    FLOAT:
            codeblock_add( code, MN_PUSH, *( int64_t * )&token.value );
            res.lvalue     = 0;
            res.asignation = 0;
            res.constant   = 1;
            res.call       = 0;
            res.value      = token.code;
            res.fvalue     = token.value;
            res.type       = typedef_new( TYPE_DOUBLE );
            return res;

        case    STRING:         /* Strings */
            codeblock_add( code, MN_PUSH | MN_STRING, token.code );
            res.lvalue     = 0;
            res.asignation = 0;
            res.constant   = 1;
            res.call       = 0;
            res.value      = token.code;
            res.fvalue     = ( double ) res.value;
            res.type       = typedef_new( TYPE_STRING );
            return res;

        case    IDENTIFIER:     /* Constants */
            c = constants_search( token.code );
            if ( c ) {
                if ( typedef_is_string( c->type ) ) codeblock_add( code, MN_PUSH | MN_STRING, c->value );
                else                                codeblock_add( code, MN_PUSH, c->value );
                res.lvalue     = 0;
                res.asignation = 0;
                res.constant   = 1;
                res.call       = 0;
                res.value      = c->value;
                res.fvalue     = *( double * ) &c->value;
                res.type       = c->type;
                return res;
            }
            break;

        default:
            compile_error( MSG_UNKNOWN_IDENTIFIER );
            break;
    }

    /* Call to a system procedure or function */

    id = token.code;

    token_next();

    if ( token.type == IDENTIFIER && token.code == identifier_leftp ) { /* "(" */
        SYSPROC ** sysproc_list = sysproc_getall( id );
        int param_count;

        if ( sysproc_list ) {
            if ( !code ) {
                token_back();
                compile_error( MSG_INVALID_INITIALIZER );
            }
            SYSPROC * sysproc = compile_bestproc( sysproc_list );
            free( sysproc_list );

            token_next();
            if ( token.type != IDENTIFIER || token.code != identifier_rightp ) compile_error( MSG_EXPECTED, ")" ) ; /* ")" */

            res.fvalue     = 0.0;
            res.lvalue     = 0;
            res.asignation = 0;
            res.constant   = 0;
            res.call       = 1;
            res.value      = 0;
            res.type       = typedef_new( sysproc->type );
            return res;
        }

        /* Calls a user-defined procedure */

        PROCDEF * cproc = procdef_search( id );
        if ( !cproc ) {
            if ( autodeclare ) {
                cproc = procdef_new( procdef_getid(), id );
            } else {
                token_back();
                compile_error( MSG_UNDEFINED_PROC );
                return res; /* Avoid scan-build warning */
            }
        }

        token_next();

        if ( token.type != IDENTIFIER || token.code != identifier_rightp ) { /* ")" */
            token_back();
            param_count = compile_paramlist( cproc->paramtype, 0 );
            token_next();
        } else {
            param_count = 0;
        }

        if ( token.type != IDENTIFIER || token.code != identifier_rightp ) compile_error( MSG_EXPECTED, ")" ) ; /* ")" */

        if ( cproc->params == -1 ) {
            cproc->params = param_count;
        } else if ( cproc->params != param_count ) {
            if ( cproc->minparams == -1 || cproc->minparams > param_count || cproc->params < param_count ) {
                compile_error( MSG_INCORRECT_PARAMC, identifier_name( cproc->identifier ), cproc->minparams == -1 ? cproc->params : cproc->minparams );
            }
            for ( int i = param_count; i < cproc->params; i++ ) {
                if ( cproc->paramtype[i] == TYPE_STRING ) {
                    codeblock_add( code, MN_PUSH | MN_STRING, cproc->paramivalue[i] );
                }
                else
                if ( cproc->paramtype[i] == TYPE_FLOAT ) {
                    float f = (float) cproc->paramfvalue[i];
                    codeblock_add( code, MN_PUSH, *(int32_t *)&f );
                }
                else
                {
                    codeblock_add( code, MN_PUSH, cproc->paramivalue[i] );
                }
            }
        }

        codeblock_add( code, MN_CALL, id );
        res.fvalue     = 0.0;
        res.lvalue     = 0;
        res.asignation = 0;
        res.constant   = 0;
        res.call       = 1;
        res.value      = 0;
        res.type       = cproc->type;
        return res;
    }

    token_back();

    /* Assignable value */

    return compile_sublvalue( 0, 0, NULL );

}

static int compile_factor_recursion_level = 0;

/* Step 14 */
expresion_result compile_factor() {
    expresion_result res, part;
    BASETYPE t;

    compile_factor_recursion_level++;

    if ( compile_factor_recursion_level > 2048 ) compile_error( MSG_TOO_COMPLEX );


    token_next();

    res.value       = 0;
    res.fvalue      = 0;
    res.lvalue      = 0;
    res.call        = 0;
    res.constant    = 0;
    res.asignation  = 0;

    /* "+2" (Positive) */
    if ( token.type == IDENTIFIER && token.code == identifier_plus ) token_next() ; /* "+" */

    if ( token.type == IDENTIFIER ) {
        if ( token.code == identifier_minus ) { /* "-" */ /* "-2" (Negative) */
            part = compile_factor();
            if ( part.lvalue ) codeblock_add( code, mntype( part.type, 0 ) | MN_PTR, 0 );
            codeblock_add( code, ( ( ( t = mntype( part.type, 0 ) ) == MN_DOUBLE || t == MN_FLOAT ) ? t : 0 ) | MN_NEG, 0 );
            res.type = part.type;
            if ( typedef_is_numeric( part.type ) ) {
                res.constant = part.constant;
                res.value    = -part.value;
                if ( typedef_is_unsigned( res.type ) ) {
                    res.fvalue   = ( uint64_t ) -part.value;
                } else {
                    res.fvalue   = -part.fvalue;
                }
                compile_factor_recursion_level--;
                return res;
            }
            compile_error( MSG_NUMBER_REQUIRED );
        } else if ( token.code == identifier_not ) { /* "NOT" or "!" */
            part = compile_factor();
            if ( part.lvalue ) codeblock_add( code, mntype( part.type, 0 ) | MN_PTR, 0 );
            if ( typedef_is_pointer( part.type ) ) {
                /* codeblock_add (code, mntype(part.type, 0) | MN_POINTER2BOL, 0) ; */
                part.type = typedef_new( TYPE_QWORD );
            }
            codeblock_add( code, ( ( ( t = mntype( part.type, 0 ) ) == MN_DOUBLE || t == MN_FLOAT ) ? t : 0 ) | MN_NOT, 0 );
            if ( typedef_is_numeric( part.type ) || typedef_is_pointer( part.type ) ) {
                res.constant = part.constant;
                res.value    = !part.value;
                res.fvalue   = ( double )!part.fvalue;
                res.type     = part.type;
                compile_factor_recursion_level--;
                return res;
            }
            compile_error( MSG_NUMBER_REQUIRED );
            return res;
        } else if ( token.code == identifier_bnot ) { /* "BNOT" or "~" */
            part = compile_factor();
            if ( part.lvalue ) codeblock_add( code, mntype( part.type, 0 ) | MN_PTR, 0 );
            if ( typedef_is_pointer( part.type ) ) {
                /* codeblock_add (code, mntype(part.type, 0) | MN_POINTER2BOL, 0) ; */
                part.type = typedef_new( TYPE_QWORD );
            }

            if ( ( t = mntype( part.type, 0 ) ) == MN_DOUBLE || t == MN_FLOAT ) compile_error( MSG_INTEGER_REQUIRED );

            codeblock_add( code, mntype( part.type, 0 ) | MN_BNOT, 0 );
            if ( typedef_is_numeric( part.type ) ) {
                res.constant = part.constant;
                res.value    = ~part.value;
                res.type     = part.type /*typedef_new( TYPE_INT )*/;
                compile_factor_recursion_level--;
                return res;
            }
            compile_error( MSG_NUMBER_REQUIRED );
            return res;
        } else if ( token.type == IDENTIFIER && token.code == identifier_plusplus ) { /* "++" */
            part = compile_factor();
            if ( !part.lvalue ) compile_error( MSG_VARIABLE_REQUIRED );
            if ( typedef_is_string( part.type ) ) compile_error( MSG_INCOMP_TYPE );
            if ( typedef_is_pointer( part.type ) ) codeblock_add( code, MN_INC, typedef_size( typedef_reduce( part.type ) ) );
            else                                   codeblock_add( code, mntype( part.type, 0 ) | MN_INC, 1 );
            res.asignation = 1;
            res.lvalue = 1;
            res.type = part.type;
            compile_factor_recursion_level--;
            return res;
        } else if ( token.type == IDENTIFIER && token.code == identifier_minusminus ) { /* "--" */
            part = compile_factor();
            if ( !part.lvalue ) compile_error( MSG_VARIABLE_REQUIRED );
            if ( typedef_is_string( part.type ) ) compile_error( MSG_INCOMP_TYPE );
            if ( typedef_is_pointer( part.type ) ) codeblock_add( code, MN_DEC, typedef_size( typedef_reduce( part.type ) ) );
            else                                   codeblock_add( code, mntype( part.type, 0 ) | MN_DEC, 1 );
            res.asignation = 1;
            res.lvalue = 1;
            res.type = part.type;
            compile_factor_recursion_level--;
            return res;
        }
    }
    token_back();

    part = compile_value();

    /* Suffixes (operators ., [], etc) */

    for (;;) {
        token_next();

        if ( token.type == IDENTIFIER ) {
            if ( token.code == identifier_point ) { /* "." */ /* Operator "." */
                if ( typedef_is_array( part.type ) ) {
                    while ( typedef_is_array ( part.type = typedef_reduce( part.type ) ) );
                }

                if ( typedef_is_pointer( part.type ) ) {
                    part.type = typedef_reduce( part.type );
                    if ( !typedef_is_struct( part.type ) ) compile_error( MSG_STRUCT_REQUIRED );
                    codeblock_add( code, MN_PTR, 0 );
                }

                if ( typedef_is_struct( part.type ) ) {
                    VARSPACE * v = typedef_members( part.type );
                    if ( !v->vars ) {
                        compile_error( MSG_STRUCT_REQUIRED );
                        compile_factor_recursion_level--;
                        return res; /* Avoid scan-build warning */
                    }
                    part = compile_sublvalue( v, v->vars[0].offset, NULL );
                } else {
                    VARSPACE * v = typedef_members( part.type );
                    if ( typedef_base( part.type ) != TYPE_QWORD && typedef_base( part.type ) != TYPE_INT ) compile_error( MSG_INTEGER_REQUIRED );
                    if ( part.lvalue ) codeblock_add( code, mntype( part.type, 0 ) | MN_PTR, 0 );

                    part = compile_sublvalue( &local, 0, v ) ; /* REMOTE referenced by process */
                }
                continue;
            } else if ( token.code == identifier_plusplus ) { /* Postfix ++ operator */
                if ( !part.lvalue ) compile_error( MSG_VARIABLE_REQUIRED );
                if ( typedef_is_string( part.type ) ) compile_error( MSG_INCOMP_TYPE );
                if ( typedef_is_pointer( part.type ) ) codeblock_add( code, MN_POSTINC, typedef_size( typedef_reduce( part.type ) ) );
                else                                   codeblock_add( code, mntype( part.type, 0 ) | MN_POSTINC, 1 );
                part.asignation = 1;
                part.lvalue = 0;
                continue;
            } else if ( token.code == identifier_minusminus ) { /* Postfix -- operator */
                if ( !part.lvalue ) compile_error( MSG_VARIABLE_REQUIRED );
                if ( typedef_is_string( part.type ) ) compile_error( MSG_INCOMP_TYPE );
                if ( typedef_is_pointer( part.type ) ) codeblock_add( code, MN_POSTDEC, typedef_size( typedef_reduce( part.type ) ) );
                else                                   codeblock_add( code, mntype( part.type, 0 ) | MN_POSTDEC, 1 );
                part.asignation = 1;
                part.lvalue = 0;
                continue;
            }
        }

        /* Indexed via [...] */

        if ( token.type == IDENTIFIER && token.code == identifier_leftb ) { /* "[" */
            if ( typedef_is_pointer( part.type ) ) { /* Of pointers */
                if ( part.lvalue ) codeblock_add( code, mntype( part.type, 0 ) | MN_PTR, 0 );
                part.type = typedef_reduce( part.type );

                res = compile_subexpresion();
                if ( !typedef_is_integer( res.type ) ) compile_error( MSG_INTEGER_REQUIRED );
                if ( res.lvalue ) codeblock_add( code, mntype( res.type, 0 ) | MN_PTR, 0 );

                codeblock_add( code, MN_ARRAY, typedef_size( part.type ) );
                part.lvalue = 1;

                token_next();

                if ( token.type != IDENTIFIER || token.code != identifier_rightb ) compile_error( MSG_EXPECTED, "]" ) ; /* "]" */
                continue;
            }

            if ( typedef_is_string( part.type ) ) { /* Of strings */
                if ( part.lvalue ) codeblock_add( code, MN_STRING | MN_PTR, 0 );

                res = compile_subexpresion();
                if ( !typedef_is_integer( res.type ) ) compile_error( MSG_INTEGER_REQUIRED );
                if ( res.lvalue ) codeblock_add( code, mntype( res.type, 0 ) | MN_PTR, 0 );

                codeblock_add( code, MN_STRI2CHR, 0 );

                token_next();
                if ( token.type != IDENTIFIER || token.code != identifier_rightb ) compile_error( MSG_EXPECTED, "]" ) ; /* "]" */
/*                part.type   = typedef_new( TYPE_STRING ) ; */
                part.type   = typedef_new( TYPE_CHAR );
                part.lvalue = 0;
            }
            continue;
        }

        break;
    }

    token_back();

    compile_factor_recursion_level--;

    return part;
}

/* Step 13 */
expresion_result compile_operand() {
    expresion_result left = compile_factor(), right, res;
    int64_t op, t;

    for (;;) {
        token_next();

        if ( token.type == IDENTIFIER && token.code == identifier_multiply ) { /* "*" */
            if ( left.lvalue ) codeblock_add( code, mntype( left.type, 0 ) | MN_PTR, 0 );

            right = compile_factor();
            if ( right.lvalue ) codeblock_add( code, mntype( right.type, 0 ) | MN_PTR, 0 );

            t = check_numeric_types( &left, &right );
            codeblock_add( code, MN_MUL | t, 0 );

            res.constant = ( right.constant && left.constant );

            if ( t == MN_DOUBLE || t == MN_FLOAT ) {
                res.value  = 0.0;
                res.type   = typedef_new( ( t == MN_DOUBLE ) ? TYPE_DOUBLE : TYPE_FLOAT );
                res.fvalue = left.fvalue * right.fvalue;
            } else {
                res.type   = typedef_new( TYPE_INT );
                res.value  = left.value * right.value;
                res.fvalue = 0.0;
            }

            res.lvalue     = 0;
            res.asignation = 0;
            res.call       = 0;

            left = res;
            continue;
        } else if ( token.type == IDENTIFIER && ( token.code == identifier_divide || token.code == identifier_mod ) ) { /* "/" or "%" */
            op = token.code == identifier_mod ? MN_MOD : MN_DIV;
//            if ( op == MN_MOD && ( typedef_base( left.type ) == TYPE_DOUBLE || typedef_base( left.type ) == TYPE_FLOAT ) ) compile_error( MSG_INTEGER_REQUIRED );

            if ( left.lvalue ) codeblock_add( code, mntype( left.type, 0 ) | MN_PTR, 0 );

            right = compile_factor();
//            if ( op == MN_MOD && ( typedef_base( right.type ) == TYPE_DOUBLE || typedef_base( right.type ) == TYPE_FLOAT ) ) compile_error( MSG_INTEGER_REQUIRED );
            if ( right.lvalue ) codeblock_add( code, mntype( right.type, 0 ) | MN_PTR, 0 );

            t = check_numeric_types( &left, &right );
            codeblock_add( code, op | t, 0 );
            res.constant   = ( right.constant && left.constant );
            res.value      = 0;
            res.fvalue     = 0.0;
            res.lvalue     = 0;
            res.asignation = 0;
            res.call       = 0;
            switch ( t ) {
                case    MN_FLOAT:
                    res.type = typedef_new( TYPE_FLOAT );
                    break;

                case    MN_DOUBLE:
                    res.type = typedef_new( TYPE_DOUBLE );
                    break;

                case    MN_QWORD:
                    res.type = typedef_new( TYPE_INT );
                    break;

                case    MN_DWORD:
                    res.type = typedef_new( TYPE_INT32 );
                    break;

                case    MN_WORD:
                    res.type = typedef_new( TYPE_WORD );
                    break;

                case    MN_BYTE:
                    res.type = typedef_new( TYPE_BYTE );
                    break;
            }

            if ( res.constant ) {
                if ( t == MN_DOUBLE ) {
                    if ( op == MN_MOD ) compile_error( MSG_NUMBER_REQUIRED );
                    res.fvalue = left.fvalue / right.fvalue;
                    res.type = typedef_new( TYPE_DOUBLE );
                    res.value = 0;
                } else if ( t == MN_FLOAT ) {
                    if ( op == MN_MOD ) compile_error( MSG_NUMBER_REQUIRED );
                    res.fvalue = left.fvalue / right.fvalue;
                    res.type = typedef_new( TYPE_FLOAT );
                    res.value = 0;
                } else {
                    if ( right.value == 0 ) { compile_error( MSG_DIVIDE_BY_ZERO ); return left; /* Return avoid scan-build warning */ }
                    res.value = op == MN_MOD ? left.value % right.value : left.value / right.value;
                    res.type = typedef_new( TYPE_INT );
                    res.fvalue = 0.0;
                }
            }

            left = res;
            continue;
        }
        token_back();
        break;
    }

    return left;
}

/* Step 12 */
expresion_result compile_operation() {
    expresion_result left = compile_operand(), right, res;
    int64_t op, t;

    for (;;) {
        token_next();

        /* Integer addition (or subtraction) to a pointer */

        if ( typedef_is_pointer( left.type ) && token.type == IDENTIFIER && ( token.code == identifier_plus || token.code == identifier_minus ) ) { /* "+" o "-" */
            TYPEDEF ptr_t = typedef_reduce( left.type );

            op = token.code == identifier_plus ? MN_ADD : MN_SUB;
            if ( left.lvalue ) codeblock_add( code, mntype( left.type, 0 ) | MN_PTR, 0 );

            right = compile_operand();
            if ( right.lvalue ) codeblock_add( code, mntype( right.type, 0 ) | MN_PTR, 0 );

            if ( !typedef_is_integer( right.type ) && !typedef_is_pointer( right.type )) compile_error( MSG_INCOMP_TYPES );

            if ( typedef_size( ptr_t ) > 1 ) codeblock_add( code, MN_ARRAY, ( op == MN_ADD ? 1 : -1 ) * typedef_size( ptr_t ) );
            else                             codeblock_add( code, op, 0 );

            res.value      = 0;
            res.fvalue     = 0.0;
            res.constant   = 0;
            res.lvalue     = 0;
            res.asignation = 0;
            res.call       = 0;
            res.type       = left.type;

            left = res;
            continue;
        }

        /* String concatenation */

        if ( typedef_is_array( left.type ) && left.lvalue && token.type == IDENTIFIER && token.code == identifier_plus && left.type.chunk[1].type == TYPE_CHAR ) { /* "+" */
            codeblock_add( code, MN_A2STR, 0 );
            left.lvalue = 0;
            left.type = typedef_new( TYPE_STRING );
        }

        /* Addition/subtraction of numeric values */

        if ( token.type == IDENTIFIER && ( token.code == identifier_plus || token.code == identifier_minus ) ) { /* "+" or "-" */
            op = ( token.code == identifier_plus ) ? MN_ADD : MN_SUB;
            if ( left.lvalue ) codeblock_add( code, mntype( left.type, 0 ) | MN_PTR, 0 );
            right = compile_operand();

            /* String concatenation */

            if (( typedef_is_string( left.type ) || typedef_is_string( right.type ) ) && op == MN_ADD ) {
                if ( typedef_is_array( right.type ) && right.lvalue && right.type.chunk[1].type == TYPE_CHAR ) {
                    codeblock_add( code, MN_A2STR, 0 );
                    right.type = typedef_new( TYPE_STRING );
                    right.lvalue = 0;
                }
                if ( right.lvalue ) codeblock_add( code, mntype( right.type, 0 ) | MN_PTR, 0 );

                     if ( typedef_is_integer( right.type ) )        codeblock_add( code, MN_INT2STR | mntype( right.type, 0 ), 0 );
                else if ( typedef_is_double( right.type ) )         codeblock_add( code, MN_DOUBLE2STR, 0 );
                else if ( typedef_is_float( right.type ) )          codeblock_add( code, MN_FLOAT2STR, 0 );
                else if ( typedef_is_pointer( right.type ) )        codeblock_add( code, MN_POINTER2STR, 0 );
                else if ( typedef_base( right.type ) == TYPE_CHAR ) codeblock_add( code, MN_CHR2STR, 0 );
                else if ( !typedef_is_string( right.type ) )        compile_error( MSG_INCOMP_TYPES );

                     if ( typedef_is_integer( left.type ) )         codeblock_add( code, MN_INT2STR | mntype( left.type, 0 ), 1 );
                else if ( typedef_is_double( left.type ) )          codeblock_add( code, MN_DOUBLE2STR, 1 );
                else if ( typedef_is_float( left.type ) )           codeblock_add( code, MN_FLOAT2STR, 1 );
                else if ( typedef_is_pointer( left.type ) )         codeblock_add( code, MN_POINTER2STR, 1 );
                else if ( typedef_base( left.type ) == TYPE_CHAR )  codeblock_add( code, MN_CHR2STR, 1 );
                else if ( !typedef_is_string( left.type ) )         compile_error( MSG_INCOMP_TYPES );

                codeblock_add( code, MN_STRING | MN_ADD, 0 );
                res.fvalue     = 0.0;
                res.value      = 0;
                res.constant   = 0;
                res.lvalue     = 0;
                res.asignation = 0;
                res.call       = 0;
                res.type       = typedef_new( TYPE_STRING );

                left = res;
                continue;
            }

            if ( right.lvalue ) codeblock_add( code, mntype( right.type, 0 ) | MN_PTR, 0 );

            t = check_numeric_types( &left, &right );
            if ( t != MN_FLOAT && t != MN_DOUBLE ) t = MN_QWORD;

            codeblock_add( code, op | t, 0 );
            res.constant   = ( right.constant && left.constant );
            res.lvalue     = 0;
            res.asignation = 0;
            res.call       = 0;

            if ( t == MN_DOUBLE || t == MN_FLOAT ) {
                res.value  = 0.0;
                res.type   = typedef_new( ( t == MN_DOUBLE ) ? TYPE_DOUBLE : TYPE_FLOAT );
                res.fvalue = ( op == MN_ADD ) ? left.fvalue + right.fvalue : left.fvalue - right.fvalue;
            } else {
                res.fvalue = 0.0;
                res.type   = left.type /*typedef_new( TYPE_INT )*/;
                res.value  = ( op == MN_ADD ) ? left.value + right.value : left.value - right.value;
            }

            left = res;
            continue;
        }
        token_back();
        break;
    }

    return left;
}

/* Step 11 */
expresion_result compile_rotation() {
    expresion_result left = compile_operation(), right, res;
    BASETYPE t;
    int64_t op;

    for (;;) {
        token_next();
        if ( token.type == IDENTIFIER && ( token.code == identifier_ror || token.code == identifier_rol ) ) { /* ">>" or "<<" */
            op = token.code == identifier_ror ? MN_ROR : MN_ROL;
            if ( left.lvalue ) codeblock_add( code, mntype( left.type, 0 ) | MN_PTR, 0 );

            right = compile_operation();
            if ( right.lvalue ) codeblock_add( code, mntype( right.type, 0 ) | MN_PTR, 0 );

            t = check_numeric_types( &left, &right );
            codeblock_add( code, op | mntype( left.type, 0 ), 0 );

            res.constant   = ( right.constant && left.constant );

            if ( t == MN_FLOAT || t == MN_DOUBLE ) compile_error( MSG_INTEGER_REQUIRED );

            res.type       = left.type /*typedef_new( TYPE_QWORD ) */;
            res.value      = ( op == MN_ROR ? ( left.value >> right.value ) : ( left.value << right.value ) );
            res.lvalue     = 0;
            res.asignation = 0;
            res.call       = 0;

            left = res;
            continue;
        }

        token_back();
        break;
    }

    return left;
}

/* Step 10 */
expresion_result compile_comparison_1() {
    expresion_result left = compile_rotation(), right, res;
    int64_t op, t;

    for (;;) {
        token_next();
        if (token.type == IDENTIFIER && (
            token.code == identifier_gt ||      /* ">" */
            token.code == identifier_lt ||      /* "<" */
            token.code == identifier_gte ||     /* ">=" or "=>" */
            token.code == identifier_lte ) )    /* "<=" or "=<" */
        {
            int is_unsigned = 0;

            op = token.code ;
            if ( left.lvalue && ( left.type.chunk[0].type != TYPE_ARRAY || left.type.chunk[1].type != TYPE_CHAR ) )
                codeblock_add( code, mntype( left.type, 0 ) | MN_PTR, 0 );

            right = compile_rotation();
            if ( right.lvalue && ( right.type.chunk[0].type != TYPE_ARRAY || right.type.chunk[1].type != TYPE_CHAR ) )
                codeblock_add( code, mntype( right.type, 0 ) | MN_PTR, 0 );

            t = check_numeric_or_string_types( &left, &right );
            if ( t != MN_FLOAT && t != MN_DOUBLE && t != MN_STRING ) t = MN_QWORD;

            if ( ( typedef_is_unsigned( left.type ) || typedef_is_unsigned( right.type ) ) &&
                 ( typedef_is_integer( left.type ) && typedef_is_integer( right.type ) ) ) is_unsigned = MN_UNSIGNED;

            res.value = 0;
            if ( op == identifier_gt ) {  /* ">" */
                codeblock_add( code, t | MN_GT | is_unsigned, 0 );
                if ( left.constant && right.constant ) {
                    if ( t == MN_QWORD )                        res.value = left.value > right.value;
                    else if ( t == MN_FLOAT || t == MN_DOUBLE ) res.value = ( left.fvalue > right.fvalue );
                    else                                        res.value = strcmp( string_get( left.value ), string_get( right.value ) ) > 0;
                }
            } else if ( op == identifier_lt ) { /* "<" */
                codeblock_add( code, t | MN_LT | is_unsigned, 0 );
                if ( left.constant && right.constant ) {
                    if ( t == MN_QWORD )                        res.value = left.value < right.value;
                    else if ( t == MN_FLOAT || t == MN_DOUBLE ) res.value = ( left.fvalue < right.fvalue );
                    else                                        res.value = strcmp( string_get( left.value ), string_get( right.value ) ) < 0;
                }
            } else if ( op == identifier_gte ) { /* ">=" or "=>" */
                codeblock_add( code, t | MN_GTE | is_unsigned, 0 );
                if ( left.constant && right.constant ) {
                    if ( t == MN_QWORD )                        res.value = left.value >= right.value;
                    else if ( t == MN_FLOAT || t == MN_DOUBLE ) res.value = ( left.fvalue >= right.fvalue );
                    else                                        res.value = strcmp( string_get( left.value ), string_get( right.value ) ) >= 0;
                }
            } else if ( op == identifier_lte ) { /* "<=" or "=<" */
                codeblock_add( code, t | MN_LTE | is_unsigned, 0 );
                if ( left.constant && right.constant ) {
                    if ( t == MN_QWORD )                        res.value = left.value <= right.value;
                    else if ( t == MN_FLOAT || t == MN_DOUBLE ) res.value = ( left.fvalue <= right.fvalue );
                    else                                        res.value = strcmp( string_get( left.value ), string_get( right.value ) ) <= 0;
                }
            }
            res.lvalue     = 0;
            res.asignation = 0;
            res.call       = 0;
            res.constant   = ( right.constant && left.constant );
            res.type       = typedef_new( TYPE_INT );
            left = res;
            continue;
        }
        token_back();
        break;
    }

    return left;
}

/* Step 9 */
expresion_result compile_comparison_2() {
    expresion_result left = compile_comparison_1(), right, res;
    int64_t op, t;

    for (;;) {
        token_next();
        if ( token.type == IDENTIFIER &&
             ( token.code == identifier_eq ||  /* "==" */
               token.code == identifier_ne ) ) /* "!=" or "<>" */
        {
//            int is_unsigned = 0;

            op = token.code;
            if ( left.lvalue && ( left.type.chunk[0].type != TYPE_ARRAY || left.type.chunk[1].type != TYPE_CHAR ) )
                codeblock_add( code, mntype( left.type, 0 ) | MN_PTR, 0 );

            right = compile_comparison_1();
            if ( right.lvalue && ( right.type.chunk[0].type != TYPE_ARRAY || right.type.chunk[1].type != TYPE_CHAR ) )
                codeblock_add( code, mntype( right.type, 0 ) | MN_PTR, 0 );

            t = check_numeric_or_string_types( &left, &right );
            if ( t != MN_FLOAT && t != MN_DOUBLE && t != MN_STRING ) t = MN_QWORD;

//            if ( typedef_is_unsigned( left.type ) && typedef_is_unsigned( right.type ) ) is_unsigned = MN_UNSIGNED;

            res.value = 0;

            if ( op == identifier_eq ) { /* "==" */
                codeblock_add( code, t | MN_EQ, 0 );
                if ( left.constant && right.constant ) {
                    if ( t == MN_QWORD )                        res.value = left.value == right.value;
                    else if ( t == MN_FLOAT || t == MN_DOUBLE ) res.value = ( left.fvalue == right.fvalue );
                    else                                        res.value = strcmp( string_get( left.value ), string_get( right.value ) ) == 0;
                }
            } else if ( op == identifier_ne ) { /* "!=" or "<>" */
                codeblock_add( code, t | MN_NE, 0 );
                if ( left.constant && right.constant ) {
                    if ( t == MN_QWORD )                        res.value = left.value != right.value;
                    else if ( t == MN_FLOAT || t == MN_DOUBLE ) res.value = ( left.fvalue != right.fvalue );
                    else                                        res.value = strcmp( string_get( left.value ), string_get( right.value ) ) != 0;
                }
            }
            res.lvalue     = 0;
            res.asignation = 0;
            res.call       = 0;
            res.constant   = ( right.constant && left.constant );
            res.type       = typedef_new( TYPE_INT );
            left = res;
            continue;
        }
        token_back();
        break;
    }

    return left;
}

/* Step 8 */
expresion_result compile_bitwise_and() {
    expresion_result left = compile_comparison_2(), right, res;

    for (;;) {
        token_next();
        if ( token.type == IDENTIFIER && ( token.code == identifier_band || token.code == identifier_bandoffset ) ) { /* "BAND" or "&" */
            if ( left.lvalue ) codeblock_add( code, mntype( left.type, 0 ) | MN_PTR, 0 );
            right = compile_comparison_2();
            if ( right.lvalue ) codeblock_add( code, mntype( right.type, 0 ) | MN_PTR, 0 );
            check_integer_types( &left, &right );

            codeblock_add( code, MN_BAND, 0 );

            res.lvalue     = 0;
            res.asignation = 0;
            res.call       = 0;
            res.constant   = ( right.constant && left.constant );
            res.value      = ( left.value & right.value );
            res.type       = left.type /* typedef_new( TYPE_INT )*/;

            left = res;
            continue;
        }

        token_back();
        break;
    }

    return left;
}

/* Step 7 */
expresion_result compile_bitwise_xor() {
    expresion_result left = compile_bitwise_and(), right, res;

    for (;;) {
        token_next();
        if ( token.type == IDENTIFIER && token.code == identifier_bxor ) { /* "BXOR" or "^" */
            if ( left.lvalue ) codeblock_add( code, mntype( left.type, 0 ) | MN_PTR, 0 );
            right = compile_bitwise_and();
            if ( right.lvalue ) codeblock_add( code, mntype( right.type, 0 ) | MN_PTR, 0 );
            check_integer_types( &left, &right );

            codeblock_add( code, MN_BXOR, 0 );

            res.lvalue     = 0;
            res.asignation = 0;
            res.call       = 0;
            res.constant   = ( right.constant && left.constant );
            res.value      = ( left.value ^ right.value );
            res.type       = left.type /* typedef_new( TYPE_INT )*/;

            left = res;
            continue;
        }

        token_back();
        break;
    }

    return left;
}

/* Step 6 */
expresion_result compile_bitwise_or() {
    expresion_result left = compile_bitwise_xor(), right, res;

    for (;;) {
        token_next();
        if ( token.type == IDENTIFIER && token.code == identifier_bor ) { /* "BOR" or "|" */
            if ( left.lvalue ) codeblock_add( code, mntype( left.type, 0 ) | MN_PTR, 0 );
            right = compile_bitwise_xor();
            if ( right.lvalue ) codeblock_add( code, mntype( right.type, 0 ) | MN_PTR, 0 );
            check_integer_types( &left, &right );

            codeblock_add( code, MN_BOR, 0 );

            res.lvalue     = 0;
            res.asignation = 0;
            res.call       = 0;
            res.constant   = ( right.constant && left.constant );
            res.value      = ( left.value | right.value );
            res.type       = left.type /*typedef_new( TYPE_INT )*/;

            left = res;
            continue;
        }

        token_back();
        break;
    }

    return left;
}


/* Step 5 */
expresion_result compile_logical_and() {
    expresion_result left = compile_bitwise_or(), right, res;
    int et1;

    for (;;) {
        token_next();
        if ( token.type == IDENTIFIER && token.code == identifier_and ) { /* "AND" or "&&" */
            et1 = codeblock_label_add( code, -1 );

            if ( left.lvalue ) codeblock_add( code, MN_PTR | mntype( left.type, 0 ), 0 );
            check_integer_type( &left );
            codeblock_add( code, MN_JTFALSE, et1 );
            right = compile_bitwise_or();
            if ( right.lvalue ) codeblock_add( code, MN_PTR | mntype( right.type, 0 ), 0 );
            check_integer_type( &right );

            codeblock_add( code, MN_AND, 0 );

            codeblock_label_set( code, et1, code->current );

            res.lvalue     = 0;
            res.asignation = 0;
            res.call       = 0;
            res.constant   = ( right.constant && left.constant );
            res.value      = ( left.value && right.value );
            res.type       = typedef_new( TYPE_INT );

            left = res;
            continue;
        }

        token_back();
        break;
    }

    return left;
}

/* Step 4 */
expresion_result compile_logical_xor() {
    expresion_result left = compile_logical_and(), right, res;

    for (;;) {
        token_next();
        if ( token.type == IDENTIFIER && token.code == identifier_xor ) { /* "XOR" or "^^" */
            if ( left.lvalue ) codeblock_add( code, mntype( left.type, 0 ) | MN_PTR, 0 );
            right = compile_logical_and();
            if ( right.lvalue ) codeblock_add( code, mntype( right.type, 0 ) | MN_PTR, 0 );
            check_integer_types( &left, &right );

            codeblock_add( code, MN_XOR, 0 );

            res.lvalue     = 0;
            res.asignation = 0;
            res.call       = 0;
            res.constant   = ( right.constant && left.constant );
            res.value      = (( left.value != 0 ) ^( right.value != 0 ) );
            res.type       = typedef_new( TYPE_INT );

            left = res;
            continue;
        }

        token_back();
        break;
    }

    return left;
}

/* Step 3 */
expresion_result compile_logical_or() {
    expresion_result left = compile_logical_xor(), right, res;
    int et1, et2;

    for (;;) {
        token_next();
        if ( token.type == IDENTIFIER && token.code == identifier_or ) { /* "OR" or "||" */
            et1 = codeblock_label_add( code, -1 );
            et2 = codeblock_label_add( code, -1 );

            if ( left.lvalue ) codeblock_add( code,  mntype( left.type, 0 ) | MN_PTR, 0 );
            check_integer_type( &left );
            codeblock_add( code, MN_JTFALSE, et1 );

            codeblock_add( code, MN_POP, 0 );
            codeblock_add( code, MN_PUSH, 1 );
            codeblock_add( code, MN_JUMP, et2 );

            codeblock_label_set( code, et1, code->current );

            right = compile_logical_xor();
            if ( right.lvalue ) codeblock_add( code, mntype( right.type, 0 ) | MN_PTR, 0 );
            check_integer_type( &right );

            codeblock_add( code, MN_OR, 0 );

            codeblock_label_set( code, et2, code->current );

            res.lvalue     = 0;
            res.asignation = 0;
            res.call       = 0;
            res.constant   = ( right.constant && left.constant );
            res.value      = ( left.value || right.value );
            res.type       = typedef_new( TYPE_INT );

            left = res;
            continue;
        }

        token_back();
        break;
    }

    return left;
}

/* Step 2 */
expresion_result compile_ternarycond() {
    CODEBLOCK_POS pos = { 0 };

    if ( code ) pos = codeblock_pos( code );

    expresion_result base = compile_logical_or(), right, res;

    token_next();
    /* Operador EXPR ? TRUE : FALSE */
    if ( token.type == IDENTIFIER && token.code == identifier_question ) { /* "?" */
        if ( base.lvalue ) codeblock_add( code, mntype( base.type, 0 ) | MN_PTR, 0 );

        if ( base.constant ) {
            codeblock_setpos( code, pos );

            if ( ( typedef_is_integer( base.type ) && base.value ) ||
                 ( ( typedef_is_double( base.type ) || typedef_is_float( base.type ) ) && base.fvalue ) )
            {
                right = compile_expresion( 0, 0, 0, TYPE_UNDEFINED );
                if ( code ) pos = codeblock_pos( code );
                token_next();
                if ( token.type != IDENTIFIER || token.code != identifier_colon ) compile_error( MSG_EXPECTED, ":" ); /* ":" */
                res = compile_expresion( 0, 0, 0, right.type.chunk[0].type );
                codeblock_setpos( code, pos );
            } else {
                if ( code ) pos = codeblock_pos( code );
                right = compile_expresion( 0, 0, 0, TYPE_UNDEFINED );
                codeblock_setpos( code, pos );
                token_next();
                if ( token.type != IDENTIFIER || token.code != identifier_colon ) compile_error( MSG_EXPECTED, ":" ); /* ":" */
                res = compile_expresion( 0, 0, 0, right.type.chunk[0].type );
            }
        } else {
            int et1, et2;
            et1 = codeblock_label_add( code, -1 );
            et2 = codeblock_label_add( code, -1 );
            codeblock_add( code, MN_JFALSE, et1 );

            right = compile_expresion( 0, 0, 0, TYPE_UNDEFINED );
            codeblock_add( code, MN_JUMP, et2 );

            codeblock_label_set( code, et1, code->current );
            token_next();

            if ( token.type != IDENTIFIER || token.code != identifier_colon ) compile_error( MSG_EXPECTED, ":" ); /* ":" */

            res = compile_expresion( 0, 0, 0, right.type.chunk[0].type );
            codeblock_add( code, MN_NOP, 0 );
            codeblock_label_set( code, et2, code->current );
        }

        if ( base.constant && res.constant && right.constant ) {
            if ( typedef_is_integer( base.type ) ) {
                return base.value ? right : res;
            } else if ( typedef_is_double( base.type ) || typedef_is_float( base.type ) ) {
                return base.fvalue ? right : res;
            }
        }

        res.constant = 0;
        res.lvalue = ( right.lvalue && res.lvalue );

        return res;
    }

    token_back();
    return base;
}

/* Step 1 */
expresion_result compile_subexpresion() {
    expresion_result base = compile_ternarycond(), right, res;

    token_next();

    if ( token.type == IDENTIFIER ) {

        int cstring_target = 0;
        if ( token.code == identifier_dollar ) {
            cstring_target = 1;
            token_next();
        }

        /* Assignments */
        if ( token.code == identifier_equal ) { /* "=" */
            if ( typedef_is_array( base.type ) && base.lvalue && base.type.chunk[1].type == TYPE_CHAR ) { /* Assignments to fixed-width strings */
                if ( cstring_target ) {
                    token_back();
                    compile_error( MSG_POINTER_VARIABLE_REQUIRED );
                }

                right = compile_expresion( 0, 0, 0, TYPE_UNDEFINED );
                if ( typedef_is_integer( right.type ) ) {
                    compile_warning( 1, "implicit conversion (INT64 to CHAR[])" );
                    codeblock_add( code, MN_INT2STR | mntype( right.type, 0 ), 0 );
                } else if ( typedef_is_double( right.type ) ) {
                    compile_warning( 1, "implicit conversion (DOUBLE to CHAR[])" );
                    codeblock_add( code, MN_DOUBLE2STR, 0 );
                } else if ( typedef_is_float( right.type ) ) {
                    compile_warning( 1, "implicit conversion (FLOAT to CHAR[])" );
                    codeblock_add( code, MN_FLOAT2STR, 0 );
                } else if ( !typedef_is_string( right.type ) ) {
                    compile_error( MSG_INCOMP_TYPE );
                }
                codeblock_add( code, MN_STR2A, base.type.chunk[0].count - 1 );
                right.asignation = 1;
                return right;
            }

            if ( typedef_is_pointer( base.type ) ) { /* Assignments to pointers */
                if ( !base.lvalue ) compile_error( MSG_VARIABLE_REQUIRED );

                right = compile_expresion( 0, 0, 0, TYPE_UNDEFINED );

                if ( typedef_is_integer( right.type ) && right.constant && right.value == 0 ) {
                    right.type = base.type;
                }

                if ( typedef_is_double( right.type ) ) {
                    // implicit conversion (DOUBLE to POINTER)
                    codeblock_add( code, MN_DOUBLE2INT | mntype( base.type, 0 ), 0 );
                    right.value = right.fvalue;
                }

                if ( typedef_is_float( right.type ) ) {
                    // implicit conversion (FLOAT to POINTER)
                    codeblock_add( code, MN_FLOAT2INT | mntype( base.type, 0 ), 0 );
                    right.value = right.fvalue;
                }

                if ( typedef_is_string( right.type ) ) {
#if 0
                    if ( base.type.chunk[1].type == TYPE_CHAR ) {
                        if ( cstring_target ) {
                            // implicit conversion (STRING to C CHAR*)
                            codeblock_add( code, MN_STR2CHARNUL, 0 );
                        } else {
                            // implicit conversion (STRING to POINTER)
                            codeblock_add( code, MN_STR2POINTER, 0 );
                        }
                    } else {
                        // implicit conversion (STRING to POINTER)
                        codeblock_add( code, MN_STR2POINTER, 0 );
                        right.value = string_atop( ( unsigned char * ) string_get( right.value ) );
                    }
#endif
                    if ( cstring_target ) {
                        // implicit conversion (STRING to C CHAR*)
                        codeblock_add( code, MN_STR2CHARNUL, 0 );
                    } else {
                        // implicit conversion (STRING to POINTER)
                        codeblock_add( code, MN_STR2POINTER, 0 );
                    }
                    right.value = string_atop( ( unsigned char * ) string_get( right.value ) );
                }

                /* A "void" pointer can be assigned to any other */
                if ( !typedef_is_equal( right.type, base.type ) ) compile_warning( 1, MSG_CAST_POINTER_INCOMPATIBLE );

                codeblock_add( code, MN_QWORD | MN_LET, 0 );

                res.lvalue     = 1;
                res.asignation = 1;
                res.call       = 0;
                res.constant   = 0;
                res.value      = 0;
                res.type       = base.type;

                return res;
            }

            if ( cstring_target ) {
                token_back();
                compile_error( MSG_POINTER_VARIABLE_REQUIRED );
            }

            if ( typedef_base( base.type ) == TYPE_CHAR ) { /* Assignments to chars */
                if ( !base.lvalue ) compile_error( MSG_VARIABLE_REQUIRED );

                right = compile_expresion( 0, 0, 0, TYPE_UNDEFINED );

                if ( typedef_is_string( right.type ) ) {
                    compile_warning( 1, "implicit conversion (STRING to CHAR)" );
                    codeblock_add( code, MN_STR2CHR, 0 );
                } else if ( typedef_is_double( right.type ) ) {
                    compile_warning( 1, "implicit conversion (DOUBLE to CHAR)" );
                    codeblock_add( code, MN_DOUBLE2INT | mntype( base.type, 0 ), 0 );
                } else if ( typedef_is_float( right.type ) ) {
                    compile_warning( 1, "implicit conversion (FLOAT to CHAR)" );
                    codeblock_add( code, MN_FLOAT2INT | mntype( base.type, 0 ), 0 );
                } else if ( !typedef_is_numeric( right.type ) ) {
                    compile_error( MSG_INCOMP_TYPE );
                }

                codeblock_add( code, MN_LET | MN_BYTE, 0 );

                res.lvalue     = 1;
                res.asignation = 1;
                res.call       = 0;
                res.constant   = 0;
                res.value      = 0;
                res.type       = typedef_new( TYPE_CHAR );

                return res;
            }

            if ( typedef_is_string( base.type ) ) { /* Assignments to strings */
                if ( !base.lvalue ) compile_error( MSG_VARIABLE_REQUIRED );

                right = compile_expresion( 0, 0, 0, TYPE_UNDEFINED );

                if ( typedef_base( right.type ) == TYPE_CHAR ) {
                    compile_warning( 1, "implicit conversion (CHAR to STRING)" );
                    codeblock_add( code, MN_CHR2STR, 0 );
                } else if ( typedef_is_integer( right.type ) ) {
                    compile_warning( 1, "implicit conversion (INTEGER to STRING)" );
                    codeblock_add( code, MN_INT2STR | mntype( right.type, 0 ), 0 );
                } else if ( typedef_is_double( right.type ) ) {
                    compile_warning( 1, "implicit conversion (DOUBLE to STRING)" );
                    codeblock_add( code, MN_DOUBLE2STR, 0 );
                } else if ( typedef_is_float( right.type ) ) {
                    compile_warning( 1, "implicit conversion (FLOAT to STRING)" );
                    codeblock_add( code, MN_FLOAT2STR, 0 );
                } else if ( typedef_is_pointer( right.type ) ) {
                    compile_warning( 1, "implicit conversion (POINTER to STRING)" );
                    codeblock_add( code, MN_POINTER2STR, 0 );
                } else if ( !typedef_is_string( right.type ) ) {
                    compile_error( MSG_INCOMP_TYPE );
                }
                codeblock_add( code, MN_STRING | MN_LET, 0 );

                res.lvalue     = 1;
                res.asignation = 1;
                res.call       = 0;
                res.constant   = 0;
                res.value      = 0;
                res.type       = typedef_new( TYPE_STRING );

                return res;
            }
        }

        int64_t op, type;

        /* Pointer += integer */

        if ( typedef_is_pointer( base.type ) && ( token.code == identifier_plusequal || token.code == identifier_minusequal ) ) { /* "+=" or "-=" */
            TYPEDEF pointer_type;

            op = ( token.code == identifier_plusequal ? MN_VARADD : MN_VARSUB );
            pointer_type = typedef_reduce( base.type );
            if ( !base.lvalue ) compile_error( MSG_VARIABLE_REQUIRED );

            right = compile_expresion( 0, 0, 0, TYPE_UNDEFINED );

//            if ( !typedef_is_integer( right.type ) ) compile_error( MSG_INCOMP_TYPES );

            type = check_integer_types( &base, &right );

            if ( typedef_size( pointer_type ) > 1 ) {
                codeblock_add( code, MN_PUSH, typedef_size( pointer_type ) );
                codeblock_add( code, MN_MUL | type, 0 );
                codeblock_add( code, op | MN_QWORD, 0 );
//                codeblock_add( code, MN_ARRAY, ( op == MN_VARADD ? 1 : -1 ) * typedef_size( pointer_type ) );
            } else
                codeblock_add( code, op, 0 );

            res.lvalue     = 1;
            res.asignation = 1;
            res.call       = 0;
            res.constant   = 0;
            res.value      = 0;
            res.type       = typedef_new( TYPE_POINTER );

            return res;
        }

        /* Increment and assign */

        if ( token.code == identifier_plusequal ) { /* "+=" */
            if ( typedef_is_array( base.type ) && base.lvalue && base.type.chunk[1].type == TYPE_CHAR ) { /* String += fixed string */
                right = compile_expresion( 0, 0, 0, TYPE_UNDEFINED );
                if ( typedef_is_integer( right.type ) ) {
                    compile_warning( 1, "implicit conversion (INTEGER to CHAR[])" );
                    codeblock_add( code, MN_INT2STR | mntype( right.type, 0 ), 0 );
                } else if ( typedef_is_double( right.type ) ) {
                    compile_warning( 1, "implicit conversion (DOUBLE to CHAR[])" );
                    codeblock_add( code, MN_DOUBLE2STR, 0 );
                } else if ( typedef_is_float( right.type ) ) {
                    compile_warning( 1, "implicit conversion (FLOAT to CHAR[])" );
                    codeblock_add( code, MN_FLOAT2STR, 0 );
                } else if ( typedef_base( right.type ) == TYPE_CHAR ) {
                    compile_warning( 1, "implicit conversion (CHAR to CHAR[])" );
                    codeblock_add( code, MN_CHR2STR, 0 );
                } else if ( !typedef_is_string( right.type ) ) {
                    compile_error( MSG_INCOMP_TYPE );
                }
                codeblock_add( code, MN_STRACAT, base.type.chunk[0].count );
                right.asignation = 1;

                return right;
            }

            if ( typedef_is_string( base.type ) ) {
                if ( !base.lvalue ) compile_error( MSG_VARIABLE_REQUIRED );
                right = compile_expresion( 0, 0, 0, TYPE_UNDEFINED );
                if ( typedef_is_integer( right.type ) ) {
                    compile_warning( 1, "implicit conversion (INTEGER to STRING)" );
                    codeblock_add( code, MN_INT2STR | mntype( right.type, 0 ), 0 );
                } else if ( typedef_is_double( right.type ) ) {
                    compile_warning( 1, "implicit conversion (DOUBLE to STRING)" );
                    codeblock_add( code, MN_FLOAT2STR, 0 );
                } else if ( typedef_is_float( right.type ) ) {
                    compile_warning( 1, "implicit conversion (FLOAT to STRING)" );
                    codeblock_add( code, MN_FLOAT2STR, 0 );
                } else if ( typedef_base( right.type ) == TYPE_CHAR ) {
                    compile_warning( 1, "implicit conversion (CHAR to STRING)" );
                    codeblock_add( code, MN_CHR2STR, 0 );
                } else if ( !typedef_is_string( right.type ) ) {
                    compile_error( MSG_INCOMP_TYPE );
                }
                codeblock_add( code, MN_STRING | MN_VARADD, 0 );

                res.lvalue     = 1;
                res.asignation = 1;
                res.call       = 0;
                res.constant   = 0;
                res.value      = 0;
                res.type       = typedef_new( TYPE_STRING );

                return res;
            }
        }

        /* Another possible combination (for not string/char[]/pointers) */

        if (    token.code == identifier_plusequal      /* "+=" */
            ||  token.code == identifier_minusequal     /* "-=" */
            ||  token.code == identifier_multequal      /* "*=" */
            ||  token.code == identifier_divequal       /* "/=" */
            ||  token.code == identifier_modequal       /* "%=" */
            ||  token.code == identifier_andequal       /* "&=" */
            ||  token.code == identifier_xorequal       /* "^=" */
            ||  token.code == identifier_orequal        /* "|=" */
            ||  token.code == identifier_rolequal       /* "<<=" */
            ||  token.code == identifier_rorequal       /* ">>=" */
            ||  token.code == identifier_equal )        /* "=" */
        {
            op = token.code;

            /* Assignation to struct: struct copy */

            TYPEDEF btype = base.type;
            while ( typedef_is_array( btype ) ) btype = typedef_reduce( btype );

            if ( typedef_is_struct( btype ) ) {
                if ( token.code != identifier_equal ) compile_error( MSG_EXPECTED, "=" ) ; /* "=" */

                right = compile_expresion( 0, 0, 0, TYPE_UNDEFINED );

                TYPEDEF rtype = right.type;
                while ( typedef_is_array( rtype ) || typedef_is_pointer( rtype ) ) rtype = typedef_reduce( rtype );

                if ( typedef_is_pointer( right.type ) ) right.type = typedef_reduce( right.type );

                if ( !typedef_is_struct( rtype ) ) compile_error( MSG_STRUCT_REQUIRED );

                if ( !typedef_is_equal( right.type, base.type ) ) compile_error( MSG_TYPES_NOT_THE_SAME );

                /* Struct copy operator */
                while ( typedef_is_pointer( base.type ) ) {
                    codeblock_add( code, MN_PTR, 0 );
                    res.type = typedef_reduce( base.type );
                }

                if ( typedef_base( btype ) != TYPE_STRUCT ) compile_error( MSG_STRUCT_REQUIRED );

                if ( right.type.varspace->stringvar_count > 0 ) {
                    /* True struct copy version */
                    int size = right.type.varspace->count * sizeof( DCB_TYPEDEF );
                    segment_ensure_capacity(globaldata, size);
                    codeblock_add( code, MN_GLOBAL, globaldata->current );

                    for ( int nvar = 0 ; nvar < right.type.varspace->count ; nvar++ ) {
                        DCB_TYPEDEF dcbtype;
                        dcb_settype( &dcbtype, &right.type.varspace->vars[nvar].type );
                        memcpy(( uint8_t* )globaldata->bytes + globaldata->current, &dcbtype, sizeof( DCB_TYPEDEF ) );
                        globaldata->current += sizeof( DCB_TYPEDEF );
                    }
                    codeblock_add( code, MN_PUSH, right.type.varspace->count );
                    codeblock_add( code, MN_PUSH, typedef_tcount( right.type ) );
                    codeblock_add( code, MN_COPY_STRUCT, 0 );
                } else {
                    /* Optimized fast memcopy version */
                    codeblock_add( code, MN_PUSH, typedef_size( right.type ) );
                    codeblock_add( code, MN_COPY_ARRAY | MN_BYTE, 0 );
                }

                base.lvalue     = 1;
                base.asignation = 1;
                base.call       = 0;
                base.constant   = 0;
                base.value      = 0;
                base.type       = right.type;

                return base;
            }

            if ( op != identifier_equal && typedef_is_array( base.type ) ) compile_error( MSG_EXPECTED, "[" );

            if ( !base.lvalue ) compile_error( MSG_VARIABLE_REQUIRED );
            right = compile_expresion( 0, 0, 0, typedef_base( base.type ) );

            /* Array copy */
            if ( op == identifier_equal && typedef_is_array( base.type ) && typedef_is_array( right.type ) ) {
                if ( !typedef_is_equal( base.type, right.type ) ) compile_error( MSG_TYPES_NOT_THE_SAME );

                int count = typedef_tcount( base.type );

                /* Optimized fast memcopy version */
                codeblock_add( code, MN_PUSH, count );
                codeblock_add( code, MN_COPY_ARRAY | mntype( base.type, 1 ), 0 );

                base.lvalue     = 1;
                base.asignation = 1;
                base.call       = 0;
                base.constant   = 0;
                base.value      = 0;
                base.type       = right.type;

                return base;
            }

            if ( right.lvalue ) codeblock_add( code, mntype( right.type, 0 ) | MN_PTR, 0 );

            type = check_numeric_types( &base, &right );

            if ( op == identifier_plusequal )               /* "+=" */
                codeblock_add( code, type | MN_VARADD, 0 );
            else if ( op == identifier_minusequal )         /* "-=" */
                codeblock_add( code, type | MN_VARSUB, 0 );
            else if ( op == identifier_multequal )          /* "*=" */
                codeblock_add( code, type | MN_VARMUL, 0 );
            else if ( op == identifier_divequal )           /* "/=" */
                codeblock_add( code, type | MN_VARDIV, 0 );
            else if ( op == identifier_modequal )           /* "%=" */
                codeblock_add( code, type | MN_VARMOD, 0 );
            else if ( op == identifier_orequal )            /* "|=" */
                codeblock_add( code, type | MN_VAROR, 0 );
            else if ( op == identifier_andequal )           /* "&=" */
                codeblock_add( code, type | MN_VARAND, 0 );
            else if ( op == identifier_xorequal )           /* "^=" */
                codeblock_add( code, type | MN_VARXOR, 0 );
            else if ( op == identifier_rorequal )           /* ">>=" */
                codeblock_add( code, type | MN_VARROR, 0 );
            else if ( op == identifier_rolequal )           /* "<<=" */
                codeblock_add( code, type | MN_VARROL, 0 );
            else if ( op == identifier_equal )              /* "=" */
                codeblock_add( code, type | MN_LET, 0 );

            res.lvalue     = 1;
            res.asignation = 1;
            res.call       = 0;
            res.constant   = 0;
            res.value      = 0;
            res.type       = right.type;

            return res;
        } else {
            token_back();
        }
    } else {
        token_back();
    }

    return base;
}

expresion_result compile_expresion( int need_constant, int need_lvalue, int discart_code, BASETYPE t ) {
    expresion_result res;
    CODEBLOCK_POS pos = { 0 };

    if ( code ) pos = codeblock_pos( code );

    res = compile_subexpresion();

    /* Interprets a structure as a pointer to itself */

    if ( res.lvalue && typedef_base( res.type ) == TYPE_STRUCT && !need_lvalue ) {
        res.type = typedef_pointer( res.type );
        res.lvalue = 0;
        res.constant = 0;
    }

    /* Interprets arrays of bytes as strings */

    if ( typedef_base( res.type ) == TYPE_ARRAY && res.type.chunk[1].type == TYPE_CHAR && res.lvalue && !need_lvalue ) {
        codeblock_add( code, MN_A2STR, 0 );
        res.type = typedef_new( TYPE_STRING ) ; /* Array -> String */
        res.lvalue = 0;
    }

    /* Removes the lvalue */

    if ( !need_lvalue && res.lvalue && !typedef_is_array( res.type ) ) {
        res.lvalue = 0;
        codeblock_add( code, mntype( res.type, 0 ) | MN_PTR, 0 );
    }

    /* Type conversions */

    if ( t != TYPE_UNDEFINED ) res = convert_result_type( res, t );

    /* Constant data optimization */

    if ( res.constant ) {
        if ( code ) codeblock_setpos( code, pos );
        if ( typedef_is_double( res.type ) ) {
            codeblock_add( code, MN_PUSH, *( int64_t * )&res.fvalue );
        } else if ( typedef_is_float( res.type ) ) {
            float f = ( float ) res.fvalue;
            codeblock_add( code, MN_PUSH, *( int32_t * )&f );
        } else if ( typedef_base( res.type ) == TYPE_STRING ) {
            codeblock_add( code, MN_PUSH | MN_STRING, res.value );
        } else {
            codeblock_add( code, MN_PUSH, res.value );
        }
    }

    if ( need_lvalue && !res.lvalue ) compile_error( MSG_VARIABLE_REQUIRED );
    if ( need_constant && !res.constant ) compile_error( MSG_CONSTANT_EXP );

    if ( discart_code && code ) codeblock_setpos( code, pos );

    return res;
}

int compile_sentence_end() {
    token_next();

    if ( token.type == NOTOKEN ) return 1;

    if ( token.type == IDENTIFIER && token.code == identifier_semicolon ) return 0 ; /* ";" */

    /*
    if (token.type == IDENTIFIER && token.code == identifier_end)
    {
        token_back();
        return 1;
    }
    */

    compile_error( MSG_EXPECTED, ";" );
    return 0;
}

extern int dcb_options;

void compile_block( PROCDEF * p ) {
    int loop, last_loop, et1, et2, codelabel, is_process;
    expresion_result res, from, to;
    PROCDEF * ps = NULL;

    proc = p;
    code = &p->code;

    incrementCounter(&lvlstk);
    incrementLevel(&lvlstk);

    for (;;) {
        token_next();
        if ( token.type == NOTOKEN ) break;

        tok_pos tokp = token_pos();

        ps = NULL;
        /* Compile inline variable definitions */
        if ( identifier_is_basic_type( token.code ) || token.code == identifier_struct || typedef_by_name( token.code ) || ( ps = procdef_search( token.code ) ) ) {
            is_process = 0;
            if ( ps ) {
                token_next();
                if ( token.code == identifier_leftp ) is_process = 1; // is a process
            }
            token_set_pos( tokp );

            if ( !is_process ) {
                if ( dcb_options & DCB_DEBUG ) codeblock_add( code, MN_SENTENCE, line_count + ( current_file << 20 ) );
                token_back();
                /* It is allowed to declare a variable as private that has been declared as global; it's a local variable, not the global one */
                VARSPACE * v[] = {&local, p->pubvars, NULL};
                compile_varspace( p->privars, p->pridata, 1, 1, v, DEFAULT_ALIGNMENT, 1, 1, 0, 1 );
                continue;
            }
        } else if (( !proc->declared ) && ( token.code == identifier_local || token.code == identifier_public ) ) {
            /* Local declarations are local only to the process but visible from every process */
            /* It is allowed to declare a variable as local/public that has been declared global; it's a local variable, not the global one */
            VARSPACE * v[] = {&local, p->privars, NULL};
            compile_varspace( p->pubvars, p->pubdata, 1, 1, v, DEFAULT_ALIGNMENT, 1, 1, 0, 0 );
        }

        if ( token.type == IDENTIFIER ) {
            if ( token.code == identifier_end    ||  /* "END" */
                 token.code == identifier_until  ||  /* "UNTIL" */
                 token.code == identifier_else   ||  /* "ELSE" */
                 token.code == identifier_elseif )   /* "ELSEIF" */
                break;

            if ( token.code == identifier_semicolon ) /* ";" */
                continue;

            if ( token.code == identifier_colon )   /* ":" */
                continue;

            if ( token.code == identifier_continue ) { /* "CONTINUE" */
                if ( dcb_options & DCB_DEBUG ) codeblock_add( code, MN_SENTENCE, line_count + ( current_file << 20 ) );

                if ( !code->loop_active ) compile_error( MSG_NO_LOOP );

                codeblock_add( code, MN_REPEAT, code->loop_active );
                compile_sentence_end();
                continue;
            }

            if ( token.code == identifier_break ) { /* "BREAK" */
                if ( dcb_options & DCB_DEBUG ) codeblock_add( code, MN_SENTENCE, line_count + ( current_file << 20 ) );

                if ( !code->loop_active ) compile_error( MSG_NO_LOOP );

                codeblock_add( code, MN_BREAK, code->loop_active );
                compile_sentence_end();
                continue;
            }

            if ( token.code == identifier_frame || token.code == identifier_yield ) { /* "FRAME or YIELD " */
                if ( typedef_base( proc->type ) != TYPE_INT && typedef_base( proc->type ) != TYPE_QWORD )
                    if ( !( proc->flags & PROC_FUNCTION ) ) compile_error( MSG_FRAME_REQUIRES_INT );

                if ( dcb_options & DCB_DEBUG ) codeblock_add( code, MN_SENTENCE, line_count + ( current_file << 20 ) );

                token_next();
                if ( token.type != IDENTIFIER || token.code != identifier_semicolon ) { /* ";" */
                    token_back();
                    compile_expresion( 0, 0, 0, TYPE_QWORD );
                    codeblock_add( code, MN_FRAME, 0 );
                    compile_sentence_end();
                } else {
                    codeblock_add( code, MN_PUSH, 100 );
                    codeblock_add( code, MN_FRAME, 0 );
                }
                proc->flags |= PROC_USES_FRAME;
                continue;
            }

            if ( token.code == identifier_debug ) { /* "DEBUG" */
                if ( dcb_options & DCB_DEBUG ) codeblock_add( code, MN_SENTENCE, line_count + ( current_file << 20 ) );

                codeblock_add( code, MN_DEBUG, 0 );
                compile_sentence_end();
                continue;
            }

            if ( token.code == identifier_return ) { /* "RETURN" */
                if ( dcb_options & DCB_DEBUG ) codeblock_add( code, MN_SENTENCE, line_count + ( current_file << 20 ) );

                token_next();
                if ( token.type != IDENTIFIER || token.code != identifier_semicolon ) { /* ";" */
                    token_back();
                    compile_expresion( 0, 0, 0, typedef_base( p->type ) );
                    codeblock_add( code, MN_RETURN, 0 );
                    compile_sentence_end();
                } else {
                    codeblock_add( code, MN_END, 0 );
                }
                continue;
            }

            if ( token.code == identifier_clone ) { /* "CLONE" */
                if ( dcb_options & DCB_DEBUG ) codeblock_add( code, MN_SENTENCE, line_count + ( current_file << 20 ) );

                et1 = codeblock_label_add( code, -1 );
                codeblock_add( code, MN_CLONE, et1 );
                compile_block( p );
                codeblock_label_set( code, et1, code->current );
                proc->flags |= PROC_USES_FRAME;
                continue;
            }

            if ( token.code == identifier_begin ) { /* "BEGIN .. END" */
                if ( dcb_options & DCB_DEBUG ) codeblock_add( code, MN_SENTENCE, line_count + ( current_file << 20 ) );
                compile_block( p );
                continue;
            }

            if ( token.code == identifier_if ) { /* "IF" */
                /* Label at the end of a IF/ELSEIF/ELSEIF/ELSE chain */
                int end_of_chain = -1;

                if ( dcb_options & DCB_DEBUG ) codeblock_add( code, MN_SENTENCE, line_count + ( current_file << 20 ) );

                for (;;) {
                    token_next();
                    if ( token.type != IDENTIFIER || token.code != identifier_leftp ) { /* "(" */
                        token_back();
                        compile_expresion( 0, 0, 0, TYPE_QWORD );
                        token_next();
                        if ( token.type != IDENTIFIER || ( token.code != identifier_semicolon && token.code != identifier_colon ) ) /* ";" or ":" */
                            compile_error( MSG_EXPECTED, ";" MSG_OR ":" );
                    } else {
                        compile_expresion( 0, 0, 0, TYPE_QWORD );
                        token_next();
                        if ( token.type != IDENTIFIER || token.code != identifier_rightp ) /* ")" */
                            compile_error( MSG_EXPECTED, ")" );
                    }

                    et1 = codeblock_label_add( code, -1 );
                    codeblock_add( code, MN_JFALSE, et1 );
                    compile_block( p );
                    if ( token.type == IDENTIFIER && token.code == identifier_else ) { /* "ELSE" */
                        et2 = codeblock_label_add( code, -1 );
                        codeblock_add( code, MN_JUMP, et2 );
                        codeblock_label_set( code, et1, code->current );
                        compile_block( p );
                        codeblock_label_set( code, et2, code->current );
                        if ( token.code == identifier_else ) compile_error( MSG_ELSE_WOUT_IF );
                        break;
                    } else if ( token.type == IDENTIFIER && token.code == identifier_elseif ) { /* "ELSEIF" */
                        if ( dcb_options & DCB_DEBUG ) codeblock_add( code, MN_SENTENCE, line_count + ( current_file << 20 ) );
                        if ( end_of_chain == -1 ) end_of_chain = codeblock_label_add( code, -1 );
                        codeblock_add( code, MN_JUMP, end_of_chain );
                        codeblock_label_set( code, et1, code->current );
                        continue;
                    } else {
                        codeblock_label_set( code, et1, code->current );
                        break;
                    }
                }
                if ( end_of_chain != -1 ) codeblock_label_set( code, end_of_chain, code->current );
                continue;
            }

            if ( token.code == identifier_for ) { /* "FOR" */
                int need_decrement_level = 0;

                if ( dcb_options & DCB_DEBUG ) codeblock_add( code, MN_SENTENCE, line_count + ( current_file << 20 ) );

                loop = codeblock_loop_add( code );
                et1 = codeblock_label_add( code, -1 );
                et2 = codeblock_label_add( code, -1 );

                token_next();
                if ( token.type != IDENTIFIER || token.code != identifier_leftp ) /* "(" */
                    compile_error( MSG_EXPECTED, "(" );

                /* Initializers */
                token_next();
                if ( token.type != IDENTIFIER || token.code != identifier_semicolon ) { /* ";" */
                    int cond = identifier_is_basic_type(token.code);
                    token_back();
                    if ( cond ) {
                        need_decrement_level = 1;
                        incrementCounter(&lvlstk);
                        incrementLevel(&lvlstk);
                        VARSPACE * v[] = {&local, p->pubvars, NULL};
                        compile_varspace( p->privars, p->pridata, 1, 1, v, DEFAULT_ALIGNMENT, 1, 1, 0, 1 );
                    } else {
                        do {
                            compile_expresion( 0, 0, 0, TYPE_UNDEFINED /*TYPE_QWORD*/ );
                            codeblock_add( code, MN_POP, 0 );
                            token_next();
                        }
                        while ( token.type == IDENTIFIER && token.code == identifier_comma ) ; /* "," */
                    }
                }

                if ( token.type != IDENTIFIER || token.code != identifier_semicolon ) /* ";" */
                    compile_error( MSG_EXPECTED, ";" );

                codeblock_label_set( code, et1, code->current );

                /* Conditions */
                token_next();
                if ( token.type != IDENTIFIER || token.code != identifier_semicolon ) { /* ";" */
                    if ( dcb_options & DCB_DEBUG ) codeblock_add( code, MN_SENTENCE, line_count + ( current_file << 20 ) );
                    token_back();
                    do {
                        compile_expresion( 0, 0, 0, TYPE_UNDEFINED /*TYPE_QWORD*/ );
                        codeblock_add( code, MN_BRFALSE, loop );
                        token_next();
                    } while ( token.type == IDENTIFIER && token.code == identifier_comma ); /* "," */
                }

                if ( token.type != IDENTIFIER || token.code != identifier_semicolon ) /* ";" */
                    compile_error( MSG_EXPECTED, ";" );

                codeblock_add( code, MN_JUMP, et2 );

                /* Increments */
                codeblock_loop_start( code, loop, code->current );

                token_next();
                if ( token.type != IDENTIFIER || token.code != identifier_rightp ) { /* ")" */
                    if ( dcb_options & DCB_DEBUG ) codeblock_add( code, MN_SENTENCE, line_count + ( current_file << 20 ) );
                    token_back();
                    do {
                        compile_expresion( 0, 0, 0, TYPE_UNDEFINED /*TYPE_QWORD*/ );
                        codeblock_add( code, MN_POP, 0 );
                        token_next();
                    } while ( token.type == IDENTIFIER && token.code == identifier_comma ) ;  /* "," */
                }

                if ( token.type != IDENTIFIER || token.code != identifier_rightp )  /* ")" */
                    compile_error( MSG_EXPECTED, ")" );

                codeblock_add( code, MN_JUMP, et1 );

                /* Block */
                codeblock_label_set( code, et2, code->current );

                last_loop = code->loop_active;
                code->loop_active = loop;
                compile_block( p );
                code->loop_active = last_loop;

                if ( dcb_options & DCB_DEBUG ) codeblock_add( code, MN_SENTENCE, line_count + ( current_file << 20 ) );

                codeblock_add( code, MN_REPEAT, loop );
                codeblock_loop_end( code, loop, code->current );

                if ( need_decrement_level ) decrementLevel(&lvlstk);
                continue;
            }

            if ( token.code == identifier_switch ) { /* "SWITCH" */
                BASETYPE switch_type = TYPE_UNDEFINED;
                expresion_result switch_exp;

                if ( dcb_options & DCB_DEBUG ) codeblock_add( code, MN_SENTENCE, line_count + ( current_file << 20 ) );

                token_next();
                if ( token.type != IDENTIFIER || token.code != identifier_leftp ) /* "(" */
                    compile_error( MSG_EXPECTED, "(" );

                switch_exp = compile_expresion( 0, 0, 0, TYPE_UNDEFINED );
                switch_type = typedef_base( switch_exp.type );

                if ( switch_type == TYPE_ARRAY && switch_exp.type.chunk[0].type == TYPE_CHAR ) {
                    codeblock_add( code, MN_A2STR, 0 );
                    switch_type = TYPE_STRING;
                } else if ( typedef_is_integer( switch_exp.type ) ) {
                    convert_result_type( switch_exp, TYPE_INT );
                    switch_type = TYPE_INT;
                }
                token_next();

                if ( token.type != IDENTIFIER || token.code != identifier_rightp )  /* ")" */
                    compile_error( MSG_EXPECTED, ")" );

                token_next();
                if ( token.type != IDENTIFIER || ( token.code != identifier_semicolon && token.code != identifier_colon ) ) /* ";" or ":" */
                    token_back();

                if ( switch_type == TYPE_STRING ) codeblock_add( code, MN_SWITCH | MN_STRING, 0 );
                else                              codeblock_add( code, MN_SWITCH, 0 );

                et1 = codeblock_label_add( code, -1 );
                for (;;) {
                    token_next();
                    if ( token.type == IDENTIFIER && token.code == identifier_case ) { /* "CASE" */
                        if ( dcb_options & DCB_DEBUG ) codeblock_add( code, MN_SENTENCE, line_count + ( current_file << 20 ) );

                        for (;;) {
                            token_next();
                            if ( token.type == IDENTIFIER && token.code == identifier_colon ) /* ":" */
                                break;
                            if ( token.type == IDENTIFIER && token.code == identifier_comma ) /* "," */
                                continue;
                            token_back();

                            compile_expresion( 0, 0, 0, switch_type );
                            token_next();
                            if ( token.type == IDENTIFIER && token.code == identifier_twopoints ) { /* ".." */
                                compile_expresion( 0, 0, 0, switch_type );
                                if ( switch_type == TYPE_STRING ) {
                                    codeblock_add( code, MN_CASE_R | MN_STRING, 0 );
                                } else {
                                    codeblock_add( code, MN_CASE_R, 0 );
                                }
                                token_next();
                            } else {
                                if ( switch_type == TYPE_STRING ) {
                                    codeblock_add( code, MN_CASE | MN_STRING, 0 );
                                } else {
                                    codeblock_add( code, MN_CASE, 0 );
                                }
                            }

                            if ( token.type == IDENTIFIER && token.code == identifier_colon ) /* ":" */
                                break;

                            if ( token.type != IDENTIFIER || token.code != identifier_comma ) /* "," */
                                compile_error( MSG_EXPECTED, "," );
                        }
                        et2 = codeblock_label_add( code, -1 );
                        codeblock_add( code, MN_JNOCASE, et2 );
                        compile_block( p );
                        codeblock_add( code, MN_JUMP, et1 );
                        codeblock_label_set( code, et2, code->current );
                    } else if ( token.type == IDENTIFIER && token.code == identifier_default ) { /* "DEFAULT" */
                        if ( dcb_options & DCB_DEBUG ) codeblock_add( code, MN_SENTENCE, line_count + ( current_file << 20 ) );

                        token_next();

                        if ( token.type != IDENTIFIER || token.code != identifier_colon ) /* ":" */
                            compile_error( MSG_EXPECTED, ":" );

                        compile_block( p );
                    }
                    else if ( token.type == IDENTIFIER && token.code == identifier_semicolon )  /* ";" */
                        continue;
                    else if ( token.type == IDENTIFIER && token.code == identifier_end )  /* "END" */
                        break;
                    else
                        compile_error( MSG_EXPECTED, "CASE" );
                }
                codeblock_label_set( code, et1, code->current );
                continue;
            }

            if ( token.code == identifier_loop ) { /* "LOOP" */
                loop = codeblock_loop_add( code );
                codeblock_loop_start( code, loop, code->current );

                if ( dcb_options & DCB_DEBUG ) codeblock_add( code, MN_SENTENCE, line_count + ( current_file << 20 ) );

                last_loop = code->loop_active;
                code->loop_active = loop;
                compile_block( p );
                code->loop_active = last_loop;

                if ( dcb_options & DCB_DEBUG ) codeblock_add( code, MN_SENTENCE, line_count + ( current_file << 20 ) );

                codeblock_add( code, MN_REPEAT, loop );
                codeblock_loop_end( code, loop, code->current );
                continue;
            }

            /* FROM ... TO */

            if ( token.code == identifier_from ) { /* "FROM" */
                int64_t inc = 1;
                CODEBLOCK_POS var_pos;
                CODEBLOCK_POS var_end;
                int is_unsigned = 0, is_float = 0;
                BASETYPE res_type = TYPE_UNDEFINED;

                et1 = codeblock_label_add( code, -1 );

                /* Compile the variable access */

                loop = codeblock_loop_add( code );

                if ( dcb_options & DCB_DEBUG ) codeblock_add( code, MN_SENTENCE, line_count + ( current_file << 20 ) );

                var_pos = codeblock_pos( code );

                res = compile_value();

                res_type = typedef_base( res.type );

                if ( typedef_is_unsigned( res.type ) ) is_unsigned = MN_UNSIGNED;
                if ( typedef_is_double( res.type ) ) is_float = MN_DOUBLE;
                if ( typedef_is_float( res.type ) ) is_float = MN_FLOAT;

                var_end = codeblock_pos( code );

                if ( !res.lvalue ) compile_error( MSG_VARIABLE_REQUIRED );
                if ( !typedef_is_numeric( res.type ) ) compile_error( MSG_NUMBER_REQUIRED );

                /* Compile the assignation of first value */

                token_next();

                if ( token.type != IDENTIFIER || token.code != identifier_equal ) /* "=" */
                    compile_error( MSG_EXPECTED, "=" );

                from = compile_expresion( 0, 0, 0, res_type );
                codeblock_add( code, MN_LETNP | mntype( res.type, 0 ), 0 );

                /* Compile the loop termination check */

                codeblock_label_set( code, et1, code->current );

                codeblock_add_block( code, var_pos, var_end );
                codeblock_add( code, MN_PTR | mntype( res.type, 0 ), 0 );
                token_next();

                if ( token.type != IDENTIFIER || token.code != identifier_to )  /* "TO" */
                    compile_error( MSG_EXPECTED, "TO" );

                tok_pos to_pos = token_pos();

                to = compile_expresion( 0, 0, 0, res_type );

                token_next();

                if ( token.type == IDENTIFIER && token.code == identifier_step ) { /* "STEP" */
                    CODEBLOCK_POS pos = codeblock_pos( code );
                    expresion_result r = compile_expresion( 1, 0, 0, res_type );

                    if ( !r.constant ) compile_error( MSG_CONSTANT_EXP );
                    if ( !typedef_is_numeric( r.type ) ) compile_error( MSG_NUMBER_REQUIRED );

                    if ( res_type == TYPE_DOUBLE || res_type == TYPE_FLOAT )  {
                        if ( r.fvalue == 0.0 ) compile_error( MSG_INVALID_STEP );
                        inc = *( int64_t * ) &r.fvalue;
                    } else {
                        if ( r.value == 0 ) compile_error( MSG_INVALID_STEP );
                        inc = r.value;
                    }

                    codeblock_setpos( code, pos );

                    if ( ( ( res_type == TYPE_FLOAT || res_type == TYPE_DOUBLE ) && r.fvalue > 0 ) || ( ( res_type != TYPE_FLOAT && res_type != TYPE_DOUBLE ) && r.value > 0 ) ) {
                        codeblock_add( code, MN_LTE | is_float | is_unsigned, 0 );
                    }
                    else
                    {
                        codeblock_add( code, MN_GTE | is_float | is_unsigned, 0 );
                    }
                } else {
                    if ( from.constant && to.constant ) {
                        int64_t dec = 0;
                        if ( ( res_type == TYPE_DOUBLE || res_type == TYPE_FLOAT ) && from.fvalue > to.fvalue ) {
                            double v = -1;
                            inc = *(( int64_t * ) &v );
                            dec = 1;
                        } else if ( from.value > to.value ) {
                            inc = -1;
                            dec = 1;
                        }
                        codeblock_add( code, ( dec ? MN_GTE : MN_LTE ) | is_float | is_unsigned, 0 );
                    } else {
                        codeblock_add( code, MN_LTE | is_float | is_unsigned, 0 );
                    }
                    token_back();
                }

                codeblock_add( code, MN_BRFALSE, loop );

                token_next();
                if ( token.type != IDENTIFIER || token.code != identifier_semicolon ) /* ";" */
                    compile_error( MSG_EXPECTED, ";" );

                if ( dcb_options & DCB_DEBUG ) codeblock_add( code, MN_SENTENCE, line_count + ( current_file << 20 ) );

                /* Compile the loop block contents */

                last_loop = code->loop_active;
                code->loop_active = loop;
                compile_block( p );
                code->loop_active = last_loop;

                tok_pos continue_pos = token_pos();

                /* Compile the increment and looping code */

                codeblock_loop_start( code, loop, code->current );

                codeblock_add_block( code, var_pos, var_end );
                codeblock_add( code, MN_PTR | mntype( res.type, 0 ), 0 );

                // Cond 2

                token_set_pos( to_pos );
                to = compile_expresion( 0, 0, 0, res_type );
                token_set_pos( continue_pos );

                codeblock_add( code, ( inc < 0 ? MN_GT : MN_LT ) | is_float | is_unsigned, 0 );

                codeblock_add( code, MN_BRFALSE, loop );

                codeblock_add_block( code, var_pos, var_end );

                     if ( inc ==  1 ) codeblock_add( code, MN_INC | mntype( res.type, 0 ), 1 );
                else if ( inc == -1 ) codeblock_add( code, MN_DEC | mntype( res.type, 0 ), 1 );
                else {
                    codeblock_add( code, MN_PUSH, inc );
                    codeblock_add( code, MN_VARADD | mntype( res.type, 0 ), 0 );
                }

                codeblock_add( code, MN_POP, 0 );
                codeblock_add( code, MN_JUMP, et1 );

                codeblock_loop_end( code, loop, code->current );
                continue;
            }

            /* REPEAT ... UNTIL */

            if ( token.code == identifier_repeat ) { /* "REPEAT" */
                et1 = codeblock_label_add( code, -1 );
                et2 = codeblock_label_add( code, -1 );

                loop = codeblock_loop_add( code );

                codeblock_add( code, MN_JUMP, et1 );

                codeblock_loop_start( code, loop, code->current );

                codeblock_add( code, MN_JUMP, et2 );
                codeblock_label_set( code, et1, code->current );

                if ( dcb_options & DCB_DEBUG ) codeblock_add( code, MN_SENTENCE, line_count + ( current_file << 20 ) );

                last_loop = code->loop_active;
                code->loop_active = loop;
                compile_block( p );
                code->loop_active = last_loop;

                codeblock_label_set( code, et2, code->current );

                if ( dcb_options & DCB_DEBUG ) codeblock_add( code, MN_SENTENCE, line_count + ( current_file << 20 ) );

                if ( token.type != IDENTIFIER || token.code != identifier_until ) /* "UNTIL" */
                    compile_error( MSG_EXPECTED, "UNTIL" );

                token_next();
                if ( token.type == IDENTIFIER && token.code == identifier_leftp ) { /* "(" */
                    compile_expresion( 0, 0, 0, TYPE_QWORD );
                    token_next();
                    if ( token.type != IDENTIFIER || token.code != identifier_rightp ) /* ")" */
                        compile_error( MSG_EXPECTED, ")" );
                } else {
                    token_back();
                    compile_expresion( 0, 0, 0, TYPE_QWORD );
                    token_next();
                    if ( token.type != IDENTIFIER || token.code != identifier_semicolon ) /* ";" */
                        compile_error( MSG_EXPECTED, ";" );
                }
                codeblock_add( code, MN_JFALSE, et1 );
                codeblock_loop_end( code, loop, code->current );
                continue;
            }

            /* WHILE ... END */

            if ( token.code == identifier_while ) { /* "WHILE" */
                token_next();
                if ( token.type != IDENTIFIER || token.code != identifier_leftp ) { /* "(" */
                    token_back();
                    loop = codeblock_loop_add( code );
                    codeblock_loop_start( code, loop, code->current );

                    if ( dcb_options & DCB_DEBUG ) codeblock_add( code, MN_SENTENCE, line_count + ( current_file << 20 ) );

                    compile_expresion( 0, 0, 0, TYPE_QWORD );
                    token_next();
                    if ( token.type != IDENTIFIER || ( token.code != identifier_semicolon && token.code != identifier_colon ) ) /* ";" or ":" */
                        compile_error( MSG_EXPECTED, ";" MSG_OR ":" );
                } else {
                    loop = codeblock_loop_add( code );
                    codeblock_loop_start( code, loop, code->current );

                    if ( dcb_options & DCB_DEBUG ) codeblock_add( code, MN_SENTENCE, line_count + ( current_file << 20 ) );

                    compile_expresion( 0, 0, 0, TYPE_QWORD );
                    token_next();
                    if ( token.type != IDENTIFIER || token.code != identifier_rightp ) /* ")" */
                        compile_error( MSG_EXPECTED, ")" );
                }

                codeblock_add( code, MN_BRFALSE, loop );

                last_loop = code->loop_active;
                code->loop_active = loop;
                compile_block( p );
                code->loop_active = last_loop;

                if ( dcb_options & DCB_DEBUG ) codeblock_add( code, MN_SENTENCE, line_count + ( current_file << 20 ) );

                codeblock_add( code, MN_REPEAT, loop );
                codeblock_loop_end( code, loop, code->current );
                continue;
            }

            if ( token.code == identifier_jmp ) { /* JMP */
                if ( dcb_options & DCB_DEBUG ) codeblock_add( code, MN_SENTENCE, line_count + ( current_file << 20 ) );

                token_next();

                if ( token.type != IDENTIFIER ) compile_error( MSG_INVALID_PARAM );

                if (( codelabel = codeblock_label_get_id_by_name( code, token.code ) ) == -1 )
                    codelabel = codeblock_label_add( code, token.code );

                codeblock_add( code, MN_JUMP, codelabel );

                token_next();
                if ( token.type != IDENTIFIER || token.code != identifier_semicolon ) compile_error( MSG_EXPECTED, ";" );
                continue;
            }

            if ( token.code == identifier_call ) { /* CALL */
                if ( dcb_options & DCB_DEBUG ) codeblock_add( code, MN_SENTENCE, line_count + ( current_file << 20 ) );

                token_next();

                if ( token.type != IDENTIFIER ) compile_error( MSG_INVALID_PARAM );

                if (( codelabel = codeblock_label_get_id_by_name( code, token.code ) ) == -1 )
                    codelabel = codeblock_label_add( code, token.code );

                codeblock_add( code, MN_NCALL, codelabel );

                token_next();
                if ( token.type != IDENTIFIER || token.code != identifier_semicolon ) compile_error( MSG_EXPECTED, ";" );
                continue;
            }

            if ( token.code == identifier_on ) { /* ON EXIT/ON ERROR */
                int64_t on_req = 0;

                if ( dcb_options & DCB_DEBUG ) codeblock_add( code, MN_SENTENCE, line_count + ( current_file << 20 ) );

                token_next();

                if ( token.type != IDENTIFIER || ( token.code != identifier_exit && token.code != identifier_error ) ) compile_error( MSG_ON_PARAM_ERR );

                on_req = token.code;

                token_next();
                if ( token.type != IDENTIFIER || token.code != identifier_jmp ) compile_error( MSG_GOTO_EXP );

                token_next();
                if ( token.type == NUMBER && token.code == 0 )
                    codelabel = -1;
                else if (( codelabel = codeblock_label_get_id_by_name( code, token.code ) ) == -1 )
                    codelabel = codeblock_label_add( code, token.code );

                codeblock_add( code, on_req == identifier_exit ? MN_EXITHNDLR : MN_ERRHNDLR, codelabel );

                token_next();
                if ( token.type != IDENTIFIER || token.code != identifier_semicolon ) compile_error( MSG_EXPECTED, ";" );
                continue;
            }
        }

        if ( token.type == LABEL ||     /* Label */
             ( token.type == IDENTIFIER && ( token.code == identifier_onexit || token.code == identifier_onerror ) ) /* ONEXIT or ONERROR */
           )
        {
            if ( token.code == identifier_onexit ) { /* "ONEXIT" */
                /* The current block is concluded, and all code following is onexit */
                codeblock_add( code, MN_END, 0 );
                p->exitcode = code->current;
            }

            if ( token.code == identifier_onerror ) { /* "ONERROR" */
                /* The current block is concluded, and all code following is onerror */
                codeblock_add( code, MN_END, 0 );
                p->errorcode = code->current;
            }

            if (( codelabel = codeblock_label_get_id_by_name( code, token.code ) ) != -1 ) {
                if ( codeblock_label_get( code, codelabel ) != -1 )
                    compile_error( "Label already defined" );
            } else {
                codelabel = codeblock_label_add( code, token.code );
            }

            codeblock_label_set( code, codelabel, code->current );
            continue;
        }

        if ( token.type != IDENTIFIER ) /* || token.code < reserved_words) */ compile_error( MSG_INVALID_SENTENCE );

        if ( dcb_options & DCB_DEBUG ) codeblock_add( code, MN_SENTENCE, line_count + ( current_file << 20 ) );

        token_back();

        /* Asignation */

        res = compile_subexpresion();
        if ( !res.asignation && !res.call ) compile_error( MSG_INVALID_SENTENCE );
        if ( typedef_is_string( res.type ) && !res.lvalue ) codeblock_add( code, MN_POP | MN_STRING, 0 );
        else                                                codeblock_add( code, MN_POP, 0 );

        if ( compile_sentence_end() ) break;
    }

    decrementLevel(&lvlstk);
}
