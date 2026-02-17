/**
 * @title: Parser Context for P3 Bottom-Up Parser
 */

#ifndef P3_CONTEXT_H
#define P3_CONTEXT_H

typedef struct {
    const char *input_cscn_path;
    const char *language_path;
    const char *debug_output_path;
} ParserContext;

int parse_arguments(int argc, char **argv, ParserContext *ctx);

#endif /* P3_CONTEXT_H */
