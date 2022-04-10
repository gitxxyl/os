#include <stdint.h>
#include <include/port.h>
#include <lib/printf.h>

uint8_t cmos_getregister(uint8_t reg){
    outb(0x70, (0 << 7) | reg);
    uint8_t ret = inb(0x71);   
    return ret; 
}
char* rtc_datetime(){
    char* ret = kmalloc(64);
    uint8_t s = cmos_getregister(0x00);
    uint8_t min = cmos_getregister(0x02);
    uint8_t hour = cmos_getregister(0x04);
    uint8_t day = cmos_getregister(0x07);
    uint8_t month = cmos_getregister(0x08);
    uint8_t year = cmos_getregister(0x09);

    uint8_t registerB = cmos_getregister(0x0B);
    if(!(registerB & 0x04)){
        s = (s & 0x0F) + ((s / 16) * 10);
        min = (min & 0x0F) + ((min / 16) * 10);
        hour = ( (hour & 0x0F) + (((hour & 0x70) / 16) * 10) ) | (hour & 0x80);
        day = (day & 0x0F) + ((day / 16) * 10);
        month = (month & 0x0F) + ((month / 16) * 10);
        year = (year & 0x0F) + ((year / 16) * 10);
    }
    if (!(registerB & 0x02) && (hour & 0x80)) {
        hour = ((hour & 0x7F) + 12) % 24;
    } 
    
    sprintf(ret, "%d/%d/20%d %d:%d:%02d", day, month, year, hour, min, s);
    return ret; 
}
