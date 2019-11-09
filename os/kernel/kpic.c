#include "kpic.h"

#include <stdint.h>
#include "kstd.h"

void k_pic_init()
{
    /*
     * The Kernel starts in protected mode. We need to init the 
     * PICs. 
     */

    //first we'll save the interrupt masks currently in the PICs
    uint8_t mask_a = inb(PIC1_DATA);
    uint8_t mask_b = inb(PIC2_DATA);

    //ICW1: Initialize the PIC
    outb(PIC1_CMD, 0x11);
    outb(PIC2_CMD, 0x11);

    //ICW2
    outb(PIC1_DATA, 0x20); //remap IRQs 0-7  to IDT indexes 32-39
    outb(PIC2_DATA, 0x28); //rmeap IRQs 8-15 to IDT indexes 40-47

    //ICW3
    outb(PIC1_DATA, 0x04); //PIC1 connected to PIC2 via IRQ2
    outb(PIC2_DATA, 0x02); //PIC2 connected to PIC1 via IRQ1

    //ICW4
    outb(PIC1_DATA, 0x01); //enable 8086/88 mode, PIC1 is master
    outb(PIC2_DATA, 0x01); //enable 8086/88 mode

    //replace masks
    outb(PIC1_DATA, mask_a);
    outb(PIC2_DATA, mask_b);

    k_println("pic initialized");
}

void k_pic_eoi(uint8_t irq)
{
    if(irq >= 8)
	outb(PIC2_CMD, PIC_EOI);
    
    outb(PIC1_CMD, PIC_EOI);
}
