#include <lib/bitmap.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

void set_bit(bitmap_t* bitmap, size_t bit){
    bitmap->map[bit / 8] |= (1 << (bit % 8));
}
void clear_bit(bitmap_t* bitmap, size_t bit){
    bitmap->map[bit / 8] &= ~(1 << (bit % 8));
}
bool get_bit(bitmap_t* bitmap, size_t bit){
    return (bitmap->map[bit / 8] & (1 << (bit % 8))) != 0;
}
uint64_t find_first_free(bitmap_t* bitmap){
    for(uint64_t i = 0; i < bitmap->size; i++){
        if(bitmap->map[i] != 0xFF)
            for(uint64_t j = 0; j < 8; j++){
                if(!(bitmap->map[i] & (1 << j)))
                    return i * 8 + j;
            }
    }
    return -1;
}