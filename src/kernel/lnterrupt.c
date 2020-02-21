#include "lnterrupt.h"

#include "bios.h"
#include "info.h"

#include <stddef.h>
#include <stdint.h>

#define TRAP 0x8F00
#define INTR 0x8E00

#define SYSTEM_TRAP 0xEF00
#define SYSTEM_INTR 0xEE00

static void set_idt(size_t index, uint16_t type, void *fun)
{
    uint16_t low[4] = {0};
    uint16_t high[4] = {0};

    low[0] = ((uint16_t *)&fun)[0];
    //段偏移
    low[1] = 0x8;
    //属性
    low[2] = type;
    low[3] = ((uint16_t *)&fun)[1];

    high[0] = ((uint16_t *)&fun)[2];
    high[1] = ((uint16_t *)&fun)[3];
    high[2] = 0;
    high[3] = 0;

    info.IDT[index * 2] = *(uint64_t *)low;
    info.IDT[index * 2 + 1] = *(uint64_t *)high;
}

void __divide_error()
{
    bios_message("Trigger a divide_error interrupt\n", ERROR);
    bios_exit();
}

void __debug()
{
    bios_message("Trigger a debug interrupt\n", ERROR);
    bios_exit();
}

void __nmi()
{
    bios_message("Trigger a nmi interrupt\n", ERROR);
    bios_exit();
}

void __int3()
{
    bios_message("Trigger a int3 interrupt\n", ERROR);
    bios_exit();
}

void __overflow()
{
    bios_message("Trigger a overflow interrupt\n", ERROR);
    bios_exit();
}

void __bounds()
{
    bios_message("Trigger a bounds interrupt\n", ERROR);
    bios_exit();
}

void __undefined_opcode()
{
    bios_message("Trigger a undefined_opcode interrupt\n", ERROR);
    bios_exit();
}

void __dev_not_available()
{
    bios_message("Trigger a dev_not_available interrupt\n", ERROR);
    bios_exit();
}

void __double_fault()
{
    bios_message("Trigger a double_fault interrupt\n", ERROR);
    bios_exit();
}

void __coprocessor_segment_overrun()
{
    bios_message("Trigger a coprocessor_segment_overrun interrupt\n", ERROR);
    bios_exit();
}

void __invalid_tss()
{
    bios_message("Trigger a invalid_tss interrupt\n", ERROR);
    bios_exit();
}

void __segment_not_present()
{
    bios_message("Trigger a segment_not_present interrupt\n", ERROR);
    bios_exit();
}

void __stack_segment_fault()
{
    bios_message("Trigger a stack_segment_fault interrupt\n", ERROR);
    bios_exit();
}

void __general_protection()
{
    bios_message("Trigger a general_protection interrupt\n", ERROR);
    bios_exit();
}

void __page_fault()
{
    bios_message("Trigger a page_fault interrupt\n", ERROR);
    bios_exit();
}

void __x87_fpu_error()
{
    bios_message("Trigger a x87_fpu_error interrupt\n", ERROR);
    bios_exit();
}

void __alignment_check()
{
    bios_message("Trigger a alignment_check interrupt\n", ERROR);
    bios_exit();
}

void __machine_check()
{
    bios_message("Trigger a machine_check interrupt\n", ERROR);
    bios_exit();
}

void __simd_exception()
{
    bios_message("Trigger a simd_exception interrupt\n", ERROR);
    bios_exit();
}

void __virtualization_exception()
{
    bios_message("Trigger a virtualization_exception interrupt\n", ERROR);
    bios_exit();
}

extern void divide_error();

extern void debug();

extern void nmi();

extern void int3();

extern void overflow();

extern void bounds();

extern void undefined_opcode();

extern void dev_not_available();

extern void double_fault();

extern void coprocessor_segment_overrun();

extern void invalid_tss();

extern void segment_not_present();

extern void stack_segment_fault();

extern void general_protection();

extern void page_fault();

extern void x87_fpu_error();

extern void alignment_check();

extern void machine_check();

extern void simd_exception();

extern void virtualization_exception();

void init_idt()
{
    set_idt(0, TRAP, divide_error);

    set_idt(1, TRAP, debug);

    set_idt(2, TRAP, nmi);

    set_idt(3, TRAP, int3);

    set_idt(4, TRAP, overflow);

    set_idt(5, TRAP, bounds);

    set_idt(6, TRAP, undefined_opcode);

    set_idt(7, TRAP, dev_not_available);

    set_idt(8, TRAP, double_fault);

    set_idt(9, TRAP, coprocessor_segment_overrun);

    set_idt(10, TRAP, invalid_tss);

    set_idt(11, TRAP, segment_not_present);

    set_idt(12, TRAP, stack_segment_fault);

    set_idt(13, TRAP, general_protection);

    set_idt(14, TRAP, page_fault);

    //15号中断向量 Intel 保留
    set_idt(16, TRAP, x87_fpu_error);

    set_idt(17, TRAP, alignment_check);

    set_idt(18, TRAP, machine_check);

    set_idt(19, TRAP, simd_exception);

    set_idt(20, TRAP, virtualization_exception);
}