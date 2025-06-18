#include <auro.h>
#include <interrupts.h>
#include <system.h>
#include <util.h>
#include <error.h>
#include <log.h>

#include "core/timer.h"
#include "mem/vmm.h"
#include "ui/system/screen.h"
#include "../user/apps/terminal/terminal.cpp"

void test() {
	terminal();
}
    
extern "C" void _main(u32 magic) {
    log_init();
    log_print("\033[32m [AuroraOS]\033[0m - Starting initialization.. \n");
    idt_init();
    isr_init();
    fpu_init(); // floating point unit.
    irq_init();
    keyboard_init();
    /*
    imap_kernel();
    init_vmm();
    */
	
    timer_init();
    screen_init();
    log_print("\033[32m [AuroraOS]\033[0m - Initialization [OK]... \n");

    bool quit = false;

    
	
    while (quit != true) {
	u64 now = timer_get();
    	u32 last = 0;
	    
	if (now != last) {
		last = now;
	}
	

        screen_clear(COLOR(0, 0, 7));
	test();
        screen_swap();
    }

    log_print("\033[32m [AuroraOS]\033[0m - Shutting down... \n");

    // if we ever end up here, loop infinitly...
    for (;;) {}
}
