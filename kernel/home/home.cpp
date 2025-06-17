#include "home.h"
#include "../ux/auro.h"

Home::DrawApps() {
  
}

Home::DrawBackground() {
  screen_fill(255, 255, 255); // Haven't implemented image rendering yet.
}

Home::main() {
  Home::DrawBackground(); // Start with the background.
  Home::DrawApps();
}
