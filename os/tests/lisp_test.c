#include "../lisp/parser.h"
#include "../lisp/types.h"
#include "../lisp/env.h"
#include "../lisp/eval.h"

#include "../kernel/kstd.h"

int main()
{
    env_t* env = make_base_env();
    lobj_t* ast1 = lisp_read("(define x 10)");
    lobj_t* ast2 = lisp_read("(add x x)");
    print_sexp(eval(env, ast1));
    print_sexp(eval(env, ast2));
}
