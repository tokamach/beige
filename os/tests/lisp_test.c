#include "../lisp/parser.h"
#include "../lisp/types.h"
#include "../lisp/env.h"
#include "../lisp/base_env.h"
#include "../lisp/eval.h"

int main()
{
    env_t* env = make_base_env();
    print_sexp(eval(env, lisp_read("(+ 1 10)")));
}
