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

#ifndef __MESSAGES_H
#define __MESSAGES_H

#include "messages_common.h"

#define MSG_LIBRARY_NOT_FOUND                   "Library \"%s\" not found"
#define MSG_FILE_NOT_FOUND                      "%s: file not found"
#define MSG_FILE_TOO_BIG                        "%s: file too big"
#define MSG_FILE_EMPTY                          "%s: file is empty"
#define MSG_READ_ERROR                          "%s: file reading error"
#define MSG_DIRECTORY_MISSING                   "You must specify a directory"
#define MSG_TOO_MANY_FILES                      "Too many files specified"
#define MSG_DCBL_DCB_VERSION_ERROR              "%s isn't 7.10 DCB version, you need a 7.10 version or greater for use this feature\n"

#define MSG_USAGE                               "Usage: %s [options] filename\n\n"

#define MSG_OPTION_D                            "   -d              Debugging mode\n"
#define MSG_OPTIONS                             "   -o ouputfile    Ouput DCB file\n" \
                                                "   -i dir          Adds the directory to the PATH\n" \
                                                "   -a              Automaticaly adds all files to the DCB\n" \
                                                "   -f file         Adds a single file to the DCB\n" \
                                                "   -l lang         Specify locale settings\n" \
                                                "   -s stub         Generate a stubbed executable from the given stub\n" \
                                                "   -g              Stores debugging information at the DCB\n" \
                                                "   -D macro=text   Set a macro\n" \
                                                "   -p|--pedantic   Don't use automatic declare\n" \
                                                "   --libmode       Build a library\n" \
                                                "   -L library      Include a library\n" \
                                                "   -C options      Specify compiler options\n" \
                                                "                   Where options are:\n" \
                                                "                       a    Enable automatic declare functions\n\n"

