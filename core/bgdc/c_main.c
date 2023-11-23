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

#include <loadlib.h> /* Must be first include */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "bgdc.h"

#include "levelstack.h"

#include <bgddl.h>

/* ---------------------------------------------------------------------- */

extern void token_dump();

static unsigned char * import_filename = NULL;
static int64_t import_line = 0;

int64_t nimports_hnd = 0;
void * imports_hnd[512] = { 0 };    /* Modules Handles */
int64_t nimports = 0;
int64_t imports[512] = { 0 } ;      /* Modules */

/* ---------------------------------------------------------------------- */
/* Main module of the compiler. It contains code that initializes known  */
/* identifiers, as well as the top-level compilation functions.           */
/* ---------------------------------------------------------------------- */

/* Types */

int64_t identifier_qword,       identifier_int64,       identifier_double,
        identifier_dword,       identifier_word,        identifier_byte,
        identifier_int32,       identifier_short,       identifier_int8,
        identifier_char,        identifier_dollar,
        identifier_unsigned,    identifier_signed,      identifier_string,
        identifier_float,       identifier_struct,      identifier_type,

        identifier_declare,     identifier_program,     identifier_debug,
        identifier_begin,       identifier_end,
        identifier_global,      identifier_local,       identifier_public,      identifier_private,
        identifier_const,       identifier_dup,         identifier_from,        identifier_step,
        identifier_to,          identifier_if,          identifier_else,        identifier_elseif,
        identifier_for,         identifier_while,       identifier_frame,       identifier_clone,       identifier_yield,

        identifier_onexit,      identifier_onerror,     identifier_on,
        identifier_exit,        identifier_error,       identifier_resume,      identifier_stm_next,

        identifier_jmp,         identifier_call,

        identifier_switch,      identifier_case,        identifier_default,
        identifier_repeat,      identifier_until,
        identifier_loop,        identifier_break,       identifier_continue,
        identifier_return,
        identifier_process,     identifier_function,

        identifier_bandoffset,  identifier_offset,      identifier_sizeof,      identifier_pointer,

        identifier_and,         identifier_or,          identifier_xor,         identifier_not,

        identifier_band,        identifier_bor,         identifier_bxor,        identifier_bnot,

        identifier_plus,        identifier_minus,       identifier_plusplus,    identifier_minusminus,
        identifier_equal,       identifier_multiply,    identifier_mod,         identifier_divide,
        identifier_semicolon,   identifier_colon,       identifier_comma,
        identifier_ror,         identifier_rol,
        identifier_rightp,      identifier_leftp,
        identifier_rightb,      identifier_leftb,
        identifier_point,       identifier_twopoints,

        identifier_eq,          identifier_ne,          identifier_gte,         identifier_lte,
        identifier_lt,          identifier_gt,          identifier_question,

        identifier_plusequal,   identifier_minusequal,  identifier_multequal,   identifier_divequal,
        identifier_modequal,    identifier_orequal,     identifier_xorequal,    identifier_andequal,
        identifier_rorequal,    identifier_rolequal,
        identifier_mouse,
        identifier_include,     identifier_import;

int64_t reserved_words;
int64_t debug = 0;

/* ---------------------------------------------------------------------- */

