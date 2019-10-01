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

Atom_t* make_atom(const char* str)
{
    Atom_t* ret = kmalloc(sizeof(Atom_t));
    ret->str = kmalloc(strlen(str));
    strcopy(str, ret->str);
    return ret;
}

void free_atom(Atom_t* atom)
{
    kfree(atom);
}

SExpElem_t* make_sexp_elem_atom(Atom_t* atom)
{
    SExpElem_t* tmp = kmalloc(sizeof(SExpElem_t));
    tmp->type = Atom;
    tmp->val.atom = atom;
    tmp->next = NULL;
    return tmp;
}

SExpElem_t* make_sexp_elem_list(SExp_t* list)
{
    SExpElem_t* tmp = kmalloc(sizeof(SExpElem_t));
    tmp->type = List;
    tmp->val.list = list;
    tmp->next = NULL;
    return tmp;
}

void free_sexp_elem(SExpElem_t* elem)
{
    if(elem->type == List)
	free_sexp(elem->val.list);
/*    else
      kfree(elem->val.atom); */
    
    kfree(elem);
}

SExp_t* make_sexp()
{
    SExp_t* tmp = kmalloc(sizeof(SExp_t));
    tmp->first = NULL;
    tmp->size  = 0;
    return tmp;
}

void free_sexp(SExp_t* sexp)
{
    SExpElem_t* nextnode = sexp->first;
    SExpElem_t* tmp = nextnode;

    //waht the fuck is this
    while(nextnode)
    {
	tmp = nextnode;
	nextnode = nextnode->next;
	free_sexp_elem(tmp);
    }

    kfree(sexp);
}

SExpElem_t* sexp_elem_at(SExp_t* sexp, int index)
{
    SExpElem_t* tmp = sexp->first;
    
    if(!tmp)
	return NULL;
    
    for(int i = 0; i < index; i++)
    {
	tmp = tmp->next;
	/*if(!tmp)
	    while(1) {k_print("LISTERR");} //out of range, hang TODO: actual err
	*/
    }

    return tmp;
}

void sexp_add_elem(SExp_t* sexp, SExpElem_t* new)
{
    SExpElem_t* end = sexp_elem_at(sexp, sexp->size - 1);
    
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

void sexp_pop(SExp_t* sexp)
{
    //get last elem
    SExpElem_t* tmp = sexp_elem_at(sexp, sexp->size - 1);
    //free it
    free_sexp_elem(tmp);

    //set penultimate elem's next to null, reduce sexp size
    sexp_elem_at(sexp, sexp->size - 2)->next = NULL;
    sexp->size--;
}
