/**
 * @title: lexer.c
 * @authors: Joan Vicente, Pau Puig
 * @creation:
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "lexer_config.h"
#include "token.h"
#include "utils/error.h"
#include "utils/error_utils.h"
#include "utils/string_utils.h"

#define AUTOMATA_CANDIDATE_1 "./resources/automata.txt"
#define AUTOMATA_CANDIDATE_2 "p2lx/resources/automata.txt"
#define AUTOMATA_CANDIDATE_3 "../resources/automata.txt"

#define LINE_BUFFER_SIZE 4096

/**
 * @brief Checks whether a character acts as a lexical separator.
 * @param Receives: `char c`.
 * @return Returns `1` for whitespace and `0` otherwise.
 * @details Used to delimit tokens during character-by-character scanning.
 */
static int is_whitespace_char(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\f' || c == '\v';
}

/**
 * @brief Resolves the automata file path among candidate locations.
 * @param Does not receive parameters.
 * @return Returns the valid path or `NULL` when no file is found.
 * @details Allows running from different working directories without breaking the scanner.
 */
static const char *resolve_automata_path(void) {
    const char *candidates[] = {
        AUTOMATA_CANDIDATE_1,
        AUTOMATA_CANDIDATE_2,
        AUTOMATA_CANDIDATE_3,
        NULL
    };
    int i = 0;

    while (candidates[i]) {
        FILE *probe = fopen(candidates[i], "r");
        if (probe) {
            fclose(probe);
            return candidates[i];
        }
        i++;
    }

    return NULL;
}

/**
 * @brief Checks whether a character belongs to the supported operator set.
 * @param Receives: `char c`.
 * @return Returns `1` for operator characters and `0` otherwise.
 * @details Quickly isolates operator tokens without relying on the general NFA.
 */
static int is_operator_char(char c) {
    return c == '=' || c == '>' || c == '+' || c == '*';
}

/**
 * @brief Checks whether a character belongs to the supported special-character set.
 * @param Receives: `char c`.
 * @return Returns `1` for special characters and `0` otherwise.
 * @details Helps generate one-character tokens for language punctuation.
 */
static int is_special_char(char c) {
    return c == '(' || c == ')' || c == ';' || c == '{' || c == '}' ||
           c == '[' || c == ']' || c == ',';
}

/**
 * @brief Copies a substring into a new dynamic buffer.
 * @param Receives: `const char *src, int start, int end`.
 * @return Returns the allocated substring.
 * @details Used to extract lexemes without mutating the input buffer.
 */
static char *copy_slice(const char *src, int start, int end) {
    int len;
    char *out;

    if (end < start) {
        return get_copy("");
    }

    len = end - start + 1;
    out = (char *)calloc((size_t)len + 1, sizeof(char));
    if (!out) {
        return NULL;
    }

    memcpy(out, src + start, (size_t)len);
    out[len] = '\0';
    return out;
}

/**
 * @brief Appends text to a dynamic buffer with automatic growth.
 * @param Receives: `char **buffer, size_t *cap, size_t *len, const char *text`.
 * @return Returns `1` on success and `0` on allocation failure.
 * @details Centralizes reallocation logic while building output lines.
 */
static int append_text(char **buffer, size_t *cap, size_t *len, const char *text) {
    size_t need;
    char *grown;

    if (!text) {
        return 1;
    }

    need = *len + strlen(text) + 1;
    if (need > *cap) {
        size_t next_cap = (*cap == 0 ? 128 : *cap);
        while (next_cap < need) {
            next_cap *= 2;
        }
        grown = (char *)realloc(*buffer, next_cap);
        if (!grown) {
            return 0;
        }
        *buffer = grown;
        *cap = next_cap;
    }

    strcpy(*buffer + *len, text);
    *len += strlen(text);
    return 1;
}

/**
 * @brief Formats and appends the textual representation of one token.
 * @param Receives: `char **line_buffer, size_t *cap, size_t *len, const char *lexeme, TokenCategory category, int include_sep`.
 * @return Returns `1` when appended correctly and `0` on allocation failure.
 * @details Mantiene un unico formato `<lexeme, CAT_X>` para todos los modos.
 */
static int append_token_repr(char **line_buffer, size_t *cap, size_t *len,
                             const char *lexeme, TokenCategory category,
                             int include_sep) {
    char token_repr[2048];

    snprintf(token_repr, sizeof(token_repr), "<%s, %s>", lexeme,
             token_category_text(category));

    if (include_sep && !append_text(line_buffer, cap, len, " ")) {
        return 0;
    }
    if (!append_text(line_buffer, cap, len, token_repr)) {
        return 0;
    }

    return 1;
}

