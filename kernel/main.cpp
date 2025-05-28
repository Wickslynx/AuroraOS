#include "ui/auro.h"
#include "core/interrupts.h"
#include "ui/system/screen.h"
#include "core/system.h"
#include "core/util.h"
#include "mem/vmm.h"
#include "core/error.h"


    
extern "C" void _main(u32 magic) {
    idt_init();
    isr_init();
    fpu_init(); // floating point unit.
    irq_init();
    // imap_kernel(); Smth is wrong with this, can't care enough to fix it for now so ill js comment it out.
    init_vmm();
    screen_init();

    bool quit = false;
    
    while (quit != true) {
        screen_clear(COLOR(0, 255, 0));
        screen_swap();
    }

    

    // if we ever end up here, loop infinitly...
    for (;;) {}
}
