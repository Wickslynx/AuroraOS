#include <auro.h>
#include <interrupts.h>
#include <system.h>
#include <util.h>
#include <error.h>
#include <log.h>

#include "mem/vmm.h"
#include "ui/system/screen.h"
#include "../user/home/home.cpp"
#include "core/process.h"
#include "core/domain"


u8 simple_program[] = {
    0xEB, 0xFE  // JMP $
};

void test() {
	__home();
	u32 fake_entry_addr = 0x400000;  // Arbitrary user address
	u32 fake_stack_addr = 0x800000;  // Arbitrary user stack top
	
	// create some test proc.
	Process* test_proc = process_create("test_proc", 0, Domain domain, fake_entry_addr, fake_stack_addr);
	if (!test_proc) {
	    LOG_ERROR("Failed to create test process");
	    return;
	}
	
	 // load the test program
	if (load_program(test_proc, simple_program, sizeof(simple_program)) != 0) {
	    LOG_ERROR("Failed to load test program");
	    return;
	}
	
	// for now - simulate the scheduler.. This will be done automatically once i enable the scheduler.
	scheduler();
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

    LOG_INFO(" kernel/main.cpp: Entering main loop!");
	
    while (quit != true) {
        screen_clear(COLOR(0, 0, 7));
	test();
        screen_swap();
    }

    log_print("\033[32m [AuroraOS]\033[0m - Shutting down... \n");

    // if we ever end up here, loop infinitly...
    for (;;) {}
}
