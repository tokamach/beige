#include "../lisp/parser.h"
#include "../lisp/types.h"

int main()
{
    char* code = "\
(defun factorial (x)\
  (if (zerop x)\
    1\
    (* x (factorial (- x 1)))))";
    
    cons_t* tree = lisp_read("((a b) (c d))");
    /*cons_t* tree = cons(atom("a"),
			cons(atom("b"),
			     cons(atom("see"),
			     NULL)));*/
    print_cons_debug(tree);
    printf("\n");
}
