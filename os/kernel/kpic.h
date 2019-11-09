#pragma once

#include<stdint.h>

#define PIC1_CMD 0x20 //Master Command Port
#define PIC1_DATA 0x21 //Master Data Port

#define PIC2_CMD 0xA0 //Slave Command Port
#define PIC2_DATA 0xA1 //Slave Data Port

#define PIC_EOI 0x20

void k_pic_init();
void k_pic_eoi(uint8_t irq); //send End of Interrupt code
