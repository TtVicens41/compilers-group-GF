/**
 * @file parser.c
 * @brief High-level parser driver.
 *
 * Assembles all subsystems (Language, ParseTable, DFA, Stack, SRA,
 * TokenList, Logger), runs the parse, and tears everything down.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "sra.h"
#include "../language/language.h"
#include "../language/grammar.h"
#include "../automaton/parse_table.h"
#include "../automaton/dfa.h"
#include "../stack/stack.h"
#include "../tokens/token_list.h"
#include "../tokens/token_loader.h"
#include "../utils/logger.h"

/* ── verbose trace helpers ──────────────────────────────────────────── */

static const char *action_type_str(ActionType t)
{
    switch (t) {
    case ACTION_SHIFT:  return "S";
    case ACTION_REDUCE: return "R";
    case ACTION_ACCEPT: return "A";
    default:            return ".";
    }
}

static void print_symbols(const Language *lang)
{
    printf("\n╔══════════════════════════════════════════════╗\n");
    printf("║            SYMBOL TABLE                      ║\n");
    printf("╠════╦════════════╦════════════════════════════╣\n");
    printf("║ ID ║    Type    ║  Name                      ║\n");
    printf("╠════╬════════════╬════════════════════════════╣\n");
    for (int i = 0; i < lang->symbol_count; i++) {
        const char *type = (lang->symbols[i].type == SYMBOL_TERMINAL)
                           ? "TERMINAL" : "NONTERM ";
        printf("║ %2d ║ %s  ║  %-24s ║\n",
               lang->symbols[i].id, type, lang->symbols[i].name);
    }
    printf("╚════╩════════════╩════════════════════════════╝\n");
    printf("  Start symbol: id %d (%s)\n\n",
           lang->start_symbol,
           grammar_symbol_name(lang, lang->start_symbol));
}

static void print_productions(const Language *lang)
{
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║            PRODUCTIONS                       ║\n");
    printf("╠════╦═════════════════════════════════════════╣\n");
    for (int i = 0; i < lang->production_count; i++) {
        char *pstr = grammar_format_production(lang, lang->productions[i].id);
        printf("║ %2d ║  %-39s ║\n", lang->productions[i].id,
               pstr ? pstr : "?");
        free(pstr);
    }
    printf("╚════╩═════════════════════════════════════════╝\n\n");
}

static void print_action_table(const ParseTable *pt, const Language *lang)
{
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║                      ACTION TABLE                        ║\n");
    printf("╠═══════╦═══════════════════════════════════════════════════╣\n");

    /* header row with terminal names */
    printf("║ State ║");
    for (int t = 0; t < pt->terminal_count; t++) {
        printf(" %6s", grammar_symbol_name(lang, t));
    }
    printf("  ║\n");
    printf("╠═══════╬═══════════════════════════════════════════════════╣\n");

    for (int s = 0; s < pt->state_count; s++) {
        printf("║   %2d  ║", s);
        for (int t = 0; t < pt->terminal_count; t++) {
            Action a = pt->action_table[s][t];
            if (a.type == ACTION_ERROR) {
                printf("      .");
            } else if (a.type == ACTION_ACCEPT) {
                printf("    ACC");
            } else {
                printf("   %s%-3d", action_type_str(a.type), a.value);
            }
        }
        printf("  ║\n");
    }
    printf("╚═══════╩═══════════════════════════════════════════════════╝\n\n");
}

static void print_goto_table(const ParseTable *pt, const Language *lang)
{
    printf("╔═══════════════════════════════════════════╗\n");
    printf("║              GOTO TABLE                   ║\n");
    printf("╠═══════╦═══════════════════════════════════╣\n");

    printf("║ State ║");
    for (int n = 0; n < pt->nonterminal_count; n++) {
        printf(" %6s", grammar_symbol_name(lang, n + pt->terminal_count));
    }
    printf("  ║\n");
    printf("╠═══════╬═══════════════════════════════════╣\n");

    for (int s = 0; s < pt->state_count; s++) {
        int any = 0;
        for (int n = 0; n < pt->nonterminal_count; n++)
            if (pt->goto_table[s][n] >= 0) { any = 1; break; }
        if (!any) continue;  /* skip empty rows */

        printf("║   %2d  ║", s);
        for (int n = 0; n < pt->nonterminal_count; n++) {
            int g = pt->goto_table[s][n];
            if (g < 0)  printf("      .");
            else        printf("   %3d", g);
        }
        printf("  ║\n");
    }
    printf("╚═══════╩═══════════════════════════════════╝\n\n");
}

