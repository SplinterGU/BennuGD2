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

/* --------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "bgdrtm.h"
#include "bgddl.h"

#include "xstrings.h"

#include "libbggfx.h"
#include "libmod_debug.h"
#include "libbginput.h"

#include "dlvaracc.h"

#include <SDL.h>

/* --------------------------------------------------------------------------- */

#ifndef _vsnprintf
#define _vsnprintf vsnprintf
#endif

#ifndef _snprintf
#define _snprintf snprintf
#endif

/* --------------------------------------------------------------------------- */

#define MAXTEXT         5000

#define CHARWIDTH       6
#define CHARHEIGHT      8

#define CONSOLE_HISTORY 512
#define CONSOLE_LINES   25
#define CONSOLE_COLUMNS 80
#define COMMAND_HISTORY 128

#define MAX_EXPRESSIONS 128

/* --------------------------------------------------------------------------- */

#define HELPTXT \
    COLOR_GREEN "Process Info" COLOR_SILVER "\n"                                                        \
    COLOR_TEAL "INSTANCES       " COLOR_SILVER "  List all running processes\n"                         \
    COLOR_TEAL "GLOBALS         " COLOR_SILVER "  Show global vars with values\n"                       \
    COLOR_TEAL "LOCALS proc     " COLOR_SILVER "  Show a process's local vars\n"                        \
    COLOR_TEAL "PRIVATES proc   " COLOR_SILVER "  Show a process's private vars\n"                      \
    COLOR_TEAL "PUBLICS proc    " COLOR_SILVER "  Show a process's public vars\n"                       \
    "\n"                                                                                                \
    COLOR_GREEN "Execution Commands" COLOR_SILVER "\n"                                                  \
    COLOR_TEAL "GO              " COLOR_SILVER "  Continue the execution\n"                             \
    COLOR_TEAL "TRACE           " COLOR_SILVER "  Execute one instruction and Debug\n"                  \
    COLOR_TEAL "STEP            " COLOR_SILVER "  Execute one instruction/process/function and Debug\n" \
    COLOR_TEAL "NEXTFRAME       " COLOR_SILVER "  Continue to next frame\n"                             \
    COLOR_TEAL "NEXTPROC        " COLOR_SILVER "  Continue to next process\n"                           \
    "\n"                                                                                                \
    COLOR_GREEN "Breakpoints" COLOR_SILVER "\n"                                                         \
    COLOR_TEAL "BREAK           " COLOR_SILVER "  List breakpoints\n"                                   \
    COLOR_TEAL "BREAK proc      " COLOR_SILVER "  Set a breakpoint on process\n"                        \
    COLOR_TEAL "BREAKALL        " COLOR_SILVER "  Set breakpoints on all processes\n"                   \
    COLOR_TEAL "BREAKALLTYPES   " COLOR_SILVER "  Set breakpoints on all processes types\n"             \
    COLOR_TEAL "DELETE proc     " COLOR_SILVER "  Delete a breakpoint\n"                                \
    COLOR_TEAL "DELETEALL       " COLOR_SILVER "  Delete all breakpoints on all processess\n"           \
    COLOR_TEAL "DELETEALLTYPES  " COLOR_SILVER "  Delete all breakpoints on all processess types\n"     \
    "\n"                                                                                                \
    COLOR_GREEN "Process Interaction" COLOR_SILVER "\n"                                                 \
    COLOR_TEAL "RUN proc [args] " COLOR_SILVER "  Run a process\n"                                      \
    COLOR_TEAL "KILL proc       " COLOR_SILVER "  Kill a process\n"                                     \
    COLOR_TEAL "WAKEUP proc     " COLOR_SILVER "  Wakeup a process\n"                                   \
    COLOR_TEAL "SLEEP proc      " COLOR_SILVER "  Sleep a process\n"                                    \
    COLOR_TEAL "FREEZE proc     " COLOR_SILVER "  Freeze a process\n"                                   \
    COLOR_TEAL "KILLALL proc    " COLOR_SILVER "  Kill all process with criteria\n"                     \
    COLOR_TEAL "WAKEUPALL proc  " COLOR_SILVER "  Wakeup all process with criteria\n"                   \
    COLOR_TEAL "SLEEPALL proc   " COLOR_SILVER "  Sleep all process with criteria\n"                    \
    COLOR_TEAL "FREEZEALL proc  " COLOR_SILVER "  Freeze all process with criteria\n"                   \
    "\n"                                                                                                \
    COLOR_GREEN "Misc" COLOR_SILVER "\n"                                                                \
    COLOR_TEAL "SHOW expression " COLOR_SILVER "  Evaluate and show some expression\n"                  \
    COLOR_TEAL "SHOW            " COLOR_SILVER "  List all expressions to show\n"                       \
    COLOR_TEAL "SHOWDEL ExpID   " COLOR_SILVER "  Delete an expression (by ID, use SHOW)\n"             \
    COLOR_TEAL "SHOWDELALL      " COLOR_SILVER "  Delete all expression\n"                              \
    COLOR_TEAL "STRINGS         " COLOR_SILVER "  Show all strings in memory\n"                         \
    COLOR_TEAL "VARS            " COLOR_SILVER "  Show internals vars\n"                                \
    COLOR_TEAL "QUIT            " COLOR_SILVER "  Kill the program and exit\n"                          \
    "\n"                                                                                                \
    COLOR_GREEN "Keys" COLOR_SILVER "\n"                                                                \
    COLOR_TEAL "ESC             " COLOR_SILVER "  Cancel command\n"                                     \
    COLOR_TEAL "UP/DOWN         " COLOR_SILVER "  Command history navigation\n"                         \
    COLOR_TEAL "PGUP/PGDN       " COLOR_SILVER "  Page Up/Page Down\n"                                  \
    COLOR_TEAL "CTRL+CURSORS    " COLOR_SILVER "  Console scroll\n"                                     \
    COLOR_TEAL "ALT+CURSORS     " COLOR_SILVER "  Console window size\n"                                \
    COLOR_TEAL "SHIFT+CURSORS   " COLOR_SILVER "  List window scroll\n"                                 \
    "\n"                                                                                                \
    "You can evaluate free expressions in the console, and you can see/change\n"                        \
    "local, public and private vars using the '.' operator\n"                                           \
    "(pe: 65535.X, MAIN.X, etc.)\n\n"

/* --------------------------------------------------------------------------- */

#define HOTKEYHELP_SIZE 60

#define HOTKEYHELP1  COLOR_NAVY "F1:" COLOR_BLACK "?\x03" COLOR_NAVY "F2:" COLOR_BLACK "Procs\x03" COLOR_NAVY "F5:" COLOR_BLACK "Go\x03" COLOR_NAVY "F7:" COLOR_BLACK "Trace\x03" COLOR_NAVY "F8:" COLOR_BLACK "Step\x03" COLOR_NAVY "F10:" COLOR_BLACK "NFrame\x03" COLOR_NAVY "F11:" COLOR_BLACK "NProc"
#define HOTKEYHELP2  COLOR_NAVY "F1:" COLOR_BLACK "?\x03" COLOR_NAVY "F2:" COLOR_BLACK "Brief\x03" COLOR_NAVY "F6:" COLOR_BLACK "Procs\x03" COLOR_NAVY "F9:" COLOR_BLACK "Break"
#define HOTKEYHELP3  COLOR_NAVY "F1:" COLOR_BLACK "?\x03" COLOR_NAVY "F2:" COLOR_BLACK "Brief\x03" COLOR_NAVY "F3:" COLOR_BLACK "Loc\x03" COLOR_NAVY "F4:" COLOR_BLACK "Pri\x03" COLOR_NAVY "F5:" COLOR_BLACK "Pub\x03" COLOR_NAVY "F6:" COLOR_BLACK "Types\x03" COLOR_NAVY "F9:" COLOR_BLACK "Brk"

/* --------------------------------------------------------------------------- */

#define N_CONSOLE_VARS (sizeof(console_vars)/sizeof(console_vars[0]))

#define CON_QWORD               0x0001
#define CON_DWORD               0x0002
#define CON_WORD                0x0004
#define CON_BYTE                0x0008
#define CON_QWORD_HEX           0x8001
#define CON_DWORD_HEX           0x8002
#define CON_WORD_HEX            0x8004
#define CON_BYTE_HEX            0x8008

/* --------------------------------------------------------------------------- */

static struct {
    enum { RT_ERROR, RT_VARIABLE, RT_CONSTANT, RT_CONSTANT_DOUBLE, RT_STRING } type ;
    DCB_VAR var ;
    struct {
        int64_t int_value;
        double double_value;
    };
    void * data ;
    char name[1024] ;
} result, lvalue ;

static struct {
    enum { IDENTIFIER, STRING, NUMBER_INT, NUMBER_DOUBLE, OPERATOR, NOTOKEN } type ;
    char name[1024] ;
    struct {
        int64_t code;
        double double_value;
    };
} token ;

static const char * token_ptr ;

/* --------------------------------------------------------------------------- */

/* Console contents */
static int console_lines = CONSOLE_LINES ;
static int console_columns = CONSOLE_COLUMNS ;

static char * console[CONSOLE_HISTORY] ;
static int console_initialized = 0 ;
static int console_head = 0 ;
static int console_tail = 0 ;

static char * command[COMMAND_HISTORY] ;
static int command_initialized = 0 ;
static int command_head = 0 ;
static int command_tail = 0 ;
static int command_count = 0 ;

static char * show_expression[MAX_EXPRESSIONS] = { NULL };
static int show_expression_count = 0;
static int32_t console_showcolor = 0xffffff;
static uint8_t console_alpha = 224;

static int console_y = 0 ;

/* --------------------------------------------------------------------------- */

static int debug_on_frame = 0;
static int break_on_next_proc = 0;

/* --------------------------------------------------------------------------- */

static struct {
    char * name ;
    void * value ;
    int    type ;
}
console_vars[] = {
    { "CONSOLE_ALPHA",      &console_alpha,         CON_BYTE        },
    { "SHOW_COLOR",         &console_showcolor,     CON_DWORD_HEX   },
    { "FILES",              &opened_files,          CON_DWORD       },
    { "DEBUG_LEVEL",        &debug,                 CON_QWORD       },
} ;

/* --------------------------------------------------------------------------- */

static int console_scroll_pos = 0 ;
static int console_scroll_lateral_pos = 0 ;
static char console_input[4096] ;

/* --------------------------------------------------------------------------- */

static void eval_immediate();
static void eval_value();
static void eval_factor();
static void eval_subexpression();
static char * eval_expression( const char * here, int interactive );

static char * describe_type( DCB_TYPEDEF type, int from );
static char * show_value( DCB_TYPEDEF type, void * data );
static void   show_struct( int num, char * title, int indent, void * data );
static void   show_var( DCB_VAR var, char * name, void * data, char * title, int indent );

static int type_size( DCB_TYPEDEF orig );
static DCB_TYPEDEF reduce_type( DCB_TYPEDEF orig );
static void var2const();

static void console_do( const char * command );

/* --------------------------------------------------------------------------- */

static void * text_foreground_color = NULL;

/* --------------------------------------------------------------------------- */

