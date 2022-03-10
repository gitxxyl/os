#include <stdint.h>

uint64_t pow(uint64_t base, uint64_t exp){
    if(exp < 0){
        base = 1/base;
        exp = -exp;
    }
    if (exp == 0){
        return 1;
    }
    uint64_t y = 1;
    while(exp > 1){
        if(exp%2==0){
            base = base*base;
            exp = exp / 2;
        } else {
            y = base * y;
            base = base * base;
            exp = (exp - 1) /2;
        }
    }
    return base * y;
}
