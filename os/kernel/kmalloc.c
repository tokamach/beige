#include <stddef.h>
#include <stdint.h>

#include "kmalloc.h"

#include "multiboot.h"
#include "kmem.h"
#include "kterm.h"
#include "kstd.h"

//malloc data
size_t malloc_offset = 0;
uint16_t total_blocks = 0;
HeapHeader_t* first_header;

static const size_t MAX_HEAP_SIZE = (2^10) * (2^4); //16k

HeapHeader_t* add_header(size_t addr, size_t size)
{
    HeapHeader_t* head = (HeapHeader_t*)addr;
    head->marked = 0;
    head->size = size;
    total_blocks++;
    return head;
}

void k_malloc_init(multiboot_info_t* mbd)
{
    //_mbd = mbd;
    
    if(!(mbd->flags & MULTIBOOT_INFO_MEMORY))
    {
	//error
	k_print("[ERROR][k_malloc_init] multiboot flag 0 not set\n");
	return;
    }
    
    //initalise heap buddy tree
    first_header = add_header(mem_region_start, MAX_HEAP_SIZE);
    //first_header = (HeapHeader_t*) mem_region_start;
    //first_header->size = mem_region_size - sizeof(HeapHeader_t*);
    //first_header->size = MAX_HEAP_SIZE;

    first_header->marked = 0;

    k_print("[DEBUG][k_malloc_init] created inital block at ");
    k_print_hex((int)first_header);
    k_print(" with size ");
    k_print_hex(first_header->size);
    k_print("\n");
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
    
    HeapHeader_t* new_header = add_header(new_addr, new_size);
    header->marked = 0;
    header->size = new_size;

    total_blocks++;
    
    return header;
}

void k_mem_print_blocks();
void* kmalloc(size_t size)
{
    size = align(size);
    
    if(first_header->marked)
    {
	//memory all used FUCK
	//k_println("[ERROR][kmalloc] first header marked");

	//while(1){/*hang*/}
	    
	//return null pointer and pray
	//return 0;
    }

    HeapHeader_t* cur_header = first_header;
    
    //keep seeking until we find a header that can fit the object and is unmarked
    int DEBUG_SKIP_COUNT = 0;
    while(size > cur_header->size && cur_header->marked != 0)
    {
	cur_header = next_block(cur_header);
	DEBUG_SKIP_COUNT++;
    }

    int DEBUG_SPLIT_COUNT = 0;
    while((cur_header->size / 2) > size)
    {
	cur_header = split_block(cur_header);
	DEBUG_SPLIT_COUNT++;
    }
    
    k_print("[DEBUG][kmalloc] Found block ");
    k_print_hex((size_t) cur_header);
    k_print(":");
    k_print_num(size);
    k_print(" by skipping ");
    k_print_num(DEBUG_SKIP_COUNT);
    k_print(" times and splitting ");
    k_print_num(DEBUG_SPLIT_COUNT);
    k_println(" times");
    
    //k_mem_print_blocks();

    //we've found our ideal block
    cur_header->marked = 1;

    void* result = (char*)cur_header + (size_t)sizeof(HeapHeader_t);
    return result;
}

void kfree(void* addr)
{
    //TODO: merge blocks
    HeapHeader_t* block = addr - sizeof(HeapHeader_t);


    //DEBUG
    k_print("[DEBUG][kfree] Freeing ");
    k_print_hex((size_t)block);
    k_print(", marked: ");
    k_print_num(block->marked);
    k_print(", size: ");
    k_print_hex(block->size);
    k_print("\n");
	
    block->marked = 0;
}

void k_mem_print_block(HeapHeader_t* header)
{
    k_print_hex((int)header);
    k_print(":");
    k_print_num(header->marked);
    k_print(":");
    k_print_num(header->size);
    k_print("; ");
}

void k_mem_print_blocks()
{
    HeapHeader_t* h = first_header;
    for(int i = 0; i < total_blocks; i++)
    {
	k_mem_print_block(h);
	h = next_block(h);
    }
    k_print("\n");
}
