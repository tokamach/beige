#pragma once

#include <stdint.h>
#include <stddef.h>

#define MAX_SYM_LEN 128

/*
 * Types that a Lisp object can be, take note of the capital letter,
 * so as not to pollute the global scope
 */
typedef enum e_lobj_type {
    Cons, // a cons cell (car, cdr), use to make lists 
    Sym,  // a symbol (word), should/could be stored as symbol id
    Num,  // a number. stored as int32_t

    String, // a string of chars(u8)

    U8,   //  8 bit unsigned integer, used for systems stuff
    U16,  // 16 bit unsigned integer, as above
    U32,  // 32 bit unisgned int, ditto

    Func, // a function. stored as args list and body

    Err   // an error. Somewhere in the eval or 
} lobj_type;

/*
 * Any Object in Lisp is one of these. Each Object has a type, one of
 * the types listed above, and a value, as shown below. Note that an
 * unnamed union is used, so each of the below values is accessed 
 * directly. Make sure to access the right one based on the type.
 */
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
	int32_t numl;

	// u8, u16, u32
	uint8_t u8;
	uint16_t u16;
	uint32_t u32;

	// Func
	struct {
	    struct lobj* args;
	    struct lobj* body;
	    struct env_t* env; // Pointer to captured scope
	};

	// Error
	struct {
	    uint8_t errcode;
	    const char* errmsg;
	};
    };
} lobj_t;

/*
 * C functions to construct a new lisp object of their respective type.
 * These will call kmalloc, and should be the only Lisp code that does so.
 * (any GC registering logic will go in these)
 */
lobj_t* cons(lobj_t* car, lobj_t* cdr);
lobj_t* sym(char* str);
lobj_t* num(int32_t num);

lobj_t*  u8( uint8_t num);
lobj_t* u16(uint16_t num);
lobj_t* u32(uint32_t num);

lobj_t* func(lobj_t* args, lobj_t* body);

lobj_t* error(uint8_t code, const char *msg);
void free_lobj(lobj_t* elem);

/*
 * List operations for Cons cells.
 */
lobj_t* append(lobj_t* list, lobj_t* elem);
void append_inplace(lobj_t* list, lobj_t* elem);
size_t length(lobj_t* list);
lobj_t* nth(lobj_t* list, size_t n);

/*
 * C macros for lisp functions
 */
#define car(x) (((lobj_t*) x)->car)
#define cdr(x) (((lobj_t*) x)->cdr)
#define caar(x) (((lobj_t*) x)->car->car)
#define cadr(x) (((lobj_t*) x)->car->cdr)
#define cdar(x) (((lobj_t*) x)->cdr->car)
#define cddr(x) (((lobj_t*) x)->cdr->cdr)

#define keval_lisp(x) eval(kenv, lisp_read(#x))
#define kprint_lisp(x) print_sexp(eval(kenv, lisp_read(#x)))
#define kprinteq_lisp(x) print_sexp(lisp_read(#x)); k_print(" => "); print_sexp(eval(kenv, lisp_read(#x))); k_print("\n");
