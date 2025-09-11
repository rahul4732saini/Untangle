#ifndef _STRUCTS_H
#define _STRUCTS_H

#include "typedefs.h"

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
    char *name;
    dtype_t *args;
    len_t args_size;
    char **prompts;
    len_t prompts_size;
    Result *(*handler)(void *, ...);
} Problem;

typedef struct
{
    char *name;
    Problem **problems;
    len_t size;
} Field;

typedef struct
{
    char *name;
    Field **fields;
    len_t size;
} Domain;

typedef struct
{
    Domain **domains;
    len_t size;
} Domains;

#endif
