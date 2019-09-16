#pragma once

#include <stdint.h>

typedef struct LVar LVar_t;

typedef enum LType {
    Nil,
    Cons,
    List,
    Atom,
    T,
    u8,
    u16,
    u32,
    Num
} LType_t;

typedef struct LConsCell {
    LVar_t* car;
    LVar_t* cdr;
} LConsCell_t;

typedef struct LVar {
    LType_t type;

    union {
	//Nil
	LConsCell_t cons;
	//list:     uses cons internally
	char*       atom;
	uint8_t       u8;
	uint16_t     u16;
	uint32_t     u32;
	uint32_t     num;
    } data;
} LVar_t;