void compile_init() {

    /* Initialize reserved words */

    identifier_qword        = identifier_add( "QWORD" );
    identifier_dword        = identifier_add( "DWORD" );
    identifier_word         = identifier_add( "WORD" );
    identifier_byte         = identifier_add( "BYTE" );
    identifier_int64        = identifier_add( "INT" );
    identifier_int32        = identifier_add( "INT32" );
    identifier_short        = identifier_add( "SHORT" );
    identifier_int8         = identifier_add( "INT8" );
    identifier_char         = identifier_add( "CHAR" );
    identifier_dollar       = identifier_add( "$");
    identifier_unsigned     = identifier_add( "UNSIGNED" );
    identifier_signed       = identifier_add( "SIGNED" );
    identifier_float        = identifier_add( "FLOAT" );
    identifier_double       = identifier_add( "DOUBLE" );
    identifier_string       = identifier_add( "STRING" );
    identifier_struct       = identifier_add( "STRUCT" );

    identifier_add_as( "BOOL", identifier_byte );
    identifier_add_as( "LONG", identifier_int64 );

    identifier_add_as( "UINT",      identifier_qword );
    identifier_add_as( "UINT32",    identifier_dword );
    identifier_add_as( "UINT16",    identifier_word );
    identifier_add_as( "UINT8",     identifier_byte );

    identifier_add_as( "INT16", identifier_short );
//    identifier_add_as( "INT8",  identifier_char );

    identifier_declare      = identifier_add( "DECLARE" );

    identifier_include      = identifier_add( "INCLUDE" );

    identifier_program      = identifier_add( "PROGRAM" );
    identifier_debug        = identifier_add( "DEBUG" );
    identifier_const        = identifier_add( "CONST" );
    identifier_begin        = identifier_add( "BEGIN" );
    identifier_end          = identifier_add( "END" );
    identifier_process      = identifier_add( "PROCESS" );
    identifier_function     = identifier_add( "FUNCTION" );
    identifier_global       = identifier_add( "GLOBAL" );
    identifier_local        = identifier_add( "LOCAL" );
    identifier_public       = identifier_add( "PUBLIC" );
    identifier_private      = identifier_add( "PRIVATE" );
    identifier_dup          = identifier_add( "DUP" );
    identifier_from         = identifier_add( "FROM" );
    identifier_to           = identifier_add( "TO" );
    identifier_step         = identifier_add( "STEP" );
    identifier_for          = identifier_add( "FOR" );
    identifier_while        = identifier_add( "WHILE" );
    identifier_repeat       = identifier_add( "REPEAT" );
    identifier_until        = identifier_add( "UNTIL" );
    identifier_switch       = identifier_add( "SWITCH" );
    identifier_case         = identifier_add( "CASE" );
    identifier_default      = identifier_add( "DEFAULT" );
    identifier_loop         = identifier_add( "LOOP" );
    identifier_break        = identifier_add( "BREAK" );
    identifier_continue     = identifier_add( "CONTINUE" );
    identifier_return       = identifier_add( "RETURN" );
    identifier_if           = identifier_add( "IF" );
    identifier_else         = identifier_add( "ELSE" );
    identifier_elseif       = identifier_add( "ELSEIF" );
    identifier_frame        = identifier_add( "FRAME" );
    identifier_clone        = identifier_add( "CLONE" );

    identifier_yield        = identifier_add( "YIELD" );

    identifier_onexit       = identifier_add( "ONEXIT" );
    identifier_onerror      = identifier_add( "ONERROR" );

    identifier_on           = identifier_add( "ON" );
    identifier_exit         = identifier_add( "EXIT" );
    identifier_error        = identifier_add( "ERROR" );
    identifier_resume       = identifier_add( "RESUME" );
    identifier_stm_next     = identifier_add( "NEXT" );

    identifier_jmp          = identifier_add( "JMP" );
    identifier_add_as( "GOTO", identifier_jmp );

    identifier_call         = identifier_add( "CALL" );

    identifier_and          = identifier_add( "AND" );
    identifier_or           = identifier_add( "OR" );
    identifier_xor          = identifier_add( "XOR" );
    identifier_not          = identifier_add( "NOT" );

    identifier_band         = identifier_add( "BAND" );
    identifier_bor          = identifier_add( "BOR" );
    identifier_bxor         = identifier_add( "BXOR" );
    identifier_bnot         = identifier_add( "BNOT" );

    identifier_sizeof       = identifier_add( "SIZEOF" );
    identifier_offset       = identifier_add( "OFFSET" );
    identifier_pointer      = identifier_add( "POINTER" );
    identifier_type         = identifier_add( "TYPE" );

    identifier_bandoffset   = identifier_add( "&" );

    identifier_add_as( "!", identifier_not );
    identifier_add_as( "&&", identifier_and );
    identifier_add_as( "||", identifier_or );
    identifier_add_as( "^^", identifier_xor );

    identifier_add_as( "~", identifier_bnot );
    identifier_add_as( "|", identifier_bor );
    identifier_add_as( "^", identifier_bxor );

    identifier_plus         = identifier_add( "+" );
    identifier_minus        = identifier_add( "-" );
    identifier_plusplus     = identifier_add( "++" );
    identifier_minusminus   = identifier_add( "--" );
    identifier_multiply     = identifier_add( "*" );
    identifier_mod          = identifier_add( "%" );
    identifier_divide       = identifier_add( "/" );
    identifier_equal        = identifier_add( "=" );
    identifier_semicolon    = identifier_add( ";" );
    identifier_comma        = identifier_add( "," );
    identifier_ror          = identifier_add( ">>" );
    identifier_rol          = identifier_add( "<<" );
    identifier_rightp       = identifier_add( ")" );
    identifier_leftp        = identifier_add( "(" );
    identifier_rightb       = identifier_add( "]" );
    identifier_leftb        = identifier_add( "[" );
    identifier_point        = identifier_add( "." );
    identifier_twopoints    = identifier_add( ".." );
    identifier_question     = identifier_add( "?" );

    identifier_add_as( "MOD", identifier_mod );
    identifier_add_as( "ELIF", identifier_elseif );
    identifier_add_as( "ELSIF", identifier_elseif );

    identifier_colon        = identifier_add( ":" );

    identifier_eq           = identifier_add( "==" );
    identifier_ne           = identifier_add( "!=" );
    identifier_gt           = identifier_add( ">" );
    identifier_lt           = identifier_add( "<" );
    identifier_gte          = identifier_add( ">=" );
    identifier_lte          = identifier_add( "<=" );

    identifier_add_as( "<>", identifier_ne );
    identifier_add_as( "=>", identifier_gte );
    identifier_add_as( "=<", identifier_lte );

    identifier_plusequal    = identifier_add( "+=" );
    identifier_andequal     = identifier_add( "&=" );
    identifier_xorequal     = identifier_add( "^=" );
    identifier_orequal      = identifier_add( "|=" );
    identifier_divequal     = identifier_add( "/=" );
    identifier_modequal     = identifier_add( "%=" );
    identifier_multequal    = identifier_add( "*=" );
    identifier_minusequal   = identifier_add( "-=" );
    identifier_rorequal     = identifier_add( ">>=" );
    identifier_rolequal     = identifier_add( "<<=" );

    identifier_import       = identifier_add( "IMPORT" );

    reserved_words          = identifier_next_code();

    identifier_mouse        = identifier_add( "MOUSE" ) ; /* Hack */

    varspace_init( &global );
    varspace_init( &local );
    globaldata = segment_new();
    localdata = segment_new();

}

