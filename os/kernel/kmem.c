#include <stddef.h>
#include <stdint.h>

#include "kmem.h"

typedef struct MMapEntry {
    uint32_t BaseL;
    uint32_t BaseH;
    uint32_t LengthL;
    uint32_t LengthH;
    uint32_t Type;
}__attribute__((packed)) MMapEntry_t;

size_t k_query_low_mem()
{
    //Detect low memory
    size_t low_mem_size;
    asm(
	".code16gcc;"
	"push %%eax;"
	"clc;"
	"int $0x12;"
	"mov %%eax, %0;"
	"pop %%eax"
	: "=r"(low_mem_size));

    return low_mem_size;
}

size_t k_query_mem_map(size_t loc)
{
    //TODO: check using multiboot memmap (grub)    
    
    return 0;
}
