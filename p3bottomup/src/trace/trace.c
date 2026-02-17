/**
 * @title: Trace Writer Module for P3 Bottom-Up Parser
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trace.h"
#include "../language_defs.h"
#include "../utils/file_utils.h"

static const char *safe_lookahead(const Token *lookahead) {
    if (!lookahead) {
        return "-";
    }
    return lookahead->lexeme;
}

static void trace_line(
    TraceWriter *tw,
    const char *op,
    int input_pos,
    const char *lookahead,
    int prev_state,
    int new_state,
    const char *rule,
    const char *stack_repr
) {
    if (!tw || !tw->fp) {
        return;
    }

    tw->step++;
    fprintf(
        tw->fp,
        "%d | %s | %d | %s | %d | %d | %s | %s\n",
        tw->step,
        op,
        input_pos,
        lookahead ? lookahead : "-",
        prev_state,
        new_state,
        rule ? rule : "-",
        stack_repr ? stack_repr : "[]"
    );
}

TraceWriter *open_trace_for_input(const char *input_cscn_path) {
    TraceWriter *tw;
    char *output_path;

    output_path = build_output_path_with_suffix(
        input_cscn_path,
        P3_TRACE_SUFFIX,
        P3_TRACE_EXTENSION
    );
    if (!output_path) {
        return NULL;
    }

    tw = (TraceWriter *)calloc(1, sizeof(TraceWriter));
    if (!tw) {
        free(output_path);
        return NULL;
    }

    strncpy(tw->output_path, output_path, TRACE_PATH_MAX - 1);
    tw->output_path[TRACE_PATH_MAX - 1] = '\0';
    free(output_path);

    tw->fp = open_file_or_null(tw->output_path, "w");
    if (!tw->fp) {
        free(tw);
        return NULL;
    }

    tw->step = 0;
    fprintf(tw->fp, "STEP | OP | INPUT_POS | LOOKAHEAD | PREV_STATE | NEW_STATE | RULE | STACK\n");

    return tw;
}

void trace_shift(
    TraceWriter *tw,
    int input_pos,
    int prev_state,
    int new_state,
    const Token *lookahead,
    const char *stack_repr
) {
    trace_line(tw, "SHIFT", input_pos, safe_lookahead(lookahead), prev_state, new_state, "-", stack_repr);
}

void trace_reduce(
    TraceWriter *tw,
    int input_pos,
    int prev_state,
    int new_state,
    const ProductionRule *rule,
    const char *stack_repr
) {
    const char *rule_text = rule ? rule->text : "-";
    trace_line(tw, "REDUCE", input_pos, "-", prev_state, new_state, rule_text, stack_repr);
}

void trace_accept(TraceWriter *tw, int input_pos, int state, const char *stack_repr) {
    trace_line(tw, "ACCEPT", input_pos, "$", state, state, "-", stack_repr);
}

void trace_error(TraceWriter *tw, int input_pos, int state, const Token *lookahead, const char *stack_repr) {
    trace_line(tw, "ERROR", input_pos, safe_lookahead(lookahead), state, -1, "-", stack_repr);
}

void close_trace(TraceWriter **tw) {
    if (!tw || !*tw) {
        return;
    }

    if ((*tw)->fp) {
        fclose((*tw)->fp);
        (*tw)->fp = NULL;
    }

    free(*tw);
    *tw = NULL;
}
