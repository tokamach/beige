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

size_t count_env_size(env_t* env)
{
    if(env->outer)
	return env->entry_count + count_env_size(env->outer);
    else
	return env->entry_count;
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
