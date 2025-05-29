 #include "auro.h"
#include "../mem/mem.h"
#include "../core/util.h"
#include "../fs/fs.h"

/*

Included files:
system/screen.h -- Pixel manupilation.
system/font.h -- Core font functions.
core/util.h -- Kernel Utils..
fs/fs.h -- File opening, reading and closing functions.


---- Information -------

Note: Everything under the "Internal" namespace is internal... 
Application developers, when you use these functions you are writing to the framebuffer, and not your application.
Don't use them.

Everything under 
--------------------
*/

namespace Auro {


WindowManager* WindowManager::instance = nullptr;


//  Func defs

void Internal::drawRectangle(u16 color, int x, int y, int width, int height, int border_radius) {
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            bool shouldDraw = true;
            
            // Just alot of advanced math, i don't understand anything..
            if (i < border_radius && j < border_radius) {
                int dx = border_radius - i;
                int dy = border_radius - j;
                shouldDraw = (dx*dx + dy*dy) <= (border_radius*border_radius);
            }
            else if (i >= width - border_radius && j < border_radius) {
                int dx = i - (width - border_radius - 1);
                int dy = border_radius - j;
                shouldDraw = (dx*dx + dy*dy) <= (border_radius*border_radius);
            }
            else if (i < border_radius && j >= height - border_radius) {
                int dx = border_radius - i;
                int dy = j - (height - border_radius - 1);
                shouldDraw = (dx*dx + dy*dy) <= (border_radius*border_radius);
            }
            else if (i >= width - border_radius && j >= height - border_radius) {
                int dx = i - (width - border_radius - 1);
                int dy = j - (height - border_radius - 1);
                shouldDraw = (dx*dx + dy*dy) <= (border_radius*border_radius);
            }
            
            if (shouldDraw) {
                screen_fill(color, x + i, y + j, 1, 1);
            }
        }
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
    
    // set window props
    x = posX;
    y = posY;
    height = windowHeight;
    width = windowWidth;
    
    // draw the window border
    Internal::drawRectangle(COLOR(7, 7, 3), x, y, width, height, 10);
    Internal::drawRectangle(COLOR(86, 86, 86), x, y-(y/10*9), width, height/10, 0);
    Internal::drawRectangle(COLOR(0, 0, 0), x+5, y-5, 10, 10, 0);
    Internal::drawRectangle(COLOR(0, 0, 0), x+20, y-5, 10, 10, 0);
    Internal::drawRectangle(COLOR(0, 0, 0), x+35, y-5, 10, 10, 0); 


    
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
        Internal::drawRectangle(color, x + posX, y + posY, rectWidth, rectHeight, 0);
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


/*
bool Image(Window* window, const char* path) {
    if (!window || !path) {
        return false;
    }

    FILE* file = file::open(path, "r");
    int size = file::sizeof(file);

    if (file[1, 8] != "\x89PNG\r\n\x1a\n") {
        #include "image/png.h"
        int pixels[10000][100000][100000] = png::parse(file);
        for (int i = 0; i < size; i++) {
            screen_set(pixels[i][i][i]);
        }
    }

    file::close(file);
    
    
    return true;
}
*/
//  Public functions without any classes.

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
