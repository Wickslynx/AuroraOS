#include "memory.h" 

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//Please see the contributer_introduction file for more info on how to commit!

/*   --Memory systems built for AuroraOS--


Important stuff to know:

More stuff added in the future.



*/ 

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------


void OSmemset(void *s, int c, unsigned int n) {
  unsigned char *p = s;
  while (n--) {
    *p++ = (unsigned char)c;
  }
}

void OSmemcpy(void *dest, const void *src, unsigned int n) {
  unsigned char *d = dest; 
  const unsigned char *s = src;

  while (n--) {
    *d++ = *s++;
  }
}

void* OSmalloc(uint32 size) {
    // Align size to 4 bytes
    size = (size + 3) & ~3;
    
    if (current_offset + size > sizeof(memory_pool)) {
        return 0;  // Out of memory
    }
    
    void* ptr = &memory_pool[current_offset];
    current_offset += size;
    return ptr;
}

