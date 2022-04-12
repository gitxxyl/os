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
mcfg_t* mcfg = 0;

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
    
    mcfg = (mcfg_t*) sdt_find(xsdt, "MCFG");
    assert(mcfg != NULL);
    for(int i = 0; i < 4; i++) dprintf("%c", mcfg->sdt_header.signature[i]);
}

pci_device_header_t* pci_enumerate(mcfg_t* a_mcfg, uint16_t a_vendorid, uint16_t a_deviceid){
    if(a_mcfg == NULL) a_mcfg = mcfg;
    uint64_t entries = (a_mcfg->sdt_header.length - sizeof(mcfg_t)) / sizeof(pci_config_struct_t);
    for(uint64_t i = 0; i < entries; i++){
        pci_config_struct_t* pci_config = (pci_config_struct_t*)((uint64_t)a_mcfg + (i * sizeof(pci_config_struct_t)) + sizeof(mcfg_t));
        for(int bus = pci_config->start_bus_num; bus <= pci_config->end_bus_num; bus++){
            pci_device_header_t* ret_hdr = pci_enumerate_bus(pci_config->base_addr, bus, a_vendorid, a_deviceid);
            if(ret_hdr != NULL) return ret_hdr;
        }
    }
}

pci_device_header_t* pci_enumerate_bus(uint64_t base_addr, uint8_t bus, uint16_t a_vendorid, uint16_t a_deviceid){
    uint64_t offset = bus << 20;
    uint64_t bus_addr = base_addr + offset;

    // PCI bus uses MMIO - needs to be mapped 
    vmm_map_page(k_pml4, bus_addr, bus_addr, 0b11);

    pci_device_header_t* device_header = (pci_device_header_t*)bus_addr;
    if(device_header->vendor_id == 0xFFFF || device_header->device_id == 0xFFFF || device_header->device_id == 0x0) return NULL;

    for(uint64_t device = 0; device < 32; device++){
        pci_device_header_t* ret_hdr = pci_enumerate_device(bus_addr, device, a_vendorid, a_deviceid);
        if(ret_hdr != NULL) return ret_hdr;
    }
}

pci_device_header_t* pci_enumerate_device(uint64_t bus_addr, uint8_t device, uint16_t a_vendorid, uint16_t a_deviceid){
    uint64_t offset = device << 15;
    uint64_t device_addr = bus_addr + offset;

    vmm_map_page(k_pml4, device_addr, device_addr, 0b11);

    pci_device_header_t* device_header = (pci_device_header_t*)device_addr;
    if(device_header->vendor_id == 0xFFFF || device_header->device_id == 0xFFFF || device_header->device_id == 0x0) return NULL;

    for(uint64_t function = 0; function < 8; function++){
        pci_device_header_t* func_header = pci_enumerate_function(device_addr, function, a_vendorid, a_deviceid);
        if(func_header != NULL) return func_header;
    }
}

pci_device_header_t* pci_enumerate_function(uint64_t device_addr, uint8_t function, uint16_t a_vendorid, uint16_t a_deviceid){
    uint64_t offset = function << 12;
    uint64_t function_addr = device_addr + offset;

    vmm_map_page(k_pml4, function_addr, function, 0b11);

    pci_device_header_t* device_header = (pci_device_header_t*)function_addr;
    if(device_header->vendor_id == 0xFFFF || device_header->device_id == 0xFFFF || device_header->device_id == 0x0) return NULL;
    printf("PCI DEVICE: %x:%x / %s / %s / %s / %s / %s\n",
        device_header->vendor_id,
        device_header->device_id,
        GetVendorName(device_header->vendor_id),
        GetDeviceName(device_header->vendor_id, device_header->device_id),
        DeviceClasses[device_header->class],
        GetSubclassName(device_header->class, device_header->subclass),
        GetProgIFName(device_header->class, device_header->subclass, device_header->progif));
    if(device_header->vendor_id == a_vendorid && device_header->device_id == a_deviceid) return device_header;
    else return NULL;
    
}

/**
 * Get PCI descriptor names from IDs given.
 * Taken from PonchoOS - https://github.com/Absurdponcho/PonchoOS/tree/Episode-17-PCI-2/kernel/src
 **/

const char* DeviceClasses[] = {
    "Unclassified",
    "Mass Storage Controller",
    "Network Controller",
    "Display Controller",
    "Multimedia Controller",
    "Memory Controller",
    "Bridge Device",
    "Simple Communication Controller",
    "Base System Peripheral",
    "Input Device Controller",
    "Docking Station", 
    "Processor",
    "Serial Bus Controller",
    "Wireless Controller",
    "Intelligent Controller",
    "Satellite Communication Controller",
    "Encryption Controller",
    "Signal Processing Controller",
    "Processing Accelerator",
    "Non Essential Instrumentation"
};

