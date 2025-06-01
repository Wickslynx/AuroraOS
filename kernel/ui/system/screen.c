#include "screen.h"

#ifdef __cplusplus
extern "C" {
#endif 

extern u32 FB_ADDR


static u8* BUFFER = (u8*) 0xA0000;

// double buffers
u8 _sbuffers[2][SCREEN_SIZE];
u8 _sback = 0;

#define CURRENT (_sbuffers[_sback])
#define SWAP() (_sback = 1 - _sback)

// VGA control port addresses
#define PALETTE_MASK 0x3C6
#define PALETTE_READ 0x3C7
#define PALETTE_WRITE 0x3C8
#define PALETTE_DATA 0x3C9

void screen_swap() {
    memcpy(BUFFER, &CURRENT, SCREEN_SIZE);
    SWAP();
}

void screen_clear(u8 color) {
    if (!CURRENT_VIDEO_MODE) {
        panic("ERROR: VARIABLE - CURRENT_VIDEO_MODE not found!")
    } else if (CURRENT_VIDEO_MODE >= 0x100) {
        for (size_t i = 0; i < SCREEN_SIZE / 2; i++) {
            ((u16 *)CURRENT)[i] = color; // 16-bit write
        }
        return;
    } else {
        memset(&CURRENT, color, SCREEN_SIZE);
        return;
    }
}

void screen_init() {
    if (!CURRENT_VIDEO_MODE) {
        panic("ERROR: VARIABLE - CURRENT_VIDEO_MODE not found!")
    } else if (CURRENT_VIDEO_MODE >= 0x100) { // VBE - rn this aint used.
        BUFFER = (u8 *) FB_ADDR;
        return;
    } else { // VGA
        // configure palette with 8-bit RRRGGGBB color
        outportb(PALETTE_MASK, 0xFF);
        outportb(PALETTE_WRITE, 0);
        for (u8 i = 0; i < 255; i++) {
            outportb(PALETTE_DATA, (((i >> 5) & 0x7) * (256 / 8)) / 4);
            outportb(PALETTE_DATA, (((i >> 2) & 0x7) * (256 / 8)) / 4);
            outportb(PALETTE_DATA, (((i >> 0) & 0x3) * (256 / 4)) / 4);
        }
        // set color 255 = white
        outportb(PALETTE_DATA, 0x3F);
        outportb(PALETTE_DATA, 0x3F);
        outportb(PALETTE_DATA, 0x3F);
    }
    
}

int COLOR(float _r, float _b, float _g) {
    if (!CURRENT_VIDEO_MODE) {
        panic("ERROR: VARIABLE - CURRENT_VIDEO_MODE not found!")
    } else if (CURRENT_VIDEO_MODE >= 0x100) { // VBE - rn this aint used.
        return ((u32)(((_r) << 16) | ((_g) << 8) | (_b)));
    } else if (CURRENT_VIDEO_MODe < 0x100) { // VGA
        _r /= 255;
        _b /= 255;
        _g /= 255;
        return (u8)(((_r) & 0x7) << 5) | (((_g) & 0x7) << 2) | (((_b) & 0x3) << 0));
    } else {
        panic("ERROR: UNKNOWN ERROR!");
    };
}

int setVideoMode(unsigned short mode) {
    u16 result;
    if (mode >= 0x100) {
        // VESA mode 
        asm (
            "int $0x10"
            : "=a" (result)
            : "a" (0x4F02), "b" (mode | 0x4000)
            : "memory"
        );

        return (result == 0x004F) ? 1 : 0; // Error check.
    } else {
        // VGA Mode
        asm (
            "int $0x10"
            :
            : "a" (mode)
            : "memory"
        );
        return 1;
    }
    return 0;  // should never reach here.
}


#ifdef __cplusplus
}
#endif 
