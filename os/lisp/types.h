#pragma once

#include <stdint.h>
#include <stddef.h>

#define MAX_SYM_LEN 128

/*
 * Sym
 */

/*
 * Cons struct and cons type enum
 */
typedef enum e_cons_type {
    Cons,
    Sym,
    Num
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
	int numl;
    };
} cons_t;

cons_t* cons(cons_t* car, cons_t* cdr);
cons_t* sym(char* str);
cons_t* literal(int num);
void free_cons(cons_t* elem);

void append(cons_t* list, cons_t* elem);
size_t length(cons_t* list);
cons_t* nth(cons_t* list, size_t n);

/*
 * C macros for lisp functions
 */
//#define cons(x, y) make_cons(x, y)
#define car(x) (((cons_t*) x)->car)
#define cdr(x) (((cons_t*) x)->cdr)

#define keval_lisp(x) eval(kenv, lisp_read(#x))
#define kprint_lisp(x) print_sexp(eval(kenv, lisp_read(#x)))
#define kprinteq_lisp(x) print_sexp(lisp_read(#x)); k_print("=>"); print_sexp(eval(kenv, lisp_read(#x)))
