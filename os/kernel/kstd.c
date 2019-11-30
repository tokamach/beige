#include <stddef.h>
#include <stdint.h>

#include "kstd.h"
#include "kmalloc.h"

/*
 * Number utilities
 */
uint32_t power(uint32_t n, uint32_t exp)
{
    uint32_t result = 1;
    for(uint32_t i = 0; i < exp; i++)
	result *= n;
    return result;
}

/*
 * String utilities
 */
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

int streq(const char* stra, const char* strb)
{
    size_t lena = strlen(stra);
    size_t lenb = strlen(strb);

    if(lena != lenb)
	return 0;

    for(int i = 0; i < lena; i++)
	if(stra[i] != strb[i])
	    return 0;

    return 1;
}

int is_alpha(char c)
{
    size_t cval = (size_t) c;
    //if(c > )
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

// convert string to int
int atoi(char *str)
{
    //TODO: hex check
    int ret = 0;

    for(int i = 0; str[i] != '\0'; ++i)
    {
	ret = str[i] - '0' + ret * 10;
    }

    return ret;
}

int atoi16(char *str)
{
    //TODO: hex check
    int ret = 0;

    for(int i = 0; str[i] != '\0'; ++i)
    {
	char n = str[i];
	if(n >= 'a' && n <= 'f')
	    ret = (str[i] - 'a' + 10) + (ret * 16);
	else if(n >= '0' && n <= '9')
	    ret = (str[i] - '0') + (ret * 16);
    }

    return ret;
}

/*
 * Bitwise utilities
 */
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

