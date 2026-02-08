/**
 * @title: Hash Map module
 * @author: Marc Bosch Manzano
 * @creation: 2026/02/08
 */

#ifndef HASH_MAP_H
#define HASH_MAP_H

typedef struct {
    int *array;
    int size;
} HashMap;

HashMap *init_hash_map(void);
void update_hash_map(void *ptr);

#endif