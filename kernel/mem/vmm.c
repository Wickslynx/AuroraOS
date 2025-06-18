#include "vmm.h"


page_dir_t *kernel_dir __attribute__((aligned(4096)));
page_table_t *kernel_tables[1024];


void init_vmm() {
   LOG_INFO(" mem/vmm.c: Setting up VMM...");
   kernel_dir = (page_dir_t*)vmalloc(sizeof(page_dir_t));
   memset(kernel_dir, 0, sizeof(page_dir_t));
   imap_kernel();
   idt_set(14, pfault, 0x08, 0x8E);
   enable_paging((u32)kernel_dir);  // defined in ../boot/start.S
}
void imap_kernel() {
    LOG_INFO(" mem/vmm.c: Mapping kernel...");
    kernel_tables[0] = (page_table_t*)vmalloc(sizeof(page_table_t));
    memset(kernel_tables[0], 0, sizeof(page_table_t));
    
    // i map first 1024 pages.
    for (u32 i = 0; i < 1024; i++) {
        (*kernel_tables[0])[i].present = 1;
        (*kernel_tables[0])[i].rw = 1;
        (*kernel_tables[0])[i].user = 0;
        (*kernel_tables[0])[i].frame = i;
    }
    
    // install pages.
    (*kernel_dir)[0].present = 1;
    (*kernel_dir)[0].rw = 1;
    (*kernel_dir)[0].user = 0;
    (*kernel_dir)[0].frame = ((u32)kernel_tables[0]) >> 12;
  
}

void create_page(page_dir_t *dir) {
    memset(dir, 0, sizeof(page_dir_t));
    (*dir)[0] = (*kernel_dir)[0]; // map kernel
}


void allocate_page(page_dir_t *dir, u32 vaddr) {
    u32 dir_index = vaddr >> 22;
    u32 table_index = (vaddr >> 12) & 0x3FF;

   // if there are no page table.
    if (!(*dir)[dir_index].present) {
        page_table_t *new_table = (page_table_t*)vmalloc(sizeof(page_table_t));
        memset(new_table, 0, sizeof(page_table_t));

        (*dir)[dir_index].present = 1;
        (*dir)[dir_index].rw = 1;
        (*dir)[dir_index].user = 1; // user page.
        (*dir)[dir_index].frame = ((u32)new_table) >> 12;
    }


    page_table_t *table = (page_table_t *)((*dir)[dir_index].frame << 12);

  
    u32 pframe = ((u32)vmalloc(4096)) >> 12;  // Allocate one page (4KB).

    (*table)[table_index].present = 1;
    (*table)[table_index].rw = 1;
    (*table)[table_index].user = 1; //  user accesible.
    (*table)[table_index].frame = pframe;
}


void imap_user(page_dir_t *dir, u32 vaddr, u32 pframe) {
    u32 dir_index = vaddr >> 22;
    u32 table_index = (vaddr >> 12) & 0x3FF;
    
    if (!(*dir)[dir_index].present) { // if dir exists.
        page_table_t *new_table = (page_table_t*)vmalloc(sizeof(page_table_t));
        memset(new_table, 0, sizeof(page_table_t));
        
        (*dir)[dir_index].present = 1;
        (*dir)[dir_index].rw = 1;
        (*dir)[dir_index].user = 1;
        (*dir)[dir_index].frame = ((u32)new_table) >> 12;
    }
    
    // get page table
    page_table_t *table = (page_table_t*)((*dir)[dir_index].frame << 12);
    
    // map it.
    (*table)[table_index].present = 1;
    (*table)[table_index].rw = 1;
    (*table)[table_index].user = 1;
    (*table)[table_index].frame = pframe;
}

void* vmalloc(u32 size) {
    heap_ptr = (heap_ptr + 0xFFF) & ~0xFFF; // align heap pointer -> boundary
    
    void* addr = (void*)heap_ptr;
    if (!(heap_ptr >= 0x140000)) {
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
