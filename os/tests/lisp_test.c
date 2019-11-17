#include "../lisp/parser.h"
#include "../lisp/types.h"
#include "../lisp/env.h"
#include "../lisp/eval.h"

#include "../kernel/kstd.h"

int main()
{
    env_t* env = make_base_env();
    lobj_t* ast = lisp_read("(add 2 (add 1 2))");    
    print_sexp(eval(env, ast));
}
