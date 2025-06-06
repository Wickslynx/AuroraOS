.code32
.section .text.prologue

.global _start
_start:
    movl $stack, %esp
    andl $-16, %esp

    movl %esp, %eax

    cli
    call _main

    /* we should never end up here, if we do just return. */
    hlt

.section .text
.align 4

.global idt_load
.type idt_load, @function
idt_load:
    mov 4(%esp), %eax
    lidt (%eax)
    ret

.macro ISR_NO_ERR index
    .global _isr\index
    _isr\index:
        cli
        push $0
        push $\index
        jmp isr_common
.endm

.macro ISR_ERR index
    .global _isr\index
    _isr\index:
        cli
        push $\index
        jmp isr_common
.endm

/* all ISR's */
ISR_NO_ERR 0
ISR_NO_ERR 1
ISR_NO_ERR 2
ISR_NO_ERR 3
ISR_NO_ERR 4
ISR_NO_ERR 5
ISR_NO_ERR 6
ISR_NO_ERR 7
ISR_ERR 8
ISR_NO_ERR 9
ISR_ERR 10
ISR_ERR 11
ISR_ERR 12
ISR_ERR 13
ISR_ERR 14
ISR_NO_ERR 15
ISR_NO_ERR 16
ISR_NO_ERR 17
ISR_NO_ERR 18
ISR_NO_ERR 19
ISR_NO_ERR 20
ISR_NO_ERR 21
ISR_NO_ERR 22
ISR_NO_ERR 23
ISR_NO_ERR 24
ISR_NO_ERR 25
ISR_NO_ERR 26
ISR_NO_ERR 27
ISR_NO_ERR 28
ISR_NO_ERR 29
ISR_NO_ERR 30
ISR_NO_ERR 31
ISR_NO_ERR 32
ISR_NO_ERR 33
ISR_NO_ERR 34
ISR_NO_ERR 35
ISR_NO_ERR 36
ISR_NO_ERR 37
ISR_NO_ERR 38
ISR_NO_ERR 39
ISR_NO_ERR 40
ISR_NO_ERR 41
ISR_NO_ERR 42
ISR_NO_ERR 43
ISR_NO_ERR 44
ISR_NO_ERR 45
ISR_NO_ERR 46
ISR_NO_ERR 47

/* defined in interrupts.c */
.extern isr_handler
.type isr_handler, @function

isr_common:
    /* for Registers struct */
    pusha
    push %ds
    push %es
    push %fs
    push %gs

    /* load kernel */
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    cld

    mov %esp, %eax
    push %eax /* pass registers as a parameters */


    call isr_handler
    add $4, %esp

    pop %gs
    pop %fs
    pop %es
    pop %ds

    popa

    add $8, %esp /* remove error code */
    iret

.global enable_paging
enable_paging:
    pushl %ebp
    movl %ebp, %esp /* new stack frame */

    movl 8(%ebp), %eax /* Get first param */
    movl %eax, %cr3

    /* enable paging */
    movl %cr0, %eax
    orl $0x80000000, %eax
    movl %eax, %cr0

    popl %ebp
    ret
    

.section .data
.align 32
stack_begin:
    .fill 0x4000 /* 16 kb */
stack:
