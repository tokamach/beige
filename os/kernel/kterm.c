#include <stddef.h>
#include <stdint.h>

#include "kterm.h"
#include "kstd.h"

static const size_t VGA_HEIGHT = 25;
static const size_t VGA_WIDTH  = 80;

static const size_t VGA_BUF_ADDRESS = 0xB8000;

static const uint8_t VGA_FG = 7;
static const uint8_t VGA_BG = 6;

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
	    vga_buf[(i * VGA_WIDTH) + j] = vga_entry(' ', vga_color(VGA_FG, VGA_BG));
	}
    }
}

void k_term_enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5)  & 0xC0) | cursor_start);

    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x03D5) & 0xE0) | cursor_end);
}

void k_term_disable_cursor()
{
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}

void k_term_move_cursor(uint8_t x, uint8_t y)
{
    uint16_t pos = y * VGA_WIDTH + x;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t) (pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void k_term_update_cursor()
{
    k_term_move_cursor(term_col, term_row);
}

void k_term_put_char(const char c, size_t x, size_t y)
{
    vga_buf[(y * VGA_WIDTH) + x] = vga_entry(c, vga_color(VGA_FG, VGA_BG));
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

    k_term_update_cursor();
}

void k_println(const char * s)
{
    k_print(s);
    k_print("\n");
}

void k_print_num(int i)
{
    char buf[21];
    itoa(i, 10, buf);
    k_print(buf);
}

void k_print_hex(int i)
{
    char buf[21];
    itoa(i, 16, buf);
    k_print("0x");
    k_print(buf);
}

void k_print_bin(int i)
{
    char buf[21];
    itoa(i, 2, buf);
    k_print("0b");
    k_print(buf);
}
