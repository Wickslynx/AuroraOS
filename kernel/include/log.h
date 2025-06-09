#ifndef LOG_H
#define LOG_H

#include "system.h"

#define COM1_PORT 0x3F8


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

#define LOG_ERROR(message) log_print("\033[31m [ERROR] \033[0m " message)
#define LOG_INFO(message) log_print("\033[96m [INFO] \033[0m " message)
#define LOG_WARNING(message) log_print("\033[95m [WARNING] \033[0m " message)


#endif
