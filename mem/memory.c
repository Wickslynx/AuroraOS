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