static uint8_t sysfont[] = {
                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b00000000,

                                0b01110000,
                                0b10001000,
                                0b10001000,
                                0b11111000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b00000000,

                                0b11110000,
                                0b10001000,
                                0b10001000,
                                0b11110000,
                                0b10001000,
                                0b10001000,
                                0b11110000,
                                0b00000000,

                                0b01110000,
                                0b10001000,
                                0b10000000,
                                0b10000000,
                                0b10000000,
                                0b10001000,
                                0b01110000,
                                0b00000000,

                                0b11110000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b11110000,
                                0b00000000,

                                0b11111000,
                                0b10000000,
                                0b10000000,
                                0b11110000,
                                0b10000000,
                                0b10000000,
                                0b11111000,
                                0b00000000,

                                0b11111000,
                                0b10000000,
                                0b10000000,
                                0b11110000,
                                0b10000000,
                                0b10000000,
                                0b10000000,
                                0b00000000,

                                0b01110000,
                                0b10001000,
                                0b10000000,
                                0b10011000,
                                0b10001000,
                                0b10001000,
                                0b01110000,
                                0b00000000,

                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b11111000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b00000000,

                                0b11111000,
                                0b00100000,
                                0b00100000,
                                0b00100000,
                                0b00100000,
                                0b00100000,
                                0b11111000,
                                0b00000000,

                                0b00001000,
                                0b00001000,
                                0b00001000,
                                0b00001000,
                                0b00001000,
                                0b10001000,
                                0b01110000,
                                0b00000000,

                                0b10001000,
                                0b10010000,
                                0b10100000,
                                0b11000000,
                                0b10100000,
                                0b10010000,
                                0b10001000,
                                0b00000000,

                                0b10000000,
                                0b10000000,
                                0b10000000,
                                0b10000000,
                                0b10000000,
                                0b10000000,
                                0b11111000,
                                0b00000000,

                                0b10001000,
                                0b11011000,
                                0b10101000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b00000000,

                                0b11001000,
                                0b11001000,
                                0b10101000,
                                0b10101000,
                                0b10101000,
                                0b10011000,
                                0b10011000,
                                0b00000000,

                                0b01110000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b01110000,
                                0b00000000,

                                0b11110000,
                                0b10001000,
                                0b10001000,
                                0b11110000,
                                0b10000000,
                                0b10000000,
                                0b10000000,
                                0b00000000,

                                0b01110000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b10101000,
                                0b11001000,
                                0b01110000,
                                0b00000000,

                                0b11110000,
                                0b10001000,
                                0b10001000,
                                0b11110000,
                                0b10100000,
                                0b10010000,
                                0b10001000,
                                0b00000000,

                                0b01110000,
                                0b10001000,
                                0b10000000,
                                0b01110000,
                                0b00001000,
                                0b10001000,
                                0b01110000,
                                0b00000000,

                                0b11111000,
                                0b00100000,
                                0b00100000,
                                0b00100000,
                                0b00100000,
                                0b00100000,
                                0b00100000,
                                0b00000000,

                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b01110000,
                                0b00000000,

                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b01010000,
                                0b01010000,
                                0b00100000,
                                0b00000000,

                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b10101000,
                                0b10101000,
                                0b01010000,
                                0b00000000,

                                0b10001000,
                                0b10001000,
                                0b01010000,
                                0b00100000,
                                0b01010000,
                                0b10001000,
                                0b10001000,
                                0b00000000,

                                0b10001000,
                                0b10001000,
                                0b01010000,
                                0b00100000,
                                0b00100000,
                                0b00100000,
                                0b00100000,
                                0b00000000,

                                0b11111000,
                                0b00001000,
                                0b00010000,
                                0b00100000,
                                0b01000000,
                                0b10000000,
                                0b11111000,
                                0b00000000,

                                0b01110000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b01110000,
                                0b00000000,

                                0b00100000,
                                0b01100000,
                                0b10100000,
                                0b00100000,
                                0b00100000,
                                0b00100000,
                                0b11111000,
                                0b00000000,

                                0b01110000,
                                0b10001000,
                                0b00001000,
                                0b00110000,
                                0b01000000,
                                0b10000000,
                                0b11111000,
                                0b00000000,

                                0b01110000,
                                0b10001000,
                                0b00001000,
                                0b01110000,
                                0b00001000,
                                0b10001000,
                                0b01110000,
                                0b00000000,

                                0b00001000,
                                0b10001000,
                                0b10001000,
                                0b11111000,
                                0b00001000,
                                0b00001000,
                                0b00001000,
                                0b00000000,

                                0b11111000,
                                0b10000000,
                                0b11110000,
                                0b00001000,
                                0b00001000,
                                0b00001000,
                                0b11110000,
                                0b00000000,

                                0b01110000,
                                0b10000000,
                                0b11110000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b01110000,
                                0b00000000,

                                0b11111000,
                                0b00001000,
                                0b00010000,
                                0b00100000,
                                0b01000000,
                                0b10000000,
                                0b10000000,
                                0b00000000,

                                0b01110000,
                                0b10001000,
                                0b10001000,
                                0b01110000,
                                0b10001000,
                                0b10001000,
                                0b01110000,
                                0b00000000,

                                0b01110000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b01111000,
                                0b00001000,
                                0b01110000,
                                0b00000000,

                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b00110000,
                                0b00000000,

                                0b00000000,
                                0b00000000,
                                0b00110000,
                                0b00000000,
                                0b00000000,
                                0b00110000,
                                0b00000000,
                                0b00000000,

                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b00110000,
                                0b00000000,
                                0b00110000,
                                0b01100000,

                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b01111000,
                                0b00000000,
                                0b01111000,
                                0b00000000,
                                0b00000000,

                                0b11000000,
                                0b10001000,
                                0b00010000,
                                0b00100000,
                                0b01000000,
                                0b10001000,
                                0b00011000,
                                0b00000000,

                                0b01110000,
                                0b10001000,
                                0b10001000,
                                0b10111000,
                                0b10101000,
                                0b10110000,
                                0b10000000,
                                0b01110000,

                                0b00000000,
                                0b01001000,
                                0b11111100,
                                0b01001000,
                                0b01001000,
                                0b01001000,
                                0b11111100,
                                0b01001000,

                                0b00000000,
                                0b00000000,
                                0b01110000,
                                0b00001000,
                                0b01111000,
                                0b10001000,
                                0b01111000,
                                0b00000000,

                                0b10000000,
                                0b10000000,
                                0b11110000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b11110000,
                                0b00000000,

                                0b00000000,
                                0b00000000,
                                0b01110000,
                                0b10001000,
                                0b10000000,
                                0b10001000,
                                0b01110000,
                                0b00000000,

                                0b00001000,
                                0b00001000,
                                0b01111000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b01111000,
                                0b00000000,

                                0b00000000,
                                0b00000000,
                                0b01110000,
                                0b10001000,
                                0b11110000,
                                0b10000000,
                                0b01111000,
                                0b00000000,

                                0b01110000,
                                0b10001000,
                                0b10000000,
                                0b11100000,
                                0b10000000,
                                0b10000000,
                                0b10000000,
                                0b00000000,

                                0b00000000,
                                0b00000000,
                                0b01111000,
                                0b10001000,
                                0b10001000,
                                0b01111000,
                                0b00001000,
                                0b01110000,

                                0b10000000,
                                0b10000000,
                                0b11110000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b00000000,

                                0b00000000,
                                0b00100000,
                                0b00000000,
                                0b01100000,
                                0b00100000,
                                0b00100000,
                                0b01110000,
                                0b00000000,

                                0b00000000,
                                0b00001000,
                                0b00000000,
                                0b00111000,
                                0b00001000,
                                0b00001000,
                                0b10001000,
                                0b01110000,

                                0b10000000,
                                0b10000000,
                                0b10000000,
                                0b10010000,
                                0b11100000,
                                0b10010000,
                                0b10001000,
                                0b00000000,

                                0b01100000,
                                0b00100000,
                                0b00100000,
                                0b00100000,
                                0b00100000,
                                0b00100000,
                                0b11111000,
                                0b00000000,

                                0b00000000,
                                0b00000000,
                                0b11010000,
                                0b10101000,
                                0b10101000,
                                0b10101000,
                                0b10101000,
                                0b00000000,

                                0b00000000,
                                0b00000000,
                                0b11110000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b00000000,

                                0b00000000,
                                0b00000000,
                                0b01110000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b01110000,
                                0b00000000,

                                0b00000000,
                                0b00000000,
                                0b11110000,
                                0b10001000,
                                0b10001000,
                                0b11110000,
                                0b10000000,
                                0b10000000,

                                0b00000000,
                                0b00000000,
                                0b01111000,
                                0b10001000,
                                0b10001000,
                                0b01111000,
                                0b00001000,
                                0b00001000,

                                0b00000000,
                                0b00000000,
                                0b10110000,
                                0b11001000,
                                0b10000000,
                                0b10000000,
                                0b10000000,
                                0b00000000,

                                0b00000000,
                                0b00000000,
                                0b01110000,
                                0b10000000,
                                0b01110000,
                                0b00001000,
                                0b11110000,
                                0b00000000,

                                0b01000000,
                                0b01000000,
                                0b11110000,
                                0b01000000,
                                0b01000000,
                                0b01001000,
                                0b00110000,
                                0b00000000,

                                0b00000000,
                                0b00000000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b01110000,
                                0b00000000,

                                0b00000000,
                                0b00000000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b01010000,
                                0b00100000,
                                0b00000000,

                                0b00000000,
                                0b00000000,
                                0b10001000,
                                0b10001000,
                                0b10101000,
                                0b10101000,
                                0b01010000,
                                0b00000000,

                                0b00000000,
                                0b00000000,
                                0b10001000,
                                0b01010000,
                                0b00100000,
                                0b01010000,
                                0b10001000,
                                0b00000000,

                                0b00000000,
                                0b00000000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b01111000,
                                0b00001000,
                                0b11110000,

                                0b00000000,
                                0b00000000,
                                0b11111000,
                                0b00010000,
                                0b00100000,
                                0b01000000,
                                0b11111000,
                                0b00000000,

                                0b01110000,
                                0b01000000,
                                0b01000000,
                                0b01000000,
                                0b01000000,
                                0b01000000,
                                0b01110000,
                                0b00000000,

                                0b01110000,
                                0b00010000,
                                0b00010000,
                                0b00010000,
                                0b00010000,
                                0b00010000,
                                0b01110000,
                                0b00000000,

                                0b00110000,
                                0b01000000,
                                0b01000000,
                                0b01000000,
                                0b01000000,
                                0b01000000,
                                0b00110000,
                                0b00000000,

                                0b01100000,
                                0b00010000,
                                0b00010000,
                                0b00010000,
                                0b00010000,
                                0b00010000,
                                0b01100000,
                                0b00000000,

                                0b00110000,
                                0b01000000,
                                0b01000000,
                                0b10000000,
                                0b01000000,
                                0b01000000,
                                0b00110000,
                                0b00000000,

                                0b01100000,
                                0b00010000,
                                0b00010000,
                                0b00001000,
                                0b00010000,
                                0b00010000,
                                0b01100000,
                                0b00000000,

                                0b00100000,
                                0b00100000,
                                0b00100000,
                                0b00000000,
                                0b00100000,
                                0b00100000,
                                0b00100000,
                                0b00000000,

                                0b00100000,
                                0b01010000,
                                0b01010000,
                                0b00100000,
                                0b01010100,
                                0b01001000,
                                0b00110100,
                                0b00000000,

                                0b000100000,
                                0b001111000,
                                0b010100000,
                                0b001110000,
                                0b000101000,
                                0b000101000,
                                0b011110000,
                                0b000100000,

                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b11111000,
                                0b00000000,
                                0b00000000,
                                0b00000000,

                                0b01000000,
                                0b00100000,
                                0b00010000,
                                0b00001000,
                                0b00010000,
                                0b00100000,
                                0b01000000,
                                0b00000000,

                                0b00001000,
                                0b00010000,
                                0b00100000,
                                0b01000000,
                                0b00100000,
                                0b00010000,
                                0b00001000,
                                0b00000000,

                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b11111000,
                                0b00000000,

                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b00010000,
                                0b01100000,

                                0b00000000,
                                0b10000000,
                                0b01000000,
                                0b00100000,
                                0b00010000,
                                0b00001000,
                                0b00000100,
                                0b00000000,

                                0b00000000,
                                0b00000100,
                                0b00001000,
                                0b00010000,
                                0b00100000,
                                0b01000000,
                                0b10000000,
                                0b00000000,

                                0b00000000,
                                0b00000000,
                                0b00100000,
                                0b00100000,
                                0b11111000,
                                0b00100000,
                                0b00100000,
                                0b00000000,

                                0b00000000,
                                0b01010000,
                                0b00100000,
                                0b11111000,
                                0b00100000,
                                0b01010000,
                                0b00000000,
                                0b00000000,

                                0b00000000,
                                0b00100000,
                                0b00100000,
                                0b00100000,
                                0b00100000,
                                0b00000000,
                                0b00100000,
                                0b00000000,

                                0b00000000,
                                0b00100000,
                                0b00000000,
                                0b00100000,
                                0b00100000,
                                0b00100000,
                                0b00100000,
                                0b00000000,

                                0b00000000,
                                0b01110000,
                                0b10001000,
                                0b00010000,
                                0b00100000,
                                0b00000000,
                                0b00100000,
                                0b00000000,

                                0b00000000,
                                0b00100000,
                                0b00000000,
                                0b00100000,
                                0b01000000,
                                0b10001000,
                                0b01110000,
                                0b00000000,

                                0b01100110,
                                0b11011000,
                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b00000000,

                                0b00100000,
                                0b01010000,
                                0b10001000,
                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b00000000,

                                0b01010000,
                                0b01010000,
                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b00000000,

                                0b00010000,
                                0b00100000,
                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b00000000,

                                0b00100000,
                                0b00010000,
                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b00000000,

                                0b00100000,
                                0b00100000,
                                0b00100000,
                                0b00100000,
                                0b00111100,
                                0b00100000,
                                0b00100000,
                                0b00100000,

                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b11111100,
                                0b00000000,
                                0b00000000,
                                0b00000000,

                                0b00100000,
                                0b00100000,
                                0b00100000,
                                0b00100000,
                                0b00100000,
                                0b00100000,
                                0b00100000,
                                0b00100000,

                                0b00010000,
                                0b00100000,
                                0b01110000,
                                0b10001000,
                                0b11111000,
                                0b10001000,
                                0b10001000,
                                0b00000000,

                                0b00010000,
                                0b00100000,
                                0b11111000,
                                0b10000000,
                                0b11110000,
                                0b10000000,
                                0b11111000,
                                0b00000000,

                                0b00010000,
                                0b00100000,
                                0b11111000,
                                0b00100000,
                                0b00100000,
                                0b00100000,
                                0b11111000,
                                0b00000000,

                                0b00010000,
                                0b00100000,
                                0b01110000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b01110000,
                                0b00000000,

                                0b00010000,
                                0b00100000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b01110000,
                                0b00000000,

                                0b01110000,
                                0b00000000,
                                0b11001000,
                                0b11001000,
                                0b10101000,
                                0b10011000,
                                0b10011000,
                                0b00000000,

                                0b00010000,
                                0b00100000,
                                0b01110000,
                                0b00001000,
                                0b01111000,
                                0b10001000,
                                0b01111000,
                                0b00000000,

                                0b00010000,
                                0b00100000,
                                0b01110000,
                                0b10001000,
                                0b11110000,
                                0b10000000,
                                0b01111000,
                                0b00000000,

                                0b00010000,
                                0b00100000,
                                0b00000000,
                                0b01100000,
                                0b00100000,
                                0b00100000,
                                0b01110000,
                                0b00000000,

                                0b00010000,
                                0b00100000,
                                0b01110000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b01110000,
                                0b00000000,

                                0b00010000,
                                0b00100000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b01110000,
                                0b00000000,

                                0b11110000,
                                0b00000000,
                                0b11110000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b10001000,
                                0b00000000,

                                0b11111100,
                                0b11111100,
                                0b11111100,
                                0b11111100,
                                0b11111100,
                                0b11111100,
                                0b11111100,
                                0b11111100,

                                0b01100000,
                                0b10010000,
                                0b10010000,
                                0b01100000,
                                0b00000000,
                                0b00000000,
                                0b00000000,
                                0b00000000,

                            };

// static uint8_t * letters = ( uint8_t * ) " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.:;=%@#abcdefghijklmnopqrstuvwxyz[](){}|&$-><_,\\/+*!¡?¿~^\"'`\x01\x02\x03ÁÉÍÓÚÑáéíóúñ\xffº" ;
static const uint8_t * letters = ( uint8_t * ) " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.:;=%@#abcdefghijklmnopqrstuvwxyz[](){}|&$-><_,\\/+*!\xA1?\xBF~^\"'`\x01\x02\x03\xC1\xC9\xCD\xD3\xDA\xD1\xE1\xE9\xED\xF3\xFA\xF1\xFF\xBA";

static int64_t debug_sysfont = -1;

/* --------------------------------------------------------------------------- */

static void create_debug_sysfont() {
    int nchars = strlen( ( const char * ) letters );
    Uint32 * pixels = malloc( 8 * 8 * nchars * sizeof( Uint32 ) );
    if ( !pixels ) return;

    int i, ii, iii;

    // Convert 1bpp data to 32bpp data
    for ( i = 0; i < nchars; i++ )
        for ( ii = 0; ii < 8; ii++ )
            for ( iii = 0; iii < 8; iii++ )
                pixels[ i * 64 + ii * 8 + iii ] = ( sysfont[ i * 8 + ii ] & ( 0x80 >> iii ) ) ? 0xffffffff : 0;

    SDL_Surface * surface = SDL_CreateRGBSurfaceFrom(   pixels,
                                                        8,                          /* width  */
                                                        8 * nchars,                 /* height */
                                                        gPixelFormat->BitsPerPixel, /* depth  */
                                                        8 * 4,                      /* pitch  */
                                                        gPixelFormat->Rmask,
                                                        gPixelFormat->Gmask,
                                                        gPixelFormat->Bmask,
                                                        gPixelFormat->Amask
                                                    );
    if ( !surface ) return;
//    SDL_SetColorKey( surface, SDL_TRUE, 0 );

    GRAPH *map = bitmap_new( 0, 0, 0, surface );
    if ( !map ) {
        SDL_FreeSurface( surface );
        return;
    }

    bitmap_add_cpoint( map, 0, 0 );

#ifdef USE_SDL2
    debug_sysfont = gr_font_new_from_bitmap( map, NULL, /*CHARSET_CP850*/ CHARSET_ISO8859, CHARWIDTH, CHARHEIGHT, 0, 0, NFB_FIXEDWIDTH, letters, 0, 0, 0, 0, 0 );
#endif
#ifdef USE_SDL2_GPU
    debug_sysfont = gr_font_new_from_bitmap( map, surface, NULL, /*CHARSET_CP850*/ CHARSET_ISO8859, CHARWIDTH, CHARHEIGHT, 0, 0, NFB_FIXEDWIDTH, letters, 0, 0, 0, 0, 0 );
#endif

    SDL_FreeSurface( surface );

}

/* --------------------------------------------------------------------------- */

static void systext_color( int64_t foreground_color ) {
    if ( !text_foreground_color ) text_foreground_color = gr_text_alloc();
    gr_text_setrgba_ptext( text_foreground_color, ( foreground_color >> 16 ) & 0xff, ( foreground_color >> 8 ) & 0xff, foreground_color & 0xff, 255 );
}

/* --------------------------------------------------------------------------- */

static void systext_puts( int64_t x, int64_t y, char * str ) {
    uint8_t r, g, b;

    gr_text_getrgba_ptext( text_foreground_color, &r, &g, &b, NULL );
    gr_text_setrgba_ptext( text_foreground_color, r, g, b, console_alpha );

    gr_text_put( NULL, text_foreground_color, NULL, debug_sysfont, x, y, ( unsigned char * ) str );
}

/* --------------------------------------------------------------------------- */

static void systext_putchar( int64_t x, int64_t y, char c ) {
    char str[2];
    str[0] = c;
    str[1] = '\0';
    systext_puts( x, y, str );
}

/* --------------------------------------------------------------------------- */

static void console_scroll( int direction ) {
    console_scroll_pos += direction ;
    if ( direction < 0 ) {
        if ( console_scroll_pos < 0 ) console_scroll_pos = 0 ;
    } else {
        if ( console_scroll_pos > CONSOLE_HISTORY ) console_scroll_pos = CONSOLE_HISTORY ;
    }
}

/* --------------------------------------------------------------------------- */

static void console_putline( char * text ) {
    if ( !console_initialized ) {
        memset( console, 0, sizeof( console ) ) ;
        console_initialized = 1 ;
    }

    if ( console[console_tail] ) free( console[console_tail] ) ;
    console[console_tail] = strdup( text ) ;

    console_tail++ ;
    if ( console_tail == CONSOLE_HISTORY ) console_tail = 0 ;
    if ( console_tail == console_head ) {
        console_head++ ;
        if ( console_head == CONSOLE_HISTORY ) console_head = 0 ;
    }
}

/* --------------------------------------------------------------------------- */

