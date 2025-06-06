#ifndef SCREEN_H
#define SCREEN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../core/util.h"
#include "../../core/error.h"
#include "../../mem/mem.h"


#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define BPP 2  // 16-bit = 2 bytes per pixel
#define SCREEN_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT * BPP)


extern u8 *sbuffers[2];
extern u8 _sback;


#define screen_buffer() (sbuffers[_sback])

#define screen_set(_p, _x, _y) \
    (((u16*)sbuffers[_sback])[((_y) * SCREEN_WIDTH + (_x))] = (_p))


#define screen_offset(_x, _y) \
    (&((u16*)screen_buffer())[(_y) * SCREEN_WIDTH + (_x)])


#define screen_fill(_c, _x, _y, _w, _h) do { \
    int __x = (_x); \
    int __y = (_y); \
    int __w = (_w); \
    int __h = (_h); \
    u16 __c = (_c); \
    u16 *buffer = (u16*)screen_buffer(); \
    for (int row = 0; row < __h; row++) { \
        for (int col = 0; col < __w; col++) { \
            buffer[((__y + row) * SCREEN_WIDTH + (__x + col))] = __c; \
        } \
    } \
} while (0)


// Function declarations
void screen_swap();
void screen_clear(u16 color);  
void screen_init();
int setVideoMode(u16 mode);


// red: 5 bits, green: 6 bits, blue: 5 bits
#define COLOR(_r, _g, _b) \
    ((u16)((((_r) & 0x1F) << 11) | (((_g) & 0x3F) << 5) | ((_b) & 0x1F)))


#define COLOR_RGB888(_r, _g, _b) \
    ((u16)(((((_r) >> 3) & 0x1F) << 11) | ((((_g) >> 2) & 0x3F) << 5) | (((_b) >> 3) & 0x1F)))



#ifdef __cplusplus
}
#endif

#endif
