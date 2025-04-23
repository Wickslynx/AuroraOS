#ifndef IDT_H
#define IDT_H

#include "../core/util.h"
#include "isr.h"

#define IDT_ENTRIES 256

typedef struct IDTEntry {
    u16 offset_low;   // offset (low)
    u16 selector;     // segment selector
    u8 zero;          // 0 :skull:
    u8 type_attr;     // 
    u16 offset_high;  // offset (high, upper 16 bits)
} PACKED IDTEntry;

/
typedef struct IDTPointer {
    u16 limit;        
    uintptr_t base;   
} PACKED IDTPointer;


static struct {
    IDTEntry entries[IDT_ENTRIES];
    IDTPointer pointer;             
} idt;

// declared in start.S
extern void idt_load(IDTPointer* idt_ptr);


void idt_set(u8 index, void (*handler)(struct Registers*), u16 selector, u8 flags) {
    idt.entries[index].offset_low = ((uintptr_t) handler) & 0xFFFF;
    idt.entries[index].offset_high = (((uintptr_t) handler) >> 16) & 0xFFFF;
    idt.entries[index].selector = selector;
    idt.entries[index].type_attr = flags | 0x60;
    idt.entries[index].zero = 0;
}

// --- Init IDT ---
void idt_init() {
    if (!idt) {
        print("Something unexpected happened, restarting.");
        return;
    }

    idt.pointer.limit = sizeof(idt.entries) - 1;
    idt.pointer.base = (uintptr_t) &idt.entries[0];
    memset(&idt.entries[0], 0, sizeof(idt.entries)); // all entrys to 0
    idt_load(&idt.pointer);                        
}

#endif