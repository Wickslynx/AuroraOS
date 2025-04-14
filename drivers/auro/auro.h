#ifndef AURO_H
#define AURO_H

#include "internal/screen.h"
#include "internal/font.h"


// ------- HELPERS ------------

static int abs(int x) {
    return (x < 0) ? -x : x;
}



void AuroraOSFrontendInit() {
  screen_init();
  screen_clear(0); // Clear screen to black
  screen_swap();
}

void AuroRectangle(int x, int y, int width, int height) {
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

void AuroLine(uint8 color, int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0), sx = (x0 < x1) ? 1 : -1;
    int dy = -abs(y1 - y0), sy = (y0 < y1) ? 1 : -1;
    int err = dx + dy, e2;
    
    while (1) {
        screen_set(color, x0, y0);  // Set pixel at (x0, y0)
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void AuroPolygon(uint8 color, int points[][2], int pointCount) {
    for (int i = 0; i < pointCount; i++) {
        int x0 = points[i][0], y0 = points[i][1];
        int x1 = points[(i + 1) % pointCount][0], y1 = points[(i + 1) % pointCount][1];
        AuroLine(color, x0, y0, x1, y1);
    }
}

#endif
