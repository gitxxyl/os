#ifndef MEMORY_H
#define MEMORY_H
#include <stdint.h>
#include <thirdparty/stivale2.h>

#define PAGE_SIZE 0x1000

#define HH_KERNEL   0xffffffff80000000
#define HH_MEMORY   0xffff800000000000

// PMM 
// FIXME: should probably separate into multiple .h files
void pmm_init(struct stivale2_struct*);
void* pmm_alloc_pages(uint64_t);
void pmm_free_pages(uint64_t, uint64_t);

// VMM

#endif // MEMORY_H