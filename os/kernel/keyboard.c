#include "keyboard.h"

#include "kterm.h"

uint8_t read_scan_code()
{
    return inb(KBD_PORT);
}

static void keyboard_handler()
{
    k_print("KEY HIT");
    asm("hlt");
}

void k_keyboard_init()
{
    k_register_interrupt_handler(33, keyboard_handler);
    k_println("Keyboard driver initialized");
}
