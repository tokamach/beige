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

Reader_t* get_reader(char* str)
{
    Reader_t* r = kmalloc(sizeof(Reader_t));
    r->offset = 0;
    r->str = str;
    return r;
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

    char c = reader_next(reader);
    //loop on char != whitespace
    //add char to result str
    do
    {
	tok[offset++] = c;
    }
    while((c = reader_next(reader)) && (c != ' ' || c != '\n'));
    
    //return result atom
    return make_atom(tok);
}

/*
LCons_t* parse_sexp_to_cons(char* str)
{

    Reader_t* r = get_reader(str);
    char c = *r->str;
	
    if(c != '(')
    {
	//error
	while(1) {} //hang
    }

    /*
      loop
      create cons
          car:
              if token, car = token
	      if (, car = loop on new cons
	  cdr:
              if ), cdr = nil
	      if token, cdr = loop on new cons
    */
/*
    LCons_t* cons = kmalloc(sizeof(LCons_t));

    while((reader_next(r)))
    {
	//car
	if(*r->str == '(')
	{
	    cons->car->type = LType_t.Cons;
	    
	}
	else if(*r->str == ')')
	{
	    
	}
	else //if(is_alpha(r->str))
	{
	    
	}

	//cdr
    }

    return cons;
}
*/

SExp_t* parse_sexp(Reader_t* r)
{
    SExp_t* sexp = make_sexp();
    /*
      loop on c
          c = '(':   recurse
          c = token: tokenize(r), add to sexp->list
	  c = ')': return sexp
    */

    char c;
    while((c = reader_next(r)))
    {
	if(c == '(')
	{
	    //todo tail recurse (pointless)
	    SExpElem_t* elem = make_sexp_elem(List, parse_sexp(r));
	    sexp_add_elem(sexp, elem);
	}
	else if (c == ')')
	{
	    return sexp;
	}
	else
	{
	    //TODO: check if c = valid token
	    sexp_add_elem(sexp, make_sexp_elem(Atom, tokenize_one(r)));
	}
    }
}

SExp_t* lisp_read(char* str)
{
    Reader_t* r = get_reader(str);

    SExp_t* root;

    char c = *r->str;
    if(c == '(')
    {
	root = parse_sexp(r);
    }
    else
    {
	//error sexp doesn't start with (
	while(1) {} //TODO: add error
    }

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
    pad_print(depth, "\n");
    
    for(int i = 0; i < root->list.size; i++)
    {
	//this is fucking disgusting
	//TODO: CLEAN THIS SHIT UP
	SExpElem_t* elem = ((SExpElem_t*)list_elem_at(&root->list, i));
	if(elem->type == Atom)
	{
	    //its atom
	    pad_print(depth, ((Atom_t*)elem->val)->val.str);
	    k_print("\n");
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
