#include <stdint.h>
#include <stddef.h>
#include <cpuid.h>

#include <lib/string.h>
#include <lib/math.h>
#include <lib/printf.h>
#include <lib/alloc.h>
#include <lib/assert.h>
#include <lib/ordered_array.h>

#include <thirdparty/stivale2.h>

#include <include/memory.h>
#include <include/cpuid.h>
#include <include/graphics.h>
#include <include/stivale_tag.h>
#include <include/timer.h>
#include <include/init.h>
#include <include/vfs.h>
#include <include/targa.h>
#include <include/initrd.h>


static uint8_t stack[8192];
char* wpaper;

// static struct stivale2_tag paging_hdr_tag = {
//     .identifier = STIVALE2_HEADER_TAG_5LV_PAGING_ID,
//     .next = 0
// };
static struct stivale2_header_tag_terminal terminal_hdr_tag = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_TERMINAL_ID,
        .next = 0},
    .flags = 0};

static struct stivale2_header_tag_framebuffer framebuffer_hdr_tag = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
        .next = (uint64_t)&terminal_hdr_tag},
    .framebuffer_width = 0,
    .framebuffer_height = 0,
    .framebuffer_bpp = 0};

__attribute__((section(".stivale2hdr"), used)) static struct stivale2_header stivale_hdr = {
    .entry_point = 0,
    .stack = (uintptr_t)stack + sizeof(stack),
    .flags = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4),
    .tags = (uintptr_t)&framebuffer_hdr_tag};

void* stivale2_get_tag(struct stivale2_struct *stivale2_struct, uint64_t id)
{
    struct stivale2_tag *current_tag = (void*) stivale2_struct->tags;
    for (;;)
    {
        if (current_tag == NULL)
        {
            return NULL;
        }
        if (current_tag->identifier == id)
        {
            return current_tag;
        }
        current_tag = (void*) current_tag->next;
    }
}

void _start(struct stivale2_struct *stivale2_struct)
{
    serial_init();
    dprint("\n\n");
    graphics_init(stivale2_struct, 0xFF000000, 0xFFFFFFFF);
    printf_c(GREEN, "Framebuffer initialised.\n");
    printf_c(GREEN, "Serial port initialised.\n");
    

    interrupts_init();
    printf_c(GREEN, "Interrupts initialised.\n");
    keyboard_init();
    printf_c(GREEN, "Keyboard initialised.\n");
    timer_init(1000);
    printf_c(GREEN, "Timer initialised.\n");
    
    pmm_init(stivale2_struct);
    printf_c(GREEN, "Physical memory manager initialised.\n");
    vmm_init(stivale2_struct);
    printf_c(GREEN, "Virtual memory manager initialised.\n");
    heap_init();
    printf_c(GREEN, "Heap initialised.\n");

    pci_init(stivale2_struct);

    struct stivale2_struct_tag_modules *modules_tag = stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_MODULES_ID);
    assert(modules_tag != NULL);
    assert(modules_tag->module_count > 0);
    uint32_t* initrd_addr = modules_tag->modules[0].begin;
    dprintf("module %s, %x\n", modules_tag->modules[0].string, *initrd_addr);
    root = initrd_init(initrd_addr);
    dprintf("Initrd initialised.\n");

    shell_init(stivale2_struct);
    dprintf("\n\n[REDACTED]OS v0.3 booted successfully on Limine v%s\n", stivale2_struct->bootloader_version);


    // No more proactive code from here, all initialization must be completed
    asm("sti");
    
    // asm volatile ("int3"); // breakpoint exception for interrupt debugging
    for (;;)
    {
        asm("hlt");
    }
}
