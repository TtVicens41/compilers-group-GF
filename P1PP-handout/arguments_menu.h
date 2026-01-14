#ifndef ARGUMENTS_MENU_H
#define ARGUMENTS_MENU_H

#define MANUAL_PAGE "man_page.txt"

typedef struct {
    char comment;
    char directive;
    char all;
    char help;
    int file_position;
} Flags;

Flags process_arguments(int argc, char *argv[]);
void execute_arguments(int argc, char *argv[]);

#endif