/**
 * @file string_array.c
 * @brief String Array Utilities
 * @author Marc Bosch Manzano
 * @since 2026-02-09
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "./string_array.h"
#include "./string_utils.h"

#define UNSET_TOKEN (-1)

StringArray *init_string_array(char **buffer, int size) {
    StringArray *string_list = malloc(sizeof(StringArray));
    if (!string_list) { return NULL; }

    string_list->buffer = buffer;
    string_list->size = size;

    return string_list;
}

int is_token_set(int token_start) {
    return token_start != UNSET_TOKEN;
}

void reset_token(int *token_start) {
    *token_start = UNSET_TOKEN;
}

int is_split_condition(char c, char splitter) {
    return c == splitter || c == '\0';
}

StringArray *string_split(const char *string, char splitter) {
    int length = strlen(string);
    
    char **buffer = NULL;
    int size = 0;   
    int token_start = UNSET_TOKEN;

    for (int i = 0; i <= length; i++) {
        if (!is_split_condition(string[i], splitter)) {
            if (!is_token_set(token_start)) {
                token_start = i; 
            }
        } else {
            if (is_token_set(token_start)) {
                buffer = realloc(buffer, (size + 1) * sizeof(char *));
                buffer[size++] = get_substring(string, token_start, i - 1);
                reset_token(&token_start);
            }
        }
    }

    return init_string_array(buffer, size);
}

StringArray *apply_string_array(
    StringArray *string_list, 
    char *(*applier)(const char *),
    int delete
) {
    if (!string_list) { 
        return NULL; 
    }
    int size = string_list->size;
    char **buffer = calloc(size, sizeof(char *));
    for (int i = 0; i < size; i++) {
        buffer[i] = applier(string_list->buffer[i]);
    }
    if (delete) {
        delete_string_array(&string_list);
    }
    return init_string_array(buffer, size);
}

StringArray *filter_string_list(
    StringArray *string_list,
    int (*condition)(const char *),
    int delete
) {    
    if (!string_list) { 
        return NULL; 
    }
    int size = 0;
    char **buffer = NULL;
    for (int i = 0; i < string_list->size; i++) {
        if (condition(string_list->buffer[i])) {
            buffer = realloc(buffer, (size + 1) * sizeof(char *));
            buffer[size++] = get_copy(string_list->buffer[i]);
        }
    }
    if (delete) {
        delete_string_array(&string_list);
    }
    return init_string_array(buffer, size);
}


void clear_string_array(StringArray *string_list) {
    for (int i = 0; i < string_list->size; i++) {
        free(string_list->buffer[i]);
        string_list->buffer[i] = NULL;
    }
    free(string_list->buffer);
    string_list->buffer = NULL;
    string_list->size = 0;
}

void delete_string_array(StringArray **string_list) {
    clear_string_array(*string_list);
    free(*string_list);
    *string_list = NULL;
}

void print_string_array(const StringArray *string_list, int raw) {
    if (!string_list) { return; }
    if (!string_list->buffer) { return; }

    printf("[\n");
    for (int i = 0; i < string_list->size; i++) {
        if (raw) {
            printf("\"%s\"", get_raw_string(string_list->buffer[i]));
        } else {
            printf("%s", string_list->buffer[i]);
        }
        if (i < (string_list->size - 1)) {
            printf(",\n");
        }
    }
    printf("\n]\n");
}

int *to_integer_array(StringArray *string_list, int delete) {
    if (!string_list) { 
        return NULL;
    }
    const int size = string_list->size;
    int *array = calloc(size, sizeof(int));
    for (int i = 0; i < size; i++) {
        array[i] = atoi(string_list->buffer[i]);
    }
    if (delete) {
        delete_string_array(&string_list);
    }
    return array;
}