static int console_printf( const char *fmt, ... ) {
    char text[MAXTEXT], * ptr, * iptr ;

    va_list ap;
    va_start( ap, fmt );
    _vsnprintf( text, sizeof( text ), fmt, ap );
    va_end( ap );
    text[sizeof( text )-1] = 0;

    if ( *text == '[' ) {
        int l = strlen( COLOR_GRAY ), l2 = strlen( COLOR_SILVER );
        memmove( text + l, text, strlen( text ) + 1 ) ;
        memmove( text, COLOR_GRAY, l ) ;
        ptr = strchr( text, ']' ) ;
        if ( ptr ) {
            ptr++ ;
            memmove( ptr + l2, ptr, strlen( ptr ) + 1 ) ;
            memmove( ptr, COLOR_SILVER, l2 ) ;
        }
    }

    iptr = text ;
    ptr  = text ;

    while ( *ptr ) {
        if ( *ptr == '\n' ) {
            *ptr = 0 ;
            console_putline( iptr ) ;
            iptr = ptr + 1 ;
        }
        ptr++ ;
    }

    if ( ptr > iptr ) console_putline( iptr ) ;

    return 0;
}

/* --------------------------------------------------------------------------- */

static void console_putcommand( const char * commandline ) {
    if ( !command_initialized ) {
        memset( command, 0, sizeof( command ) );
        command_initialized = 1;
    }

    if ( command[command_tail] ) free( command[command_tail] );
    command[command_tail++] = strdup( commandline );
    if ( command_tail == COMMAND_HISTORY ) command_tail = 0;
    if ( command_tail == command_head ) {
        if ( ++command_head == COMMAND_HISTORY ) command_head = 0;
    } else
        command_count++;
}

/* --------------------------------------------------------------------------- */

static const char * console_getcommand( int offset ) {
    if ( offset >= 0 || offset < -command_count ) return NULL;

    offset = command_tail + offset;
    while ( offset < 0 ) offset = COMMAND_HISTORY + offset;
    return command[offset];
}

/* --------------------------------------------------------------------------- */

static int cursor_pos = 0;
static char tmp_buffer[4096];

static void console_getkey( int sym ) {
    static int history_offset = 0;
    const char * cmd;

    if ( sym == SDLK_UP ) {
        cmd = console_getcommand( --history_offset );
        if ( cmd == NULL )  history_offset++;
        else                strncpy( console_input, cmd, sizeof( console_input ) - 3 );
        cursor_pos = strlen( console_input );
    }

    if ( sym == SDLK_DOWN ) {
        if ( history_offset == -1 ) {
            *console_input = 0;
            history_offset++;
        } else {
            cmd = console_getcommand( ++history_offset );
            if ( cmd == NULL )  history_offset--;
            else                strncpy( console_input, cmd, sizeof( console_input ) - 3 );
        }
        cursor_pos = strlen( console_input );
    }

    if ( sym == SDLK_BACKSPACE && *console_input ) {
        if ( cursor_pos > 0 ) {
            strncpy(tmp_buffer, console_input, cursor_pos - 1);
            strcpy(&tmp_buffer[ cursor_pos - 1 ], &console_input[ cursor_pos ]);
            strcpy(console_input, tmp_buffer);
            cursor_pos--;
        }
    }

    if ( sym == SDLK_DELETE ) {
        if ( cursor_pos < strlen( console_input ) ) {
            strncpy(tmp_buffer, console_input, cursor_pos );
            strcpy(&tmp_buffer[ cursor_pos ], &console_input[ cursor_pos + 1 ]);
            strcpy(console_input, tmp_buffer);
        }
    }

    if ( sym == SDLK_ESCAPE ) *console_input = 0 ;

    if ( sym == SDLK_RETURN || sym == SDLK_KP_ENTER ) {
        console_scroll_pos = 0 ;
        console_printf( COLOR_SILVER "> %s", console_input ) ;
        if ( * console_input ) {
            console_putcommand( console_input );
            console_do( console_input ) ;
            *console_input = 0 ;
            history_offset = 0;
            cursor_pos = 0;
        }
    }

    if ( sym >= ' ' && sym <= 255 && sym != SDLK_DELETE && strlen( console_input ) < sizeof( console_input ) - 3 ) {
        strncpy(tmp_buffer, console_input, cursor_pos );
        tmp_buffer[ cursor_pos ] = sym;
        strcpy(&tmp_buffer[ cursor_pos + 1], &console_input[ cursor_pos ]);
        strcpy(console_input, tmp_buffer);
        cursor_pos++;
    }

    if ( sym == SDLK_LEFT ) {
        cursor_pos--;
    }

    if ( sym == SDLK_RIGHT ) {
        cursor_pos++;
    }

    if ( sym == SDLK_HOME ) {
        cursor_pos = 0;
    }

    if ( sym == SDLK_END ) {
        cursor_pos = strlen( console_input );
    }

    if ( cursor_pos < 0 ) cursor_pos = 0;
    if ( cursor_pos > strlen( console_input ) ) cursor_pos = strlen( console_input );
}

/* --------------------------------------------------------------------------- */

static void console_lateral_scroll( int direction ) {
    if ( direction > 0 ) {
        console_scroll_lateral_pos-- ;
        if ( console_scroll_lateral_pos < 0 ) console_scroll_lateral_pos = 0 ;
    } else {
        console_scroll_lateral_pos++ ;
        if ( console_scroll_lateral_pos > MAXTEXT ) console_scroll_lateral_pos = MAXTEXT ;
    }
}

/* --------------------------------------------------------------------------- */

static char * describe_type( DCB_TYPEDEF type, int from ) {
    static char buffer[512] ;
    int i ;

    if ( !from ) buffer[0] = 0 ;

    switch ( type.BaseType[from] ) {
        case TYPE_ARRAY:
            for ( i = from ; type.BaseType[i] == TYPE_ARRAY; i++ ) ;
            describe_type( type, i ) ;
            for ( i = from ; type.BaseType[i] == TYPE_ARRAY; i++ )
                _snprintf( buffer + strlen( buffer ), sizeof( buffer ) - strlen( buffer ), "[%"PRId64"]", type.Count[i] - 1 ) ;
            break ;

        case TYPE_STRUCT:
            strcat( buffer, "STRUCT" ) ;
            break ;

        case TYPE_QWORD:
            strcat( buffer, "QWORD" ) ;
            break ;

        case TYPE_INT:
            strcat( buffer, "INT" ) ;
            break ;

        case TYPE_DWORD:
            strcat( buffer, "DWORD" ) ;
            break ;

        case TYPE_INT32:
            strcat( buffer, "INT32" ) ;
            break ;

        case TYPE_SHORT:
            strcat( buffer, "SHORT" ) ;
            break ;

        case TYPE_WORD:
            strcat( buffer, "WORD" ) ;
            break ;

        case TYPE_BYTE:
            strcat( buffer, "BYTE" ) ;
            break ;

        case TYPE_SBYTE:
            strcat( buffer, "SIGNED BYTE" ) ;
            break ;

        case TYPE_CHAR:
            strcat( buffer, "CHAR" ) ;
            break ;

        case TYPE_STRING:
            strcat( buffer, "STRING" ) ;
            break ;

        case TYPE_FLOAT:
            strcat( buffer, "FLOAT" ) ;
            break ;

        case TYPE_DOUBLE:
            strcat( buffer, "DOUBLE" ) ;
            break ;

        case TYPE_POINTER:
            describe_type( type, from + 1 ) ;
            strcat( buffer, " POINTER" ) ;
            break ;
    }

    return buffer ;
}

/* --------------------------------------------------------------------------- */

static char * show_value( DCB_TYPEDEF type, void * data ) {
    static char buffer[512] ;
    char * newbuffer ;
    int subsize;
    unsigned int n ;
    unsigned int count ;

    switch ( type.BaseType[0] ) {
        case TYPE_ARRAY:
            count = type.Count[0];

            type = reduce_type( type ) ;
            subsize = type_size( type ) ;
            if ( type.BaseType[0] == TYPE_STRUCT ) return "" ;
            newbuffer = ( char * ) malloc( 512 ) ;
            strcpy( newbuffer, "= (" ) ;
            for ( n = 0 ; n < count ; n++ ) {
                if ( n ) strcat( newbuffer, ", " ) ;
                show_value( type, data ) ;
                if ( strlen( newbuffer ) + strlen( buffer ) > 30 ) {
                    strcat( newbuffer, "..." ) ;
                    break ;
                }
                strcat( newbuffer, buffer + 2 ) ;
                data = ( uint8_t * )data + subsize ;
            }
            strcat( newbuffer, ")" ) ;
            strcpy( buffer, newbuffer ) ;
            free( newbuffer ) ;
            return buffer ;

        case TYPE_STRUCT:
            return "" ;

        case TYPE_STRING:
            _snprintf( buffer, sizeof( buffer ), "= \"%s\"", string_get( *( uint32_t * )data ) ) ;
            return buffer ;

        case TYPE_BYTE:
            _snprintf( buffer,sizeof( buffer ), "= %"PRIu8, *( uint8_t * )data ) ;
            return buffer ;

        case TYPE_SBYTE:
            _snprintf( buffer, sizeof( buffer ), "= %"PRId8, *( int8_t * )data ) ;
            return buffer ;

        case TYPE_CHAR:
            if ( *( uint8_t * )data >= 32 )
                _snprintf( buffer, sizeof( buffer ), "= '%c'", *( uint8_t * )data ) ;
            else
                _snprintf( buffer, sizeof( buffer ), "= '\\x%02X'", *( uint8_t * )data ) ;
            return buffer ;

        case TYPE_FLOAT:
            _snprintf( buffer, sizeof( buffer ), "= %f", *( float * )data ) ;
            return buffer ;

        case TYPE_DOUBLE:
            _snprintf( buffer, sizeof( buffer ), "= %F", *( double * )data ) ;
            return buffer ;

        case TYPE_WORD:
            _snprintf( buffer, sizeof( buffer ), "= %"PRIu16, *( uint16_t * )data ) ;
            return buffer ;

        case TYPE_DWORD:
            _snprintf( buffer, sizeof( buffer ), "= %"PRIu32, *( uint32_t * )data ) ;
            return buffer ;

        case TYPE_QWORD:
            _snprintf( buffer, sizeof( buffer ), "= %"PRIu64, *( uint64_t * )data ) ;
            return buffer ;

        case TYPE_SHORT:
            _snprintf( buffer, sizeof( buffer ), "= %"PRId16, *( int16_t * )data ) ;
            return buffer ;

        case TYPE_INT32:
            _snprintf( buffer, sizeof( buffer ), "= %"PRId32, *( int32_t * )data ) ;
            return buffer ;

        case TYPE_INT:
            _snprintf( buffer, sizeof( buffer ), "= %"PRId64, *( int64_t * )data ) ;
            return buffer ;

        case TYPE_POINTER:
            _snprintf( buffer, sizeof( buffer ), "= 0x%016X", *( uint32_t * )data ) ;
            return buffer ;

        default:
            return "?" ;
    }
}

/* --------------------------------------------------------------------------- */

static void show_struct( int num, char * title, int indent, void * data ) {
    int n, count ;
    DCB_VAR * vars ;

    vars = dcb.varspace_vars[num] ;
    count = dcb.varspace[num].NVars ;

    for ( n = 0 ; n < count ; n++ ) {
        show_var( vars[n], 0, data ? ( uint8_t * )data + vars[n].Offset : 0, title, indent ) ;
    }
}

/* --------------------------------------------------------------------------- */

static void show_var( DCB_VAR var, char * name, void * data, char * title, int indent ) {
    char spaces[indent+1] ;

    memset( spaces, ' ', indent ) ;
    spaces[indent] = 0 ;

    if ( !name ) {
        name = "?" ;
        for ( uint64_t n = 0 ; n < dcb.data.NID ; n++ ) {
            if ( dcb.id[ n ].Code == var.ID ) {
                name = ( char * ) dcb.id[ n ].Name ;
                break ;
            }
        }
    }

    if ( data ) console_printf( "%s%s %s %s %s\n", title, spaces, describe_type( var.Type, 0 ), name, show_value( var.Type, data ) ) ;
    else        console_printf( "%s%s %s %s\n", title, spaces, describe_type( var.Type, 0 ), name ) ;

    if ( var.Type.BaseType[0] == TYPE_STRUCT ) {
        show_struct( var.Type.Members, title, indent + 3, data ) ;
        console_printf( "%s%s END", title, spaces ) ;
    }
}

/* --------------------------------------------------------------------------- */

/* Very simple tokenizer */

static void get_token() {
    char * ptr ;

    while ( isspace( *token_ptr ) ) token_ptr++ ;

    if ( !*token_ptr ) {
        token.type = NOTOKEN ;
        return ;
    }

    /* Numbers */

    if ( ISNUM( *token_ptr ) ) {
        int base = 10;

        /* Hex/Bin/Octal numbers with the h/b/o sufix */
        ptr = ( char * ) token_ptr;
        while ( ISNUM( *ptr ) || ( *ptr >= 'a' && *ptr <= 'f' ) || ( *ptr >= 'A' && *ptr <= 'F' ) ) ptr++;

        if ( *ptr != 'h' && *ptr != 'H' && *ptr != 'o' && *ptr != 'O' && ( ptr[-1] == 'b' || ptr[-1] == 'B' ) ) ptr--;

        if ( *ptr == 'b' || *ptr == 'B' ) base = 2;
        if ( *ptr == 'h' || *ptr == 'H' ) base = 16;
        if ( *ptr == 'o' || *ptr == 'O' ) base = 8;

        token.code = 0 ; /* for ints values */
        token.type = NUMBER_INT;

        /* Calculate the number value */

        while ( ISNUM( *token_ptr ) || ( base > 10 && ISXNUM( *token_ptr ) ) ) {
            if ( base == 2 && *token_ptr != '0' && *token_ptr != '1' ) break;
            if ( base == 8 && ( *token_ptr < '0' || *token_ptr > '7' ) ) break;
            if ( base == 10 && !ISNUM( *token_ptr ) ) break;
            if ( base == 16 && !ISNUM( *token_ptr ) && ( TOUPPER( *token_ptr ) < 'A' || TOUPPER( *token_ptr ) > 'F' ) ) break;

            if ( ISNUM( *token_ptr ) ) {
                token.code = token.code * base + ( *token_ptr - '0' );
                token_ptr++;
            }
            if ( *token_ptr >= 'a' && *token_ptr <= 'f' && base > 10 ) {
                token.code = token.code * base + ( *token_ptr - 'a' + 10 );
                token_ptr++;
            }
            if ( *token_ptr >= 'A' && *token_ptr <= 'F' && base > 10 ) {
                token.code = token.code * base + ( *token_ptr - 'A' + 10 );
                token_ptr++;
            }
        }

        /* We have the integer part now - convert to int/double */

        token.double_value = ( double ) token.code;

        if ( *token_ptr == '.' && base == 10 ) {
            token.type = NUMBER_DOUBLE;
            token_ptr++;
            if ( !ISNUM( *token_ptr ) )
                token_ptr--;
            else {
                double dec = 0.1;
                while ( ISNUM( *token_ptr ) || ( base > 100 && ISXNUM( *token_ptr ) ) ) {
                    if ( ISNUM( *token_ptr ) ) token.double_value = token.double_value + dec * ( *token_ptr++ - '0' );
                    dec /= 10.0;
                }
            }
        }

        /* Skip the base sufix */

        if ( base == 16 && ( *token_ptr == 'h' || *token_ptr == 'H' ) ) token_ptr++;
        if ( base == 8  && ( *token_ptr == 'o' || *token_ptr == 'O' ) ) token_ptr++;
        if ( base == 2  && ( *token_ptr == 'b' || *token_ptr == 'B' ) ) token_ptr++;

        if ( token.type == NUMBER_DOUBLE )
            _snprintf( token.name, sizeof( token.name ), "%F", token.double_value ) ;
        else
            _snprintf( token.name, sizeof( token.name ), "%"PRId64, token.code ) ;
        return ;
    }

    if ( *token_ptr == '"' || *token_ptr == '\'' ) { /* Cadena */
        char c = *token_ptr++ ;
        token.type = STRING ;
        ptr = token.name;
        while ( *token_ptr && *token_ptr != c ) *ptr++ = *token_ptr++ ;
        if ( *token_ptr == c ) token_ptr++ ;
        *ptr = 0 ;
        return ;
    }

    ptr = token.name ;
    *ptr++ = TOUPPER( *token_ptr ) ;
    if ( ISWORDCHAR( *token_ptr++ ) ) {
        while ( ISWORDCHAR( *token_ptr ) )
            *ptr++ = TOUPPER( *token_ptr++ ) ;
    }
    *ptr = 0 ;

    for ( int n = 0 ; n < dcb.data.NID ; n++ ) {
        if ( strcmp( ( char * )dcb.id[n].Name, token.name ) == 0 ) {
            token.type = IDENTIFIER ;
            token.code = dcb.id[n].Code ;
            token.double_value = ( double ) token.code;
/*            strcpy( token.name, (char *)dcb.id[n].Name ) ; */
            return ;
        }
    }

    token.type = OPERATOR ;
}

