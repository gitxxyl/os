#include <lib/printf.h>
#include <include/graphics.h>
void __assert(const char* msg, const char* file, int line){
    printf_c(0xFF0000, "ASSERTION FAILED: %s, file %s, line %d\n", msg, file, line);
    dprintf("ASSERTION FAILED: %s, file %s, line %d\n", msg, file, line);
    for(;;);
}