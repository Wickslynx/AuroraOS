#ifndef MEM_H
#define MEM_H

void* operator new(unsigned int size);
void* operator new[](unsigned int size);
void operator delete(void* ptr);
void operator delete[](void* ptr);

void* malloc(u32 size);
void free(void* ptr);
#endif
