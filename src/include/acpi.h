#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef struct rsdp {
    char signature[8];
    uint8_t checksum;
    char oem_id[6];
    uint8_t revision;
    uint32_t rsdtAddress;
    uint32_t length;
    uint64_t xsdtAddress;
    uint8_t extended_checksum;
    uint8_t reserved[3];
}  __attribute__((packed)) rsdp_t;

typedef struct sdt {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oem_id[6];
    char oem_table_id[8];
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
}  __attribute__((packed)) sdt_t;


typedef struct mcfg {
    sdt_t sdt_header;
    uint64_t reserved;
} __attribute__((packed)) mcfg_t;

sdt_t* sdt_find(sdt_t* sdt, const char* signature);
bool sdt_checksum(sdt_t* sdt);