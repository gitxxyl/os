#include <include/acpi.h>
#include <stdint.h>
#include <lib/printf.h>
#include <stddef.h>

bool sdt_checksum(sdt_t* sdt) {
    uint8_t sum = 0;
    for (int i = 0; i < sdt->length; i++) {
        sum += ((uint8_t*)sdt)[i];
    }
    return sum == 0;
}

sdt_t* sdt_find(sdt_t* sdt, const char* signature) {
    uint64_t entries = (sdt->length - sizeof(sdt_t)) / sizeof(uint64_t);
    for(uint64_t i = 0; i < entries; i++){
        sdt_t* new_sdt = (sdt_t*)*(uint64_t*)((uint64_t)sdt + (i * 8) + sizeof(sdt_t));
        for(int j = 0; j < 4; j++){
            if(new_sdt->signature[j] != signature[j]) continue;
            if(j == 3) return new_sdt;
        }
    }
    return NULL;
}
