#include "macros.h"


// ----------- CURSOR ---------------

static void update_cursor(int x, int y) {
    uint16_t pos = y * VGA_WIDTH + x;

    // Send command to control port 0x3D4
    __asm__ volatile ("outb $0x0F, %%dx" : : "d"(0x3D4));
    __asm__ volatile ("outb %0, %%dx" : : "a"((uint8_t)(pos & 0xFF)), "d"(0x3D5));
    __asm__ volatile ("outb $0x0E, %%dx" : : "d"(0x3D4));
    __asm__ volatile ("outb %0, %%dx" : : "a"((uint8_t)((pos >> 8) & 0xFF)), "d"(0x3D5));
}
