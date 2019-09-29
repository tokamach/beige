#include "parser.h"

#include <stddef.h>
#include "types.h"
#include "string.h"
#include "list.h"

//testin
//#include <stdlib.h>

//real world
#include "../kernel/kmalloc.h"
#include "../kernel/kterm.h"

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
    char* tok = kmalloc(sizeof(char) * MAX_TOKEN_LEN);
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
    //return result atom
    k_print(tok);
    reader->offset--; //TODO: rewrite this whole function to avoid this
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
	    SExpElem_t* elem = make_sexp_elem(List, parse_sexp(r));
	    k_print_num(((SExp_t*)elem->val)->list.size);
	    sexp_add_elem(sexp, elem);
	}
	else if (c == ')')
	{
	    return sexp;
	}
	//TODO: is_alpha
	else if (c != ' ')
	{
	    //TODO: check if c = valid token	    
	    sexp_add_elem(sexp, make_sexp_elem(Atom, tokenize_one(r)));

	    k_print("[");
	    k_term_print_char(c);
	    k_print(":");
	    k_print_num(r->offset);
	    k_println("]");
	}
	else
	{
	    //idk lol
	}
    } while((c = reader_next(r)));
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

void print_sexp_iter(SExp_t* root, int depth)
{
    //pad_print(depth, "\n");
    
    pad_print(depth, "(");
    k_print("[");
    k_print_num(root->list.size);
    k_print("]");
    
    for(int i = 0; i < root->list.size; i++)
    {
	//this is fucking disgusting
	//TODO: CLEAN THIS SHIT UP
	
	//TODO: THIS IS WRONG
	SExpElem_t* elem = sexp_elem_at(root, i);
	//k_print_num(elem->type);
	//k_println("");
	if(elem->type == Atom)
	{
	    //its atom
	    pad_print(depth, ((Atom_t*)elem->val)->val.str);
	}
	else if(elem->type == List)
	{
	    //its list
	    print_sexp_iter(elem->val, depth + 1);
	}
    }

    pad_print(depth, ")");
}

void print_sexp(SExp_t* root)
{
    print_sexp_iter(root, 0);
}
