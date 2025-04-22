#ifndef ISR_H
#define ISR_H

#include "../core/util.h"
#include "idt.h"
#include "../core/system.h"

#define NUM_INTERRUPT_SERVICE_ROUTINES 48

typedef struct Registers {
    u32 ds;                  // Data segment selector
    u32 edi, esi, ebp, esp, ebx, edx, ecx, eax; // General purpose registers
    u32 int_no, err_code;    // Interrupt number and error code (if any)
    u32 eip, cs, eflags, useresp, ss; // Saved segment registers
} Registers;

// Forward declarations of assembly ISR stubs
extern void _isr0(Registers*);
extern void _isr1(Registers*);
extern void _isr2(Registers*);
extern void _isr3(Registers*);
extern void _isr4(Registers*);
extern void _isr5(Registers*);
extern void _isr6(Registers*);
extern void _isr7(Registers*);
extern void _isr8(Registers*);
extern void _isr9(Registers*);
extern void _isr10(Registers*);
extern void _isr11(Registers*);
extern void _isr12(Registers*);
extern void _isr13(Registers*);
extern void _isr14(Registers*);
extern void _isr15(Registers*);
extern void _isr16(Registers*);
extern void _isr17(Registers*);
extern void _isr18(Registers*);
extern void _isr19(Registers*);
extern void _isr20(Registers*);
extern void _isr21(Registers*);
extern void _isr22(Registers*);
extern void _isr23(Registers*);
extern void _isr24(Registers*);
extern void _isr25(Registers*);
extern void _isr26(Registers*);
extern void _isr27(Registers*);
extern void _isr28(Registers*);
extern void _isr29(Registers*);
extern void _isr30(Registers*);
extern void _isr31(Registers*);
extern void _isr32(Registers*);
extern void _isr33(Registers*);
extern void _isr34(Registers*);
extern void _isr35(Registers*);
extern void _isr36(Registers*);
extern void _isr37(Registers*);
extern void _isr38(Registers*);
extern void _isr39(Registers*);
extern void _isr40(Registers*);
extern void _isr41(Registers*);
extern void _isr42(Registers*);
extern void _isr43(Registers*);
extern void _isr44(Registers*);
extern void _isr45(Registers*);
extern void _isr46(Registers*);
extern void _isr47(Registers*);

// Array of pointers to the assembly ISR stubs (definition)
void (*isr_stubs[NUM_INTERRUPT_SERVICE_ROUTINES])(Registers*) = {
    _isr0,  _isr1,  _isr2,  _isr3,  _isr4,  _isr5,  _isr6,  _isr7,
    _isr8,  _isr9,  _isr10, _isr11, _isr12, _isr13, _isr14, _isr15,
    _isr16, _isr17, _isr18, _isr19, _isr20, _isr21, _isr22, _isr23,
    _isr24, _isr25, _isr26, _isr27, _isr28, _isr29, _isr30, _isr31,
    _isr32, _isr33, _isr34, _isr35, _isr36, _isr37, _isr38, _isr39,
    _isr40, _isr41, _isr42, _isr43, _isr44, _isr45, _isr46, _isr47
};

// Descriptions of the first 32 CPU exceptions (definition)
const char *exception_messages[32] = {
    "Divide-by-zero Error",
    "Debug Exception",
    "Non-Maskable Interrupt",
    "Breakpoint Exception",
    "Overflow Exception",
    "BOUND Range Exceeded Exception",
    "Invalid Opcode Exception",
    "Device Not Available Exception",
    "Double Fault",
    "Coprocessor Segment Overrun (legacy)",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Fault Exception",
    "General Protection Fault",
    "Page Fault",
    "Reserved (Vector 15)",
    "x87 FPU Floating-Point Error",
    "Alignment Check Exception",
    "Machine-Check Exception",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Reserved", "Reserved", "Reserved",
    "Reserved", "Reserved", "Reserved", "Reserved",
    "Reserved", "Reserved", "Reserved", "Reserved"
};

// Structure to hold ISR information
typedef struct {
    size_t vector_index;
    void (*handler_stub)(Registers*);
} isr_entry_t;

// Array of ISR entries (definition)
isr_entry_t interrupt_service_routines[NUM_INTERRUPT_SERVICE_ROUTINES];

// Array of registered handlers for each ISR (definition)
void (*interrupt_handlers[NUM_INTERRUPT_SERVICE_ROUTINES])(Registers*) = { 0 };

// Registers a handler function for a specific interrupt vector (implementation)
void isr_register_handler(size_t interrupt_vector, void (*handler)(Registers*)) {
    interrupt_handlers[interrupt_vector] = handler;
}

// Common ISR handler called from assembly (implementation)
void isr_handler(Registers *regs) {
    if (interrupt_handlers[regs->int_no]) {
        interrupt_handlers[regs->int_no](regs);
    }
}

// Default handler for CPU exceptions (implementation)
void exception_handler(Registers *regs) {
    panic(exception_messages[regs->int_no]);
}

// Initializes the Interrupt Service Routines (implementation)
void isr_init() {
    for (size_t i = 0; i < NUM_INTERRUPT_SERVICE_ROUTINES; i++) {
        interrupt_service_routines[i].vector_index = i;
        interrupt_service_routines[i].handler_stub = isr_stubs[i];
        idt_set(interrupt_service_routines[i].vector_index, interrupt_service_routines[i].handler_stub, 0x08, 0x8E);
    }

    // Install the default exception handler for the first 32 ISRs (CPU exceptions)
    for (size_t i = 0; i < 32; i++) {
        isr_register_handler(i, exception_handler);
    }
}

#endif
