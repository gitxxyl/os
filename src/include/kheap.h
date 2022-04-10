#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <lib/ordered_array.h>

#define KHEAP_START         0xffff800001100000
#define KHEAP_INITIAL_SIZE  0x6400000
#define HEAP_INDEX_SIZE     0x20000
#define HEAP_MAGIC          0xBAAAAAAD
#define HEAP_MIN_SIZE       0x70000

typedef struct header {
    uint32_t magic; 
    uint32_t hole;
    uint64_t size;
} header_t;

typedef struct footer {
    uint32_t magic;
    header_t* header; // points to its own header
} footer_t;

typedef struct heap {
    ordered_array_t index;
    uint64_t start;
    uint64_t end;
    uint64_t max;
    bool us;
    bool rw;
} heap_t;

heap_t* create_heap(uint64_t start, uint64_t end, uint64_t max, bool us, bool rw);
void* alloc(uint64_t size, bool page_align, heap_t* heap);
void free(void* ptr, heap_t* heap);



