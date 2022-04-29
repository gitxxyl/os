#pragma once 

#include <stdint.h>
#include <include/vfs.h>
#include <thirdparty/stivale2.h>

typedef struct initrd_header {
    uint32_t nfiles;
} initrd_header_t;

typedef struct initrd_file_header {
    uint8_t magic;
    char name[64];
    uint32_t offset;
    uint32_t length;
} initrd_file_header_t;

fs_node_t* initrd_init(struct stivale2_struct*);