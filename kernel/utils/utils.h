#ifndef UTIL_H
#define UTIL_H



// Fixed-width integer types
typedef unsigned int uint32;
typedef signed int int32;
typedef unsigned short uint16;
typedef signed short int16;
typedef unsigned char uint8;
typedef signed char int8_t;
typedef unsigned long long uint64;
typedef signed long long int64;
typedef unsigned long size;


#define true  1
#define false 0
    

#define NULL  ((void*)0)

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
static inline u16 inports(u16 port) {
    u16 r;
    asm("inw %1, %0" : "=a" (r) : "dN" (port));
    return r;
}

static inline void outports(uint16 port, u16 data) {
    asm("outw %1, %0" : : "dN" (port), "a" (data));
}

static inline uint8 inportb(uint16 port) {
    uint8 r;
    asm("inb %1, %0" : "=a" (r) : "dN" (port));
    return r;
}

static inline void outportb(uint16 port, uint8 data) {
    asm("outb %1, %0" : : "dN" (port), "a" (data));
}

// String functions
static inline size_t strlen(const char *str) {
    size_t l = 0;
    while (*str++) l++;
    return l;
}

static inline char *itoa(int32 x, char *s, size_t sz) {
    if (sz < 20) {
        extern void panic(const char *);
        panic("ITOA BUFFER TOO SMALL");
    }

    u32 tmp = x;
    i32 i = 0;

    do {
        tmp = x % 10;
        s[i++] = (tmp < 10) ? (tmp + '0') : (tmp + 'a' - 10);
    } while (x /= 10);
    s[i--] = 0;

    for (i32 j = 0; j < i; j++, i--) {
        tmp = s[j];
        s[j] = s[i];
        s[i] = tmp;
    }

    return s;
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
    return d;
}

static inline void *OSmemmove(void *dst, const void *src, size_t n) {
    if (dst < src) return memcpy(dst, src, n);

    uint8 *d = dst;
    const uint8 *s = src;

    for (size_t i = n; i > 0; i--) {
        d[i - 1] = s[i - 1];
    }

    return dst;
}

// Safe string concatenation
static inline size_t strlcat(char *dst, const char *src, size_t size) {
    size_t sl = strlen(src);
    size_t dl = strlen(dst);

    if (dl == size) return size + sl;

    if (sl < (size - dl)) {
        memcpy(dst + dl, src, sl + 1);
    } else {
        memcpy(dst + dl, src, size - dl - 1);
        dst[size - 1] = '\0';
    }

    return sl + dl;
}

static inline size_t strlcpy(char *dst, const char *src, size_t n) {
    char *d = dst;
    const char *s = src;
    size_t size = n;

    while (--n > 0) {
        if ((*d++ = *s++) == 0) break;
    }

    if (n == 0) {
        if (size != 0) *d = 0;
        while (*s++);
    }

    return s - src - 1;
}

#endif
