#include <stddef.h>
#include <stdint.h>

#include "multiboot.h"

#include "kterm.h"
#include "kmem.h"
#include "kmalloc.h"
#include "kinterrupt.h"
#include "kpic.h"
#include "kstd.h"
#include "kstatus.h"

#include "../lisp/parser.h"
#include "../lisp/types.h"
#include "../lisp/env.h"
#include "../lisp/eval.h"

// kernel entry point
void k_main(uint32_t multiboot_magic, multiboot_info_t* mbd)
{
    k_term_init();
    k_print("kernel init\n");
    k_mem_init(mbd);
    k_malloc_init(mbd);
    k_pic_init();
    k_interrupt_init();

    k_status.lisp_state = Running;
    k_status.lisp_world_size = (size_t)25;
    
    k_term_update_statline();

    env_t* kenv = make_kernel_env();
    lobj_t* aga = lisp_read("(add 2 2)");
    print_cons(aga);
    //kprinteq_lisp((add 2 3));

    //enable memory, paging
    //start scheduler
    //start userspace, userspace processes (shell)
}
