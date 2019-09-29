#include <stddef.h>
#include <stdint.h>

#include "multiboot.h"

#include "kterm.h"
#include "kmem.h"
#include "kmalloc.h"
#include "kstd.h"

#include "../lisp/parser.h"
#include "../lisp/types.h"

// kernel entry point
void k_main(uint32_t multiboot_magic, multiboot_info_t* mbd)
{
    k_term_init();
    k_print("kernel init\n");    
    k_mem_init(mbd);
    k_malloc_init(mbd);

    SExp_t* tree = lisp_read("(defun f (x y) (+ x y))");
    k_println("");
    print_sexp(tree);

    //free_sexp(tree);

    //enable memory, paging
    //start scheduler
    //start userspace, userspace processes (shell)
}
