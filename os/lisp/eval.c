#include "eval.h"
#include "types.h"
#include "env.h"
#include "defun.h" //for LIST_ITER

#include "../kernel/kassert.h"

#ifdef LISP_TEST
#include <stdio.h>
#include "../tests/kernel_mappings.h"
#endif

#include <stddef.h>

/*
 * Apply the arglist in args to the function fun.
 * env: the environment/ns to eval in
 * fun: symbol containing the name of the function to call
 * args: list of arguments
 */
lobj_t* apply(env_t* env, lobj_t* fun, lobj_t* args)
{
    /*
     * TODO: check if fun is symbol pointing to function
     * or a function itself
     */
    
    env_entry_t* funentry = get_env_entry(env, fun->car->val);
    uint8_t argc = length(args);

    //TODO: type (signature) check

    //TODO: eval args
    /*
     * If args are not nil, we create a new env, mapping the
     * function specified args to our supplied args
     */

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
	lobj_t* func = funentry->lispf;
	//TODO: check argc
	//TODO: eval each earg, create subenv, call func
	break;
    }
    /*
     * nativef means all args are passed to native function
     * nativef1-fn mean each arg is passed in its relative pos
     * to function
     */
    case nativef:
	return (*funentry->nativef)(env, args);
	break;
    case nativef1:
	assert(argc == 1, "argc didn't match 1");
	return (*funentry->nativef)(env, nth(args, 0));
    case nativef2:
	assert(argc == 2, "argc didn't match 2");
	return (*funentry->nativef2)(env, nth(args, 0), nth(args, 1));
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

lobj_t* eval(env_t* env, lobj_t* exp)
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
    lobj_t* func = exp;
    lobj_t* args = cdr(exp);

    lobj_t* result = apply(env, func, args);

    return result;
}
