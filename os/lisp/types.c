#include "types.h"

#ifdef LISP_TEST
//testin
#include <stdio.h>
#include "../tests/kernel_mappings.h"
#else
//real world
#include "../kernel/kmalloc.h"
#include "../kernel/kterm.h"
#endif

lobj_t* cons(lobj_t* car, lobj_t* cdr)
{
    lobj_t* tmp = kmalloc(sizeof(lobj_t));
    tmp->type = Cons;
    tmp->car = car;
    tmp->cdr = cdr;
    return tmp;
}

lobj_t* sym(char* str)
{
    lobj_t* tmp = kmalloc(sizeof(lobj_t));
    tmp->type = Sym;
    tmp->val = kmalloc(sizeof(char) * strlen(str));
    strcopy(str, tmp->val);
    return tmp;
}

lobj_t* num(int val)
{
    lobj_t* tmp = kmalloc(sizeof(lobj_t));
    tmp->type = Num;
    tmp->numl = val;
    return tmp;    
}

lobj_t* func(lobj_t* args, lobj_t* body)
{
    lobj_t* tmp = kmalloc(sizeof(lobj_t));
    tmp->type = Func;
    tmp->args = args;
    tmp->body = body;
    return tmp;
}

lobj_t* error(uint8_t code, const char* msg)
{
    lobj_t* tmp = kmalloc(sizeof(lobj_t));
    tmp->type = Err;
    tmp->errcode = code;
    tmp->errmsg = msg;
    return tmp;
}

void free_lobj(lobj_t* elem)
{
    if(elem->type == Cons)
    {
	free_lobj(elem->car);
	free_lobj(elem->cdr);
    }
    else if(elem->type == Sym)
	kfree(elem->val);
    /*else if(elem->type == Num)
      kfree(elem->numl); */
    
    kfree(elem);
}

void append_inplace(lobj_t* list, lobj_t* elem)
{
    if(list == NULL)
    {
	/* 
	 * as we can't return a new value, we'll just do nothing 
	 * this isn't good though. It shouldn't happen
	 */
	k_print("([ERR] cannot append to NULL)");
	return;
    }

    if(list->car == NULL &&
       list->cdr == NULL)
    {
	/* 
	 * This list is empty.
	 */

	list->car = elem;
    }


    lobj_t* end = list;
    while(end->cdr != NULL)
	end = end->cdr;
    
    end->cdr = cons(elem, NULL);   
}

lobj_t* append(lobj_t* list, lobj_t* elem)
{
    // emtpy list
    if(list == NULL)
    {
	if(elem == NULL)
	{
	    return NULL;
	}
	/*if(elem->type == Cons)
	{
	    return elem;
	    }*/
	else
	{
	    /* make a list with 1 element */
	    return cons(elem, NULL);
	}
    }
    /* Are we even adding to a list? */
    else if(list->type != Cons)
    {
	/* are we adding a list to our not list */
	if(elem->type == Cons)
	{
	    return cons(list, elem);
	}
	else
	{
	    /* 
	     * We actually have two elements, no matter
	     * we can just put them in a list!
	     */
	    return cons(list, cons(elem, NULL));	    
	}
    }
    else
    {
	//TODO: if possible make this no sideeffect
	/* list is a list, we should seek to the end */
	lobj_t* end = list;
	while(end->cdr != NULL)
	    end = end->cdr;

	if(elem->type == Cons)
	{
	    end->cdr = elem;
	    return list;
	}
	else
	{
	    end->cdr = cons(elem, NULL);
	    return list;
	}
    }

    return error(1, "append broke");
}

inline size_t length(lobj_t* list)
{
    //TODO: assert
    //we can't find the length of a list with no cdr
    if(list->type != Cons)
	return 1;

    size_t len;
    for(len = 1; list->cdr != NULL; len++)
	list = list->cdr;

    return len;
}

inline lobj_t* nth(lobj_t* list, size_t n)
{
    for(size_t i = 0; i < n; i++)
	list = list->cdr;

    return list->car;
}
