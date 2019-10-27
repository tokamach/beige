#include "../lisp/parser.h"
#include "../lisp/types.h"

int main()
{
    char* code = "\
(defun factorial (x)\
  (if (zerop x)\
    1\
    (* x (factorial (- x 1)))))";
    
    cons_t* tree = lisp_read(code);

    cons_t* treeb = cons(cons(atom("a"),
			      cons(atom("b"), NULL)),
			 cons(cons(atom("c"),
				   cons(atom("d"), NULL)),
			      cons(cons(atom("e"),
					cons(atom("f"), NULL)),
				   NULL)));
    print_cons_debug(tree);
    printf("\n");
}
