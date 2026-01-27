/**
 * @title: Language-Specific Definitions.
 * @description: Centralized header containing all character constants, 
 *               string constants, and language-specific defines used 
 *               throughout the preprocessor.
 * @authors: Davi Penna-Mattos Dias de Paiva
 * @creation: 2026/01/27.
 */

#ifndef LANGUAGE_DEFS_H
#define LANGUAGE_DEFS_H

/* ============================================================================
 * CHARACTER CONSTANTS - Single characters used for parsing
 * ============================================================================ */

/* Preprocessor directive character */
#define CHAR_HASH               '#'

/* Quote characters */
#define CHAR_DOUBLE_QUOTE       '"'
#define CHAR_SINGLE_QUOTE       '\''

/* Bracket characters */
#define CHAR_ANGLE_BRACKET_OPEN '<'
#define CHAR_ANGLE_BRACKET_CLOSE '>'
#define CHAR_PAREN_OPEN         '('
#define CHAR_PAREN_CLOSE        ')'
#define CHAR_BRACE_OPEN         '{'
#define CHAR_BRACE_CLOSE        '}'
#define CHAR_BRACKET_OPEN       '['
#define CHAR_BRACKET_CLOSE      ']'

/* Separator characters */
#define CHAR_COMMA              ','
#define CHAR_SEMICOLON          ';'
#define CHAR_COLON              ':'
#define CHAR_DOT                '.'

/* Operator characters */
#define CHAR_UNDERSCORE         '_'
#define CHAR_DASH               '-'
#define CHAR_PLUS               '+'
#define CHAR_ASTERISK           '*'
#define CHAR_SLASH              '/'
#define CHAR_BACKSLASH          '\\'
#define CHAR_EQUALS             '='

/* Whitespace characters */
#define CHAR_SPACE              ' '
#define CHAR_TAB                '\t'
#define CHAR_NEWLINE            '\n'
#define CHAR_CARRIAGE_RETURN    '\r'

/* Special characters */
#define CHAR_NULL               '\0'

/* ============================================================================
 * PREPROCESSOR DIRECTIVE KEYWORDS
 * ============================================================================ */

#define KEYWORD_DEFINE          "define"
#define KEYWORD_INCLUDE         "include"
#define KEYWORD_IFDEF           "ifdef"
#define KEYWORD_IFNDEF          "ifndef"
#define KEYWORD_ENDIF           "endif"
#define KEYWORD_ELSE            "else"
#define KEYWORD_ELIF            "elif"
#define KEYWORD_UNDEF           "undef"
#define KEYWORD_PRAGMA          "pragma"
#define KEYWORD_ERROR           "error"
#define KEYWORD_WARNING         "warning"

/* Keyword lengths (for efficient parsing) */
#define KEYWORD_DEFINE_LENGTH   6
#define KEYWORD_INCLUDE_LENGTH  7
#define KEYWORD_IFDEF_LENGTH    5
#define KEYWORD_IFNDEF_LENGTH   6
#define KEYWORD_ENDIF_LENGTH    5
#define KEYWORD_ELSE_LENGTH     4
#define KEYWORD_ELIF_LENGTH     4
#define KEYWORD_UNDEF_LENGTH    5

/* ============================================================================
 * COMMENT DELIMITERS
 * ============================================================================ */

#define COMMENT_BLOCK_START     "/*"
#define COMMENT_BLOCK_END       "*/"
#define COMMENT_LINE            "//"

/* ============================================================================
 * FILE HANDLING CONSTANTS
 * ============================================================================ */

/* File path separators */
#define PATH_SEPARATOR          '/'
#define PATH_SEPARATOR_STR      "/"
#define EXTENSION_SEPARATOR     '.'

/* File modes */
#define FILE_MODE_READ          "r"
#define FILE_MODE_WRITE         "w"
#define FILE_MODE_APPEND        "a"

/* Preprocessor output extension */
#define PREPROCESSOR_EXTENSION  "_pp"

/* ============================================================================
 * COMMAND LINE FLAGS
 * ============================================================================ */

#define FLAG_HELP               "-help"
#define FLAG_HELP_SHORT         "-h"
#define FLAG_DEBUG              "-d"
#define FLAG_DIRECTIVES         "-d"
#define FLAG_ALL                "-all"
#define FLAG_COMMENTS           "-c"
#define FLAG_PREFIX             '-'

/* ============================================================================
 * BUFFER SIZE CONSTANTS
 * ============================================================================ */

/* General buffer sizes */
#define BUFFER_SIZE_TINY        64
#define BUFFER_SIZE_SMALL       128
#define BUFFER_SIZE_MEDIUM      256
#define BUFFER_SIZE_LARGE       512
#define BUFFER_SIZE_XLARGE      1024
#define BUFFER_SIZE_LINE        BUFFER_SIZE_XLARGE

/* Symbol table sizes */
#define SYMBOL_TABLE_SIZE       256
#define SYMBOL_NAME_SIZE        64
#define SYMBOL_VALUE_SIZE       256

/* Include handling sizes */
#define MAX_INCLUDED_FILES      512
#define MAX_PATH_LENGTH         128
#define MAX_FILENAME_IN_DIRECTIVE 256

/* Macro handling sizes */
#define MAX_MACRO_ARGS          10
#define MAX_IDENTIFIER_LENGTH   64
#define MAX_KEYWORD_LENGTH      64
#define MAX_ARG_LENGTH          256

/* ============================================================================
 * BOOLEAN CONSTANTS (for pre-C99 compatibility)
 * ============================================================================ */

#ifndef __bool_true_false_are_defined
#define TRUE                    1
#define FALSE                   0
#endif

/* ============================================================================
 * WHITESPACE CHECK MACRO
 * ============================================================================ */

#define IS_WHITESPACE(c) ((c) == CHAR_SPACE || (c) == CHAR_TAB || \
                          (c) == CHAR_NEWLINE || (c) == CHAR_CARRIAGE_RETURN)

/* ============================================================================
 * IDENTIFIER CHARACTER CHECK MACROS
 * ============================================================================ */

#define IS_IDENTIFIER_START(c)  (isalpha(c) || (c) == CHAR_UNDERSCORE)
#define IS_IDENTIFIER_CHAR(c)   (isalnum(c) || (c) == CHAR_UNDERSCORE)

#endif /* LANGUAGE_DEFS_H */
