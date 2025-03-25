//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//Please see the contributer_introduction file for more info on how to commit!

/*   --Frontend built for AuroraOS--


Important stuff to know:

Color values are declared in:
utils/macros/macros.h

Update cursor func declared in:
utils/funcs/functions.h

More stuff added in the future.



*/ 

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

#include "frontend.h"

// Video memory constants.
#define VGA_MEMORY 0xb8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_BYTES_PER_CHAR 2

// ---Global variables---
static uint8_t current_color = 0x07; // Default: light grey on black
static uint16_t cursor_position = 0;


int frontend_init() {
    //Initilize VGA mem.
    char *vidptr = (char*)VGA_MEMORY;
    unsigned int screen_size = VGA_WIDTH * VGA_HEIGHT * VGA_BYTES_PER_CHAR;
    
    // Clear screen.
    for (unsigned int i = 0; i < screen_size; i += 2) {
        vidptr[i] = ' ';
        vidptr[i+1] = current_color;
    }
    

    cursor_position = 0;// Reset cursor position
    
    update_cursor(0, 0); //Set cursors position to 0, 0.
    
    return 0;
}







void OsSetBGColor(uint8_t color) {
    if (color > 0xF) return; // Invalid color
    current_color = (current_color & 0x0F) | (color << 4);
    
    // Update entire screen with new background
    char *vidptr = (char*)VGA_MEMORY;
    unsigned int screen_size = VGA_WIDTH * VGA_HEIGHT * VGA_BYTES_PER_CHAR;
    
    for (unsigned int i = 1; i < screen_size; i += 2) {
        vidptr[i] = current_color;
    }
}

void OsOpenWindow(int x, int y, int width, int height, const char* title) {
    if (x < 0 || y < 0 || width <= 0 || height <= 0) return;
    if (x + width > VGA_WIDTH || y + height > VGA_HEIGHT) return;
    
    char *vidptr = (char*)VGA_MEMORY;
    
    // Draw window border
    for (int i = 0; i < width; i++) {
        // Top border
        int top_pos = (y * VGA_WIDTH + x + i) * 2;
        vidptr[top_pos] = '-';
        vidptr[top_pos + 1] = current_color;
        
        // Bottom border
        int bottom_pos = ((y + height - 1) * VGA_WIDTH + x + i) * 2;
        vidptr[bottom_pos] = '-';
        vidptr[bottom_pos + 1] = current_color;
    }
    
    for (int i = 0; i < height; i++) {
        // Left border
        int left_pos = ((y + i) * VGA_WIDTH + x) * 2;
        vidptr[left_pos] = '|';
        vidptr[left_pos + 1] = current_color;
        
        // Right border
        int right_pos = ((y + i) * VGA_WIDTH + x + width - 1) * 2;
        vidptr[right_pos] = '|';
        vidptr[right_pos + 1] = current_color;
    }
    
    // Draw title
    int title_pos = (y * VGA_WIDTH + x + 2) * 2;
    for (int i = 0; title[i] != '\0' && i < width - 4; i++) {
        vidptr[title_pos + i * 2] = title[i];
        vidptr[title_pos + i * 2 + 1] = current_color;
    }
}

void OSSetTextColor(uint8_t color) {
    if (color > 0xF) return; // Invalid color
    current_color = (current_color & 0xF0) | color;
}

char OSGetUserInput() {
    unsigned char temp_c = 0; // Changed to ensure proper type for input
    
    __asm__ volatile (
        "1: inb $0x64, %%al\n\t"    // Read keyboard status
        "testb $0x1, %%al\n\t"      // Check if input buffer is full
        "jz 1b\n\t"                 // If not, keep checking
        "inb $0x60, %%al\n\t"       // Read the character into al
        "movb %%al, %0"             // Move the character to temp_c
        : "=r"(temp_c)              // Output operand
        :                           // No input operands
        : "eax", "memory"           // Clobbered registers
    );
    
    return (char)temp_c;
}


void OsWriteText(const char *str) {
    if (!str) return;
    
    char *vidptr = (char*)VGA_MEMORY;
    int i = 0;
    
    while (str[i] != '\0') {
        if (cursor_position >= VGA_WIDTH * VGA_HEIGHT) {
            // Scroll screen up one line
            for (int y = 0; y < VGA_HEIGHT - 1; y++) {
                for (int x = 0; x < VGA_WIDTH * 2; x++) {
                    vidptr[y * VGA_WIDTH * 2 + x] = vidptr[(y + 1) * VGA_WIDTH * 2 + x];
                }
            }
            // Clear bottom line
            for (int x = 0; x < VGA_WIDTH * 2; x += 2) {
                vidptr[(VGA_HEIGHT - 1) * VGA_WIDTH * 2 + x] = ' ';
                vidptr[(VGA_HEIGHT - 1) * VGA_WIDTH * 2 + x + 1] = current_color;
            }
            cursor_position -= VGA_WIDTH;
        }
        
        if (str[i] == '\n') {
            // Move to next line
            cursor_position = ((cursor_position / VGA_WIDTH) + 1) * VGA_WIDTH;
        } else {
            // Write character
            vidptr[cursor_position * 2] = str[i];
            vidptr[cursor_position * 2 + 1] = current_color;
            cursor_position++;
        }
        i++;
    }
    
    // Update hardware cursor
    update_cursor(cursor_position % VGA_WIDTH, cursor_position / VGA_WIDTH);
}

