#include <stddef.h>
#include <stdint.h>

#include "kstd.h"
#include "kmalloc.h"

size_t strlen(const char *s)
{
    size_t size = 0;
    while(s[size])
	size++;
    return size;
}

//assumes new is allocated and fits str
void strcopy(const char* str, char* new)
{
    size_t oldlen = strlen(str);
    for(size_t i = 0; i < oldlen; i++)
	new[i] = str[i];
}

// convert int to string
int itoa(int num, int base, char* str)
{
    //64-bit integer is max 20 digits long in str
    int digits = 0;
    char ret[21];
    
    //modulo base, add result to front of string, div by base, loop again
    do {
	
	int n = num % base;
	if(n > 9)
	{
	    n -= 10;
	    ret[digits] = n + 'a';   
	}
	else if (n <= 9)
	    ret[digits] = n + '0';
	
    	num = num / base;
    	digits++;
    } while (num > 0);

    for(int i = 0; i < digits; i++)
    	str[i] = ret[digits - i - 1];
    
    str[digits] = '\0';
    
    return 0;
}

size_t align(size_t n)
{
    return (n + sizeof(size_t) - 1) & ~(sizeof(size_t) - 1);
}

uint8_t inb(uint16_t port)
{
    uint8_t inval; 
    asm(
	"inb %1, %0;"
	: "=a" (inval)
	: "Nd" (port)
	);
    return inval;
}

uint16_t inw(uint16_t port)
{
    uint16_t inval;
    asm(
	"inw %1, %0;"
	: "=a" (inval)
	: "Nd" (port)
	);
    return inval;
}

void outb(uint16_t port, uint8_t val)
{
    asm(
	"outb %b0, %w1;"
	:: "a"(val), "Nd"(port)
	);
}

void outw(uint16_t port, uint16_t val)
{
    asm(
	"outw %w0, %w1;"
	:: "a"(val), "Nd"(port)
	);
}

