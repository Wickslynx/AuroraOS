#include "ux/system/screen.h"
#include "core/interrupts.h"




void _main(u32 magic) {
    idt_init();
    isr_init();
    fpu_init(); // floating point unit.
    irq_init();
    
    screen_init();

    while (true) {
         screen_clear(COLOR(0, 0, 4));
    }

    screen_swap();
}

