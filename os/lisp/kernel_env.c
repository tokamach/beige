#include "kernel_env.h"

#include "env.h"
#include "types.h"
#include "base_env.h"
#include "../kernel/kassert.h"
#include "../kernel/kstd.h"

#define assert_numeric(obj, msg) assert(car(obj)->type == Num ||	\
					car(obj)->type == U8  ||	\
					car(obj)->type == U16 ||	\
					car(obj)->type == U32, #msg);

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

lobj_t* fn_ptr(env_t* env, lobj_t* addr, lobj_t* size)
{
    assert_numeric(addr, "cannot make pointer from non numeric address");
    assert_numeric(size, "cannot make pointer from non numeric size");

    return cons(addr, size);
}

lobj_t* fn_addr_of(env_t* env, lobj_t* obj)
{
    // return (address of obj . size of obj)
    return cons(num((int)obj), sizeof(obj));
}

lobj_t* fn_ptr_dref(env_t* env, lobj_t* ptr)
{
    assert(ptr->type == Cons, "must dref ptr of form (addr . size)");
    assert_numeric(ptr, "cannot deref non numeric type");
    
    /*
     * ptr is a pair of form: (addr . size) size in bytes
     * so a pointer to a u8 at 0x5000 is (0x5000 . 1)
     */

    //return *(j);
    return NULL;
}

lobj_t* fn_memset_u8(env_t* env, lobj_t* addr, lobj_t* val)
{
    assert_numeric(addr, "mem_set addr must be numeric");
    assert(val->type == U8,  "mem_set val must be numeric");

    *(uint8_t*)addr = val->u8;
    return NULL;
}

lobj_t* fn_memset_u16(env_t* env, lobj_t* addr, lobj_t* val)
{
    assert_numeric(addr, "mem_set addr must be numeric");
    assert(val->type == U16,  "mem_set val must be numeric");

    *(uint8_t*)addr = val->u16;
    return NULL;
}

lobj_t* fn_memset_u32(env_t* env, lobj_t* addr, lobj_t* val)
{
    assert_numeric(addr, "mem_set addr must be numeric");
    assert(val->type == U32,  "mem_set val must be numeric");

    *(uint8_t*)addr = val->u32;
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
    
    add_env_entry_native(env, nativef1, add_symbol("addr-of"), &fn_addr_of);
    add_env_entry_native(env, nativef2, add_symbol("memset-u8"), &fn_memset_u8);
    add_env_entry_native(env, nativef2, add_symbol("memset-u16"), &fn_memset_u16);
    add_env_entry_native(env, nativef2, add_symbol("memset-u32"), &fn_memset_u32);

    add_env_entry_native(env, nativef2, add_symbol("<<"), &fn_shl);
    add_env_entry_native(env, nativef2, add_symbol(">>"), &fn_shr);
    add_env_entry_native(env, nativef2, add_symbol("bor"), &fn_bor);
    add_env_entry_native(env, nativef2, add_symbol("band"), &fn_band);
    
    return env;
}
