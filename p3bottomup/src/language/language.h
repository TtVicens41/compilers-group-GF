/**
 * @title: Language Specification Module for P3 Bottom-Up Parser
 */

#ifndef P3_LANGUAGE_H
#define P3_LANGUAGE_H

#include "../language_defs.h"

typedef enum {
    SYM_TERM,
    SYM_NONTERM
} SymbolKind;

typedef enum {
    ACT_ERROR,
    ACT_SHIFT,
    ACT_REDUCE,
    ACT_ACCEPT
} ActionType;

typedef struct {
    int id;
    char name[SYMBOL_NAME_MAX];
    SymbolKind kind;
} GrammarSymbol;

typedef struct {
    int id;
    int lhs_id;
    int rhs_ids[RULE_RHS_MAX];
    int rhs_len;
    char text[RULE_TEXT_MAX];
} ProductionRule;

typedef struct {
    ActionType type;
    int value;
} ParseAction;

typedef struct {
    int symbol_count;
    GrammarSymbol *symbols;

    int prod_count;
    ProductionRule *prods;

    int state_count;
    int term_count;
    int nonterm_count;

    ParseAction **action;
    int **go_to;

    int start_symbol_id;
    int eof_symbol_id;
} LanguageSpec;

LanguageSpec *load_language_spec(const char *path);
void free_language_spec(LanguageSpec **spec);

int find_symbol_id(const LanguageSpec *spec, const char *name, SymbolKind kind);
ParseAction get_action(const LanguageSpec *spec, int state, int term_symbol_id);
int get_goto_state(const LanguageSpec *spec, int state, int nonterm_symbol_id);

#endif /* P3_LANGUAGE_H */
