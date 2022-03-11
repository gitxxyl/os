#include <stdint.h>
#include <stddef.h>
#include <stivale2.h>


void (*term_write)(const char *string, size_t length);
void vga_init(struct stivale2_struct* st_struct){
    struct stivale2_struct_tag_terminal *term_str_tag;
    term_str_tag = stivale2_get_tag(st_struct, STIVALE2_STRUCT_TAG_TERMINAL_ID);

    if (term_str_tag == NULL) { // check if term tag is there
        for (;;) {
            asm ("hlt");
        }
    }
    void *term_write_ptr = (void *)term_str_tag->term_write;
    term_write = term_write_ptr;
}

void kprint(const char* str){
    term_write(str, strlen(str)+1);
}

void kprint_dec(uint64_t value){
    kprint(itoa(value, 10));
}

void kprint_hex(uint64_t value){
    kprint(itoa(value, 16));
}