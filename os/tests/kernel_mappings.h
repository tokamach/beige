#include <stdio.h>
#include <stdlib.h>

#define kmalloc(n) malloc((n)
#define kfree(n)   free((n))

#define k_print(string) printf("%s", (string))
#define k_println(string) printf("%s\n", (string))
#define k_print_num(integer) printf("%i", (integer))
#define k_print_hex(hex_int) printf("%x", (hex_int))
