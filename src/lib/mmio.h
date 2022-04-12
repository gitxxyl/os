/**
 * mmio.h: memory mapped io operations
 * functions: mmoutb, mmoutw, mmoutl, mmoutd, mminb, mminw, mminl, mmoutd
 **/
#pragma once
#include <stdint.h>

void mmoutb(uint64_t addr, uint8_t data);
void mmoutw(uint64_t addr, uint16_t data);
void mmoutl(uint64_t addr, uint32_t data);
void mmoutd(uint64_t addr, uint64_t data);

uint8_t mminb(uint64_t addr);
uint16_t mminw(uint64_t addr);
uint32_t mminl(uint64_t addr);
uint64_t mmind(uint64_t addr);


