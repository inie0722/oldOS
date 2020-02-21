.section .text
.globl _start
_start:
	//设置IDT
	lidt IDT_ptr
	
	//设置GDT
	lgdt GDT_ptr

	mov $0x10, %ax
	mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

	mov $0x7e00, %rsp

	//跳转到内核
	leaq main, %rax
	pushq $0x08	
	pushq %rax
	lretq


.globl info
info:
.quad PML4E
.quad PDPTE
.quad PDE

.quad GDT
.quad IDT
.quad TSS

.org 0x1000
PML4E:
	.fill 512, 8, 0

.org 0x2000
PDPTE:
	.fill 512, 8, 0

.org 0x3000
PDE:
	.fill 512, 8, 0

GDT:
	.quad 0x0
	.quad 0x0020980000000000 #64位 内核 代码段 
	.quad 0x0000920000000000 #64位 内核 数据段
	.quad 0x0020f80000000000 #64位 用户 代码段
	.quad 0x0000f20000000000 #64位 用户 数据段
GDT_ptr:
	.word . - GDT - 1
	.quad GDT
IDT:
	.fill 512, 8, 0
IDT_ptr:
	.word . - IDT - 1
	.quad IDT
TSS:
	.fill 512, 8, 0
