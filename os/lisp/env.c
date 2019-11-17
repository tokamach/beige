#include "env.h"

#include "types.h"
#include "defun.h"

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
    env->entry_count++;

    /*if(type == lispf)
    {
	ret->lispf = val;
    }
    else if(type == syml)
    {
	ret->syml = val->syml;
    }
    else if(type == numl)
    {
	ret->sym = numl;
	}*/
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

env_entry_t* get_env_entry(env_t* env, char* sym)
{
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
 * Lisp fundamentals
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

/*
 * Maths
 */
lobj_t* add(env_t* env, lobj_t* args)
{
    size_t total = 0;
    LIST_ITER(args, i)
    {
	total += nth(args, i)->car->numl;
    }
	
    return num(total);
}
/*
lobj_t* sub(size_t a, size_t b)
{
    return num(a - b);
}

lobj_t* mul(size_t a, size_t b)
{
    return num(a * b);
}

lobj_t* div(size_t a, size_t b)
{
    return num(a / b);
}

qlobj_t* quote(lobj_t* exp)
{
    return cons(sym("quote"), exp);
}
*/

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
    add_env_entry_native(env, nativef1, add_symbol("car"), &fn_car);
    add_env_entry_native(env, nativef1, add_symbol("cdr"), &fn_cdr);

    /* Mathematics operators */
    add_env_entry_native(env, nativef, add_symbol("add"), &add);
    /*add_env_entry_native(env, nativef, "sub", &sub);
    add_env_entry_native(env, nativef, "mul", &mul);
    add_env_entry_native(env, nativef, "div", &div);
    add_env_entry_native(env, nativef, "quote", &quote);*/
    
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
