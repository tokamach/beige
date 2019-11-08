#include "eval.h"
#include "types.h"
#include "env.h"

#include <stddef.h>

cons_t* apply(env_t* env, cons_t* fun, cons_t* args)
{
    if(args->car->type != Atom &&
       args->car->type != Literal)
    {
	k_println("lisperr: car of args wasn't an atom or literal");
	return NULL;
    }

    env_entry_t* funentry = get_env_entry(env, fun->val);
    
    switch(funentry->type)
    {
    case lispf:
	//dunno waht to do lol
	break;

    case nativef1:
	return (*funentry->nativef1)(args->car->val);
	break;

    case nativef2:
	return (*funentry->nativef2)((int)args->car->numval, (int)args->cdr->car->numval);
	break;
    }
}

cons_t* eval(env_t* env, cons_t* exp)
{
    if(exp == NULL)
	return NULL;

    if(exp->type == Literal ||
       exp->type == Atom)
    {
	//atoms and literals evaluate to themselves
	return exp;
    }

    //we're a cons, apply as a function
    cons_t* func = car(exp);
    cons_t* args = cdr(exp);

    cons_t* result = apply(env, func, args);

    return result;
}
