#pragma once

#include <stdint.h>

#define MAX_ATOM_LEN 128

/*
 * Atom
 */
/*
typedef struct atom {
    char* str;
    //TODO: add lisp data type
} atom_t;

atom_t* make_atom(const char* str);
void free_atom(atom_t* atom);
*/

/*
 * Cons struct and cons type enum
 */
typedef enum e_cons_type {
    Cons,
    Atom,
    Literal
} cons_type;

// cons cell, the core of any lisp
// TODO: rename to lisp object (technically not a cons since it can be atom)
typedef struct cons {
    cons_type type;
    union {
	// normal cons cell
	struct {
	    struct cons* car;
	    struct cons* cdr;
	};
	// atom
	struct {
	    //TODO: symbol_t and symbol table with lookup
	    char* val;
	};
	int numval;
    };
} cons_t;

cons_t* cons(cons_t* car, cons_t* cdr);
cons_t* atom(char* str);
cons_t* literal(int num);
void free_cons(cons_t* elem);

void append(cons_t* list, cons_t* elem);

/*
 * C macros for lisp functions
 */
//#define cons(x, y) make_cons(x, y)
#define car(x) (((cons_t*) x)->car)
#define cdr(x) (((cons_t*) x)->cdr)


/*
 * Sexp
 */

/*
sexp_t* make_sexp();
void free_sexp(cons_t* sexp);
void sexp_add_elem(cons_t* sexp, cons_t* elem);
cons_t* sexp_elem_at(cons_t* sexp, int index);
*/

/*
 * Env
 */
