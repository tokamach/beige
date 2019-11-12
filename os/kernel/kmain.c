#include <stddef.h>
#include <stdint.h>

#include "multiboot.h"

#include "kterm.h"
#include "kmem.h"
#include "kmalloc.h"
#include "kinterrupt.h"
#include "kpic.h"
#include "kstd.h"

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

    env_t* kenv = make_kernel_env();

    kprinteq_lisp((add 2 (add 2 3)));
    //kprinteq_lisp((add 1 2 ));

    //enable memory, paging
    //start scheduler
    //start userspace, userspace processes (shell)
}
