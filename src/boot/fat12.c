#include "fat12.h"
#include "bios.h"

#include <stdint.h>
#include <stdbool.h>

void fat12_read_disk(uint32_t LBA, uint8_t count, void *buf)
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

static bool cmp_file_name(char *name_1, char *name_2)
{
    for (size_t i = 0; i < 11; i++)
    {
        if (name_1[i] != name_2[i])
        {
            return false;
        }
    }

    return true;
}

DIR_entry *fat12_search_file(BPB *bpb, DIR_entry *entry, char *DIR_name)
{
    for (size_t i = 0; i < bpb->BPB_RootEntCnt; i++)
    {
        //存在起始簇
        if (entry->DIR_FstClus != 0)
        {
            if (cmp_file_name(entry->DIR_Name, DIR_name))
            {
                return entry;
            }
        }
        ++entry;
    }
    bios_exit();
    return NULL;
}

void fat12_read_file(BPB *bpb, uint8_t *fat, DIR_entry *entry, void *buf)
{
    uint16_t FstClus = entry->DIR_FstClus;

    while (FstClus < 0xff8)
    {
        //读取一个簇
        uint32_t LBA = bpb->data_sector + (FstClus - 2) * bpb->BPB_SecPerClus;
        fat12_read_disk(LBA, bpb->BPB_SecPerClus, buf);
        buf = (char *)buf + bpb->BPB_BytsPerSec * bpb->BPB_SecPerClus;
        //寻找下一个簇
        uint16_t offset = FstClus / 2 + FstClus;
        if (FstClus % 2)
        {
            FstClus = fat[offset] >> 4;
            FstClus += fat[offset + 1] << 4;
        }
        //偶数
        else
        {
            FstClus = fat[offset];
            FstClus += (uint16_t)((uint8_t)(fat[offset + 1] << 4)) << 4;
        }
    }
}

void fat12_get_root(BPB *bpb, void *buf)
{
    fat12_read_disk(bpb->root_sector, bpb->data_sector - bpb->root_sector, buf);
}

void fat12_get_fat(BPB *bpb, void *buf)
{
    fat12_read_disk(bpb->fat_sector, bpb->BPB_FATSz16, buf);
}

//boot 起始地址
extern void _start();

#define ORG 0x100000
#define KERNEL_NAME "KERNEL  BIN"

//禁止优化入口函数
void *fat12_read_kernel()
{
    BPB *bpb = (BPB *)_start;

    DIR_entry *root = (DIR_entry *)0x1000;
    fat12_get_root(bpb, root);

    char kernel_name[] = KERNEL_NAME;
    DIR_entry *kernel = fat12_search_file(bpb, root, kernel_name);

    void *fat = (void *)0x3000;
    fat12_get_fat(bpb, fat);
    fat12_read_file(bpb, fat, kernel, (void *)ORG);

    return (void *)ORG;
}
