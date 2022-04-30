#include <stdint.h>
#include <stdbool.h>

#include <include/memory.h>
#include <include/kheap.h>

#include <lib/alloc.h>
#include <lib/panic.h>
#include <lib/assert.h>
#include <lib/printf.h>
#include <lib/ordered_array.h>

heap_t* kheap = 0;

static bool header_lessthan(void* a, void* b){
    return (((header_t*)a)->size < ((header_t*)b)->size)?true:false;
}
static int64_t find_smallest_hole(uint64_t size, bool page_align, heap_t* heap){
    int64_t i = 0;
    while (i < heap->index.size){
        header_t* header = (header_t*)lookup_ordered_array(i, &heap->index);
        if (page_align){
            uint64_t location = (uint64_t)header;
            uint64_t offset = 0;
            if ((location + sizeof(header_t)) % PAGE_SIZE){
                offset = PAGE_SIZE - (location + sizeof(header_t)) % PAGE_SIZE;
            }
            if (header->size >= size + offset)
                return i;
        }
        else if (header->size >= size)
            return i;  
        i++;   
    }
    return -1;
}
void expand(uint64_t size, heap_t* heap){
    assert(size > heap->end - heap->start);
    if(size & ~(0xFFF) != 0){
        size &= 0xFFFFF000;
        size += 0x1000;
    }
    heap->end = heap->start + size;
}
uint64_t contract(uint64_t size, heap_t* heap){
    assert(size < heap->end - heap->start);
    if(size & ~(0xFFF) != 0){
        size &= 0xFFFFF000;
        size += 0x1000;
    }
    if(size < HEAP_MIN_SIZE) size = HEAP_MIN_SIZE;
    pmm_free_pages(heap->start + size, heap->end - heap->start - size);
    heap->end = heap->start + size;
    return size;
}
heap_t* create_heap(uint64_t start, uint64_t end, uint64_t max, bool us, bool rw){
    heap_t* heap = (heap_t*) kmalloc(sizeof(heap_t));
    dprintf_c(BLUE_BOLD, "\n[HEAP] Info\n");
    dprintf("Heap start address: 0x%p\nHeap end address: 0x%p\nHeap max size: 0x%p\n", start, end, max);

    assert(heap != 0);
    assert(start % PAGE_SIZE == 0);
    assert(end % PAGE_SIZE == 0);

    heap->index = place_ordered_array((void*)start, HEAP_INDEX_SIZE, &header_lessthan);
    start += sizeof(void*) * HEAP_INDEX_SIZE;
    if(start & ~(0xFFF) != 0){
        start &= 0xFFFFF000;
        start += 0x1000;
    }
    heap->start = start;
    heap->end = end;
    heap->max = max;
    heap->us = us;
    heap->rw = rw;
    
    header_t* hole = (header_t*) start;
    hole->magic = HEAP_MAGIC;
    hole->size = end - start;
    dprintf("Heap address: 0x%p\nInitial hole size: 0x%llx\n", heap, hole->size);
    hole->hole = 1;
    insert_ordered_array((void*)hole, &heap->index);

    return heap;
}

