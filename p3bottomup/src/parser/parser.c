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

int parser_run(const char *language_file,
               const char *table_file,
               const char *token_file,
               const char *output_file)
{
    int rc = -1;

    /* components – NULL until successfully created */
    Language   *lang   = NULL;
    ParseTable *ptable = NULL;
    DFA        *dfa    = NULL;
    Stack      *stack  = NULL;
    SRA        *sra    = NULL;
    TokenList  *tokens = NULL;
    Logger     *logger = NULL;
    char       *out_path = NULL;

    /* ── 1. Load language ──────────────────────────────────────────── */
    lang = language_load_from_file(language_file);
    if (!lang) {
        fprintf(stderr, "parser: failed to load language from '%s'\n",
                language_file);
        goto cleanup;
    }
    printf("Language loaded: %d symbols, %d productions\n",
           lang->symbol_count, lang->production_count);

    /* ── 2. Load parse table ───────────────────────────────────────── */
    ptable = parse_table_load_from_file(table_file);
    if (!ptable) {
        fprintf(stderr, "parser: failed to load parse table from '%s'\n",
                table_file);
        goto cleanup;
    }
    printf("Parse table loaded: %d states\n", ptable->state_count);

    /* ── 3. Load tokens ────────────────────────────────────────────── */
    tokens = token_loader_load_from_file(token_file, lang);
    if (!tokens) {
        fprintf(stderr, "parser: failed to load tokens from '%s'\n",
                token_file);
        goto cleanup;
    }
    printf("Tokens loaded successfully\n");

    /* ── 4. Build engine components ────────────────────────────────── */
    dfa   = dfa_init(ptable);
    stack = stack_init(128);
    if (!dfa || !stack) {
        fprintf(stderr, "parser: failed to initialise DFA or stack\n");
        goto cleanup;
    }

    sra = sra_init(dfa, stack, lang);
    if (!sra) {
        fprintf(stderr, "parser: failed to initialise SRA\n");
        goto cleanup;
    }

    /* ── 5. Open logger ────────────────────────────────────────────── */
    if (output_file) {
        out_path = NULL;   /* use the provided path directly */
        logger = logger_init(output_file);
    } else {
        out_path = derive_output_path(token_file);
        logger = logger_init(out_path);
    }
    if (!logger) {
        fprintf(stderr, "parser: warning – could not open log file\n");
        /* continue without logging */
    } else {
        printf("Debug output: %s\n",
               output_file ? output_file : out_path);
    }

    /* ── 6. Run parse ──────────────────────────────────────────────── */
    rc = sra_parse(sra, tokens, logger);

    if (rc == 0) {
        printf("Parse result: ACCEPT\n");
    } else {
        printf("Parse result: ERROR\n");
    }

cleanup:
    logger_close(logger);
    sra_destroy(sra);
    stack_destroy(stack);
    dfa_destroy(dfa);
    token_list_destroy(tokens);
    parse_table_destroy(ptable);
    language_destroy(lang);
    free(out_path);
    return rc;
}
