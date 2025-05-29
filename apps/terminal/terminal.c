#include "../../kernel/ui/auro.h"
#include "../../drivers/keyboard.h"

#define WINDOW_WIDTH SCREEN_WIDTH - 20
#define WINDOW_HEIGHT SCREEN_HEIGHT - 20

int terminal() {
  Auro::Window window;
  window.create("Aurora Terminal", 10, 10, WINDOW_HEIGHT, WINDOW_WIDTH);

  window.rectangle(COLOR(0, 0, 0), 0, 0, WINDOW_HEIGHT, WINDOW_WIDTH);
  window.text("user@aurora - $ ", WINDOW_HEIGHT - 10, WINDOW_WIDTH / 20);
  return 0;
}
