#ifndef VIRTUAL_FRAMEBUFFER_H
#define VIRTUAL_FRAMEBUFFER_H

// Basic type definitions (consistent with your existing system)
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int bool;

// Maximum constraints for the system
#define MAX_APPLICATIONS 16
#define MAX_WINDOW_WIDTH 1024
#define MAX_WINDOW_HEIGHT 768
#define MAX_WINDOWS_PER_APP 8

// Color and pixel definitions
typedef struct {
    uint8_t r, g, b, a;  // Added alpha for better compositing
} Pixel;

// Window states
typedef enum {
    WINDOW_STATE_INACTIVE,
    WINDOW_STATE_ACTIVE,
    WINDOW_STATE_MINIMIZED,
    WINDOW_STATE_MAXIMIZED
} WindowState;

// Event types for input handling
typedef enum {
    EVENT_MOUSE_MOVE,
    EVENT_MOUSE_CLICK,
    EVENT_MOUSE_RELEASE,
    EVENT_KEY_PRESS,
    EVENT_KEY_RELEASE,
    EVENT_WINDOW_FOCUS,
    EVENT_WINDOW_BLUR
} EventType;

// Input event structure
typedef struct {
    EventType type;
    union {
        struct {
            uint16_t x, y;
            uint8_t button;
        } mouse;
        struct {
            uint16_t key_code;
            bool is_shift;
            bool is_ctrl;
            bool is_alt;
        } keyboard;
    } data;
} InputEvent;

// Window structure
typedef struct {
    uint16_t id;            // Unique window identifier
    uint16_t x, y;          // Window position
    uint16_t width, height; // Window dimensions
    WindowState state;      // Current window state
    Pixel* buffer;          // Virtual framebuffer for this window
    bool needs_redraw;      // Flag to optimize rendering
    uint8_t z_index;        // Layering order
    
    // Optional window metadata
    char title[64];         // Window title
    void* app_context;      // Pointer to application-specific data
} Window;

// Application context
typedef struct {
    uint16_t app_id;                    // Unique application identifier
    Window windows[MAX_WINDOWS_PER_APP]; // Windows owned by this application
    uint8_t window_count;               // Number of active windows
    char app_name[64];                  // Application name
} Application;

// Framebuffer management system
typedef struct {
    Pixel* physical_framebuffer;        // Actual screen memory
    Application applications[MAX_APPLICATIONS];
    uint8_t active_app_count;           // Number of active applications
    
    // Rendering and composition settings
    uint16_t screen_width;
    uint16_t screen_height;
    uint8_t color_depth;                // Bits per pixel
} FramebufferManager;

// Global framebuffer manager
extern FramebufferManager g_framebuffer_manager;

// Initialization and setup
int vfb_init(uint16_t screen_width, uint16_t screen_height, uint8_t color_depth);
void vfb_shutdown();

// Application and window management
Application* vfb_create_application(const char* app_name);
Window* vfb_create_window(Application* app, uint16_t width, uint16_t height, const char* title);
void vfb_destroy_window(Window* window);
void vfb_destroy_application(Application* app);

// Drawing primitives
void vfb_set_pixel(Window* window, uint16_t x, uint16_t y, Pixel color);
void vfb_draw_rectangle(Window* window, uint16_t x, uint16_t y, 
                        uint16_t width, uint16_t height, Pixel color);
void vfb_draw_line(Window* window, uint16_t x1, uint16_t y1, 
                   uint16_t x2, uint16_t y2, Pixel color);
void vfb_draw_text(Window* window, uint16_t x, uint16_t y, 
                   const char* text, Pixel color);

// Composition and rendering
void vfb_compose_screen();
void vfb_update_window(Window* window);

// Event handling
int vfb_handle_input_event(InputEvent* event);

// Utility functions
Pixel vfb_blend_pixels(Pixel bottom, Pixel top);
void* vfb_malloc(uint32_t size);
void vfb_free(void* ptr);

#endif // VIRTUAL_FRAMEBUFFER_H

// Implementation file follows
#include "virtual_framebuffer.h"

// Global framebuffer manager instance
FramebufferManager g_framebuffer_manager;

