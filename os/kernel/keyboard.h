#pragma once

#include "kstd.h"

#define KBD_PORT 0x60

uint8_t read_scan_code();
void k_keyboard_init();

