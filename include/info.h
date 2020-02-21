#pragma once

#include <stdint.h>

typedef struct
{
    volatile uint64_t *PML4E;
    volatile uint64_t *PDPTE;
    volatile uint64_t *PDE;
    volatile uint64_t *GDT;
    volatile uint64_t *IDT;
    volatile uint64_t *TSS;
} Info;

extern Info info;