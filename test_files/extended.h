/*
 * Header File: extended.h
 * Extended header that includes base.h
 */

#ifndef EXTENDED_H
#define EXTENDED_H

#include "base.h"

#define EXTENDED_CONSTANT 200
#define EXTENDED_NAME "ExtendedModule"

typedef struct {
    BaseType base;
    int extra_field;
} ExtendedType;

#endif
