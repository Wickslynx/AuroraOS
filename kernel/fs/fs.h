#ifndef FS_H
#define FS_H

#ifdef __cplusplus
extern "C" {
#endif

#define FILE int

#include "../core/util.h"
#include "../core/error.h" 

int write(const char* name, int size, int data);
int read(const char* name);

int mkdir(const char* name);
int rm(const char* name);

typedef enum {
  DATA, 
  SIZE,
  PERMS
} file;

#ifdef __cplusplus
}
#endif



#endif
