#include "../../kernel/ui/auro.h"
#include "../../kernel/drivers/keyboard.h"
#include "../../kernel/core/util.h"


void runcmd(const char *cmd) {
  return;
}


int terminal() {
  Auro::Window window;
  int width = SCREEN_WIDTH - 20;
  int height = SCREEN_HEIGHT - 30;
  int twidth =  (width - font_width("user@aurora - $ ")) / 20; // Starting width
  char cmd[1024]; //
  int len = 0;
  
  window.create("Aurora Terminal", 10, 10, height, width);

  window.clear(COLOR(0, 0, 0));
  window.text("user@aurora - $ ", twidth, 10, COLOR(0, 7, 0));

  char key = stoc(keyboard_get_last_char());
  
  
  if (key == 'KEY_ENTER') {
    runcmd(cmd); // placeholder
  } else {
    if (len < 1024) {
      cmd[len] = key;
    }

    char buffer[1]; // one char
    itoa(key, buffer, sizeof(key));
    window.text(buffer, twidth+2, 10, COLOR(7, 7, 3));
    len++;
  }

  return 0;
}
