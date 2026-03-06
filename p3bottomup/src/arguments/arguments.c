/**
 * @file arguments.c
 * @brief Module for parse of command-line arguments 
 * @author Marc Bosch Manzano
 * @since 2026/02/25
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "arguments.h"
#include "../utils/file_utils.h"
#include "../utils/string_utils.h"

void parse_arguments(int argc, char *argv[], ParserContext *ctx)
{
    if (argc <= 1) {
        print_file(PATH_MAN_PAGE);
        ctx->status = EXIT_FAILURE;
        return;
    }

    ctx->man_page      = PATH_MAN_PAGE;
    ctx->language_file = PATH_LANGUAGE;
    ctx->table_file    = PATH_PARSE_TABLE;
    ctx->input_file    = argv[1];
    ctx->output_file   = replace_extension(argv[1], DEBUG_SUFFIX);    
    ctx->status        = EXIT_SUCCESS;
    
    return;
}

void context_exit(const ParserContext *ctx) 
{
    if (ctx->status == EXIT_FAILURE) {
        exit(ctx->status);
    }
}

void context_cleanup(ParserContext *ctx) 
{
    logger_close(ctx->logger);
    stack_destroy(ctx->stack);
    parse_table_destroy(ctx->dfa->table);
    dfa_destroy(ctx->dfa);
    token_list_destroy(ctx->tokens);
    language_destroy(ctx->lang);
}

static const char* const STATUS[] = { "EXIT_SUCCESS", "EXIT_FAILURE" };

char *context_string(const ParserContext *ctx, int level) 
{
    if (!ctx) {
        return NULL;
    }
    char *s = get_copy("\n");
    int n = level + 1;
    jsonify_wrap(&s, level, 1, "{");
    jsonify(&s, n, 1, 1, "man_page_path",      add_quotes(ctx->man_page));
    jsonify(&s, n, 1, 1, "input_file_path",    add_quotes(ctx->input_file));
    jsonify(&s, n, 1, 1, "output_file_path",   add_quotes(ctx->output_file));
    jsonify(&s, n, 1, 1, "language_file_path", add_quotes(ctx->language_file));
    jsonify(&s, n, 1, 1, "table_file_path",    add_quotes(ctx->table_file));
    jsonify(&s, n, 1, 1, "sra",                sra_string(ctx->sra, n));
    jsonify(&s, n, 1, 1, "tokens",             token_list_string(ctx->tokens, n));
    jsonify(&s, n, 1, 1, "logger",             logger_string(ctx->logger, n));
    jsonify(&s, n, 0, 1, "status",             add_quotes(STATUS[ctx->status]));
    jsonify_wrap(&s, level, 0, "}");
    return s;
}