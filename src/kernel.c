#include <stdint.h>
#include <stddef.h>
#include <stivale2.h>
#include <include/common.h>
#include <lib/math.h>
#include <cpuid.h>
#include <lib/printf.h>
// We need to tell the stivale bootloader where we want our stack to be.
// We are going to allocate our stack as an array in .bss.
static uint8_t stack[8192];

// stivale2 uses a linked list of tags for both communicating TO the
// bootloader, or receiving info FROM it. More information about these tags
// is found in the stivale2 specification.

// stivale2 offers a runtime terminal service which can be ditched at any
// time, but it provides an easy way to print out to graphical terminal,
// especially during early boot.
// Read the notes about the requirements for using this feature below this
// code block.

static struct stivale2_tag paging_hdr_tag = {
    .identifier = STIVALE2_HEADER_TAG_5LV_PAGING_ID,
    .next = 0
};
static struct stivale2_header_tag_terminal terminal_hdr_tag = {
    // All tags need to begin with an identifier and a pointer to the next tag.
    .tag = {
        // Identification constant defined in stivale2.h and the specification.
        .identifier = STIVALE2_HEADER_TAG_TERMINAL_ID,
        // If next is 0, it marks the end of the linked list of header tags.
        .next = (uint64_t)&paging_hdr_tag},
    // The terminal header tag possesses a flags field, leave it as 0 for now
    // as it is unused.
    .flags = 0};

// We are now going to define a framebuffer header tag.
// This tag tells the bootloader that we want a graphical framebuffer instead
// of a CGA-compatible text mode. Omitting this tag will make the bootloader
// default to text mode, if available.
static struct stivale2_header_tag_framebuffer framebuffer_hdr_tag = {
    // Same as above.
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
        // Instead of 0, we now point to the previous header tag. The order in
        // which header tags are linked does not matter.
        .next = (uint64_t)&terminal_hdr_tag},
    // We set all the framebuffer specifics to 0 as we want the bootloader
    // to pick the best it can.
    .framebuffer_width = 0,
    .framebuffer_height = 0,
    .framebuffer_bpp = 0};

// The stivale2 specification says we need to define a "header structure".
// This structure needs to reside in the .stivale2hdr ELF section in order
// for the bootloader to find it. We use this __attribute__ directive to
// tell the compiler to put the following structure in said section.
__attribute__((section(".stivale2hdr"), used)) static struct stivale2_header stivale_hdr = {
    // The entry_point member is used to specify an alternative entry
    // point that the bootloader should jump to instead of the executable's
    // ELF entry point. We do not care about that so we leave it zeroed.
    .entry_point = 0,
    // Let's tell the bootloader where our stack is.
    // We need to add the sizeof(stack) since in x86(_64) the stack grows
    // downwards.
    .stack = (uintptr_t)stack + sizeof(stack),
    // Bit 1, if set, causes the bootloader to return to us pointers in the
    // higher half, which we likely want since this is a higher half kernel.
    // Bit 2, if set, tells the bootloader to enable protected memory ranges,
    // that is, to respect the ELF PHDR mandated permissions for the executable's
    // segments.
    // Bit 3, if set, enables fully virtual kernel mappings, which we want as
    // they allow the bootloader to pick whichever *physical* memory address is
    // available to load the kernel, rather than relying on us telling it where
    // to load it.
    // Bit 4 disables a deprecated feature and should always be set.
    .flags = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4),
    // This header structure is the root of the linked list of header tags and
    // points to the first one in the linked list.
    .tags = (uintptr_t)&framebuffer_hdr_tag};

// We will now write a helper function which will allow us to scan for tags
// that we want FROM the bootloader (structure tags).
void *stivale2_get_tag(struct stivale2_struct *stivale2_struct, uint64_t id)
{
    struct stivale2_tag *current_tag = (void*) stivale2_struct->tags;
    for (;;)
    {
        // If the tag pointer is NULL (end of linked list), we did not find
        // the tag. Return NULL to signal this.
        if (current_tag == NULL)
        {
            return NULL;
        }

        // Check whether the identifier matches. If it does, return a pointer
        // to the matching tag.
        if (current_tag->identifier == id)
        {
            return current_tag;
        }

        // Get a pointer to the next tag in the linked list and repeat.
        current_tag = (void*) current_tag->next;
    }
}

extern void sse_init();
char* dtoa(float f);

// The following will be our kernel's entry point.
void _start(struct stivale2_struct *stivale2_struct)
{
    serial_init();
    sprint("\n\n");
    graphics_init(stivale2_struct, 0xFF000000, 0xFFFFFFFF);
    fb_print("Framebuffer initialised.\n");
    fb_print("Serial port initialised.\n");
    interrupts_init();
    fb_print("Interrupts initialised.\n");
    keyboard_init();
    fb_print("Keyboard initialised.\n");
    timer_init(1000);
    fb_print("Timer initialised.\n");
    
    sprint("\n\n[REDACTED]OS v0.2 booted successfully on Limine v");
    sprint(stivale2_struct->bootloader_version);
    sprint("\n");

    sprint(itoa(pow(2,2), 10));

    fb_print("\nMemory map information:\n");
    pmm_init(stivale2_struct);

    get_cpuid();
    fb_print("\nSSE: ");
    if(check_sse()){
        fb_print("Present\n");
        sse_init();
    } else {
        fb_print("Not present\n");
    }
    fb_print_color("\n\n[REDACTED]OS v0.2 booted successfully on Limine v", 0x00FF00);
    fb_print_color(stivale2_struct->bootloader_version, 0x00FF00);
    
    fb_print("\nInterrupts enabled, system ready.\n"); 
    // No more proactive code from here, all initialization must be completed
    asm ("sti");
    
    // asm volatile ("int3"); // breakpoint exception for interrupt debugging

    for (;;)
    {
        asm("hlt");
    }
}
