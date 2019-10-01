#pragma once

#include <stddef.h>
#include "types.h"

#define MAX_TOKEN_LEN 128

//Stores state of parsing
typedef struct Reader {
    int offset;
    char* str;
} Reader_t;

SExp_t* lisp_read(char* str);
void print_sexp(SExp_t* sexp);
void print_sexp_debug(SExp_t* sexp);
