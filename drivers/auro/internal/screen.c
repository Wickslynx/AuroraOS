#include "screen.h"

static volatile uint8 *BUFFER = (volatile uint8 *)(uintptr_t)0xA0000;

uint8 _sbuffers[2][SCREEN_SIZE];
uint8 _sback = 0;

#define CURRENT (_sbuffers[_sback])
#define SWAP() (_sback = 1 - _sback)

#define PALETTE_MASK 0x3C6
#define PALETTE_READ 0x3C7
#define PALETTE_WRITE 0x3C8
#define PALETTE_DATA 0x3C9

void screen_swap() {
    OSmemcpy((void*)BUFFER, CURRENT, SCREEN_SIZE);
    SWAP();
}

void screen_clear(uint8 color) {
    OSmemset(CURRENT, color, SCREEN_SIZE);
}

void screen_init() {
    outportb(PALETTE_MASK, 0xFF);
    outportb(PALETTE_WRITE, 0);
    for (uint8 i = 0; i < 255; i++) {
        outportb(PALETTE_DATA, (((i >> 5) & 0x7) * (256 / 8)) / 4);
        outportb(PALETTE_DATA, (((i >> 2) & 0x7) * (256 / 8)) / 4);
        outportb(PALETTE_DATA, (((i >> 0) & 0x3) * (256 / 4)) / 4);
    }

    outportb(PALETTE_DATA, 0x3F);
    outportb(PALETTE_DATA, 0x3F);
    outportb(PALETTE_DATA, 0x3F);
}
