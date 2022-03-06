#include <include/port.h>
#include <stdint.h>

#define PORT 0x3f8 // serial COM1 port

/**
 * Serial port communication driver for debugging.
 * Qemu prints serial port output in the terminal - sprint() and sputch() to print to terminal.
 **/

uint32_t serial_init(){
    outb(PORT + 1, 0x00);
    outb(PORT + 3, 0x80);
    outb(PORT + 0, 0x03);
    outb(PORT + 1, 0x00);
    outb(PORT + 3, 0x03);
    outb(PORT + 2, 0xc7);
    outb(PORT + 4, 0x0b);
    outb(PORT + 4, 0x1e);
    outb(PORT + 0, 0xae);
    if(inb(PORT + 0) != 0xAE) {
        return 1;
    }
    outb(PORT + 4, 0x0F);
    return 0;
}
static uint32_t is_transmit_empty(){
    return inb(PORT+5) & 0x20;
}
void sputch(char a){
    while(is_transmit_empty()==0);
    outb(PORT, a);
}
void sprint(char* text){
    uint32_t i;

    for (i = 0; i < strlen(text); i++)
    {
        sputch(text[i]);
    }
}