/**
 * @brief Writes one tokenized line to the output file using the active format.
 * @param Receives: `Lexer *lexer, int input_line, const char *line_text, int has_tokens`.
 * @return Returns `1` if writing succeeds and `0` otherwise.
 * @details Keeps RELEASE/DEBUG output policy isolated from scanning logic.
 */
static int write_line_output(Lexer *lexer, int input_line, const char *line_text,
                             int has_tokens) {
    if (!has_tokens) {
        return 1;
    }

#if OUTFORMAT == OUTFORMAT_DEBUG
    fprintf(lexer->output_file, "%d %s\n\n", input_line, line_text);
#else
    (void)input_line;
    fprintf(lexer->output_file, "%s\n", line_text);
#endif
    COUNT_IO(&lexer->counter, input_line, (long)strlen(line_text) + 1);
    return 1;
}

/**
 * @brief Inserts one token in memory and in the current rendered line.
 * @param Receives: `Lexer *lexer, const char *lexeme, TokenCategory category, int line, int column, char **out_line, size_t *out_cap, size_t *out_len, int *line_has_tokens`.
 * @return Returns `1` on success and `0` on failure.
 * @details Keeps the internal token list synchronized with textual output.
 */
static int push_token(Lexer *lexer, const char *lexeme, TokenCategory category,
                      int line, int column, char **out_line, size_t *out_cap,
                      size_t *out_len, int *line_has_tokens) {
    if (!token_list_push(&lexer->tokens, lexeme, category, line, column)) {
        return 0;
    }

    if (!append_token_repr(out_line, out_cap, out_len, lexeme, category,
                           *line_has_tokens)) {
        return 0;
    }

    *line_has_tokens = 1;
    COUNT_GEN(&lexer->counter, line, 1);
    return 1;
}

/**
 * @brief Reports a non-recognized lexeme with line and column context.
 * @param Receives: `Lexer *lexer, int line, int column, const char *lexeme`.
 * @return Does not return a value.
 * @details Centraliza el mensaje para mantener formato de error uniforme.
 */
static void report_nonrecognized(Lexer *lexer, int line, int column,
                                 const char *lexeme) {
    report_lexer_error(lexer->debug_stream, ERR_NON_RECOGNIZED, line, column, lexeme,
                       "Token emitted as CAT_NONRECOGNIZED");
}

/**
 * @brief Parses a quoted literal starting at the current cursor position.
 * @param Receives: `const char *line, int *i, int len, char **lexeme_out, int *is_terminated`.
 * @return Returns `1` after processing the literal and updating cursors.
 * @details Detects unterminated literals and emits a specific lexical error.
 */
static int parse_literal(const char *line, int *i, int len, char **lexeme_out,
                         int *is_terminated) {
    int start = *i;
    int cursor = *i + 1;

    while (cursor < len && line[cursor] != '"' && line[cursor] != '\n') {
        cursor++;
    }

    if (cursor < len && line[cursor] == '"') {
        *lexeme_out = copy_slice(line, start, cursor);
        *i = cursor + 1;
        *is_terminated = 1;
        return 1;
    }

    *lexeme_out = copy_slice(line, start, len - 1);
    *i = len;
    *is_terminated = 0;
    return 1;
}

/**
 * @brief Extracts a generic chunk until a lexical delimiter is found.
 * @param Receives: `const char *line, int *i, int len, char **lexeme_out`.
 * @return Returns `1` after producing the lexeme and moving the cursor.
 * @details Used for numbers, identifiers, and non-recognized chunks.
 */
static int parse_generic_chunk(const char *line, int *i, int len, char **lexeme_out) {
    int start = *i;
    int cursor = *i;

    while (cursor < len && !is_whitespace_char(line[cursor]) &&
           !is_operator_char(line[cursor]) && !is_special_char(line[cursor]) &&
           line[cursor] != '"') {
        cursor++;
    }

    *lexeme_out = copy_slice(line, start, cursor - 1);
    *i = cursor;
    return 1;
}

/**
 * @brief Classifies a generic lexeme using the loaded NFA.
 * @param Receives: `const Lexer *lexer, const char *lexeme`.
 * @return Returns the resulting token category.
 * @details Explicitly filters categories accepted by this scanning flow.
 */
static TokenCategory classify_generic_chunk(const Lexer *lexer, const char *lexeme) {
    TokenCategory category = classify_lexeme_nfa(lexer->nfa, lexeme);

    if (category == CAT_NONRECOGNIZED) {
        return CAT_NONRECOGNIZED;
    }

    if (category == CAT_NUMBER) {
        return CAT_NUMBER;
    }

    if (category == CAT_KEYWORD) {
        return CAT_KEYWORD;
    }

    if (category == CAT_IDENTIFIER) {
        return CAT_IDENTIFIER;
    }

    return CAT_NONRECOGNIZED;
}

