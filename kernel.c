#include "security/security.c"
#include "fs/file_system.c"
#include "frontend/frontend.c"

void kernel_main() {
  frontend_init();
  fs_init();
  emb_init();

  OSWriteText("Hello, this OS is still in its experimental stage, all rights reserved @Wickslynx 2025.")

  
}

