#pragma once
#include <stdint.h>

typedef struct {
    uint8_t magic1;
    uint8_t colormap;
    uint8_t encoding;
    uint16_t cmaporig, cmaplen;
    uint8_t cmapent;
    uint16_t x;
    uint16_t y;                 
    uint16_t h;                 
    uint16_t w;                 
    uint8_t bpp;
    uint8_t pixeltype;
} __attribute__((packed)) tga_header_t;
