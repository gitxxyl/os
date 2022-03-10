#include <include/bitmap.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

void set_bit(bitmap_t* bitmap, size_t bit){
    bitmap->map[bit / 8] |= (1 << (bit % 8));
}
void clear_bit(bitmap_t* bitmap, size_t bit){
    bitmap->map[bit / 8] &= ~(1 << (bit % 8));
}
void set_bits(bitmap_t* bitmap, size_t start, size_t end){
    for(size_t i = start; i <= end; i++){
        set_bit(bitmap, i);
    }
}
void clear_bits(bitmap_t* bitmap, size_t start, size_t end){
    for(size_t i = start; i <= end; i++){
        clear_bit(bitmap, i);
    }
}

bool get_bit(bitmap_t* bitmap, size_t bit){
    return (bitmap->map[bit / 8] & (1 << (bit % 8))) != 0;
}