static void print_token_stream(const TokenList *tokens, const Language *lang)
{
    if (!tokens || !lang) { return; }
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║            INPUT TOKEN STREAM                ║\n");
    printf("╠═════╦══════════════╦═════════════════════════╣\n");
    printf("║ Pos ║  Terminal    ║  Lexeme                  ║\n");
    printf("╠═════╬══════════════╬═════════════════════════╣\n");
    int pos = 0;
    for (Token *t = tokens->head; t; t = t->next) {
        printf("║  %2d ║ %-12s ║  %-23s ║\n",
               pos, grammar_symbol_name(lang, t->type), t->lexeme);
        pos++;
    }
    printf("╚═════╩══════════════╩═════════════════════════╝\n\n");
}

/* ── output-path derivation ─────────────────────────────────────────── */

/**
 * Derive the debug-output path from the token file path.
 * Rule: replace the extension (or append) with "_p3dbg.txt".
 * Caller must free() the result.
 */
static char *derive_output_path(const char *token_file)
{
    size_t len = strlen(token_file);
    /* find last dot */
    const char *dot = strrchr(token_file, '.');
    size_t base_len = dot ? (size_t)(dot - token_file) : len;

    const char *suffix = "_p3dbg.txt";
    size_t slen = strlen(suffix);
    char *out = malloc(base_len + slen + 1);
    if (!out) return NULL;

    memcpy(out, token_file, base_len);
    memcpy(out + base_len, suffix, slen);
    out[base_len + slen] = '\0';
    return out;
}

/* ── public API ─────────────────────────────────────────────────────── */

void parser_run(ParserContext *ctx)
{
    ctx->status = EXIT_FAILURE;

    /* ── 1. Load language ──────────────────────────────────────────── */
    ctx->lang = language_load_from_file(ctx->language_file);
    if (!ctx->lang) {
        fprintf(stderr, "parser: failed to load language from '%s'\n",
                ctx->language_file);
        return;
    }
    printf("Language loaded: %d symbols, %d productions\n",
           ctx->lang->symbol_count, ctx->lang->production_count);
    print_symbols(ctx->lang);
    print_productions(ctx->lang);

    /* ── 2. Load parse table ───────────────────────────────────────── */
    ctx->ptable = parse_table_load_from_file(ctx->table_file);
    if (!ctx->ptable) {
        fprintf(stderr, "parser: failed to load parse table from '%s'\n",
                ctx->table_file);
        return;
    }
    printf("Parse table loaded: %d states\n", ctx->ptable->state_count);
    print_action_table(ctx->ptable, ctx->lang);
    print_goto_table(ctx->ptable, ctx->lang);

    /* ── 3. Load tokens ────────────────────────────────────────────── */
    ctx->tokens = token_loader_load_from_file(ctx->input_file, ctx->lang);
    if (!ctx->tokens) {
        fprintf(stderr, "parser: failed to load tokens from '%s'\n",
                ctx->input_file);
        return;
    }
    printf("Tokens loaded successfully\n");
    print_token_stream(ctx->tokens, ctx->lang);

    /* ── 4. Build engine components ────────────────────────────────── */
    ctx->dfa   = dfa_init(ctx->ptable);
    ctx->stack = stack_init();
    if (!ctx->dfa || !ctx->stack) {
        fprintf(stderr, "parser: failed to initialise DFA or stack\n");
        return;
    }
    
    ctx->sra = sra_init(ctx->dfa, ctx->stack, ctx->lang);
    if (!ctx->sra) {
        fprintf(stderr, "parser: failed to initialise SRA\n");
        return;
    }

    /* ── 4. Load tokens ────────────────────────────────────────────── */
    ctx->tokens = token_loader_load_from_file(ctx->input_file, ctx->lang);
    if (!ctx->tokens) {
        fprintf(stderr, "parser: failed to load tokens from '%s'\n",
                ctx->input_file);
        return;
    }
    fprintf(stdout, "Tokens loaded successfully\n");

    /* ── 5. Open logger ────────────────────────────────────────────── */
    ctx->logger = logger_init(ctx->output_file);
    if (!ctx->logger) {
        fprintf(stderr, "parser: warning – could not open log file\n");
    } else {
        fprintf(stdout, "Debug output: %s\n", ctx->output_file);
    }

    /* ── 6. Run parse ──────────────────────────────────────────────── */
    ctx->status = sra_parse(ctx->sra, ctx->tokens, ctx->logger);
    if (ctx->status == EXIT_SUCCESS) {
        fprintf(stdout, "Parse result: ACCEPT\n");
    } else {
        fprintf(stdout, "Parse result: ERROR\n");
    }    
}
