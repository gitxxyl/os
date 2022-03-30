#pragma once 

#include <stdint.h>
#include <stddef.h>

uint32_t strlen(const char* str);
char* strrev(char* str);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t n);
char* itoa(uint64_t num, uint64_t base);
char* dtoa(float f);
