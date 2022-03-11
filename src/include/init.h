#ifndef INIT_H
#define INIT_H

void serial_init();
void graphics_init(struct stivale2_struct *stivale2_struct, uint32_t bgcolor, uint32_t fgcolor);
void timer_init();
void pmm_init();
void interrupts_init();
void keyboard_init();

#endif // INIT_H