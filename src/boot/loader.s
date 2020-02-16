.section .loader, "ax"
.code16
.globl loader

loader:
//准备切换到x86_32
    //开启a20
    mov $0x2401, %ax
    int $0x15

    //关闭中断
    cli
    //设置gdt
    lgdt GDT_ptr32

    //切换到x86_32
    movl %cr0, %eax
    orl  $1, %eax
    movl %eax, %cr0

    //使用远跳转指令设置CS段寄存器
    ljmp $0x8, $x86_32

.code32
x86_32:
    //设置段寄存器
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

//准备切换到x86_64
    //设置临时页面录
    movl $0x91007, (0x90000)
    movl $0x91007, (0x90800)

    movl $0x92007, (0x91000)	
    movl $0x000083, (0x92000)	
    movl $0x200083, (0x92008)
    movl $0x400083, (0x92010)	
    movl $0x600083, (0x92018)	
    movl $0x800083, (0x92020)
    movl $0xa00083, (0x92028)

    //设置gdt
    lgdt GDT_ptr64

    //开启物理地址扩展功能
	mov %cr4, %eax
	bts $0x5, %eax
	mov %eax, %cr4

    //设置临时页目录
	mov $0x90000, %eax
	mov %eax, %cr3

    //切换到x86_64 兼容模式
	mov	$0x0C0000080, %ecx
	rdmsr

	bts	$8, %eax
	wrmsr
    
    //开启分页机制
    mov	%cr0, %eax
	bts	$0, %eax
	bts	$31, %eax
	mov	%eax, %cr0

    //跳转到x86_64 长模式
    ljmp $0x8, $x86_64

.code64
x86_64:
    //设置段寄存器
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    
    //读取内核
    callq fat12_read_kernel
    jmp *%rax



GDT_32:
//gdt 第一个表必须为空
.word 0b0000000000000000
.word 0b0000000000000000
.byte 0b00000000
.byte 0b00000000
.byte 0b00000000
.byte 0b00000000

GDT_code_32:
.word 0b1111111111111111  # Limit:[0, 16)
.word 0b0000000000000000  # Base:[0, 16)
.byte 0b00000000          # Base:[16, 24)
.byte 0b10011010          # (Pr)1 (Privl)00 1 (Ex)1 (DC)0 (RW)1 (Ac)0
.byte 0b11001111          # (Gr)1 (Sz)1 (L)0 0 Limit:[16,20)
.byte 0b00000000          # Base:[24, 32)

GDT_data_32:
.word 0b1111111111111111  # Limit:[0, 16)
.word 0b0000000000000000  # Base:[0, 16)
.byte 0b00000000          # Base:[16, 24)
.byte 0b10010010          # (Pr)1 (Privl)00 1 (Ex)0 (DC)0 (RW)1 (Ac)0
.byte 0b11001111          # (Gr)1 (Sz)1 (L)0 0 Limit:[16,20)
.byte 0b00000000          # Base:[24, 32)

GDT_ptr32:
.word . - GDT_32 - 1
.long GDT_32

GDT_64:
.quad 0x0000000000000000

GDT_code_64:
.quad 0x0020980000000000

GDT_data_64:
.quad 0x0000920000000000

GDT_ptr64:
.word . - GDT_64 - 1
.long GDT_64
