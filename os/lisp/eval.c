#include "eval.h"
#include "types.h"
#include "env.h"

#include "../kernel/kassert.h"

#include <stddef.h>

/*
 * Apply the arglist in args to the function fun.
 * env: the environment/ns to eval in
 * fun: symbol containing the name of the function to call
 * args: list of arguments
 */
cons_t* apply(env_t* env, cons_t* fun, cons_t* args)
{
    env_entry_t* funentry = get_env_entry(env, fun->val);
    uint8_t argc = length(args);

    //TODO: type (signature) check

    //TODO: eval args

    /*
     * Before we can call fun, we need to eval our arguments
     * If they're just literals or vars, then that's cool, but
     * they often need evaluating themselves i.e. (+ (* 2 5) 1)
     *
     * Calling eval(arg) guarantees arg is in the form of
     * a Sym or Num.
     */
    switch(funentry->type)
    {
	/*
	 * A function implemented in Lisp.
	 */
    case lispf:
    {
	cons_t* func = funentry->lispf;
	//TODO: check argc
	//TODO: eval each earg, create subenv, call func
	break;
    }
    case nativef:
	return (*funentry->nativef)(env, args);
	break;
    case syml:
	/* ERROR. Attempted to call symbol 'x' as a function */
	break;
    case numl:
	/* ERROR. Attempted to call Num 'x' as a function */
	break;
    case empty:
	// Blank entry has been called like a fun, /should/ never happen
	// TODO: notify that something has gone wrong
	break;
    }
}

cons_t* eval(env_t* env, cons_t* exp)
{
    /* eval nil, return nil */
    if(exp == NULL)
	return NULL;

    /* Sym and Num evaluate to themselves */
    if(exp->type == Num ||
       exp->type == Sym)
	return exp;

    //TODO: handle quoted list

    /*
     * Since we're not a Sym or Num, we know that we're a function
     * 
     */
    cons_t* func = car(exp);
    cons_t* args = cdr(exp);

    cons_t* result = apply(env, func, args);

    return result;
}
