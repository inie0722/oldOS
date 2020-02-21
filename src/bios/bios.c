#include "bios.h"

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define VIDEO_MEM (char *)0xb8000
#define MAX_ROWS 25
#define MAX_COLUMNS 160

static volatile char *video_mem = VIDEO_MEM;

void bios_clear()
{
    for (size_t i = 0; i < MAX_COLUMNS * MAX_ROWS; i++)
    {
        video_mem[0] = ' ';
        video_mem[1] = 0;
        video_mem += 2;
    }
    video_mem = VIDEO_MEM;
}

void bios_puts(const char *str, uint8_t f_color, uint8_t b_color, bool flicker)
{
    char Font = f_color + (b_color << 4) + (flicker << 7);
    while (*str)
    {
        if (*str == '\n')
        {
            ptrdiff_t index = video_mem - VIDEO_MEM;
            video_mem += MAX_COLUMNS - index % MAX_COLUMNS;
            ++str;
            continue;
        }

        video_mem[0] = *str;
        video_mem[1] = Font;
        video_mem += 2;
        ++str;

        if (video_mem > VIDEO_MEM + MAX_COLUMNS * MAX_ROWS)
        {
            video_mem = VIDEO_MEM;
        }
    }
}

const char *Prefix[] = {"[  Ok   ]: ", "[Warning]: ", "[ Error ]: "};
const uint8_t f_color[] = {BRIGHT_GREEN, BRIGHT_YELLOW, BRIGHT_RED};
const bool flicker[] = {false, false, true};

void bios_message(const char *mes, int level)
{
    bios_puts(Prefix[level], f_color[level], DARK_BLACK, flicker[level]);
    bios_puts(mes, f_color[level], DARK_BLACK, flicker[level]);
}

void bios_read_disk(uint32_t LBA, uint8_t count, void *buf)
{
    uint16_t dx = 0x1f2;
    //设置读取数量
    asm volatile(
        "out %%al, %%dx\n\t"
        :
        : "a"(count),
          "d"(dx)
        :);

    //设置起始扇区
    for (size_t i = 0; i < 4; i++)
    {
        ++dx;
        asm volatile(
            "out %%al, %%dx\n\t"
            :
            : "a"(LBA >> i * 8),
              "d"(dx)
            :);
    }

    //设置功能号
    ++dx;
    asm volatile(
        "out %%al, %%dx\n\t"
        :
        : "a"(0x20),
          "d"(dx)
        :);

    //等待硬盘读写操作完成
    uint8_t al;
    do
    {
        asm volatile(
            "in %%dx, %%al"
            : "=a"(al)
            : "d"(dx)
            :);
        al &= 0x88;
    } while (al != 0x8);

    //开始读取数据
    dx = 0x1f0;
    char *rbx = (char *)buf;
    for (int i = count * 512; i > 0; i -= 2)
    {
        asm volatile(
            "in %%dx, %%ax\n\t"
            "mov %%ax, (%%rbx)\n\t"
            :
            : "b"(rbx),
              "d"(dx)
            : "ax");
        rbx += 2;
    }
}

void bios_exit()
{
    bios_message("BIOS has exited\n", ERROR);
    while (1)
        ;
}
