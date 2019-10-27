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

cons_t* atom(char* str)
{
    cons_t* tmp = kmalloc(sizeof(cons_t));
    tmp->type = Atom;
    tmp->val = kmalloc(sizeof(char) * strlen(str));
    strcopy(str, tmp->val);
    return tmp;
}

void free_cons(cons_t* elem)
{
    if(elem->type == Cons)
    {
	free_cons(elem->car);
	free_cons(elem->cdr);
    }
    else
      kfree(elem->val);
    
    kfree(elem);
}

void append(cons_t* list, cons_t* elem)
{
    if(list->type == Atom)
	return; //ERROR: can't access cdr of atom
	
    while(list->cdr != NULL)
	list = list->cdr;

    list->cdr = elem;
}

/*
sexp_t* make_sexp()
{
    sexp_t* tmp = kmalloc(sizeof(sexp_t));
    tmp->first = NULL;
    tmp->size  = 0;
    return tmp;
}

void free_sexp(sexp_t* sexp)
{
    cons_t* nextnode = sexp->first;
    cons_t* tmp = nextnode;

    //waht the fuck is this
    while(nextnode)
    {
	tmp = nextnode;
	nextnode = nextnode->next;
	free_sexp_elem(tmp);
    }

    kfree(sexp);
}

cons_t* sexp_elem_at(sexp_t* sexp, int index)
{
    cons_t* tmp = sexp->first;
    
    if(!tmp)
	return NULL;
    
    for(int i = 0; i < index; i++)
    {
	tmp = tmp->next;
	if(!tmp)
	    while(1) {k_print("LISTERR");} //out of range, hang TODO: actual err
	
    }

    return tmp;
}

void sexp_add_elem(sexp_t* sexp, cons_t* new)
{
    cons_t* end = sexp_elem_at(sexp, sexp->size - 1);
    
    if(!end)
    {
	//list is empty
	sexp->first = new;
    }
    else
    {
	end->next = new;	
    }
    
    sexp->size++;
}

void sexp_pop(sexp_t* sexp)
{
    //get last elem
    cons_t* tmp = sexp_elem_at(sexp, sexp->size - 1);
    //free it
    free_sexp_elem(tmp);

    //set penultimate elem's next to null, reduce sexp size
    sexp_elem_at(sexp, sexp->size - 2)->next = NULL;
    sexp->size--;
}
*/
