#include "auro.h"
#include "../core/util.h"
#include <cstring>

/*

Included files:
system/screen.h
system/font.h

---- Information --

Note: Everything under the "Internal" namespace is internal... 
Application developers, when you use these functions you are writing to the framebuffer, and not your application.
Don't use them.

--------------------
*/

namespace Auro {
WindowManager* WindowManager::instance = nullptr;

void Internal::drawRectangle(u16 color, int x, int y, int width, int height) {
    for (int j = 0; j < height; j++) {
        screen_fill(color, x, y + j, width, 1);
    }
}

void Internal::drawText(const char* text, int x, int y, u8 color) {
    font_str(text, x, y, color);
}

void Window::create(const char* windowName, int posX, int posY, int windowHeight, int windowWidth) {
    // free previous name if it exits.
    if (name) {
        delete[] name;
    }
    
    // copy the window name
    size_t nameLength = strlen(windowName) + 1;
    name = new char[nameLength];
    strcpy(name, windowName);
    
    // set window properties
    x = posX;
    y = posY;
    height = windowHeight;
    width = windowWidth;
    
    // draw the window border
    Internal::drawRectangle(COLOR(7, 7, 3), x, y, width, height);
}

void Window::addRectangle(u16 color, int posX, int posY, int rectHeight, int rectWidth) {
    if (rectHeight > height || rectWidth > width) {
        return;
    }
    
    if (widgetCount < MAX_WIDGETS) {
        // create a new widget
        Widget widget;
        widgets[widgetCount++] = widget;
        
        // Draw the rectangle
        Internal::drawRectangle(color, x + posX, y + posY, rectWidth, rectHeight);
    }
}

WindowManager* WindowManager::initialize() {
    if (!instance) {
        instance = new WindowManager();
    }
    return instance;
}

bool WindowManager::addWindow(Window* window) {
    if (!window || windowCount >= MAX_WINDOWS) {
        return false;
    }
    
    windows[windowCount++] = window;
    return true;
}

void initialize() {
    WindowManager::initialize();
}

Window* createWindow(const char* name, int x, int y, int height, int width) {
    Window* window = new Window();
    window->create(name, x, y, height, width);
    
    WindowManager* manager = WindowManager::getInstance();
    if (manager) {
        manager->addWindow(window);
    }
    
    return window;
}

} // (namespace) Auro
