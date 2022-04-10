#pragma once 

void serial_init();
void graphics_init(struct stivale2_struct *stivale2_struct, uint32_t bgcolor, uint32_t fgcolor);
void timer_init();
void pmm_init(struct stivale2_struct* stivale2_struct);
void interrupts_init();
void keyboard_init();
void vmm_init(struct stivale2_struct* stivale2_struct);
void heap_init();
void pci_init(struct stivale2_struct* stivale2_struct);