/**
 * @brief Processes a full input line and generates tokens in memory and output.
 * @param Receives: `Lexer *lexer, const char *line`.
 * @return Returns `1` if the line is processed correctly and `0` on failure.
 * @details This is the line-level scanning core with lexical error handling.
 */
static int process_line(Lexer *lexer, const char *line) {
    int len = (int)strlen(line);
    int i = 0;
    int line_has_tokens = 0;
    char *line_render = NULL;
    size_t line_render_cap = 0;
    size_t line_render_len = 0;

    while (i < len) {
        int column = i + 1;
        char c = line[i];

        COUNT_COMP(&lexer->counter, lexer->current_line, 1);

        if (is_whitespace_char(c)) {
            i++;
            continue;
        }

        if (is_operator_char(c) || is_special_char(c)) {
            char lexeme[2] = {c, '\0'};
            TokenCategory category = is_operator_char(c) ? CAT_OPERATOR : CAT_SPECIALCHAR;

            if (!push_token(lexer, lexeme, category, lexer->current_line, column,
                            &line_render, &line_render_cap, &line_render_len,
                            &line_has_tokens)) {
                free(line_render);
                return 0;
            }

            i++;
            continue;
        }

        if (c == '"') {
            char *lexeme = NULL;
            int terminated = 0;
            if (!parse_literal(line, &i, len, &lexeme, &terminated)) {
                free(line_render);
                return 0;
            }

            if (terminated) {
                if (!push_token(lexer, lexeme, CAT_LITERAL, lexer->current_line, column,
                                &line_render, &line_render_cap, &line_render_len,
                                &line_has_tokens)) {
                    free(lexeme);
                    free(line_render);
                    return 0;
                }
            } else {
                report_lexer_error(lexer->debug_stream, ERR_UNTERMINATED_LITERAL,
                                   lexer->current_line, column, lexeme,
                                   "Literal closed at end-of-line without quote");
                if (!push_token(lexer, lexeme, CAT_NONRECOGNIZED,
                                lexer->current_line, column, &line_render,
                                &line_render_cap, &line_render_len,
                                &line_has_tokens)) {
                    free(lexeme);
                    free(line_render);
                    return 0;
                }
            }

            free(lexeme);
            continue;
        }

        {
            char *lexeme = NULL;
            TokenCategory category;

            if (!parse_generic_chunk(line, &i, len, &lexeme)) {
                free(line_render);
                return 0;
            }

            category = classify_generic_chunk(lexer, lexeme);
            if (category == CAT_NONRECOGNIZED) {
                report_nonrecognized(lexer, lexer->current_line, column, lexeme);
            }

            if (!push_token(lexer, lexeme, category, lexer->current_line, column,
                            &line_render, &line_render_cap, &line_render_len,
                            &line_has_tokens)) {
                free(lexeme);
                free(line_render);
                return 0;
            }

            free(lexeme);
        }
    }

    if (!line_render) {
        line_render = get_copy("");
        if (!line_render) {
            return 0;
        }
    }

    if (!write_line_output(lexer, lexer->current_line, line_render, line_has_tokens)) {
        free(line_render);
        return 0;
    }

    free(line_render);
    return 1;
}

/**
 * @brief Builds the scanner output path by appending `scn`.
 * @param Receives: `const char *input_path`.
 * @return Returns a newly allocated output path or `NULL`.
 * @details Preserves the `<input>.cscn` convention required by the assignment.
 */
char *build_scanner_output_path(const char *input_path) {
    size_t len;
    char *out;

    if (!input_path) {
        return NULL;
    }

    len = strlen(input_path);
    out = (char *)calloc(len + strlen("scn") + 1, sizeof(char));
    if (!out) {
        return NULL;
    }

    strcpy(out, input_path);
    strcat(out, "scn");
    return out;
}

/**
 * @brief Builds the counter output path by appending `dbgcnt`.
 * @param Receives: `const char *input_path`.
 * @return Returns the allocated counter file path or `NULL`.
 * @details Used when `COUNTOUT` redirects counting messages to a separate file.
 */
char *build_count_output_path(const char *input_path) {
    size_t len;
    char *out;

    if (!input_path) {
        return NULL;
    }

    len = strlen(input_path);
    out = (char *)calloc(len + strlen("dbgcnt") + 1, sizeof(char));
    if (!out) {
        return NULL;
    }

    strcpy(out, input_path);
    strcat(out, "dbgcnt");
    return out;
}

/**
 * @brief Validates that the input file has `.c` extension.
 * @param Receives: `const char *path`.
 * @return Returns `1` if the extension is valid and `0` otherwise.
 * @details Enforces the input restriction defined by the P2 specification.
 */
