#include <stdint.h>
#include <stddef.h>

void *memcpy(void *dest, const void *src, uint32_t count)
{
    const char *sp = (const char *)src;
    char *dp = (char *)dest;
    for(; count != 0; count--) *dp++ = *sp++;
    return dest;
}

void *memset(void *pointer, uint32_t value, size_t size)
{
    uint8_t *buffer_pointer = (uint8_t *)pointer;

    for (size_t i = 0; i < size; i++)
        *buffer_pointer++ = value;

    return pointer;
}

uint16_t *memsetw(uint16_t *dest, uint16_t val, uint32_t count)
{
    uint16_t *temp = (uint16_t *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}