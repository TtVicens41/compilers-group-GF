#ifdef DEPRECATED
/**
 * Alphabet Definition for the Language Lexer
 * 
 * @author: Marc Bosch Manzano
 * @creation: 2026/01/30
 */

#ifndef ALPHABET_H
#define ALPHABET_H

/** 
 * @brief Recognized keywords of the language lexer.
 * 
 * The elements of this enum are defined to be integer identifiers. The string
 * definition can be found in table `KEYWORDS`.
 */
typedef enum {
    IF,
    ELSE,
    WHILE,
    RETURN
} Keywords;

/** 
 * @brief Supported data types for the language lexer.
 * 
 * The elements of this enum are defined to be integer identifiers. The string
 * definition can be found in table `TYPES`.
 */
typedef enum {
    INT,
    VOID,
    CHAR
} Types;

/** 
 * @brief Supported operators for the language lexer.
 * 
 * The elements of this enum are defined to be ASCII characters. They can be 
 * iterated in table `OPERATORS`.
 */
typedef enum {
    ASSIGNATION = '=',
    GREATER_THAN = '>',
    SUM = '+',
    ASTERISK = '*',
} Operators;

/** 
 * @brief Supported special characters for the language lexer.
 * 
 * The elements of this enum are defined to be ASCII characters. They specify 
 * an ordred between tokens, separation between statements, groups of 
 * statements, etc. They can be iterated in table `SPECIAL_CHARACTERS`.
 */
typedef enum {
    SEMICOLON = ';',
    COMMA = ',',
    OPEN_PARENTHESIS = '(',
    CLOSE_PARENTHESIS = ')',
    OPEN_CLAUDATOR = '{',
    CLOSE_CLAUDATOR = '}',
    OPEN_BRACKET = '[',
    CLOSE_BRACKET = ']'
} SpecialCharacters;

/** 
 * @brief Non-essential characters (separators) for the language lexer.
 * 
 * The elements of this enum are defined to be ASCII characters. They are used
 * to delimit tokens, but they are not relevant by the compiler when the 
 * process is done. They can be iterated in table `SPECIAL_CHARACTERS`.
 */
typedef enum {
    END_LINE = '\n',
    TABULATOR = '\t',
    SPACE = ' '
} NonEssentialCharacters;

/** 
 * @brief A constant table of names for C reserved words. 
 */
static const char *const KEYWORDS[] = {
    [IF] = "if",
    [ELSE] = "else",
    [WHILE] = "while",
    [RETURN] = "return"
};

/**
 * @brief A constant table of codes and names C data types.
 */
static const char *const TYPES[] = {
    [INT] = "int",
    [VOID] = "void",
    [CHAR] = "char"
};

/** 
 * @brief A constant table of C operation characters. 
 */
static const char OPERATORS[] = {
    ASSIGNATION,
    GREATER_THAN,
    SUM,
    ASTERISK
};

/** 
 * @brief A constant table of C special characters. 
 */
static const char SPECIAL_CHARACTERS[] = {
    SEMICOLON,
    OPEN_PARENTHESIS,
    CLOSE_PARENTHESIS,
    OPEN_CLAUDATOR,
    CLOSE_CLAUDATOR,
    OPEN_BRACKET,
    CLOSE_BRACKET
};

/** 
 * @brief A constant table of C non essential characters.
 */
static const char NON_ESSENTIAL_CHARACTERS[] = {
    END_LINE,
    TABULATOR,
    SPACE
};

/** 
 * @brief The delimiter for strings accepted for the C language.
 */
static const char LITERAL = '\"';

#endif // ALPHABET_H
#endif