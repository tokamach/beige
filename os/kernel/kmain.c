#include <stddef.h>
#include <stdint.h>

#include "multiboot.h"

#include "kterm.h"
#include "kmem.h"
#include "kmalloc.h"
#include "kinterrupt.h"
#include "kpic.h"
#include "kgdt.h"
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
    k_gdt_init();
    /*k_mem_init(mbd);
      k_malloc_init(mbd);*/
    /*k_pic_init();
      k_interrupt_init();*/

    /*env_t* kenv = make_kernel_env();
    kprinteq_lisp((define l (list 2 3 4)));
    k_print("\n");
    kprinteq_lisp((define cadr (lambda (list) (car (cdr list)))));
    k_print("\n");
    kprinteq_lisp((cadr l));*/
/*
    k_status.lisp_state = Running;
    k_status.lisp_world_size = &(kenv->entry_count);
    
    k_term_update_statline();
*/
    
    //enable memory, paging
    //start scheduler
    //start userspace, userspace processes (shell)
}
