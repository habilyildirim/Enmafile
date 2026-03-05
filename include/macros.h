#ifndef MACROS_H
#define MACROS_H

//VERSION
#define VERSION               "v0.0.9"

/* === ENMA ARGS === */
#define VERSION_ARG_KEY       "-v"
#define PATH_OPTION_ARG_KEY   "-p"
#define SILENCE_ARG_KEY       "-s"
#define SILENCE_URSLT_ARG_KEY "-sur"
#define HELP_ARG_KEY          "-h"

#define VERSION_OUT           "enma version\n"
#define HELP_OUT              "Useage: enma [options]\n" \
							  "Options:\n" \
							  "-p          Change directory and run.\n" \
							  "-s          Don't echo recipes.\n" \
							  "-sur        Don't echo unexpected results.\n"\
							  "-v          Print version and exit.\n" \
							  "-h          Print this message and exit.\n"

/* === GENERIC KEYS === */
#define FILE_READ_KEY     "enmafile"
#define DEFAULT_ARG       "enma"
#define VAR_KEY           "!"
#define ASSIGNMENT_KEY    '='
#define USE_VAR_START     "^"
#define USE_VAR_END       "^"
#define COMMENT_KEY       "#"
#define DIRECTIVE_KEY     "\t"
#define AC_CON_KEY        "\t"

/* === TOOL KEYS === */
#define SYS_VAR_START     "s>"
#define SYS_VAR_END       "<s"
#define DEP_KEY           "deps->"
#define SILENCE_KEY       "@"

/**/
#define UP_TO_DATE_0      "[enma "
#define UP_TO_DATE_1      " is up to date]"

/* === LOGICAL KEYS === */
#define IF_KEY            "if->"
#define ELIF_KEY          "elif->"
#define ELSE_KEY          "else->"
#define DEF_KEY           "def->"
			
/* === ERROR MACROS === */
#define ERROR_FILE_NOT_EXISTS   "ec000"
#define EC000                   "File not exists"

#define ERROR_TARGET_NOT_EXISTS "ec001"
#define EC001                   "Target not exists" 

#define ERROR_NULL_VAR_NAME     "ec00"
#define EC00                    "Var name is null"

#define ERROR_NULL_VALUE        "ec01"
#define EC01                    "Value is null"

#define ERROR_VAR_NOT_EXISTS    "ec02"
#define EC02                    "Var not exists"

#define ERROR_DEPS_NOT_EXISTS   "ec03"
#define EC03                    "One or more missing"

#define ERROR_PATH_O_NOT_EXISTS "ec04"
#define EC04                    "Directory not exists"

#define ERROR_0                 "LINE -> "
#define ERROR_1                 " ERROR; "

#define RES_ERROR_0             "[enma directive result -> "
#define RES_ERROR_1             "]"

#endif
