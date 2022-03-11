#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <stdint.h>

void fb_printchar(char);
void fb_print(char*);
void fb_print_color(char*, uint32_t);

#endif // GRAPHICS_H