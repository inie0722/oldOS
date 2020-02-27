.section .begin, "ax"
.code16
.globl  _start

.equ org, 0x7c00

_start:
//fat12 bpb
    jmp boot
    nop
    .string  "InOS   "      #生产厂商 长度8字节 包括结束符
    .word   512             #每扇区字节数
    .byte   1               #每簇扇区数
    .word   8               #保留扇区数
    .byte   2               #FAT表 个数
    .word   224             #根目录最大文件数
    .word   2880            #总扇区数
    .byte   0xf0            #存储介质
    .word   9               #FAT扇区数
    .word   18              #磁道扇区数
    .word   2               #磁头数
    .long   0               #隐藏扇区数
    .long   0               #如果BPB_FATSz16为0，该值为FAT扇区数
    .byte   0               #int 0x13 的驱动号
    .byte   0               #未使用
    .byte   0x29            #扩展引导标记(0x29)
    .long   0x0             #卷序列号
    .string  "InOS      "   #卷标 长度11字节 包括结束符
    .string  "FAT12  "      #文件系统类型 长度8字节 包括结束符

//附加信息
    .long 9                 #fat表起始扇区
    .long 9 + 9 * 2         #root目录起始扇区
    .long (9 + 9 * 2) + ((224 * 32 + 512 - 1) / 512) #数据 起始扇区

boot:
//将loader fat12 加载到内存中
    
//设置读取扇区数量
    mov $0x1f2, %dx
    mov $0x08, %al
    out %al, %dx

//设置读取扇区起始地址
    inc %dx
    mov $0x2, %al
    out %al, %dx
    
    inc %dx
    mov $0x0, %al
    out %al, %dx

    inc %dx
    mov $0x0, %al
    out %al, %dx
    
    inc %dx
    mov $0x0, %al
    out %al, %dx

    //设置读取功能
    inc %dx
    mov $0x20, %al
    out %al, %dx
    
    //等待硬盘准备好数据
.waits:
    in %dx, %al

    and $0x88, %al
    cmp $0x08, %al
    jnz .waits
    
    //设置buf地址
    mov $org + 512, %bx
    //读取数量
    mov $512 * 8, %cx
    //开始读取
    mov $0x1f0, %dx
    .readw:
        in %dx, %ax
        mov %ax, (%bx)
        add $2, %bx
        dec %cx
        loop .readw

//跳转到loader
    jmp loader


.section .end, "a"
    .byte 0x55
    .byte 0xaa
