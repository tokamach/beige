#include <stddef.h>
#include <stdint.h>

#include "kmem.h"

#include "kterm.h"
#include "multiboot.h"

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

size_t k_query_mem_map(size_t addr, size_t length)
{
    //TODO: check using multiboot memmap (grub)

    int entries = length / sizeof(MMapEntry_t);

    k_print("|Size|Addr|Len|Type|\n");
    for(int i = 0; i < entries; i++)
    {
	MMapEntry_t* entry = (MMapEntry_t*)(addr + (i * sizeof(MMapEntry_t)));

	uint64_t addr = entry->AddrH;
	addr <<= 32;
	addr += entry->AddrL;

	uint64_t len = entry->LengthH;
	len <<= 32;
	len += entry->LengthL;
	
	k_print("|");
	k_print_hex(entry->Size);
	k_print("|");
	k_print_hex(addr);
	k_print("|");
	k_print_hex(len);
	k_print("|");

	switch(entry->Type)
	{
	case MEMORY_AVAILABLE:
	    k_print("Available");
	    break;

	case MEMORY_RESERVED:
	    k_print("Reserved");
	    break;

	default:
	    break;
	}
	
	k_print("|");
	
	k_print("\n");
    }
        
    return 0;
}

void k_segment_init()
{
    
}

void k_paging_init()
{
    //build page 
}