/* ---------------------------------------------------------------------- */

void compile_message( int notoken, int errl, const char *fmt, va_list ap ) {
    char * fname = ( import_filename ) ? import_filename : (( current_file != -1 && files[current_file] && *files[current_file] ) ? files[current_file] : NULL );

    va_list ap_copy;
    va_copy( ap_copy, ap );

    int size = vsnprintf( NULL, 0, fmt, ap );
    if ( size < 0 ) {
        va_end( ap_copy );
        return; // Error in vsprintf
    }

    // Allocate memory for the buffer and format the string
    unsigned char *text = ( char * ) malloc( size + 1 );
    if ( !text ) {
        va_end( ap_copy );
        return; // Error in memory allocation
    }

    vsprintf( text, fmt, ap_copy );
    va_end( ap_copy );

    fprintf( stdout, errl ? MSG_COMPILE_ERROR : MSG_COMPILE_WARNING,
            ( fname && ( fname[0] != '/' && fname[0] != '\\' && fname[1] != ':' ) ) ?  main_path : "",
            fname ? fname : "N/A",
            ( import_filename ) ? import_line : line_count,
            text );
    free( text );
    if ( !notoken ) {
        if ( errl )
            fprintf( stdout, " ( token error: " );
        else
            fprintf( stdout, " ( token warning: " );
        token_dump();
        fprintf( stdout, " )");
    }
    fprintf( stdout, ".\n" );

    if ( errl ) exit( 1 );
}

/* ---------------------------------------------------------------------- */

void compile_warning(int notoken, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    compile_message( notoken, 0, fmt, ap );
    va_end(ap);
}

/* ---------------------------------------------------------------------- */

void compile_error(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    compile_message( 0, 1, fmt, ap );
    va_end(ap);
}

/* ---------------------------------------------------------------------- */

static char * trim( char * ptr ) {
    char * ostr = ptr;
    char * bptr = ptr;
    while ( *ptr == ' ' || *ptr == '\n' || *ptr == '\r' || *ptr == '\t' ) ptr++;
    while ( *ptr ) *bptr++ = *ptr++;
    while ( bptr > ostr && ( bptr[-1] == ' ' || bptr[-1] == '\n' || bptr[-1] == '\r' || bptr[-1] == '\t' ) ) bptr--;
    *bptr = 0;
    return ( ostr );
}

/* ---------------------------------------------------------------------- */

static int import_exists( char * libname ) {
    int n;
    for ( n = 0; n < nimports; n++ ) if ( !strcmp( libname, string_get( imports[n] ) ) ) return n;
    return -1;
}

/* ---------------------------------------------------------------------- */

void compile_type() {
    TYPEDEF    t = typedef_new( TYPE_STRUCT ), *tvalidate;
    VARSPACE * v = varspace_new();
    segment  * s = segment_new();
    int64_t code;

    t.chunk[0].count = 1;

    token_next();
    if (( code = token.code ) < reserved_words || token.type != IDENTIFIER ) compile_error( MSG_INVALID_TYPE );

    tok_pos tokp = token_pos();

    t.varspace = v;

    compile_varspace( v, s, 0, 1, NULL, 0, 1, 0, 1, 0 );

    if ( token.code != identifier_end ) compile_error( MSG_NO_END );

    if ( ( tvalidate = typedef_by_name( code ) ) ) {
        tok_pos curr_pos = token_pos();
        token_set_pos( tokp );
        if ( !typedef_is_equal( t, *tvalidate ) ) {
            compile_error( MSG_PROTO_ERROR );
        } else {
            compile_warning( 0, MSG_TYPE_REDECLARE );            
        }
        token_set_pos( curr_pos );

        varspace_destroy( v );
        segment_destroy( s );
    }
    else
    {
        typedef_name( t, code );
        segment_name( s, code );
    }
}

/* ---------------------------------------------------------------------- */

static char * modules_exts[] = {
    ".dll",
    ".dylib",
    ".so",
    NULL
};

static char * dlsearchpath[] = {
    "modules",
    "mod",
    "mods",
    "extensions",
    "plugins",
    "data",
    NULL
};

