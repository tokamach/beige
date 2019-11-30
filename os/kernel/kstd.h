#pragma once

#include <stddef.h>
#include <stdint.h>

//Number
uint32_t power(uint32_t n, uint32_t exp);

//String
size_t strlen(const char *s);
void strcopy(const char *str, char* new);
int streq(const char *stra, const char* strb);
int itoa(int num, int base, char* str);
int atoi(char* str);
int atoi16(char* str);

//Bitwise
size_t align(size_t n);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);
void outb(uint16_t port, uint8_t val);
void outw(uint16_t port, uint16_t val);
