#include "fs/file_sys.h"
#include "../drivers/auro/auro.h"

void kernel_main() {
    fs_init();
    AuroraOSFrontendInit();

    int OS_RUNNING = 1;

    while (OS_RUNNING) {
        AuroRectangle(100, 200, 30, 50);
        AuroSwapBuffers();
    }
}
