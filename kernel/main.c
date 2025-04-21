#include "init/init.c"
#include "ux/ux.c"
#include "core/timer.h"

#include "core/util.h"
#include "ux/screen.h"
#include "init/idt.h"
#include "init/isr.h"
#include "init/irq.h"
#include "core/timer.h"
#include "ux/font.h"
#include "core/system.h"
#include "drivers/keyboard.h"
#include "init/fpu.h"

// THIS IS THE MAIN FUNCTION THAT WILL BE CALLED WHEN THE OS BOOTS.
void _main(u32 magic) {
    kernel_init();
    ux_init();


    while (true) {
        // const u32 now = (u32) timer_get();
        
        screen_swap();       
    }
}
