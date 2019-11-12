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
#include "../kernel/kstd.h"
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
    
    //return result in sym
    return tok;
}

int valid_dec_num(char* str)
{
    int ret = 1;
    while(*str != '\0' &&
	  ret)
    {
	if(*str < '0' ||
	   *str > '9')
	{
	    ret = 0;
	}
	str++;
    }

    return ret;
}

int valid_sym(char* str)
{
    int ret = 1;
    while(*str != '\0' &&
	  ret)
    {
	if((*str < 'a'  ||
	    *str > 'z') &&
	   (*str != '-' &&
	    *str != '*'))
	{
	    ret = 0;
	}
	str++;
    }

    return ret;
}

cons_t* parse_sym(reader_t* r)
{
    char* str = tokenize_one(r);
    cons_t* newsym = NULL;
	    
    if (valid_sym(str))
    {
	newsym = sym(str);	    
    }
    else if(valid_dec_num(str))
    {
	newsym = num(atoi(str));
    }

    kfree(str);
    
    return newsym;	
}

cons_t* parse_list(reader_t* r)
{
    /*
      c = '(': car = recurse
               cdr = continue loop
      c = tok: cur->car = tokenize(r)
               cur = cur->cdr               
      c = ')': cdr = nil
               return sexp
    */

    char c;
    cons_t* ret = NULL; //maintains pointer to root
    
    while((c = reader_next(r)) &&
	   c != ')')
    {
	if(c == '(')
	{
	    cons_t* newcons = cons(parse_list(r), NULL);
	    if(ret == NULL)
		ret = newcons;
	    else
		append(ret, newcons);
	}
	else if(c == ' ' || c == '\n' || c == ' ') //TODO: is_blank
	{
	    continue;
	}
	else// if (c != ' ') //TODO: is_alpha
	{
	    //TODO: check if c = sym
	    //TODO: check if c = int
	    
	    if(ret == NULL)
		ret = cons(parse_sym(r), NULL);
	    else
		append(ret, cons(parse_sym(r), NULL));
	}
    }

    return ret;
}

cons_t* parse_exp(reader_t* r)
{
    char c = reader_cur(r);    
    if(c == '(')
	return parse_list(r);
    else //TODO: is_alpha
	return parse_sym(r);
}

cons_t* lisp_read(char* str)
{	
    reader_t* r = make_reader(str);
    //r->offset; //TODO: fix this dumb hack
    return parse_exp(r); //TODO: fix this car thing
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
	    k_print("Cons");
	    k_print(":");
	    k_print_hex((size_t)(elem->car));
	    k_print(":");
	    k_print_hex((size_t)(elem->cdr));
	    k_print("}");
	}
	else if(elem->type == Sym)
	{
	    k_print("{");
	    k_print("Sym");
	    k_print(":");
	    k_print_hex((size_t)(elem->val));
	    k_print("} ");
	}
	else if(elem->type == Num)
	{
	    k_print("{");
	    k_print("Num");
	    k_print(":");
	    k_print_hex((size_t)(elem->numl));
	    k_print("} ");
	}
    }

    if(elem->type == Sym)
    {
	//its sym
	/*if(i != 0)
	  k_print(" ");*/

	pad_print(1, elem->val);
    }
    else if(elem->type == Num)
    {
	k_print(" ");
	k_print_num(elem->numl);
    }
    else if(elem->type == Cons)
    {
	k_print("-\\");

	//its list
	k_print("\n");
	pad_print(depth + 1, "|-");
	print_cons_iter(elem->car, depth + 2, debug);

	k_print("\n");
	pad_print(depth + 1, "\\-");
	print_cons_iter(elem->cdr, depth + 2, debug);
    }
}

void print_cons(cons_t* root)
{
    print_cons_iter(root, 0, 0);
    k_print("\n");
}

void print_cons_debug(cons_t* root)
{
    print_cons_iter(root, 0, 1);
    k_print("\n");
}

void print_sexp_iter(cons_t* root, int depth, int debug)
{
    cons_t* elem = root;

    /*
     * each elem->car.type =
     *    sym: print val, space
     *    cons: recur
     */

    if(elem->type == Cons)
	pad_print(depth, "(");
    
    while(elem)
    {
	if(debug && elem)
	{
	    if(elem->type == Cons)
	    {
		k_print("{");
		k_print("Cons");
		k_print(":");
		k_print_hex((size_t)(elem->car));
		k_print(":");
		k_print_hex((size_t)(elem->cdr));
		k_print("}");
	    }
	    else if(elem->type == Sym)
	    {
		k_print("{");
		k_print("Sym");
		k_print(":");
		k_print_hex((size_t)(elem->val));
		k_print("} ");
	    }
	    else if(elem->type == Num)
	    {
		k_print("{");
		k_print("Num");
		k_print(":");
		k_print_hex((size_t)(elem->numl));
		k_print("} ");
	    }

	}

	if(elem == NULL)
	{
	    break;
	}
	else if(elem->type == Sym)
	{
	    k_print(" ");
	    k_print(elem->val);
	    return;
	}
	else if(elem->type == Num)
	{
	    k_print(" ");
	    k_print_num(elem->numl);
	    return;
	}
	else if(elem->type == Cons)
	{
	    if(elem->car->type == Sym)
	    {
		k_print(" ");
		k_print(elem->car->val);
	    }
	    else if(elem->car->type == Num)
	    {
		k_print(" ");
		k_print_num(elem->car->numl);
	    }
	    else if(elem->car->type == Cons)
	    {
		//its list
		k_print("\n");
		print_sexp_iter(elem->car, depth + 2, debug);
	    }
	}
	
	elem = elem->cdr;
    }

    k_print(")");
}

void print_sexp(cons_t* root)
{
    print_sexp_iter(root, 0, 0);
}

void print_sexp_debug(cons_t* root)
{
    print_sexp_iter(root, 0, 1);
}
