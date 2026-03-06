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
    fprintf(stdout, "Language loaded: %d symbols, %d productions\n",
            ctx->lang->symbol_count, ctx->lang->production_count);

    /* ── 2. Load parse table ───────────────────────────────────────── */
    ctx->ptable = parse_table_load_from_file(ctx->table_file);
    if (!ctx->ptable) {
        fprintf(stderr, "parser: failed to load parse table from '%s'\n",
                ctx->table_file);
        return;
    }
    fprintf(stdout, "Parse table loaded: %d states\n", ctx->ptable->state_count);

    
    /* ── 3. Build engine components ────────────────────────────────── */
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

    return;
    /* ── 6. Run parse ──────────────────────────────────────────────── */
    ctx->status = sra_parse(ctx->sra, ctx->tokens, ctx->logger);
    if (ctx->status == EXIT_SUCCESS) {
        fprintf(stdout, "Parse result: ACCEPT\n");
    } else {
        fprintf(stdout, "Parse result: ERROR\n");
    }    
}
