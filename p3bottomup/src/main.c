/**
 * @title: Main Program for P3 Bottom-Up Parser
 */

#include <stdio.h>
#include <string.h>

#include "context.h"
#include "language/language.h"
#include "token/token.h"
#include "token/token_loader.h"
#include "parser/parser.h"
#include "trace/trace.h"

int parse_arguments(int argc, char **argv, ParserContext *ctx) {
    if (!ctx) {
        return FALSE;
    }

    if (argc != 3) {
        return FALSE;
    }

    ctx->input_cscn_path = argv[1];
    ctx->language_path = argv[2];
    ctx->debug_output_path = NULL;

    return TRUE;
}

int main(int argc, char **argv) {
    ParserContext ctx;
    LanguageSpec *spec = NULL;
    TokenStream *tokens = NULL;
    TraceWriter *trace = NULL;
    ParseResult result;
    char debug_path[TRACE_PATH_MAX] = {0};

    memset(&ctx, 0, sizeof(ctx));

    if (!parse_arguments(argc, argv, &ctx)) {
        fprintf(stderr, "Usage: %s <input.cscn> <language.txt>\n", argv[0]);
        return 1;
    }

    spec = load_language_spec(ctx.language_path);
    if (!spec) {
        fprintf(stderr, "Error: failed to load language specification: %s\n", ctx.language_path);
        return 1;
    }

    tokens = create_token_stream(16);
    if (!tokens) {
        fprintf(stderr, "Error: failed to allocate token stream\n");
        free_language_spec(&spec);
        return 1;
    }

    if (!load_tokens_from_cscn_tsv(ctx.input_cscn_path, tokens, spec)) {
        fprintf(stderr, "Error: failed to load token file (.cscn): %s\n", ctx.input_cscn_path);
        free_token_stream(&tokens);
        free_language_spec(&spec);
        return 1;
    }

    trace = open_trace_for_input(ctx.input_cscn_path);
    if (!trace) {
        fprintf(stderr, "Error: failed to open debug trace output\n");
        free_token_stream(&tokens);
        free_language_spec(&spec);
        return 1;
    }
    ctx.debug_output_path = trace->output_path;
    strncpy(debug_path, trace->output_path, TRACE_PATH_MAX - 1);
    debug_path[TRACE_PATH_MAX - 1] = '\0';

    result = run_bottomup_parser(spec, tokens, trace);

    close_trace(&trace);
    free_token_stream(&tokens);
    free_language_spec(&spec);

    if (result.status == PARSE_ACCEPTED) {
        printf("PARSE_ACCEPTED\n");
        printf("Debug trace: %s\n", debug_path);
        return 0;
    }

    if (result.status == PARSE_REJECTED) {
        printf("PARSE_REJECTED\n");
        printf("Debug trace: %s\n", debug_path);
        return 2;
    }

    printf("PARSE_INTERNAL_ERROR\n");
    printf("Debug trace: %s\n", debug_path[0] ? debug_path : "-");
    return 1;
}
