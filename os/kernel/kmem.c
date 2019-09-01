#include <stddef.h>
#include <stdint.h>

#include "kmem.h"

#include "kterm.h"
#include "kstd.h"
#include "multiboot.h"

static const size_t MAX_HEAP_SIZE = (2^10) * 64; //1 MB * 64 = 64MB

multiboot_info_t* _mbd;

size_t mem_map_addr;
size_t mem_map_size;
size_t mem_map_entries;

//region to fill
void k_mem_init(multiboot_info_t* mbd)
{
    _mbd = mbd;
    
    if(!(mbd->flags & MULTIBOOT_INFO_MEMORY))
    {
	//error
	k_print("[ERROR][k_mem_init] multiboot flag 0 not set\n");
	return;
    }
    
    //init mem map info
    mem_map_addr = mbd->mmap_addr;
    mem_map_size = mbd->mmap_length;
    mem_map_entries = mem_map_size / sizeof(MMapEntry_t);
    
    //find largest region
    size_t   biggest_size = 0;
    uint16_t biggest_index;

    for(uint16_t i = 0; i < mem_map_entries; i++)
    {
	MMapEntry_t* entry = k_mem_map_entry(i);
	uint64_t addr = entry->AddrH;
	addr <<= 32;
	addr = addr | entry->AddrL;

	uint64_t len = entry->LengthH;
	len <<= 32;
	len = len | entry->LengthL;

	//if(len > biggest_size)
	//temporarily use first mem
	if(i == 0)
	{
	    biggest_size  = len;
	    biggest_index = i;

	    mem_region_start = addr;
	    mem_region_size  = len;
	}
    }

    //set working memory region
    working_mem_region = k_mem_map_entry(biggest_index);
    mem_region_end = mem_region_start + mem_region_size;
    
    //debug print found region
    k_print("[DEBUG][k_mem_init] Found largest memory entry ");
    k_print_num(biggest_index);
    k_print(" with size ");
    k_print_hex(biggest_size);
    k_print("\n");

    k_print("Memory initialized\n");
}

MMapEntry_t* k_mem_map_entry(uint16_t entry)
{
    MMapEntry_t* res = (MMapEntry_t*)(mem_map_addr + (entry * sizeof(MMapEntry_t)));
    return res;
}

MMapEntry_t* k_mem_map_entry_at(size_t addr, uint16_t entry)
{
    MMapEntry_t* res = (MMapEntry_t*)(addr + (entry * sizeof(MMapEntry_t)));
    return res;
}

void k_print_mem_map()
{
    k_print("lower memory: ");
    k_print_hex(_mbd->mem_lower);
    k_print("\nupper memory: ");
    k_print_hex(_mbd->mem_upper);
    k_print_mem_map();
    
    if(_mbd->flags & MULTIBOOT_INFO_MEM_MAP)
    {
	k_print("\nmem map location: ");
	k_print_hex(_mbd->mmap_addr);
	k_print("\nmem map length: ");
	k_print_hex(_mbd->mmap_length);

	k_print("\n\nmem map:\n");

	int entries = mem_map_size / sizeof(MMapEntry_t);

	k_print("|Size|Addr|Len|Type|\n");
	for(int i = 0; i < entries; i++)
	{
	    MMapEntry_t* entry = k_mem_map_entry(i);

	    uint64_t addr = entry->AddrH;
	    addr <<= 32;
	    addr = addr | entry->AddrL;

	    uint64_t len = entry->LengthH;
	    len <<= 32;
	    len = len | entry->LengthL;
	
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
		k_print("UNIMPLEMENTED MEM TYPE");
		break;
	    }
	
	    k_print("|");	
	    k_print("\n");
	}
    }
}

void k_segment_init()
{
    
}

void k_paging_init()
{
    //build page 
}
