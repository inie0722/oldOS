#include "fat12.h"
#include "bios.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

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

    return NULL;
}

void fat12_read_file(BPB *bpb, uint8_t *fat, DIR_entry *entry, void *buf)
{
    uint16_t FstClus = entry->DIR_FstClus;

    while (FstClus < 0xff8)
    {
        //读取一个簇
        uint32_t LBA = bpb->data_sector + (FstClus - 2) * bpb->BPB_SecPerClus;
        bios_read_disk(LBA, bpb->BPB_SecPerClus, buf);
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
    bios_read_disk(bpb->root_sector, bpb->data_sector - bpb->root_sector, buf);
}

void fat12_get_fat(BPB *bpb, void *buf)
{
    bios_read_disk(bpb->fat_sector, bpb->BPB_FATSz16, buf);
}
