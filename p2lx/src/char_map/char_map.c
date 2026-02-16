/**
 * Char Map Module
 * 
 * @author: Marc Bosch Manzano
 * @creation: 2026/02/08
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
    
char *init_char_map(const char *string, const char *category) {
    char *map = empty_char_map();
    if (!map) { return NULL; }

    size_t length = strlen(string);
    char value = 0;
    for (size_t i = 0; i < length; i++) {
        char key = string[i];
        if (map[key] == KEY_ERROR) {
            map[key] = value++;
        }
    }

    if (strcmp(category, category_text[CAT_LITERAL]) == 0) {
        for (size_t key = 0; key < ASCII_SIZE; key++) {
            if (map[key] == KEY_ERROR) {
                map[key] = value;
            }
        }
    }

    return map;
}

void print_char_map(const char *map) {
    if (!map) { return; }

    printf("[\n");
    for (size_t i = 0; i < ASCII_SIZE; i++) {
        printf("%c %d\n", (char)i, map[i]);
    }
    printf("]\n");
}

#if (0)
int main(void) {
    char *char_map = init_char_map("if");
    printf("%d\n", char_map['i']);
    printf("%d\n", char_map['f']);
    printf("%d\n", char_map['2']);
}
#endif