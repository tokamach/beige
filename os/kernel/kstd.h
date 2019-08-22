#pragma once

#include <stddef.h>

size_t strlen(const char *s)
{
    size_t size = 0;
    while(s[size])
	size++;
    return size;
}

// convert int to string
int itoa(int num, int base, char* str)
{
    //64-bit integer is max 20 digits long in str
    int digits = 0;
    char ret[21];
    
    //modulo ten, add result to front of string, div by 10, loop again
    do {
    	ret[digits] = (num % base) + '0';
    	num = num / 10;
    	digits++;
    } while (num > 0);

    for(int i = 0; i < digits; i++)
    	str[i] = ret[digits - i - 1];
    
    str[digits + 1] = '\0';
    
    return 0;
}
