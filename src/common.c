#include <stdint.h>
#include <stddef.h>

uint32_t strlen(const char* str){
    uint32_t len = 0;
    while(str[len] != '\0'){
        len++;
    }
    return len;
}

char* strrev(char* str){
    uint32_t start = 0;
    uint32_t end = strlen(str) - 1;
    while(start < end){
        char tmp = str[start];
        str[start] = str[end];
        str[end] = tmp;
        start++;
        end--;
    }
    return str;
}

char* itoa(uint64_t num, uint64_t base) {
  static char hold[] = "0123456789ABCDEFGHIJKLMNOPQRTSUVWXYZ";
  static char buffer[50];
  char *str;

  str = &buffer[49];
  *str = '\0';

  do {
    *--str = hold[num % base];
    num /= base;
  } while (num != 0);

  return str;
}