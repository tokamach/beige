#include "parser.h"

#include <stddef.h>
#include "types.h"

#ifdef LISP_TEST
//testin
#include <stdio.h>
#include "../tests/kernel_mappings.h"
#else
//real world
#include "../kernel/kmalloc.h"
#include "../kernel/kterm.h"
#endif

reader_t* make_reader(char* str)
{
    reader_t* r = kmalloc(sizeof(reader_t));
    r->offset = 0;
    r->str = str;
    return r;
}

char reader_cur(reader_t* reader)
{
    return reader->str[reader->offset];
}

char reader_peek(reader_t* reader)
{
    return reader->str[reader->offset + 1];
}

char reader_next(reader_t* reader)
{
    return reader->str[++reader->offset];
}

char* tokenize_one(reader_t* reader)
{
    char* tok = kmalloc(sizeof(char) * (MAX_TOKEN_LEN + 1));
    int offset = 0;

    char c = reader_cur(reader);
    //loop on char != whitespace
    //add char to result str
    while((c != ' '  &&
	   c != '\n' &&
	   c != '('  &&
	   c != ')'))
    {
	tok[offset++] = c;
	c = reader_next(reader);
    }
	   

    tok[offset] = '\0';
    reader->offset--; //TODO: fix workaround for eating chars
    
    //return result in atom
    return tok;
}

cons_t* parse_sexp(reader_t* r)
{
    /*
      loop on c
          c = '(':   recurse
          c = token: tokenize(r), add to sexp->list
	  c = ')': return sexp
    */
    char c;
TOP:
    c = reader_next(r);
    cons_t* ret;
    if(c == '(')
    {		
	return cons(parse_sexp(r), parse_sexp(r));
    }
    else if (c == ')' || c == '\0')
    {
	return NULL;
    }
    else if (c == '.')
    {
	//TODO: cons
    }
    else if(c == ' ')
    {
	goto TOP;
    }
    else// if (c != ' ') //TODO: is_alpha
    {
	//TODO: check if c = atom
	//TODO: check if c = int
	char* str = tokenize_one(r);
	printf("[%s, %c]", str, reader_cur(r));
	ret = atom(str);
	kfree(str);
	
	return cons(ret, parse_sexp(r));
    }
}

cons_t* lisp_read(char* str)
{	
    reader_t* r = make_reader(str);
    r->offset--; //TODO: fix this dumb hack
    return parse_sexp(r); //TODO: fix this car thing
}

void pad_print(int padding, char* str)
{
    for(int i = 0; i < padding; i++)
      k_print(" ");

    k_print(str);
}

void print_cons_iter(cons_t* root, int depth, int debug)
{
    cons_t* elem = root;
    
    if(elem == NULL)
    {
	k_print(" nil");
	return;
    }

    if(debug)
    {
	if(elem->type == Cons)
	{
	    k_print("{");
	    k_print(elem->type ? "List" : "Atom");
	    k_print(":");
	    k_print_hex((size_t)(elem->car));
	    k_print(":");
	    k_print_hex((size_t)(elem->cdr));
	    k_print("}");
	}
	else if(elem->type == Atom)
	{
	    k_print("{");
	    k_print("Atom");
	    k_print(":");
	    k_print_hex((size_t)(elem->val));
	    k_print("} ");
	}
    }

    if(elem->type == Atom)
    {
	//its atom
	/*if(i != 0)
	  k_print(" ");*/

	pad_print(1, elem->val);
    }
    else if(elem->type == Cons)
    {
	k_print("─┐");

	//its list
	k_print("\n");
	pad_print(depth + 1, "├");
	print_cons_iter(elem->car, depth + 2, debug);

	k_print("\n");
	pad_print(depth + 1, "└");
	print_cons_iter(elem->cdr, depth + 2, debug);
    }
}

void print_cons(cons_t* root)
{
    k_print("\n");
    print_cons_iter(root, 0, 0);
}

void print_cons_debug(cons_t* root)
{
    print_cons_iter(root, 0, 1);
}
