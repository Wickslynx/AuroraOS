#include "screen.h"

static uint8 *BUFFER = (uint8 *) 0xA0000;

// double buffers
uint8 _sbuffers[2][SCREEN_SIZE];
uint8 _sback = 0;

#define CURRENT (_sbuffers[_sback])
#define SWAP() (_sback = 1 - _sback)

// VGA control port addresses
#define PALETTE_MASK 0x3C6
#define PALETTE_READ 0x3C7
#define PALETTE_WRITE 0x3C8
#define PALETTE_DATA 0x3C9

void screen_swap() {
    OSmemcpy(BUFFER, &CURRENT, SCREEN_SIZE);
    SWAP();
}

void screen_clear(uint8 color) {
    OSmemset(&CURRENT, color, SCREEN_SIZE);
}

void screen_init() {
    // configure palette with 8-bit RRRGGGBB color
    outportb(PALETTE_MASK, 0xFF);
    outportb(PALETTE_WRITE, 0);
    for (uint8 i = 0; i < 255; i++) {
        outportb(PALETTE_DATA, (((i >> 5) & 0x7) * (256 / 8)) / 4);
        outportb(PALETTE_DATA, (((i >> 2) & 0x7) * (256 / 8)) / 4);
        outportb(PALETTE_DATA, (((i >> 0) & 0x3) * (256 / 4)) / 4);
    }

    // set color 255 = white
    outportb(PALETTE_DATA, 0x3F);
    outportb(PALETTE_DATA, 0x3F);
    outportb(PALETTE_DATA, 0x3F);
}
