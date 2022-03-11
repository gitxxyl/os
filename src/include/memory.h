#ifndef MEMORY_H
#define MEMORY_H
#include <thirdparty/stivale2.h>

void pmm_init(struct stivale2_struct*);
uint64_t alloc_pages(uint32_t);
void free_pages(uint64_t, uint32_t);

#endif // MEMORY_H