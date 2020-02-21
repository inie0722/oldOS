#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define DARK_BLACK 0x0
#define DARK_NAVY 0X1
#define DARK_GREEN 0X2
#define DARK_BLUE 0x3
#define DARK_RED 0x4
#define DARK_PINK 0x5
#define DARK_YELLOW 0x6
#define DARK_WHITE 0x7

#define BRIGHT_BLACK 0x8
#define BRIGHT_NAVY 0x9
#define BRIGHT_GREEN 0xa
#define BRIGHT_BLUE 0xb
#define BRIGHT_RED 0xc
#define BRIGHT_PINK 0xd
#define BRIGHT_YELLOW 0xe
#define BRIGHT_WHITE 0xf

#define OK 0x0
#define WARNING 0X1
#define ERROR 0X2

extern void bios_clear();

extern void bios_puts(const char *str, uint8_t f_color, uint8_t b_color, bool flicker);

extern void bios_message(const char *mes, int level);

extern void bios_read_disk(uint32_t LBA, uint8_t count, void *buf);

extern void bios_exit();

extern void *bios_read_kernel();