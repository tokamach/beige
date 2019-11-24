#include "kinterrupt.h"

#include <stdint.h>

#include "kmalloc.h"
#include "kterm.h"
#include "kpic.h"
#include "kgdt.h"

void k_add_interrupt(uint8_t  entry,
		     uint32_t offset,
		     uint8_t type,
		     uint8_t  dpl,
		     uint16_t size)
{
    uint8_t flags = 0;
    flags |= (1 << 7); //is present, set to 0 for unused interrupt
    flags |= ((dpl & 0b11) << 5); //DPL, descriptor privelege level (0-3)
    flags |= (size << 4); //size (set to 0 for interrupt and trap gates)
    flags |= type; //gate type
    
    idt.entries[entry].offset_low       = (uint16_t)offset;
    idt.entries[entry].segment_selector = 0x08; //TODO: chuck in a constant
    idt.entries[entry].zero             = 0;
    idt.entries[entry].flags            = flags;
    idt.entries[entry].offset_high      = (offset >> 16);
}

void k_interrupt_init()
{
    idt_ptr_t* idt_ptr = &idt.pointer;
    idt_ptr->limit = IDT_ENTRIES - 1;
    idt_ptr->base  = (uint32_t)&idt.entries[0];

    for(uint8_t i = 0; i < IDT_ENTRIES - 1; i++)
	idt.entries[i] = (idt_entry_t){0, 0, 0, 0, 0};

    k_add_interrupt(31, (uintptr_t)&interrupt_handler, 0, 0, 0);
    
    //load IDT
    load_idt(idt_ptr);
    
    enable_interrupts();
    
    k_println("interrupts enabled");
}

 
__attribute__((interrupt))
void interrupt_handler(interrupt_frame_t* frame)
{
    k_print_hex(frame->error_code);
    //call exception or software dependent on error
    k_pic_eoi(frame->error_code);
}

//TODO: write exception handler (error code in args)
//TODO: write software interrupt (syscall) handler
