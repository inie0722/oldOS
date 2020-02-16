#include "bios.h"

#include <stddef.h>
#include <stdint.h>

void bios_clear()
{
    asm(
        "mov $0x0600, %%ax\t\n"
        "mov $0x0700, %%bx\t\n"
        "mov $0x0, %%cx\t\n"
        "mov $0x0184f, %%dx\t\n"
        "int $0x10\t\n"
        :
        :
        :);
}

static uint16_t bios_strlen(const char *str)
{
    uint16_t i = 0;
    for (; *str; i++)
    {
        str++;
    }
    return i;
}

void bios_puts(const char *str)
{
    static uint16_t dh = 0;
    uint16_t dx = dh << 8;
    uint16_t cx = bios_strlen(str) + 1;

    asm(
        "push %%bp\n\t"
        "mov %%ax, %%bp\n\t"
        "mov $0x0, %%bh\n\t"
        "mov $0x0f, %%bl\n\t"
        "mov $0x13, %%ah\n\t"
        "mov $0x1, %%al\n\t"
        "int $0x10\n\t"
        "pop %%bp\n\t"
        :
        : "a"(str),
          "c"(cx),
          "d"(dx)
        :);
    ++dh;
}

void bios_read_disk(uint8_t driver, uint8_t head, uint8_t cylinder, uint8_t sector, uint8_t count, void *buf)
{
    uint16_t dx = ((uint16_t)head << 8) + driver;
    uint16_t ax = ((uint16_t)0x2 << 8) + count;
    uint16_t cx = ((uint16_t)cylinder << 8) + sector;

    asm(
        "int $0x13\n\t"
        :
        : "a"(ax),
          "b"(buf),
          "c"(cx),
          "d"(dx)
        :);
}

/*
void Test_bios_read_disk(uint8_t driver, uint8_t head, uint8_t cylinder, uint8_t sector, uint8_t count, void *buf)
{
    uint16_t dx = ((uint16_t)head << 8) + driver;
    uint16_t ax = ((uint16_t)0x2 << 8) + count;
    uint16_t cx = ((uint16_t)cylinder << 8) + sector;

    asm(
        "T: jmp T\n\t"
        "int $0x13\n\t"
        :
        : "a"(ax),
          "b"(buf),
          "c"(cx),
          "d"(dx)
        :);
}
*/
void bios_exit()
{
    while (1)
        ;
}
