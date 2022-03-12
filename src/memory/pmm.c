/**
 * pmm.c - Physical Memory Manager
 * Parses memory map and allocates pages using a bitmap.
 * functions - alloc_pages(), free_pages()
 **/

#include <thirdparty/stivale2.h>
#include <stddef.h>
#include <stdbool.h>
#include <include/bitmap.h>
#include <include/memory.h>
#include <lib/printf.h>
#include <lib/memory.h>
#include <lib/assert.h>
#include <lib/string.h>
#include <include/stivale_tag.h>

static uint64_t available_memory = 0;
static uint64_t total_memory = 0;
static uint64_t highest_page = 0;
bitmap_t bmp; 
static char* get_memory_type_string(int i);
static inline bool isL5(void);

uint64_t high_to_phys_data(uint64_t vaddr){ if(isL5()) {return vaddr - L5HIGHER_HALF_DATA_OFFSET;} return vaddr - L4HIGHER_HALF_DATA_OFFSET; }
uint64_t high_to_phys_code(uint64_t vaddr){ return vaddr - HIGHER_HALF_CODE_OFFSET; }
uint64_t phys_to_high_data(uint64_t paddr){ if(isL5()) {return paddr + L5HIGHER_HALF_DATA_OFFSET;} return paddr + L4HIGHER_HALF_DATA_OFFSET; }
uint64_t phys_to_high_code(uint64_t paddr){ return paddr + HIGHER_HALF_CODE_OFFSET; }

void pmm_init(struct stivale2_struct* stivale2_struct){
    
    // INIT MEMMAP
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
        if(mmap_tag->memmap[i].base > highest_page) highest_page = mmap_tag->memmap[i].base;
    }
    printf("Total memory: 0x%08x\n", total_memory);
    printf("Available memory: 0x%08x\n", available_memory);
    
    // INIT BITMAP

    bmp.size = (available_memory / PAGE_SIZE) / 8; // 1 bit per page (4096b), 8 bits per byte

    // place bitmap at the first usable memory address
    for(uint64_t i = 0; i < mmap_tag->entries; i++){
        if(mmap_tag->memmap[i].type == 1 && mmap_tag->memmap[i].length > bmp.size){
            bmp.map = (uint8_t*)(phys_to_high_data(mmap_tag->memmap[i].base));

            // the bitmap is not usable memory
            mmap_tag->memmap[i].base += bmp.size; 
            mmap_tag->memmap[i].length -= bmp.size;
            break;
        }
    }
    memset(bmp.map, 1, bmp.size);
    printf("Bitmap of size %d bytes initialised at 0x%08x\n", bmp.size, bmp.map);

    // test bitmap
    clear_bit(&bmp, 10);
    assert(get_bit(&bmp, 10) == 0);
    set_bit(&bmp, 10);
    assert(get_bit(&bmp, 10) == 1);

    for(uint64_t i = 0; i < mmap_tag->entries; i++){
        if(mmap_tag->memmap[i].type == 1){
            for(uint64_t j = 0; j < (mmap_tag->memmap[i].length / PAGE_SIZE); j++){
                clear_bit(&bmp, mmap_tag->memmap[i].base / PAGE_SIZE + j);
            }
        }
    }
    set_bit(&bmp, 0);
}


void* alloc_pages(uint64_t num){
    for(uint64_t i = 0; i < (highest_page / PAGE_SIZE); i++){
        if(!get_bit(&bmp, i)){
            for(uint64_t n = 0; n < num; n++)
                set_bit(&bmp, i + n);
            return (void*)(uint64_t)(phys_to_high_data(i * PAGE_SIZE));
        }
    }
}

void* liballoc_alloc(int num){
    return alloc_pages((uint64_t) num);
}

void free_pages(uint64_t vaddr, uint64_t num){
    uint64_t paddr = high_to_phys_data(vaddr);
    for(uint64_t i = 0; i < num; i++){
        clear_bit(&bmp, paddr / PAGE_SIZE + i);
    } 
    // TODO: track used pages 
}

int liballoc_free(void* vaddr, int num){
    free_pages((uint64_t) vaddr, (uint64_t) num);
    return 0;
}

int liballoc_lock(){
    asm("cli");
}

int liballoc_unlock(){
    asm("sti");
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

static inline bool isL5(void) {
    uint64_t cr4;

    asm volatile("mov %%cr4, %0" : "=rax"(cr4));

    return (cr4 >> 12) & 1;
}

