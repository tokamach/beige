#include "eval.h"
#include "types.h"
#include "env.h"

#include "../kernel/kassert.h"

#include <stddef.h>

cons_t* apply(env_t* env, cons_t* fun, cons_t* args)
{
    if(args->car->type != Sym &&
       args->car->type != Num)
    {
	k_println("lisperr: car of args wasn't an sym or num");
	return NULL;
    }

    env_entry_t* funentry = get_env_entry(env, fun->val);
    uint8_t argc = length(args);

    //TODO: type (signature) check
    
    switch(funentry->type)
    {
    case lispf:
    {
	cons_t* func = funentry->lispf;
	//TODO: substitute vars, execute code
	break;
    }
    case nativef1:
	assert(argc == 1);
	return (*funentry->nativef1)(args->car->val);
	break;

    case nativef2:
	assert(argc == 2);
	return (*funentry->nativef2)((int)args->car->numl, (int)args->cdr->car->numl);
	break;
    }
}

cons_t* eval(env_t* env, cons_t* exp)
{
    if(exp == NULL)
	return NULL;

    if(exp->type == Num ||
       exp->type == Sym)
    {
	//syms and nums evaluate to themselves
	return exp;
    }

    //we're a cons, apply as a function
    cons_t* func = car(exp);
    cons_t* args = cdr(exp);

    cons_t* result = apply(env, func, args);

    return result;
}
