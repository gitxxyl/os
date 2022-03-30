#include <include/memory.h>
#include <include/kheap.h>

#include <lib/alloc.h>
#include <lib/printf.h>
#include <lib/assert.h>
#include <lib/ordered_array.h>

extern heap_t* kheap;

void* kmalloc(uint64_t size){
    if(kheap == 0){
        uint64_t pages = (size % PAGE_SIZE) ? (size / PAGE_SIZE + 1) : (size / PAGE_SIZE);
        void* ptr = pmm_alloc_pages(pages);
        return (void*) (ptr + HH_MEMORY);   
    } else {
        return alloc(size, false, kheap);
    }
}

void kfree(void* ptr){
    assert(kheap != 0);
    free(ptr, kheap);
}



