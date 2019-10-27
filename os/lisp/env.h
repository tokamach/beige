#pragma once

#include "types.h"

#define ENV_SIZE 256

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
	cons_t atoml;
    } val;
} env_entry_t;

typedef struct env {
    //TODO:
    //assoc map (hashmap etc) of env_entry_t

    //layering
    //pointer to parent
    //pointer to child

    env_entry_t entries[ENV_SIZE];
} env_t;

env_entry_t* make_env_entry();
env_entry_t* env_entry();
