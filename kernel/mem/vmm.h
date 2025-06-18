#ifndef VMM_H
#define VMM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <util.h>
#include <error.h>
#include <interrupts.h>


static u32 heap_ptr = 0x100000; // Heap starts after kernel.


extern void enable_paging(u32 page_directory); // defined in ../boot/start.S

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

} page_t;

typedef page_t page_dir_t[1024];
typedef page_t page_table_t[1024];


void init_vmm(); 

void vmap(u32 vaddr, u32 paddr, int present, int rw); 
void imap_kernel();
void* vmalloc(u32 size);
void create_page(page_dir_t *dir);

static inline void pfault(struct Registers* regs);

#ifdef __cplusplus
}
#endif

#endif
