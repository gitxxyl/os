#ifndef MEMORY_H
#define MEMORY_H
#include <stdint.h>
#include <thirdparty/stivale2.h>

#define PAGE_SIZE 0x1000

#define HIGHER_HALF_CODE_OFFSET   0//0xFFFFFFFF80000000UL
#define L4HIGHER_HALF_DATA_OFFSET 0//0xFFFF800000000000UL
#define L5HIGHER_HALF_DATA_OFFSET 0//0xFF00000000000000UL

// PMM 
// FIXME: should probably separate into multiple .h files
void pmm_init(struct stivale2_struct*);
void* alloc_pages(uint64_t);
void free_pages(uint64_t, uint64_t);

// VMM

#endif // MEMORY_H