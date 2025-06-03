#include "screen.h"
#include <string.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif 

extern u32 FB_ADDR;
extern u16 CURRENT_VIDEO_MODE;


static u16* BUFFER = NULL;


u8 *sbuffers[2];
u8 _sback = 0;

#define CURRENT ((u16*)sbuffers[_sback])
#define SWAP() (_sback = 1 - _sback)

void screen_swap() {
    // SCREEN_SIZE is in bytes, divide by two.
    memcpy(BUFFER, CURRENT, SCREEN_SIZE);
    SWAP();
}

void screen_clear(u16 color) {
    u16 *current_buffer = CURRENT;
    for (size_t i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        current_buffer[i] = color;
    }
}

void screen_init() {
    if (!CURRENT_VIDEO_MODE) {
        panic("ERROR: VARIABLE - CURRENT_VIDEO_MODE not found!");
    }
    
    BUFFER = (u16 *)FB_ADDR;
    

    size_t buffer_size = SCREEN_SIZE; 
    sbuffers[0] = (u8 *)malloc(buffer_size);
    sbuffers[1] = (u8 *)malloc(buffer_size);
    
    if (!sbuffers[0] || !sbuffers[1]) {
        panic("UNABLE TO ALLOCATE BUFFERS");
    }
    

    memset(sbuffers[0], 0, buffer_size);
    memset(sbuffers[1], 0, buffer_size);
}

int setVideoMode(u16 mode) {
    u16 result;
    
    if (mode >= 0x100) {
        // VESA mode 
        asm (
            "int $0x10"
            : "=a" (result)
            : "a" (0x4F02), "b" (mode | 0x4000)  // 0x4000 enables linear framebuffer
            : "memory"
        );
        return (result == 0x004F) ? 1 : 0;
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
}


#ifdef __cplusplus
}
#endif