static void import_module( const char * filename ) {
    int         libid;
    void        * library = NULL;
    char        ** globals_def = NULL;
    char        ** locals_def = NULL;
    DLCONSTANT  * constants_def = NULL;
    DLSYSFUNCS  * functions_exports = NULL;
    char        ** modules_dependency = NULL;
    char        ** types_def = NULL;

    char        soname[ __MAX_PATH ], fullsoname[ __MAX_PATH + 2 ], **spath;
    char        * ptr;
    char        ** pex;

#if defined( WIN32 )
#define DLLEXT      ".dll"
#elif defined(TARGET_MAC)
#define DLLEXT      ".dylib"
#else
#define DLLEXT      ".so"
#endif

    strncpy( soname, filename, sizeof( soname ) );

    for ( ptr = soname; *ptr; ptr++ ) {
        if ( *ptr == PATH_CHAR_SEP ) *ptr = PATH_CHAR_ISEP;
        else *ptr = TOLOWER( *ptr );
    }

    pex = modules_exts;
    while ( pex && * pex ) {
        int nlen = strlen( soname );
        int elen = strlen( *pex );
        if ( nlen > elen && strcmp( &soname[nlen - elen], *pex ) == 0 ) {
            soname[nlen - elen] = '\0';
            pex = modules_exts;
        } else {
            pex++;
        }
    }

    if ( import_exists(( char * )soname ) != -1 ) return;

    filename = soname;
    libid = string_new( soname );

    imports[nimports++] = libid;

    strcat( soname, DLLEXT );

    fullsoname[0] = '\0';

    spath = dlsearchpath;
    while( !library && spath && *spath ) {
        snprintf( fullsoname, sizeof( fullsoname ), "%s%s/%s", appexepath, *spath, filename );
        library  = dlibopen( fullsoname );
        spath++;
    }

    if ( !library ) library  = dlibopen( filename );

    if ( !library ) compile_error( MSG_LIBRARY_NOT_FOUND, filename );


    imports_hnd[nimports_hnd++] = library;

    modules_dependency = ( char ** ) _dlibaddr( library, "modules_dependency" );
    if ( modules_dependency ) {
        char * old_import_filename = import_filename;
        while ( *modules_dependency ) {
            if ( import_exists( *modules_dependency ) == -1 ) {
                import_filename = *modules_dependency;
                import_module( *modules_dependency );
                import_filename = NULL;
            }
            modules_dependency++;
        }
        import_filename = old_import_filename;
    }

    constants_def = ( DLCONSTANT * ) _dlibaddr( library, "constants_def" );
    if ( constants_def ) {
        while ( constants_def->name ) {
            int code = identifier_search_or_add( constants_def->name );
            constants_add( code, typedef_new( constants_def->type ), constants_def->code );
            constants_def++;
        }
    }

    types_def = ( char ** ) _dlibaddr( library, "types_def" );
    if ( types_def && *types_def ) {
        token_init( *types_def, -1 );
        token_next();
        while ( token.type == IDENTIFIER && token.code == identifier_type ) {
            compile_type();
            token_next();
        }
    }

    globals_def = ( char ** ) _dlibaddr( library, "globals_def" );
    if ( globals_def && *globals_def ) {
        VARSPACE * v[] = {&local, NULL};
        token_init( *globals_def, -1 );
        compile_varspace( &global, globaldata, 1, 1, v, DEFAULT_ALIGNMENT, 1, 0, 0, 0 );
    }

    locals_def  = ( char ** ) _dlibaddr( library, "locals_def" );
    if ( locals_def && *locals_def ) {
        VARSPACE * v[] = {&global, NULL};
        token_init( *locals_def, -1 );
        compile_varspace( &local, localdata, 1, 1, v, DEFAULT_ALIGNMENT, 1, 0, 0, 0 );
    }

    functions_exports = ( DLSYSFUNCS * ) _dlibaddr( library, "functions_exports" );
    if ( functions_exports ) {
        while ( functions_exports->name ) {
            sysproc_add( functions_exports->name, functions_exports->paramtypes, functions_exports->type, functions_exports->func );
            functions_exports++;
        }
    }
}

/* ---------------------------------------------------------------------- */

void import_files( char * filename ) {
    file * fp = file_open( filename, "rb0" );
    char libname[__MAX_PATH];

    if ( !fp ) return;

    import_line = 0;

    while ( !file_eof( fp ) ) {
        import_line++;
        if ( !file_gets( fp, libname, sizeof( libname ) ) ) break;
        trim( libname );
        if ( *libname == '\0' ) continue;
        if ( import_exists( libname ) != -1 ) continue;
        import_filename = filename;
        import_module( libname );
        import_filename = NULL;
    }

    file_close( fp );
}

/* ---------------------------------------------------------------------- */

void import_mod( char * libname ) {
    import_line = 0;
    if ( import_exists( libname ) != -1 ) return;
    import_filename = libname;
    import_module( libname );
    import_filename = NULL;
}

