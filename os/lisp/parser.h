#pragma once

#include <stddef.h>
#include "types.h"

#define MAX_TOKEN_LEN 128

//Stores state of parsing
typedef struct Reader {
    int offset;
    char* str;
} Reader_t;

LCons_t* parse_sexp_to_cons(char* str);
SExp_t* lisp_read(char* str);
void print_sexp(SExp_t* sexp);
