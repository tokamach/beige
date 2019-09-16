#pragma once

#include <stddef.h>
#include "types.h"

#define MAX_TOKEN_LEN 128

typedef enum SExpEntryEnum
{
    Val,
    List
} SExpEntryEnum_t;

typedef struct SExpListEntry {
    SExpEntryEnum_t type;
    union {
	struct SExpListEntry* list;
	LVal* val;
    } val;
} SExpListEntry_t;

typedef struct SExp {
    
} SExp_t;

typedef struct LVarList {
    LVar_t var;
    struct LVarList* next;
} LVarList_t;

//LVarList_t* list_next(LVarList_t* list);
size_t list_size(LVarList_t* list);
void list_append(LVarList_t* list, LVar_t* elem);
void list_concat(LVarList_t* lista, LVarList_t* listb);

typedef struct SExp {
    SExpEntry_t* contents;
    struct ASTNode* next;
} SExp_t;

//Stores state of parsing
typedef struct Reader {
    int offset;
    char* str;
} Reader_t;

ASTNode_t* lisp_read(char* str);
