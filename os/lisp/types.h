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
typedef enum e_lobj_type {
    Cons,
    Sym,
    Num
} lobj_type;

// cons cell, the core of any lisp
// TODO: rename to lisp object (technically not a cons since it can be sym)
typedef struct lobj {
    lobj_type type;
    union {
	// normal cons cell
	struct {
	    struct lobj* car;
	    struct lobj* cdr;
	};
	// sym
	struct {
	    //TODO: symbol val, symbol lookup
	    char* val;
	};
	uint32_t numl;
    };
} lobj_t;

lobj_t* cons(lobj_t* car, lobj_t* cdr);
lobj_t* sym(char* str);
lobj_t* num(int num);
void free_lobj(lobj_t* elem);

void append(lobj_t* list, lobj_t* elem);
size_t length(lobj_t* list);
lobj_t* nth(lobj_t* list, size_t n);

/*
 * C macros for lisp functions
 */
//#define cons(x, y) make_cons(x, y)
#define car(x) (((lobj_t*) x)->car)
#define cdr(x) (((lobj_t*) x)->cdr)

#define keval_lisp(x) eval(kenv, lisp_read(#x))
#define kprint_lisp(x) print_sexp(eval(kenv, lisp_read(#x)))
#define kprinteq_lisp(x) print_sexp(lisp_read(#x)); k_print("=>"); print_sexp(eval(kenv, lisp_read(#x)))
