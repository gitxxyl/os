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
    graphics_init(stivale2_struct, 0xFF000000, 0xFFFFFFFF);
    serial_init();
    interrupts_init();
    keyboard_init();
    timer_init(1000);
    pmm_init(stivale2_struct);
    vmm_init(stivale2_struct);
    heap_init();
    pci_init(stivale2_struct);
    root = initrd_init(stivale2_struct);
    shell_init(stivale2_struct);
    dprintf("\n\n[REDACTED]OS v0.3 booted successfully on Limine v%s\n", stivale2_struct->bootloader_version);

    asm("sti");
    for (;;) asm("hlt");
}