/* --------------------------------------------------------------------------- */

static DCB_TYPEDEF reduce_type( DCB_TYPEDEF orig ) {
    int n ;
    for ( n = 0 ; n < MAX_TYPECHUNKS - 1 ; n++ ) {
        orig.BaseType[n] = orig.BaseType[n+1] ;
        orig.Count[n] = orig.Count[n+1] ;
    }
    return orig ;
}

/* --------------------------------------------------------------------------- */

static void var2const() {
    while ( result.type == RT_VARIABLE && result.var.Type.BaseType[0] == TYPE_ARRAY )
        result.var.Type = reduce_type( result.var.Type ) ;

    if ( result.type == RT_VARIABLE && result.var.Type.BaseType[0] == TYPE_STRING ) {
        result.type = RT_STRING ;
        strncpy( result.name, ( const char * ) string_get( *( int64_t * )( result.data ) ), sizeof( result.name ) ) ;
        result.name[sizeof( result.name ) - 1] = 0 ;
    }

    if ( result.type == RT_VARIABLE && result.var.Type.BaseType[0] == TYPE_FLOAT ) {
        result.type = RT_CONSTANT_DOUBLE ;
        result.double_value = *( float * )( result.data ) ;
        result.int_value = ( int64_t ) result.double_value ;
    }

    if ( result.type == RT_VARIABLE && result.var.Type.BaseType[0] == TYPE_DOUBLE ) {
        result.type = RT_CONSTANT_DOUBLE ;
        result.double_value = *( double * )( result.data ) ;
        result.int_value = ( int64_t ) result.double_value ;
    }

    if ( result.type == RT_VARIABLE && ( result.var.Type.BaseType[0] == TYPE_QWORD || result.var.Type.BaseType[0] == TYPE_INT ) ) {
        result.type = RT_CONSTANT ;
        result.int_value = *( int64_t * )( result.data ) ;
        result.double_value = ( double ) result.int_value ;
    }

    if ( result.type == RT_VARIABLE && ( result.var.Type.BaseType[0] == TYPE_DWORD || result.var.Type.BaseType[0] == TYPE_INT32 ) ) {
        result.type = RT_CONSTANT ;
        result.int_value = *( int32_t * )( result.data ) ;
        result.double_value = ( double ) result.int_value ;
    }

    if ( result.type == RT_VARIABLE && ( result.var.Type.BaseType[0] == TYPE_WORD || result.var.Type.BaseType[0] == TYPE_SHORT ) ) {
        result.type = RT_CONSTANT ;
        result.int_value = *( int16_t * )( result.data ) ;
        result.double_value = ( double ) result.int_value ;
    }

    if ( result.type == RT_VARIABLE && ( result.var.Type.BaseType[0] == TYPE_BYTE || result.var.Type.BaseType[0] == TYPE_SBYTE ) ) {
        result.type = RT_CONSTANT ;
        result.int_value = *( int8_t * )( result.data ) ;
        result.double_value = ( double ) result.int_value ;
    }

    if ( result.type == RT_VARIABLE && result.var.Type.BaseType[0] == TYPE_CHAR ) {
        result.type = RT_STRING ;
        if ( *( uint8_t * )result.data >= 32 )
            _snprintf( result.name, sizeof( result.name ), "%c", *( uint8_t * )result.data ) ;
        else
            _snprintf( result.name, sizeof( result.name ), "\\x%02X", *( uint8_t * )result.data ) ;
    }
}

/* --------------------------------------------------------------------------- */

static int type_size( DCB_TYPEDEF orig ) {
    unsigned int n, total ;

    switch ( orig.BaseType[0] ) {
        case TYPE_ARRAY:
            return orig.Count[0] * type_size( reduce_type( orig ) ) ;

        case TYPE_POINTER:
        case TYPE_STRING:
        case TYPE_QWORD:
        case TYPE_DOUBLE:
        case TYPE_INT:
            return 8;

        case TYPE_DWORD:
        case TYPE_FLOAT:
        case TYPE_INT32:
            return 4 ;

        case TYPE_WORD:
        case TYPE_SHORT:
            return 2 ;

        case TYPE_BYTE:
        case TYPE_SBYTE:
        case TYPE_CHAR:
            return 1 ;

        case TYPE_STRUCT:
            total = 0 ;
            for ( n = 0 ; n < dcb.varspace[orig.Members].NVars ; n++ )
                total += type_size( dcb.varspace_vars[orig.Members][n].Type ) ;
            return total ;

        default:
            return 0 ;
    }
}

/* --------------------------------------------------------------------------- */

static void eval_local( DCB_PROC * proc, INSTANCE * i ) {
    unsigned int n ;

    for ( n = 0 ; n < dcb.data.NLocVars ; n++ ) {
        if ( dcb.locvar[n].ID == token.code ) {
            strcpy( result.name, token.name ) ;
            result.type = RT_VARIABLE ;
            result.var  = dcb.locvar[n] ;
            result.data = ( uint8_t * )i->locdata + dcb.locvar[n].Offset ;
            get_token() ;
            return ;
        }
    }

    for ( n = 0 ; n < proc->data.NPriVars ; n++ ) {
        if ( proc->privar[n].ID == token.code ) {
            strcpy( result.name, token.name ) ;
            result.type = RT_VARIABLE ;
            result.var  = proc->privar[n] ;
            result.data = ( uint8_t * )i->pridata + proc->privar[n].Offset ;
            get_token() ;
            return ;
        }
    }

    for ( n = 0 ; n < proc->data.NPubVars ; n++ ) {
        if ( proc->pubvar[n].ID == token.code ) {
            strcpy( result.name, token.name ) ;
            result.type = RT_VARIABLE ;
            result.var  = proc->pubvar[n] ;
            result.data = ( uint8_t * )i->pubdata + proc->pubvar[n].Offset ;
            get_token() ;
            return ;
        }
    }

    console_printf( COLOR_RED "Local or private or public variable not found" COLOR_SILVER ) ;
    result.type = RT_ERROR ;
}

/* --------------------------------------------------------------------------- */

static void eval_immediate() {
    unsigned int n ;

    if ( token.type == NUMBER_INT ) {
        result.type = RT_CONSTANT ;
        result.int_value = token.code ;
        result.double_value = ( double ) result.int_value ;
        get_token() ;
        return ;
    }

    if ( token.type == NUMBER_DOUBLE ) {
        result.type = RT_CONSTANT_DOUBLE ;
        result.double_value = token.double_value ;
        result.int_value = ( int64_t ) result.double_value;
        get_token() ;
        return ;
    }

    if ( token.type == STRING ) {
        result.type = RT_STRING ;
        _snprintf( result.name, sizeof( result.name ), "%s", token.name ) ;
        get_token() ;
        return ;
    }

    if ( token.type != IDENTIFIER ) {
        console_printf( COLOR_RED "Not a valid expression" COLOR_SILVER ) ;
        result.type = RT_ERROR ;
        return ;
    }

    if ( token.name[0] == '(' ) {
        get_token() ;
        eval_subexpression() ;
        if ( token.name[0] != ')' ) {
            console_printf( COLOR_RED "Unbalanced parens" COLOR_SILVER ) ;
            result.type = RT_ERROR ;
            return ;
        }
        get_token() ;
        return ;
    }

    if ( token.name[0] == '-' ) {
        get_token() ;
        eval_immediate() ;
        var2const() ;

        switch ( result.type ) {
            case RT_CONSTANT:
                result.int_value = -result.int_value;
                result.double_value = ( double ) result.int_value;
                _snprintf( result.name, sizeof( result.name ), "%"PRId64, result.int_value ) ;
                break;

            case RT_CONSTANT_DOUBLE:
                result.double_value = -result.double_value;
                result.int_value = ( int64_t ) result.double_value;
                _snprintf( result.name, sizeof( result.name ), "%F", result.double_value ) ;
                break;

            default:
                console_printf( COLOR_RED "Operand is not a number" COLOR_SILVER ) ;
                result.type = RT_ERROR ;
                return ;
        }

        return ;
    }

    for ( n = 0 ; n < dcb.data.NGloVars ; n++ ) {
        if ( dcb.glovar[n].ID == token.code ) {
            strcpy( result.name, token.name ) ;
            result.type = RT_VARIABLE ;
            result.var  = dcb.glovar[n] ;
            result.data = ( uint8_t * )globaldata + dcb.glovar[n].Offset ;

            get_token() ;
            return ;
        }
    }

    if ( strcmp( token.name, "MAIN" ) == 0 )
        token.code = dcb.proc[0].data.ID ;

    for ( n = 0 ; n < dcb.data.NProcs ; n++ ) {
        if ( dcb.proc[n].data.ID == token.code ) {
            INSTANCE * i = first_instance ;

            while ( i ) {
                if ( i->proc->type == ( int )n ) break ;
                i = i->next ;
            }
            if ( !i ) {
                console_printf( COLOR_RED "No instance of process %s is active" COLOR_SILVER, token.name ) ;
                result.type = RT_ERROR ;
                return ;
            }

            get_token() ;
            if ( token.name[0] != '.' ) {
                console_printf( COLOR_RED "Invalid use of a process name" COLOR_SILVER ) ;
                result.type = RT_ERROR ;
                return ;
            }
            get_token() ;
            eval_local( &dcb.proc[n], i ) ;
            return ;
        }
    }

    console_printf( COLOR_RED "Variable does not exist (%s)" COLOR_SILVER, token.name ) ;
    result.type = RT_ERROR ;
    return ;
}

/* --------------------------------------------------------------------------- */

static void eval_value() {
    eval_immediate() ;
    if ( result.type == RT_ERROR ) return ;

    for ( ;; ) {
        if ( token.name[0] == '.' ) {
            DCB_VARSPACE * v ;
            DCB_VAR * var ;
            unsigned int n ;

            var2const() ;

            if ( result.type == RT_CONSTANT || result.type == RT_CONSTANT_DOUBLE ) {
                INSTANCE * i ;
                i = instance_get( ( int64_t ) result.int_value ) ;
                if ( !i ) {
                    result.type = RT_ERROR ;
                    console_printf( COLOR_RED "Instance %"PRId64" does not exist" COLOR_SILVER, ( int64_t )result.int_value ) ;
                    return ;
                }
                get_token() ;
                eval_local( &dcb.proc[i->proc->type], i ) ;
                continue;
            }

            if ( result.type != RT_VARIABLE || result.var.Type.BaseType[0] != TYPE_STRUCT ) {
                console_printf( COLOR_RED "%s is not an struct" COLOR_SILVER, result.name );
                result.type = RT_ERROR ;
                return ;
            }

            get_token() ;
            if ( token.type != IDENTIFIER ) {
                console_printf( COLOR_RED "%s is not a member" COLOR_SILVER, token.name ) ;
                result.type = RT_ERROR ;
                return ;
            }

            v = &dcb.varspace[result.var.Type.Members] ;
            var = dcb.varspace_vars[result.var.Type.Members] ;
            for ( n = 0 ; n < v->NVars ; n++ ) {
                if ( var[n].ID == token.code )
                    break ;
            }
            if ( n == v->NVars ) {
                console_printf( COLOR_RED "%s is not a member" COLOR_SILVER, token.name ) ;
                result.type = RT_ERROR ;
                return ;
            }

            result.type = RT_VARIABLE ;
            result.var = var[n] ;
            result.data = ( uint8_t * )result.data + var[n].Offset ;
            strcat( result.name, "." ) ;
            strcat( result.name, token.name ) ;
            get_token() ;
            continue ;
        }

        if ( token.name[0] == '[' ) {
            DCB_VAR i = result.var ;
            void * i_data = result.data ;
            char name[1024];

            if ( result.type != RT_VARIABLE || result.var.Type.BaseType[0] != TYPE_ARRAY ) {
                console_printf( COLOR_RED "%s is not an array" COLOR_SILVER, result.name ) ;
                result.type = RT_ERROR ;
                return ;
            }

            strcpy( name, result.name ) ;
            get_token() ;
            eval_subexpression() ;
            if ( result.type == RT_ERROR ) return ;
            if ( token.name[0] == ']' ) get_token() ;
            var2const() ;

            if ( result.type != RT_CONSTANT ) {
                console_printf( COLOR_RED "%s is not an integer" COLOR_SILVER, result.name ) ;
                result.type = RT_ERROR ;
                return ;
            }

            if ( result.int_value < 0 ) {
                console_printf( COLOR_RED "Index (%d) less than zero" COLOR_SILVER, result.int_value ) ;
                result.type = RT_ERROR ;
                return ;
            }

            if ( result.int_value >= i.Type.Count[0] ) {
                console_printf( COLOR_RED "Index (%d) out of bounds" COLOR_SILVER, result.int_value ) ;
                result.type = RT_ERROR ;
                return ;
            }

            result.type = RT_VARIABLE ;
            result.var = i ;
            result.var.Type = reduce_type( i.Type ) ;
            result.data = ( uint8_t * )i_data + ( int64_t )result.int_value * type_size( result.var.Type ) ;
            _snprintf( result.name, sizeof( result.name ), "%s[%"PRId64"]", name, ( int64_t )result.int_value ) ;
            continue ;
        }

        break ;
    }
}

/* --------------------------------------------------------------------------- */

static void eval_factor() {
    double double_base = 1.0 ;
    int base = 1;
    int op = 0 ;

    for ( ;; ) {
        eval_value() ;

        if ( result.type == RT_ERROR ) return ;

        if ( ( !token.name[0] || !strchr( "*/%", token.name[0] ) ) && !op ) return ;

        var2const() ;

        if ( result.type != RT_CONSTANT && result.type != RT_CONSTANT_DOUBLE ) {
            result.type = RT_ERROR ;
            console_printf( COLOR_RED "Operand is not a number" COLOR_SILVER ) ;
            return ;
        }

        if ( !op ) op = 1 ;

        if ( op > 1 && !result.int_value && result.type == RT_CONSTANT ) {
            result.type = RT_ERROR ;
            console_printf( COLOR_RED "Divide by zero" COLOR_SILVER ) ;
            return ;
        }

        switch( result.type ) {
            case RT_CONSTANT:
                if ( op == 1 ) base *= result.int_value ;
                if ( op == 2 ) base /= result.int_value ;
                if ( op == 3 ) base = ( int64_t ) base % ( int64_t ) result.int_value ;
                double_base = ( double ) base;
                break;

            case RT_CONSTANT_DOUBLE:
                if ( op == 1 ) double_base *= result.double_value ;
                if ( op == 2 ) double_base /= result.double_value ;
                if ( op == 3 ) double_base = fmod( double_base, result.double_value );
                base = ( int64_t ) double_base;
                break;

            default:
                break;
        }

        if ( ( !token.name[0] || !strchr( "*/%", token.name[0] ) ) ) {
            switch( result.type ) {
                case RT_CONSTANT:
                    result.int_value = base;
                    result.double_value = ( double ) result.int_value ;
                    _snprintf( result.name, sizeof( result.name ), "%"PRId64, result.int_value ) ;
                    break;

                case RT_CONSTANT_DOUBLE:
                    result.double_value = double_base;
                    result.int_value = ( int64_t ) result.double_value ;
                    _snprintf( result.name, sizeof( result.name ), "%F", result.double_value ) ;
                    break;

                default:
                    break;
            }
            return ;
        }

        op = ( token.name[0] == '*' ) ? 1 : ( token.name[0] == '/' ) ? 2 : 3 ;

        get_token() ;
    }
}

/* --------------------------------------------------------------------------- */

static void eval_subexpression() {
    double double_base = 0 ;
    int base = 0;
    int op = 0 ;

    for ( ;; ) {
        eval_factor() ;

        if ( result.type == RT_ERROR ) return ;

        if ( token.name[0] != '+' && token.name[0] != '-' && !op ) return ;

        var2const() ;

        if ( !op ) op = 1 ;

        switch ( result.type ) {
            case RT_CONSTANT:
                base += op * result.int_value ;
                double_base = ( double ) base;
                break;

            case RT_CONSTANT_DOUBLE:
                double_base += op * result.double_value ;
                base = ( int64_t ) double_base;
                break;

            default:
                console_printf( COLOR_RED "Operand is not a number" COLOR_SILVER ) ;
                result.type = RT_ERROR ;
                return ;
        }

        if ( token.name[0] != '+' && token.name[0] != '-' ) {
            switch( result.type ) {
                case RT_CONSTANT:
                    result.int_value = base;
                    result.double_value = ( double ) result.int_value ;
                    _snprintf( result.name, sizeof( result.name ), "%"PRId64, result.int_value ) ;
                    break;

                case RT_CONSTANT_DOUBLE:
                    result.double_value = double_base;
                    result.int_value = ( int64_t ) result.double_value ;
                    _snprintf( result.name, sizeof( result.name ), "%F", result.double_value ) ;
                    break;

                default:
                    break;
            }
            return ;
        }

        op = ( token.name[0] == '+' ) ? 1 : -1 ;

        get_token() ;
    }
}

