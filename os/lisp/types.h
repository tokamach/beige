#pragma once

#include <stdint.h>
#include "list.h"
#include "string.h"

#define MAX_ATOM_LEN 128

//Atom
typedef struct Atom {
    char* val;
    //TODO: add lisp data type
} Atom_t;

Atom_t* make_atom(char* str);
void free_atom(Atom_t* atom);

typedef enum e_elem_type {
    Atom,
    List
} ElemType;

//Boxed void pointer with "type" annontation
typedef struct SExpElem {
    ElemType type;
    void* val; //either List_t or Atom
} SExpElem_t;

SExpElem_t* make_sexp_elem(ElemType type, void* val);
void free_sexp_elem(SExpElem_t* elem);

//Sexp
typedef struct SExp {
    List_t list;
} SExp_t;

SExp_t* make_sexp();
void free_sexp(SExp_t* sexp);
void sexp_add_elem(SExp_t* sexp, SExpElem_t* elem);
SExpElem_t* sexp_elem_at(SExp_t* sexp, int index);

//old:
typedef struct LCons {
    void* car;
    void* cdr;
} LCons_t;
