#include "screen.h"

#ifdef __cplusplus
extern "C" {
#endif 

extern u32 FB_ADDR;
extern u16 CURRENT_VIDEO_MODE;
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
    for (size_t i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
         CURRENT[i] = color;
    }
}

void screen_init() {
    if (!CURRENT_VIDEO_MODE) {
        panic("ERROR: VARIABLE - CURRENT_VIDEO_MODE not found!"); // Add missing semicolon
    };

    BUFFER = (u16 *)FB_ADDR;

    size_t buffer_size = SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(u16);
    sbuffers[0] = (u16 *)malloc(buffer_size);
    sbuffers[1] = (u16 *)malloc(buffer_size);

    if (!sbuffers[0] || !sbuffers[1]) {
        panic("UNABLE TO ALLOCATE BUFFERS");
    }
}


int COLOR(int _r, int _g, int _b) {
    if (!CURRENT_VIDEO_MODE) {
        panic("ERROR: VARIABLE - CURRENT_VIDEO_MODE not found!");
    } else if (CURRENT_VIDEO_MODE >= 0x100) { // VBE - rn this aint used.
        return ((u32)(((_r) << 16) | ((_g) << 8) | (_b)));
    } else if (CURRENT_VIDEO_MODE < 0x100) { 
        _r = (_r * 7) / 255; // 3-bit (0-7)
        _g = (_g * 7) / 255; // 3-bit (0-7)  
        _b = (_b * 3) / 255; // 2-bit (0-3)
        return (u8)(((_r & 0x7) << 5) | ((_g & 0x7) << 2) | ((_b & 0x3) << 0)); 
    } else {
        panic("ERROR: UNKNOWN ERROR!"); 
    }
    return 0; 
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
