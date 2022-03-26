/**
 * Virtual Memory Manager - manage paging
 * FIXME: this doesn't really work, but that's okay
 * Using level 4 paging
 **/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <lib/printf.h>
#include <lib/assert.h>
#include <lib/memory.h>

#include <include/memory.h>
#include <include/paging.h>

#include <stivale2.h>

uint64_t* pml4 = 0;

uint64_t* vmm_get_pagetable(uint64_t* base, uint16_t offset, uint64_t flags){
    if(!(base[offset] & 0x1)){ // page not present
        base[offset] = ((uint64_t)pmm_alloc_pages(1));
        base[offset] |= flags;
    }
    return (uint64_t*)(base[offset] & ~0x1FF + HH_MEMORY); 
}

void vmm_map_page(uint64_t* map, uint64_t paddr, uint64_t vaddr, uint64_t flags){
    uint64_t pml4_index = (vaddr & ((uint64_t)0x1ff << 39)) >> 39;
    uint64_t pdp_index  = (vaddr & ((uint64_t)0x1ff << 30)) >> 30;
    uint64_t pd_index   = (vaddr & ((uint64_t)0x1ff << 21)) >> 21;
    uint64_t pt_index   = (vaddr & ((uint64_t)0x1ff << 12)) >> 12;
    
    uint64_t* pdp = vmm_get_pagetable((uint64_t*)map, pml4_index, flags);
    uint64_t* pd = vmm_get_pagetable((uint64_t*)pdp, pdp_index, flags);
    uint64_t* pt = vmm_get_pagetable((uint64_t*)pd, pd_index, flags);

    pt[pt_index] = ((uint64_t) paddr) | flags;
    invlpg(vaddr);
}

void load_pml4(uint64_t cpml4){
    asm volatile("mov %0, %%cr3" :: "r"(cpml4));
    for(;;);
}

void invlpg(uint64_t addr){
    // the Translation Lookaside Buffer holds cached translations, 
    // so this method is called when mappings are changed to invalidate those caches
    asm volatile ("invlpg (%0)" :: "r" (addr));
}

void vmm_init(struct stivale2_struct* stivale2_struct){
    struct stivale2_struct_tag_memmap* mmap_tag;
    mmap_tag = (struct stivale2_struct_tag_memmap*) stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_MEMMAP_ID);
    assert(mmap_tag != NULL);

    pml4 = (uint64_t*)pmm_alloc_pages(1);
    //dprintf("pml4: %p\n", pml4);

    // identity map the first 4gb
    for (uint64_t i = 0; i < 0x100000000; i += PAGE_SIZE){
        vmm_map_page(pml4, i, i, 0b11);
    }

    // // map first 4gb to higher half offset
    for (uint64_t k = 0; k < 0x100000000; k += PAGE_SIZE){
        vmm_map_page(pml4, k, HH_MEMORY + k, 0b11);
    }

    for (uint64_t j = 0; j < 0x2000000; j += PAGE_SIZE){
        vmm_map_page(pml4, j, HH_KERNEL + j, 0b11);
    }

    for (uint64_t i = 0; i < mmap_tag->entries; i++){
        struct stivale2_mmap_entry cur = mmap_tag->memmap[i];
        uint64_t base = cur.base - (cur.base % PAGE_SIZE);
        uint64_t length = (((cur.length) + (PAGE_SIZE - 1)) / PAGE_SIZE) * PAGE_SIZE;
        if(cur.base % PAGE_SIZE){
            length += PAGE_SIZE;
        }
        for (uint64_t j = 0; j * PAGE_SIZE < length; j++){
            uint64_t addr = cur.base + j * PAGE_SIZE;
            if (addr < 0x100000000) continue;
            dprintf("actually allocating something");
            vmm_map_page(pml4, addr, HH_MEMORY + addr, 0b11);
        }
    }
    //for(;;);
    load_pml4((uint64_t) pml4);
    for(;;);
    
    
}
