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
    nativef1, //native function 1 arg
    nativef2, //native function 2 args
    nativef3,
    nativef4,
    nativef5,
    lispf,    //lisp function (pointer to lisp sexp)
    atoml     //atom literal
} env_entry_type;

typedef struct env_entry {
    env_entry_type type;
    uint8_t sym;
    union {
	//TODO: macroize this
	cons_t* (*nativef1)(const void* arg1);
	cons_t* (*nativef2)(const void* arg1, const void* arg2);
	cons_t* (*nativef3)(const void* arg1, const void* arg2, const void* arg3);
	cons_t* (*nativef4)(const void* arg1, const void* arg2, const void* arg3,
			    const void* arg4);
	cons_t* (*nativef5)(const void* arg1, const void* arg2, const void* arg3,
			    const void* arg4, const void* arg5);
	cons_t* lispf;
	cons_t* atom;
    };
} env_entry_t;

typedef struct env {
    //TODO:
    //assoc map (hashmap etc) of env_entry_t

    //layering
    //pointer to parent
    //pointer to child

    env_entry_t entries[ENV_SIZE];
    uint8_t entry_count;
} env_t;

void add_env_entry_cons(env_t* env, env_entry_type type, char *sym, cons_t* val);
void add_env_entry_native(env_t* env, env_entry_type type, char *sym, void* fun);

env_entry_t* get_env_entry(env_t* env, char* sym);

/*
 * Core env
 */
env_t* make_core_env();
