#pragma once
#include <include/acpi.h>
#include <stdint.h>

typedef struct pci_config_struct {
    uint64_t base_addr;
    uint16_t segment_grp_num;
    uint8_t start_bus_num;
    uint8_t end_bus_num;
    uint32_t reserved;
}__attribute__((packed)) pci_config_struct_t;

typedef struct pci_device_header {
    uint16_t vendor_id;
    uint16_t device_id;
    uint16_t command;
    uint16_t status;
    uint8_t  revision_id;
    uint8_t  progif;
    uint8_t  subclass;
    uint8_t  class;
    uint8_t  cache_line_size;
    uint8_t  latency_timer;
    uint8_t  header_type;
    uint8_t  BIST;
}__attribute__((packed)) pci_device_header_t;

void pci_enumerate(mcfg_t* mcfg);

void pci_enumerate_bus(uint64_t base_addr, uint8_t bus);

void pci_enumerate_device(uint64_t bus_addr, uint8_t device);

void pci_enumerate_function(uint64_t device_addr, uint8_t function);