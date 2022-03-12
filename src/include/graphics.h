#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <stdint.h>

void fb_printchar(char);
void fb_print(char*);
void fb_print_color(char*, uint32_t);
void fb_print_bits(uint64_t num);
void fb_changefg(uint32_t color);
void fb_changebg(uint32_t color);
uint32_t fb_getfg();

#endif // GRAPHICS_H