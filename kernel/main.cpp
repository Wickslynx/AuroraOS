#include <auro.h>
#include <interrupts.h>
#include <system.h>
#include <util.h>
#include <error.h>
#include <log.h>

#include "mem/vmm.h"
#include "ui/system/screen.h"
#include "../user/home/home.cpp"

void test() {
	__home();
}
    
extern "C" void _main(u32 magic) {
    log_init();
    log_print("\033[32m [AuroraOS]\033[0m - Starting initialization.. \n");
    idt_init();
    isr_init();
    fpu_init(); // floating point unit.
    irq_init();
    //keyboard_init();
    screen_init();
    log_print("\033[32m [AuroraOS]\033[0m - Initialization [OK]... \n");

    bool quit = false;

    LOG_INFO("Reaching main loop..");
	
    while (quit != true) {
        screen_clear(COLOR(0, 0, 7));
	test();
        screen_swap();
    }

    log_print("\033[32m [AuroraOS]\033[0m - Shutting down... \n");

    // if we ever end up here, loop infinitly...
    for (;;) {}
}
