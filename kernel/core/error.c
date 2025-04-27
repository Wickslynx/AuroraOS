#include "error.h"

void panic(const char *err) {
    screen_clear(COLOR(7, 0, 0));     
    if (err != NULL) {        
        font_str(err, (SCREEN_WIDTH - font_width(err)) / 2, SCREEN_HEIGHT / 2 - 4, COLOR(7, 7, 3));    
    }      
    screen_swap();     
    for (;;) {}
  
}