/* --------------------------------------------------------------------------- */

static char * eval_expression( const char * here, int interactive ) {
    static char buffer[9216];
    static char part[8192];

    while ( *here == ' ' ) here++;

    token_ptr = here ;
    get_token() ;
    eval_subexpression() ;

    if ( token.type != NOTOKEN && token.name[0] != ',' && token.name[0] != '=' ) {
        if ( result.type != RT_ERROR ) {
            console_printf( COLOR_RED "Invalid expression" COLOR_SILVER );
            result.type = RT_ERROR;
        }
        return 0;
    }

    memset( part, 0, sizeof( part ) );
    strncpy( part, here, token_ptr - here - (( token.type != NOTOKEN ) ? 1 : 0 ) );

    if ( result.type == RT_CONSTANT ) {
        _snprintf( buffer, sizeof( buffer ), "%s = %"PRId64, part, result.int_value );
        if ( interactive ) console_printf( COLOR_SILVER "%s", buffer ) ;
    } else
    if ( result.type == RT_CONSTANT_DOUBLE ) {
        _snprintf( buffer, sizeof( buffer ), "%s = %F", part, result.double_value );
        if ( interactive ) console_printf( COLOR_SILVER "%s", buffer ) ;
    } else
    if ( result.type == RT_STRING ) {
        if ( interactive ) console_printf( COLOR_SILVER "%s = \"%s\"", part, result.name ) ;
    } else
    if ( result.type == RT_VARIABLE ) {
        lvalue = result ;

        if ( token.name[0] == '=' ) {
            if ( lvalue.type != RT_VARIABLE ) {
                strcpy( buffer, "Not an lvalue" ) ;
                if ( interactive ) console_printf( COLOR_RED "%s" COLOR_SILVER, buffer ) ;
                return buffer ;
            }
            get_token() ;
            eval_subexpression() ;

            if ( result.type == RT_ERROR ) return "" ;

            var2const() ;

            if (( lvalue.var.Type.BaseType[0] == TYPE_QWORD || lvalue.var.Type.BaseType[0] == TYPE_INT ) && result.type == RT_CONSTANT )
                *( int64_t * )( lvalue.data ) = ( int64_t )result.int_value ;
            else
            if (( lvalue.var.Type.BaseType[0] == TYPE_DWORD || lvalue.var.Type.BaseType[0] == TYPE_INT32 ) && result.type == RT_CONSTANT )
                *( int32_t * )( lvalue.data ) = ( int32_t )result.int_value ;
            else
            if (( lvalue.var.Type.BaseType[0] == TYPE_WORD || lvalue.var.Type.BaseType[0] == TYPE_SHORT ) && result.type == RT_CONSTANT )
                *( uint16_t * )( lvalue.data ) = ( uint16_t )result.int_value ;
            else
            if (( lvalue.var.Type.BaseType[0] == TYPE_BYTE || lvalue.var.Type.BaseType[0] == TYPE_SBYTE ) && result.type == RT_CONSTANT )
                *( uint8_t * )( lvalue.data ) = ( uint8_t )result.int_value ;
            else
            if ( lvalue.var.Type.BaseType[0] == TYPE_DOUBLE && result.type == RT_CONSTANT )
                *( double * )( lvalue.data ) = ( double )result.int_value ;
            else
            if ( lvalue.var.Type.BaseType[0] == TYPE_FLOAT && result.type == RT_CONSTANT )
                *( float * )( lvalue.data ) = ( float )result.int_value ;
            else
            if (( lvalue.var.Type.BaseType[0] == TYPE_QWORD || lvalue.var.Type.BaseType[0] == TYPE_INT ) && result.type == RT_CONSTANT_DOUBLE )
                *( int64_t * )( lvalue.data ) = ( int64_t )result.double_value ;
            else
            if (( lvalue.var.Type.BaseType[0] == TYPE_DWORD || lvalue.var.Type.BaseType[0] == TYPE_INT32 ) && result.type == RT_CONSTANT_DOUBLE )
                *( int32_t * )( lvalue.data ) = ( int32_t )result.double_value ;
            else
            if (( lvalue.var.Type.BaseType[0] == TYPE_WORD || lvalue.var.Type.BaseType[0] == TYPE_SHORT ) && result.type == RT_CONSTANT_DOUBLE )
                *( uint16_t * )( lvalue.data ) = ( uint16_t )result.double_value ;
            else
            if (( lvalue.var.Type.BaseType[0] == TYPE_BYTE || lvalue.var.Type.BaseType[0] == TYPE_SBYTE ) && result.type == RT_CONSTANT_DOUBLE )
                *( uint8_t * )( lvalue.data ) = ( uint8_t )result.double_value ;
            else
            if ( lvalue.var.Type.BaseType[0] == TYPE_DOUBLE && result.type == RT_CONSTANT_DOUBLE )
                *( double * )( lvalue.data ) = ( double )result.double_value ;
            else
            if ( lvalue.var.Type.BaseType[0] == TYPE_FLOAT && result.type == RT_CONSTANT_DOUBLE )
                *( float * )( lvalue.data ) = ( float )result.double_value ;
            else
            if ( lvalue.var.Type.BaseType[0] == TYPE_CHAR && result.type == RT_STRING ) {
                if ( *result.name == '\\' && *( result.name + 1 ) == 'x' )
                    *( uint8_t * )( lvalue.data ) = ( uint8_t )strtol( result.name + 2, NULL, 16 );
                else
                    *( uint8_t * )( lvalue.data ) = ( uint8_t ) * ( result.name );
            } else
            if ( lvalue.var.Type.BaseType[0] == TYPE_STRING && result.type == RT_STRING ) {
                string_discard( *( uint32_t * ) lvalue.data ) ;
                *( uint32_t * )( lvalue.data ) = string_new( ( const unsigned char * ) result.name ) ;
                string_use( *( uint32_t * ) lvalue.data ) ;
            }
            else
            {
                strcpy( buffer, "Invalid assignation" ) ;
                if ( interactive ) console_printf( COLOR_RED "%s" COLOR_SILVER, buffer ) ;
                return buffer ;
            }
        }

        if ( interactive ) {
            show_var( lvalue.var, lvalue.name, lvalue.data, "", 0 ) ;
        } else {
            strcpy( buffer, part ) ;
            strcat( buffer, " " ) ;
            strcat( buffer, show_value( lvalue.var.Type, lvalue.data ) );
        }
    }

    if ( token.name[0] == ',' ) {
        char * temporary = strdup( buffer );
        int    size = strlen( temporary );

        if ( eval_expression( token_ptr, interactive ) == 0 ) {
            free( temporary );
            return 0;
        }
        if ( strlen( buffer ) + size < 1020 && !interactive ) {
            memmove( buffer + size + 2, buffer, strlen( buffer ) + 1 );
            memcpy( buffer, temporary, size );
            memcpy( buffer + size, ", ", 2 );
        }
        free( temporary );
        return buffer;
    }

    return buffer;
}

/* --------------------------------------------------------------------------- */

static void console_instance_dump( INSTANCE * father, int indent ) {
    INSTANCE * i, * next ;
    PROCDEF * proc ;
    DCB_PROC * dcbproc ;
    char buffer[128] ;
    char line[128+32] ;
    int n, nid ;
    int64_t son ;

    i = father ;
    if ( !father ) i = first_instance ;

    proc = i->proc ;
    dcbproc = &dcb.proc[proc->type] ;

    nid = LOCINT64( libmod_debug, i, PROCESS_ID ) ;
    if ( dcb.data.NSourceFiles && dcbproc->data.ID )
        sprintf( buffer, "%s", getid_name( dcbproc->data.ID ) ) ;
    else
        sprintf( buffer, "%s", ( proc->type == 0 ) ? "MAIN" : "PROC" ) ;

    line[0] = 0 ;
    for ( n = 0 ; n < indent - 1; n++ ) strcat( line, " \x03 " );

    sprintf( line + strlen( line ), " \x01\x02 %-12s ", buffer ) ;

    n = strlen( line ) % 4 ;
    while ( n && ( n++ < 4 ) ) strcat( line, " " ) ;
    sprintf( line + strlen( line ), "%7d", nid ) ;

    if ( LOCQWORD( libmod_debug, i, STATUS ) & STATUS_WAITING_MASK ) strcat( line, "[W]" );
    if ( LOCQWORD( libmod_debug, i, STATUS ) & STATUS_PAUSED_MASK ) strcat( line, "[P]" );
    switch ( LOCQWORD( libmod_debug, i, STATUS ) & ~( STATUS_WAITING_MASK | STATUS_PAUSED_MASK ) ) {
        case STATUS_DEAD        :   strcat( line, "[D]" ) ; break ;
        case STATUS_KILLED      :   strcat( line, "[K]" ) ; break ;
        case STATUS_RUNNING     :   strcat( line, "   " ) ; break ;
        case STATUS_SLEEPING    :   strcat( line, "[S]" ) ; break ;
        case STATUS_FROZEN      :   strcat( line, "[F]" ) ; break ;
    }

    console_printf( COLOR_SILVER "%s", line ) ;

    if ( !( son = LOCINT64( libmod_debug, i, SON ) ) ) return ;

    next = instance_get( son ) ;
    if ( !next ) console_printf( COLOR_RED "\12**PANIC**\7 SON %d does not exist" COLOR_SILVER, son ) ;

    i = next ;

    while ( i ) {
        proc = i->proc ;
        dcbproc = &dcb.proc[proc->type] ;

        if ( LOCINT64( libmod_debug, i, SON ) ) {
            console_instance_dump( i, indent + 1 ) ;
        } else {
            nid = LOCINT64( libmod_debug, i, PROCESS_ID ) ;
            if ( dcb.data.NSourceFiles && dcbproc->data.ID ) {
                sprintf( buffer, "%s", getid_name( dcbproc->data.ID ) ) ;
            } else {
                sprintf( buffer, "%s", ( proc->type == 0 ) ? "MAIN" : "PROC" ) ;
            }

            line[0] = 0 ;
            for ( n = 0 ; n < indent; n++ ) strcat( line, " \x03 " );

            sprintf( line + strlen( line ), " \x01\x02 %-12s ", buffer ) ;

            n = strlen( line ) % 4 ;
            while ( n && ( n++ < 4 ) ) strcat( line, " " ) ;
            sprintf( line + strlen( line ), "%7d", nid ) ;

            if ( LOCQWORD( libmod_debug, i, STATUS ) & STATUS_WAITING_MASK ) strcat( line, "[W]" );
            if ( LOCQWORD( libmod_debug, i, STATUS ) & STATUS_PAUSED_MASK ) strcat( line, "[P]" );
            switch ( LOCQWORD( libmod_debug, i, STATUS ) & ~( STATUS_WAITING_MASK | STATUS_PAUSED_MASK ) ) {
                case STATUS_DEAD        :   strcat( line, "[D]" ) ; break ;
                case STATUS_KILLED      :   strcat( line, "[K]" ) ; break ;
                case STATUS_RUNNING     :   strcat( line, "   " ) ; break ;
                case STATUS_SLEEPING    :   strcat( line, "[S]" ) ; break ;
                case STATUS_FROZEN      :   strcat( line, "[F]" ) ; break ;
            }
            console_printf( COLOR_SILVER "%s", line ) ;
        }

        int64_t bigbro;
        if ( ( bigbro = LOCINT64( libmod_debug, i, BIGBRO ) ) ) {
            next = instance_get( bigbro ) ;
            if ( !next ) console_printf( COLOR_RED "\12**PANIC**\7 BIGBRO %d does not exist" COLOR_SILVER, bigbro ) ;
            i = next ;
        } else
            break ;
    }
}

/* --------------------------------------------------------------------------- */
/* Muestra el árbol de instancias */

static void console_instance_dump_all() {
    INSTANCE * i ;

    console_printf( COLOR_GREEN "INSTANCES TREE" COLOR_SILVER "\n\n" );

    for ( i = first_instance ; i ; i = i->next ) {
        int64_t father;
        if ( !( father = LOCINT64( libmod_debug, i, FATHER )) || !instance_get( father ) ) {
            console_instance_dump( i, 1 ) ;
        }
    }

    console_printf( "\n" ) ;
}

/* --------------------------------------------------------------------------- */

static void console_instance_dump_all_brief() {
    INSTANCE * i ;
    char status[32] ;
    int sz = 20 + strlen( COLOR_BLACK );

    console_printf( COLOR_GREEN "INSTANCES BRIEF LIST" COLOR_SILVER "\n\n" );
    console_printf( COLOR_GREEN "Id         Father     Status               Name" COLOR_SILVER "\n" );

    for ( i = first_instance ; i ; i = i->next ) {
        status[0] = '\0';
        if ( LOCQWORD( libmod_debug, i, STATUS ) & STATUS_WAITING_MASK ) strcat( status, "wait+" );
        if ( LOCQWORD( libmod_debug, i, STATUS ) & STATUS_PAUSED_MASK  ) strcat( status, "pause+" );
        switch ( LOCQWORD( libmod_debug, i, STATUS ) & ~( STATUS_WAITING_MASK | STATUS_PAUSED_MASK ) ) {
            case STATUS_DEAD        :   strcat( status, COLOR_OLIVE  "dead"     ) ; break ;
            case STATUS_KILLED      :   strcat( status, COLOR_RED    "killed"   ) ; break ;
            case STATUS_SLEEPING    :   strcat( status, COLOR_BLUE   "sleeping" ) ; break ;
            case STATUS_FROZEN      :   strcat( status, COLOR_TEAL   "frozen"   ) ; break ;
            case STATUS_RUNNING     :   strcat( status, COLOR_LIME   "running"  ) ; break ;
        }

        int64_t father = LOCINT64( libmod_debug, i, FATHER );

        console_printf( "%-10d %s%-10d" COLOR_GRAY " %-*.*s " COLOR_WHITE "%s" COLOR_SILVER "\n",
                LOCINT64( libmod_debug, i, PROCESS_ID ),
                instance_get( father ) ? "" : COLOR_RED,
                father,
                sz,
                sz,
                status,
                ( dcb.data.NSourceFiles && dcb.proc[i->proc->type].data.ID ) ? getid_name( dcb.proc[i->proc->type].data.ID ) : (( i->proc->type == 0 ) ? "Main" : "proc" )
            ) ;
    }

    console_printf( "\n" ) ;
}

/* --------------------------------------------------------------------------- */

static int console_list_current = 0;
static int console_list_y_pos[2] = { 0 };
static int console_list_x_pos[2] = { 0 };
static int console_list_y_max[2] = { 0 };
static INSTANCE * console_list_current_instance = NULL;

#define WINDOW_LIST_COLS    40

