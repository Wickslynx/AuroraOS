#include "../../kernel/ui/auro.h"
#include "../../kernel/drivers/keyboard.h"

int terminal() {
  Auro::Window window;
  window.create("Aurora Terminal", 10, 10, window.height, window.width);

  window.rectangle(COLOR(0, 0, 0), 0, 0, window.height, window.width);
  window.text("user@aurora - $ ", window.height - 10, window.width / 20, COLOR(0, 7, 0));
  return 0;
}
