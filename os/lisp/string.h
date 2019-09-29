#pragma once

//more advanced strings in c

typedef struct string {
    char* str;
    int   len;
} string_t;

string_t* make_string(const char *str);
string_t* string_append(string_t* string, char c);
string_t* string_copy(string_t* stra, string_t* strb);
string_t* string_concat(string_t* stra, string_t* strb);


