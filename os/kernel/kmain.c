#include <stddef.h>
#include <stdint.h>

#include "kterm.h"
#include "kmem.h"

// kernel entry point
void k_main()
{
    k_term_init();
    k_print("kernel init\n");
    k_print("lower memory size: ");
    size_t low_mem = k_query_low_mem();
    k_print_num(low_mem);

    //switch to protected mode
    //start scheduler
    //start shell
}
