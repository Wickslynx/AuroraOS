#include "ux/system/screen.h"




void _main(u32 magic) {
    screen_init();

    while (true) {
         screen_clear(COLOR(0, 0, 4));
    }

    screen_swap();
}

