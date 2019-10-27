#pragma once

#include <stddef.h>
#include "types.h"

#define MAX_TOKEN_LEN 128

//Stores state of parsing
typedef struct reader {
    int offset;
    char* str;
} reader_t;

cons_t* lisp_read(char* str);
void print_cons(cons_t* root);
void print_cons_debug(cons_t* root);

void print_list(cons_t* root);
void print_list_debug(cons_t* root);
