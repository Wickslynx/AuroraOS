#include "vmm.h"


page_dir_t *kernel_dir __attribute__((aligned(4096)));
page_table_t *kernel_tables[1024];

void init_vmm() {
   kernel_dir = (page_dir_t*)vmalloc(sizeof(page_dir_t));
   memset(kernel_dir, 0, sizeof(page_dir_t));

   imap_kernel();

   idt_set(14, pfault, 0x08, 0x8E);

   enable_paging((u32)kernel_dir);  // defined in ../boot/start.S
}

void imap_kernel() {
    u32 i;

    kernel_tables[0] = (page_table_t*)vmalloc(sizeof(page_table_t));
    memset(kernel_tables[0], 0, sizeof(page_table_t));

    // fill the page dir.
    for (i = 0; i < 1024; i++) {
         (*kernel_tables[0])[i].present = 1;
         (*kernel_tables[0])[i].rw = 1;
         (*kernel_tables[0])[i].user = 0;
         (*kernel_tables[0])[i].frame = i; // physical frame -> virtual frame..
   
    }

    // set the entry
    (*kernel_tables[0])[i].present = 1;
    (*kernel_tables[0])[i].rw = 1;
    (*kernel_tables[0])[i].user = 0;
    (*kernel_tables[0])[i].frame = ((u32)kernel_tables[0]) >> 12;
  
}

void* vmalloc(u32 size) {
    heap_ptr = (heap_ptr + 0xFFF) & ~0xFFF; // align heap pointer -> boundary
    
    void* addr = (void*)heap_ptr;

    if (!heap_ptr >= 0x140000) {
       heap_ptr += size;
    } else {
       panic("ALLOCATING MEMORY BEYOND BOUNDS"); // should change to a warning.
    }
       
    return addr;
}

static inline void pfault(struct Registers* regs) {
    u32 faddress; // error adress.
   
    asm("mov %%cr2, %0" : "=r" (faddress));
    
    // error code
    int present = !(regs->err_code & 0x1);
    int rw = regs->err_code & 0x2;
    int us = regs->err_code & 0x4;
    
    panic("Page fault...."); // There is no printf yet, so i can't print the error code.. TODO: Make this possible
    
    // we should never end up here, if we do we should just halt..
    for(;;);  
}
