#include <stddef.h>
#include <stdint.h>

#include "multiboot.h"

#include "kterm.h"
#include "kmem.h"
#include "kstd.h"

// kernel entry point
void k_main(uint32_t multiboot_magic, multiboot_info_t* mbd)
{
    k_term_init();
    k_print("kernel init\n");    
    k_mem_init(mbd);

    //TODO: move this to memory
    k_print("lower memory: ");
    k_print_hex(mbd->mem_lower);
    k_print("\nupper memory: ");
    k_print_hex(mbd->mem_upper);
    k_print_mem_map();

    int* i = kmalloc(sizeof(i));
    *i = 10;

    k_print_num(*i);

    //kfree(i);

    //enable memory, paging
    //start scheduler
    //start userspace, userspace processes (shell)
}
