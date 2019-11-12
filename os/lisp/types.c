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

cons_t* cons(cons_t* car, cons_t* cdr)
{
    cons_t* tmp = kmalloc(sizeof(cons_t));
    tmp->type = Cons;
    tmp->car = car;
    tmp->cdr = cdr;
    return tmp;
}

cons_t* sym(char* str)
{
    cons_t* tmp = kmalloc(sizeof(cons_t));
    tmp->type = Sym;
    tmp->val = kmalloc(sizeof(char) * strlen(str));
    strcopy(str, tmp->val);
    return tmp;
}

cons_t* num(int val)
{
    cons_t* tmp = kmalloc(sizeof(cons_t));
    tmp->type = Num;
    tmp->numl = val;
    return tmp;    
}

void free_cons(cons_t* elem)
{
    if(elem->type == Cons)
    {
	free_cons(elem->car);
	free_cons(elem->cdr);
    }
    else if(elem->type == Sym)
	kfree(elem->val);
    else if(elem->type == Num)
	kfree(elem->numl);
    
    kfree(elem);
}

void append(cons_t* list, cons_t* elem)
{
    if(list->type == Sym)
	return; //ERROR: can't access cdr of sym
	
    while(list->cdr != NULL)
	list = list->cdr;

    list->cdr = elem;
}

inline size_t length(cons_t* list)
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

inline cons_t* nth(cons_t* list, size_t n)
{
    for(size_t i = 0; i < n; i++)
	list = list->cdr;

    return list->car;
}
