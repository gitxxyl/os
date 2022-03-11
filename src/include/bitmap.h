#ifndef BITMAP_H
#define BITMAP_H
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
typedef struct {
    uint8_t* map;
    size_t size;
} bitmap_t;

void set_bit(bitmap_t* bitmap, size_t bit);
void clear_bit(bitmap_t* bitmap, size_t bit);
void set_bits(bitmap_t* bitmap, size_t start, size_t end);
void clear_bits(bitmap_t* bitmap, size_t start, size_t end);
bool get_bit(bitmap_t* bitmap, size_t bit);
#endif // BITMAP_H