#pragma once

#include <stdint.h>

#define MAX_SYM_LEN 128

/*
 * Sym
 */
/*
typedef struct sym {
    char* str;
    //TODO: add lisp data type
} sym_t;

sym_t* make_sym(const char* str);
void free_sym(sym_t* sym);
*/

/*
 * Cons struct and cons type enum
 */
typedef enum e_cons_type {
    Cons,
    Sym,
    Literal
} cons_type;

// cons cell, the core of any lisp
// TODO: rename to lisp object (technically not a cons since it can be sym)
typedef struct cons {
    cons_type type;
    union {
	// normal cons cell
	struct {
	    struct cons* car;
	    struct cons* cdr;
	};
	// sym
	struct {
	    //TODO: symbol_t and symbol table with lookup
	    char* val;
	};
	int numval;
    };
} cons_t;

cons_t* cons(cons_t* car, cons_t* cdr);
cons_t* sym(char* str);
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
