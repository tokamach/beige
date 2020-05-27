#include "kgdt.h"

#include "kterm.h"


void gdt_add_entry(uint16_t index, uint32_t base, uint32_t limit, uint8_t ring, uint8_t type)
{
    gdt_entry_t* tmp = &gdt[index];

    tmp->limit0_15 = limit & 0xFFFF;
    tmp->limit16_19_flags = (limit >> 16) & 0b111; //upper limit
    
    /* Flags (4kb Segments, Protected Mode) */
    tmp->limit16_19_flags |= (0x01 << 7) | (0x01 << 6);

    tmp->base0_15  = base & 0xFFFF;
    tmp->base16_23 = (base >> 16) & 0xFF;
    tmp->base24_31 = (base >> 24) & 0xFF;

    tmp->access = 0;
    tmp->access |= 0x1 << 7; // Present bit
    tmp->access |= (ring & 0b11) << 5; // Privelege level
    tmp->access |= (0x1 << 4); // Descriptor type (set 1 for code and data)
    tmp->access |= type & 0b1111; // Access bits (Executable, Direction, R/W, Accessed)
}

/*
 * While we won't use actual segmentation for memory protection, we still
 * need to initialize a flat gdt, otherwise we'll hit errors in Bochs. 
 * (and compliancy is good)
 */
void k_gdt_init()
{
    gdt_ptr_t gdt_ptr;
    gdt_ptr.size = sizeof(gdt_entry_t) * GDT_ENTRIES;
    gdt_ptr.base = (uint32_t) &gdt;

    // The Null Entry
    gdt_add_entry(0, SEG_BASE, SEG_LIMIT, 0, 0);

    // Kernel Code and Data
    gdt_add_entry(1, SEG_BASE, SEG_LIMIT, 0, CODE_TYPE);
    gdt_add_entry(2, SEG_BASE, SEG_LIMIT, 0, DATA_TYPE);

    // User Code and Data
    //gdt_add_entry(3, SEG_BASE, SEG_LIMIT, 3, CODE_TYPE);
    //gdt_add_entry(4, SEG_BASE, SEG_LIMIT, 3, DATA_TYPE);

    //TODO: add TSS
    
    load_gdt((uint32_t)&gdt_ptr);

    k_println("gdt initalized");
}
