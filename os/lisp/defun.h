#pragma once

/*
 * Shorthand for defining a Lisp native funtion. Always returns a Lisp
 * object, and always takes an arg and an env.
 */
#define DEFUN(fnname, envnm, argnm) lobj_t* fnname(env_t* envnm, lobj_t* argnm)

/*
 * Macro for iterating through a list. 
 */
#define LIST_ITER(list, iterlbl) for(size_t iterlbl = 0; iterlbl < length(list); i++)
