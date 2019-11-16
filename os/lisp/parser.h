#pragma once

#include <stddef.h>
#include "types.h"

#define MAX_TOKEN_LEN 128

//Stores state of parsing
typedef struct reader {
    int offset;
    char* str;
} reader_t;

lobj_t* lisp_read(char* str);
void print_cons(lobj_t* root);
void print_cons_debug(lobj_t* root);

void print_sexp(lobj_t* root);
void print_sexp_debug(lobj_t* root);
