/**
 * Language-Specific Definitions for Lexer
 * 
 * @description: 
 *      Centralized header containing all character constants, 
 *      string constants, and language-specific defines used 
 *      throughout the preprocessor.
 * 
 * @author: Marc Bosch Manzano  
 * @creation: 2026/02/08
 * @see_also: Adapted from P1 Preprocessor language_defs.h.
 */

#ifndef LEXER_LANGUAGE_DEFS_H
#define LEXER_LANGUAGE_DEFS_H

#include <stdio.h>
#include <stdlib.h>

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

/* ==========================================================================
 * FILE HANDLING CONSTANTS
 * ========================================================================== */

/* File path separators */
#define PATH_SEPARATOR          '/'
#define PATH_SEPARATOR_STR      "/"
#define EXTENSION_SEPARATOR     '.'

/* File modes */
#define FILE_MODE_READ          "r"
#define FILE_MODE_WRITE         "w"
#define FILE_MODE_APPEND        "a"

/* =========================================================================
 * BUFFER SIZE CONSTANTS
 * ========================================================================= */

/* General buffer sizes */
#define BUFFER_SIZE_TINY        64     // 64 bytes
#define BUFFER_SIZE_SMALL       128    // 128 bytes
#define BUFFER_SIZE_MEDIUM      256    // 256 bytes
#define BUFFER_SIZE_LARGE       512    // 512 bytes
#define BUFFER_SIZE_XLARGE      1024   // 1 kibibyte
#define BUFFER_SIZE_XXLARGE     4096   // 4 kibibyte
#define BUFFER_SIZE_EXTREME     16384  // 16 kibibytes

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

/* =========================================================================
 * BOOLEAN CONSTANTS (for pre-C99 compatibility)
 * ========================================================================= */

#ifndef __bool_true_false_are_defined
#define TRUE                    1
#define FALSE                   0
#endif

/* =========================================================================
 * GENERAL MACROS
 * ========================================================================= */

#define IS_WHITESPACE(c) (\
    (c) == CHAR_SPACE || (c) == CHAR_TAB ||\
    (c) == CHAR_NEWLINE || (c) == CHAR_CARRIAGE_RETURN\
)
         
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))


/* ============================================================================
 * LEXER CONSTANTS - Single characters used for parsing
 * ============================================================================ */

#define AUTOMATA_FILE                   "automata.txt"
#define MANUAL_PAGE                     "man_page.txt"

#define AUTOMATA_SEPARATOR              '#'
#define AUTOMATA_LIST_SEPARATOR         ' '
#define AUTOMATA_ATTRIBUTE_SEPARATOR    '\n'
#define EMPTY_STATE_NUM                 0
#define INITIAL_STATE_NUM               1
#define NUM_CATEGORIES                  7
#define SCANNER_OUTPUT_SUFFIX           "scn"

typedef enum {
    CAT_NUMBER,
    CAT_IDENTIFIER,
    CAT_KEYWORD,
    CAT_LITERAL,
    CAT_OPERATOR,
    CAT_SPECIALCHAR,
    CAT_NONRECOGNIZED
} TokenCategory;

static const char *const category_text[] = {
    [CAT_NUMBER] = "CAT_NUMBER",
    [CAT_IDENTIFIER] = "CAT_IDENTIFIER",
    [CAT_KEYWORD] = "CAT_KEYWORD",
    [CAT_LITERAL] = "CAT_LITERAL",
    [CAT_OPERATOR] = "CAT_OPERATOR",
    [CAT_SPECIALCHAR] = "CAT_SPECIALCHAR",
    [CAT_NONRECOGNIZED] = "CAT_NONRECOGNIZED"
};

typedef enum {
    CATEGORY,
    ALPHABET,
    ACCEPTING_STATES,
    TRANSITIONS
} AutomataAttributes;

static inline const char *get_resources_dir(void) {
#if (LOCAL == 1)
    return "./resources/";
#else
    static char buffer[BUFFER_SIZE_SMALL];
    sprintf(
        buffer, 
        "%s/.local/share/lexer/",
        getenv("HOME")
    );
    return buffer;
#endif
}

#endif /* LANGUAGE_DEFS_H */
