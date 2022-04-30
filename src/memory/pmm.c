/**
 * pmm.c - Page Frame Allocator
 * Parses memory map and allocates pages using a bitmap.
 * functions - alloc_pages(), free_pages()
 **/

#include <thirdparty/stivale2.h>
#include <stddef.h>
#include <stdbool.h>
#include <lib/bitmap.h>
#include <include/memory.h>
#include <lib/printf.h>
#include <lib/memory.h>
#include <lib/assert.h>
#include <lib/string.h>
#include <include/stivale_tag.h>

static uint64_t available_memory = 0;
static uint64_t total_memory = 0;
static uint64_t highest_page = 0;
static uint64_t used_pages = 0;
bitmap_t bmp; 

static char* get_memory_type_string(int i);

void pmm_init(struct stivale2_struct* stivale2_struct){
    printf("[PHYSMM]");
    struct stivale2_struct_tag_memmap* mmap_tag;
    mmap_tag = (struct stivale2_struct_tag_memmap*) stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_MEMMAP_ID);
    assert(mmap_tag != NULL);
    dprintf_c(BLUE_BOLD, "\n[PHYSMM] Memory Map Info\n");
    for(uint64_t i = 0; i < mmap_tag->entries; i++){
        struct stivale2_mmap_entry cur = mmap_tag->memmap[i];
        dprintf("[0x%016llx - 0x%016llx]: size 0x%016llx, type %s\n", 
            cur.base, 
            cur.base + cur.length, 
            cur.length, 
            get_memory_type_string(cur.type));
        total_memory += cur.length;
        if(cur.type == 1) available_memory += cur.length;
        if(cur.base + cur.length > highest_page) highest_page = cur.base + cur.length;
    }

    bmp.size = (available_memory / PAGE_SIZE) / 8;

    // place bitmap at the first usable memory address
    for(uint64_t i = 0; i < mmap_tag->entries; i++){
        struct stivale2_mmap_entry cur = mmap_tag->memmap[i];
        if(cur.type == 1 && cur.length > bmp.size){
            bmp.map = (uint8_t*)(cur.base);
            cur.base += (bmp.size + (0x1000 - 1)) & ~(0x1000 - 1);
            cur.length -= (bmp.size + (0x1000 - 1)) & ~(0x1000 - 1);
            mmap_tag->memmap[i] = cur;
            break;
        }
    }
    memset(bmp.map, 0xFFFF, bmp.size);
    dprintf("Bitmap of size %llx bytes initialised at 0x%016llx\n", bmp.size, bmp.map);

    // test bitmap
    clear_bit(&bmp, 10);
    assert(get_bit(&bmp, 10) == 0);
    set_bit(&bmp, 10);
    assert(get_bit(&bmp, 10) == 1);

    int a = 1+1;
    int b = 1+1;

    // Make USABLE memory available to be allocated

    for(uint64_t i = 0; i < mmap_tag->entries; i++){
        struct stivale2_mmap_entry cur = mmap_tag->memmap[i];
        if(cur.type == 1){
            dprintf("FREE: [0x%016llx - 0x%016llx]: size 0x%016llx, type %s\n", 
                cur.base, 
                cur.base + cur.length, 
                cur.length, 
                get_memory_type_string(cur.type));
            free_region(cur.base, cur.length);
        }
    }
    set_bit(&bmp, 0);
    printf_c(GREEN, " Initialized\n");
    dprintf("Total memory: 0x%08llx\n", total_memory);
    dprintf("Available memory: 0x%08llx\n", available_memory);
}

void free_region(uint64_t base, uint64_t length){
    uint64_t numpages = length / PAGE_SIZE;
    for(uint64_t i = 0; i < numpages; i++){
        clear_bit(&bmp, base / PAGE_SIZE + i);
    }
}

void lock_region(uint64_t base, uint64_t length){
    uint64_t numpages = length / PAGE_SIZE;
    for(uint64_t i = 0; i < numpages; i++){
        set_bit(&bmp, base / PAGE_SIZE + i);
    }
}

void* pmm_alloc_pages(uint64_t num){
    for(uint64_t i = 0; i < (highest_page / PAGE_SIZE); i++){
        if(!get_bit(&bmp, i)){
            for(uint64_t n = 0; n < num; n++)
                set_bit(&bmp, i + n);
            memset((void*)(i * PAGE_SIZE), 0, num * PAGE_SIZE);
            return (void*)(uint64_t)(i * PAGE_SIZE);
        }
    }
}


void pmm_free_pages(uint64_t addr, uint64_t num){
    for(uint64_t i = 0; i < num; i++){
        clear_bit(&bmp, addr / PAGE_SIZE + i);
    } 
    used_pages -= num;
}

static char* get_memory_type_string(int i){
    switch(i){
        case 1:
            return "USABLE";
        case 2:
            return "RESERVED";
        case 3:
            return "ACPI_RECLAIMABLE";
        case 4:
            return "ACPI_NVS";
        case 5:
            return "BAD_MEMORY";
        case 0x1000:
            return "BOOTLOADER_RECLAIMABLE";
        case 0x1001:
            return "KERNEL_AND_MODULES";             
        case 0x1002:
            return "FRAMEBUFFER";
    }
    return "INVALID";
}

