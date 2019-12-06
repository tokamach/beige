#include "kernel_env.h"

#include "env.h"
#include "types.h"
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
    assert(ptr->type == Num ||
	   ptr->type == U8  ||
	   ptr->type == U16 ||
	   ptr->type == U32, "cannot deref non numeric type");
    
    //TODO: figure out ptr size e.g.:
    // does (ptr_dref 0x6000) make a 32bit Num from 0x6000 to 0x6004?
    // perhaps specify return type....
    return NULL;
}

lobj_t* fn_shl(env_t* env, lobj_t* value, lobj_t* amount)
{
    assert(value->type == Num ||
	   value->type == U8  ||
	   value->type == U16 ||
	   value->type == U32, "cannot shift non numeric type");
    assert(amount->type == Num ||
	   amount->type == U8  ||
	   amount->type == U16 ||
	   amount->type == U32, "cannot shift by non numeric type");

    return num((value->numl) << (amount->numl));
}

lobj_t* fn_shr(env_t* env, lobj_t* value, lobj_t* amount)
{
    assert(value->type == Num ||
	   value->type == U8  ||
	   value->type == U16 ||
	   value->type == U32, "cannot shift non numeric type");
    assert(amount->type == Num ||
	   amount->type == U8  ||
	   amount->type == U16 ||
	   amount->type == U32, "cannot shift by non numeric type");

    return num((value->numl) >> (amount->numl));
}

lobj_t* fn_bor(env_t* env, lobj_t* a, lobj_t* b)
{
    assert(a->type == Num ||
	   a->type == U8  ||
	   a->type == U16 ||
	   a->type == U32, "cannot and non numeric a");
    assert(b->type == Num ||
	   b->type == U8  ||
	   b->type == U16 ||
	   b->type == U32, "cannot and by non numeric b");

    return num((a->numl) | (b->numl));
}

lobj_t* fn_band(env_t* env, lobj_t* a, lobj_t* b)
{
    assert(a->type == Num ||
	   a->type == U8  ||
	   a->type == U16 ||
	   a->type == U32, "cannot and non numeric a");
    assert(b->type == Num ||
	   b->type == U8  ||
	   b->type == U16 ||
	   b->type == U32, "cannot and by non numeric b");

    return num((a->numl) & (b->numl));
}

env_t* make_kernel_env()
{
    env_t* env = make_env(make_base_env());

    add_env_entry_native(env, nativef1, add_symbol("inb"), &fn_inb);
    add_env_entry_native(env, nativef2, add_symbol("outb"), &fn_outb);
    add_env_entry_native(env, nativef2, add_symbol("shl"), &fn_shl);
    add_env_entry_native(env, nativef2, add_symbol("shr"), &fn_shr);
    add_env_entry_native(env, nativef2, add_symbol("bor"), &fn_bor);
    add_env_entry_native(env, nativef2, add_symbol("band"), &fn_band);
    
    return env;
}
