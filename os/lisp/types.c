#include "types.h"

#include "../kernel/kmalloc.h"
#include "../kernel/kstd.h"
#include "string.h"
#include "list.h"

Atom_t* make_atom(char* str)
{
    Atom_t* ret = kmalloc(sizeof(Atom_t));
    ret->val = kmalloc(strlen(str));
    strcopy(str, ret->val);
    return ret;
}

void free_atom(Atom_t* atom)
{
    kfree(atom);
}

SExpElem_t* make_sexp_elem(ElemType type, void* val)
{
    SExpElem_t* tmp = kmalloc(sizeof(SExpElem_t));
    tmp->type = type;
    tmp->val = val;
    return tmp;
}

void free_sexp_elem(SExpElem_t* elem)
{
    kfree(elem->val);
    kfree(elem);
}

SExp_t* make_sexp()
{
    SExp_t* tmp = kmalloc(sizeof(SExp_t));
    tmp->list = *make_list();
    return tmp;
}

void free_sexp(SExp_t* sexp)
{
    free_list(&sexp->list);
    kfree(sexp);
}

void sexp_add_elem(SExp_t* sexp, SExpElem_t* elem)
{
    list_append(&sexp->list, elem);
}

SExpElem_t* sexp_elem_at(SExp_t* sexp, int index)
{
    return (SExpElem_t*)list_elem_at(&sexp->list, index);
}