const char* GetVendorName(uint16_t vendorID){
    switch (vendorID){
        case 0x8086:
            return "Intel Corp";
        case 0x1022:
            return "AMD";
        case 0x10DE:
            return "NVIDIA Corporation";
    }
    return itoa(vendorID, 16);
}

const char* GetDeviceName(uint16_t vendorID, uint16_t deviceID){
    switch (vendorID){
        case 0x8086: // Intel
            switch(deviceID){
                case 0x29C0:
                    return "Express DRAM Controller";
                case 0x2918:
                    return "LPC Interface Controller";
                case 0x2922:
                    return "6 port SATA Controller [AHCI mode]";
                case 0x2930:
                    return "SMBus Controller";
            }
    }
    return itoa(deviceID, 16);
}

const char* MassStorageControllerSubclassName(uint8_t subclassCode){
    switch (subclassCode){
        case 0x00:
            return "SCSI Bus Controller";
        case 0x01:
            return "IDE Controller";
        case 0x02:
            return "Floppy Disk Controller";
        case 0x03:
            return "IPI Bus Controller";
        case 0x04:
            return "RAID Controller";
        case 0x05:
            return "ATA Controller";
        case 0x06:
            return "Serial ATA";
        case 0x07:
            return "Serial Attached SCSI";
        case 0x08:
            return "Non-Volatile Memory Controller";
        case 0x80:
            return "Other";
    }
    return itoa(subclassCode, 16);
}

const char* SerialBusControllerSubclassName(uint8_t subclassCode){
    switch (subclassCode){
        case 0x00:
            return "FireWire (IEEE 1394) Controller";
        case 0x01:
            return "ACCESS Bus";
        case 0x02:
            return "SSA";
        case 0x03:
            return "USB Controller";
        case 0x04:
            return "Fibre Channel";
        case 0x05:
            return "SMBus";
        case 0x06:
            return "Infiniband";
        case 0x07:
            return "IPMI Interface";
        case 0x08:
            return "SERCOS Interface (IEC 61491)";
        case 0x09:
            return "CANbus";
        case 0x80:
            return "SerialBusController - Other";
    }
    return itoa(subclassCode, 16);
}

const char* BridgeDeviceSubclassName(uint8_t subclassCode){
    switch (subclassCode){
        case 0x00:
            return "Host Bridge";
        case 0x01:
            return "ISA Bridge";
        case 0x02:
            return "EISA Bridge";
        case 0x03:
            return "MCA Bridge";
        case 0x04:
            return "PCI-to-PCI Bridge";
        case 0x05:
            return "PCMCIA Bridge";
        case 0x06:
            return "NuBus Bridge";
        case 0x07:
            return "CardBus Bridge";
        case 0x08:
            return "RACEway Bridge";
        case 0x09:
            return "PCI-to-PCI Bridge";
        case 0x0a:
            return "InfiniBand-to-PCI Host Bridge";
        case 0x80:
            return "Other";
    }
    return itoa(subclassCode, 16);
}

const char* GetSubclassName(uint8_t classCode, uint8_t subclassCode){
    switch (classCode){
        case 0x01:
            return MassStorageControllerSubclassName(subclassCode);
        case 0x03:
            switch (subclassCode){
                case 0x00:
                    return "VGA Compatible Controller";
            }
        case 0x06:
            return BridgeDeviceSubclassName(subclassCode);
        case 0x0C:
            return SerialBusControllerSubclassName(subclassCode);
    }
    return itoa(subclassCode, 16);
}

const char* GetProgIFName(uint8_t classCode, uint8_t subclassCode, uint8_t progIF){
    switch (classCode){
        case 0x01:
            switch (subclassCode){
                case 0x06:
                    switch (progIF){
                        case 0x00:
                            return "Vendor Specific Interface";
                        case 0x01:
                            return "AHCI 1.0";
                        case 0x02:
                            return "Serial Storage Bus";
                    }
            }
        case 0x03:
            switch (subclassCode){
                case 0x00:
                    switch (progIF){
                        case 0x00:
                            return "VGA Controller";
                        case 0x01:
                            return "8514-Compatible Controller";
                    }
            }
        case 0x0C:
            switch (subclassCode){
                case 0x03:
                    switch (progIF){
                        case 0x00:
                            return "UHCI Controller";
                        case 0x10:
                            return "OHCI Controller";
                        case 0x20:
                            return "EHCI (USB2) Controller";
                        case 0x30:
                            return "XHCI (USB3) Controller";
                        case 0x80:
                            return "Unspecified";
                        case 0xFE:
                            return "USB Device (Not a Host Controller)";
                    }
            }    
    }
    return itoa(progIF, 16);
}