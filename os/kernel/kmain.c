#include <stddef.h>
#include <stdint.h>

#include "multiboot.h"

#include "kterm.h"
#include "kmem.h"

// kernel entry point
void k_main(multiboot_info_t* mbd, unsigned int magic)
{
    k_term_init();
    k_print("kernel init\n");

    k_print_num(16);
    k_print("\n");
    k_print_hex(16);
    k_print("\n");
	
    k_print_num(mbd->flags);
    k_print("\n");
    
    if(!(mbd->flags & 0b1))
    {
	//error
	k_print("ERROR: multiboot flag 0 not set\n");
	return;
    }
    
    k_print("lower memory: ");
    k_print_hex(mbd->mem_lower);
    k_print("\nupper memory: ");
    k_print_hex(mbd->mem_upper);

    if(!((mbd->flags & 0b00100000) >> 5))
    {
	k_print("\nmultiboot location: ");
	k_print_hex(mbd->mmap_addr);	
    }
    
    //k_print("lower memory size: \n");
    //size_t low_mem = k_query_low_mem();
    //k_print_num(low_mem);
    //size_t upper_mem = k_query_mem_map(0, (uint16_t) 0x100);
    //k_print_num(upper_mem);

    //start scheduler
    //start shell
}