// Simple memory management (replace with your OS's memory allocation)
static uint8_t memory_pool[1024 * 1024];  // 1MB memory pool
static uint32_t current_offset = 0;

void* vfb_malloc(uint32_t size) {
    // Align size to 4 bytes
    size = (size + 3) & ~3;
    
    if (current_offset + size > sizeof(memory_pool)) {
        return 0;  // Out of memory
    }
    
    void* ptr = &memory_pool[current_offset];
    current_offset += size;
    return ptr;
}

void vfb_free(void* ptr) {
    // In this simple implementation, we don't actually free memory
    // In a real OS, you'd implement proper memory management
}

// Pixel blending for alpha compositing
Pixel vfb_blend_pixels(Pixel bottom, Pixel top) {
    Pixel result;
    uint8_t alpha = top.a;
    uint8_t inv_alpha = 255 - alpha;
    
    result.r = (top.r * alpha + bottom.r * inv_alpha) / 255;
    result.g = (top.g * alpha + bottom.g * inv_alpha) / 255;
    result.b = (top.b * alpha + bottom.b * inv_alpha) / 255;
    result.a = 255;
    
    return result;
}

// Initialize framebuffer system
int vfb_init(uint16_t screen_width, uint16_t screen_height, uint8_t color_depth) {
    // Reset global manager
    OSmemset(&g_framebuffer_manager, 0, sizeof(FramebufferManager));
    
    // Allocate physical framebuffer
    g_framebuffer_manager.physical_framebuffer = 
        (Pixel*)vfb_malloc(screen_width * screen_height * sizeof(Pixel));
    
    if (!g_framebuffer_manager.physical_framebuffer) {
        return -1;  // Allocation failed
    }
    
    // Set screen parameters
    g_framebuffer_manager.screen_width = screen_width;
    g_framebuffer_manager.screen_height = screen_height;
    g_framebuffer_manager.color_depth = color_depth;
    
    return 0;
}

// Shutdown and cleanup
void vfb_shutdown() {
    // Reset all applications and windows
    for (int i = 0; i < MAX_APPLICATIONS; i++) {
        vfb_destroy_application(&g_framebuffer_manager.applications[i]);
    }
    
    // Reset framebuffer manager
    OSmemset(&g_framebuffer_manager, 0, sizeof(FramebufferManager));
}

// Create a new application
Application* vfb_create_application(const char* app_name) {
    // Find first inactive application slot
    for (int i = 0; i < MAX_APPLICATIONS; i++) {
        if (g_framebuffer_manager.applications[i].app_id == 0) {
            Application* app = &g_framebuffer_manager.applications[i];
            
            // Initialize application
            app->app_id = i + 1;  // Start IDs from 1
            OSmemcpy(app->app_name, app_name, 
                     OSstrlen(app_name) < 64 ? OSstrlen(app_name) : 63);
            
            g_framebuffer_manager.active_app_count++;
            return app;
        }
    }
    
    return 0;  // No free slots
}

