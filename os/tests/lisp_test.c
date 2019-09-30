#include "../lisp/parser.h"
#include "../lisp/types.h"

int main()
{
    char* code = "\
(defun factorial (x)\
  (if (zerop x)\
    1\
    (* x (factorial (- x 1)))))";
    
    SExp_t* tree = lisp_read(code);
    print_sexp(tree);

}
