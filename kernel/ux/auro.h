#ifndef AURO_H
#define AURO_H

#include "system/font.h"
#include "system/screen.h"
#include "../core/util.h"

/*

Included files:
system/screen.h
system/font.h

---- Information --
Note: Everything under "__" is internal

Application developers, when you use these functions you are writing to the framebuffer, and not your application.
Don't use them.

--------------------
*/

#define MAX_WINDOWS 16
#define MAX_WIDGETS 1000

typedef struct Widget {
    bool hidden;
    bool clickable;
} Widget;

typedef struct AWindow {
    int layer;
    int id;
    int x;
    int y;
    char* name;
    int height;
    int width;

    struct Widgets {
        int count;
        struct Widget widget[MAX_WIDGETS];
    } widgets;
} AWindow;


typedef struct WindowHandler {
    int count;
    AWindow* windows[MAX_WINDOWS];
} WindowHandler;

extern struct WindowHandler *g_WindowHandler;


void __AuroRectangle(u16 color, int x, int y, int width, int height);
void __AuroText(const char *text, int x, int y, u8 color);
void AurWindowCreate(AWindow* window, char* name, int x, int y, int height, int width);
void AurRectangle(AWindow* window, u16 color, int x, int y, int height, int width);

#endif