#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct MMapEntry {
    uint32_t Size;
    uint32_t AddrL;
    uint32_t AddrH;
    uint32_t LengthL;
    uint32_t LengthH;

    #define MEMORY_AVAILABLE 1
    #define MEMORY_RESERVED  2
    uint32_t Type;
}__attribute__((packed)) MMapEntry_t;

typedef struct BuddyTree {
    uint8_t marked;      //0 = free, 1 = allocated, 2 = split
    size_t size;
    struct BuddyTree* A; //pointer to child
    struct BuddyTree* B; //pointer to other child
    
} BuddyTree_t;

typedef struct HeapHeader {
    uint8_t marked;
    size_t size;
} HeapHeader_t;

void k_mem_init();

//HeapHeader_t* k_mem_add_header(size_t addr, size_t size);
//HeapHeader_t* k_mem_next_header(HeapHeader_t* header);
//HeapHeader_t* k_mem_split_header(HeapHeader_t* header);

void* kmalloc(size_t size);
void  kfree(void* block);

MMapEntry_t* k_mem_map_entry(uint16_t entry);
MMapEntry_t* k_mem_map_entry_at(size_t addr, uint16_t entry);

void k_print_mem_map();
void k_print_mem_map_at(size_t addr, size_t length);
