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
    tmp->args = args;
    tmp->body = body;
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

inline void append(lobj_t* list, lobj_t* elem)
{
    // emtpy list
    if(list == NULL)
	*list = *elem;
    
    if(list->type == Sym)
	return; //ERROR: can't access cdr of sym
	
    while(list->cdr != NULL)
	list = list->cdr;

    if(elem->type == Sym ||
       elem->type == Num)
	list->cdr = cons(elem, NULL);
    else
	list->cdr = elem;
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
