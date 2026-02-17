/**
 * @title: Language Definitions for P3 Bottom-Up Parser
 */

#ifndef P3_LANGUAGE_DEFS_H
#define P3_LANGUAGE_DEFS_H

#define TRUE 1
#define FALSE 0

#define BUFFER_SIZE_SMALL 128
#define BUFFER_SIZE_MEDIUM 256
#define BUFFER_SIZE_LARGE 512
#define BUFFER_SIZE_LINE 1024

#define TOKEN_LEXEME_MAX 64
#define TOKEN_CATEGORY_MAX 32
#define SYMBOL_NAME_MAX 16
#define RULE_TEXT_MAX 64
#define RULE_RHS_MAX 8
#define TRACE_PATH_MAX 512
#define STACK_TRACE_MAX 2048

#define P3_TRACE_SUFFIX "_p3dbg"
#define P3_TRACE_EXTENSION ".txt"

#endif /* P3_LANGUAGE_DEFS_H */
