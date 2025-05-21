#ifndef FS_H
#define FS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../core/util.h"

int write(const char* name, int size, int data);
int read(const char* name);

int mkdir(const char* name);
int rm(const char* name);

#ifdef __cplusplus
}
#endif


#endif
