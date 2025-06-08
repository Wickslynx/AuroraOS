#include <auro.h>
#include <keyboard.h>
#include <util.h>


void runcmd(const char *cmd) {
  return;
}


int terminal() {

  int twidth =  (font_width("user@aurora - $ ")); // Starting width
  char cmd[1024]; //
  int len = 0;

  Auro::Internal::drawRectangle(COLOR(5, 10, 5), 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  Auro::Internal::drawText(" -$ ", 0, 0, COLOR(255, 255, 255), 3);
  char key = keyboard_get_last_char();
  
  
  if (key == 'KEY_ENTER') {
    runcmd(cmd); // placeholder
  } else {
    
    if (len < 1024) {
      cmd[len] = key;
    }

    if (!key == 0) {
    	char buffer[32]; // one char
    	itoa(key, buffer, sizeof(key));
    	Auro::Internal::drawText(buffer, twidth+2, 10, COLOR(7, 7, 3), 3);
    	len++;
    }
  }

  return 0;
}

