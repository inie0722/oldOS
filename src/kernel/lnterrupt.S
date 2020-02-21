#define ENTRY(name) \
    .globl name;    \
    name##:         \
    pushq %rdi;     \
	pushq %rsi;     \
    movq 16(%rsp), %rdi;\
	leaq __##name, %rsi;\
	jmp push_all;

push_all:
    pushq %rax
    pushq %rbx
    pushq %rcx
    pushq %rdx
	pushq %rbp

    pushq %r8
    pushq %r9
    pushq %r10
    pushq %r11
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15

    movq %ds, %rax
    pushq %rax
    movq %es, %rax
    pushq %rax
    movq %fs, %rax
    pushq %rax
    movq %gs, %rax
    pushq %rax
    movq %ss, %rax
    pushq %rax
	
	call *%rsi

	jmp pop_all

pop_all:
    popq %rax
    movq %rax, %ss
    popq %rax
    movq %rax, %gs
    popq %rax
    movq %rax, %fs
    popq %rax
    movq %rax, %es
    popq %rax
    movq %rax, %ds

    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %r11
    popq %r10
    popq %r9
    popq %r8
	
	popq %rbp
    popq %rdx
    popq %rcx
    popq %rbx
    popq %rax

	popq %rsi
	popq %rdi
	lretq


ENTRY(divide_error)

ENTRY(debug)

ENTRY(nmi)

ENTRY(int3)

ENTRY(overflow)

ENTRY(bounds)

ENTRY(undefined_opcode)

ENTRY(dev_not_available)

ENTRY(double_fault)

ENTRY(coprocessor_segment_overrun)

ENTRY(invalid_tss)

ENTRY(segment_not_present)

ENTRY(stack_segment_fault)

ENTRY(general_protection)

ENTRY(page_fault)

ENTRY(x87_fpu_error)

ENTRY(alignment_check)

ENTRY(machine_check)

ENTRY(simd_exception)

ENTRY(virtualization_exception)
