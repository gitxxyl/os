#include <include/pci.h>
#include <include/stivale_tag.h>
#include <include/acpi.h>
#include <include/memory.h>

#include <lib/assert.h>
#include <lib/printf.h>
#include <lib/string.h>
#include <stdint.h>
#include <thirdparty/stivale2.h>

extern uint64_t* k_pml4;

void pci_init(struct stivale2_struct* stivale2_struct){
    // get and ensure validity of rsdp from bootloader
    struct stivale2_struct_tag_rsdp* rsdp_struct = stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_RSDP_ID);
    assert(rsdp_struct != NULL);
    rsdp_t* rsdp = rsdp_struct->rsdp;

    // rsdp checks
    assert(rsdp != NULL);
    assert(strcmp(rsdp->signature, "RSD PTR ") != 0);
    dprintf("RSDP found at %p with revision %d and oemID %s\n", rsdp, rsdp->revision, rsdp->oem_id);

    sdt_t* xsdt = (sdt_t*)(rsdp->xsdtAddress);
    assert(xsdt != NULL);
    
    mcfg_t* mcfg = (mcfg_t*) sdt_find(xsdt, "MCFG");
    assert(mcfg != NULL);
    for(int i = 0; i < 4; i++) dprintf("%c", mcfg->sdt_header.signature[i]);
    pci_enumerate(mcfg);
}

void pci_enumerate(mcfg_t* mcfg){
    uint64_t entries = (mcfg->sdt_header.length - sizeof(mcfg_t)) / sizeof(pci_config_struct_t);
    for(uint64_t i = 0; i < entries; i++){
        pci_config_struct_t* pci_config = (pci_config_struct_t*)((uint64_t)mcfg + (i * sizeof(pci_config_struct_t)) + sizeof(mcfg_t));
        for(int bus = pci_config->start_bus_num; bus <= pci_config->end_bus_num; bus++){
            pci_enumerate_bus(pci_config->base_addr, bus);
        }
    }
}

void pci_enumerate_bus(uint64_t base_addr, uint8_t bus){
    uint64_t offset = bus << 20;
    uint64_t bus_addr = base_addr + offset;

    // PCI bus uses MMIO - needs to be mapped in the pmm
    vmm_map_page(k_pml4, bus_addr, bus_addr, 0b11);

    pci_device_header_t* device_header = (pci_device_header_t*)bus_addr;
    if(device_header->vendor_id == 0xFFFF || device_header->device_id == 0xFFFF || device_header->device_id == 0x0) return;

    for(uint64_t device = 0; device < 32; device++){
        pci_enumerate_device(bus_addr, device);
    }
}

void pci_enumerate_device(uint64_t bus_addr, uint8_t device){
    uint64_t offset = device << 15;
    uint64_t device_addr = bus_addr + offset;

    // PCI bus uses MMIO - needs to be mapped in the pmm
    vmm_map_page(k_pml4, device_addr, device_addr, 0b11);

    pci_device_header_t* device_header = (pci_device_header_t*)bus_addr;
    if(device_header->vendor_id == 0xFFFF || device_header->device_id == 0xFFFF || device_header->device_id == 0x0) return;

    for(uint64_t function = 0; function < 8; function++){
        pci_enumerate_function(device_addr, function);
    }
}

void pci_enumerate_function(uint64_t device_addr, uint8_t function){
    uint64_t offset = function << 12;
    uint64_t function_addr = device_addr + offset;

    // PCI bus uses MMIO - needs to be mapped in the pmm
    vmm_map_page(k_pml4, device_addr, device_addr, 0b11);

    pci_device_header_t* device_header = (pci_device_header_t*)device_addr;
    if(device_header->vendor_id == 0xFFFF || device_header->device_id == 0xFFFF || device_header->device_id == 0x0) return;

    dprintf("vendor: %x, device: %x\n", device_header->vendor_id, device_header->device_id);
}
