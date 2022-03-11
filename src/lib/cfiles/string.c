#include <stdint.h>
#include <lib/memory.h>

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

char* dtoa(float f){
    char result[100];
    int dVal, dec, i;

    f += 0.005;   // added after a comment from Matt McNabb, see below.

    dVal = f;
    dec = (int)(f * 100) % 100;

    memset(result, 0, 100);
    result[0] = (dec % 10) + '0';
    result[1] = (dec / 10) + '0';
    result[2] = '.';

    i = 3;
    while (dVal > 0)
    {
        result[i] = (dVal % 10) + '0';
        dVal /= 10;
        i++;
    }
    return result;
}