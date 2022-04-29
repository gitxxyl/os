#include <cpuid.h>
#include <lib/string.h>
#include <lib/printf.h>
#include <stdbool.h>


void get_cpuid(){
    int a, b, c, d;
    for(int i = 0; i < 5; i++){
        __cpuid(i, a, b, c, d);
        dprintf("%x %x %x %x\n", a, b, c, d);
    }
}
bool check_sse(){
    int a,b,c,d;
    __cpuid(1, a, b, c, d);
    return (d & (1 << 25));
}