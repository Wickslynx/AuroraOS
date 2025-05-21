extern "C" {
#include "ui/auro.h"
#include "core/interrupts.h"
#include "ui/system/screen.h"
#include "core/util.h"
}


    
extern "C" void _main(u32 magic) {
    idt_init();
    isr_init();
    fpu_init(); // floating point unit.
    irq_init();

    panic("TEST");
    
    screen_init();

    while (1) {
        screen_clear(COLOR(255, 0, 0));
        screen_swap();

    }
}

