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
    //k_interrupt_init();

//    while(1){ asm("nop;");}
  
    env_t* kenv = make_kernel_env();

    char *fib = "\
(define countdown (lambda (start)\
(if (eq start 1)\
start\
(list start (countdown (sub start 1))))))";
    
    lobj_t* fibast = lisp_read(fib);
    print_sexp(fibast);
    k_print("\n");
    eval(kenv, fibast);
    kprinteq_lisp((countdown 10));


    k_status.lisp_state = Running;
    k_status.lisp_world_size = &(kenv->entry_count);
    
    k_term_update_statline();
    
    //start scheduler
    //start lisp repl
    //start userspace, userspace processes (shell)
}
