/**
 * @title: Bottom-Up Parser Module for P3
 */

#ifndef P3_PARSER_H
#define P3_PARSER_H

#include "../language/language.h"
#include "../token/token.h"
#include "../trace/trace.h"

typedef enum {
    PARSE_ACCEPTED,
    PARSE_REJECTED,
    PARSE_INTERNAL_ERROR
} ParseStatus;

typedef struct {
    ParseStatus status;
    int consumed_tokens;
    int steps;
    int final_state;
    int last_rule_id;
} ParseResult;

ParseResult run_bottomup_parser(const LanguageSpec *spec, TokenStream *tokens, TraceWriter *trace);

#endif /* P3_PARSER_H */
