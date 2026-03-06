/**
 * @file logger.c
 * @brief Debug logger for the shift/reduce parser.
 * 
 * @author Davi Penna-Mattos Dias de Paiva.
 * @review: Marc Bosch Manzano.
 */

#include <stdlib.h>
#include <string.h>

#include "logger.h"
#include "string_utils.h"


Logger *logger_init(const char *output_path)
{
    if (!output_path) return NULL;

    Logger *lg = malloc(sizeof(Logger));
    if (!lg) return NULL;

    lg->file = fopen(output_path, "w");
    if (!lg->file) {
        fprintf(stderr, "logger: cannot open '%s' for writing\n",
                output_path);
        free(lg);
        return NULL;
    }
    lg->step = 0;
    return lg;
}

void logger_log_shift(Logger *logger, int input_pos,
                      int prev_state, int new_state,
                      const char *stack_str)
{
    if (!logger || !logger->file) return;
    logger->step++;
    fprintf(logger->file,
        "STEP %d\n"
        "  Operation:     SHIFT\n"
        "  InputPos:      %d\n"
        "  PreviousState: %d\n"
        "  NewState:      %d\n"
        "  Stack:         %s\n\n",
        logger->step, input_pos, prev_state, new_state,
        stack_str ? stack_str : "");
}

void logger_log_reduce(Logger *logger, int prod_id,
                       const char *prod_str, int popped,
                       int goto_state, const char *stack_str)
{
    if (!logger || !logger->file) return;
    logger->step++;
    fprintf(logger->file,
        "STEP %d\n"
        "  Operation:     REDUCE\n"
        "  Production:    %d (%s)\n"
        "  Popped:        %d\n"
        "  GotoState:     %d\n"
        "  Stack:         %s\n\n",
        logger->step, prod_id,
        prod_str ? prod_str : "?",
        popped, goto_state,
        stack_str ? stack_str : "");
}

void logger_log_accept(Logger *logger, const char *stack_str)
{
    if (!logger || !logger->file) return;
    logger->step++;
    fprintf(logger->file,
        "STEP %d\n"
        "  Operation:     ACCEPT\n"
        "  Stack:         %s\n\n",
        logger->step,
        stack_str ? stack_str : "");
}

void logger_log_error(Logger *logger, int state,
                      const char *token_str, const char *stack_str)
{
    if (!logger || !logger->file) return;
    logger->step++;
    fprintf(logger->file,
        "STEP %d\n"
        "  Operation:     ERROR\n"
        "  State:         %d\n"
        "  Token:         %s\n"
        "  Stack:         %s\n\n",
        logger->step, state,
        token_str ? token_str : "?",
        stack_str ? stack_str : "");
}

void logger_close(Logger *logger)
{
    if (!logger) return;
    if (logger->file) fclose(logger->file);
    free(logger);
}

char *logger_string(const Logger *logger, int level)
{
    if (!logger) {
        return NULL;
    }
    char *s = NULL;
    int n = level + 1;
    jsonify_wrap(&s, level, 1, "{");
    jsonify(&s, n, 1, 1, "file", ptr_to_str(logger->file));
    jsonify(&s, n, 0, 1, "step", int_to_str(logger->step));
    jsonify_wrap(&s, level, 0, "}");
    return s;
}