void* alloc(uint64_t size, bool page_align, heap_t* heap){
    if(size % 8 != 0)
        size += 8 - (size % 8);
    uint64_t realsize = size + sizeof(header_t) + sizeof(footer_t);
    int i = find_smallest_hole(realsize, page_align, heap);
    if(i == -1){
        dprintf_c(RED, "Heap out of memory\n");
        dprintf("heap size: %d\n", heap->index.size);
        for(int j = 0; j < heap->index.size; j++){
            dprintf("[heap] %p, size = %llx\n", lookup_ordered_array(j, &heap->index), ((header_t*)lookup_ordered_array(j, &heap->index))->size);
            dprintf("requested size = %llx\n", realsize);
        }
        panic("Heap out of memory");
    }
 
    header_t* old_header = (header_t*)lookup_ordered_array(i, &heap->index);
    uint64_t old_size = old_header->size;

    if(old_size - realsize < sizeof(header_t) + sizeof(footer_t)){
        size += old_size - realsize;
        realsize = old_size;
    }   

    remove_ordered_array(i, &heap->index);

    // the block that we are allocating
    header_t* block_header = old_header;
    block_header->magic = HEAP_MAGIC;
    block_header->size = size + sizeof(header_t) + sizeof(footer_t);
    footer_t* block_footer = (footer_t*) ((uint64_t) old_header + sizeof(header_t) + size);
    block_footer->magic = HEAP_MAGIC;
    block_footer->header = block_header;

    // if there's space left in the hole to split
    if(old_size > realsize){
        header_t* hole_header = (header_t*) ((uint64_t) old_header + sizeof(header_t) + size + sizeof(footer_t));
        hole_header->magic = HEAP_MAGIC;
        hole_header->size = old_size - realsize;
        hole_header->hole = 1;
        footer_t* hole_footer = (footer_t*) ((uint64_t) hole_header + old_size - realsize - sizeof(footer_t));
        if((uint64_t) hole_footer < heap->end){
            hole_footer->magic = HEAP_MAGIC;
            hole_footer->header = hole_header;
        }
        insert_ordered_array((void*)hole_header, &heap->index);
    }
    return (void*) ((uint64_t) block_header + sizeof(header_t));
}

void free(void* ptr, heap_t* heap){
    if(ptr == 0) return;

    header_t *header = (header_t*) ((uint64_t)ptr - sizeof(header_t));
    footer_t *footer = (footer_t*) ((uint64_t)header + header->size - sizeof(footer_t));

    assert(header->magic == HEAP_MAGIC);
    assert(footer->magic == HEAP_MAGIC);

  
    header->hole = 1;
    char do_add = 1;

    // Unify left
    footer_t *test_footer = (footer_t*) ( (uint64_t)header - sizeof(footer_t) );
    if (test_footer->magic == HEAP_MAGIC &&
        !test_footer->header->hole)
    {
        uint64_t cache_size = header->size; 
        header = test_footer->header;     
        footer->header = header;          
        header->size += cache_size;       
        do_add = 0;                       
    }

    // Unify right
    header_t *test_header = (header_t*) ( (uint64_t)footer + sizeof(footer_t) );
    if (test_header->magic == HEAP_MAGIC &&
        !test_header->hole)
    {
        header->size += test_header->size; 
        test_footer = (footer_t*) ( (uint64_t)test_header + test_header->size - sizeof(footer_t) );
        footer = test_footer;
        uint64_t i = 0;
        while ((i < heap->index.size) && (lookup_ordered_array(i, &heap->index) != (void*)test_header)) i++;
        if(i < heap->index.size)
            remove_ordered_array(i, &heap->index);
    }
    if ( (uint64_t)footer+sizeof(footer_t) == heap->end)
    {
        uint64_t old_length = heap->end-heap->start;
        uint64_t new_length = contract((uint64_t)header - heap->start, heap);
        if (header->size - (old_length-new_length) > 0)
        {
            header->size -= old_length-new_length;
            footer = (footer_t*) ( (uint64_t)header + header->size - sizeof(footer_t) );
            footer->magic = HEAP_MAGIC;
            footer->header = header;
        }
        else
        {
            uint64_t i = 0;
            while ( (i < heap->index.size) &&
                    (lookup_ordered_array(i, &heap->index) != (void*)test_header) )
                i++;
            if (i < heap->index.size)
                remove_ordered_array(i, &heap->index);
        }
    }

    if (do_add == 1){
        insert_ordered_array((void*)header, &heap->index);
    }

    // dprintf("FREED %p, %llx\n", header, header->size);
    
}

void heap_init(){
    printf("[K_HEAP]");
    kheap = create_heap(KHEAP_START, 
                        KHEAP_START + (sizeof(void*) * HEAP_INDEX_SIZE) + KHEAP_INITIAL_SIZE, 
                        KHEAP_START + (sizeof(void*) * HEAP_INDEX_SIZE) + KHEAP_INITIAL_SIZE, 
                        true, 
                        true);
    printf_c(GREEN, " Initialized\n");
}