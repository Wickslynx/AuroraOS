#ifndef LOG_H
#define LOG_H

#include "system.h"

#define COM1_PORT 0x3F8

static inline bool serial_ready(void) {
    return inportb(COM1_PORT + 5) & 0x20;
}

static inline void serial_write(char c) {

    while (!serial_ready());
    
    outportb(COM1_PORT, c);
}

static inline void log_init(void) {
    outportb(COM1_PORT + 1, 0x00); // disable interrupts
    
    outportb(COM1_PORT + 3, 0x80);
    outportb(COM1_PORT + 0, 0x03);
    outportb(COM1_PORT + 1, 0x00);
    outportb(COM1_PORT + 3, 0x03);
    outportb(COM1_PORT + 2, 0xC7);
    
    outportb(COM1_PORT + 4, 0x0B);
}


static inline void log_print(const char* str) {
    if (!str) return;
    
    while (*str) {
        if (*str == '\n') {
            serial_write('\r'); 
        }
        serial_write(*str++);
    }
}

#endif
