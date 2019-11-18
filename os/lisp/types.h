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
    Cons, // a cons cell (car, cdr), use to make lists 
    Sym,  // a symbol (word), should/could be stored as symbol id
    Num,  // a literal number. stored as uint32_t
    Func, // a function. stored as args list and body
    Err   // an error. Somewhere in the eval or 
} lobj_type;

// cons cell, the core of any lisp
// TODO: rename to lisp object (technically not a cons since it can be sym)
typedef struct lobj {
    lobj_type type;
    union {
	// Cons
	struct {
	    struct lobj* car;
	    struct lobj* cdr;
	};
	
	// Sym
	struct {
	    //TODO: symbol val, symbol lookup
	    char* val;
	};
	
	// Num
	uint32_t numl;

	// Func
	//TODO: instead just use Cons?
	struct {
	    struct lobj* args;
	    struct lobj* body;
	};

	// Error
	struct {
	    uint8_t code;
	    char* msg;
	};
    };
} lobj_t;

lobj_t* cons(lobj_t* car, lobj_t* cdr);
lobj_t* sym(char* str);
lobj_t* num(int num);
lobj_t* func(lobj_t* args, lobj_t* body);
void free_lobj(lobj_t* elem);

lobj_t* append(lobj_t* list, lobj_t* elem);
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
