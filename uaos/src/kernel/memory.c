#include "kernel.h"
#include "memory.h"

static uint8_t heap[HEAP_SIZE];
static uint32_t heap_pointer = 0;
static struct memory_block* free_list = NULL;

struct memory_block {
    uint32_t size;
    bool free;
    struct memory_block* next;
};

void memory_init(void) {
    // Initialize heap with one large free block
    struct memory_block* initial_block = (struct memory_block*)heap;
    initial_block->size = HEAP_SIZE - sizeof(struct memory_block);
    initial_block->free = true;
    initial_block->next = NULL;
    free_list = initial_block;
}

void* malloc(uint32_t size) {
    // Align to 4-byte boundary
    size = (size + 3) & ~3;
    
    struct memory_block* block = free_list;
    struct memory_block* prev = NULL;
    
    // Find suitable free block
    while (block) {
        if (block->free && block->size >= size) {
            // Split block if too large
            if (block->size > size + sizeof(struct memory_block)) {
                struct memory_block* new_block = 
                    (struct memory_block*)((uint8_t*)block + sizeof(struct memory_block) + size);
                new_block->size = block->size - size - sizeof(struct memory_block);
                new_block->free = true;
                new_block->next = block->next;
                
                block->size = size;
                block->next = new_block;
            }
            
            block->free = false;
            return (uint8_t*)block + sizeof(struct memory_block);
        }
        prev = block;
        block = block->next;
    }
    
    return NULL; // Out of memory
}

void free(void* ptr) {
    if (!ptr) return;
    
    struct memory_block* block = (struct memory_block*)((uint8_t*)ptr - sizeof(struct memory_block));
    block->free = true;
    
    // Coalesce adjacent free blocks
    struct memory_block* current = free_list;
    while (current && current->next) {
        if (current->free && current->next->free &&
            (uint8_t*)current + sizeof(struct memory_block) + current->size == (uint8_t*)current->next) {
            current->size += current->next->size + sizeof(struct memory_block);
            current->next = current->next->next;
        }
        current = current->next;
    }
}
