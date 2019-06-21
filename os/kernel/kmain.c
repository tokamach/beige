#include <stddef.h>
#include <stdint.h>

#include "kterm.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

// kernel entry point
void kernel_main()
{
    k_term_init();
    k_print("Hello World!");
}
