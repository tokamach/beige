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

    if(!((mbd->flags & 0b00100000) >> 5))
    {
	k_print("\nmultiboot location: ");
	k_print_hex(mbd->mmap_addr);	
    }
    
    //start scheduler
    //start shell
}
