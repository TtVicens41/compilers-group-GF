/**
 * @title: Trace Writer Module for P3 Bottom-Up Parser
 */

#ifndef P3_TRACE_H
#define P3_TRACE_H

#include <stdio.h>

#include "../language/language.h"
#include "../token/token.h"

typedef struct {
    FILE *fp;
    char output_path[TRACE_PATH_MAX];
    int step;
} TraceWriter;

TraceWriter *open_trace_for_input(const char *input_cscn_path);
void trace_shift(TraceWriter *tw, int input_pos, int prev_state, int new_state, const Token *lookahead, const char *stack_repr);
void trace_reduce(TraceWriter *tw, int input_pos, int prev_state, int new_state, const ProductionRule *rule, const char *stack_repr);
void trace_accept(TraceWriter *tw, int input_pos, int state, const char *stack_repr);
void trace_error(TraceWriter *tw, int input_pos, int state, const Token *lookahead, const char *stack_repr);
void close_trace(TraceWriter **tw);

#endif /* P3_TRACE_H */
