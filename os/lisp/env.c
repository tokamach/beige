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
uint8_t symbol_count = 0;

uint8_t add_symbol(char* symbol)
{
    //TODO: sanity check
    symbol_table[symbol_count] = (symbol_t){symbol_count, symbol};
    return symbol_count++;
}

uint8_t lookup_symbol(char *symbol)
{
    for(int i = 0; i < MAX_SYMBOLS; i++)
	if(streq(symbol_table[i].symbol, symbol))
	    return i;

    //fuck dis won't work
    return -1;
}

char* lookup_id(uint8_t i)
{
    return symbol_table[i].symbol;
}


/*
 * Env Stuff
 */
void add_env_entry_cons(env_t* env, env_entry_type type, char* sym, cons_t* val)
{
    env_entry_t* ret = &env->entries[env->entry_count];
    ret->type = type;
    ret->sym = add_symbol(sym);

    if(type == lispf)
    {
	ret->lispf = val;
    }
    else if(type == syml)
    {
	ret->sym = val;
    }

    env->entry_count++;
}

void add_env_entry_native(env_t* env, char* sym, void* fun)
{
    env_entry_t* ret = &env->entries[env->entry_count];
    ret->type = nativef;
    ret->sym = add_symbol(sym);
    ret->nativef = fun;
}

env_entry_t* get_env_entry(env_t* env, char* sym)
{
    uint8_t symid = lookup_symbol(sym);
    
    for(int i = 0; i < env->entry_count; i++)
    {
	if(env->entries[i].sym == symid)
	    return &env->entries[i];
    }

    //not in current env, look in outer
    if(env->outer)
	return get_env_entry(env->outer, sym);
    else
	return NULL;
}

/*
 * Initalizing the defualt lisp environment
 * this contains the basic lisp functions
 * which others can be built upon
 */

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

/*
 * Lisp fundamentals
 */
cons_t* car_func(env_t* env, cons_t* list)
{
    assert(list->type == Cons);
    return list->car;
}

cons_t* cdr_func(env_t* env, cons_t* list)
{
    assert(list->type == Cons);
    return list->cdr;
}

/*
 * Maths
 */
DEFUN(add, env, args)
{
    size_t total = 0;
    LITER(args, i)
    {
	//total += nth(args, i)->car->numl;
    }
	
    return num(total);
}
/*
cons_t* sub(size_t a, size_t b)
{
    return num(a - b);
}

cons_t* mul(size_t a, size_t b)
{
    return num(a * b);
}

cons_t* div(size_t a, size_t b)
{
    return num(a / b);
}

cons_t* quote(cons_t* exp)
{
    return cons(sym("quote"), exp);
}
*/
env_t* make_base_env()
{
    //TODO: add defun, let, etc to env
    //make an empty env with no outer
    env_t* env = make_env(NULL);

    /* Lisp Fundamentals */
    add_env_entry_native(env, "car", &car_func);
    add_env_entry_native(env, "cdr", &cdr_func);

    /* Mathematics operators */
    add_env_entry_native(env, "add", &add);
    /*add_env_entry_native(env, nativef, "sub", &sub);
    add_env_entry_native(env, nativef, "mul", &mul);
    add_env_entry_native(env, nativef, "div", &div);
    add_env_entry_native(env, nativef, "quote", &quote);*/
    
    return make_env(NULL);
}

/*
 * Kernel env stuff. here we define the functions that the kernel
 * will probably want to use, memory access, inb/outb
 */

env_t* make_kernel_env()
{
    env_t* env = make_env(make_base_env());
    

    //k_print_hex(get_env_entry(env, "add")->nativef2);
    return env;
}
