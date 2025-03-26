#include "io.h"
#include "mem.h"
#include "core.h"

void kernel_main() {
  StartupOS();
  
  while (!OS_RUNNNING) {

    if (OSHandleEvent()) {
      OSError("FATAL", "Internal issue, event handling failed.");
    }

    if (FrameBufferHandle("FATAL", "Internal issue, framebuffer failed.")) {
      OSError();
    }
    
  }

}