static void show_list_window() {
    int n;
    int pos = console_list_y_pos[console_list_current] > console_y / CHARHEIGHT - 2 ? console_list_y_pos[console_list_current] - ( console_y / CHARHEIGHT - 2 ) : 0 ;
    int x = ( scr_width - console_columns * CHARWIDTH ) / 2 + ( console_columns - WINDOW_LIST_COLS ) * CHARWIDTH ;
    int y = 0;
    char * font_color = NULL;

    int64_t __drawing_blend_mode = drawing_blend_mode;
    uint8_t __drawing_color_r = drawing_color_r;
    uint8_t __drawing_color_g = drawing_color_g;
    uint8_t __drawing_color_b = drawing_color_b;
    uint8_t __drawing_color_a = drawing_color_a;

    systext_color( 0x000000 ) ;

    drawing_blend_mode = 0; drawing_color_r = 0x00; drawing_color_g = 0xC0; drawing_color_b = 0xC0; drawing_color_a = console_alpha;
    draw_rectangle_filled( NULL, NULL, x, y, WINDOW_LIST_COLS * CHARWIDTH, CHARHEIGHT );

    switch ( console_list_current ) {
        case    0:
            console_list_y_max[0] = procdef_count ;

            systext_puts( x, y, COLOR_BLACK "PROCESS TYPES" );
            for ( n = 0 ; pos + n < procdef_count && y < console_y - CHARHEIGHT ; n++ ) {
                y += CHARHEIGHT;

                font_color = COLOR_WHITE;

                if ( console_list_y_pos[console_list_current] == pos + n ) {
                    if ( procs[pos + n].breakpoint )
                        drawing_blend_mode = 0, drawing_color_r = 0x80, drawing_color_g = 0x00, drawing_color_b = 0x80, drawing_color_a = console_alpha;
                    else
                        drawing_blend_mode = 0, drawing_color_r = 0x00, drawing_color_g = 0x00, drawing_color_b = 0x80, drawing_color_a = console_alpha;
                } else {
                    if ( procs[pos + n].breakpoint )
                        drawing_blend_mode = 0, drawing_color_r = 0x80, drawing_color_g = 0x00, drawing_color_b = 0x00, drawing_color_a = console_alpha;
                    else
                        font_color = COLOR_BLACK, drawing_blend_mode = 0, drawing_color_r = 0xC0, drawing_color_g = 0xC0, drawing_color_b = 0xC0, drawing_color_a = console_alpha;
                }
                draw_rectangle_filled( NULL, NULL, x, y, WINDOW_LIST_COLS * CHARWIDTH, CHARHEIGHT );
                systext_puts( x, y, font_color );
                if ( console_list_x_pos[console_list_current] < strlen( procs[pos + n].name ) )
                    systext_puts( x, y, &procs[pos + n].name[console_list_x_pos[console_list_current]] );
            }
            break;

        case    1:
        {
            INSTANCE * i ;
            int c ;
            char line[128];
            char status[10];

            console_list_y_max[1] = 0 ;

            systext_puts( x, y, COLOR_BLACK "INSTANCES" );

            for ( c = 0, n = 0, i = first_instance ; i ; i = i->next, c++ ) {
                console_list_y_max[1]++ ;

                if ( c < pos || y >= console_y - CHARHEIGHT ) continue ;
                y += CHARHEIGHT;

                font_color = COLOR_WHITE;

                if ( console_list_y_pos[console_list_current] == pos + n ) {
                    console_list_current_instance = i;
                    if ( i->breakpoint )
                        drawing_blend_mode = 0, drawing_color_r = 0x80, drawing_color_g = 0x00, drawing_color_b = 0x80, drawing_color_a = console_alpha;
                    else
                        drawing_blend_mode = 0, drawing_color_r = 0x00, drawing_color_g = 0x00, drawing_color_b = 0x80, drawing_color_a = console_alpha;
                } else {
                    if ( i->breakpoint )
                        drawing_blend_mode = 0, drawing_color_r = 0x80, drawing_color_g = 0x00, drawing_color_b = 0x00, drawing_color_a = console_alpha;
                    else
                        font_color = COLOR_BLACK, drawing_blend_mode = 0, drawing_color_r = 0xC0, drawing_color_g = 0xC0, drawing_color_b = 0xC0, drawing_color_a = console_alpha;
                }

                draw_rectangle_filled( NULL, NULL, x, y, WINDOW_LIST_COLS * CHARWIDTH, CHARHEIGHT );
                systext_puts( x, y, font_color );

                status[0] = '\0';
                if ( LOCQWORD( libmod_debug, i, STATUS ) & STATUS_WAITING_MASK ) strcat( status, "[W]" );
                if ( LOCQWORD( libmod_debug, i, STATUS ) & STATUS_PAUSED_MASK  ) strcat( status, "[P]" );
                switch ( LOCQWORD( libmod_debug, i, STATUS ) & ~( STATUS_WAITING_MASK | STATUS_PAUSED_MASK ) ) {
                    case STATUS_DEAD        :   strcat( status, "[D]" ) ; break ;
                    case STATUS_KILLED      :   strcat( status, "[K]" ) ; break ;
                    case STATUS_SLEEPING    :   strcat( status, "[S]" ) ; break ;
                    case STATUS_FROZEN      :   strcat( status, "[F]" ) ; break ;
                    case STATUS_RUNNING     :   strcat( status, "[R]" ) ; break ;
                }

                sprintf( line, "%"PRIu64" %-9.9s %s",
                        LOCINT64( libmod_debug, i, PROCESS_ID ),
                        status,
                        ( dcb.data.NSourceFiles && dcb.proc[i->proc->type].data.ID ) ? getid_name( dcb.proc[i->proc->type].data.ID ) : (( i->proc->type == 0 ) ? "Main" : "proc" )
                       );
                if ( console_list_x_pos[console_list_current] < strlen( line ) )
                    systext_puts( x, y, &line[console_list_x_pos[console_list_current]] );

                n++ ;
            }
        }
        break;
    }
    drawing_blend_mode = __drawing_blend_mode; drawing_color_r = __drawing_color_r; drawing_color_g = __drawing_color_g; drawing_color_b = __drawing_color_b; drawing_color_a = __drawing_color_a;
}

/* --------------------------------------------------------------------------- */

static INSTANCE * findproc( INSTANCE * last, char * action, char * ptr ) {
    INSTANCE * i = NULL;

    if ( *ptr ) {
        int64_t procno;
        if ( *ptr >= '0' && *ptr <= '9' ) {
            if ( last ) return NULL;
            procno = atol( ptr );
            for ( i = first_instance ; i ; i = i->next )
                if ( LOCINT64( libmod_debug, i, PROCESS_ID ) == procno )
                    break;
            if ( !i ) {
                console_printf( COLOR_RED "Instance %d does not exist" COLOR_SILVER, procno );
                return NULL;
            }
        } else {
            char * aptr = action;
            while ( ISWORDCHAR( *ptr ) ) {
                *aptr++ = TOUPPER( *ptr ); ptr++;
            }
            *aptr = 0;

            int n;

            for ( n = 0 ; n < ( int64_t )dcb.data.NID ; n++ )
                if ( strcmp( ( char * )dcb.id[n].Name, action ) == 0 )
                    break;
            for ( procno = 0 ; procno < ( int64_t )dcb.data.NProcs ; procno++ )
                if ( dcb.proc[procno].data.ID == dcb.id[n].Code )
                    break;
            if ( procno == ( int64_t )dcb.data.NProcs ) {
                console_printf( COLOR_RED "Unknown process %s" COLOR_SILVER, action );
                return NULL;
            }
            for ( i = last ? last->next : first_instance ; i ; i = i->next )
                if ( i->proc->type == procno )
                    break;
            if ( !i && !last ) {
                console_printf( COLOR_RED "No instance of process %s created" COLOR_SILVER, action );
                return NULL;
            }
        }
    }

    return ( i );
}

/* --------------------------------------------------------------------------- */

