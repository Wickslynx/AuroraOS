#ifndef FB_H
#define FB_H

#define MAX_APPLICATIONS 16
#define MAX_WINDOW_WIDTH 1024
#define MAX_WINDOW_HEIGHT 768
#define MAX_WINDOWS_PER_APP 8


typedef struct {
    uint8_t r, g, b, a; 
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
    uint16_t id;            //Window ID.
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





// Global framebuffer manager instance
FramebufferManager g_framebuffer_manager;

static uint8_t memory_pool[1024 * 1024];  // 1MB memory pool
static uint32_t current_offset = 0;



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
int fb_init(uint16_t screen_width, uint16_t screen_height, uint8_t color_depth) {
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
void fb_shutdown() {
    // Reset all applications and windows
    for (int i = 0; i < MAX_APPLICATIONS; i++) {
        vfb_destroy_application(&g_framebuffer_manager.applications[i]);
    }
    
    // Reset framebuffer manager
    OSmemset(&g_framebuffer_manager, 0, sizeof(FramebufferManager));
}


// Composition function
void fb_compose_screen() {
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
int os_handle_event(InputEvent* event) {
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
    return 0;
}

// Utility drawing functions
void os_set_pixel(Window* window, uint16_t x, uint16_t y, Pixel color) {
    if (!window || x >= window->width || y >= window->height) return;
    
    uint32_t index = y * window->width + x;
    window->buffer[index] = color;
    window->needs_redraw = true;
}

#endif

