#include "internal/screen.h"
#include "internal/font.h"


void AuroraOSFrontendInit() {
  screen_init();
  screen_clear(0); // Clear screen to black
  screen_swap();
}

void AuroRectangle(x, y, width, height) {
  screen_fill(COLOR(7, 7, 3), x, y, width, height);
}

void AuroTriangle(uint8 color, int x, int y, int size) {
    for (int dy = 0; dy < size; dy++) {
        for (int dx = 0; dx <= dy; dx++) {
            screen_set(color, x + dx, y + dy);
        }
    }
}


void AuroCircle(uint8 color, int xc, int yc, int radius) {
    int x = radius, y = 0;
    int P = 1 - radius;
    
    while (x >= y) {
        screen_set(color, xc + x, yc + y);
        screen_set(color, xc - x, yc + y);
        screen_set(color, xc + x, yc - y);
        screen_set(color, xc - x, yc - y);
        screen_set(color, xc + y, yc + x);
        screen_set(color, xc - y, yc + x);
        screen_set(color, xc + y, yc - x);
        screen_set(color, xc - y, yc - x);
        
        y++;
        if (P <= 0) P += 2 * y + 1;
        else {
            x--;
            P += 2 * (y - x) + 1;
        }
    }
}


