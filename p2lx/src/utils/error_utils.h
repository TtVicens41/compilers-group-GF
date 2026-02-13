/**
 * @title: error_utils.h
 * @authors:
 * @creation:
 */

#ifndef ERRORS_HANDLING_H
#define ERRORS_HANDLING_H

#include <stdio.h>

#include "error.h"

/**
 * @brief Explica la responsabilidad de `print_file_error` en el flujo del compilador.
 * @param Recibe: `const char *path`.
 * @return No devuelve valor.
 * @details Ejecuta una tarea concreta para mantener el codigo modular y facilitar mantenimiento.
 */
void print_file_error(const char *path);
void report_lexer_error(FILE *stream, ErrorCode code, int line, int column,
                        const char *lexeme, const char *extra);

#endif
