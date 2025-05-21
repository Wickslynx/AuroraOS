#include "ux/system/screen.h"
#include "core/interrupts.h"




extern "C" void _main(u32 magic) {
    idt_init();
    isr_init();
    fpu_init(); // floating point unit.
    irq_init();
    
    screen_init();

    while (1) {
        screen_clear(COLOR(255, 0, 0));
        screen_swap();

    }
}

