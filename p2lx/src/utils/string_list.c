/**
 * @title: string_list.c
 * @authors:
 * @creation:
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "string_list.h"
#include "string_utils.h"

StringList *init_string_list(char **buffer, int size) {
    StringList *string_list = malloc(sizeof(StringList));
    if (!string_list) {
        return NULL;
    }
    string_list->buffer = buffer;
    string_list->size = size;
    return string_list;
}

#define UNSET_TOKEN (-1)
        return init_string_list(NULL, 0);
    }

    length = (int)strlen(string);

    for (i = 0; i <= length; i++) {
        if (!is_split_condition(string[i], splitter)) {
            if (!is_token_set(token_start)) {
                token_start = i;
            }
        } else if (is_token_set(token_start)) {
            char **grown = realloc(buffer, (size + 1) * sizeof(char *));
            if (!grown) {
                free(buffer);
                return NULL;
            }
            buffer = grown;
            buffer[size++] = get_substring(string, token_start, i - 1);
            reset_token(&token_start);
        }
    }
    return init_string_list(buffer, size);
}

StringList *apply_string_list(
    StringList *string_list,
    char *(*applier)(const char *),
    int delete
) {
    char **buffer;
    int size;
    int i;

    if (!string_list) {
        return NULL;
    }

    buffer = calloc(string_list->size, sizeof(char *));
    if (!buffer) {
        return NULL;
    }

    size = string_list->size;
    for (i = 0; i < size; i++) {
        buffer[i] = applier(string_list->buffer[i]);
    }

    if (delete) {
        delete_string_list(&string_list);
    }
    return init_string_list(buffer, size);
}

/**
 * @brief Explica la responsabilidad de `clear_string_list` en el flujo del compilador.
 * @param Recibe: `StringList *string_list`.
 * @return No devuelve valor.
 * @details Ejecuta una tarea concreta para mantener el codigo modular y facilitar mantenimiento.
 */
void clear_string_list(StringList *string_list) {
    int i;
    if (!string_list) {
        return;
    }

    for (i = 0; i < string_list->size; i++) {
        free(string_list->buffer[i]);
        string_list->buffer[i] = NULL;
    }

    free(string_list->buffer);
    string_list->buffer = NULL;
    string_list->size = 0;
}

/**
 * @brief Explica la responsabilidad de `delete_string_list` en el flujo del compilador.
 * @param Recibe: `StringList **string_list`.
 * @return No devuelve valor.
 * @details Ejecuta una tarea concreta para mantener el codigo modular y facilitar mantenimiento.
 */
void delete_string_list(StringList **string_list) {
    if (!string_list || !*string_list) {
        return;
    }
    clear_string_list(*string_list);
    free(*string_list);
    *string_list = NULL;
}

/**
 * @brief Explica la responsabilidad de `print_string_list` en el flujo del compilador.
 * @param Recibe: `StringList *string_list, int raw`.
 * @return No devuelve valor.
 * @details Ejecuta una tarea concreta para mantener el codigo modular y facilitar mantenimiento.
 */
void print_string_list(StringList *string_list, int raw) {
    int i;
    if (!string_list || !string_list->buffer) {
        return;
    }

    printf("[\n");
    for (i = 0; i < string_list->size; i++) {
        if (raw) {
            char *rendered = get_raw_string(string_list->buffer[i]);
            printf("\"%s\"", rendered ? rendered : "");
            free(rendered);
        } else {
            printf("%s", string_list->buffer[i]);
        }

        if (i < string_list->size - 1) {
            printf(",\n");
        }
    }
    printf("\n]\n");
}

int *to_integer_array(StringList *string_list, int delete) {
    int *array;
    int i;

    if (!string_list) {
        return NULL;
    }

    array = calloc(string_list->size, sizeof(int));
    if (!array) {
        return NULL;
    }

    for (i = 0; i < string_list->size; i++) {
        array[i] = atoi(string_list->buffer[i]);
    }

    if (delete) {
        delete_string_list(&string_list);
    }

    return array;
}
