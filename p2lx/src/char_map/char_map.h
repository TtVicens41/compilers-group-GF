/**
 * @title: Char Map Module 
 * @description: A char map has static number of elements, defined by
 *      ASCII_SIZE. Each position
 * @author: Marc Bosch Manzano
 * @creation: 2026/02/08
 */

#ifndef HASH_MAP_H
#define HASH_MAP_H

#define ASCII_SIZE 128
#define KEY_ERROR (-1)

/**
 * Allocate a hash map of SIZE bytes per element.
 */
char *empty_char_map(void);
char *init_char_map(const char *string);

// void update(HashMap *map, void *key, void *value);
// void *get(HashMap *map, void *key);
// void delete(HashMap *map);

#endif
