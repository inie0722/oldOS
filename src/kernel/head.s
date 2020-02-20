.section .text
.globl _start
_start:

jmp main


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
	.fill 512, 8, 0
IDT:
	.fill 512, 8, 0
TSS:
	.fill 512, 8, 0