/* ---------------------------------------------------------------------- */

void free_imports() {
    while( nimports_hnd-- ) {
        dlibclose( imports_hnd[ nimports_hnd ] );
    }
}

/* ---------------------------------------------------------------------- */

void compile_import( void ) {
    no_include_this_file = 1;

    token_next();
    if ( token.type != STRING ) compile_error( MSG_STRING_EXP );

    import_module( string_get( token.code ) );
}

/* ---------------------------------------------------------------------- */

void compile_constants() {
    int64_t code;
    expresion_result res;

    for ( ;; ) {
        token_next();
        if ( token.type == NOTOKEN ) break;

        if ( token.type != IDENTIFIER ) compile_error( MSG_CONSTANT_NAME_EXP );

        if ( token.code == identifier_semicolon ) continue;

        if ( token.code == identifier_begin ||
             token.code == identifier_local ||
             token.code == identifier_public ||
             token.code == identifier_private ||
             token.code == identifier_global ||
             identifier_is_basic_type( token.code ) ||
             token.code == identifier_struct ||
             procdef_search( token.code )
          )
        {
            token_back();
            return;
        }

        if ( token.code == identifier_end ) return;

        if ( token.code < reserved_words ) compile_error( MSG_INVALID_IDENTIFIER );

        code = token.code;

        token_next();
        if ( token.type != IDENTIFIER || token.code != identifier_equal ) compile_error( MSG_EXPECTED, "=" );

        res = compile_expresion( 1, 0, 0, 0 );
        if ( !typedef_is_double( res.type ) && !typedef_is_float( res.type ) && !typedef_is_string( res.type ) && !typedef_is_integer( res.type ) ) compile_error( MSG_INVALID_TYPE );

        constants_add( code, res.type, typedef_base( res.type ) == TYPE_DOUBLE ? *( int64_t * ) &res.fvalue : res.value );

        token_next();
        if ( token.type == IDENTIFIER && token.code == identifier_semicolon ) continue;

        compile_error( MSG_EXPECTED, ";" );
    }
}

/* ---------------------------------------------------------------------- */

LevelStack lvlstk = { .top = -1 } ;

