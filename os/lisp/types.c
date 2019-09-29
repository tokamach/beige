#include "types.h"

#ifdef LISP_TEST
#include "../tests/kernel_mappings.h"
#endif

#include "../kernel/kmalloc.h"
#include "string.h"
#include "list.h"

Atom_t* make_atom(char* str)
{
    Atom_t* ret = malloc(sizeof(Atom_t));
    ret->val = *make_string(str);
    return ret;
}

void free_atom(Atom_t* atom)
{
    free(atom);
}

SExpElem_t* make_sexp_elem(ElemType type, void* val)
{
    SExpElem_t* tmp = malloc(sizeof(SExpElem_t));
    tmp->type = type;
    tmp->val = val;
    return tmp;
}

void free_sexp_elem(SExpElem_t* elem)
{
    free(elem->val);
    free(elem);
}

SExp_t* make_sexp()
{
    SExp_t* tmp = malloc(sizeof(SExp_t));
    tmp->list = *make_list();
    return tmp;
}

void free_sexp(SExp_t* sexp)
{
    free_list(&sexp->list);
    free(sexp);
}

void sexp_add_elem(SExp_t* sexp, SExpElem_t* elem)
{
    list_append(&sexp->list, elem);
}

SExpElem_t* sexp_elem_at(SExp_t* sexp, int index)
{
    return (SExpElem_t*)list_elem_at(&sexp->list, index);
}
