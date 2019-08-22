#pragma once

#include <stddef.h>

void k_term_init();
void k_term_put_char(const char c, size_t x, size_t y);
void k_term_print_char(const char c);
void k_print(const char * s);
void k_print_num(int i);
