#pragma once

#include <stddef.h>
#include "types.h"

#define MAX_TOKEN_LEN 128

//Stores state of parsing
typedef struct reader {
    int offset;
    char* str;
} reader_t;

sexp_t* lisp_read(char* str);
void print_sexp(sexp_t* sexp);
void print_sexp_debug(sexp_t* sexp);
