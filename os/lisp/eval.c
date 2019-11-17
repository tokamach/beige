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

    //TODO: check that funentry isn't NULL
    //TODO: type (signature) check

    /*
     * Before we can call fun, we need to eval our arguments,
     * and add them to a new env, the scope of the new function.
     * If the args are just literals or vars, then that's cool, but
     * they often need evaluating themselves i.e. (+ (* 2 5) 1)
     *
     * It can be said that calling eval(arg) provides us with a
     * guarantee that arg is in the form of a Sym or Num.
     */

    //TODO: make append return only (no side effects) and fix this hack
    lobj_t* evald_args = cons(NULL, NULL);
    LIST_ITER(args, i)
    {
	append(evald_args, eval(env, nth(args, i)));
    }
    evald_args = cdr(evald_args);

    // The value returned by this apply() call. Defaults to nil
    lobj_t* retval = NULL;
    switch(funentry->type)
    {
	/*
	 * A function implemented in Lisp.
	 */
    case lobj:
    {
	//TODO: check if entry isn't a function, error (can't call function of var)
	// Make a new env, inner to the current env.
	env_t* func_env = make_env(env);

	/* For each arg, evaluate it and add the result to the new env
	   with the name of the relative argname in the function definition */
	LIST_ITER(args, i)
	{
	    lobj_t* arg = nth(evald_args, i);
	    symbol_id argnm = lookup_symbol(nth(car(cdr(args)), i)->car->val);
	    add_env_entry_lobj(func_env, argnm, arg);
	}

	lobj_t* func = funentry->lobj;

	/* functions follow the format: 
	 * (lamdba fname (args) (<function>))
	 * now that */
	retval = eval(func_env, nth(func, 3));
	break;
    }
    
    /*
     * nativef means all args are passed to native function
     * nativef1-fn mean each arg is passed in its relative pos
     * to function
     */
    case nativef:
	retval = (*funentry->nativef)(env, evald_args);
	break;
	
    case nativef1:
	assert(argc == 1, "argc didn't match 1");
	retval = (*funentry->nativef)(env, nth(evald_args, 0));
	break;
	
    case nativef2:
	assert(argc == 2, "argc didn't match 2");
	retval = (*funentry->nativef2)(env, nth(evald_args, 0), nth(evald_args, 1));
	break;
	
    case empty:
	// Blank entry has been called like a fun, /should/ never happen
	// TODO: notify that something has gone wrong
	break;
    }

    free_env(env);
    return retval;
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

    /* how to explain this... */
    if(exp->car->type == Num)
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
