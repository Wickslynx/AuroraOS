#ifndef AURO_H
#define AURO_H

extern "C" {
#include "../ui/system/font.h"
#include "../ui/system/screen.h"
#include "../ui/system/image.h"
}

#include "util.h"
#include "mem.h"

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

namespace Auro {

class Widget {
public:
    Widget() : hidden(false), clickable(false) {}
    
    bool isHidden() const { return hidden; }
    void setHidden(bool value) { hidden = value; }
    
    bool isClickable() const { return clickable; }
    void setClickable(bool value) { clickable = value; }

private:
    bool hidden;
    bool clickable;
};

class Window {
public:
    Window() : layer(0), id(0), x(0), y(0), name(nullptr), height(0), width(0), widgetCount(0) {}
    ~Window() { if (name) delete[] name; }
    
    void create(const char* windowName, int posX, int posY, int windowHeight, int windowWidth);
    
    void rectangle(u16 color, int posX, int posY, int rectHeight, int rectWidth);
    void text(const char *text, int posX, int posY, u16 color, u8 size);
    void clear(u16 color);


    // All get operations.
    int getLayer() const { return layer; }
    int getId() const { return id; }
    int getX() const { return x; }
    int getY() const { return y; }
    const char* getName() const { return name; }
    int getHeight() const { return height; }
    int getWidth() const { return width; }

    // All set operations.
    void setLayer(int value) { layer = value; }
    void setId(int value) { id = value; }

private:
    int layer;
    int id;
    int x;
    int y;
    char* name;
    int height;
    int width;
    
    int widgetCount;
    Widget widgets[MAX_WIDGETS];
    
    friend class WindowManager;
};

class WindowManager {
public:
    WindowManager() : windowCount(0) {}
    

    static WindowManager* initialize();
    

    bool addWindow(Window* window);

    int getWindowCount() const { return windowCount; }
    
    static WindowManager* getInstance() { return instance; }

private:
    int windowCount;
    Window* windows[MAX_WINDOWS];
    
    static WindowManager* instance;
};


namespace Internal {
    void drawRectangle(u16 color, int x, int y, int width, int height, int border_radius);
    void drawText(const char* text, int x, int y, u8 color, u8 size);
}

int setVideoMode(unsigned short mode);

void initialize();
Window* createWindow(const char* name, int x, int y, int height, int width);

} // (namespace) Auro

#endif // AURO_H
