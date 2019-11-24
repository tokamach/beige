#pragma once

#include <stddef.h>
#include <stdint.h>

void k_term_clear();
void k_term_init();

/*
 * Cursor Functions
 */
void k_term_enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void k_term_disable_cursor();
void k_term_move_cursor(uint8_t x, uint8_t y);
void k_term_update_cursor();

/*
 * Printing individual chars, should be internal only
 */
void k_term_put_char(const char c, size_t x, size_t y);
void k_term_print_char(const char c);

/*
 * Statline functions
 */
void k_term_update_statline();

/*
 * Kernel wide printing functions
 */
void k_print(const char * s);
void k_println(const char * s);
void k_print_num(int i);
void k_print_hex(int i);
void k_print_bin(int i);
