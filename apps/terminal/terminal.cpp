#include "../../kernel/ui/auro.h"
#include "../../kernel/drivers/keyboard.h"

int terminal() {
  Auro::Window window;
  int width = SCREEN_WIDTH - 20;
  int height = SCREEN_HEIGHT - 30;
  
  window.create("Aurora Terminal", 10, 10, height, width);

  window.clear(COLOR(0, 0, 0));
  window.text("user@aurora - $ ", (width - font_width("user@aurora - $")) / 20, 10, COLOR(0, 7, 0));
  return 0;
}
