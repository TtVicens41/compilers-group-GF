/**
 * @title: string_list.c
 * @authors: Alejandro Poole
 * @creation: 16/02/2026
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "string_list.h"
#include "string_utils.h"

StringList *init_string_list(char **buffer, int size) {
    StringList *string_list = malloc(sizeof(StringList));
    if (!string_list) { return NULL; }

    string_list->buffer = buffer;
    string_list->size = size;

    return string_list;
}

#define UNSET_TOKEN (-1)

int is_token_set(int token_start) {
    return token_start != UNSET_TOKEN;
}

void reset_token(int *token_start) {
    *token_start = UNSET_TOKEN;
}

int is_split_condition(char c, char splitter) {
    return c == splitter || c == '\0';
}

StringList *string_split(const char *string, char splitter) {
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

    return init_string_list(buffer, size);
}

StringList *apply_string_list(
    StringList *string_list, 
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
        delete_string_list(&string_list);
    }
    return init_string_list(buffer, size);
}

StringList *filter_string_list(
    StringList *string_list,
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
        delete_string_list(&string_list);
    }
    return init_string_list(buffer, size);
}


void clear_string_list(StringList *string_list) {
    for (int i = 0; i < string_list->size; i++) {
        free(string_list->buffer[i]);
        string_list->buffer[i] = NULL;
    }
    free(string_list->buffer);
    string_list->buffer = NULL;
    string_list->size = 0;
}

void delete_string_list(StringList **string_list) {
    clear_string_list(*string_list);
    free(*string_list);
    *string_list = NULL;
}

void print_string_list(const StringList *string_list, int raw) {
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

int *to_integer_array(StringList *string_list, int delete) {
    if (!string_list) { 
        return NULL;
    }
    const int size = string_list->size;
    int *array = calloc(size, sizeof(int));
    for (int i = 0; i < size; i++) {
        array[i] = atoi(string_list->buffer[i]);
    }
    if (delete) {
        delete_string_list(&string_list);
    }
    return array;
}
