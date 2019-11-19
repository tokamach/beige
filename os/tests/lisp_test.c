#include "../lisp/parser.h"
#include "../lisp/types.h"
#include "../lisp/env.h"
#include "../lisp/eval.h"

#include "../kernel/kstd.h"

int main()
{
    env_t* env = make_base_env();
    lobj_t* ast2 = lisp_read("(define cadr (lambda (l) (car (cdr l))))");
    lobj_t* ast1 = lisp_read("(define l (quote 2 3 4)))");
    lobj_t* ast3 = lisp_read("(cadr (quote 5 6 7))");
    //print_sexp(eval(env, ast1));
    eval(env, ast1);
    eval(env, ast2);
    print_sexp(eval(env, ast3));
}
