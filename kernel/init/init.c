#include "irq.h"
#include "fpu.h"
#include "isr.h"
#include "idt.h"


void kernel_init() {
    idt_init();
    isr_init();
    fpu_init();
    irq_init();
}