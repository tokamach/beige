#include "../lisp/parser.h"
#include "../lisp/types.h"

#include "../kernel/kstd.h"

int main()
{
    char* code = "(defun f (x) (* x 20)";
    
    cons_t* tree = lisp_read(code);
    print_sexp(tree);
}
