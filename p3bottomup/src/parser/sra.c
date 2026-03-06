/**
 * @file sra.c
 * @brief Shift-Reduce Automaton (SRA) engine.
 * 
 * Orchestrates the DFA (which owns the parse table) and the Stack
 * according to productions defined in the Language.  All grammar-
 * specific knowledge comes from the data files; this module contains
 * only the generic SRA algorithm. SRA is table-driven and grammar-independent.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sra.h"
#include "../language/grammar.h"
#include "../utils/string_utils.h"

/* ── stack snapshot formatting ──────────────────────────────────────── */

/**
 * Build a human-readable snapshot of the stack for logging.
 * Format: [state,symbol_name][state,symbol_name]...
 * @param stack A read-only stack.
 * @param lang A read-only language.
 * @warning Caller must free() the returned buffer.
 */
static char *format_stack(const Stack *stack, const Language *lang)
{
    int n = stack_size(stack);
    /* estimate: each entry "[%d,%s]" ≤ 12 + 31 + 3 = 46 bytes */
    int cap = n * 48 + 4;
    char *buf = malloc((size_t)cap);
    if (!buf) return NULL;

    int pos = 0;
    for (int i = 0; i < n; i++) {
        StackItem item = stack_get(stack, i);
        const char *name = grammar_symbol_name(lang, item.symbol);
        pos += snprintf(buf + pos, (size_t)(cap - pos),
                        "[%d,%s]", item.state, name);
    }
    if (pos == 0) buf[0] = '\0';
    return buf;
}

/* ── SRA lifecycle ──────────────────────────────────────────────────── */

SRA *sra_init(DFA *dfa, Stack *stack, Language *language)
{
    if (!dfa || !stack || !language) return NULL;

    SRA *sra = malloc(sizeof(SRA));
    if (!sra) return NULL;

    sra->dfa      = dfa;
    sra->stack    = stack;
    sra->language = language;

    /* push initial state 0 with a bottom-of-stack marker symbol (-1) */
    stack_push(stack, 0, -1);
    dfa_set_state(dfa, 0);
    return sra;
}

void sra_destroy(SRA *sra)
{
    free(sra);  /* does NOT free the components it wraps */
}

/* ── primitive operations ───────────────────────────────────────────── */

void sra_shift(SRA *sra, int symbol, int target_state)
{
    stack_push(sra->stack, target_state, symbol);
    dfa_set_state(sra->dfa, target_state);
}

void sra_reduce(SRA *sra, const Production *prod)
{
    /* pop |rhs| items */
    for (int i = 0; i < prod->rhs_len; i++) {
        stack_pop(sra->stack);
    }

    /* the exposed top now determines the GOTO state */
    int exposed_state = stack_peek(sra->stack).state;
    dfa_set_state(sra->dfa, exposed_state);

    int goto_state = dfa_get_goto(sra->dfa, prod->lhs);
    if (goto_state < 0) {
        fprintf(stderr, "sra: GOTO error after reducing production %d "
                "(state %d, lhs %d)\n", prod->id, exposed_state, prod->lhs);
        return;
    }

    stack_push(sra->stack, goto_state, prod->lhs);
    dfa_set_state(sra->dfa, goto_state);
}

/* ── high-level helpers for the parse loop ──────────────────────────── */

static void execute_shift(SRA *sra, Token *token, int target_state,
                          int input_pos, Logger *logger)
{
    int prev = stack_peek(sra->stack).state;
    sra_shift(sra, token->type, target_state);

    if (logger) {
        char *snap = format_stack(sra->stack, sra->language);
        logger_log_shift(logger, input_pos, prev, target_state, snap);
        free(snap);
    }
}

static void execute_reduce(SRA *sra, int prod_id, Logger *logger)
{
    const Production *prod =
        grammar_get_production(sra->language, prod_id);
    if (!prod) {
        fprintf(stderr, "sra: unknown production %d\n", prod_id);
        return;
    }

    sra_reduce(sra, prod);

    if (logger) {
        char *pstr = grammar_format_production(sra->language, prod_id);
        int goto_state = stack_peek(sra->stack).state;
        char *snap = format_stack(sra->stack, sra->language);
        logger_log_reduce(logger, prod_id, pstr,
                          prod->rhs_len, goto_state, snap);
        free(pstr);
        free(snap);
    }
}

static void handle_accept(SRA *sra, Logger *logger)
{
    if (logger) {
        char *snap = format_stack(sra->stack, sra->language);
        logger_log_accept(logger, snap);
        free(snap);
    }
}

static void handle_error(SRA *sra, Token *token, Logger *logger)
{
    int state = stack_peek(sra->stack).state;
    const char *tname = token ? 
        grammar_symbol_name(sra->language, token->type) : 
        "EOF";

    fprintf(stderr, "sra: syntax error in state %d on token '%s'\n",
            state, tname);

    if (logger) {
        char *snap = format_stack(sra->stack, sra->language);
        logger_log_error(logger, state, tname, snap);
        free(snap);
    }
}

/* ── main parse loop ────────────────────────────────────────────────── */

int sra_parse(SRA *sra, TokenList *tokens, Logger *logger)
{
    if (!sra || !tokens) 
        return EXIT_FAILURE;

    Token *current = tokens->head;
    int input_pos = 0;

    while (1) {
        if (!current) {
            handle_error(sra, NULL, logger);
            return EXIT_FAILURE;
        }

        int state = stack_peek(sra->stack).state;
        dfa_set_state(sra->dfa, state);

        Action act = dfa_get_action(sra->dfa, current->type);

        switch (act.type) {
        case ACTION_SHIFT:
            execute_shift(sra, current, act.value, input_pos, logger);
            current = current->next;
            input_pos++;
            break;

        case ACTION_REDUCE:
            execute_reduce(sra, act.value, logger);
            /* do NOT advance token – re-examine same lookahead */
            break;

        case ACTION_ACCEPT:
            handle_accept(sra, logger);
            return EXIT_SUCCESS;

        case ACTION_ERROR:
        default:
            handle_error(sra, current, logger);
            return EXIT_FAILURE;
        }
    }
}

char *sra_string(const SRA *sra, int level) {
    if (!sra) {
        return NULL;
    }
    char *s = get_copy("\n");
    int n = level + 1;
    jsonify_wrap(&s, level, 1, "{");
    jsonify(&s, n, 1, 1, "dfa",      dfa_string(sra->dfa, n));
    jsonify(&s, n, 1, 1, "stack",    stack_string(sra->stack, n));
    jsonify(&s, n, 0, 1, "language", language_string(sra->language, n));
    jsonify_wrap(&s, level, 0, "}");
    return s;
}
