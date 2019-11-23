#include "env.h"

#include "types.h"
#include "defun.h"
#include "eval.h"

#ifdef LISP_TEST
//testin
#include <stdio.h>
#include "../tests/kernel_mappings.h"
#else
//real world
#include "../kernel/kmalloc.h"
#include "../kernel/kterm.h"
#include "../kernel/kstd.h"
#endif

#include "../kernel/kassert.h"

/*
 * Symbol stuff
 */
uint16_t symbol_count = 0;

symbol_id add_symbol(char* symbol)
{
    //TODO: fix hacky code here
    symbol_id preexisting_sym = lookup_symbol(symbol);
    if(preexisting_sym != 65535)
      return preexisting_sym;
	
    symbol_table[symbol_count] = (symbol_t){symbol_count, symbol};
    return symbol_count++;
}

symbol_id lookup_symbol(char *symbol)
{
    for(int i = 0; i < symbol_count; i++)
	if(streq(symbol_table[i].symbol, symbol))
	    return i;

    //fuck dis won't work
    return -1;
}

char* lookup_id(symbol_id i)
{
    return symbol_table[i].symbol;
}


/*
 * Env Stuff
 */
void add_env_entry_lobj(env_t* env, symbol_id sym, lobj_t* val)
{
    env_entry_t* ret = &env->entries[env->entry_count];
    ret->type = lobj;
    ret->sym = sym;
    ret->lobj = val;
    env->entry_count++;
}

void add_env_entry_native(env_t* env, env_entry_type type, symbol_id sym, void* fun)
{
    //TODO: check type = nativef-nativef5
    env_entry_t* ret = &env->entries[env->entry_count];
    ret->type = type;
    ret->sym = sym;
    ret->nativef = fun;
    env->entry_count++;
}

//TODO rename env_lookup
env_entry_t* get_env_entry(env_t* env, char* sym)
{
    // TODO: stop having to do this, encode symid inline
    uint8_t symid = lookup_symbol(sym);
    
    for(int i = 0; i < env->entry_count; i++)
    {
	if(env->entries[i].sym == symid)
	    return &(env->entries[i]);
    }

    //not in current env, look in outer
    if(env->outer)
	return get_env_entry(env->outer, sym);
    else
	return NULL;
}

env_t* make_env(env_t* outer)
{
    env_t* ret = kmalloc(sizeof(env_t));
    for(int i = 0; i < ENV_SIZE; i++)
	ret->entries[i] = (env_entry_t){empty, 0, {0}};
    
    if(outer)
	ret->outer = outer;
    else
	ret->outer = NULL;
    
    return ret;
}

void free_env(env_t* env)
{
    /* env_t has no pointers to children, so we don't need to 
     * free anything else */
    kfree(env);
}

/*
 * Builtin Special Forms
 */
lobj_t* sp_quote(env_t* env, lobj_t* args)
{
    //argc should be 1
    assert(length(args) == 1, "quote needs 1 arg");
    return nth(args, 0);
}

lobj_t* sp_progn(env_t* env, lobj_t* args)
{
    int argc = length(args);
    for(int i = 0; i < argc; i++)
	eval(env, nth(args, i));

    return eval(env, args);
}

/*
 * (if exp1 exp2 exp3)
 * evaluate exp1, if not nil exp2, else exp3
 */
lobj_t* sp_if(env_t* env, lobj_t* args)
{
    if(eval(env, nth(args, 0)))
    {
	return eval(env, nth(args, 1));
    }
    else
    {
	return eval(env, nth(args, 2));
    }
}

lobj_t* sp_lambda(env_t* env, lobj_t* args)
{
    assert(args->type == Cons, "Must pass list of args to lambda");
    
    lobj_t* funargs = car(args);
    lobj_t* funbody = car(cdr(args));
    
    return func(funargs, funbody);
}

