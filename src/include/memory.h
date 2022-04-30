#pragma once 
#include <stdint.h>
#include <stdbool.h>
#include <thirdparty/stivale2.h>

#define PAGE_SIZE 0x1000
#define HUGE_PAGE_SIZE 0x200000

#define HH_KERNEL   0xffffffff80000000
#define HH_MEMORY   0xffff800000000000

// PMM
void pmm_init(struct stivale2_struct*);
void* pmm_alloc_pages(uint64_t);
void pmm_free_pages(uint64_t, uint64_t);

// VMM
void vmm_map_page(uint64_t*, uint64_t, uint64_t, uint64_t, bool);
uint64_t vmm_virtual_to_phys(uint64_t*, uint64_t);