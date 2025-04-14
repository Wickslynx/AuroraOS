#ifndef UTIL_H
#define UTIL_H

// Fixed-width integer types
typedef unsigned int uint32;
typedef signed int int32;
typedef unsigned short uint16;
typedef signed short int16;
typedef unsigned char uint8;
typedef signed char int8;
typedef unsigned long long uint64;
typedef signed long long int64;
typedef unsigned long size;
typedef uint32 size_t;

#define TRUE  1
#define FALSE 0

// Macro utilities
#define CONCAT(x, y) x##y
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define CLAMP(x, min, max) MAX(min, MIN(x, max))

// Bit manipulation
#define HIBIT(x) (31 - __builtin_clz(x))
#define LOBIT(x) HIBIT((x) & -(x))
#define BIT_SET(v, n, x) ((v) ^ (((-x) ^ (v)) & (1 << (n))))

#define PACKED __attribute__((packed))

#ifndef asm
#define asm __asm__ volatile
#endif

#define CLI() asm ("cli")
#define STI() asm ("sti")

// Port I/O operations
static inline uint16 inports(uint16 port) {
    uint16 r;
    asm volatile("inw %1, %0" : "=a" (r) : "dN" (port));
    return r;
}

static inline void outports(uint16 port, uint16 data) {
    asm volatile("outw %1, %0" : : "dN" (port), "a" (data));
}

static inline uint8 inportb(uint16 port) {
    uint8 r;
    asm volatile("inb %1, %0" : "=a" (r) : "dN" (port));
    return r;
}

static inline void outportb(uint16 port, uint8 data) {
    asm volatile("outb %1, %0" : : "dN" (port), "a" (data));
}

// String functions
static inline size_t strlen(const char *str) {
    size_t l = 0;
    while (*str++) l++;
    return l;
}

// Memory functions
static inline void OSmemset(void *dst, uint8 value, size_t n) {
    uint8 *d = dst;
    while (n-- > 0) *d++ = value;
}

static inline void *OSmemcpy(void *dst, const void *src, size_t n) {
    uint8 *d = dst;
    const uint8 *s = src;
    while (n-- > 0) *d++ = *s++;
    return dst;
}

static inline void *OSmemmove(void *dst, const void *src, size_t n) {
    if (dst < src) return OSmemcpy(dst, src, n);

    uint8 *d = dst;
    const uint8 *s = src;
    for (size_t i = n; i > 0; i--) {
        d[i - 1] = s[i - 1];
    }

    return dst;
}

#endif // UTIL_H
