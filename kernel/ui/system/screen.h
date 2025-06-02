#ifndef SCREEN_H
#define SCREEN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../core/util.h"
#include "../../core/error.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define BPP 2
#define SCREEN_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT * BPP)




extern u8 _sbuffers[2][SCREEN_SIZE];
extern u8 _sback;

#define screen_buffer() (_sbuffers[_sback])

#define screen_set(_p, _x, _y)\
    (_sbuffers[_sback][((_y) * SCREEN_WIDTH + (_x))]=(_p))

#define screen_offset(_x, _y) (screen_buffer()[(_y) * SCREEN_WIDTH + (_x)])

#define screen_fill(_c, _x, _y, _w, _h) do {\
        __typeof__(_x) __x = (_x);\
        __typeof__(_y) __y = (_y);\
        __typeof__(_w) __w = (_w);\
        __typeof__(_y) __ymax = __y + (_h);\
        __typeof__(_c) __c = (_c);\
        for (; __y < __ymax; __y++) {\
            memset(&screen_buffer()[__y * SCREEN_WIDTH + __x], __c, __w);\
        }\
    } while (0)

void screen_swap();
void screen_clear(u8 color);
void screen_init();
int setVideoMode(u16 mode);

int COLOR(int _r, int  _b, int _g);

#ifdef __cplusplus
}
#endif

#endif
