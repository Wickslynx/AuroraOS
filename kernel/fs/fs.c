#include "fs.h"

int __open(const char* name, file type) {
  switch (type) {
    case DATA:
      __read(fp);
      break;
    case SIZE:
      break;
    default:
      panic("Filesystem called with an unsupported type"); // This should probably be a warning, for now i'll leave it as a panic.
      break;
  }
}

int write(const char* name, int data) {
  int dsize = sizeof(data);
  int data = __open(name, file.DATA);
}

int read(const char* name) {
  return __open(name, file.DATA);
}

int mkdir(const char* name) {
  
}
int rm(const char* name) {
  
}
