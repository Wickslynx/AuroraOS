#include "image.h"


// WARNING: This is AI generated.
int image_bmp(const u8 *bmp_data, u32 bmp_size, int x, int y) {
    if (bmp_size < sizeof(bmp_file_t) + sizeof(bmp_info_t)) {
        return -1;
    }
    
    bmp_file_t *file_header = (bmp_file_t*)bmp_data;
    bmp_info_t *info_header = (bmp_info_t*)(bmp_data + sizeof(bmp_file_t));
    
    // Check that it's an BMP
    if (file_header->signature != 0x4D42) { 
	LOG_WARNING("BMP trying to be loaded is not a valid BMP.. Returning");
        return -1;
    }
    
    // only uncompressed
    if (info_header->compression != 0 || info_header->bits_per_pixel != 24) {
        return -1;
    }
    
    // pixel data
    const u8 *pixel_data = bmp_data + file_header->data_offset;
    u32 width = info_header->width;
    u32 height = info_header->height;
    
    // row data (4 b alignment)
    u32 bytes_per_row = width * 3;
    u32 padding = (4 - (bytes_per_row % 4)) % 4;
    u32 padded_row_size = bytes_per_row + padding;
    
    // render pixels
    for (u32 row = 0; row < height; row++) {
        int screen_y = y + height - 1 - row; // flip
        if (screen_y < 0 || screen_y >= SCREEN_HEIGHT) continue;
        
        const u8 *row_data = pixel_data + (row * padded_row_size);
        
        for (u32 col = 0; col < width; col++) {
            int screen_x = x + col;
            if (screen_x < 0 || screen_x >= SCREEN_WIDTH) continue;
            
            // BGR
            u8 b = row_data[col * 3];
            u8 g = row_data[col * 3 + 1];
            u8 r = row_data[col * 3 + 2];
            
            u16 color = COLOR_RGB888(r, g, b);
            screen_set(color, screen_x, screen_y);
        }
    }
    
    return 0;
}

