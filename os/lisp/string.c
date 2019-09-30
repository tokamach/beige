#include "string.h"

#ifdef LISP_TEST
//testin
#include <stdio.h>
#include "../tests/kernel_mappings.h"
#else
//real world
#include "../kernel/kmalloc.h"
#include "../kernel/kterm.h"
#endif

string_t* make_string(const char *str)
{
    string_t* s = kmalloc(sizeof(string_t));
    s->str = kmalloc(sizeof(char) * strlen(str));
    strcopy(str, s->str);
    s->len = strlen(str);
    return s;
}

void free_string(string_t* string)
{
    kfree(string->str);
    kfree(string);
}

string_t* string_append(string_t* string, char c)
{
    char* tmp = kmalloc(sizeof(char) * (string->len + 1));
    strcopy(string->str, tmp);
    kfree(string->str);
    string->str = tmp;
    string->str[string->len + 1] = c;
    string->len++;
    return string;
}

string_t* string_concat(string_t* stra, string_t* strb)
{
    int newlen = stra->len + strb->len;
    char* newstr = kmalloc(sizeof(char) * newlen);
    strcopy(stra->str, newstr);
    
    //strcopy algorithm with offset
    for(int i = 0; i < strb->len; i++)
	newstr[stra->len + i] = strb->str[i];

    return make_string(newstr);
}
