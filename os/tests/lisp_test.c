#include "../lisp/parser.h"
#include "../lisp/types.h"

int main()
{
    SExp_t* tree = lisp_read("(defun f (x y) (+ x y))");
    k_println("");
    print_sexp(tree);
}