static int has_c_extension(const char *path) {
    const char *dot;
    if (!path) {
        return 0;
    }

    dot = strrchr(path, '.');
    if (!dot) {
        return 0;
    }

    return strcmp(dot, ".c") == 0;
}

/**
 * @brief Runs the complete scanner pipeline on one input file.
 * @param Receives: `const char *input_path, const char *output_path, TokenList *out_tokens`.
 * @return Returns `0` on success and `1` on error.
 * @details Initializes resources, processes lines, handles errors, and frees memory.
 */
int run_lexer(const char *input_path, const char *output_path, TokenList *out_tokens) {
    Lexer lexer;
    FILE *input = NULL;
    char line[LINE_BUFFER_SIZE];
    int status = 1;
    int counting_enabled = 0;

    if (!input_path || !output_path || !out_tokens) {
        return 1;
    }

    if (!has_c_extension(input_path)) {
        report_lexer_error(stderr, ERR_FILE_INPUT, 0, 0, input_path,
                           "Input must have .c extension");
        return 1;
    }

    memset(&lexer, 0, sizeof(Lexer));
    token_list_init(&lexer.tokens);

    input = fopen(input_path, "r");
    if (!input) {
        report_lexer_error(stderr, ERR_FILE_INPUT, 0, 0, input_path, NULL);
        return 1;
    }

    lexer.output_file = fopen(output_path, "w");
    if (!lexer.output_file) {
        report_lexer_error(stderr, ERR_FILE_OUTPUT, 0, 0, output_path, NULL);
        fclose(input);
        return 1;
    }

#if DEBUG == DEBUG_ON
    lexer.debug_stream = lexer.output_file;
#else
    lexer.debug_stream = stdout;
#endif

#if (COUNTCONFIG == COUNT_ENABLED) && (OUTFORMAT == OUTFORMAT_DEBUG)
    counting_enabled = 1;
#endif

    counter_init(&lexer.counter, NULL, counting_enabled);
    if (counting_enabled) {
#if COUNTOUT == OUT
        lexer.count_stream = lexer.output_file;
#else
        char *count_path = build_count_output_path(input_path);
        lexer.count_stream = fopen(count_path, "w");
        free(count_path);
#endif
        counter_set_stream(&lexer.counter, lexer.count_stream);
    }

    {
        const char *automata_path = resolve_automata_path();
        if (!automata_path) {
            report_lexer_error(lexer.debug_stream, ERR_INTERNAL, 0, 0,
                               "automata", "Automata file not found");
            goto cleanup;
        }
        lexer.nfa = read_union_nfa(automata_path);
    }
    if (!lexer.nfa) {
        report_lexer_error(lexer.debug_stream, ERR_INTERNAL, 0, 0,
                           "automata", "Failed to read automata file");
        goto cleanup;
    }

    lexer.current_line = 0;

    while (fgets(line, sizeof(line), input)) {
        lexer.current_line++;
        COUNT_IO(&lexer.counter, lexer.current_line, (long)strlen(line));
        if (!process_line(&lexer, line)) {
            report_lexer_error(lexer.debug_stream, ERR_INTERNAL, lexer.current_line, 0,
                               "line", "Failed while processing input line");
            goto cleanup;
        }
    }

    *out_tokens = lexer.tokens;
    status = 0;

cleanup:
    if (status != 0) {
        token_list_free(&lexer.tokens);
    }

    free_nfa(lexer.nfa);

    if (lexer.count_stream && lexer.count_stream != lexer.output_file) {
        fclose(lexer.count_stream);
    }

    if (lexer.output_file) {
        fclose(lexer.output_file);
    }

    if (input) {
        fclose(input);
    }

    return status;
}

/**
 * @brief Runs scanner and optionally chains the parser phase.
 * @param Receives: `const char *input_path, ParserHookFn parser_hook, TokenList *out_tokens, char **generated_output_path`.
 * @return Returns `0` on success and `1` when any phase fails.
 * @details Leaves a prepared hook to integrate P3 without redesigning the pipeline.
 */
int run_pipeline_with_optional_parser(const char *input_path,
                                      ParserHookFn parser_hook,
                                      TokenList *out_tokens,
                                      char **generated_output_path) {
    char *output_path;
    int status;

    output_path = build_scanner_output_path(input_path);
    if (!output_path) {
        return 1;
    }

    status = run_lexer(input_path, output_path, out_tokens);
    if (status == 0) {
#if ENABLE_PARSER_HOOK
        if (parser_hook) {
            status = parser_hook(out_tokens, output_path);
        }
#else
        (void)parser_hook;
#endif
    }

    if (generated_output_path) {
        *generated_output_path = output_path;
    } else {
        free(output_path);
    }

    return status;
}
