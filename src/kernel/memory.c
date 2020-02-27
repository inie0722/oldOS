#include <stddef.h>

#include "info.h"
#include "printk.h"
#include "memory.h"

#pragma pack(1)
typedef struct
{
    char *addr;
    uint64_t length;
    uint32_t type;
} E820;
#pragma pack()

typedef struct
{
    Page *page_array;
    unsigned long pages_length;

    char *begin_addr;
    char *end_addr;

    unsigned long attribute;

    unsigned long page_using_count;
    unsigned long page_free_count;
} Zone;

typedef struct
{
    E820 *e820;
    unsigned long e820_length;

    Zone *zone_array;
    unsigned long zones_length;
} Memory_Descriptor;

static inline unsigned long align_up(unsigned long bytes, size_t size)
{
    return (bytes + size) - (bytes % size);
}

static inline unsigned long align_down(unsigned long bytes, size_t size)
{
    return bytes - (bytes % size);
}

static void init_page()
{

    info.PML4E[0] = (uint64_t)info.PDPTE + 0x7;
    info.PDPTE[0] = (uint64_t)info.PDE + 0x3;

    //前10m线性地址 = 物理地址 存放内核代码 和 中断栈
    size_t index = 0;
    for (size_t i = 0; i < 5; i++)
    {
        info.PDE[index] = i * 0x200000 + 0x83;
        ++index;
    }

    //帧缓存区 = 线性地址0xa00000
    for (size_t i = 0; i < 8; i++)
    {
        info.PDE[index] = 0xe0000000 + i * 0x200000 + 0x83;
        ++index;
    }

    //30m内存 用于内核堆 0x1a00000
    for (size_t i = 0; i < 15; i++)
    {
        info.PDE[index] = 0xa00000 + i * 0x200000 + 0x83;
        ++index;
    }

    asm volatile(
        "mov %%rax, %%cr3\n\t"
        :
        : "a"(info.PML4E)
        :);
}

static char *_end = (char *)0x1a00000;

void *allock(size_t size)
{
    if (!size)
    {
        return NULL;
    }

    void *ret = (char *)align_up((unsigned long)_end, size);
    _end = (char *)ret + size;
    return ret;
}

#define PAGE_4K_SIZE 4096

static Memory_Descriptor mem_descriptor;

void init_memory()
{
    init_page();
    mem_descriptor.e820 = allock(sizeof(E820));

    //设置E820
    E820 *p = (E820 *)0x7000;
    mem_descriptor.e820_length = 0;
    mem_descriptor.zones_length = 0;
    for (size_t i = 0; p->length != 0 && p->type < 4; i++)
    {
        mem_descriptor.e820[i].addr = p->addr;
        mem_descriptor.e820[i].length = p->length;
        mem_descriptor.e820[i].type = p->type;

        if (p->type == 1)
        {
            ++mem_descriptor.zones_length;
        }

        ++mem_descriptor.e820_length;
        ++p;
    }

    allock(sizeof(E820) * (mem_descriptor.e820_length - 1));
    mem_descriptor.zone_array = (Zone *)allock(sizeof(Zone) * mem_descriptor.zones_length);

    //设置区域和页属性
    unsigned long TotalMem = 0;
    size_t z = 0;
    for (size_t i = 0; i < mem_descriptor.e820_length; i++)
    {
        if (mem_descriptor.e820[i].type != 1)
            continue;

        char *volatile begin = (char *)align_up((unsigned long)mem_descriptor.e820[i].addr, PAGE_4K_SIZE);
        char *volatile end = (char *)align_down((unsigned long)mem_descriptor.e820[i].addr + mem_descriptor.e820[i].length, 1024 * 4);

        if (end - begin)
        {
            TotalMem += (end - begin) / PAGE_4K_SIZE;

            Zone *zone = &mem_descriptor.zone_array[z];
            zone->begin_addr = begin;
            zone->end_addr = end;
            zone->pages_length = (end - begin) / PAGE_4K_SIZE;
            zone->page_free_count = (end - begin) / PAGE_4K_SIZE;
            zone->page_using_count = 0;
            zone->attribute = 0;
            zone->page_array = allock(zone->pages_length * sizeof(Page));

            for (size_t j = 0; j < zone->pages_length; j++)
            {
                zone->page_array[j].addr = zone->begin_addr + j * PAGE_4K_SIZE;
                zone->page_array[j].age = 0;
                zone->page_array[j].attribute = 0;
                zone->page_array[j].reference_count = 0;
            }
            z++;
        }
    }

    //设置内核表属性
    char *addr = (char *)0;
    for (size_t i = 0; i < 20; i++)
    {
        volatile size_t j = 0;
        for (; j < mem_descriptor.zones_length; j++)
        {
            if (mem_descriptor.zone_array[j].begin_addr <= addr && mem_descriptor.zone_array[j].end_addr > addr)
                break;
        }

        addr += PAGE_4K_SIZE;
        if (j == mem_descriptor.zones_length)
            continue;

        mem_descriptor.zone_array[j].page_using_count++;
        mem_descriptor.zone_array[j].page_free_count--;

        size_t index = (addr - mem_descriptor.zone_array[j].begin_addr) / PAGE_4K_SIZE;
        mem_descriptor.zone_array[j].page_array[index].reference_count += 1;
    }
}

Page *alloc_pages()
{
    for (size_t i = 0; i < mem_descriptor.zones_length; i++)
    {
        if (mem_descriptor.zone_array[i].page_free_count > 0)
        {
            for (size_t j = 0; j < mem_descriptor.zone_array[i].pages_length; j++)
            {
                if (mem_descriptor.zone_array[i].page_array[j].reference_count == 0)
                {
                    mem_descriptor.zone_array[i].page_free_count--;
                    mem_descriptor.zone_array[i].page_using_count++;
                    mem_descriptor.zone_array[i].page_array[j].reference_count++;
                    return &mem_descriptor.zone_array[i].page_array[j];
                }
            }
        }
    }
    return NULL;
}