#include "../lisp/parser.h"
#include "../lisp/types.h"
#include "../lisp/env.h"
#include "../lisp/eval.h"

#include "../kernel/kstd.h"

int main()
{
    char *fib = "\
(define countdown (lambda (start)\
(if (eq start 1)\
start\
(list start (countdown (sub start 1))))))";
    env_t* env = make_base_env();
    eval(env, lisp_read(fib));
    print_sexp(eval(env, lisp_read("(countdown 10)")));
}
