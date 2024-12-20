#include "memory.h" 


void memset(void *s, int c, unsigned int n) {
  unsigned char *p = s;
  while (n--) {
    *p++ = (unsigned char)c;
  }
}

void memcpy(void *dest, void *src, unsigned int n) {
  unsigned char *d = dest; 
  const unsigned char *s = src;

  while (n--) {
    *d++ = *s++;
  }
}
