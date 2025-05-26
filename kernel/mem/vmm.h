#ifndef VMM_H
#define VMM_H

#ifdef __cpluslus
extern "C" {
#endif

#include "../core/util.h"
#include "../core/error.h"
#include "../core/interrupts.h"


static u32 heap_ptr = 0x100000; // Heap starts after kernel.

page_dir_t *kernel_dir __attribute__((aligned(4096)));
page_table_t *kernel_tables[1024];

typedef struct {
  u32 present : 1; // page in mem
  u32 rw : 1; // read/write perms
  u32 user : 1; // user/supervisor privs
  u32 pwt : 1; // write through
  u32 pcd : 1; //chache disabled
  u32 accesed : 1; // has been accesed
  u32 dirty : 1; // has been written to (PGE)
  u32 pat : 1; // page attribute tabel
  u32 global : 1; //global, not flushed
  u32 available : 3; // can be used
  u32 frame : 32; // frame adress

}  page_t;

void init_vmm(); 

void vmap(u32 vaddr, u32 paddr, int present, int rw); 
void imap_kernel();
void* vmalloc(u32 size);
static inline void pfault();

static inline void pfault(struct Registers* regs);

#ifdef __cplusplus
}
#endif

#endif
