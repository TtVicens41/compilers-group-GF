/**
 * @title: main.c
 * @authors:
 * @creation:
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "token.h"
#include "utils/file_utils.h"

/**
 * @brief Explica la responsabilidad de `print_usage` en el flujo del compilador.
 * @param Recibe: `const char *argv0`.
 * @return No devuelve valor.
 * @details Ejecuta una tarea concreta para mantener el codigo modular y facilitar mantenimiento.
 */
static void print_usage(const char *argv0) {
    fprintf(stderr, "Usage: %s <input_file.c>\n", argv0);
    fprintf(stderr, "       %s -help\n", argv0);
}

/**
 * @brief Explica la responsabilidad de `print_manual` en el flujo del compilador.
 * @param No recibe parametros.
 * @return No devuelve valor.
 * @details Ejecuta una tarea concreta para mantener el codigo modular y facilitar mantenimiento.
 */
static void print_manual(void) {
    const char *candidates[] = {
        "./src/man_page.txt",
        "p2lx/src/man_page.txt",
        NULL
    };
    int i = 0;

    while (candidates[i]) {
        FILE *probe = fopen(candidates[i], "r");
        if (probe) {
            fclose(probe);
            print_file(candidates[i]);
            return;
        }
        i++;
    }

    fprintf(stderr, "Man page not found.\n");
}

/**
 * @brief Explica la responsabilidad de `main` en el flujo del compilador.
 * @param Recibe: `int argc, char **argv`.
 * @return Devuelve un valor de tipo `int`.
 * @details Ejecuta una tarea concreta para mantener el codigo modular y facilitar mantenimiento.
 */
int main(int argc, char **argv) {
    TokenList tokens;
    char *output_path = NULL;
    int status;

    if (argc == 2 && strcmp(argv[1], "-help") == 0) {
        print_manual();
        return 0;
    }

    if (argc != 2) {
        print_usage(argv[0]);
        return 1;
    }

    token_list_init(&tokens);

    status = run_pipeline_with_optional_parser(argv[1], NULL, &tokens, &output_path);

    if (status == 0) {
        fprintf(stdout, "Scanner finished: %s\n", output_path ? output_path : "(no output)");
        fprintf(stdout, "Tokens generated in memory: %zu\n", tokens.size);
    }

    free(output_path);
    token_list_free(&tokens);
    return status;
}
