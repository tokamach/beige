#include "parser.h"

#include <stddef.h>
#include "types.h"

//testin
//#include <stdlib.h>

//real world
#include "../kernel/kmalloc.h"
#include "../kernel/kterm.h"

size_t list_size(LVarList_t* list)
{
    size_t n = 0;
    while((list = list->next))
	n++;

    return n;
}

void list_append(LVarList_t* list, LVar_t* elem)
{
    //seek to end
    while((list = list->next)) {}

    LVarList_t* new = kmalloc(sizeof(LVarList_t));
    new->var = elem;
    new->next = NULL;
    list->next = new;
}

void list_concat(LVarList_t* lista, LVarList_t* listb)
{
    //seek to end
    while((lista = lista->next)) {}

    lista->next = listb;
}

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

LVar_t* tokenize_one(Reader_t* reader)
{
    LVar_t* res = kmalloc(sizeof(LVar_t));
    res->type = Atom;
    
    char* tok = kmalloc(sizeof(char) * MAX_TOKEN_LEN);
    int offset = 0;

    char c;
    //loop on char != whitespace
    //add char to result str
    do
    {
	tok[offset++] = c;
    }
    while((c = reader_next(reader)) && (c != ' ' || c != '\n'));
    
    //return result str
    res->data.atom = tok;

    return res;
}

ASTNode_t* parse_sexp(Reader_t* r)
{
    ASTNode_t* ret = kmalloc(sizeof(ASTNode_t));
    
    char c;
    while((c = reader_next(r)) && c != ')')
    {
	if(c == '(')
	{
	    //opening or closing level of s-exp
	    //if ) end ASTNode
	    //if ( create new ASTNode
	    ret->next = parse_sexp(r);
	}
	else if(c == ' ' || c == '\n')
	{
	    //c is whitespace, skip
	}
	else
	{
	    //c is alphanumeric
	    
	    //tokenize
	    list_append(&ret->contents, tokenize_one(r));
	}
    }
    return ret;
}

ASTNode_t* lisp_read(char* str)
{
    Reader_t* r = get_reader(str);

    ASTNode_t* root;

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

void print_ast(ASTNode_t* ast)
{
    int indent = 0;
    do
    {
	k_println("(");
	LVarList_t l = ast->contents;
	
	for(int i = 0; i < list_size(&l); i++)
	{
	    for(int n = 0; n < indent; n++)
		k_print(" ");

	    k_print(l.var.data.atom);
	    l = l.next;
	}
	indent++;
    } while((ast = ast->next));
}
