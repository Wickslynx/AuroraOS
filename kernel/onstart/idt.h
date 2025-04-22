#ifndef IDT_H
#define IDT_H

#include "../core/util.h"
#include "isr.h"

#define IDT_ENTRIES 256

// Structure for an IDT entry
typedef struct IDTEntry {
    u16 offset_low;   //
    u16 selector;     // Kernel code segment selector
    u8 zero;          // Must be zero
    u8 type_attr;     // Type and attributes (present, privilege, type)
    u16 offset_high;  // Upper 16 bits of the handler address
} PACKED IDTEntry;

// Structure for the IDT pointer
typedef struct IDTPointer {
    u16 limit;        // Size of the IDT table - 1
    uintptr_t base;   // Base address of the IDT table
} PACKED IDTPointer;

// Global IDT structure
static struct {
    IDTEntry entries[IDT_ENTRIES]; // Array of IDT entries
    IDTPointer pointer;             // IDT pointer
} idt;

// Assembly function to load the IDT
extern void idt_load(IDTPointer* idt_ptr);

// Sets an entry in the IDT
void idt_set(u8 index, void (*handler)(struct Registers*), u16 selector, u8 flags) {
    idt.entries[index].offset_low = ((uintptr_t) handler) & 0xFFFF;
    idt.entries[index].offset_high = (((uintptr_t) handler) >> 16) & 0xFFFF;
    idt.entries[index].selector = selector;
    idt.entries[index].type_attr = flags | 0x60; // Present bit (0x80) and Descriptor Privilege Level (0x00) are often combined here, Gate Type (0x0E for interrupt gate)
    idt.entries[index].zero = 0;
}

// Initializes the IDT
void idt_init() {
    idt.pointer.limit = sizeof(idt.entries) - 1;
    idt.pointer.base = (uintptr_t) &idt.entries[0];
    memset(&idt.entries[0], 0, sizeof(idt.entries)); // Initialize all IDT entries to zero
    idt_load(&idt.pointer);                           // Load the IDT
}

#endif