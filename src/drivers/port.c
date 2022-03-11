#include <stdint.h>

uint8_t inb(uint16_t port){
    uint8_t data;
    __asm__ __volatile__("inb %1, %0" : "=a" (data) : "dN" (port));
    return data;
}
uint16_t inw(uint16_t port){
    uint16_t data;
    __asm__ __volatile__("inw %1, %0" : "=a" (data) : "dN" (port));
    return data;
}
void outb(uint16_t port, uint8_t data){
    __asm__ __volatile__("outb %1, %0" : : "dN" (port), "a" (data));
}
void outw(uint16_t port, uint16_t data){
    __asm__ __volatile__("outw %1, %0" : : "dN" (port), "a" (data));
}
