#include "env.h"

#include "types.h"

#include "../kernel/kstd.h"
#include "../kernel/kmalloc.h"
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

void add_env_entry_native(env_t* env, env_entry_type type, char* sym, void* fun)
{
    env_entry_t* ret = &env->entries[env->entry_count];
    ret->type = type;
    ret->sym = add_symbol(sym);

    switch(type)
    {
    case nativef1:
	ret->nativef1 = fun;
	break;

    case nativef2:
	ret->nativef2 = fun;
	break;

    case nativef3:
	ret->nativef3 = fun;
	break;

    case nativef4:
	ret->nativef4 = fun;
	break;

    case nativef5:
	ret->nativef5 = fun;
	break;

    default:
	break;
    }

    env->entry_count++;
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
cons_t* car_func(cons_t* list)
{
    assert(list->type == Cons);
    return list->car;
}

cons_t* cdr_func(cons_t* list)
{
    assert(list->type == Cons);
    return list->cdr;
}

/*
 * Maths
 */
cons_t* add(size_t a, size_t b)
{
    return num(a + b);
}

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

env_t* make_base_env()
{
    //TODO: add defun, let, etc to env
    //make an empty env with no outer
    env_t* env = make_env(NULL);

    /* Lisp Fundamentals */
    add_env_entry_native(env, nativef1, "car", &car_func);
    add_env_entry_native(env, nativef1, "cdr", &cdr_func);

    /* Mathematics operators */
    add_env_entry_native(env, nativef2, "add", &add);
    add_env_entry_native(env, nativef2, "sub", &sub);
    add_env_entry_native(env, nativef2, "mul", &mul);
    add_env_entry_native(env, nativef2, "div", &div);
    add_env_entry_native(env, nativef1, "quote", &quote);
    
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
