#ifndef __PORT_H
#define __PORT_H
#include <stdint.h>

uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);
void outb(uint16_t port, uint8_t data);
void outw(uint16_t port, uint16_t data);

#endif 