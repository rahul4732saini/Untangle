#ifndef _STRUCTS_H
#define _STRUCTS_H

#include "typedefs.h"

typedef struct
{
    const char **strings;
    len_t size;
} Strings;

typedef struct
{
    double real;
    double imag;
} Number;

typedef struct
{
    dtype_t type;
    void *data;
} ResultStep;

typedef struct
{
    ResultStep *steps;
    len_t size;
} Result;

typedef struct
{
    const char *name;
    const dtype_t *args;
    len_t args_size;
    const Strings *prompts;
    Result *(*handler)(void *, ...);
} Problem;

#endif
