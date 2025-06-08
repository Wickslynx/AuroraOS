#include "../include/mem.h"

void* operator new(unsigned int size) {
  return (void*) 0x0000; // Placeholder, no fs nor memory system is implemented.
}

void* operator new[](unsigned int size) {
  return (void*) 0x0000;
}

void operator delete(void* ptr) {
  (void)ptr;
  return;
}

void operator delete[](void* ptr) {
  (void)ptr;
  return;
}


typedef struct block {
    u32 size;
    struct block* next;
} block_t;

static block_t* free_list = NULL;
static u8* heap_start = (u8*)0x140000;
static u8* heap_end = (u8*)0x540000;  // 1MB heap

void* malloc(u32 size) {
    size = (size + 7) & ~7;  // 8-byte align
    
    // init heap.
    if (!free_list) {
        free_list = (block_t*)heap_start;
        free_list->size = heap_end - heap_start - sizeof(block_t);
        free_list->next = NULL;
    }
    
    // find free block
    block_t** current = &free_list;
    while (*current) {
        if ((*current)->size >= size) {
            block_t* block = *current;
            
            // Split if block is much larger
            if (block->size > size + sizeof(block_t) + 16) {
                block_t* new_block = (block_t*)((u8*)block + sizeof(block_t) + size);
                new_block->size = block->size - size - sizeof(block_t);
                new_block->next = block->next;
                block->size = size;
                block->next = new_block;
            }
            
            // Remove from free list
            *current = block->next;
            return (u8*)block + sizeof(block_t);
        }
        current = &(*current)->next;
    }
    
    return NULL;  // Out of memory
}

void free(void* ptr) {
    if (!ptr) return;
    
    block_t* block = (block_t*)((u8*)ptr - sizeof(block_t));
    

    block->next = free_list;
    free_list = block;
    
    block_t* current = free_list;
    while (current && current->next) {
        u8* current_end = (u8*)current + sizeof(block_t) + current->size;
        if (current_end == (u8*)current->next) {
            current->size += sizeof(block_t) + current->next->size;
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
}
