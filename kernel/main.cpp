#include "ui/auro.h"
#include "core/interrupts.h"
#include "ui/system/screen.h"
#include "core/system.h"
#include "core/util.h"
#include "mem/vmm.h"
#include "core/error.h"


void test() {
    if (Auro::setVideoMode(0x117)) {
	    panic("VIDEO MODE NOT SUPPORTED!");
    }
    Auro::Window window;
    window.create("Test", 10, 10, SCREEN_HEIGHT - 20, SCREEN_WIDTH - 20);
}
    
extern "C" void _main(u32 magic) {
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
        screen_clear(COLOR(0, 255, 0));
	test();
        screen_swap();
    }

    

    // if we ever end up here, loop infinitly...
    for (;;) {}
}
