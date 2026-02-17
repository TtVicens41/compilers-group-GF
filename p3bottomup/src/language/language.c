/**
 * @title: Language Specification Module for P3 Bottom-Up Parser
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "language.h"
#include "../utils/string_utils.h"

typedef enum {
    SEC_NONE,
    SEC_SYMBOLS,
    SEC_PRODUCTIONS,
    SEC_ACTION,
    SEC_GOTO
} ParseSection;

typedef struct {
    char **items;
    int size;
    int capacity;
} StringVec;

static int split_ws(char *line, char **parts, int max_parts) {
    int count = 0;
    char *tok = strtok(line, " \t\r\n");
    while (tok && count < max_parts) {
        parts[count++] = tok;
        tok = strtok(NULL, " \t\r\n");
    }
    return count;
}

static int push_string(StringVec *vec, const char *line) {
    char *copy;
    if (vec->size >= vec->capacity) {
        int next_capacity = (vec->capacity == 0) ? 8 : vec->capacity * 2;
        char **next_items = (char **)realloc(vec->items, next_capacity * sizeof(char *));
        if (!next_items) {
            return FALSE;
        }
        vec->items = next_items;
        vec->capacity = next_capacity;
    }

    copy = get_copy(line);
    if (!copy) {
        return FALSE;
    }

    vec->items[vec->size++] = copy;
    return TRUE;
}

static void free_string_vec(StringVec *vec) {
    int i;
    for (i = 0; i < vec->size; i++) {
        free(vec->items[i]);
    }
    free(vec->items);
    vec->items = NULL;
    vec->size = 0;
    vec->capacity = 0;
}

static int parse_symbol_kind(const char *text, SymbolKind *kind_out) {
    if (strcmp(text, "TERM") == 0) {
        *kind_out = SYM_TERM;
        return TRUE;
    }
    if (strcmp(text, "NONTERM") == 0) {
        *kind_out = SYM_NONTERM;
        return TRUE;
    }
    return FALSE;
}

static int parse_action_token(const char *token, ParseAction *out) {
    if (strcmp(token, "err") == 0) {
        out->type = ACT_ERROR;
        out->value = -1;
        return TRUE;
    }
    if (strcmp(token, "acc") == 0) {
        out->type = ACT_ACCEPT;
        out->value = -1;
        return TRUE;
    }
    if (token[0] == 's') {
        out->type = ACT_SHIFT;
        out->value = atoi(token + 1);
        return TRUE;
    }
    if (token[0] == 'r') {
        out->type = ACT_REDUCE;
        out->value = atoi(token + 1);
        return TRUE;
    }
    return FALSE;
}

int find_symbol_id(const LanguageSpec *spec, const char *name, SymbolKind kind) {
    int i;
    if (!spec || !name) {
        return -1;
    }
    for (i = 0; i < spec->symbol_count; i++) {
        if (spec->symbols[i].kind == kind && strcmp(spec->symbols[i].name, name) == 0) {
            return spec->symbols[i].id;
        }
    }
    return -1;
}

static int get_term_column(const LanguageSpec *spec, int term_symbol_id) {
    int i;
    int column = 0;
    for (i = 0; i < spec->symbol_count; i++) {
        if (spec->symbols[i].kind == SYM_TERM) {
            if (spec->symbols[i].id == term_symbol_id) {
                return column;
            }
            column++;
        }
    }
    return -1;
}

static int get_nonterm_column(const LanguageSpec *spec, int nonterm_symbol_id) {
    int i;
    int column = 0;
    for (i = 0; i < spec->symbol_count; i++) {
        if (spec->symbols[i].kind == SYM_NONTERM) {
            if (spec->symbols[i].id == nonterm_symbol_id) {
                return column;
            }
            column++;
        }
    }
    return -1;
}

ParseAction get_action(const LanguageSpec *spec, int state, int term_symbol_id) {
    ParseAction error_action;
    int column;

    error_action.type = ACT_ERROR;
    error_action.value = -1;

    if (!spec || state < 0 || state >= spec->state_count || term_symbol_id < 0) {
        return error_action;
    }

    column = get_term_column(spec, term_symbol_id);
    if (column < 0 || column >= spec->term_count) {
        return error_action;
    }

    return spec->action[state][column];
}

int get_goto_state(const LanguageSpec *spec, int state, int nonterm_symbol_id) {
    int column;

    if (!spec || state < 0 || state >= spec->state_count || nonterm_symbol_id < 0) {
        return -1;
    }

    column = get_nonterm_column(spec, nonterm_symbol_id);
    if (column < 0 || column >= spec->nonterm_count) {
        return -1;
    }

    return spec->go_to[state][column];
}

static int add_symbol(LanguageSpec *spec, int id, SymbolKind kind, const char *name) {
    GrammarSymbol *next_symbols;
    next_symbols = (GrammarSymbol *)realloc(
        spec->symbols,
        (spec->symbol_count + 1) * sizeof(GrammarSymbol)
    );
    if (!next_symbols) {
        return FALSE;
    }

    spec->symbols = next_symbols;
    spec->symbols[spec->symbol_count].id = id;
    spec->symbols[spec->symbol_count].kind = kind;
    strncpy(spec->symbols[spec->symbol_count].name, name, SYMBOL_NAME_MAX - 1);
    spec->symbols[spec->symbol_count].name[SYMBOL_NAME_MAX - 1] = '\0';
    spec->symbol_count++;

    return TRUE;
}

static int add_production(LanguageSpec *spec, const ProductionRule *rule) {
    ProductionRule *next_prods;
    next_prods = (ProductionRule *)realloc(
        spec->prods,
        (spec->prod_count + 1) * sizeof(ProductionRule)
    );
    if (!next_prods) {
        return FALSE;
    }

    spec->prods = next_prods;
    spec->prods[spec->prod_count] = *rule;
    spec->prod_count++;

    return TRUE;
}

static void free_action_table(ParseAction **action, int rows) {
    int i;
    if (!action) {
        return;
    }
    for (i = 0; i < rows; i++) {
        free(action[i]);
    }
    free(action);
}

static void free_goto_table(int **go_to, int rows) {
    int i;
    if (!go_to) {
        return;
    }
    for (i = 0; i < rows; i++) {
        free(go_to[i]);
    }
    free(go_to);
}

void free_language_spec(LanguageSpec **spec) {
    if (!spec || !*spec) {
        return;
    }

    free((*spec)->symbols);
    free((*spec)->prods);
    free_action_table((*spec)->action, (*spec)->state_count);
    free_goto_table((*spec)->go_to, (*spec)->state_count);

    free(*spec);
    *spec = NULL;
}

LanguageSpec *load_language_spec(const char *path) {
    FILE *fp;
    char line[BUFFER_SIZE_LINE];
    ParseSection section = SEC_NONE;

    LanguageSpec *spec;
    char start_symbol_name[SYMBOL_NAME_MAX] = {0};
    char eof_symbol_name[SYMBOL_NAME_MAX] = {0};

    char *action_header = NULL;
    char *goto_header = NULL;
    StringVec action_rows = {0};
    StringVec goto_rows = {0};

    int i;

    fp = fopen(path, "r");
    if (!fp) {
        return NULL;
    }

    spec = (LanguageSpec *)calloc(1, sizeof(LanguageSpec));
    if (!spec) {
        fclose(fp);
        return NULL;
    }

    spec->start_symbol_id = -1;
    spec->eof_symbol_id = -1;

    while (fgets(line, sizeof(line), fp)) {
        char *parts[32];
        int count;

        trim_whitespace(line);
        if (line[0] == '\0' || line[0] == '#') {
            continue;
        }

        if (strcmp(line, "SYMBOLS") == 0) {
            section = SEC_SYMBOLS;
            continue;
        }
        if (strcmp(line, "END_SYMBOLS") == 0) {
            section = SEC_NONE;
            continue;
        }
        if (strcmp(line, "PRODUCTIONS") == 0) {
            section = SEC_PRODUCTIONS;
            continue;
        }
        if (strcmp(line, "END_PRODUCTIONS") == 0) {
            section = SEC_NONE;
            continue;
        }
        if (strcmp(line, "ACTION_TABLE") == 0) {
            section = SEC_ACTION;
            continue;
        }
        if (strcmp(line, "END_ACTION_TABLE") == 0) {
            section = SEC_NONE;
            continue;
        }
        if (strcmp(line, "GOTO_TABLE") == 0) {
            section = SEC_GOTO;
            continue;
        }
        if (strcmp(line, "END_GOTO_TABLE") == 0) {
            section = SEC_NONE;
            continue;
        }

        if (strncmp(line, "START_SYMBOL", 12) == 0) {
            count = sscanf(line, "START_SYMBOL %15s", start_symbol_name);
            if (count != 1) {
                goto fail;
            }
            continue;
        }
        if (strncmp(line, "EOF_SYMBOL", 10) == 0) {
            count = sscanf(line, "EOF_SYMBOL %15s", eof_symbol_name);
            if (count != 1) {
                goto fail;
            }
            continue;
        }

        if (section == SEC_SYMBOLS) {
            SymbolKind kind;
            int id;
            count = split_ws(line, parts, 4);
            if (count != 3) {
                goto fail;
            }

            id = atoi(parts[0]);
            if (!parse_symbol_kind(parts[1], &kind)) {
                goto fail;
            }
            if (!add_symbol(spec, id, kind, parts[2])) {
                goto fail;
            }
            continue;
        }

        if (section == SEC_PRODUCTIONS) {
            ProductionRule rule;
            int id;
            int lhs_id;
            int arrow_pos = -1;
            char raw_line[BUFFER_SIZE_LINE];

            memset(&rule, 0, sizeof(rule));
            strncpy(raw_line, line, sizeof(raw_line) - 1);
            raw_line[sizeof(raw_line) - 1] = '\0';

            count = split_ws(line, parts, 32);
            if (count < 4) {
                goto fail;
            }

            id = atoi(parts[0]);
            lhs_id = find_symbol_id(spec, parts[1], SYM_NONTERM);
            if (lhs_id < 0) {
                goto fail;
            }

            for (i = 0; i < count; i++) {
                if (strcmp(parts[i], "->") == 0) {
                    arrow_pos = i;
                    break;
                }
            }
            if (arrow_pos != 2) {
                goto fail;
            }

            rule.id = id;
            rule.lhs_id = lhs_id;
            rule.rhs_len = 0;

            if (count == 4 && strcmp(parts[3], "EPS") == 0) {
                rule.rhs_len = 0;
            } else {
                for (i = 3; i < count; i++) {
                    int rhs_symbol_id;
                    if (rule.rhs_len >= RULE_RHS_MAX) {
                        goto fail;
                    }
                    rhs_symbol_id = find_symbol_id(spec, parts[i], SYM_TERM);
                    if (rhs_symbol_id < 0) {
                        rhs_symbol_id = find_symbol_id(spec, parts[i], SYM_NONTERM);
                    }
                    if (rhs_symbol_id < 0) {
                        goto fail;
                    }
                    rule.rhs_ids[rule.rhs_len++] = rhs_symbol_id;
                }
            }

            strncpy(rule.text, raw_line, RULE_TEXT_MAX - 1);
            rule.text[RULE_TEXT_MAX - 1] = '\0';

            if (!add_production(spec, &rule)) {
                goto fail;
            }
            continue;
        }

        if (section == SEC_ACTION) {
            if (strncmp(line, "TERMINALS", 9) == 0) {
                free(action_header);
                action_header = get_copy(line);
                if (!action_header) {
                    goto fail;
                }
            } else {
                if (!push_string(&action_rows, line)) {
                    goto fail;
                }
            }
            continue;
        }

        if (section == SEC_GOTO) {
            if (strncmp(line, "NONTERMINALS", 12) == 0) {
                free(goto_header);
                goto_header = get_copy(line);
                if (!goto_header) {
                    goto fail;
                }
            } else {
                if (!push_string(&goto_rows, line)) {
                    goto fail;
                }
            }
            continue;
        }
    }

    if (!action_header || !goto_header) {
        goto fail;
    }

    {
        char *parts[64];
        int count;
        char *header_copy = get_copy(action_header);
        if (!header_copy) {
            goto fail;
        }
        count = split_ws(header_copy, parts, 64);
        if (count < 2) {
            free(header_copy);
            goto fail;
        }
        spec->term_count = count - 1;
        free(header_copy);
    }

    {
        char *parts[64];
        int count;
        char *header_copy = get_copy(goto_header);
        if (!header_copy) {
            goto fail;
        }
        count = split_ws(header_copy, parts, 64);
        if (count < 2) {
            free(header_copy);
            goto fail;
        }
        spec->nonterm_count = count - 1;
        free(header_copy);
    }

    spec->state_count = 0;
    for (i = 0; i < action_rows.size; i++) {
        int st;
        if (sscanf(action_rows.items[i], "%d", &st) == 1 && st + 1 > spec->state_count) {
            spec->state_count = st + 1;
        }
    }
    for (i = 0; i < goto_rows.size; i++) {
        int st;
        if (sscanf(goto_rows.items[i], "%d", &st) == 1 && st + 1 > spec->state_count) {
            spec->state_count = st + 1;
        }
    }

    if (spec->state_count <= 0 || spec->term_count <= 0 || spec->nonterm_count <= 0) {
        goto fail;
    }

    spec->action = (ParseAction **)calloc(spec->state_count, sizeof(ParseAction *));
    spec->go_to = (int **)calloc(spec->state_count, sizeof(int *));
    if (!spec->action || !spec->go_to) {
        goto fail;
    }

    for (i = 0; i < spec->state_count; i++) {
        int j;
        spec->action[i] = (ParseAction *)calloc(spec->term_count, sizeof(ParseAction));
        spec->go_to[i] = (int *)calloc(spec->nonterm_count, sizeof(int));
        if (!spec->action[i] || !spec->go_to[i]) {
            goto fail;
        }
        for (j = 0; j < spec->term_count; j++) {
            spec->action[i][j].type = ACT_ERROR;
            spec->action[i][j].value = -1;
        }
        for (j = 0; j < spec->nonterm_count; j++) {
            spec->go_to[i][j] = -1;
        }
    }

    for (i = 0; i < action_rows.size; i++) {
        char *parts[64];
        int count;
        int state;
        int col;
        char *row_copy = get_copy(action_rows.items[i]);
        if (!row_copy) {
            goto fail;
        }

        count = split_ws(row_copy, parts, 64);
        if (count < 1) {
            free(row_copy);
            continue;
        }

        state = atoi(parts[0]);
        if (state < 0 || state >= spec->state_count) {
            free(row_copy);
            goto fail;
        }

        for (col = 0; col < spec->term_count; col++) {
            ParseAction parsed;
            const char *token = (col + 1 < count) ? parts[col + 1] : "err";
            if (!parse_action_token(token, &parsed)) {
                free(row_copy);
                goto fail;
            }
            spec->action[state][col] = parsed;
        }

        free(row_copy);
    }

    for (i = 0; i < goto_rows.size; i++) {
        char *parts[64];
        int count;
        int state;
        int col;
        char *row_copy = get_copy(goto_rows.items[i]);
        if (!row_copy) {
            goto fail;
        }

        count = split_ws(row_copy, parts, 64);
        if (count < 1) {
            free(row_copy);
            continue;
        }

        state = atoi(parts[0]);
        if (state < 0 || state >= spec->state_count) {
            free(row_copy);
            goto fail;
        }

        for (col = 0; col < spec->nonterm_count; col++) {
            int goto_state = -1;
            if (col + 1 < count) {
                goto_state = atoi(parts[col + 1]);
            }
            spec->go_to[state][col] = goto_state;
        }

        free(row_copy);
    }

    spec->start_symbol_id = find_symbol_id(spec, start_symbol_name, SYM_NONTERM);
    spec->eof_symbol_id = find_symbol_id(spec, eof_symbol_name, SYM_TERM);

    if (spec->start_symbol_id < 0 || spec->eof_symbol_id < 0) {
        goto fail;
    }

    fclose(fp);
    free(action_header);
    free(goto_header);
    free_string_vec(&action_rows);
    free_string_vec(&goto_rows);
    return spec;

fail:
    fclose(fp);
    free(action_header);
    free(goto_header);
    free_string_vec(&action_rows);
    free_string_vec(&goto_rows);
    free_language_spec(&spec);
    return NULL;
}
