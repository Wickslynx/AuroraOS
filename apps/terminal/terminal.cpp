#include "../../kernel/ui/auro.h"
#include "../../kernel/drivers/keyboard.h"

int terminal() {
  Auro::Window window;
  int width = SCREEN_WIDTH - 20;
  int height = SCREEN_HEIGHT - 30
  
  window.create("Aurora Terminal", 10, 10, height, width);

  window.rectangle(COLOR(0, 0, 0), 0, 0, height, width);
  window.text("user@aurora - $ ", height - 10, width / 20, COLOR(0, 7, 0));
  return 0;
}
