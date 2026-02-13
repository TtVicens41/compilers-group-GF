/**
 * @title: file_utils.h
 * @authors:
 * @creation:
 */

#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include "../language_defs.h"

/**
 * @brief Explica la responsabilidad de `print_file` en el flujo del compilador.
 * @param Recibe: `const char *path`.
 * @return No devuelve valor.
 * @details Ejecuta una tarea concreta para mantener el codigo modular y facilitar mantenimiento.
 */
void print_file(const char *path);
char *read_file(const char *path);
/**
 * @brief Explica la responsabilidad de `dump_file` en el flujo del compilador.
 * @param Recibe: `const char *path, char *str, int max_len`.
 * @return No devuelve valor.
 * @details Ejecuta una tarea concreta para mantener el codigo modular y facilitar mantenimiento.
 */
void dump_file(const char *path, char *str, int max_len);
/**
 * @brief Explica la responsabilidad de `copy_file` en el flujo del compilador.
 * @param Recibe: `const char *input_path, const char *output_path`.
 * @return No devuelve valor.
 * @details Ejecuta una tarea concreta para mantener el codigo modular y facilitar mantenimiento.
 */
void copy_file(const char *input_path, const char *output_path);
/**
 * @brief Explica la responsabilidad de `check_input_file` en el flujo del compilador.
 * @param Recibe: `const char *input_path`.
 * @return Devuelve un valor de tipo `int`.
 * @details Ejecuta una tarea concreta para mantener el codigo modular y facilitar mantenimiento.
 */
int check_input_file(const char *input_path);
/**
 * @brief Explica la responsabilidad de `check_output_file` en el flujo del compilador.
 * @param Recibe: `const char *output_path`.
 * @return Devuelve un valor de tipo `int`.
 * @details Ejecuta una tarea concreta para mantener el codigo modular y facilitar mantenimiento.
 */
int check_output_file(const char *output_path);

#endif
