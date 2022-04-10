#include <include/pci.h>
#include <include/stivale_tag.h>
#include <include/acpi.h>

#include <lib/assert.h>
#include <lib/printf.h>
#include <lib/string.h>
#include <stdint.h>
#include <thirdparty/stivale2.h>x`

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
}
