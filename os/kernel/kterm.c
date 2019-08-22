#include <stddef.h>
#include <stdint.h>

#include "kterm.h"
#include "kstd.h"

static const size_t VGA_HEIGHT = 25;
static const size_t VGA_WIDTH  = 80;

static const size_t VGA_BUF_ADDRESS = 0xB8000;

//bg color, white on black TODO: add colour support
//static const uint8_t color = 15 | 0 << 4;

size_t term_col;
size_t term_row;

uint16_t *vga_buf;

uint8_t vga_color(uint8_t fg, uint8_t bg)
{
    return (uint8_t) fg | bg << 4;
}

uint16_t vga_entry(unsigned char c, uint8_t color)
{
    return (uint16_t) c | (uint16_t) color << 8;
}

void k_term_init()
{
    term_col = 0;
    term_row = 0;
    vga_buf = (uint16_t*)VGA_BUF_ADDRESS;
    
    for(size_t i = 0; i < VGA_HEIGHT; i++)
    {
	for(size_t j = 0; j < VGA_WIDTH; j++)
	{
	    vga_buf[(i * VGA_WIDTH) + j] = vga_entry(' ', vga_color(0, 0));
	}
    }
}

void k_term_put_char(const char c, size_t x, size_t y)
{
    vga_buf[(y * VGA_WIDTH) + x] = vga_entry(c, vga_color(15, 0));
}

void k_term_print_char(const char c)
{
    //TODO: implement additional control characters
    if(c == '\n')
    {
	term_row++;
	term_col = 0;
    }
    else
    {
	//printing visible character
	k_term_put_char(c, term_col, term_row);
	term_col++;
    }
    
    if(term_col >= VGA_WIDTH)
    {
	term_row++;
	term_col = 0;
    }

    if(term_row >= VGA_HEIGHT)
    {
	term_row = 0;
	term_col = 0;
    }
}

void k_print(const char * s)
{
    for(size_t i = 0; i < strlen(s); i++)
    {
	k_term_print_char(s[i]);
    }
}

void k_print_num(int i)
{
    char buf[21];
    itoa(i, 10, buf);
    k_print(buf);
}

