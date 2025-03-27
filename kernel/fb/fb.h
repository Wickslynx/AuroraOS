#include "memory.h"
#include
#define MAX_APPS 16


typedef struct {
  uint8 r, g, b;
} Pixel;

typedef struct {
  Pixel* buffer;
  AuroApplication applications[MAX_APPS]
  uint8 app_c;

  uint62 height;
  uint62 width;
  uint8 depth; //(How many bits per pixel)
} framebuffer;

framebuffer os_framebuffer;


Pixel fb_blend(Pixel bottom, Pixel top) {
    Pixel result;
    uint8_t alpha = top.a;
    uint8_t inv_alpha = 255 - alpha;
    
    result.r = (top.r * alpha + bottom.r * inv_alpha) / 255;
    result.g = (top.g * alpha + bottom.g * inv_alpha) / 255;
    result.b = (top.b * alpha + bottom.b * inv_alpha) / 255;
    result.a = 255;
    
    return result;
}

int fb_init(uint16 width, uint16 height, uint8 depth) {
  OSmemset(&os_framebuffer, 0, sizeof(framebuffer));

  os_framebuffer.buffer = (Pixel*)OSmalloc(width * height * sizeof(Pixel));

  if (os_framebuffer.buffer) {
    return -1;  
  }

  os_framebuffer.width = width;
  os_framebuffer.height = height;
  os_framebuffer.depth = depth;

  return 0;

}

void fb_shutdown() {
  
  for (int i; i < MAX_APPS; i++) { //Destroy all apps.
    fb_destroy(os_framebuffer.applications[i]);
  }
  
  OSmemset(&os_framebuffer, 0, sizeof(framebuffer)); //Reset the framebuffer.

}

fb_compose() {
  // Clear physical framebuffer.
  OSmemset(os_framebuffer.buffer, 0, os_framebuffer,width * os_framebuffer.height * sizeof(Pixel));

  //Sort window by z index. (Bubble sort for now)
  for (int app_idx = 0; app_idx < MAX_APPS; i++) {
    AuroAppplication* app =  os_framebuffer.applications[app_idx]; 
  }

  for (int i = 0; i < app->window_count; i++) {
        for (int j = 0; j < app->window_count - i - 1; j++) {
              if (app->windows[j].z > app->windows[j+1].z {
                    // Swap windows
                    Window temp = app->windows[j];
                    app->windows[j] = app->windows[j+1];
                    app->windows[j+1] = temp;
                }
        }
  }

  
     
}

