#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include <stdint.h>
struct fb_struct{
    uint64_t addr;
    uint16_t width;
    uint16_t height;
    uint16_t bpp;
    uint16_t pitch;
};
#endif // FRAMEBUFFER_H