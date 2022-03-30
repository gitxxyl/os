#pragma once 

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
typedef struct bitmap{
    uint8_t* map;
    size_t size;
} bitmap_t;

void set_bit(bitmap_t* bitmap, size_t bit);
void clear_bit(bitmap_t* bitmap, size_t bit);
bool get_bit(bitmap_t* bitmap, size_t bit);
uint64_t find_first_free(bitmap_t* bitmap);
