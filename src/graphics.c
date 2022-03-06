#include <stivale2.h>
#include <stdint.h>
#include <stddef.h>
#include <include/framebuffer.h>
#include <include/common.h>
#define SSFN_CONSOLEBITMAP_TRUECOLOR   
#include <ssfn.h>

#define CHARACTER_WIDTH 8
#define CHARACTER_HEIGHT 16 

uint64_t framebuffer_addr;
struct fb_struct fb;

extern uint8_t _binary_sfn_fonts_unifont_sfn_start;

void graphics_init(struct stivale2_struct *stivale2_struct, uint32_t bgcolor, uint32_t fgcolor){
    struct stivale2_struct_tag_framebuffer* framebuffer_tag;
    framebuffer_tag = stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);
    if (framebuffer_tag == NULL) { // check if framebuffer tag is there
        for (;;) {
            asm ("hlt");
        }
    }
    sprint("Framebuffer found at 0x");
    sprint(itoa(framebuffer_tag->framebuffer_addr, 16));

    fb.addr = framebuffer_tag->framebuffer_addr;
    fb.width = framebuffer_tag->framebuffer_width;
    fb.height = framebuffer_tag->framebuffer_height;
    fb.bpp = framebuffer_tag->framebuffer_bpp;
    fb.pitch = framebuffer_tag->framebuffer_pitch;

    ssfn_src = (ssfn_font_t*) &_binary_sfn_fonts_unifont_sfn_start;
    ssfn_dst.ptr = (uint8_t*) fb.addr;
    ssfn_dst.w = fb.width;
    ssfn_dst.h = fb.height;
    ssfn_dst.p = fb.pitch;
    ssfn_dst.x = 0;
    ssfn_dst.y = 0;


    fb_changebg(bgcolor);
    ssfn_dst.fg = fgcolor;

    // fb_print("(Black screen with text; The sound of buzzing bees can be heard) Narrator: According to all known laws of aviation, there is no way a bee should be able to fly. Its wings are too small to get its fat little body off the ground. The bee, of course, flies anyway because bees don't care what humans think is impossible. (Barry is picking out a shirt) Barry: Yellow, black. Yellow, black. Yellow, black. Yellow, black. Ooh, black and yellow! Let's shake it up a little. Janet: Barry! Breakfast is ready! Barry: Coming! Hang on a second. (Barry uses his antenna like a phone) Barry: Hello (Through phone) Adam: Barry? Barry: Adam? Adam: Can you believe this is happening? Barry: I can't. I'll pick you up. (Barry flies down the stairs) Martin: Looking sharp. Janet: Use the stairs. Your father paid good money for those. Barry: Sorry. I'm excited.");
}

void fb_plotpixel(int32_t x, int32_t y, uint32_t color){
    size_t fb_index = y * (fb.pitch / 4) + x;
    uint32_t* fb_ptr = (uint32_t*) fb.addr;
    fb_ptr[fb_index] = color;
}

void fb_changebg(uint32_t color){
    ssfn_dst.bg = color;
    for (int y = 0; y < fb.height; y++){
        for(int x = 0; x < fb.width; x++){
            fb_plotpixel(x,y,color);
        }
    }
}

void fb_printchar(char c){
    switch(c){
        case '\n':
            ssfn_dst.x = 0;
            ssfn_dst.y += CHARACTER_HEIGHT;
            break;
        case '\b':
            if(ssfn_dst.x == 0 && ssfn_dst.y == 0) break;
            if(ssfn_dst.x > 0) ssfn_dst.x -= CHARACTER_WIDTH;
            else{
                ssfn_dst.x = fb.width - CHARACTER_WIDTH;
                ssfn_dst.y -= CHARACTER_HEIGHT;
            }
            ssfn_putc(' ');
            if(ssfn_dst.x == 0 && ssfn_dst.y == 0) break;
            if(ssfn_dst.x > 0) ssfn_dst.x -= CHARACTER_WIDTH;
            else{
                ssfn_dst.x = fb.width - CHARACTER_WIDTH;
                ssfn_dst.y -= CHARACTER_HEIGHT;
            }
            break;
        default:
            ssfn_putc(c);
            if(ssfn_dst.x >= fb.width){
                ssfn_dst.y += CHARACTER_HEIGHT;
                ssfn_dst.x = 0;
            }
            break;
    }
}
void fb_print(char* str){
    for(int i = 0; i < strlen(str); i++){
        fb_printchar(str[i]);
    }   
}

void fb_print_color(char* str, uint32_t color){
    uint32_t tmp = ssfn_dst.fg;
    ssfn_dst.fg = color;
    fb_print(str);
    ssfn_dst.fg = tmp;
}

void fb_lgbt(){
    // fb_changebg(0xFFFFFFFF);
    int y = 0;
    for(; y < (fb.height / 6); y++){
        for(int x = 0; x < fb.width; x++){
            fb_plotpixel(x, y, 0xFFFF0018);
        }
    }
    for(; y < (2 * (fb.height / 6)); y++){
        for(int x = 0; x < fb.width; x++){
            fb_plotpixel(x, y, 0xFFFFA52C);
        }
    }
    for(; y < (3 * (fb.height / 6)); y++){
        for(int x = 0; x < fb.width; x++){
            fb_plotpixel(x, y, 0xFFFFFF41);
        }
    }
    for(; y < (4 * (fb.height / 6)); y++){
        for(int x = 0; x < fb.width; x++){
            fb_plotpixel(x, y, 0xFF008018);
        }
    }
    for(; y < (5 * (fb.height / 6)); y++){
        for(int x = 0; x < fb.width; x++){
            fb_plotpixel(x, y, 0xFF0000F9);
        }
    }
    for(; y < (6 * (fb.height / 6)); y++){
        for(int x = 0; x < fb.width; x++){
            fb_plotpixel(x, y, 0xFF86007D);
        }
    }
}