static void console_do( const char * cmd ) {
    char * ptr, * aptr ;
    char action[4096] ;
    unsigned int var ;
    int64_t n, procno = 0;
    PROCDEF * p = NULL ;
    INSTANCE * i = NULL ;

    ptr = ( char * ) cmd ;
    while ( *ptr && *ptr != ' ' && *ptr != '=' ) ptr++ ;
    strncpy( action, cmd, ptr - cmd ) ;
    action[ptr - cmd] = 0 ;
    while ( *ptr == ' ' ) ptr++ ;
    aptr = action ;
    while ( *aptr ) {
        *aptr = TOUPPER( *aptr );
        aptr++;
    }

    /* Comandos */

    if ( strcmp( action, "HELP" ) == 0 ) {
        console_printf( HELPTXT );
        return;
    }

    if ( strcmp( action, "GO" ) == 0 ) {
        debugger_show_console = 0;
        debugger_trace = 0;
        debugger_step = 0;
        debug_on_frame = 0;
        break_on_next_proc = 0;
        return ;
    }

    if ( strcmp( action, "NEXTFRAME" ) == 0 ) {
        debugger_show_console = 0;
        debugger_trace = 0;
        debugger_step = 0;
        debug_on_frame = 1;
        break_on_next_proc = 0;
        return ;
    }

    if ( strcmp( action, "NEXTPROC" ) == 0 ) {
        debugger_show_console = 0;
        debugger_trace = 0;
        debugger_step = 0;
        debug_on_frame = 0;
        break_on_next_proc = 1;
        return ;
    }

    if ( strcmp( action, "TRACE" ) == 0 ) {
        debugger_show_console = 0;
        debugger_trace = 1;
        debugger_step = 0;
        debug_on_frame = 0;
        break_on_next_proc = 0;
        return ;
    }

    if ( strcmp( action, "STEP" ) == 0 ) {
        debugger_show_console = 0;
        debugger_trace = 0;
        debugger_step = 1;
        debug_on_frame = 0;
        break_on_next_proc = 0;
        return ;
    }

    if ( strcmp( action, "BREAK" ) == 0 ) {
        if ( *ptr ) {
            if ( *ptr >= '0' && *ptr <= '9' ) {
                procno = atoi( ptr );
                for ( i = first_instance ; i ; i = i->next )
                    if ( LOCINT64( libmod_debug, i, PROCESS_ID ) == procno )
                        break;
                if ( !i ) {
                    console_printf( COLOR_RED "Instance %d does not exist" COLOR_SILVER, procno );
                } else {
                    i->breakpoint = 1;
                    console_printf( COLOR_SILVER "OK" );
                }
            } else {
                aptr = action;
                while ( ISWORDCHAR( *ptr ) ) {
                    *aptr++ = TOUPPER( *ptr ); ptr++;
                }
                *aptr = 0;

                if ( *action ) {
                    p = procdef_get_by_name( action );
                    if ( !p ) {
                        console_printf( COLOR_RED "Process type %d does not exist" COLOR_SILVER, action );
                    } else {
                        p->breakpoint = 1;
                        console_printf( COLOR_SILVER "OK" );
                    }
                }
            }
        } else {
            int f = 0;
            for ( n = 0 ; n < procdef_count; n++ ) {
                if ( procs[n].breakpoint ) {
                    if ( !f ) {
                        console_printf( COLOR_GREEN "PROCESS TYPE BREAKPOINTS" COLOR_SILVER "\n\n" );
                        f = 1;
                    }
                    console_printf( COLOR_SILVER "%s\n", procs[n].name );
                }
            }
            if ( f )
                console_printf( "\n" );

            f = 0;
            for ( i = first_instance ; i ; i = i->next ) {
                if ( i->breakpoint ) {
                    if ( !f ) {
                        console_printf( COLOR_GREEN "PROCESS BREAKPOINTS" COLOR_SILVER "\n\n" );
                        f = 1;
                    }
                    console_printf( COLOR_SILVER "%"PRId64, LOCINT64( libmod_debug, i, PROCESS_ID ) );
                }
            }
            if ( f ) console_printf( "\n" );
        }
        return ;
    }

    if ( strcmp( action, "BREAKALL" ) == 0 ) {
        for ( i = first_instance ; i ; i = i->next ) i->breakpoint = 1;
        console_printf( COLOR_SILVER "OK" );
        return ;
    }

    if ( strcmp( action, "BREAKALLTYPES" ) == 0 ) {
        for ( n = 0 ; n < procdef_count; n++ ) procs[n].breakpoint = 1;
        console_printf( COLOR_SILVER "OK" );
        return ;
    }

    if ( strcmp( action, "DELETEALL" ) == 0 ) {
        for ( i = first_instance ; i ; i = i->next ) i->breakpoint = 0;
        console_printf( COLOR_SILVER "OK" );
        return ;
    }

    if ( strcmp( action, "DELETEALLTYPES" ) == 0 ) {
        for ( n = 0 ; n < procdef_count; n++ ) procs[n].breakpoint = 0;
        console_printf( COLOR_SILVER "OK" );
        return ;
    }

    if ( strcmp( action, "DELETE" ) == 0 ) {
        if ( *ptr ) {
            if ( *ptr >= '0' && *ptr <= '9' ) {
                procno = atoi( ptr );
                for ( i = first_instance ; i ; i = i->next )
                    if ( LOCINT64( libmod_debug, i, PROCESS_ID ) == procno )
                        break;
                if ( !i ) {
                    console_printf( COLOR_RED "Instance %d does not exist" COLOR_SILVER, procno );
                } else {
                    i->breakpoint = 0;
                    console_printf( COLOR_SILVER "OK" );
                }
            } else {
                aptr = action;
                while ( ISWORDCHAR( *ptr ) ) {
                    *aptr++ = TOUPPER( *ptr ); ptr++;
                }
                *aptr = 0;

                p = procdef_get_by_name( action );
                if ( !p ) {
                    console_printf( COLOR_RED "Process type %d does not exist" COLOR_SILVER, procno );
                } else {
                    p->breakpoint = 0;
                    console_printf( COLOR_SILVER "OK" );
                }
            }
        }
        return ;
    }

    if ( strcmp( action, "STRINGS" ) == 0 ) {
        string_dump(console_printf) ;
        return ;
    }

    if ( strcmp( action, "INSTANCES" ) == 0 ) {
        console_instance_dump_all() ;
        return ;
    }

    if ( strcmp( action, "GLOBALS" ) == 0 ) {
        for ( var = 0 ; var < dcb.data.NGloVars ; var++ ) {
            DCB_VAR * v = &dcb.glovar[var] ;
            show_var( *v, 0, ( uint8_t * )globaldata + v->Offset, "[GLO]", 0 ) ;
        }
        console_printf( "\n" );
        return ;
    }

    if (
        strcmp( action, "LOCALS" ) == 0 ||
        strcmp( action, "PRIVATES" ) == 0 ||
        strcmp( action, "PUBLICS" ) == 0 ) {
        int show_locals = action[0] == 'L';
        int show_public = action[0] == 'P' && action[1] == 'U' ;

        i = findproc( NULL, action, ptr );

        if ( show_locals ) {
            for ( var = 0 ; var < dcb.data.NLocVars ; var++ ) {
                DCB_VAR * v = &dcb.locvar[var] ;
                show_var( *v, 0, i ? ( char* )i->locdata + v->Offset : 0, "[LOC]", 0 ) ;
            }
        } else if ( show_public ) {
            if ( !i ) {
                console_printf( COLOR_SILVER "Use: PUBLICS process" );
                return;
            }
            for ( var = 0 ; var < dcb.proc[i->proc->type].data.NPubVars ; var++ ) {
                DCB_VAR * v = &dcb.proc[i->proc->type].pubvar[var] ;

                /* Unnamed private vars are temporary params and loop
                   counters, and are ignored by this command */
                if (( int64_t )v->ID >= 0 ) {
                    show_var( *v, 0, ( char* )i->pubdata + v->Offset, "[PUB]", 0 ) ;
                }
            }
        } else {
            if ( !i ) {
                console_printf( COLOR_SILVER "Use: PRIVATES process" );
                return;
            }
            for ( var = 0 ; var < dcb.proc[i->proc->type].data.NPriVars ; var++ ) {
                DCB_VAR * v = &dcb.proc[i->proc->type].privar[var] ;

                /* Unnamed private vars are temporary params and loop
                   counters, and are ignored by this command */
                if (( int64_t )v->ID >= 0 ) {
                    show_var( *v, 0, ( char* )i->pridata + v->Offset, "[PRI]", 0 ) ;
                }
            }
        }
        console_printf( "\n" );
        return ;
    }

    if ( strcmp( action, "SHOW" ) == 0 ) {
        if ( *ptr ) {
            char * res = eval_expression( ptr, 0 );

            if ( !res || result.type == RT_STRING ) {
                console_printf( COLOR_RED "Invalid argument" COLOR_SILVER );
                return ;
            }

            for ( n = 0; n < MAX_EXPRESSIONS; n++ ) {
                if ( show_expression[n] && !strcmp( show_expression[n], ptr ) ) {
                    console_printf( COLOR_RED "Already exists" COLOR_SILVER );
                    return ;
                }
            }

            for ( n = 0; n < MAX_EXPRESSIONS; n++ ) {
                if ( !show_expression[n] ) {
                    show_expression[n] = strdup( ptr );
                    show_expression_count++;
                    console_printf( COLOR_SILVER "OK" );
                    return;
                }
            }

            console_printf( COLOR_RED "No more expressions are possibles" COLOR_SILVER );
        } else {
            int nn = 0;
            for ( n = 0; n < MAX_EXPRESSIONS; n++ ) {
                if ( show_expression[n] ) {
                    console_printf( "%d: %s", n + 1, show_expression[n] );
                    nn++;
                }
            }

            if ( !nn ) console_printf( COLOR_RED "No expressions availables" COLOR_SILVER );
        }

        return;
    }


    if ( strcmp( action, "SHOWDEL" ) == 0 ) {
        if ( *ptr ) {
//            char * p = ptr;
//            while( ISNUM( *p ) ) p++;

            if ( ISNUM( *ptr ) ) {
                int pos = atol( ptr ) - 1;
                if ( pos >= 0 && pos < MAX_EXPRESSIONS && show_expression[pos] ) {
                    free( show_expression[pos] );
                    show_expression[pos] = NULL;
                    show_expression_count--;
                    console_printf( COLOR_SILVER "OK" );
                    return;
                }
            }
        }

        console_printf( COLOR_RED "Invalid argument" COLOR_SILVER );

        return;
    }

    if ( strcmp( action, "SHOWDELALL" ) == 0 ) {
        for ( n = 0; n < MAX_EXPRESSIONS; n++ ) {
            if ( show_expression[n] ) {
                free( show_expression[n] );
                show_expression[n] = NULL;
                show_expression_count--;
            }
        }
        console_printf( COLOR_SILVER "OK" );
        return;
    }

    if ( strcmp( action, "VARS" ) == 0 ) {
        for ( var = 0 ; var < N_CONSOLE_VARS ; var++ ) {
            switch ( console_vars[var].type ) {
                case CON_QWORD:
                    console_printf( COLOR_SILVER "%s = %"PRIu64"\n", console_vars[var].name, *( uint64_t * )console_vars[var].value ) ;
                    break;

                case CON_QWORD_HEX:
                    console_printf( COLOR_SILVER "%s = %016Xh\n", console_vars[var].name, *( uint64_t * )console_vars[var].value ) ;
                    break;

                case CON_DWORD:
                    console_printf( COLOR_SILVER "%s = %"PRIu32"\n", console_vars[var].name, *( uint32_t * )console_vars[var].value ) ;
                    break;

                case CON_DWORD_HEX:
                    console_printf( COLOR_SILVER "%s = %08Xh\n", console_vars[var].name, *( uint32_t * )console_vars[var].value ) ;
                    break;

                case CON_WORD:
                    console_printf( COLOR_SILVER "%s = %u\n", console_vars[var].name, *( uint16_t * )console_vars[var].value ) ;
                    break;

                case CON_WORD_HEX:
                    console_printf( COLOR_SILVER "%s = %04Xh\n", console_vars[var].name, *( uint16_t * )console_vars[var].value ) ;
                    break;

                case CON_BYTE:
                    console_printf( COLOR_SILVER "%s = %u\n", console_vars[var].name, *( uint8_t * )console_vars[var].value ) ;
                    break;

                case CON_BYTE_HEX:
                    console_printf( COLOR_SILVER "%s = %02Xh\n", console_vars[var].name, *( uint8_t * )console_vars[var].value ) ;
                    break;
            }
        }
        console_printf( "\n" );
        return ;
    }

    if ( strcmp( action, "RUN" ) == 0 ) {
        if ( *ptr ) {
            aptr = action;
            while ( ISWORDCHAR( *ptr ) ) {
                *aptr++ = TOUPPER( *ptr ); ptr++;
            }
            *aptr = 0;

            if ( *action ) {
                p = procdef_get_by_name( action );
                if ( p ) {
                    token_ptr = ptr ;
                    console_printf( COLOR_SILVER "%s", ptr );
                    INSTANCE * inst = instance_new( p, NULL );

                    for ( n = 0; n < p->params; n++ ) {
                        int type = dcb.proc[ p->type ].privar[ n ].Type.BaseType[0];
                        get_token() ;
                        eval_subexpression() ;

                        if ( result.type == RT_VARIABLE )
                            var2const();

                        switch ( result.type ) {
                            case RT_CONSTANT:
                                switch ( type ) {
                                    case    TYPE_DOUBLE:
                                        if ( result.type == RT_CONSTANT ) {
                                            double v = ( double ) result.int_value;
                                            PRIQWORD( inst, sizeof( int64_t ) * n ) = *( int64_t * ) &v;
                                        }
                                        else {
                                            PRIQWORD( inst, sizeof( int64_t ) * n ) = *( int64_t * ) &result.double_value;
                                        }
                                        break;

                                    case    TYPE_FLOAT:
                                        if ( result.type == RT_CONSTANT ) {
                                            float v = ( float ) result.int_value;
                                            PRIQWORD( inst, sizeof( int64_t ) * n ) = *( int32_t * ) &v;
                                        }
                                        else {
                                            float v = ( float ) result.double_value;
                                            PRIQWORD( inst, sizeof( int64_t ) * n ) = *( int32_t * ) &result.double_value;
                                        }
                                        break;

                                    case    TYPE_INT:
                                    case    TYPE_QWORD:
                                    case    TYPE_POINTER:
                                        PRIQWORD( inst, sizeof( int64_t ) * n ) = ( int64_t ) ( result.type == RT_CONSTANT ? result.int_value : result.double_value );
                                        break;

                                    case    TYPE_INT32:
                                    case    TYPE_DWORD:
                                    {
                                        int32_t v = (int32_t) ( result.type == RT_CONSTANT ? result.int_value : result.double_value );
                                        PRIQWORD( inst, sizeof( int64_t ) * n ) = ( int64_t ) v;
                                        break;
                                    }

                                    case    TYPE_SHORT:
                                    case    TYPE_WORD:
                                    {
                                        int16_t v = (int16_t) ( result.type == RT_CONSTANT ? result.int_value : result.double_value );
                                        PRIQWORD( inst, sizeof( int64_t ) * n ) = ( int64_t ) v;
                                        break;
                                    }

                                    case    TYPE_BYTE:
                                    case    TYPE_SBYTE:
                                    case    TYPE_CHAR:
                                    {
                                        int8_t v = (int8_t) ( result.type == RT_CONSTANT ? result.int_value : result.double_value );
                                        PRIQWORD( inst, sizeof( int64_t ) * n ) = ( int64_t ) v;
                                        break;
                                    }

                                    case    TYPE_STRING:
                                    default:
                                        instance_destroy( inst );
                                        console_printf( COLOR_RED "Invalid argument %d" COLOR_SILVER, n );
                                        return;
                                }
                                break;

                            case RT_STRING:
                                PRIQWORD( inst, sizeof( int64_t ) * n ) = ( int64_t ) string_new( ( const unsigned char * ) result.name ) ;
                                string_use( PRIQWORD( inst, sizeof( int64_t ) * n ) );
                                break;

                            case RT_VARIABLE:
                            default:
                                instance_destroy( inst );
                                console_printf( COLOR_RED "Invalid argument %d" COLOR_SILVER, n );
                                return;
                        }
                    }
                    console_printf( COLOR_SILVER "Process %s is executed", p->name );
                } else {
                    console_printf( COLOR_RED "Process %s not found" COLOR_SILVER, action );
                }
                return;
            }
        }
    }

    if (
        strcmp( action, "KILLALL" ) == 0 ||
        strcmp( action, "SLEEPALL" ) == 0 ||
        strcmp( action, "WAKEUPALL" ) == 0 ||
        strcmp( action, "FREEZEALL" ) == 0 ) {
        char    act = *action;
        int     found = 0;
        char    * oaction = strdup( action );
        char    * optr = ptr;

        i = NULL;
        while (( i = findproc( i, action, ptr ) ) ) {
            found = 1;
            switch ( act ) {
                case 'K':
                    LOCQWORD( libmod_debug, i, STATUS ) = ( LOCQWORD( libmod_debug, i, STATUS ) & ( STATUS_WAITING_MASK | STATUS_PAUSED_MASK ) ) | STATUS_KILLED ;
                    break;

                case 'W':
                    LOCQWORD( libmod_debug, i, STATUS ) = ( LOCQWORD( libmod_debug, i, STATUS ) & ( STATUS_WAITING_MASK | STATUS_PAUSED_MASK ) ) | STATUS_RUNNING ;
                    break ;

                case 'S':
                    LOCQWORD( libmod_debug, i, STATUS ) = ( LOCQWORD( libmod_debug, i, STATUS ) & ( STATUS_WAITING_MASK | STATUS_PAUSED_MASK ) ) | STATUS_SLEEPING ;
                    break ;

                case 'F':
                    LOCQWORD( libmod_debug, i, STATUS ) = ( LOCQWORD( libmod_debug, i, STATUS ) & ( STATUS_WAITING_MASK | STATUS_PAUSED_MASK ) ) | STATUS_FROZEN ;
                    break;
            }
            strcpy( action, oaction );
            ptr = optr;
        }

        if ( oaction ) free( oaction );
        if ( found ) console_printf( COLOR_SILVER "OK" );
        return ;
    }

    if (
        strcmp( action, "KILL" ) == 0 ||
        strcmp( action, "SLEEP" ) == 0 ||
        strcmp( action, "WAKEUP" ) == 0 ||
        strcmp( action, "FREEZE" ) == 0 ) {
        char act = *action;
        i = findproc( NULL, action, ptr );
        if ( !i ) return;

        switch ( act ) {
            case 'K':
                LOCQWORD( libmod_debug, i, STATUS ) = ( LOCQWORD( libmod_debug, i, STATUS ) & ( STATUS_WAITING_MASK | STATUS_PAUSED_MASK ) ) | STATUS_KILLED ;
                break;

            case 'W':
                LOCQWORD( libmod_debug, i, STATUS ) = ( LOCQWORD( libmod_debug, i, STATUS ) & ( STATUS_WAITING_MASK | STATUS_PAUSED_MASK ) ) | STATUS_RUNNING ;
                break ;

            case 'S':
                LOCQWORD( libmod_debug, i, STATUS ) = ( LOCQWORD( libmod_debug, i, STATUS ) & ( STATUS_WAITING_MASK | STATUS_PAUSED_MASK ) ) | STATUS_SLEEPING ;
                break ;

            case 'F':
                LOCQWORD( libmod_debug, i, STATUS ) = ( LOCQWORD( libmod_debug, i, STATUS ) & ( STATUS_WAITING_MASK | STATUS_PAUSED_MASK ) ) | STATUS_FROZEN ;
                break;
        }
        console_printf( COLOR_SILVER "OK" );
        return ;
    }

    if ( strcmp( action, "QUIT" ) == 0 ) {
        debugger_show_console = 0;
        exit_value = 0;
        must_exit = 1 ;
        return ;
    }

    /* Variables del ENGINE */

    for ( var = 0 ; var < N_CONSOLE_VARS ; var++ ) {
        if ( strcmp( console_vars[var].name, action ) == 0 ) {
            switch ( console_vars[var].type ) {
                case CON_QWORD:
                    if ( *ptr ) {
                        while ( *ptr == '=' || *ptr == ' ' ) ptr++;
                        eval_expression( ptr, 0 );
                        if ( result.type != RT_ERROR )
                            *( int64_t * )console_vars[var].value = ( int64_t ) result.int_value ;
                    }
                    console_printf( COLOR_SILVER "%s = %"PRId64, console_vars[var].name, *( int64_t * )console_vars[var].value ) ;
                    return ;

                case CON_QWORD_HEX:
                    if ( *ptr ) {
                        while ( *ptr == '=' || *ptr == ' ' ) ptr++;
                        eval_expression( ptr, 0 );
                        if ( result.type != RT_ERROR )
                            *( int64_t * )console_vars[var].value = ( int64_t ) result.int_value ;
                    }
                    console_printf( COLOR_SILVER "%s = %016Xh\n", console_vars[var].name, *( int64_t * )console_vars[var].value ) ;
                    return ;

                case CON_DWORD:
                    if ( *ptr ) {
                        while ( *ptr == '=' || *ptr == ' ' ) ptr++;
                        eval_expression( ptr, 0 );
                        if ( result.type != RT_ERROR )
                            *( int32_t * )console_vars[var].value = ( int32_t ) ( result.int_value < 0 ? 0 : result.int_value > 4294967295LL ? 4294967295LL : result.int_value ); ;
                    }
                    console_printf( COLOR_SILVER "%s = %"PRId32, console_vars[var].name, *( int32_t * )console_vars[var].value ) ;
                    return ;

                case CON_DWORD_HEX:
                    if ( *ptr ) {
                        while ( *ptr == '=' || *ptr == ' ' ) ptr++;
                        eval_expression( ptr, 0 );
                        if ( result.type != RT_ERROR )
                            *( int32_t * )console_vars[var].value = ( int32_t ) ( result.int_value < 0 ? 0 : result.int_value > 1674294967295LL ? 4294967295LL : result.int_value ); ;
                    }
                    console_printf( COLOR_SILVER "%s = %08Xh\n", console_vars[var].name, *( int32_t * )console_vars[var].value ) ;
                    return ;

                case CON_WORD:
                    if ( *ptr ) {
                        while ( *ptr == '=' || *ptr == ' ' ) ptr++;
                        eval_expression( ptr, 0 );
                        if ( result.type != RT_ERROR )
                            *( int16_t * )console_vars[var].value = ( int16_t ) ( result.int_value < 0 ? 0 : result.int_value > 65535 ? 65535 : result.int_value ); ;
                    }
                    console_printf( COLOR_SILVER "%s = %d", console_vars[var].name, *( int16_t * )console_vars[var].value ) ;
                    return ;

                case CON_WORD_HEX:
                    if ( *ptr ) {
                        while ( *ptr == '=' || *ptr == ' ' ) ptr++;
                        eval_expression( ptr, 0 );
                        if ( result.type != RT_ERROR )
                            *( int16_t * )console_vars[var].value = ( int16_t ) ( result.int_value < 0 ? 0 : result.int_value > 65535 ? 65535 : result.int_value ); ;
                    }
                    console_printf( COLOR_SILVER "%s = %04Xh\n", console_vars[var].name, *( int16_t * )console_vars[var].value ) ;
                    return ;

                case CON_BYTE:
                    if ( *ptr ) {
                        while ( *ptr == '=' || *ptr == ' ' ) ptr++;
                        eval_expression( ptr, 0 );
                        if ( result.type != RT_ERROR )
                            *( int8_t * )console_vars[var].value = ( int8_t ) ( result.int_value < 0 ? 0 : result.int_value > 255 ? 255 : result.int_value );
                    }
                    console_printf( COLOR_SILVER "%s = %d", console_vars[var].name, *( int16_t * )console_vars[var].value ) ;
                    return ;

                case CON_BYTE_HEX:
                    if ( *ptr ) {
                        while ( *ptr == '=' || *ptr == ' ' ) ptr++;
                        eval_expression( ptr, 0 );
                        if ( result.type != RT_ERROR )
                            *( int8_t * )console_vars[var].value = ( int8_t ) ( result.int_value < 0 ? 0 : result.int_value > 255 ? 255 : result.int_value );
                    }
                    console_printf( COLOR_SILVER "%s = %02Xh\n", console_vars[var].name, *( int8_t * )console_vars[var].value ) ;
                    return ;

            }
        }
    }

    /* Expresiones */

    eval_expression( cmd, 1 ) ;
}

/* --------------------------------------------------------------------------- */
/* Hotkey for exit (ALT+X)                                                     */
/* --------------------------------------------------------------------------- */

static int force_exit_cb( SDL_Keysym k ) {
    debugger_show_console = 0;
    exit_value = 0;
    must_exit = 1 ;
    return 1;
}

/* --------------------------------------------------------------------------- */
/* Hotkeys for activate/deactivate console                                     */
/* --------------------------------------------------------------------------- */

