#include "../include/error.h"
#include <log.h>

void panic(const char *err) {
    screen_clear(COLOR(63, 0, 0));
    
    if (err != NULL) {        
        font_str(err, (SCREEN_WIDTH - font_width(err)) / 2, SCREEN_HEIGHT / 2 - 4, COLOR(7, 7, 3), 30);    
    } 
    
    LOG_ERROR(" core/error.c: PANIC called with \"");
    log_print(err);
    log_print("\"...");
    screen_swap();     
    for (;;) {}
  
}
