#include <stddef.h>
#include <stdint.h>

#include "kterm.h"

size_t k_print_mem_info(size_t addr)
{
    //Detect low memory
    size_t low_mem_size;
    asm(
	"clc;"
	"int 0x12;"
	"mov %%eax, %0;"
	: "=r"(low_mem_size));

    k_print();

    //Detect high memory
    size_t mem_size;
    
    asm(
	"push %%eax;"
	"mov $0xe820, %%eax;"
	"int $0x15;"
	""
	: "=r"(mem_size));

    return mem_size;
}

void k_disable_cursor()
{
     asm(
	"pushf     \n"
	"push %eax \n"
	"push %edx \n"

	"mov $0x3D4, %dx \n"
	"mov $0xA, %al   \n"
	"out %al, %dx    \n"

	"inc %dx         \n"
	"mov $0x20, %al  \n"
	"out %al, %dx    \n"

	"pop %edx \n"
	"pop %eax \n"
	"popf     \n"
	"ret      \n"
	);   
}

// kernel entry point
void k_main()
{
    k_term_init();
    k_print("kernel init\n");
    k_print("sugma");
    // k_print_bios_info();
}
