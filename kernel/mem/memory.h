//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//Please see the contributer_introduction file for more info on how to commit!

/*   --Memory related systems built for AuroraOS--


Important stuff to know:

More stuff added in the future.



*/ 

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

//#include "kernel.h" // TODO: No main kernel header rn

void OSmemset(void *s, int c, unsigned int n);

void OSmemcpy(void *dest, const void *src, unsigned int n);

void* OSmalloc(uint32 size);

