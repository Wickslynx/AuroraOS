#ifndef VMM_H
#define VMM_H

#ifdef __cpluslus
extern "C" {
#endif

#include "../core/util.h"

typedef struct {
  u32 present : 1; // page in mem
  u32 rw : 1; // read/write perms
  u32 user : 1; // user/sudo privs
  u32 pwt : 1; // write through
  u32 pcd : 1; //chache disabled
  u32 accesed : 1; // has been accesed
  u32 dirty : 1; // has been written to (PGE)
  u32 pat : 1; // page attribute tabel
  u32 global : 1; //global, not flushed
  u32 available : 3; // can be used
  u32 frame : 30; // frame adress

}  page_t;

void init_vmm(); 

void vmap(u32 vaddr, u32 paddr, int present, int rw); 
void vmap_kernel();
void* vmalloc();


static inline void pfault();

#ifdef __cplusplus
}
#endif

#endif