#define MSG_COMPILE_ERROR                       "%s%s:%" PRId64 ": error: %s"
#define MSG_COMPILE_WARNING                     "%s%s:%" PRId64 ": warning: %s"
#define MSG_CONSTANT_NAME_EXP                   "Constant name expected"
#define MSG_INVALID_IDENTIFIER                  "Invalid identifier"
#define MSG_EXPECTED                            "\"%s\" expected"
#define MSG_INVALID_PARAM                       "Parameter name invalid"
#define MSG_INVALID_PARAMT                      "Parameter type invalid"
#define MSG_TOO_MANY_PARAMS                     "Too many parameters in a definition"
#define MSG_INCORRECT_PARAMC                    "Incorrect number of parameters. Function: %s MinParams: %" PRId64 "."
#define MSG_NO_BEGIN                            "BEGIN expected"
#define MSG_NO_END                              "END expected"
#define MSG_ELSE_WOUT_IF                        "ELSE without IF"
#define MSG_PROGRAM_NAME_EXP                    "Program name expected"
#define MSG_PROCESS_NAME_EXP                    "Procedure name expected"
#define MSG_INVALID_TYPE                        "Invalid data type"
#define MSG_UNEXPECTED_TOKEN                    "Unexpected token (too many ENDs?)"
#define MSG_UNEXPECTED_TOKEN_GENERIC            "Unexpected token"
#define MSG_NO_MAIN                             "Main procedure was not defined"
#define MSG_DEFAULT_REQUIRED                    "Default value required" 
#define MSG_INTEGER_REQUIRED                    "Integer type required"
#define MSG_NUMBER_REQUIRED                     "Numeric type required"
#define MSG_INCOMP_TYPES                        "Data types not compatible with operation"
#define MSG_INCOMP_TYPE                         "Data type not accepted here"
#define MSG_UNKNOWN_IDENTIFIER                  "Unknown identifier"
#define MSG_NOT_AN_ARRAY                        "Not an array or struct array"
#define MSG_BOUND                               "Index out of range"
#define MSG_IDENTIFIER_EXP                      "Identifier expected"
#define MSG_NOT_AN_LVALUE                       "Can't get the address of an inmediate value"
#define MSG_NOT_A_POINTER                       "Pointer required"
#define MSG_VARIABLE_REQUIRED                   "Variable required"
#define MSG_POINTER_VARIABLE_REQUIRED           "Pointer variable required"
#define MSG_STRUCT_REQUIRED                     "Struct required"
#define MSG_DIVIDE_BY_ZERO                      "Division by zero"
#define MSG_TYPES_NOT_THE_SAME                  "Values are of incompatible type"
#define MSG_CONSTANT_EXP                        "Constant value expected"
#define MSG_STRING_EXP                          "String expected"
#define MSG_NO_LOOP                             "Out of loop"
#define MSG_INVALID_STEP                        "Invalid STEP"
#define MSG_INVALID_SENTENCE                    "Invalid sentence"
#define MSG_VTA                                 "Can't create an array of undefined multiple sizes"
#define MSG_TOO_MANY_AL                         "Too many array levels"
#define MSG_CONSTANT_REDECLARED_AS_VARIABLE     "Constant redeclared as variable"
#define MSG_VARIABLE_REDECLARED_AS_CONSTANT     "Variable redeclared as constant"
#define MSG_VARIABLE_REDECLARE                  "Variable redeclared"
#define MSG_VARIABLE_REDECLARE_DIFF             "Variable redeclared with different type"
#define MSG_VARIABLE_REDECLARE_IN_DIFF_CONTEXT  "Variable already declared in another context"
#define MSG_IDENTIFIER_EXP                      "Identifier expected"
#define MSG_CANT_INIT_STR                       "This struct can't be initialized"
#define MSG_TOO_MANY_INIT                       "Too many initializers"
#define MSG_TOO_MANY_INCLUDES                   "Too many nested includes"
#define MSG_IDENTIFIER_TOO_LONG                 "Identifier too long"
#define MSG_INVALID_CHAR                        "Invalid Character"
#define MSG_TOO_MANY_TYPES                      "Too many user-defined types"
#define MSG_UNDEFINED_PROC                      "Undefined procedure"
#define MSG_NO_COMPATIBLE_DLL                   "The library is not BennuGD compatible"
#define MSG_TOO_MANY_SYSPROCS                   "Too many system functions"
#define MSG_INCORRECT_PTR_INIT                  "A pointer can only be initialized to 0"
#define MSG_NOT_ENOUGH_INIT                     "Not enough initializers"
#define MSG_MULTIPLE_PROCS_FOUND                "Various conflicting versions of %s found"
#define MSG_QUESTION_INC                        "Incompatible types at the sides of ? operator"
#define MSG_UNKNOWN_PREP                        "Unknown preprocessor directive"
#define MSG_PTR_CONVERSION                      "Invalid conversion of non-pointer to pointer"
#define MSG_CONVERSION                          "Unsupported data type cast"
#define MSG_PROC_ALREADY_DEFINED                "Process/function already defined"
#define MSG_PROC_ALREADY_DECLARED               "Process/function already declared"
#define MSG_FRAME_REQUIRES_INT                  "FRAME requires an INT return value type"
#define MSG_VARIABLE_ERROR                      "Variable is process/process type identifier"
#define MSG_PROTO_ERROR                         "Conflict with previous declaration"
#define MSG_MACRO_ERROR                         "Macro %s already declared"
#define MSG_OR                                  " or "
#define MSG_TOO_COMPLEX                         "Expression too complex"
#define MSG_EXTRA_CHAR                          "Extra character"
#define MSG_INVALID_EXP                         "Invalid expression"
#define MSG_FILENAME_EXP                        "Filename expected"
#define MSG_ON_PARAM_ERR                        "ERROR/EXIT expected"
#define MSG_GOTO_EXP                            "GOTO/JMP expected"
#define MSG_DATA_TYPE_REQUIRED                  "Data type required"
#define MSG_INVALID_INITIALIZER                 "Initializer element is not constant"
#define MSG_INLINE_ASSIGNATION_ERROR            "Inline declaration variables don't allow value assignation"
#define MSG_CAST_POINTER_INCOMPATIBLE           "Assignment to pointer without a compatible cast"
#define MSG_OUT_OF_MEMORY                       "Out of memory"

#endif
