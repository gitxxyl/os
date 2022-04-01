#pragma once
#include <stdint.h>
uint8_t cmos_getregister(uint8_t reg);
char* rtc_datetime();