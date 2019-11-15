#include "keyboard.h"

uint8_t read_scan_code()
{
    return inb(KBD_PORT);
}
