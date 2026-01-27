/**
 * @title: Argumennts Menu
 * @brief: Declarations of arguments parsing utilities.
 * @authors: Marc Bosch Manzano
 * @creation: 10/01/2026
 */

#ifndef ARGUMENTS_MENU_H
#define ARGUMENTS_MENU_H

#define MANUAL_PAGE "%s/.local/share/preprocessor/man_page.txt"

typedef struct {
    char comment;
    char directive;
    char help;
    int file_position;
} Flags;

Flags process_arguments(int argc, char *argv[]);
void execute_arguments(int argc, char *argv[]);

#endif