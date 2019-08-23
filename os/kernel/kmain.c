#include <stddef.h>
#include <stdint.h>

#include "multiboot.h"

#include "kterm.h"
#include "kmem.h"

// kernel entry point
void k_main(multiboot_info_t* mbd)
{    
    k_term_init();
    k_print("kernel init\n");

    if(!(mbd->flags & MULTIBOOT_INFO_MEMORY))
    {
	//error
	k_print("ERROR: multiboot flag 0 not set\n");
	return;
    }
    
    k_print("lower memory: ");
    k_print_hex(mbd->mem_lower);
    k_print("\nupper memory: ");
    k_print_hex(mbd->mem_upper);

    if(mbd->flags & MULTIBOOT_INFO_MEM_MAP)
    {
	size_t mmap_addr = mbd->mmap_addr;
	size_t mmap_size = mbd->mmap_length;
	
	k_print("\nmem map location: ");
	k_print_hex(mmap_addr);
	k_print("\nmem map length: ");
	k_print_hex(mmap_size);

	k_print("\n\nmem map:\n");
	k_query_mem_map(mmap_addr, mmap_size);
    }
    
    //start scheduler
    //start shell
}
