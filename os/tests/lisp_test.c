#include "../lisp/parser.h"
#include "../lisp/types.h"
#include "../lisp/env.h"
#include "../lisp/eval.h"

#include "../kernel/kstd.h"

int main()
{
    char* code = "(add 1 1)";
    cons_t* ast = lisp_read(code);
    print_sexp(ast);
    printf("\n");
    
    env_t* core_env = make_core_env();
    print_sexp(eval(core_env, ast));
}
