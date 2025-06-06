#ifndef MEM_H
#define MEM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../core/util.h"

void* malloc(u32 size);
void free(void* ptr);

#ifdef __cplusplus
}
#endif
#endif
