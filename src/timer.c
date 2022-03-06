/**
 * timer.c - driver for the PIT (IRQ 32)
 * functions: timer_init(), get_ticks(), sleep_s(), sleep_ms()
 **/

#include <stdint.h>
#include <include/interrupts.h>
#include <include/timer.h>

volatile uint64_t ticks = 0;
void on_tick_handler();

void timer_init(uint64_t frequency){
    uint16_t divisor = 1193180 / frequency;
    outb(0x43, 0x36);

    uint8_t lo = (uint8_t) (divisor & 0xFF);
    uint8_t hi = (uint8_t) ((divisor >> 8) & 0xFF);

    outb(0x40, lo);
    outb(0x40, hi);

    isr_install_handler(32, on_tick_handler);
}

uint64_t get_ticks(){
    return ticks;
}

void timer_wait(uint32_t waitticks){
    uint64_t end = get_ticks();
    while(get_ticks() < (end + waitticks));
}

void on_tick_handler(registers_t r){
    // sprint("\nTick!");
    ticks++;
}