#ifndef STRING_H
#define STRING_H
#include <stdint.h>

uint32_t strlen(const char* str);
char* strrev(char* str);
char* itoa(uint64_t num, uint64_t base);
char* dtoa(float f);

#endif // STRING_H