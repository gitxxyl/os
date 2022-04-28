#include <lib/printf.h>
#include <include/graphics.h>

void __panic(const char* msg, const char* file, int line){
    printf_c(RED, "KERNEL PANIC: %s, file %s, line %d\n", msg, file, line);
    dprintf("KERNEL PANIC: %s, file %s, line %d\n", msg, file, line);
    for(;;);
}
void __assert(const char* msg, const char* file, int line){
    printf_c(RED, "ASSERTION FAILED: %s, file %s, line %d\n", msg, file, line);
    dprintf("ASSERTION FAILED: %s, file %s, line %d\n", msg, file, line);
    for(;;);
}