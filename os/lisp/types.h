#pragma once

#include <stdint.h>
#include "list.h"

#define MAX_ATOM_LEN 128

//Atom
typedef struct Atom {
    char* str;
    //TODO: add lisp data type
} Atom_t;

Atom_t* make_atom(const char* str);
void free_atom(Atom_t* atom);

typedef enum e_elem_type {
    Atom,
    List
} ElemType;

typedef struct SExp SExp_t;
//Boxed void pointer with "type" annontation
typedef struct SExpElem {
    ElemType type;
    union {
	Atom_t* atom;
	SExp_t* list;
    } val; //either List_t or Atom
    struct SExpElem* next;
    
} SExpElem_t;

SExpElem_t* make_sexp_elem_atom(Atom_t* atom);
SExpElem_t* make_sexp_elem_list(SExp_t* list);
void free_sexp_elem(SExpElem_t* elem);

//Sexp
typedef struct SExp {
    //List_t list;
    SExpElem_t* first;
    int size;
} SExp_t;

SExp_t* make_sexp();
void free_sexp(SExp_t* sexp);
void sexp_add_elem(SExp_t* sexp, SExpElem_t* elem);
SExpElem_t* sexp_elem_at(SExp_t* sexp, int index);