static int console_keyboard_handler_cb( SDL_Keysym k ) {
    if ( dcb.data.NSourceFiles ) {
        if (( k.mod & KMOD_LALT ) && k.sym == SDLK_c ) {
            if ( !debugger_show_console ) {
                debugger_show_console = 1;
            } else {
                debugger_show_console = 0;
            }
            return 1;
        }

        if ( debugger_show_console ) {
            if ( k.sym == SDLK_F1 ) {
                console_do( "HELP" );
                return 1;
            }

            if ( k.mod & KMOD_LALT ) {
                if ( k.sym == SDLK_LEFT ) {
                    if ( console_columns > HOTKEYHELP_SIZE ) console_columns-- ;
                    return 1;
                }

                if ( k.sym == SDLK_RIGHT ) {
                    if ( console_columns < scr_width / CHARWIDTH ) console_columns++ ;
                    return 1;
                }

                if ( k.sym == SDLK_UP ) {
                    if ( console_lines > 10 ) console_lines-- ;
                    return 1;
                }

                if ( k.sym == SDLK_DOWN ) {
                    if ( console_lines < scr_height / CHARHEIGHT ) console_lines++ ;
                    return 1;
                }
            }

            if ( k.mod & ( KMOD_LSHIFT | KMOD_RSHIFT ) ) {
                if ( k.sym == SDLK_LEFT ) {
                    if ( console_list_x_pos[console_list_current] > 0 ) console_list_x_pos[console_list_current]-- ;
                    return 1;
                }

                if ( k.sym == SDLK_RIGHT ) {
                    console_list_x_pos[console_list_current]++ ;
                    return 1;
                }

                if ( k.sym == SDLK_UP ) {
                    if ( console_list_y_pos[console_list_current] > 0 ) console_list_y_pos[console_list_current]-- ;
                    return 1;
                }

                if ( k.sym == SDLK_DOWN ) {
                    if ( console_list_y_pos[console_list_current] < console_list_y_max[console_list_current] - 1 ) console_list_y_pos[console_list_current]++ ;
                    return 1;
                }

                if ( k.sym == SDLK_F2 ) {
                    console_instance_dump_all_brief();
                    return 1;
                }

                if ( console_list_current_instance && console_list_current == 1 ) {
                    char cmd[4096];

                    if ( k.sym == SDLK_F3 ) {
                        int64_t id = LOCINT64( libmod_debug, console_list_current_instance, PROCESS_ID ) ;
                        console_printf( COLOR_GREEN "%s (%d) LOCALS" COLOR_SILVER "\n\n",
                                ( dcb.data.NSourceFiles && dcb.proc[console_list_current_instance->proc->type].data.ID ) ? getid_name( dcb.proc[console_list_current_instance->proc->type].data.ID ) : (( console_list_current_instance->proc->type == 0 ) ? "Main" : "proc" ),id );
                        sprintf( cmd, "LOCALS %"PRId64, id ) ;
                        console_do( cmd );
                        return 1;
                    }

                    if ( k.sym == SDLK_F4 ) {
                        int64_t id = LOCINT64( libmod_debug, console_list_current_instance, PROCESS_ID ) ;
                        console_printf( COLOR_GREEN "%s (%d) PRIVATES" COLOR_SILVER "\n\n",
                                ( dcb.data.NSourceFiles && dcb.proc[console_list_current_instance->proc->type].data.ID ) ? getid_name( dcb.proc[console_list_current_instance->proc->type].data.ID ) : (( console_list_current_instance->proc->type == 0 ) ? "Main" : "proc" ),id );
                        sprintf( cmd, "PRIVATES %"PRId64, id ) ;
                        console_do( cmd );
                        return 1;
                    }

                    if ( k.sym == SDLK_F5 ) {
                        int64_t id = LOCINT64( libmod_debug, console_list_current_instance, PROCESS_ID ) ;
                        console_printf( COLOR_GREEN "%s (%d) PUBLICS" COLOR_SILVER "\n\n",
                                ( dcb.data.NSourceFiles && dcb.proc[console_list_current_instance->proc->type].data.ID ) ? getid_name( dcb.proc[console_list_current_instance->proc->type].data.ID ) : (( console_list_current_instance->proc->type == 0 ) ? "Main" : "proc" ),id );
                        sprintf( cmd, "PUBLICS %"PRId64, id ) ;
                        console_do( cmd );
                        return 1;
                    }
                }

                if ( k.sym == SDLK_F6 ) {
                    console_list_current ^= 1;
                    return 1;
                }

                if ( k.sym == SDLK_F9 ) {
                    switch ( console_list_current ) {
                        case    0:
                            procs[console_list_y_pos[console_list_current]].breakpoint = !procs[console_list_y_pos[console_list_current]].breakpoint;
                            break;

                        case    1:
                            if ( console_list_current_instance ) console_list_current_instance->breakpoint = !console_list_current_instance->breakpoint;
                            break;
                    }
                    return 1;
                }
            }

            if ( k.sym == SDLK_F2 ) {
                console_instance_dump_all() ;
                return 1;
            }

            if ( !( k.mod & ( KMOD_LSHIFT | KMOD_RSHIFT ) ) ) {
                if ( k.sym == SDLK_F5 ) {
                    console_do( "GO" );
                    return 1;
                }

                if ( k.sym == SDLK_F7 ) {
                    console_do( "TRACE" );
                    return 1;
                }

                if ( k.sym == SDLK_F8 ) {
                    console_do( "STEP" );
                    return 1;
                }

                if ( k.sym == SDLK_F10 ) {
                    console_do( "NEXTFRAME" );
                    return 1;
                }

                if ( k.sym == SDLK_F11 ) {
                    console_do( "NEXTPROC" );
                    return 1;
                }
            }

            if ( k.sym == SDLK_PAGEUP ) {
                console_scroll( console_lines ) ;
                return 1;
            }

            if ( k.sym == SDLK_PAGEDOWN ) {
                console_scroll( -console_lines ) ;
                return 1;
            }

            if ( k.mod & ( KMOD_RCTRL | KMOD_LCTRL ) ) {
                if ( k.sym == SDLK_LEFT ) {
                    console_lateral_scroll( 1 ) ;
                    return 1;
                }

                if ( k.sym == SDLK_RIGHT ) {
                    console_lateral_scroll( -1 ) ;
                    return 1;
                }

                if ( k.sym == SDLK_UP ) {
                    console_scroll( 1 ) ;
                    return 1;
                }

                if ( k.sym == SDLK_DOWN ) {
                    console_scroll( -1 ) ;
                    return 1;
                }
            }

            if ( !( k.mod & KMOD_LALT ) ) console_getkey( k.sym ) ;
            return 1;
        }
    }

    return 0;
}

/* --------------------------------------------------------------------------- */

static unsigned char ansi_seq[128];

static int cmd_line_start_pos = 0;

static void console_draw( void * what, REGION * clip ) {
    int x, y, line, count ;

    if ( break_on_next_proc ) return ;

    if ( debug_sysfont == -1 ) create_debug_sysfont();

    int64_t __drawing_blend_mode = drawing_blend_mode;
    uint8_t __drawing_color_r = drawing_color_r;
    uint8_t __drawing_color_g = drawing_color_g;
    uint8_t __drawing_color_b = drawing_color_b;
    uint8_t __drawing_color_a = drawing_color_a;

    if ( console_columns > scr_width / CHARWIDTH )                      console_columns = scr_width / CHARWIDTH ;
    if ( console_lines > ( scr_height - CHARHEIGHT * 3 ) / CHARHEIGHT ) console_lines = ( scr_height - CHARHEIGHT * 3 ) / CHARHEIGHT ;

    if ( debugger_show_console ) {
        if ( console_y < console_lines * CHARHEIGHT ) console_y += CHARHEIGHT ;
        if ( console_y > console_lines * CHARHEIGHT ) console_y = console_lines * CHARHEIGHT ;
    } else {
        if ( console_y > 0 ) console_y -= CHARHEIGHT ;
        if ( console_y < 0 ) console_y = 0 ;
    }

    if ( console_y > 0 ) shader_deactivate();

    x = ( scr_width - console_columns * CHARWIDTH ) / 2 ;
    y = -console_lines * CHARHEIGHT + console_y ;

    uint8_t __console_alpha = console_alpha;

    console_alpha = 255;

    int current_show = 0;

    for ( count = 0; count < MAX_EXPRESSIONS; count++ ) {
        if ( show_expression[count] ) {
            char * res = eval_expression( show_expression[count], 0 );

            if ( !res || result.type == RT_ERROR ) {
                free( show_expression[count] );
                show_expression[count] = NULL;
                show_expression_count--;
            } else {
                int ln = strlen( res );
                int rl = ln * CHARWIDTH;
                int xo = ( scr_width - rl ) / 2;
                int yo = ( ( console_y < 1 ) ? 2 : console_y + CHARHEIGHT * 2 + 1 ) + CHARHEIGHT * current_show;

                systext_color( 0 ) ;

                systext_puts( xo - 1, yo - 1, res );
                systext_puts( xo - 1, yo    , res );
                systext_puts( xo - 1, yo + 1, res );
                systext_puts( xo    , yo + 1, res );
                systext_puts( xo + 1, yo + 1, res );
                systext_puts( xo + 1, yo    , res );
                systext_puts( xo + 1, yo - 1, res );
                systext_puts( xo    , yo - 1, res );

                systext_color( console_showcolor ) ;
                systext_puts( xo, yo, res );

                current_show++;
            }
        }
    }

    console_alpha = __console_alpha;

    if ( console_y <= 0 ) return ;

    drawing_blend_mode = 0; drawing_color_r = 0; drawing_color_g = 0; drawing_color_b = 0x20; drawing_color_a = console_alpha;
    draw_rectangle_filled( NULL, NULL, x, y, console_columns * CHARWIDTH, console_lines * CHARHEIGHT );

    int64_t shiftstatus = GLOINT64( libmod_debug, SHIFTSTATUS ) ;

    line = console_tail ;

    for ( count = 0 ; count < console_lines + console_scroll_pos ; count++ ) {
        if ( line == console_head ) break ;
        line-- ;
        if ( line < 0 ) line = CONSOLE_HISTORY - 1 ;
    }
    console_scroll_pos = count - console_lines ;
    if ( console_scroll_pos < 0 ) console_scroll_pos = 0 ;

    for ( count = 0; count < console_lines; count++ ) {
        int l;
        systext_color( 0xC0C0C0 ) ;
        if ( console[line] && console_scroll_lateral_pos < ( l = strlen( console[line] ) ) ) {
            int pos = 0, pos0, diff = 0, chars = 0;
             while ( pos < l && chars < console_scroll_lateral_pos ) {
                if ( console[line][pos] == '\e' && console[line][pos + 1] == '[' ) {
                    int i = 0;
                    while( i < 127 && console[line][pos] ) {
                        ansi_seq[ i++ ] = console[line][pos];
                        if ( console[line][pos++] == 'm' ) break;
                    }
                    ansi_seq[ i ] = '\0';
                    diff+=i;
                    systext_puts( x, y, ( char * ) ansi_seq ) ;
                } else {
                    chars++;
                    pos++;
                }
            }

            pos0 = pos;
            chars = 0;
            while ( console[line][pos] && chars < console_columns ) {
                if ( console[line][pos] == '\e' && console[line][pos + 1] == '[' ) {
                    pos += 2;
                    while( console[line][pos] && console[line][pos++] != 'm' );
                } else {
                    chars++;
                    pos++;
                }
            }

            if ( chars > 0 ) {
                unsigned char c = console[line][pos]; console[line][pos] = '\0';
                systext_puts( x, y, console[line] + pos0 ) ;
                console[line][pos] = c;
            }
        }

        y += CHARHEIGHT ;
        line++ ;
        if ( line == CONSOLE_HISTORY ) line = 0 ;
    }

    if ( debugger_show_console && trace_sentence != -1 ) {
        if ( dcb.data.Version < 0x0710 ) {
            if ( trace_instance && instance_exists( trace_instance ) && dcb.sourcecount[trace_sentence >> 24] ) {
                console_printf( COLOR_SILVER "[%s(%"PRIu64"):%"PRId64"]\n" COLOR_YELLOW "%s" COLOR_SILVER "\n\n",
                        trace_instance->proc->name,
                        LOCINT64( libmod_debug, trace_instance, PROCESS_ID ),
                        trace_sentence & 0xFFFFFF,
                        dcb.sourcelines [trace_sentence >> 24] [( trace_sentence & 0xFFFFFF )-1] ) ;
            }
        } else {
            if ( trace_instance && instance_exists( trace_instance ) && dcb.sourcecount[trace_sentence >> 20] ) {
                console_printf( COLOR_SILVER "[%s(%"PRIu64"):%"PRId64"]\n" COLOR_YELLOW "%s" COLOR_SILVER "\n\n",
                        trace_instance->proc->name,
                        LOCINT64( libmod_debug, trace_instance, PROCESS_ID ),
                        trace_sentence & 0xFFFFF,
                        dcb.sourcelines [trace_sentence >> 20] [( trace_sentence & 0xFFFFF )-1] ) ;
            }
        }
//        debug_on_frame = 0;
//        debugger_show_console = 0;
        trace_sentence = -1;
    }

    systext_color( 0xFFFFFF ) ;
    drawing_blend_mode = 0; drawing_color_r = 0x40; drawing_color_g = 0x40; drawing_color_b = 0x40; drawing_color_a = console_alpha;
    draw_rectangle_filled( NULL, NULL, x, y, console_columns * CHARWIDTH, CHARHEIGHT );

    systext_puts( x, y, ">" ) ;

    if ( cursor_pos - cmd_line_start_pos + 3 > console_columns ) cmd_line_start_pos = cursor_pos - console_columns + 3;
    if ( cursor_pos < cmd_line_start_pos ) cmd_line_start_pos = cursor_pos;

    int remain = console_columns - 3;

    for ( int i = cmd_line_start_pos; i < cursor_pos && remain >= 0; i++, remain-- ) systext_putchar( x + CHARWIDTH * ( i + 2 - cmd_line_start_pos), y, console_input[ i ] );
    systext_putchar( x + CHARWIDTH * ( cursor_pos + 2 - cmd_line_start_pos ), y, '_' ) ; remain--;
    for ( int i = cursor_pos; i < strlen( console_input ) && remain >= 0; i++, remain-- ) systext_putchar( x + CHARWIDTH * ( i + 3 - cmd_line_start_pos ), y, console_input[ i ] );

    if ( shiftstatus & 3 ) show_list_window();

    systext_color( 0x404040 ) ;
    drawing_blend_mode = 0; drawing_color_r = 0x00; drawing_color_g = 0xC0; drawing_color_b = 0xC0; drawing_color_a = console_alpha;
    draw_rectangle_filled( NULL, NULL, x, y + CHARHEIGHT, console_columns * CHARWIDTH, CHARHEIGHT );

    if ( !( shiftstatus & 3 ) )
        systext_puts( x, y + CHARHEIGHT, HOTKEYHELP1 ) ;
    else if ( console_list_current )
        systext_puts( x, y + CHARHEIGHT, HOTKEYHELP3 ) ;
    else
        systext_puts( x, y + CHARHEIGHT, HOTKEYHELP2 ) ;

    drawing_blend_mode = __drawing_blend_mode; drawing_color_r = __drawing_color_r; drawing_color_g = __drawing_color_g; drawing_color_b = __drawing_color_b; drawing_color_a = __drawing_color_a;
}

/* --------------------------------------------------------------------------- */

static int console_info( void * what, REGION * clip, int64_t * z, int64_t * drawme ) {

    * drawme = debugger_show_console || show_expression_count || ( console_y > 0 );

    if ( debugger_show_console || ( console_y > 0 ) ) {
    } else if ( show_expression_count ) {
        int count, rl = 0, l;

        for ( count = 0; count < MAX_EXPRESSIONS; count++ ) {
            if ( show_expression[count] ) {
                char * res = eval_expression( show_expression[count], 0 );

                if ( !res ) {
                    free( show_expression[count] );
                    show_expression[count] = NULL;
                    show_expression_count--;
                } else {
                    if ( ( l = strlen( res ) * CHARWIDTH ) > rl ) rl = l ;
                }
            }
        }

        if ( !show_expression_count ) {
            *drawme = 0;
            return 0;
        }
    }

    return * drawme ;
}

/* --------------------------------------------------------------------------- */
/* Exportables                                                                 */
/* --------------------------------------------------------------------------- */

int64_t libmod_debug_trace( INSTANCE * my, int64_t * params ) {
    debug = params[0];
    return 0 ;
}

/* --------------------------------------------------------------------------- */

void m_debug_process_exec_hook( INSTANCE * r ) {
    if ( break_on_next_proc ) {
        break_on_next_proc = 0;
        debugger_show_console = 1;
    }
}

/* --------------------------------------------------------------------------- */

void m_debug_frame_complete_hook() {
    if ( frame_completed && debug_on_frame ) {
        debug_on_frame = 0;
        debugger_show_console = 1;
    }
}

/* --------------------------------------------------------------------------- */
/* Funciones de inicializacion del modulo/plugin                               */

void m_debug_init() {
    if ( dcb.data.NSourceFiles ) {
        hotkey_add( KMOD_LALT, SDLK_x, force_exit_cb                );
        hotkey_add( 0,              0, console_keyboard_handler_cb  );

        gr_new_object(  INT64_MIN, ( OBJ_INFO * ) console_info, ( OBJ_DRAW * ) console_draw, ( void * ) 0 );
    }
}

/* --------------------------------------------------------------------------- */
