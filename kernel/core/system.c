#include "system.h"

static u32 rseed = 1;

#define COM1_PORT 0x3F8

void seed(u32 s) {
    rseed = s;
}

bool serial_ready(void) {
    return inportb(COM1_PORT + 5) & 0x20;
}

void serial_write(char c) {

    while (!serial_ready());
    
    outportb(COM1_PORT, c);
}

bool serial_recv(void) {
    return inportb(COM1_PORT + 5) & 1; 
}

char serial_read(void) {
    while (!serial_recv());
    
    return inportb(COM1_PORT);
}

void serial_init(void) {
    outportb(COM1_PORT + 1, 0x00);  
    outportb(COM1_PORT + 3, 0x80);    
    outportb(COM1_PORT + 0, 0x03);    
    outportb(COM1_PORT + 1, 0x00);   
    outportb(COM1_PORT + 3, 0x03);   
    outportb(COM1_PORT + 2, 0xC7);    
    outportb(COM1_PORT + 4, 0x0B);    
    outportb(COM1_PORT + 4, 0x1E);   
    outportb(COM1_PORT + 0, 0xAE);    // test
    
   
    if (inportb(COM1_PORT + 0) != 0xAE) {
        return;
    }

    
    outportb(COM1_PORT + 4, 0x0F);
}


void serial_readline(char* buffer, size_t max) {
    if (!buffer || max == 0) return;  
    
    size_t i = 0;
    while (i < max - 1) {
        char c = serial_read();
        
        if (c == '\r' || c == '\n') { // newline
            serial_write('\r'); 
            serial_write('\n');  
            break;
        } else if (c == '\b' || c == 127) { // DEL
            if (i > 0) {
                i--;
                serial_write('\b');  
                serial_write(' ');  
                serial_write('\b'); 
            }
        } else if (c >= 32 && c <= 126) { // only printable characters, else they will be ignored.
            buffer[i++] = c;
            serial_write(c);  
        }
    }
    buffer[i] = '\0'; // null terminate
}


u32 rand() {
    static u32 x = 123456789;
    static u32 y = 362436069;
    static u32 z = 521288629;
    static u32 w = 88675123;

    x *= 23786259 - rseed;

    u32 t;

    t = x ^ (x << 11);
    x = y; y = z; z = w;
    return w = w ^ (w >> 19) ^ t ^ (t >> 8);
}