void compile_process() {
    PROCDEF * proc, * external_proc = NULL, *e;
    VARIABLE  * var;
    int is_declare = 0, is_function = 0, type_implicit = 1;

    BASETYPE type, type_last;
    TYPEDEF ctype, ctype_last;

    int sign_prefix = 0;

    /* Is this a declaration? */
    if ( token.code == identifier_declare ) {  
        is_declare = 1;
        token_next();
    }

    /* Is this a function? */
    if ( token.code == identifier_function ) {
        is_function = 1;
        token_next();
    }
    else
    if ( token.code == identifier_process ) {
        token_next();
    }

    BASETYPE return_type = TYPE_INT;

    /* "Sign" */
    if ( token.code == identifier_unsigned ) {
        sign_prefix = 1;
        token_next();
    } else if ( token.code == identifier_signed ) {
        sign_prefix = -1;
        token_next();
    }

    if ( identifier_is_basic_type(token.code) ) {
        return_type = typedef_basic_type_basetype_by_name( token.code, sign_prefix );
        if ( return_type == TYPE_UNDEFINED ) compile_error( MSG_INVALID_TYPE );
        token_next();
        sign_prefix = 0;
    }
    else
    if ( token.type == IDENTIFIER && segment_by_name( token.code ) ) {
        return_type = TYPE_POINTER;
        token_next();
        if ( token.code == identifier_leftp ) {
            token_back();
            compile_error( MSG_NAME_ALREADY_USED_AS_TYPE );
        }
    }
    else
    if ( procdef_search( token.code ) ) { /* Process-type variables */
        return_type = TYPE_INT;
        token_next();
        if ( token.code == identifier_leftp ) token_back();
    }

    while ( token.type == IDENTIFIER && ( token.code == identifier_pointer || token.code == identifier_multiply ) ) {
        return_type = TYPE_POINTER;
        token_next();
    }

    /* Check if the process name is valid */
    if ( token.type != IDENTIFIER || token.code < reserved_words ) compile_error( MSG_PROCESS_NAME_EXP );

    if ( segment_by_name( token.code ) ) compile_error( MSG_NAME_ALREADY_USED_AS_TYPE );

    /* Create the process if it is not defined already */
    if ( !( proc = procdef_search( token.code ) ) ) proc = procdef_new( procdef_getid(), token.code );

    if ( !proc ) compile_error( MSG_CANT_CREATE_PROCESS_FUNCTION );

    if ( proc->defined ) {
        if ( !is_declare ) compile_error( MSG_PROC_ALREADY_DEFINED );
        if ( proc->type != return_type ) compile_error( MSG_PROTO_ERROR );
    }

    /* is a function? */
    if (is_function) {
        if ( is_declare && proc->declared && !( proc->flags & PROC_FUNCTION ) ) compile_error( MSG_PROTO_ERROR );
        proc->flags |= PROC_FUNCTION;
    }

    proc->type = return_type;

    /* Parse the process parameters */

    token_next();
    if ( token.type != IDENTIFIER || token.code != identifier_leftp ) compile_error( MSG_EXPECTED, "(" );

    token_next();

    int params = 0, minparams = -1;
    type = TYPE_INT;
    type_last = TYPE_INT;
    type_implicit = 1;
    ctype = typedef_new( type );
    ctype_last = ctype;
    sign_prefix = 0;
    external_proc = NULL;

    while ( token.type != IDENTIFIER || token.code != identifier_rightp ) {
        type = type_last;
        ctype = ctype_last;

        /* "Sign" */
        if ( token.code == identifier_unsigned ) {
            sign_prefix = 1;
            token_next();
        } else if ( token.code == identifier_signed ) {
            sign_prefix = -1;
            token_next();
        }

        if ( token.type == IDENTIFIER && identifier_is_basic_type(token.code) ) {
            ctype = typedef_basic_type_by_name( token.code, sign_prefix );
            type = typedef_base( ctype );
            if ( typedef_is_undefined( ctype ) ) compile_error( MSG_INVALID_TYPE );
            token_next();
            type_implicit = 0;
            external_proc = NULL;
            sign_prefix = 0;
        }
        else if ( token.type == IDENTIFIER && segment_by_name( token.code ) ) {
            type = TYPE_STRUCT;
            ctype = *typedef_by_name( token.code ) ;
            token_next();
        }
        else if ( ( e = procdef_search( token.code ) ) ) { /* Process-type variables */
            external_proc = e;
            type_implicit = 0;
            type = TYPE_INT;
            ctype = typedef_new( type );
            token_next();
        }

        type_last = type;
        ctype_last = ctype;

        while ( token.type == IDENTIFIER && ( token.code == identifier_pointer || token.code == identifier_multiply ) ) {
            type_implicit = 0;
            type = TYPE_POINTER;
            ctype = typedef_pointer( ctype );
            token_next();
        }

        if ( type == TYPE_STRUCT ) {
            type_implicit = 0;
            type = TYPE_POINTER;
            ctype = typedef_pointer( ctype );
        }

        if ( token.type != IDENTIFIER || token.code < reserved_words ) compile_error( MSG_INVALID_PARAM );

        if ( constants_search( token.code ) ) compile_error( MSG_CONSTANT_REDECLARED_AS_VARIABLE );

        /* Check if the process was used before declared */
        if ( !proc->declared ) {
            var = varspace_search( &local, token.code );
            if ( var ) {
                /* The parameter is actually a local variable */
                if ( type_implicit ) {
                    type = typedef_base( var->type );
                    ctype = var->type;
                }

                if ( typedef_base( var->type ) != type ) {
                    if ( typedef_is_integer( var->type ) && typedef_is_integer( typedef_new( type ) ) ) {
                        /*
                            A parameter was used before the process is declared. The
                            data type declared is different from the data type used,
                            but both are integers. The error is ignored, but no
                            conversion is done. This can lead to type conversion issues.
                        */
                    } else
                        compile_error( MSG_INVALID_PARAMT );
                }
                codeblock_add( &proc->code, MN_LOCAL, var->offset );
                codeblock_add( &proc->code, MN_PRIVATE, proc->pridata->current );
                codeblock_add( &proc->code, MN_PTR, 0 );

                if ( typedef_base( var->type ) == TYPE_STRING ) codeblock_add( &proc->code, MN_LET | MN_STRING, 0 );
                else codeblock_add( &proc->code, MN_LET, 0 );

                codeblock_add( &proc->code, MN_POP, 0 );

                if ( proc->privars->reserved == proc->privars->count ) varspace_alloc( proc->privars, 16 );

                proc->privars->vars[proc->privars->count].type   = ctype;
                proc->privars->vars[proc->privars->count].offset = proc->pridata->current;
                proc->privars->vars[proc->privars->count].code   = -1; // I set code to -1 to prevent the variable from being found in a remote access
                                                                       // (since it's a private variable to the process).
                                                                       // Those parameters that are public take on the initial value (the argument's value)
                                                                       // of the private variable.

                proc->privars->count++;

                segment_add_qword( proc->pridata, 0 );
            } else {
                var = varspace_search( proc->privars, token.code );
                if ( var ) {
                    compile_error(MSG_VARIABLE_REDECLARE);
                }

                /* Create the private variable */
                if ( proc->privars->reserved == proc->privars->count ) varspace_alloc( proc->privars, 16 );

                if ( type == TYPE_STRING ) varspace_varstring( proc->privars, proc->pridata->current );

                proc->privars->vars[proc->privars->count].type   = ctype;
                proc->privars->vars[proc->privars->count].offset = proc->pridata->current;
                proc->privars->vars[proc->privars->count].code   = token.code;
                if ( external_proc ) proc->privars->vars[proc->privars->count].type.varspace = external_proc->pubvars;

                proc->privars->count++;

                segment_add_qword( proc->pridata, 0 );
            }
        } else {
            if ( proc->paramtype[params] != type || proc->paramname[params] != token.code ) compile_error( MSG_PROTO_ERROR );
        }

        if ( proc->params != -1 ) {
            /* The process was used previously */
            if ( proc->paramtype[params] == TYPE_UNDEFINED ) proc->paramtype[params] = type;
            else if (( proc->paramtype[params] == TYPE_QWORD || proc->paramtype[params] == TYPE_INT ) &&
                     (  type == TYPE_QWORD ||
                        type == TYPE_INT   ||
                        type == TYPE_DWORD ||
                        type == TYPE_INT32 ||
                        type == TYPE_BYTE  ||
                        type == TYPE_WORD  ||
                        type == TYPE_SHORT ||
                        type == TYPE_SBYTE
                     ) ) proc->paramtype[params] = type;
            else if ( type == TYPE_QWORD && ( proc->paramtype[params] == TYPE_BYTE || proc->paramtype[params] == TYPE_WORD || proc->paramtype[params] == TYPE_DWORD ) ) proc->paramtype[params] = type;
            else if ( proc->paramtype[params] != type ) compile_error( MSG_INVALID_PARAMT );
        } else
            proc->paramtype[params] = type;

        proc->paramname[params] = token.code;

        token_next();

        if ( token.type == IDENTIFIER && token.code == identifier_equal ) {
            expresion_result res = compile_expresion( 1, 0, 1, type );

            if ( type == TYPE_DOUBLE ) {
                double d = res.fvalue;
                if ( proc->minparams != -1 && proc->minparams <= params && proc->paramfvalue[params] != d ) compile_error( MSG_PROTO_ERROR );
                proc->paramfvalue[params] = d;
            }
            else
            if ( type == TYPE_FLOAT ) {
                float f = ( float ) res.fvalue;
                if ( proc->minparams != -1 && proc->minparams <= params && proc->paramfvalue[params] != f ) compile_error( MSG_PROTO_ERROR );
                proc->paramfvalue[params] = f;
            }
            else
            {
                if ( proc->minparams != -1 && proc->minparams <= params && proc->paramivalue[params] != res.value ) compile_error( MSG_PROTO_ERROR );
                proc->paramivalue[params] = res.value;
            }

            if ( minparams == -1 ) minparams = params;

            token_next();
        }
        else
        if ( ( proc->minparams == -1 || proc->minparams > params ) && minparams != -1 && minparams < params ) compile_error( MSG_DEFAULT_REQUIRED );

        params++;

        if ( params == MAX_PARAMS ) compile_error( MSG_TOO_MANY_PARAMS );

        if ( token.type == IDENTIFIER ) {
            if ( token.code != identifier_rightp && token.code != identifier_comma ) compile_error( MSG_EXPECTED, "," );
            if ( token.code == identifier_comma ) token_next();
        }

    } /* END while (token.type != IDENTIFIER || token.code != identifier_rightp) */

    if ( proc->minparams == -1 || proc->minparams > minparams ) proc->minparams = minparams;
    if ( proc->params == -1 ) proc->params = params;
    else if ( proc->params != params ) compile_error( MSG_INCORRECT_PARAMC, identifier_name( proc->identifier ), proc->params );

    token_next();

    if ( !is_declare && token.type == IDENTIFIER && token.code == identifier_semicolon ) token_next();

    int wait_for_end = !is_declare;
    int wait_semicolon = is_declare;

    /* Compile LOCAL/PRIVATE/PUBLIC sections on process/function.
       NOTE: LOCAL section here considere as PUBLIC section */

    while ( token.type == IDENTIFIER && (
            token.code == identifier_local  ||
            token.code == identifier_public ||
            token.code == identifier_private ) )
    {
        wait_semicolon = 0;
        wait_for_end = 0;
        if ( ( token.code == identifier_local || token.code == identifier_public ) ) {
            /* Local declarations are only local to the process but visible from every process */
            /* It is allowed to declare a variable as local/public that has been declared as global; it's a local variable, not the global one */
            VARSPACE * v[] = {&local, proc->privars, NULL};
            compile_varspace( proc->pubvars, proc->pubdata, 1, 1, v, DEFAULT_ALIGNMENT, 1, 0, 0, 0 );
        }
        else
        if ( token.code == identifier_private ) {
            /* It is allowed to declare a variable as private that has been declared as global; it's a private variable, not the global one */
            VARSPACE * v[] = {&local, proc->pubvars, NULL};
            compile_varspace( proc->privars, proc->pridata, 1, 1, v, DEFAULT_ALIGNMENT, 1, 0, 0, 0 );
        }

        token_next();
    }

    /* Manages processes whose parameters are local variables */

    if ( !is_declare ) {
        if ( token.type != IDENTIFIER || token.code != identifier_begin ) compile_error( MSG_NO_BEGIN );

        initStack(&lvlstk);

        compile_block( proc );

        if ( getStackLevel(&lvlstk) != -1 ) compile_error( "INTERNAL ERROR" );

        if ( token.code == identifier_else ) compile_error( MSG_ELSE_WOUT_IF );
        
        if ( token.code != identifier_end ) compile_error( MSG_NO_END );
        
        codeblock_add( &proc->code, MN_END, 0 );

    } else {
        if ( !wait_semicolon && !wait_for_end && token.code != identifier_semicolon && token.code != identifier_end ) token_back();
        if ( wait_for_end && token.code != identifier_end ) compile_error( MSG_NO_END );
        if ( wait_semicolon && token.code != identifier_semicolon && token.code != identifier_end ) compile_error( MSG_EXPECTED, "; or END" );
    }

    if ( !is_declare ) proc->defined = 1;
    proc->declared = 1;

}

