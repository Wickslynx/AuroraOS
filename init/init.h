#include "embedded/embedded.c"
#include "fs/file_sys.h"
#include "frontend/frontend.c"


int OsInit() {
  
  if (frontend_init() != 0) {
    return -1;
  }

  if (emb_init() != 0) {
    OsWriteText("Embedded systems initilaziation failed, exiting...");
    return -1;
  } 

  if (fs_init() != 0) {
    OsWriteText("File systems initilaziation failed, exiting...");
  } 

  OsWriteText("Initilaziation done, working on tests...")
  
    
}
