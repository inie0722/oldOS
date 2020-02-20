#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define RED 0xc
#define WHITE 0xf

extern void bios_clear();

extern void bios_puts(const char *str, uint8_t colour, bool flicker);

extern void bios_read_disk(uint32_t LBA, uint8_t count, void *buf);

extern void bios_exit();

extern void *bios_read_kernel();