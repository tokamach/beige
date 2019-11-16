#include "../lisp/parser.h"
#include "../lisp/types.h"
#include "../lisp/env.h"
#include "../lisp/eval.h"

#include "../kernel/kstd.h"

int main()
{
    env_t* kenv = make_kernel_env();
    lobj_t* ast = lisp_read("(add 2 2)");
    print_cons(ast);
    
    //env_t* base_env = make_base_env();
    //print_sexp(eval(base_env, ast));
}
