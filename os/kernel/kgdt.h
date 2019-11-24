#pragma once

#include <stdint.h>

/*
 * Since we're going to be using paging instead, we'll set up
 * a flat segmentation model where each segment is 0-0xfffff
 * 
 */ 
#define SEG_BASE 0x0
#define SEG_LIMIT 0xFFFFF

#define CODE_TYPE 0xA // (Executable, Grows down, Readable, Not accessed)
#define DATA_TYPE 0x2 // (Non Executable, Grows down, Writeable, No accessed)

/*
 * An entry in the gdt, follows a very weird structure.
 * See: https://wiki.osdev.org/GDT
 */ 
typedef struct gdt_entry {
    uint16_t limit0_15;
    uint16_t base0_15;
    uint8_t  base16_23;
    uint8_t  access;
    uint8_t  limit16_19_flags; //4 upper bits of limit + flags (horrible)
    uint8_t  base24_31;
    
}__attribute__((packed)) gdt_entry_t;

typedef struct gdt_ptr {
    uint16_t size;
    uint32_t base;
}__attribute__((packed)) gdt_ptr_t;

#define GDT_ENTRIES 3
static gdt_entry_t gdt[GDT_ENTRIES];

void k_gdt_init();

// from kgdt.s
void load_gdt(uint32_t descriptor_location);
