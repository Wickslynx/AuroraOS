#include "screen.h"

#ifdef __cplusplus
extern "C" {
#endif 

#define FB_ADDR 0xFD000000
#define VIDEO_MODE 0x117 


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
    if (!VIDEO_MODE) {
        panic("ERROR: VARIABLE - VIDEO_MODE not found!");
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



#ifdef __cplusplus
}
#endif
