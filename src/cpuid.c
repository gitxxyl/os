#include <cpuid.h>
#include <lib/string.h>
#include <stdbool.h>
#include <include/serial.h>

void get_cpuid(){
    int a, b, c, d;
    for(int i = 0; i < 5; i++){
        sprint("[CPUID] ");
        sprint(itoa(i, 10));
        sprint(": ");
        __cpuid(i, a, b, c, d);
        sprint(itoa(a, 2));
        sprint(" ");
        sprint(itoa(b, 2));
        sprint(" ");
        sprint(itoa(c, 2));
        sprint(" ");
        sprint(itoa(d, 2));
        sprint("\n");
    }
}
bool check_sse(){
    int a,b,c,d;
    __cpuid(1, a, b, c, d);
    return (d & (1 << 25));
}