page_dir_t *kernel_dir __attribute__((aligned(4096)));
page_table_t *kernel_tables[1024];

void init_vmm() {
   kernel_dir = (page_dir_t*)vmalloc(sizeof(page_dir_t));
   memset(kernel_dir, 0, sizeof(page_dir_t))

   imap_kernel();

   idt_set(14, page_error_handler, 0x08, 0x8E);

   enable_paging((u32)kernel_directory); // defined in ../boot/start.S
}

void imap_kernel() {
  u32 i;

  kernel_tables[0] = (page_table_t*)vmalloc(sizeof(page_table_t));
  memset(kernel_tables[0], 0, sizeof(page_table_t));

  for (;;); // NOTE: Finish this..
  
}

void vmalloc(u32 size) {
    heap_ptr = (heap_ptr + 0xFFF) & ~0xFFF; // align heap pointer -> boundary
    
    void* addr = (void*)heap_ptr;
    heap_ptr += size;
    
    return addr;
}

static inline void pfault(struct Registers* regs) {
    
    uint32_t fadress; // error adress.
    asm volatile("mov %%cr2, %0" : "=r" (faddress));
    
    // error code
    int present = !(regs->error_code & 0x1);
    int rw = regs->error_code & 0x2;
    int us = regs->error_code & 0x4;
    
    panic("Page fault...."); // There is no printf yet, so i can't print the error code.. TODO: Make this possible
    
    // we should never end up here, if we do we should just halt..
    for(;;);  
}
