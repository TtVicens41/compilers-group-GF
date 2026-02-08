/**
 * @title: Hash Map Module 
 * @description: The hash map has static number of elements. The hash
 * @author: Marc Bosch Manzano
 * @creation: 2026/02/08
 */

#ifndef HASH_MAP_H
#define HASH_MAP_H

#define ASCII_SIZE 128
#define KEY_ERROR (-1)

typedef struct {
    int alphabet_size;
    char *alphabet;
    char map[ASCII_SIZE];
} HashMap;

/**
 * Removes repeated characters on a string, getting its unique
 * characters.
 */
char *get_keys(const char *string);

/**
 * Allocate a hash map of SIZE bytes per element.
 */
char *empty_char_map(void);
char *init_char_map(const char *string);

// void update(HashMap *map, void *key, void *value);
// void *get(HashMap *map, void *key);
// void delete(HashMap *map);

#endif