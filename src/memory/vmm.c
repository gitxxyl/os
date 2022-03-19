/**
 * Virtual Memory Manager - manage paging
 * 
 * functions - 
 **/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <lib/printf.h>

#include <include/memory.h>
#include <include/paging.h>

static pml5_entry_t* PML5;

void vmm_init(){
    
}

void activate_pml5(pml5_entry_t* pml5){
    asm volatile("mov %0, %%cr3" : : "r"(pml5));
}

static inline bool isL5(void) {
    uint64_t cr4;
    asm volatile("mov %%cr4, %0" : "=rax"(cr4));
    return (cr4 >> 12) & 1;
}

