#pragma once

#include <stddef.h>

typedef struct
{
    char *addr;
    unsigned long attribute;
    unsigned long reference_count;
    unsigned long age;
} Page;

extern void init_memory();

extern void *allock(size_t size);
