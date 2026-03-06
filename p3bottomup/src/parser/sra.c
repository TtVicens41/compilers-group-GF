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

/* ── step trace helper ──────────────────────────────────────────────── */

static const char *action_type_label(ActionType t)
{
    switch (t) {
    case ACTION_SHIFT:  return "SHIFT";
    case ACTION_REDUCE: return "REDUCE";
    case ACTION_ACCEPT: return "ACCEPT";
    default:            return "ERROR";
    }
}

static void print_step_header(int step, const Stack *stack,
                               const Language *lang, Token *current)
{
    char *snap = format_stack(stack, lang);
    const char *tok_name = current
        ? grammar_symbol_name(lang, current->type) : "EOF";
    const char *tok_lex  = current ? current->lexeme : "";

    printf("────────────────────────────────────────────────\n");
    printf("  STEP %d\n", step);
    printf("    Stack:      %s\n", snap ? snap : "(empty)");
    printf("    Lookahead:  %s (\"%s\")\n", tok_name, tok_lex);
    free(snap);
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
    int step = 0;

    printf("════════════════════════════════════════════════\n");
    printf("         PARSE EXECUTION TRACE\n");
    printf("════════════════════════════════════════════════\n");

    while (1) {
        step++;

        if (!current) {
            print_step_header(step, sra->stack, sra->language, NULL);
            printf("    → No more tokens — ERROR\n");
            handle_error(sra, NULL, logger);
            return EXIT_FAILURE;
        }

        int state = stack_peek(sra->stack).state;
        dfa_set_state(sra->dfa, state);

        print_step_header(step, sra->stack, sra->language, current);

        Action act = dfa_get_action(sra->dfa, current->type);
        printf("    Lookup:     ACTION[state %d, %s] = %s",
               state,
               grammar_symbol_name(sra->language, current->type),
               action_type_label(act.type));
        if (act.type == ACTION_SHIFT)
            printf(" %d\n", act.value);
        else if (act.type == ACTION_REDUCE)
            printf(" (prod %d)\n", act.value);
        else
            printf("\n");

        switch (act.type) {
        case ACTION_SHIFT:
            printf("    Action:     Push state %d with symbol %s onto stack\n",
                   act.value,
                   grammar_symbol_name(sra->language, current->type));
            execute_shift(sra, current, act.value, input_pos, logger);
            printf("    Advance:    Input position %d → %d\n",
                   input_pos, input_pos + 1);
            current = current->next;
            input_pos++;
            break;

        case ACTION_REDUCE: {
            const Production *prod =
                grammar_get_production(sra->language, act.value);
            char *pstr = grammar_format_production(sra->language, act.value);
            printf("    Action:     Reduce by production %d: %s\n",
                   act.value, pstr ? pstr : "?");
            printf("                Pop %d symbol(s) from stack\n",
                   prod ? prod->rhs_len : 0);
            free(pstr);

            int exposed_before = -1;
            if (prod) {
                /* peek at what will be exposed after popping */
                int sz = stack_size(sra->stack);
                if (sz > prod->rhs_len) {
                    StackItem item = stack_get(sra->stack,
                                              sz - 1 - prod->rhs_len);
                    exposed_before = item.state;
                }
            }
            execute_reduce(sra, act.value, logger);

            int goto_state = stack_peek(sra->stack).state;
            printf("                GOTO[state %d, %s] = %d  → push\n",
                   exposed_before,
                   prod ? grammar_symbol_name(sra->language, prod->lhs) : "?",
                   goto_state);
            printf("    Lookahead:  (unchanged — still %s)\n",
                   grammar_symbol_name(sra->language, current->type));
            break;
        }

        case ACTION_ACCEPT: {
            char *snap = format_stack(sra->stack, sra->language);
            printf("    ★ ACCEPT ★  Input is valid!\n");
            printf("    Final stack: %s\n", snap ? snap : "");
            free(snap);
            handle_accept(sra, logger);
            printf("════════════════════════════════════════════════\n");
            return 0;
        }

        case ACTION_ERROR:
        default:
            printf("    ✘ ERROR ✘   No valid action for state %d "
                   "with token %s (\"%s\")\n",
                   state,
                   grammar_symbol_name(sra->language, current->type),
                   current->lexeme);
            handle_error(sra, current, logger);
            printf("════════════════════════════════════════════════\n");
            return -1;
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
