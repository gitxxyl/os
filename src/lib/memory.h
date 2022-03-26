#ifndef LIB_MEMORY_H
#define LIB_MEMORY_H

#include <stdint.h>

void *memcpy(void *dest, const void *src, uint32_t count);
void *memset(void *dest, char val, uint32_t count);
uint16_t *memsetw(uint16_t *dest, uint16_t val, uint32_t count);

#endif // MEMORY_H