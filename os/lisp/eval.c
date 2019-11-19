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
 * fun: symbol containing function or name of the function to call
 * args: list of arguments
 */
lobj_t* apply(env_t* env, lobj_t* fun, lobj_t* args)
{
    /*
     * The entry in the env containing our function. We need
     * to keep this around in case we're calling a nativef,
     * of which funentry will have a pointer to, and arg info
     */
    env_entry_t* funentry = NULL;
    
    // This is the value we will return. By default it's nil.
    lobj_t* retval = NULL;

    // Number of args
    uint8_t argc = length(args);    

    /*
     * As we're a Lisp, we don't any side effects. If we eval
     * args, we don't want to modify them in place. We'll put
     * them here instead.
     */ 
    lobj_t* evald_args = NULL;
    
    if(fun->type == Sym)
    {
	/*
	 * fun is a symbol referencing a function in the env.
	 * we can lookup the function in the env:
	 */
	funentry = get_env_entry(env, fun->val);

	// The requested function doesn't exist in the env. Uh oh.
	if(funentry == NULL)
	{
	    //TODO: handle errors nicely, I think add an lobj type Err
	    return NULL;
	}

	// funentry is a /special form/. It may do macro like things
	if(funentry->type == special)
	{
	    /*
	     * Special forms go here. define, defun, etc.
	     * These function similarly to macros, where we don't want
	     * the args to be eval'd before execution. We'll let the
	     * functions themselves handle their arguments, (for now...)
	     * so we'll just pass them the args list wholesale.
	     */

	    // if we need to figure stuff out, do it here
	}
	else
	{
	    /* 
	     * Whatever funentry is, these types want the args to be eval'd.
	     * Before we switch on them, we'll eval the args
	     *
	     * We should only reach this point if we're sure we're working
	     * with a function, and not a special form or macro. As such we
	     * can safely eval all our args, without fear they aren't defined
	     * as in the case of (define x 10), where x is undefined, but
	     * attempting to eval it will just cause problems.
	     *
	     * Before we can call fun, we need to eval our arguments,
	     * and add them to a new env, the scope of the new function.
	     * If the args are just literals or vars, then that's cool, but
	     * they often need evaluating themselves i.e. (+ (* 2 5) 1)
	     *
	     * It can be said that calling eval(arg) provides us with a
	     * guarantee that arg is in the form of a Sym or Num.
	     */
    
	    for(int i = 0; i < argc; i++)
	    {
		evald_args = append(evald_args, eval(env, nth(args, i)));
	    }
	}
    }
    else if(fun->type == Func)
    {
	/*
	 * fun is a lambda. instead of retreiving the function
	 * from funentry, we already have it in fun
	 */

	//TODO: figure out how to do this
    }

    switch(funentry->type)
    {
    case empty:
	// Blank entry has been called like a fun, /should/ never happen
	// TODO: notify that something has gone wrong
	return error(1, "Attempted to call empty");
	break;

	/*
	 * A special form like
	 */
    case special:
	retval = (*funentry->special)(env, args);
	break;

	/*
	 * A function implemented in Lisp. It should be of type Func
	 */
    case lobj:
    {
	lobj_t* func = funentry->lobj;
	assert(func->type == Func, "Attempted to call non Func as function");
	
	// Make a new env, inner to the current env.
	env_t* func_env = make_env(env);

	/* For each arg, evaluate it and add the result to the new env
	   with the name of the relative argname in the function definition */
	LIST_ITER(args, i)
	{
	    lobj_t* arg = nth(evald_args, i);
	    symbol_id argnm = add_symbol(nth(func->args, i)->val); //will return id if symbol already exists
	    add_env_entry_lobj(func_env, argnm, arg);
	}

	/* functions follow the format: 
	 * (lamdba fname (args) (<function>))
	 * now that */
	retval = eval(func_env, func->body);

	// we've now exited the function, and will no longer need this env
	free_env(func_env);
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

    case nativef3:
	assert(argc == 3, "argc didn't match 3");
	retval = (*funentry->nativef3)(env,
				       nth(evald_args, 0),
				       nth(evald_args, 1),
				       nth(evald_args, 2));

	
    }

    return retval;
}

/*
 * Things you /can/ pass to eval:
 * (add 1 2)
 * 1
 * add
 * (lambda (a) (ptr_dref (ptr a))
 *
 * Things you /can't/ pass to eval (so stop trying to)
 * (1 2 3)
 * Cons((add 1 2), nil)
 */
lobj_t* eval(env_t* env, lobj_t* exp)
{
    /* eval nil, return nil */
    if(exp == NULL)
	return NULL;

    /* Num evaluate to itself */
    if(exp->type == Num)
	return exp;

    /* unqoted Sym evaluates to it's value */
    //TODO: check value isn't null
    if(exp->type == Sym)
	return get_env_entry(env, exp->val)->lobj;

    //TODO: handle quoted list

    /*
     * Since we're not a Sym or Num, we know that we're a function
     * 
     */
    lobj_t* func = car(exp);
    lobj_t* args = cdr(exp);

    lobj_t* result = apply(env, func, args);

    return result;
}