/* ---------------------------------------------------------------------- */

void compile_program() {
    token_next();
    if ( token.type == IDENTIFIER && token.code == identifier_program ) {
        token_next();
        if ( token.type != IDENTIFIER || token.code < reserved_words ) compile_error( MSG_PROGRAM_NAME_EXP );

        token_next();
        if ( token.type != IDENTIFIER || token.code != identifier_semicolon ) compile_error( MSG_EXPECTED, ";" );
    } else
        token_back();

    for ( ;; ) {
        int block_var = 0;
        token_next();

        while ( token.type == IDENTIFIER && token.code == identifier_semicolon ) token_next();

             if ( token.type == IDENTIFIER && token.code == identifier_import ) compile_import();
        else if ( token.type == IDENTIFIER && token.code == identifier_const ) compile_constants();
        else if ( token.type == IDENTIFIER && token.code == identifier_local ) {
            VARSPACE * v[] = { &global, NULL };
            compile_varspace( &local, localdata, 1, 1, v, DEFAULT_ALIGNMENT, 1, 0, 0, 0 );
        } else if ( token.type == IDENTIFIER && (
                        token.code == identifier_global ||
                        ( block_var = ( identifier_is_basic_type( token.code ) || token.code == identifier_struct || procdef_search( token.code ) || segment_by_name( token.code ) ) )
                     ) ) {
            VARSPACE * v[] = { &local, NULL };
            if ( block_var ) token_back();
            compile_varspace( &global, globaldata, 1, 1, v, DEFAULT_ALIGNMENT, 1, block_var != 0, 0, 0 );
        } else if ( token.type == IDENTIFIER && token.code == identifier_private ) {
            /* It is allowed to declare a variable as private that has been declared as global; it's a private variable, not the global one */
            VARSPACE * v[] = { &local, NULL };
            compile_varspace( mainproc->privars, mainproc->pridata, 1, 1, v, DEFAULT_ALIGNMENT, 1, 0, 0, 0 );
        } else if ( token.type == IDENTIFIER && token.code == identifier_begin ) {
            if ( mainproc->defined ) {
                /* Hack to redefine the main process */
                mainproc->code.current -= 1;
            }

            mainproc->defined = 1;

            initStack(&lvlstk);

            compile_block( mainproc );

            if ( getStackLevel(&lvlstk) != -1 ) compile_error( "INTERNAL ERROR" );

            if ( token.type == IDENTIFIER && token.code == identifier_else ) compile_error( MSG_ELSE_WOUT_IF );

            if ( token.type != IDENTIFIER || token.code != identifier_end ) compile_error( MSG_NO_END );

            codeblock_add( &mainproc->code, MN_END, 0 );

        } else if ( token.type == IDENTIFIER && token.code == identifier_type ) { /* User-defined data type */
            compile_type();
        } else if ( token.type == IDENTIFIER &&
                        ( token.code == identifier_process  ||
                          token.code == identifier_function ||
                          token.code == identifier_declare  ||
                          identifier_is_basic_type( token.type )
                        )
                   ) {
            compile_process() ; /* Process definition */
        } else if ( segment_by_name( token.code ) ) {
            compile_process();
        } else
            break;
    }

    if ( global.count && debug > 1 ) {
        printf( "\n---- Global variables\n\n" );
        varspace_dump( &global, 0 );
    }

    if ( local.count && debug > 1 ) {
        printf( "\n---- Local variables\n\n" );
        varspace_dump( &local, 0 );
        /* segment_dump (localdata) ; */
    }

    if ( token.type != NOTOKEN ) compile_error( MSG_UNEXPECTED_TOKEN );

    program_postprocess();

    if ( debug > 1 ) program_dumpprocesses();

    if ( !libmode && !mainproc->defined ) compile_error( MSG_NO_MAIN );

    if ( debug > 2 ) {
        identifier_dump();
        constants_dump();
        string_dump( NULL );
    }
}

/* ---------------------------------------------------------------------- */
