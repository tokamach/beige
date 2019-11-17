#pragma once

#include "types.h"

#define ENV_SIZE 256
#define MAX_SYMBOLS 256

typedef struct {
    uint8_t id;
    char* symbol;
} symbol_t;

typedef uint16_t symbol_id;

symbol_t symbol_table[MAX_SYMBOLS];
uint16_t symbol_count;

symbol_id add_symbol(char* symbol);
symbol_id lookup_symbol(char* symbol);
char* lookup_id(symbol_id i);

typedef enum e_env_entry_type {
    empty,
    nativef,  //native function (function pointer (env_t*, lobj_t*))
    nativef1,
    nativef2,
    nativef3,
    lobj
    /*
    lispf,    //lisp function (pointer to lisp sexp)
    syml,     //sym num
    numl      //numeric num
    */
} env_entry_type;

typedef struct env env_t;
typedef struct env_entry {
    env_entry_type type;
    symbol_id sym;
    union {
	lobj_t* (*nativef)(env_t* env, lobj_t* args);
	lobj_t* (*nativef1)(env_t* env, lobj_t* args);
	lobj_t* (*nativef2)(env_t* env, lobj_t* arg1, lobj_t* arg2);
	lobj_t* (*nativef3)(env_t* env,
			    lobj_t* arg1,
			    lobj_t* arg2,
			    lobj_t* arg3);
	lobj_t* lobj;
	/*symbol_id syml;
	  size_t  numl;*/
    };
} env_entry_t;

typedef struct env {
    //TODO:
    //assoc map (hashmap etc) of env_entry_t
    struct env* outer;
    env_entry_t entries[ENV_SIZE];
    uint8_t entry_count;
} env_t;

void add_env_entry_lobj(env_t* env, symbol_id sym, lobj_t* val);
void add_env_entry_native(env_t* env, env_entry_type type, symbol_id sym, void* fun);
env_entry_t* get_env_entry(env_t* env, char* sym);

env_t* make_env(env_t* outer);
void free_env(env_t* env);

env_t* make_base_env();
env_t* make_kernel_env();
