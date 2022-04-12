#include <stdint.h>
#include <lib/mmio.h>

void mmoutb(uint64_t addr, uint8_t data){
    (*((volatile uint8_t*) addr)) = data;
}
void mmoutw(uint64_t addr, uint16_t data){
    (*((volatile uint16_t*) addr)) = data;
}
void mmoutl(uint64_t addr, uint32_t data){
    (*((volatile uint32_t*) addr)) = data;
}
void mmoutd(uint64_t addr, uint64_t data){
    (*((volatile uint64_t*) addr)) = data;
}
uint8_t mminb(uint64_t addr){
    return (*((volatile uint8_t*) addr));
}
uint16_t mminw(uint64_t addr){
    return (*((volatile uint16_t*) addr));
}
uint32_t mminl(uint64_t addr){
    return (*((volatile uint32_t*) addr));
}
uint64_t mmind(uint64_t addr){
    return (*((volatile uint64_t*) addr));
}