// Create a new window for an application
Window* vfb_create_window(Application* app, uint16_t width, uint16_t height, const char* title) {
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

// Composition function
void vfb_compose_screen() {
    // Clear physical framebuffer
    OSmemset(g_framebuffer_manager.physical_framebuffer, 0, 
             g_framebuffer_manager.screen_width * 
             g_framebuffer_manager.screen_height * sizeof(Pixel));
    
    // Sort windows by z-index (simple bubble sort)
    for (int app_idx = 0; app_idx < MAX_APPLICATIONS; app_idx++) {
        Application* app = &g_framebuffer_manager.applications[app_idx];
        
        for (int i = 0; i < app->window_count; i++) {
            for (int j = 0; j < app->window_count - i - 1; j++) {
                if (app->windows[j].z_index > app->windows[j+1].z_index) {
                    // Swap windows
                    Window temp = app->windows[j];
                    app->windows[j] = app->windows[j+1];
                    app->windows[j+1] = temp;
                }
            }
        }
        
        // Render windows for this application
        for (int win_idx = 0; win_idx < app->window_count; win_idx++) {
            Window* window = &app->windows[win_idx];
            
            if (window->state != WINDOW_STATE_ACTIVE) continue;
            
            // Copy window buffer to physical framebuffer with blending
            for (uint16_t y = 0; y < window->height; y++) {
                for (uint16_t x = 0; x < window->width; x++) {
                    // Check bounds
                    if (window->x + x >= g_framebuffer_manager.screen_width ||
                        window->y + y >= g_framebuffer_manager.screen_height) {
                        continue;
                    }
                    
                    uint32_t win_index = y * window->width + x;
                    uint32_t screen_index = 
                        (window->y + y) * g_framebuffer_manager.screen_width + 
                        (window->x + x);
                    
                    // Alpha blending
                    g_framebuffer_manager.physical_framebuffer[screen_index] = 
                        vfb_blend_pixels(
                            g_framebuffer_manager.physical_framebuffer[screen_index],
                            window->buffer[win_index]
                        );
                }
            }
        }
    }
}

// Input event handling
int vfb_handle_input_event(InputEvent* event) {
    // Find the window under the mouse cursor
    Window* target_window = 0;
    
    switch (event->type) {
        case EVENT_MOUSE_MOVE:
        case EVENT_MOUSE_CLICK:
        case EVENT_MOUSE_RELEASE: {
            for (int app_idx = 0; app_idx < MAX_APPLICATIONS; app_idx++) {
                Application* app = &g_framebuffer_manager.applications[app_idx];
                
                for (int win_idx = 0; win_idx < app->window_count; win_idx++) {
                    Window* window = &app->windows[win_idx];
                    
                    // Check if mouse is within window bounds
                    if (event->data.mouse.x >= window->x && 
                        event->data.mouse.x < window->x + window->width &&
                        event->data.mouse.y >= window->y && 
                        event->data.mouse.y < window->y + window->height) {
                        target_window = window;
                        break;
                    }
                }
                
                if (target_window) break;
            }
            break;
        }
        
        // Add handling for other event types as needed
        default:
            break;
    }
    
    // If no window found, return
    if (!target_window) return -1;
    
    // TODO: Implement specific event handling logic
    // This could involve:
    // - Raising window to top
    // - Sending event to application
    // - Updating window state
    
    return 0;
}

// Utility drawing functions
void vfb_set_pixel(Window* window, uint16_t x, uint16_t y, Pixel color) {
    if (!window || x >= window->width || y >= window->height) return;
    
    uint32_t index = y * window->width + x;
    window->buffer[index] = color;
    window->needs_redraw = true;
}

void vfb_draw_rectangle(Window* window, uint16_t x, uint16_t y, 
                        uint16_t width, uint16_t height, Pixel color) {
    if (!window) return;
    
    for (uint16_t dy = 0; dy < height; dy++) {
        for (uint16_t dx = 0; dx < width; dx++) {
            vfb_set_pixel(window, x + dx, y + dy, color);
        }
    }
}

void vfb_draw_line(Window* window, uint16_t x1, uint16_t y1, 
                   uint16_t x2, uint16_t y2, Pixel color) {
    if (!window) return;
    
    int16_t dx = x2 - x1;
    int16_t dy = y2 - y1;
    int16_t steps = (abs(dx) > abs(dy)) ? abs(dx) : abs(dy);
    
    float x_increment = (float)dx / steps;
    float y_increment = (float)dy / steps;
    
    float x = x1;
    float y = y1;
    
    for (int16_t i = 0; i <= steps; i++) {
        vfb_set_pixel(window, (uint16_t)x, (uint16_t)y, color);
        x += x_increment;
        y += y_increment;
    }
}

// Text rendering (basic implementation)
void vfb_draw_text(Window* window, uint16_t x, uint16_t y, 
                   const char* text, Pixel color) {
    if (!window || !text) return;
    
    while (*text) {
        // TODO: Implement proper text rendering
        // For now, just set pixels in a simple pattern
        for (int dy = 0; dy < 8; dy++) {
            for (int dx = 0; dx < 8; dx++) {
                vfb_set_pixel(window, x + dx, y + dy, color);
            }
        }
        
        x += 8;  // Move to next character position
        text++;
    }
}
