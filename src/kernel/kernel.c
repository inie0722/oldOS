#include <stddef.h>
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
    init_page();

    while (1)
        ;
}
