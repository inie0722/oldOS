#include <stddef.h>
#include <stdint.h>

#include "bios.h"
#include "info.h"
#include "lnterrupt.h"

void init_page()
{

    info.PML4E[0] = (uint64_t)info.PDPTE + 0x7;
    info.PDPTE[0] = (uint64_t)info.PDE + 0x3;

    //前10m线性地址 = 物理地址
    for (size_t i = 0; i < 5; i++)
    {
        info.PDE[i] = i * 0x200000 + 0x83;
    }

    //帧缓存区 = 线性地址0xa00000
    for (size_t i = 0; i < 8; i++)
    {
        info.PDE[i + 5] = 0xe0000000 + i * 0x200000 + 0x83;
    }

    asm volatile(
        "mov %%rax, %%cr3\n\t"
        :
        : "a"(info.PML4E)
        :);
}

int main()
{
    bios_clear();
    init_page();
    bios_message("Start InOS successfully\n", OK);

    init_idt();

    int *p = (int *)0xfffffffffff;
    *p = 32;

    while (1)
    {
    };

    return 0;
}
