#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stddef.h>
#include <stdint.h>

// Common types
typedef uint8_t u8;
typedef uint16_t u16;
typedef uintptr_t uintptr_t;

// Macros for atomic operations
#define CLI() asm volatile("cli")
#define STI() asm volatile("sti")
#define PACKED __attribute__((packed))

// Port I/O functions
static inline u8 inportb(u16 port) {
    u8 ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outportb(u16 port, u8 val) {
    asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

// Registers structure for interrupt handlers
struct Registers {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
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
