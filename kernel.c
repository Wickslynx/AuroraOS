#include "security/security.c"
#include "fs/file_system.c"
#include "frontend/frontend.c"

void kernel_main() {
  frontend_init();
  fs_init();
  emb_init();

  OSWriteText()

  
}

