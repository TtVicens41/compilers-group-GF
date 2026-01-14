#ifndef ERRORS_HANDLING_H
#define ERRORS_HANDLING_H

#define NOT_EXISTS_ERROR "File not exists error\n"
#define OPEN_FAILURE "Failure to open %s\n"
#define FILE_NOT_PROVIDED "No input file name provided\n"

typedef struct {
    char message[128];
    int line;
} Errors;

static Errors errors;

#endif
