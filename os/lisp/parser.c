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

Reader_t* make_reader(char* str)
{
    Reader_t* r = kmalloc(sizeof(Reader_t));
    r->offset = 0;
    r->str = str;
    return r;
}

char reader_cur(Reader_t* reader)
{
    return reader->str[reader->offset];
}

char reader_peek(Reader_t* reader)
{
    return reader->str[reader->offset + 1];
}

char reader_next(Reader_t* reader)
{
    return reader->str[++reader->offset];
}

Atom_t* tokenize_one(Reader_t* reader)
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
    
    //return result in atom
    return make_atom(tok);
}

SExp_t* parse_sexp(Reader_t* r)
{
    SExp_t* sexp = make_sexp();
    /*
      loop on c
          c = '(':   recurse
          c = token: tokenize(r), add to sexp->list
	  c = ')': return sexp
    */
    //TODO: stop assuming we have a (
    char c = reader_next(r);
    do
    {
	if(c == '(')
	{
	    //todo tail recurse (pointless)
	    SExp_t* child = parse_sexp(r);
	    SExpElem_t* elem = make_sexp_elem_list(child);
	    sexp_add_elem(sexp, elem);
	    c = reader_next(r);
	}
	//TODO: is_alpha
	else if (c != ' ')
	{
	    //TODO: check if c = valid token
	    Atom_t* a = tokenize_one(r);
	    sexp_add_elem(sexp, make_sexp_elem_atom(a));
	    c = reader_cur(r);
	}
	else
	{
	    //idk lol
	}
    } while(c != ')' && (c = reader_next(r)));
    
    return sexp;
}

SExp_t* lisp_read(char* str)
{
    Reader_t* r = make_reader(str);
    SExp_t* root;

    root = parse_sexp(r);
    return root;
}

void pad_print(int padding, char* str)
{
    for(int i = 0; i < padding; i++)
      k_print(" ");

    k_print(str);
}

void print_sexp_iter(SExp_t* root, int depth, int debug)
{
    pad_print(depth, "\n");
    pad_print(depth, "(");

    if(debug)
    {
	k_print("[");
	k_print_num(root->size);
	k_print("]");
    }
    
    for(int i = 0; i < root->size; i++)
    {
	SExpElem_t* elem = sexp_elem_at(root, i);
	
	if(debug)
	{
	    k_print("{");
	    k_print_num(elem->type);
	    k_print(":");
	    k_print(elem->type ? "List" : "Atom");
	    k_print(":");
	    k_print_hex(elem->type ? (size_t)(elem->val.list) : (size_t)(elem->val.atom));
	    k_print("}");
	}
	
	if(elem->type == Atom)
	{
	    //its atom
	    if(i != 0)
		k_print(" ");

	    k_print(elem->val.atom->str);
	}
	else if(elem->type == List)
	{
	    //its list
	    print_sexp_iter(elem->val.list, depth + 2, debug);
	}
    }

    k_print(")");
}

void print_sexp(SExp_t* root)
{
    print_sexp_iter(root, 0, 0);
}

void print_sexp_debug(SExp_t* root)
{
    print_sexp_iter(root, 0, 1);
}
