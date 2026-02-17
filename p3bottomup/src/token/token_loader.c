/**
 * @title: Token Loader Module for P3 Bottom-Up Parser
 */

#include <stdio.h>
#include <string.h>

#include "token_loader.h"
#include "../utils/string_utils.h"

int map_scanner_token_to_terminal(Token *tok, const LanguageSpec *spec) {
    int symbol_id = -1;

    if (!tok || !spec) {
        return -1;
    }

    if (strcmp(tok->category, "CAT_NUMBER") == 0) {
        symbol_id = find_symbol_id(spec, "NUM", SYM_TERM);
    } else if (strcmp(tok->category, "CAT_OPERATOR") == 0) {
        if (strcmp(tok->lexeme, "+") == 0) {
            symbol_id = find_symbol_id(spec, "+", SYM_TERM);
        } else if (strcmp(tok->lexeme, "*") == 0) {
            symbol_id = find_symbol_id(spec, "*", SYM_TERM);
        }
    } else if (strcmp(tok->category, "CAT_SPECIALCHAR") == 0) {
        if (strcmp(tok->lexeme, "(") == 0) {
            symbol_id = find_symbol_id(spec, "(", SYM_TERM);
        } else if (strcmp(tok->lexeme, ")") == 0) {
            symbol_id = find_symbol_id(spec, ")", SYM_TERM);
        }
    } else if (strcmp(tok->category, "CAT_KEYWORD") == 0) {
        /* Required to run grammar2 from the handout slides (terminal 'int'). */
        if (strcmp(tok->lexeme, "int") == 0) {
            symbol_id = find_symbol_id(spec, "int", SYM_TERM);
        }
    }

    tok->term_symbol_id = symbol_id;
    return symbol_id;
}

int load_tokens_from_cscn_tsv(const char *path, TokenStream *stream, const LanguageSpec *spec) {
    FILE *fp;
    char line[BUFFER_SIZE_LINE];
    int line_no = 0;

    if (!path || !stream || !spec) {
        return FALSE;
    }

    fp = fopen(path, "r");
    if (!fp) {
        return FALSE;
    }

    while (fgets(line, sizeof(line), fp)) {
        Token tok;
        char *tab;
        char *lexeme;
        char *category;

        line_no++;
        trim_whitespace(line);

        if (line[0] == '\0') {
            continue;
        }

        tab = strchr(line, '\t');
        if (!tab) {
            fclose(fp);
            return FALSE;
        }

        *tab = '\0';
        lexeme = line;
        category = tab + 1;

        memset(&tok, 0, sizeof(tok));
        strncpy(tok.lexeme, lexeme, TOKEN_LEXEME_MAX - 1);
        strncpy(tok.category, category, TOKEN_CATEGORY_MAX - 1);
        tok.line = line_no;
        tok.column = 1;
        tok.term_symbol_id = -1;

        map_scanner_token_to_terminal(&tok, spec);

        if (!push_token(stream, &tok)) {
            fclose(fp);
            return FALSE;
        }
    }

    fclose(fp);
    append_eof_token(stream, spec->eof_symbol_id);

    return TRUE;
}
