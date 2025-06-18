#ifdef __cplusplus
extern "C" {
#endif


#include "screen.h"


typedef struct {
    u16 signature;      // bm
    u32 file_size;     
    u16 reserved1;      
    u16 reserved2;     
    u32 data_offset;   
} __attribute__((packed)) bmp_file_t;

typedef struct {
    u32 header_size;    // 40
    u32 width;          
    u32 height;         
    u16 planes;         // (1)
    u16 bits_per_pixel; // (24 or 32)
    u32 compression;    // 0 = none
    u32 image_size;     // size
    u32 x_pixels_per_m; 
    u32 y_pixels_per_m; 
    u32 colors_used;    // num of colors used
    u32 colors_important; // num of important colors
} __attribute__((packed)) bmp_info_t;

int image_bmp(const u8 *bmp_data, u32 bmp_size, int x, int y);

#ifdef __cplusplus
}
#endif
