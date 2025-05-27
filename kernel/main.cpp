extern "C" {
#include "ui/auro.h"
#include "core/interrupts.h"
#include "ui/system/screen.h"
#include "core/util.h"
#include "mem/vmm.h"
}


    
extern "C" void _main(u32 magic) {
    idt_init();
    isr_init();
    fpu_init(); // floating point unit.
    irq_init();
    imap_kernel();
    init_vmm();
    screen_init();


    panic("TEST");
}
