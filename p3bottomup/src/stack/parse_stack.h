/**
 * @title: Parse Stack Module for P3 Bottom-Up Parser
 */

#ifndef P3_PARSE_STACK_H
#define P3_PARSE_STACK_H

#include "../token/token.h"
#include "../language/language.h"

typedef struct {
    int state;
    int symbol_id;
    Token token;
} StackEntry;

typedef struct {
    StackEntry *items;
    int size;
    int capacity;
} ParseStack;

ParseStack *create_parse_stack(int initial_capacity);
int push_stack(ParseStack *stack, const StackEntry *entry);
int pop_stack(ParseStack *stack, StackEntry *out);
StackEntry *peek_stack(ParseStack *stack);
StackEntry *peek_stack_n(const ParseStack *stack, int n_from_top);
void free_parse_stack(ParseStack **stack);
void format_stack(const ParseStack *stack, const LanguageSpec *spec, char *out, int out_len);

#endif /* P3_PARSE_STACK_H */
