//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//Please see the contributer_introduction file for more info on how to commit!

/*  ---- AuroraOS's windowing system ----


Important stuff to know:

Color values are declared in:
utils/macros/macros.h

All of the core FB is in fb/fb.h

Ground Up I/O is defined in io/io.h

Update cursor func declared in:
utils/funcs/functions.h

More stuff added in the future.



*/ 

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

#include "io.h"
#include "kernel/fb/fb.h"

typdef struct {
  uint16 id; //WINDOW ID
  uint16 x, y; //X, Y
  uint16 width, heigh; //HEIGHT, WIDTH
  Pixel *buffer; //Framebuffer
  uint8 z; //Layering (How it is gonna be drawn)
} Window;
  
typedef struct {
  uint16 id,
  Window windows[MAX_WIN]
  uint8 w_count
  char name[64];
} AuroApplication;






void AuroRectangle(Window* window, uint16_t x, uint16_t y, 
                        uint16_t width, uint16_t height, Pixel color) {
    if (!window) return;
    
    for (uint16_t dy = 0; dy < height; dy++) {
        for (uint16_t dx = 0; dx < width; dx++) {
            vfb_set_pixel(window, x + dx, y + dy, color);
        }
    }
}

void AuroLine(Window* window, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, Pixel color) {
    if (!window) return;
    
    int16_t dx = x2 - x1;
    int16_t dy = y2 - y1;
    int16_t steps = (abs(dx) > abs(dy)) ? abs(dx) : abs(dy);
    
    float x_increment = (float)dx / steps;
    float y_increment = (float)dy / steps;
    
    float x = x1;
    float y = y1;
    
    for (int16_t i = 0; i <= steps; i++) {
        os_set_pixel(window, (uint16_t)x, (uint16_t)y, color);
        x += x_increment;
        y += y_increment;
    }
}


// Create a new window for an application
Window*AuroWindowCreate(Application* app, uint16_t width, uint16_t height, const char* title) {
    if (!app || app->window_count >= MAX_WINDOWS_PER_APP) {
        return 0;
    }
    
    // Find first inactive window slot
    for (int i = 0; i < MAX_WINDOWS_PER_APP; i++) {
        if (app->windows[i].id == 0) {
            Window* window = &app->windows[i];
            
            // Allocate window buffer
            window->buffer = (Pixel*)vfb_malloc(width * height * sizeof(Pixel));
            
            if (!window->buffer) {
                return 0;  // Memory allocation failed
            }
            
            // Initialize window properties
            window->id = (app->app_id << 16) | (i + 1);
            window->width = width;
            window->height = height;
            window->state = WINDOW_STATE_ACTIVE;
            window->needs_redraw = true;
            
            // Set default position (could be improved)
            window->x = 50 * (app->window_count + 1);
            window->y = 50 * (app->window_count + 1);
            
            // Copy title
            OSmemcpy(window->title, title, 
                     OSstrlen(title) < 64 ? OSstrlen(title) : 63);
            
            app->window_count++;
            return window;
        }
    }
    
    return 0;  // No free window slots
}





