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
    k_mem_init(mbd);
    k_malloc_init(mbd);
    k_pic_init();
    k_interrupt_init();
  
    env_t* kenv = make_kernel_env();

    kprinteq_lisp((define countdown (lambda (start)
				     (if (eq start 1)
					 (cons start nil)
					 (cons start (countdown (sub start 1)))))));
    kprinteq_lisp((define x 10));
    kprinteq_lisp((countdown x));


    k_status.lisp_state = Running;
    k_status.lisp_world = kenv;
    
    k_term_update_statline();
    
    //start scheduler
    //start lisp repl
    //start userspace, userspace processes (shell)
}
