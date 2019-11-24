#include "base_env.h"

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

#include "defun.h"
#include "env.h"
#include "../kernel/kassert.h"

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
    for(int i = 0; i < argc - 1; i++)
	eval(env, nth(args, i));

    return eval(env, nth(args, argc - 1));
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
lobj_t* fn_cons(env_t* env, lobj_t* a, lobj_t* b)
{
    return cons(a, b);
}

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
    add_env_entry_native(env, nativef2, add_symbol("cons"), &fn_cons);
    add_env_entry_native(env, nativef1, add_symbol("car"),  &fn_car);
    add_env_entry_native(env, nativef1, add_symbol("cdr"),  &fn_cdr);
    add_env_entry_native(env, nativef, add_symbol("list"),  &fn_list);

    /* Mathematics operators */
    add_env_entry_native(env, nativef, add_symbol("add"), &fn_add);
    add_env_entry_native(env, nativef, add_symbol("sub"), &fn_sub);
    add_env_entry_native(env, nativef, add_symbol("mul"), &fn_mul);
    add_env_entry_native(env, nativef, add_symbol("div"), &fn_div);

    add_env_entry_native(env, nativef2, add_symbol("eq"), &fn_eq);
    
    return env;
}
