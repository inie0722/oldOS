#pragma once

#include <stdint.h>
#include <stdbool.h>

#pragma pack(1)

//引导分区结构
typedef struct
{
    uint8_t jmp[3];          //跳转指令
    char OEM_name[8];        //生产厂商
    uint16_t BPB_BytsPerSec; //每扇区字节数
    uint8_t BPB_SecPerClus;  //每簇扇区数
    uint16_t BPB_RsvdSecCnt; //保留扇区数
    uint8_t BPB_NumFATs;     //FAT表 个数
    uint16_t BPB_RootEntCnt; //根目录最大文件数
    uint16_t BPB_TotSec16;   //总扇区数
    uint8_t BPB_Media;       //存储介质
    uint16_t BPB_FATSz16;    //FAT扇区数
    uint16_t BPB_SecPerTrk;  //磁道扇区数
    uint16_t BPB_NumHeads;   //磁头数
    uint32_t BPB_HiddSec;    //隐藏扇区数
    uint32_t BPB_TotSec32;   //如果BPB_FATSz16为0，该值为FAT扇区数
    uint8_t BS_DrvNum;       //int 0x13 的驱动号
    uint8_t BS_Reserved1;    //未使用
    uint8_t BS_BootSig;      //扩展引导标记(0x29)
    uint32_t BS_VolID;       //卷序列号
    char BS_VolLab[11];      //卷标
    char BS_FileSysType[8];  //文件系统类型
    uint32_t fat_sector;     //fat 开始扇区
    uint32_t root_sector;    //更目录 开始扇区
    uint32_t data_sector;    //data 开始扇区
} BPB;

//目录项
typedef struct
{
    char DIR_Name[11];     //文件名
    uint8_t DIR_Attr;      //文件属性
    char reserved[10];     //保留
    uint16_t DIR_WrtTime;  //最后一次写入时间
    uint16_t DIR_WrtDate;  //最后一次写入日期
    uint16_t DIR_FstClus;  //起始簇号
    uint32_t DIR_FileSize; //文件大小
} DIR_entry;

#pragma pack()

DIR_entry *fat12_search_file(BPB *bpb, DIR_entry *entry, char *DIR_name);

void fat12_read_file(BPB *bpb, uint8_t *fat, DIR_entry *entry, void *buf);

void fat12_get_root(BPB *bpb, void *buf);

void fat12_get_fat(BPB *bpb, void *buf);

void fat12_read_disk(uint32_t LBA, uint8_t count, void *buf);

void *fat12_read_kernel();