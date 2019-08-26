#include <stddef.h>
#include <stdint.h>

#include "kmem.h"

#include "kterm.h"
#include "kstd.h"
#include "multiboot.h"

static const MAX_HEAP_SIZE = (2^10) * 64; //1 MB * 64 = 64MB

multiboot_info_t* _mbd;

size_t mem_map_addr;
size_t mem_map_size;
size_t mem_map_entries;

//region to fill
MMapEntry_t* working_mem_region;
size_t mem_region_start;
size_t mem_region_size;
size_t mem_region_end;

//malloc data
size_t malloc_offset = 0;
HeapHeader_t* first_header;

void k_mem_init(multiboot_info_t* mbd)
{
    _mbd = mbd;
    
    if(!(mbd->flags & MULTIBOOT_INFO_MEMORY))
    {
	//error
	k_print("ERROR: multiboot flag 0 not set\n");
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
	addr += entry->AddrL;

	uint64_t len = entry->LengthH;
	len <<= 32;
	len += entry->LengthL;

	if(len > biggest_size)
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
    k_print("Found largest memory entry ");
    k_print_num(biggest_index);
    k_print(" with size ");
    k_print_hex(biggest_size);
    k_print("\n");

    //initalise heap buddy tree
    //TODO: move to kmalloc_init function
    first_header = (HeapHeader_t*) mem_region_start;
    first_header->size = mem_region_size - sizeof(HeapHeader_t*);
    first_header->marked = 0;

    k_print("[DEBUG] created inital block at ");
    k_print_hex((int)first_header);
    k_print(" with size ");
    k_print_hex(first_header->size);
    k_print("\n");

    k_print("Memory initialized\n");
}

HeapHeader_t* add_header(size_t addr, size_t size)
{
    HeapHeader_t* head = (HeapHeader_t*)addr;
    head->marked = 0;
    head->size = size;
    return head;
}

HeapHeader_t* next_block(HeapHeader_t* header)
{
    //move to next header
    //no going back fuck lol
    //just go to the next header its that easy
    return header + sizeof(HeapHeader_t) + header->size;
}

HeapHeader_t* split_block(HeapHeader_t* header)
{
    //split header, return new first header
    //next header can be found with k_mem_next_header()
    //Split like so
    //|--------+----------------------|
    //| Header | Data                 |
    //|--------+----------------------|
    // into
    //|--------+------|--------+------|
    //| Header | Data | Header | Data |
    //|--------+------|--------+------|

    size_t old_size = header->size;
    size_t total_size = sizeof(HeapHeader_t) + old_size;
    
    //current address + half is the point for the new header to be inserted
    size_t new_addr = (size_t) header + (total_size / 2);
    size_t new_size = (total_size / 2) - sizeof(HeapHeader_t);
    
    HeapHeader_t* new_header = (HeapHeader_t*) new_addr;
    header->marked = 0;
    header->size = new_size;
    new_header->marked = 0;
    new_header->size = new_size;

    return header;
}

void* kmalloc(size_t size)
{
    //bitmap (build in init)
    //each bit represets 4byte? 1byte?
    //0 is free, 1 is allocated
    //use first fit

    if(first_header->marked)
    {
	//memory all used FUCK
	k_print("ERROR: first header marked\n");
	
	//return null pointer and pray
	return 0;
    }

    HeapHeader_t* cur_header = first_header;
    
    //keep seeking until we find a header that can fit the object and is unmarked
    int DEBUG_SKIP_COUNT = 0;
    while(!(size <= cur_header->size) && cur_header->marked != 0)
    {
	cur_header = next_block(cur_header);
	DEBUG_SKIP_COUNT++;
    }

    int DEBUG_SPLIT_COUNT = 0;
    while((cur_header->size / 2) > size)
    {
	split_block(cur_header);
	DEBUG_SPLIT_COUNT++;
    }

    k_print("[DEBUG] Found block at ");
    k_print_hex((int)cur_header);
    k_print(" by skipping ");
    k_print_num(DEBUG_SKIP_COUNT);
    k_print(" times and splitting ");
    k_print_num(DEBUG_SPLIT_COUNT);
    k_print(" times.\n");

    //we've found our ideal block
    cur_header->marked = 1;
    
    return cur_header + sizeof(HeapHeader_t*);
}

void kfree(void* addr)
{
    //TODO: merge blocks
    HeapHeader_t* block = addr - sizeof(HeapHeader_t);
    block->marked = 0;
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
    //TODO: check using multiboot memmap (grub)
    
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
		k_print("UNIMPLEMENTED MEM TYPE");
		break;
	    }
	
	    k_print("|");	
	    k_print("\n");
	}
    }
}

//DEPRECATED, use for debug
void k_print_mem_map_at(size_t addr, size_t length)
{
    //TODO: check using multiboot memmap (grub)

    int entries = length / sizeof(MMapEntry_t);

    k_print("|Size|Addr|Len|Type|\n");
    for(int i = 0; i < entries; i++)
    {
	MMapEntry_t* entry = k_mem_map_entry_at(addr, i);

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
	    k_print("UNIMPLEMENTED MEM TYPE");
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
