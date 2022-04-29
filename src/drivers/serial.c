#include <lib/port.h>
#include <stdint.h>
#include <lib/string.h>
#include <lib/printf.h>

#define PORT 0x3f8 // serial COM1 port

/**
 * Serial port communication driver for debugging.
 * Qemu prints serial port output in the terminal - dprint() and sputch() to print to terminal.
 **/

uint32_t serial_init(){
    printf("[SERIAL]");
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
    printf_c(GREEN, " Initialized\n");
    return 0;
}
static uint32_t is_transmit_empty(){
    return inb(PORT+5) & 0x20;
}
void sputch(char a){
    while(is_transmit_empty()==0);
    outb(PORT, a);
}
void _dputchar(char character){
    sputch(character);
}
void dprint(char* text){
    for (uint32_t i = 0; i < strlen(text); i++)
    {
        sputch(text[i]);
    }
}