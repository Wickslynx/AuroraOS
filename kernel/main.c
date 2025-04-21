#include "init/init.c"
#include "ux/ux.c"


// THIS IS THE MAIN FUNCTION THAT WILL BE CALLED WHEN THE OS BOOTS.
void _main(u32 magic) {
    kernel_init();
    ux_init();


    while (true) {
        const u32 now = (u32) timer_get();
        screen_swap();       
    }
}
