#include "../include/interrupts.h"
#include <log.h>
#include <error.h>

//=====================================================================
//                               FPU  
//=====================================================================

void fpu_init() {
    LOG_INFO(" core/interrupts.c: Setting up FPU.. \n");
    size_t t;
    asm("clts");
    asm("mov %%cr0, %0" : "=r"(t));
    t &= ~(1 << 2);
    t |= (1 << 1);
    asm("mov %0, %%cr0" :: "r"(t));
    asm("mov %%cr4, %0" : "=r"(t));
    t |= 3 << 9;
    asm("mov %0, %%cr4" :: "r"(t));
    asm("fninit");
}

//=====================================================================
//                             IDT
//=====================================================================

struct IDTEntry {
    u16 offset_low;
    u16 selector;
    u8 __ignored;
    u8 type;
    u16 offset_high;
} PACKED;

struct IDTPointer {
    u16 limit;
    uintptr_t base;
} PACKED;

static struct {
    struct IDTEntry entries[256];
    struct IDTPointer pointer;
} idt;

void idt_set(u8 index, void (*base)(struct Registers*), u16 selector, u8 flags) {
    idt.entries[index] = (struct IDTEntry) {
        .offset_low = ((uintptr_t) base) & 0xFFFF,
        .offset_high = (((uintptr_t) base) >> 16) & 0xFFFF,
        .selector = selector,
        .type = flags | 0x60,
        .__ignored = 0
    };
}

void idt_init() {
    LOG_INFO(" core/interrupts.c: Setting up IDT.. \n");
    idt.pointer.limit = sizeof(idt.entries) - 1;
    idt.pointer.base = (uintptr_t) &idt.entries[0];
    memset(&idt.entries[0], 0, sizeof(idt.entries));
    idt_load((uintptr_t) &idt.pointer);
}

//=====================================================================
//                          ISR
//=====================================================================

#define NUM_ISRS 48

extern void _isr0(struct Registers*);
extern void _isr1(struct Registers*);
extern void _isr2(struct Registers*);
extern void _isr3(struct Registers*);
extern void _isr4(struct Registers*);
extern void _isr5(struct Registers*);
extern void _isr6(struct Registers*);
extern void _isr7(struct Registers*);
extern void _isr8(struct Registers*);
extern void _isr9(struct Registers*);
extern void _isr10(struct Registers*);
extern void _isr11(struct Registers*);
extern void _isr12(struct Registers*);
extern void _isr13(struct Registers*);
extern void _isr14(struct Registers*);
extern void _isr15(struct Registers*);
extern void _isr16(struct Registers*);
extern void _isr17(struct Registers*);
extern void _isr18(struct Registers*);
extern void _isr19(struct Registers*);
extern void _isr20(struct Registers*);
extern void _isr21(struct Registers*);
extern void _isr22(struct Registers*);
extern void _isr23(struct Registers*);
extern void _isr24(struct Registers*);
extern void _isr25(struct Registers*);
extern void _isr26(struct Registers*);
extern void _isr27(struct Registers*);
extern void _isr28(struct Registers*);
extern void _isr29(struct Registers*);
extern void _isr30(struct Registers*);
extern void _isr31(struct Registers*);
extern void _isr32(struct Registers*);
extern void _isr33(struct Registers*);
extern void _isr34(struct Registers*);
extern void _isr35(struct Registers*);
extern void _isr36(struct Registers*);
extern void _isr37(struct Registers*);
extern void _isr38(struct Registers*);
extern void _isr39(struct Registers*);
extern void _isr40(struct Registers*);
extern void _isr41(struct Registers*);
extern void _isr42(struct Registers*);
extern void _isr43(struct Registers*);
extern void _isr44(struct Registers*);
extern void _isr45(struct Registers*);
extern void _isr46(struct Registers*);
extern void _isr47(struct Registers*);

static void (*stubs[NUM_ISRS])(struct Registers*) = {
    _isr0, _isr1, _isr2, _isr3, _isr4, _isr5, _isr6, _isr7,
    _isr8, _isr9, _isr10, _isr11, _isr12, _isr13, _isr14, _isr15,
    _isr16, _isr17, _isr18, _isr19, _isr20, _isr21, _isr22, _isr23,
    _isr24, _isr25, _isr26, _isr27, _isr28, _isr29, _isr30, _isr31,
    _isr32, _isr33, _isr34, _isr35, _isr36, _isr37, _isr38, _isr39,
    _isr40, _isr41, _isr42, _isr43, _isr44, _isr45, _isr46, _isr47
};

