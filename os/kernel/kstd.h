#pragma once

#include <stddef.h>
#include <stdint.h>

size_t strlen(const char *s);
void strcopy(const char *str, char* new);
int itoa(int num, int base, char* str);
size_t align(size_t n);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);
void outb(uint16_t port, uint8_t val);
void outw(uint16_t port, uint16_t val);
