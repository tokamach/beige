#pragma once

#include "types.h"

#define ENV_SIZE 256
#define MAX_SYMBOLS 256

typedef struct {
    uint8_t id;
    char* symbol;
} symbol_t;
    
symbol_t symbol_table[MAX_SYMBOLS];
uint8_t symbol_count;

uint8_t add_symbol(char* symbol);
uint8_t lookup_symbol(char* symbol);
char* lookup_id(uint8_t i);

typedef enum e_env_entry_type {
    empty,
    nativef,  //native function (function pointer (env_t*, cons_t*))
    lispf,    //lisp function (pointer to lisp sexp)
    syml,     //sym num
    numl      //numeric num
} env_entry_type;

typedef struct env env_t;
typedef struct env_entry {
    env_entry_type type;
    uint8_t sym;
    union {
	//TODO: macroize this
	cons_t* (*nativef)(env_t* env, cons_t* args);
	cons_t* lispf;
	cons_t* symbol;
	size_t  numl;
    };
} env_entry_t;

typedef struct env {
    //TODO:
    //assoc map (hashmap etc) of env_entry_t
    struct env* outer;
    env_entry_t entries[ENV_SIZE];
    uint8_t entry_count;
} env_t;

void add_env_entry_cons(env_t* env, env_entry_type type, char *sym, cons_t* val);
void add_env_entry_native(env_t* env, char *sym, void* fun);
env_entry_t* get_env_entry(env_t* env, char* sym);

env_t* make_env(env_t* outer);
env_t* make_base_env();
env_t* make_kernel_env();
