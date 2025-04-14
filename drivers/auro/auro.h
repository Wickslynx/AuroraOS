#include "internal/screen.h"
#include "internal/font.h"


void AuroraOSFrontendInit() {
  screen_init();
  screen_clear(0); // Clear screen to black
  screen_swap();
}
