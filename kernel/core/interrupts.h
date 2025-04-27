#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "../core/util.h"

struct Registers {
    u32 ds;
    u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
    u32 int_no, err_code;
    u32 eip, cs, eflags, useresp, ss;
};

// FPU functions
void fpu_init();

// IDT functions
void idt_init();
void idt_set(u8 index, void (*base)(struct Registers*), u16 selector, u8 flags);
void idt_load(uintptr_t ptr);

// IRQ functions
void irq_init();
void irq_install(size_t i, void (*handler)(struct Registers*));

// ISR functions
void isr_init();
void isr_install(size_t i, void (*handler)(struct Registers*));
void isr_handler(struct Registers *regs);

#endif /* INTERRUPTS_H */
