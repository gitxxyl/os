#include <include/port.h>
#include <include/interrupts.h>
#include <include/graphics.h>
#include <stdint.h>

// scancode : key map for US keyboard
unsigned char kbdus[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',		/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   15,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   15,					/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    127,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */};
    
unsigned char kbdusshift[128] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*', /* 9 */
  '(', ')', '_', '+', '\b', /* Backspace */
  '\t',         /* Tab */
  'Q', 'W', 'E', 'R',   /* 19 */
  'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n',     /* Enter key */
    0,          /* 29   - Control */
  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', /* 39 */
 '\"', '~',   0,        /* Left shift */
 '|', 'Z', 'X', 'C', 'V', 'B', 'N',            /* 49 */
  'M', '<', '>', '?',   0,                  /* Right shift */
  '*',
    0,  /* Alt */
  ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
  '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
  '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */};

uint8_t shift = 0;
uint8_t ctrl = 0;
uint8_t alt = 0;
uint8_t capslock = 0;

void keyboard_handler(registers_t* r){
    unsigned char scancode;
    scancode = inb(0x60);
    if (scancode == 0xE0){
      return;
    }

    if (scancode & 0x80)
    {
      switch(scancode){
        case 0xAA:
          shift = 0;
          break;
        case 0xB6:
          shift = 0;
          break;
        case 0x9D:
          ctrl = 0;
          break;
        case 0xB8:
          alt = 0;
          break;
        default:
          break;
      }
    }
    else
    {   
      switch(scancode){
        case 0x2A:
          shift = 1;
          break;
        case 0x36:
          shift = 1;
          break;
        case 0x1D:
          ctrl = 1;
          break;
        // case 0x1C:
        //   program();
        //   // alt = 1;
        //   break;
        default:
          if(shift){
            //key_handler(kbdusshift[scancode], scancode, shift, ctrl, alt);
            fb_printchar(kbdusshift[scancode]);
          } else {
            //key_handler(kbdus[scancode], scancode, shift, ctrl, alt);
            fb_printchar(kbdus[scancode]);
          }
          
      }
      
    }
}

void keyboard_init(){
    isr_install_handler(33, keyboard_handler);
}
