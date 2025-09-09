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
    const char *name;
    const dtype_t *args;
    len_t args_size;
    const char **prompts;
    len_t prompts_size;
    const Result *(*handler)(void *, ...);
} Problem;

typedef struct
{
    const char *name;
    const Problem **problems;
    len_t problems_size;
} Field;

typedef struct
{
    const char *name;
    const Field **fields;
    len_t fields_size;
} Domain;

typedef struct
{
    Domain **domains;
    len_t domains_size;
} Domains;

#endif
