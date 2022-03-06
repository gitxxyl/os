#include <include/bitmap.h>
#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint8_t* map;
    size_t size;
} bitmap_t;
