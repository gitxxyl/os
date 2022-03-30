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
#include <include/timer.h>
#include <include/init.h>
#include <include/vfs.h>
#include <include/initrd.h>


static uint8_t stack[8192];

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

extern void sse_init();
char* dtoa(float f);

void _start(struct stivale2_struct *stivale2_struct)
{
    serial_init();
    dprint("\n\n");
    graphics_init(stivale2_struct, 0xFF000000, 0xFFFFFFFF);
    fb_print("Framebuffer initialised.\n");
    fb_print("Serial port initialised.\n");
    

    interrupts_init();
    fb_print("Interrupts initialised.\n");
    keyboard_init();
    fb_print("Keyboard initialised.\n");
    timer_init(1000);
    fb_print("Timer initialised.\n");
    
    fb_print("\nMemory map information:\n");
    pmm_init(stivale2_struct);

    heap_init();
    fb_print("Heap initialised.\n");

    struct stivale2_struct_tag_modules *modules_tag = stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_MODULES_ID);
    assert(modules_tag != NULL);
    assert(modules_tag->module_count > 0);
    uint32_t* initrd_addr = modules_tag->modules[0].begin;
    dprintf("module %s, %x\n", modules_tag->modules[0].string, *initrd_addr);
    root = initrd_init(initrd_addr);
    dprintf("Initrd initialised.\n");


    printf_c(0xFF00FF00, "\n\n[REDACTED]OS v0.3 booted successfully on Limine v%s", stivale2_struct->bootloader_version);
    dprintf("\n\n[REDACTED]OS v0.3 booted successfully on Limine v%s\n", stivale2_struct->bootloader_version);

    fb_print("\nInterrupts enabled, system ready.\n"); 

    // No more proactive code from here, all initialization must be completed
    asm ("sti");
    
    // asm volatile ("int3"); // breakpoint exception for interrupt debugging

    for (;;)
    {
        asm("hlt");
    }
}
