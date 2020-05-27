#include <stddef.h>
#include <stdint.h>

#include "kmalloc.h"

#include "multiboot.h"
#include "kmem.h"
#include "kterm.h"
#include "kstd.h"

/*
 * Don't ask (appeasing gdb when running lisp in hosted mode)
 */
#ifdef KDEBUG
__attribute__((optimize("O0")))
void* malloc(size_t s)
{
    return kmalloc(s);
}
#endif

bitmap_status* bitmap; 
void* arena;
size_t total_blocks; //TODO: make global for printing in statline
size_t max_blocks;

static const uint8_t factor = 8;
static const size_t MAX_HEAP_SIZE = 0x1000; //(2^10) * (2^4); //(16k)

void k_malloc_init(multiboot_info_t* mbd)
{
    /*
     * Check that multiboot flag 0 is set
     */
    if(!(mbd->flags & MULTIBOOT_INFO_MEMORY))
    {
	k_print("[ERROR][k_malloc_init] multiboot flag 0 not set\n");
	return;
    }
    
    //initalise bitmap
    total_blocks = 0;
    max_blocks = MAX_HEAP_SIZE * factor;
    bitmap = (bitmap_status*)mem_region_start;
    arena  = bitmap + max_blocks;

    for(size_t i = 0; i < max_blocks; i++)
	bitmap[i] = Free;

    k_print("malloc initialized, bitmap at ");
    k_print_hex((size_t) bitmap);
    k_print(" heap at ");
    k_print_hex((size_t) arena);
    k_print("\n");
}

//TODO: sanity checks
void* kmalloc(size_t size)
{
    size_t base  = 0;
    size_t accum = 0;
    
    for(size_t i = 0; i < max_blocks; i++)
    {
	if(bitmap[i] == Free)
	{
	    if (accum == 0)
		base = i;
	    
	    accum++;
	}
	else if (bitmap[i] == Used)
	{
	    accum = 0;
	    continue;
	}
	
	if((accum * factor) >= size)
	{
	    for(size_t j = 0; j < (accum); j++)
		bitmap[base + j] = Used;

#ifdef KDEBUG
	    k_print("Alloced ");
	    k_print_hex((size_t)arena + (base * factor));
	    k_print(":");
	    k_print_hex(size);
	    k_print("\n");
	    
#endif
	    total_blocks++;
	    return (void*)(arena + (base * factor));
	}
    }

    k_print("ERROR\n");
}

// TODO: add size attribute
void kfree(void* addr)
{
    size_t bitmap_addr = (size_t)(addr) / factor;
    bitmap[bitmap_addr] = Free;

#ifdef KDEBUG
    k_print("freed ");
    k_print_hex((size_t)addr);
    k_print("\n");
#endif
}
