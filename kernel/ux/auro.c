#include "auro.n"
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



WindowHandler* WindowHandler;

void __AuroRectangle(u16 color, int x, int y, int width, int height) {
    for (int j = 0; j < height; j++) {
        screen_fill(color, x, y + j, width, 1);
    }
}

void __AuroText(const char *text, int x, int y, u8 color) {
    font_str(text, x, y, color);
}


void AurWindowCreate(AWindow* window, char* name, int x, int y, int height, int width) {
    if (!window) {
        return;
    }

    if (WindowHandler->count >= MAX_WINDOWS) {
        return;
    }

    WindowHandler->windows[WindowHandler->count] = window;
    WindowHandler->count++;


    window->name = name;
    window->x = x;
    window->y = y;
    window->height = height;
    window->width = width;

    __AuroRectangle(COLOR(7, 7, 3), x, y, width, height);

}

void AurRectangle(AWindow* window, u16 color, int x, int y, int height, int width) {
    if (!window) {
        return;
    }

    if (height > window->height) {
        return;
    }

    if (width > window->width) {
        return;
    }


    if (window->widgets.count < MAX_WIDGETS) {
        struct Widget widget;
        window->widgets.widget[window->widgets.count++] = widget;
        __AuroRectangle(color, window->x + x, window->y + y, width, height);
    }
}

void init_auro() {
    WindowHandler = (WindowHandler*)kmalloc(sizeof(WindowHandler));
    if (WindowHandler) {
        WindowHandler->count = 0;
        // Initialize the windows array if needed
    } else {
        // Handle memory allocation failure
    }
}
