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

// (lambda (x) body)
lobj_t* sp_lambda(env_t* env, lobj_t* args)
{
    assert(args->type == Cons, "Must pass list of args to lambda");
    
    lobj_t* funargs = car(args);
    lobj_t* funbody = car(cdr(args));
    
    return func(funargs, funbody);
}

// (def name value)
// (def (name args...) (body))
lobj_t* sp_define(env_t* env, lobj_t* args)
{
    if(car(args)->type == Cons)
    {
	/*
	 * we are following the form (def (name args..) (body))
	 */

	lobj_t* name = caar(args);
	lobj_t* fun_args = cadr(args);
	//TODO: implicit progn
	lobj_t* fun_body = cdar(args);

	add_env_entry_lobj(env, add_symbol(name->val), func(fun_args, fun_body));
	return name;
    }
    else if(car(args)->type == Sym)
    {
	/*
	 * we are following the form (def name value)
	 */
	lobj_t* name = car(args);
	lobj_t* body = eval(env, car(cdr(args))); //extract value from list
    
	add_env_entry_lobj(env, add_symbol(name->val), body);
    
	return name;	
    }
}

/*
 *
 *(let ((x 1)
 *      (y 2))
 *  (body)
 */
lobj_t* sp_let(env_t* env, lobj_t* args)
{

    lobj_t* bind_list = car(args);
    lobj_t* body = cadr(args);
	
    // Make a new scope to execute the let in
    env_t* local_env = make_env(env);

    // Add bindings to the local env
    for(size_t i = 0; i < length(bind_list); i++)
    {
	lobj_t* entry = nth(bind_list, i);
	add_env_entry_lobj(local_env, add_symbol(car(entry)->val), eval(env, cdr(entry)));
    }

    return eval(local_env, body);
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
 * Types
 */
lobj_t* fn_u8(env_t* env, lobj_t* num)
{
    assert(num->type == Num ||
	   num->type == U8  ||
	   num->type == U16 ||
	   num->type == U32,
	   "u8 needs numeric arg");
    return u8(num->numl & 0xFF);
}

lobj_t* fn_u16(env_t* env, lobj_t* num)
{
    assert(num->type == Num ||
	   num->type == U8  ||
	   num->type == U16 ||
	   num->type == U32,
	   "u16 needs numeric arg");
    return u16(num->numl & 0xFFFF);
}

lobj_t* fn_u32(env_t* env, lobj_t* num)
{
    assert(num->type == Num ||
	   num->type == U8  ||
	   num->type == U16 ||
	   num->type == U32,
	   "u32 needs numeric arg");
    return u32(num->numl & 0xFFFFFFFF);
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


/*
 * Comparison operators
 */
lobj_t* fn_eq(env_t* env, lobj_t* a, lobj_t* b)
{
    //TODO: return another truthy value, e.g. t
    if(a->numl == b->numl)
	return sym("t");
    else
	return NULL;
}

lobj_t* fn_greater_than(env_t* env, lobj_t* a, lobj_t* b)
{
    //TODO: return another truthy value, e.g. t
    if(a->numl > b->numl)
	return sym("t");
    else
	return NULL;
}

lobj_t* fn_less_than(env_t* env, lobj_t* a, lobj_t* b)
{
    //TODO: return another truthy value, e.g. t
    if(a->numl < b->numl)
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
    //make an empty env with no outer
    env_t* env = make_env(NULL);

    /* Lisp Fundamentals */
    // nil and t
    add_env_entry_native(env, empty, add_symbol("nil"), NULL);
    add_env_entry_lobj  (env,        add_symbol("t"),   sym("t"));

    //Eval and Apply
    
    // Special Forms
    add_env_entry_native(env, special, add_symbol("quote"),  &sp_quote);
    add_env_entry_native(env, special, add_symbol("progn"),  &sp_progn);
    add_env_entry_native(env, special, add_symbol("if"),     &sp_if);
    add_env_entry_native(env, special, add_symbol("lambda"), &sp_lambda);
    add_env_entry_native(env, special, add_symbol("def"), &sp_define);
    add_env_entry_native(env, special, add_symbol("let"), &sp_let);	

    // Fundamental Functions
    add_env_entry_native(env, nativef2, add_symbol("cons"), &fn_cons);
    add_env_entry_native(env, nativef1, add_symbol("car"),  &fn_car);
    add_env_entry_native(env, nativef1, add_symbol("cdr"),  &fn_cdr);
    add_env_entry_native(env, nativef, add_symbol("list"),  &fn_list);

    // Data types
    add_env_entry_native(env, nativef1, add_symbol("u8"),  &fn_u8);
    add_env_entry_native(env, nativef1, add_symbol("u16"), &fn_u16);
    add_env_entry_native(env, nativef1, add_symbol("u32"), &fn_u32);
    
    /* Mathematics operators */
    add_env_entry_native(env, nativef, add_symbol("+"), &fn_add);
    add_env_entry_native(env, nativef, add_symbol("-"), &fn_sub);
    add_env_entry_native(env, nativef, add_symbol("*"), &fn_mul);
    add_env_entry_native(env, nativef, add_symbol("/"), &fn_div); //"

    add_env_entry_native(env, nativef2, add_symbol("="), &fn_eq);
    add_env_entry_native(env, nativef2, add_symbol(">"), &fn_greater_than);
    add_env_entry_native(env, nativef2, add_symbol("<"), &fn_less_than);

    
    return env;
}
