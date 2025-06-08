#include <auro.h>
#include <interrupts.h>
#include "ui/system/screen.h"
#include <system.h>
#include <util.h>
#include "mem/vmm.h"
#include <error.h>
#include "../user/apps/terminal/terminal.cpp"

void test() {
	terminal();
}
    
extern "C" void _main(u32 magic) {
    log_init();
    idt_init();
    isr_init();
    fpu_init(); // floating point unit.
    irq_init();
    /*
    imap_kernel();
    init_vmm();
    */
	

    screen_init();
    bool quit = false;
    
    while (quit != true) {
        screen_clear(COLOR(0, 0, 7));
	test();
        screen_swap();
    }

    

    // if we ever end up here, loop infinitly...
    for (;;) {}
}
