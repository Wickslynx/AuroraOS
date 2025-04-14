#include "fs/file_sys.h"
#include "../drivers/auro/auro.h"

void kernel_main() {
  /*StartupOS(); // TODO: Can't include the headers cuz they aint built:skull: so ill comment this out.
  
  while (!OS_RUNNNING) {

    if (OSHandleEvent()) {
      OSError("FATAL", "Internal issue, event handling failed.");
    }

    if (FrameBufferHandle("FATAL", "Internal issue, framebuffer failed.")) {
      OSError();
    }
    
  } */

  int OS_RUNNING = 0

  fs_init();
  AuroraOSFrontendInit();
  while (OS_RUNNING) {
    AuroRectangle(100, 200, 30, 50);
    AuroSwapBuffer();
  }


}

