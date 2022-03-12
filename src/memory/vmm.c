/**
 * Virtual Memory Manager - manage paging
 * 
 * functions - 
 **/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <include/memory.h>
#include <include/paging.h>

void paging_init(){
    
}

void change_page_directory(page_directory_t* new_page_directory){
    asm volatile("mov %0, %%cr3" : : "r"((uintptr_t) new_page_directory));
}

static inline bool isL5(void) {
    uint64_t cr4;
    asm volatile("mov %%cr4, %0" : "=rax"(cr4));
    return (cr4 >> 12) & 1;
}

