#include "fat12.h"
#include "bios.h"

//boot 起始地址
extern void _start();

#define ORG 0x100000
#define KERNEL_NAME "KERNEL  BIN"

void *loader_kernel()
{
    bios_clear();

    BPB *bpb = (BPB *)_start;

    DIR_entry *root = (DIR_entry *)0x1000;
    fat12_get_root(bpb, root);

    void *fat = (void *)0x3000;
    fat12_get_fat(bpb, fat);

    DIR_entry *kernel = fat12_search_file(bpb, root, KERNEL_NAME);
    if (kernel)
        fat12_read_file(bpb, fat, kernel, (void *)ORG);
    else
    {
        bios_message("no file found kernel.bin\n", ERROR);
        bios_exit();
    }

    return (void *)ORG;
}