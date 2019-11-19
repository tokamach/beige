#include "../lisp/parser.h"
#include "../lisp/types.h"
#include "../lisp/env.h"
#include "../lisp/eval.h"

#include "../kernel/kstd.h"

int main()
{
    env_t* env = make_base_env();
    lobj_t* ast1 = lisp_read("(car (cdr (quote 2 3 4)))");
    print_sexp(eval(env, ast1));
}
