#ifndef FS_H
#define FS_H

#include "../core/util.h"

int write(const char* name, int size, int data);
int read(const char* name);

int mkdir(const char* name);
int rm(const char* name);


#endif
