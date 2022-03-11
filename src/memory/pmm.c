/**
 * pmm.c - Physical Memory Manager
 * Parses memory map and allocates pages using a bitmap.
 * functions - alloc_pages(), free_pages()
 **/

#include <thirdparty/stivale2.h>
#include <stddef.h>
#include <include/bitmap.h>
#include <lib/printf.h>
#include <lib/memory.h>
#include <lib/assert.h>
#include <lib/string.h>
#include <include/stivale_tag.h>

static uint64_t available_memory = 0;
static uint64_t total_memory = 0;
bitmap_t bmp; 
static char* get_memory_type_string(int i);

void pmm_init(struct stivale2_struct* stivale2_struct){
    
    // ---INIT MEMMAP---
    struct stivale2_struct_tag_memmap* mmap_tag;
    mmap_tag = (struct stivale2_struct_tag_memmap*) stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_MEMMAP_ID);
    if(mmap_tag == NULL){
        printf("memory map not found");
        return;
    }
    for(uint64_t i = 0; i < mmap_tag->entries; i++){
        printf("[0x%08x - 0x%08x]: size 0x%08x, type %s\n", 
            mmap_tag->memmap[i].base, 
            mmap_tag->memmap[i].base + mmap_tag->memmap[i].length, 
            mmap_tag->memmap[i].length, 
            get_memory_type_string(mmap_tag->memmap[i].type));
        total_memory += mmap_tag->memmap[i].length;
        if(mmap_tag->memmap[i].type == 1) available_memory += mmap_tag->memmap[i].length;
    }
    printf("Total memory: 0x%08x\n", total_memory);
    printf("Available memory: 0x%08x\n", available_memory);
    
    // ---INIT BITMAP---
    // 1 bit per page (4096b), 8 bits per byte
    bmp.size = (available_memory / 0x1000) / 8;
    for(uint64_t i = 0; i < mmap_tag->entries; i++){
        if(mmap_tag->memmap[i].type == 1 && mmap_tag->memmap[i].length > bmp.size){
            bmp.map = (uint8_t*) mmap_tag->memmap[i].base;
            break;
        }
    }
    memset(bmp.map, 0, bmp.size);
    printf("Bitmap of size %d bytes initialised at 0x%08x\n", bmp.size, bmp.map);
    // test bitmap
    // set_bit(&bmp, 10);
    assert(get_bit(&bmp, 10) == 1);
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