/**
 * @title: char_map.c
 * @authors:
 * @creation:
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "char_map.h"

char *empty_char_map(void) {
    char *map = calloc(ASCII_SIZE, sizeof(char));
    if (!map) { return NULL; }
    
    for (size_t key = 0; key < ASCII_SIZE; key++) {
        map[key] = KEY_ERROR;
    }

    return map;
}
    
char *init_char_map(const char *string) {
    char *map = empty_char_map();
    if (!map) {
        return NULL;
    }

    size_t length = strlen(string);
    char value = 0;
    for (size_t i = 0; i < length; i++) {
        unsigned char key = (unsigned char)string[i];
        if (map[key] == KEY_ERROR) {
            map[key] = value++;
        }
    }

   return map;
}

#if (0)
/**
 * @brief Describes the responsibility of `main` in the compiler pipeline.
 * @param Does not receive parameters.
 * @return Returns a value of type `int`.
 * @details Performs a focused task to keep the code modular and easier to maintain.
 */
int main(void) {
    char *hash_map = init_char_map("if");
    printf("%d\n", hash_map['i']);
    printf("%d\n", hash_map['f']);
    
    printf("%d\n", hash_map['2']);
}
#endif