static const char *exceptions[32] = {
    "Divide by zero", "Debug", "NMI", "Breakpoint",
    "Overflow", "OOB", "Invalid opcode", "No coprocessor",
    "Double fault", "Coprocessor segment overrun", "Bad TSS", "Segment not present",
    "Stack fault", "General protection fault", "Page fault", "Unrecognized interrupt",
    "Coprocessor fault", "Alignment check", "Machine check", "RESERVED",
    "RESERVED", "RESERVED", "RESERVED", "RESERVED",
    "RESERVED", "RESERVED", "RESERVED", "RESERVED",
    "RESERVED", "RESERVED", "RESERVED", "RESERVED"
};

static struct {
    size_t index;
    void (*stub)(struct Registers*);
} isrs[NUM_ISRS];

static void (*isr_handlers[NUM_ISRS])(struct Registers*) = { 0 };


static void exception_handler(struct Registers *regs) {
    panic(exceptions[regs->int_no]);
}

void isr_install(size_t i, void (*handler)(struct Registers*)) {
    isr_handlers[i] = handler;
}

// Called from assembly
void isr_handler(struct Registers *regs) {
    if (isr_handlers[regs->int_no]) {
        isr_handlers[regs->int_no](regs);
    }
}

void isr_init() {
    LOG_INFO(" core/interrupts.c: Setting up ISR.. \n");
    for (size_t i = 0; i < NUM_ISRS; i++) {
        isrs[i].index = i;
        isrs[i].stub = stubs[i];
        idt_set(isrs[i].index, isrs[i].stub, 0x08, 0x8E);
    }
    
    for (size_t i = 0; i < 32; i++) {
        isr_install(i, exception_handler);
    }
}

//=====================================================================
//                             IRQ
//=====================================================================

// PIC constants
#define PIC1 0x20
#define PIC1_OFFSET 0x20
#define PIC1_DATA (PIC1 + 1)
#define PIC2 0xA0
#define PIC2_OFFSET 0x28
#define PIC2_DATA (PIC2 + 1)
#define PIC_EOI 0x20
#define PIC_MODE_8086 0x01
#define ICW1_ICW4 0x01
#define ICW1_INIT 0x10

#define PIC_WAIT() do {         \
    asm ("jmp 1f\n\t"           \
         "1:\n\t"               \
         "    jmp 2f\n\t"       \
         "2:");                 \
} while (0)

static void (*irq_handlers[32])(struct Registers *regs) = { 0 };

static void irq_stub(struct Registers *regs) {
    if (regs->int_no <= 47 && regs->int_no >= 32) {
        if (irq_handlers[regs->int_no - 32]) {
            irq_handlers[regs->int_no - 32](regs);
        }
    }
    
    // Send EOI
    if (regs->int_no >= 0x40) {
        outportb(PIC2, PIC_EOI);
    }
    outportb(PIC1, PIC_EOI);
}

static void irq_remap() {
    u8 mask1 = inportb(PIC1_DATA), mask2 = inportb(PIC2_DATA);
    
    outportb(PIC1, ICW1_INIT | ICW1_ICW4);
    outportb(PIC2, ICW1_INIT | ICW1_ICW4);
    outportb(PIC1_DATA, PIC1_OFFSET);
    outportb(PIC2_DATA, PIC2_OFFSET);
    outportb(PIC1_DATA, 0x04); // PIC2 at IRQ2
    outportb(PIC2_DATA, 0x02); // Cascade identity
    outportb(PIC1_DATA, PIC_MODE_8086);
    outportb(PIC2_DATA, PIC_MODE_8086);
    outportb(PIC1_DATA, mask1);
    outportb(PIC2_DATA, mask2);
}

static void irq_set_mask(size_t i) {
    u16 port = i < 8 ? PIC1_DATA : PIC2_DATA;
    u8 value = inportb(port) | (1 << (i % 8));
    outportb(port, value);
}

static void irq_clear_mask(size_t i) {
    u16 port = i < 8 ? PIC1_DATA : PIC2_DATA;
    u8 value = inportb(port) & ~(1 << (i % 8));
    outportb(port, value);
}

void irq_install(size_t i, void (*handler)(struct Registers *)) {
    CLI();
    if (i = 1) {
        LOG_INFO(" core/interrupts.c: Installed keyboard drivers..\n");
    }
    irq_handlers[i] = handler;
    irq_clear_mask(i);
    STI();
}

void irq_init() {
    LOG_INFO(" core/interrupts.c: Setting up IRQ.. \n");
    irq_remap();
    for (size_t i = 0; i < 16; i++) {
        isr_install(32 + i, irq_stub);
    }
}

