#include "kernel_env.h"

#ifdef LISP_TEST
//testin
#include <stdio.h>
#include "../tests/kernel_mappings.h"
#else
//real world
#include "../kernel/kmalloc.h"
#include "../kernel/kterm.h"
//#include "../kernel/kstd.h"
#endif

#include "env.h"
#include "base_env.h"
#include "../kernel/kassert.h"
#include "../kernel/kstd.h"

/*
 * Kernel env stuff. here we define the functions that the kernel
 * will probably want to use, memory access, inb/outb
 */

lobj_t* fn_inb(env_t* env, lobj_t* port)
{
    assert(port->type == Num, "inb: port must be a Num");
    return num((size_t) inb(port->numl));
}

lobj_t* fn_outb(env_t* env, lobj_t* port, lobj_t* val)
{
    assert(port->type == Num, "outb: port must be a num");
    assert(val->type  == Num, "outb: val must be a num");
    outb(port->numl, val->numl);
    return NULL;
}

lobj_t* fn_ptr_dref(env_t* env, lobj_t* ptr)
{

}

env_t* make_kernel_env()
{
    env_t* env = make_env(make_base_env());

    add_env_entry_native(env, nativef1, add_symbol("inb"), &fn_inb);
    add_env_entry_native(env, nativef2, add_symbol("outb"), &fn_outb);
    
    return env;
}
