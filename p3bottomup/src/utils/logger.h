/**
 * @file logger.h
 * @brief Structured debug logger for the shift/reduce parser.
 *
 * Writes one block per parsing step to a file, producing human-readable
 * traces that document every SHIFT, REDUCE, ACCEPT, and ERROR action.
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

typedef struct {
    FILE *file;
    int step;
} Logger;

/** Open a log file and return a Logger. Returns NULL on failure. */
Logger *logger_init(const char *output_path);

/**
 * Log a SHIFT action.
 * @param input_pos   Current position in the token stream.
 * @param prev_state  State before the shift.
 * @param new_state   State after the shift.
 * @param stack_str   Formatted stack snapshot (caller provides).
 */
void logger_log_shift(Logger *logger, int input_pos,
                      int prev_state, int new_state,
                      const char *stack_str);

/**
 * Log a REDUCE action.
 * @param prod_id     Production id used for the reduction.
 * @param prod_str    Human-readable production string.
 * @param popped      Number of symbols popped.
 * @param goto_state  New state pushed after the reduction.
 * @param stack_str   Formatted stack snapshot.
 */
void logger_log_reduce(Logger *logger, int prod_id,
                       const char *prod_str, int popped,
                       int goto_state, const char *stack_str);

/**
 * Log an ACCEPT action.
 * @param stack_str  Final stack snapshot.
 */
void logger_log_accept(Logger *logger, const char *stack_str);

/**
 * Log an ERROR action.
 * @param state       State where the error occurred.
 * @param token_str   String representation of the offending token.
 * @param stack_str   Stack snapshot at the point of error.
 */
void logger_log_error(Logger *logger, int state,
                      const char *token_str, const char *stack_str);

/** Flush and close the log file. */
void logger_close(Logger *logger);

#endif /* LOGGER_H */
