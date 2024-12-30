#include "init/init.c"
#include "tests/tests.c"


void kernel_main() {
  OsInit();
  OsTest();

  OSWriteText("Hello, this OS is still in its experimental stage, all rights reserved @Wickslynx 2025.")

  
}