lobj_t* sp_define(env_t* env, lobj_t* args)
{
    lobj_t* name = car(args);
    lobj_t* body = eval(env, car(cdr(args))); //extract value from list
    
    assert(name->type == Sym, "tried to define with non sym as name");
    add_env_entry_lobj(env, add_symbol(name->val), body);
    
    return name;
}

/*
 * Builtin Functions
 */
lobj_t* fn_car(env_t* env, lobj_t* list)
{
    assert(list->type == Cons, "can't car of non cons type");
    return list->car;
}

lobj_t* fn_cdr(env_t* env, lobj_t* list)
{
    assert(list->type == Cons, "can't cdr of non cons type");
    return list->cdr;
}

lobj_t* fn_list(env_t* env, lobj_t* args)
{
    //TODO: if length(args) = 1, return args->car
    return args;
}

/*
 * Maths
 */
lobj_t* fn_add(env_t* env, lobj_t* args)
{
    size_t total = 0;
    LIST_ITER(args, i)
    {
	total += nth(args, i)->numl;
    }
	
    return num(total);
}

lobj_t* fn_sub(env_t* env, lobj_t* args)
{
    size_t total = nth(args, 0)->numl;
    for(int i = 1; i < length(args); i++)
    {
	total -= nth(args, i)->numl;
    }
	
    return num(total);
}

lobj_t* fn_mul(env_t* env, lobj_t* args)
{
    size_t total = nth(args, 0)->numl;
    for(int i = 1; i < length(args); i++)
    {
	total *= nth(args, i)->numl;
    }
	
    return num(total);
}

lobj_t* fn_div(env_t* env, lobj_t* args)
{
    size_t total = nth(args, 0)->numl;
    for(int i = 1; i < length(args); i++)
    {
	total /= nth(args, i)->numl;
    }
	
    return num(total);
}

//TODO: make variadic?
lobj_t* fn_eq(env_t* env, lobj_t* a, lobj_t* b)
{
    //TODO: return another truthy value, e.g. t
    if(a->numl == b->numl)
	return sym("t");
    else
	return NULL;
}

/*
 * Initalizing the defualt lisp environment
 * this contains the basic lisp functions
 * which others can be built upon
 */
env_t* make_base_env()
{
    //TODO: add defun, let, etc to env
    //make an empty env with no outer
    env_t* env = make_env(NULL);

    /* Lisp Fundamentals */
    // nil and t
    add_env_entry_native(env, empty, add_symbol("nil"), NULL);
    add_env_entry_lobj  (env,        add_symbol("t"),   sym("t"));
    
    // Special Forms
    add_env_entry_native(env, special, add_symbol("quote"),  &sp_quote);
    add_env_entry_native(env, special, add_symbol("progn"),  &sp_progn);
    add_env_entry_native(env, special, add_symbol("if"),     &sp_if);
    add_env_entry_native(env, special, add_symbol("lambda"), &sp_lambda);
    add_env_entry_native(env, special, add_symbol("define"), &sp_define);

    // Functions
    add_env_entry_native(env, nativef1, add_symbol("car"), &fn_car);
    add_env_entry_native(env, nativef1, add_symbol("cdr"), &fn_cdr);
    add_env_entry_native(env, nativef, add_symbol("list"), &fn_list);

    /* Mathematics operators */
    add_env_entry_native(env, nativef, add_symbol("add"), &fn_add);
    add_env_entry_native(env, nativef, add_symbol("sub"), &fn_sub);
    add_env_entry_native(env, nativef, add_symbol("mul"), &fn_mul);
    add_env_entry_native(env, nativef, add_symbol("div"), &fn_div);

    add_env_entry_native(env, nativef2, add_symbol("eq"), &fn_eq);
    
    return env;
}

/*
 * Kernel env stuff. here we define the functions that the kernel
 * will probably want to use, memory access, inb/outb
 */

env_t* make_kernel_env()
{
    env_t* env = make_env(make_base_env());
    
    return env;
}
