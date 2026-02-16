/**
 * Char Map Module 
 * 
 * @description: 
 *      A char map has static number of elements, defined by ASCII_SIZE. Each 
 *      position of the char map represents a character, and each value in each
 *      position represents an assigned integer to the character. By default,
 *      each character is assigned to KEY_ERROR.
 * 
 * @author: Marc Bosch Manzano
 * @creation: 2026/02/08
 */

#ifndef CHAR_MAP_H
#define CHAR_MAP_H

#define ASCII_SIZE 128
#define KEY_ERROR (-1)

char *empty_char_map(void);
char *init_char_map(const char *string, const char *category);
void print_char_map(const char *map);

#endif