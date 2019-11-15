#include "../lisp/parser.h"
#include "../lisp/types.h"
#include "../lisp/env.h"
#include "../lisp/eval.h"

#include "../kernel/kstd.h"

int main()
{
    char* code = "(add 1 2)";
    cons_t* ast = lisp_read(code);
    print_sexp(ast);
    printf("\n");
    
    env_t* base_env = make_base_env();
    print_sexp(eval(base_env, ast));
}
