#pragma once 

typedef struct pml5_entry {
    // PML4 Table
    uint64_t present:   1;
    uint64_t rw:        1;
    uint64_t us:        1;
    uint64_t pwt:       1;
    uint64_t pcd:       1;
    uint64_t accessed:  1;
    uint64_t ignored:   6;
    uint64_t addr:      40;
    uint64_t ignored2:  11;
    uint64_t no_execute:1;
} pml5_entry_t;

typedef struct pml4_entry {
    // Page Directory Pointer Table
    uint64_t present:   1;
    uint64_t rw:        1;
    uint64_t us:        1;
    uint64_t pwt:       1;
    uint64_t pcd:       1;
    uint64_t accessed:  1;
    uint64_t ignored:   6;
    uint64_t addr:      40;
    uint64_t ignored2:  11;
    uint64_t no_execute:1;
} pml4_entry_t;

typedef struct pdpt_entry{
    // Page Directory or 1GB Page
    uint64_t present:   1;
    uint64_t rw:        1;
    uint64_t us:        1;
    uint64_t pwt:       1;
    uint64_t pcd:       1;
    uint64_t accessed:  1;
    uint64_t dirty:     1; // ignored for page dir
    uint64_t ps:        1; // ignored for page dir
    uint64_t global:    1; // ignored for page dir
    uint64_t ignored:   3;
    uint64_t pat:       1; // ignored for page dir
    uint64_t ignored2:  17;
    uint64_t addr:      22;
    uint64_t ignored3:  7;
    uint64_t pke:       4;
    uint64_t no_execute:1;
} pdpt_entry_t;

typedef struct page_directory_entry {
    // Page Table or 2MB Page
    uint64_t present:   1;
    uint64_t rw:        1;
    uint64_t us:        1;
    uint64_t pwt:       1;
    uint64_t pcd:       1;
    uint64_t accessed:  1;
    uint64_t dirty:     1;
    uint64_t ps:        1;
    uint64_t global:    1;
    uint64_t ignored:   3;
    uint64_t pat:       1;
    uint64_t reserved:  8;
    uint64_t addr:      31;
    uint64_t ignored2:   7;
    uint64_t pke:       4;
    uint64_t no_execute:1;
} pd_entry_t;

typedef struct page_table_entry {
    // 4 KB Page
    uint64_t present:   1;
    uint64_t rw:        1;
    uint64_t us:        1;
    uint64_t pwt:       1;
    uint64_t pcd:       1;
    uint64_t accessed:  1;
    uint64_t dirty:     1;
    uint64_t pat:       1;
    uint64_t global:    1;
    uint64_t ignored:   3;
    uint64_t addr:      40;
    uint64_t ignored2:  7;
    uint64_t pke:       4;
    uint64_t no_execute:1;
} pt_entry_t;
