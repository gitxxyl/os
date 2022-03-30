#pragma once 

#include <stdint.h>

#define PIT_FREQ 1000

void timer_wait(uint32_t);
void timer_wait_ms(uint32_t);
void timer_init(uint64_t);
