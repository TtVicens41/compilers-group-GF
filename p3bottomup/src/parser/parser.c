/**
 * @title: Bottom-Up Parser Module for P3
 */

#include <string.h>

#include "parser.h"
#include "../stack/parse_stack.h"

static const ProductionRule *find_rule_by_id(const LanguageSpec *spec, int rule_id) {
    int i;
    if (!spec) {
        return NULL;
    }
    for (i = 0; i < spec->prod_count; i++) {
        if (spec->prods[i].id == rule_id) {
            return &spec->prods[i];
        }
    }
    return NULL;
}

ParseResult run_bottomup_parser(const LanguageSpec *spec, TokenStream *tokens, TraceWriter *trace) {
    ParseResult result;
    ParseStack *stack;
    StackEntry initial;
    int internal_steps = 0;

    char stack_repr[STACK_TRACE_MAX];

    memset(&result, 0, sizeof(result));
    result.status = PARSE_INTERNAL_ERROR;
    result.final_state = -1;
    result.last_rule_id = -1;

    if (!spec || !tokens) {
        return result;
    }

    stack = create_parse_stack(16);
    if (!stack) {
        return result;
    }

    memset(&initial, 0, sizeof(initial));
    initial.state = 0;
    initial.symbol_id = -1;

    if (!push_stack(stack, &initial)) {
        free_parse_stack(&stack);
        return result;
    }

    while (1) {
        StackEntry *top;
        Token *lookahead;
        ParseAction action;

        top = peek_stack(stack);
        lookahead = peek_token(tokens);

        if (!top || !lookahead) {
            result.status = PARSE_INTERNAL_ERROR;
            break;
        }

        action = get_action(spec, top->state, lookahead->term_symbol_id);
        internal_steps++;

        if (action.type == ACT_SHIFT) {
            StackEntry shifted;
            int prev_state = top->state;

            memset(&shifted, 0, sizeof(shifted));
            shifted.state = action.value;
            shifted.symbol_id = lookahead->term_symbol_id;
            shifted.token = *lookahead;

            if (!push_stack(stack, &shifted)) {
                result.status = PARSE_INTERNAL_ERROR;
                break;
            }

            format_stack(stack, spec, stack_repr, sizeof(stack_repr));
            trace_shift(trace, tokens->cursor, prev_state, shifted.state, lookahead, stack_repr);

            advance_token(tokens);
            result.consumed_tokens = tokens->cursor;
            continue;
        }

        if (action.type == ACT_REDUCE) {
            const ProductionRule *rule;
            StackEntry *new_top;
            StackEntry reduced;
            int prev_state = top->state;
            int goto_state;
            int pop_failed = FALSE;
            int i;

            rule = find_rule_by_id(spec, action.value);
            if (!rule) {
                result.status = PARSE_INTERNAL_ERROR;
                break;
            }

            for (i = 0; i < rule->rhs_len; i++) {
                if (!pop_stack(stack, NULL)) {
                    pop_failed = TRUE;
                    break;
                }
            }
            if (pop_failed) {
                result.status = PARSE_INTERNAL_ERROR;
                break;
            }

            new_top = peek_stack(stack);
            if (!new_top) {
                result.status = PARSE_INTERNAL_ERROR;
                break;
            }

            goto_state = get_goto_state(spec, new_top->state, rule->lhs_id);
            if (goto_state < 0) {
                format_stack(stack, spec, stack_repr, sizeof(stack_repr));
                trace_error(trace, tokens->cursor, new_top->state, lookahead, stack_repr);
                result.status = PARSE_REJECTED;
                result.final_state = new_top->state;
                break;
            }

            memset(&reduced, 0, sizeof(reduced));
            reduced.state = goto_state;
            reduced.symbol_id = rule->lhs_id;
            if (!push_stack(stack, &reduced)) {
                result.status = PARSE_INTERNAL_ERROR;
                break;
            }

            format_stack(stack, spec, stack_repr, sizeof(stack_repr));
            trace_reduce(trace, tokens->cursor, prev_state, goto_state, rule, stack_repr);

            result.last_rule_id = rule->id;
            continue;
        }

        if (action.type == ACT_ACCEPT) {
            format_stack(stack, spec, stack_repr, sizeof(stack_repr));
            trace_accept(trace, tokens->cursor, top->state, stack_repr);
            result.status = PARSE_ACCEPTED;
            result.final_state = top->state;
            break;
        }

        format_stack(stack, spec, stack_repr, sizeof(stack_repr));
        trace_error(trace, tokens->cursor, top->state, lookahead, stack_repr);
        result.status = PARSE_REJECTED;
        result.final_state = top->state;
        break;
    }

    result.steps = trace ? trace->step : internal_steps;

    free_parse_stack(&stack);
    return result;
}
