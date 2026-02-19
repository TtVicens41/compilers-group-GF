/**
 * @file char_map.c
 * @brief Char Map Module
 * @author Marc Bosch Manzano
 * @since 2026-02-08
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "./char_map.h"
#include "../scanner/token.h"
#include "../utils/utils.h"

char *empty_char_map(void) {
    char *map = calloc(ASCII_SIZE, sizeof(char));
    if (!map) { return NULL; }
    
    for (size_t key = 0; key < ASCII_SIZE; key++) {
        map[key] = KEY_ERROR;
    }

    return map;
}

void set_character(char *map, const char *value, char character) {
    unsigned char key = (unsigned char)character;
    if (map[key] == KEY_ERROR) {
        map[key] = *value;
    }
}

void set_range(char *map, const char *value, int start, int end) {
    if (!map) { 
        return; 
    }
    start = max(min(start, end), 0);
    end = min(max(start, end), ASCII_SIZE - 1);
    for (int key = start; key <= end; key++) {
        if (map[key] == KEY_ERROR)  {
            map[key] = *value;
        }
    }
}

void set_characters(char *map, char *value, const char *characters) {
    if (!map || !value || !characters) { 
        return; 
    }
    size_t length = strlen(characters);
    for (size_t i = 0; i < length; i++) {
        unsigned char key = (unsigned char)characters[i];
        if (map[key] == KEY_ERROR) {
            map[key] = (*value)++;
        }
    }
    (*value)--;
}

void set_regular_expression(char *map, char *value, const char *regular_expression) {
    if (strcmp(regular_expression, NUMBERS) == 0) {
        set_range(map, value, '0', '9'); 
    } 
    else if (strcmp(regular_expression, LETTERS) == 0) {
        set_range(map, value, 'A', 'Z');
        set_range(map, value, 'a', 'z');
    } 
    else if (strcmp(regular_expression, NUMBERS_LETTERS) == 0) {
        set_range(map, value, '0', '9');
        set_range(map, value, 'A', 'Z');
        set_range(map, value, 'a', 'z');
    }
    else if (strcmp(regular_expression, UPPER_CASE) == 0) {
        set_range(map, value, 'A', 'Z');
    }
    else if (strcmp(regular_expression, LOWER_CASE) == 0) {
        set_range(map, value, 'a', 'z');
    }
    else if (strcmp(regular_expression, OTHER) == 0) {
        set_range(map, value, 0, ASCII_SIZE - 1);
    }
    else if (strcmp(regular_expression, SPACE) == 0) {
        set_character(map, value, ' ');
    }
    else {
        set_characters(map, value, regular_expression);
    }
}

char *init_char_map(const char *alphabet) {
    char *map = empty_char_map();
    if (!map || !alphabet) { 
        return NULL; 
    }

    StringArray *regular_expressions;
    regular_expressions = string_split(alphabet, SYMBOL_SEPARATION);
    char value = 0;

    for (int i = 0; i < regular_expressions->size; i++) {
        char *regular_expression = regular_expressions->buffer[i];
        set_regular_expression(map, &value, regular_expression);
        value++;
    }
    return map;
}

int compute_num_assigned_chars(const char *map) {
    int count = 0;
    for (size_t key = 0; key < ASCII_SIZE; key++) {
        count += map[key] != KEY_ERROR;
    }
    return count;
}

void print_char_map(const char *map) {
    if (!map) { return; }
    
    printf("[\n");
    for (size_t i = 0; i < ASCII_SIZE; i++) {
        printf("%s %d\n", get_raw_char((char)i), map[(char)i]);
    }
    printf("]\n");
}

#if (0)
int main(void) {
    char *char_map = init_char_map("[a-z] [0-9] {} |");
    print_char_map(char_map);
}
#endif
