/**
 * pmm.c - Physical Memory Manager
 * Parses memory map and allocates pages using 5 level paging setup by Limine.
 * functions - alloc_pages(), free_pages()
 **/

#include <stivale2.h>
#include <stddef.h>
#include <include/common.h>


static uint64_t detected_memory = 0;
void pmm_init(struct stivale2_struct* stivale2_struct){
    struct stivale2_struct_tag_memmap* mmap_tag;
    mmap_tag = stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_MEMMAP_ID);
    if(mmap_tag == NULL){
        fb_print("memory map not found");
        return;
    }
    for(uint64_t i = 0; i < mmap_tag->entries; i++){
        fb_print("[0x");
        fb_print(itoa(mmap_tag->memmap[i].base, 16));
        fb_print(" - 0x");
        fb_print(itoa(mmap_tag->memmap[i].base + mmap_tag->memmap[i].length, 16));
        fb_print("]: size 0x");
        fb_print(itoa(mmap_tag->memmap[i].length, 16));
        fb_print(", type ");
        // fb_print(itoa(mmap_tag->memmap[i].type, 16));
        switch(mmap_tag->memmap[i].type){
            case 1:
                fb_print("USABLE");
                break;
            case 2:
                fb_print("RESERVED");
                break;
            case 3:
                fb_print("ACPI_RECLAIMABLE");
                break;
            case 4:
                fb_print("ACPI_NVS");
                break;
            case 5:
                fb_print("BAD_MEMORY");
                break;
            case 0x1000:
                fb_print("BOOTLOADER_RECLAIMABLE");
                break;
            case 0x1001:
                fb_print("KERNEL_AND_MODULES");
                break;                
            case 0x1002:
                fb_print("FRAMEBUFFER");
                break;
        }
        fb_print("\n");
        detected_memory += mmap_tag->memmap[i].length;
    }
    fb_print("Total: 0x");
    fb_print(itoa(detected_memory, 16));

    
}

