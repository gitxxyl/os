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

uint64_t* k_pml4 = 0;
uint64_t pagetables_end;

uint64_t* vmm_get_pagetable(uint64_t* base, uint16_t offset, uint64_t flags){
    if(!(base[offset] & 0x1)){ // page not present
        uint64_t addr = ((uint64_t)pmm_alloc_pages(1));
        if(addr > pagetables_end) pagetables_end = addr;
        base[offset] = addr;
        base[offset] |= flags;
    }
    return (uint64_t*)(base[offset] & ~0xFFF); 
}

void vmm_map_page(uint64_t* map, uint64_t paddr, uint64_t vaddr, uint64_t flags){
    //if(vaddr > HH_KERNEL || vaddr > HH_MEMORY) dprintf("%llx %llx\n", paddr, vaddr);
    uint64_t pml4_index = (vaddr & ((uint64_t)0x1ff << 39)) >> 39;
    uint64_t pdp_index  = (vaddr & ((uint64_t)0x1ff << 30)) >> 30;
    uint64_t pd_index   = (vaddr & ((uint64_t)0x1ff << 21)) >> 21;
    uint64_t pt_index   = (vaddr & ((uint64_t)0x1ff << 12)) >> 12;
    
    
    uint64_t* pdp = vmm_get_pagetable((uint64_t*)map, pml4_index, 0b111);
    uint64_t* pd = vmm_get_pagetable((uint64_t*)pdp, pdp_index, 0b111);
    uint64_t* pt = vmm_get_pagetable((uint64_t*)pd, pd_index, 0b111);

    pt[pt_index] = ((uint64_t) paddr) | flags;
    invlpg(vaddr);
}

uint64_t vmm_virtual_to_phys(uint64_t* map, uint64_t vaddr){
    uint64_t pml4_index = (vaddr & ((uint64_t)0x1ff << 39)) >> 39;
    uint64_t pdp_index  = (vaddr & ((uint64_t)0x1ff << 30)) >> 30;
    uint64_t pd_index   = (vaddr & ((uint64_t)0x1ff << 21)) >> 21;
    uint64_t pt_index   = (vaddr & ((uint64_t)0x1ff << 12)) >> 12;
    
    uint64_t* pdp = vmm_get_pagetable((uint64_t*)map, pml4_index, 0b11);
    uint64_t* pd = vmm_get_pagetable((uint64_t*)pdp, pdp_index, 0b11);
    uint64_t* pt = vmm_get_pagetable((uint64_t*)pd, pd_index, 0b11);

    uint64_t paddr = pt[pt_index] & ~0xFFF;
    return paddr;
}

void load_pml4(uint64_t cpml4){
    asm volatile("mov %0, %%cr3" :: "r"(cpml4));
    //for(;;);
}

void invlpg(uint64_t addr){
    uint64_t cr3;
    asm volatile("mov %%cr3, %0" : "=r"(cr3));
    if((cr3 & ~((uint64_t) 0xfff)) == ((uint64_t)k_pml4 & ~((uint64_t) 0xfff))){
        asm volatile("invlpg (%0)" :: "r"(addr));
    }
}

void vmm_init(struct stivale2_struct* stivale2_struct){
    printf("[VIRTMM]");
    struct stivale2_struct_tag_kernel_base_address* kernel_base_address = stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_KERNEL_BASE_ADDRESS_ID);
    assert(kernel_base_address != NULL);
    uint64_t kernel_base_phys = kernel_base_address->physical_base_address;
    uint64_t kernel_base_virt = kernel_base_address->virtual_base_address;

    struct stivale2_struct_tag_hhdm* hhdm = stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_HHDM_ID);
    uint64_t hhdm_virt = hhdm->addr;

    k_pml4 = (uint64_t*)pmm_alloc_pages(1);
    dprintf("pml4: %p\n", k_pml4);
    dprintf("kernel_base_phys: %llx\n", kernel_base_phys);
    dprintf("kernel_base_virt: %llx\n", kernel_base_virt);
    dprintf("hhdm_virt: %llx\n", hhdm_virt);

    for (uint64_t i = 0; i < 0x200000; i += PAGE_SIZE){
        vmm_map_page(k_pml4, (uint64_t) i + kernel_base_phys, (uint64_t) i + kernel_base_virt, 0b11);
    }
    
    for (uint64_t i = 0; i < 0x100000000; i += PAGE_SIZE){
        vmm_map_page(k_pml4, i, (uint64_t) i, 0b11);
        vmm_map_page(k_pml4, i, (uint64_t) i + HH_MEMORY, 0b11);
    }

    struct stivale2_struct_tag_memmap* mmap_tag = stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_MEMMAP_ID);
    assert(mmap_tag != NULL);
    dprintf("entries: %llx", mmap_tag->entries);
    for(uint64_t i = 0; i < mmap_tag->entries; i++){
        uint64_t base = mmap_tag->memmap[i].base;
        if(base < 0x4000000) continue;
        dprintf("mapping extra");
        uint64_t length = mmap_tag->memmap[i].length;
        for(uint64_t j = 0; j < length; j += PAGE_SIZE){
            vmm_map_page(k_pml4, base + j, base + j, 0b11);
            vmm_map_page(k_pml4, base + j, base + j + HH_MEMORY, 0b11);
        }
    }
    dprintf("pagetables_end = %p\n", pagetables_end);
    load_pml4((uint64_t)k_pml4);
    printf_c(GREEN, " Initialized\n");
}

