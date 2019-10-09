#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct m_map_entry {
    uint32_t Size;
    uint32_t AddrL;
    uint32_t AddrH;
    uint32_t LengthL;
    uint32_t LengthH;

    #define MEMORY_AVAILABLE 1
    #define MEMORY_RESERVED  2
    uint32_t Type;
}__attribute__((packed)) m_map_entry_t;

//TODO: move to globals
m_map_entry_t* working_mem_region;
size_t mem_region_start;
size_t mem_region_size;
size_t mem_region_end;

void k_mem_init();
m_map_entry_t* k_mem_map_entry(uint16_t entry);
void k_print_mem_map();

void k_segment_init();
void k_paging_init();
